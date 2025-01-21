#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "LedMatrix/neopixel.c"
#include "KissFFT/kissfft/kiss_fft.h"

#define MIC_CHANNEL 2
#define MIC_PIN (26 + MIC_CHANNEL)
#define ADC_CLOCK_DIV 96.f
#define SAMPLES 2048  // Increased number for better FFT precision
#define ADC_ADJUST(x) (x * 3.3f / (1 << 12u) - 1.65f)
#define LED_PIN 7
#define LED_COUNT 25
#define TOLERANCE 2.0f  // Tolerance in Hz for tuning

// Guitar string frequencies
const float string_frequencies[] = {82.4, 110.0, 146.0, 195.0, 246.0, 329.0};
const char *string_names[] = {"E1", "A1", "D2", "G2", "B3", "E3"};

uint dma_channel;

// Function to capture ADC data
void capture_adc_data(float *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buffer[i] = ADC_ADJUST(adc_read());
        if (i < 10) {  // Print the first 10 samples for debugging
            printf("ADC sample %d: %f\n", i, buffer[i]);
        }
    }
}

// Function to perform FFT and find the dominant frequency
float find_dominant_frequency(float *buffer, size_t size) {
    kiss_fft_cfg cfg = kiss_fft_alloc(size, 0, NULL, NULL);
    kiss_fft_cpx in[size], out[size];

    for (size_t i = 0; i < size; i++) {
        in[i].r = buffer[i];
        in[i].i = 0;
    }

    kiss_fft(cfg, in, out);
    free(cfg);

    float max_magnitude = 0;
    size_t max_index = 0;
    for (size_t i = 0; i < size / 2; i++) {
        float magnitude = sqrt(out[i].r * out[i].r + out[i].i * out[i].i);
        if (magnitude > max_magnitude) {
            max_magnitude = magnitude;
            max_index = i;
        }
    }

    float dominant_frequency = (float)max_index * (125000 / ADC_CLOCK_DIV) / size;
    return dominant_frequency;
}

// Function to initialize the LED matrix
void init_led_matrix() {
    npInit(LED_PIN, LED_COUNT);
    npClear();
    npWrite();
}

// Function to set LED color based on tuning status
void set_led_color(bool in_tune) {
    uint8_t r = in_tune ? 0 : 255;
    uint8_t g = in_tune ? 255 : 0;
    uint8_t b = 0;
    for (int i = 0; i < LED_COUNT; i++) {
        npSetLED(i, r, g, b);
    }
    npWrite();
    printf("LEDs set to %s\n", in_tune ? "green" : "red");  // Debug print
}

// Main function
int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(MIC_PIN);
    adc_select_input(MIC_CHANNEL);

    init_led_matrix();  // Initialize the LED matrix

    float buffer[SAMPLES];

    while (1) {
        capture_adc_data(buffer, SAMPLES);
        printf("Captured ADC data\n");  // Debug print

        float frequency = find_dominant_frequency(buffer, SAMPLES);
        printf("Dominant frequency: %f Hz\n", frequency);  // Debug print

        // Check tuning
        int in_tune = 0;
        for (size_t i = 0; i < sizeof(string_frequencies) / sizeof(string_frequencies[0]); i++) {
            printf("Checking frequency %f against string %s frequency %f\n", frequency, string_names[i], string_frequencies[i]);  // Debug print
            if (fabs(frequency - string_frequencies[i]) < TOLERANCE) {
                printf("String %s is in tune\n", string_names[i]);
                in_tune = 1;
                break;
            }
        }

        if (!in_tune) {
            printf("No string is in tune\n");
        }

        set_led_color(in_tune);  // Update LED color based on tuning status

        sleep_ms(1000);
    }

    return 0;
}
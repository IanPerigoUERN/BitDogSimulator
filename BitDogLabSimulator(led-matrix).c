#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/pio.h"
#include "ws2812b.pio.h"  // Biblioteca específica para NeoPixel

#define LED_PIN 7    // GPIO conectado ao Data In do primeiro LED
#define NUM_LEDS 25   // Número de LEDs na linha

// Função para configurar e inicializar o PIO para WS2812
void ws2812_init(PIO pio, int sm, uint pin) {
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, pin, 800000, false);
}

// Função para enviar cor para o NeoPixel
void set_pixel_color(uint32_t color) {
    for (int i = 0; i < NUM_LEDS; i++) {
        pio_sm_put_blocking(pio0, 0, color << 8);  // Envia a cor para todos os LEDs
    }
}

uint32_t rgb_to_grb(uint8_t r, uint8_t g, uint8_t b) {
    return ((g << 16) | (r << 8) | b);  // Formato GRB usado pelo WS2812
}

int main() {
    stdio_init_all();  // Inicializa comunicação UART
    PIO pio = pio0;
    int sm = 0;  // State machine 0
    ws2812_init(pio, sm, LED_PIN);

    while (true) {
        set_pixel_color(rgb_to_grb(255, 0, 0));  // Vermelho
        sleep_ms(100);
        set_pixel_color(rgb_to_grb(0, 255, 0));  // Verde
        sleep_ms(100);
        set_pixel_color(rgb_to_grb(0, 0, 255));  // Azul
        sleep_ms(100);
        set_pixel_color(rgb_to_grb(0, 0, 0));    // Desligar LEDs
        sleep_ms(100);
    }

    return 0;
}

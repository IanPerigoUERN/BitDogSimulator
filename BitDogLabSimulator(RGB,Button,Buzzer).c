#include <stdio.h>
#include "pico/stdlib.h" // Biblioteca padrão da placa raspberry PI Pico
#include "hardware/pwm.h" // COnfiguração e controle de periféricos com PWM
#include "hardware/clocks.h" // Configuração dos relógios internos
#include "hardware/pio.h" // Configuração e controle de periféricos com PIO

//#define LED_COUNT 25
#define LED_PIN 7

// Configuração do pino do buzzer
#define BUZZER_PIN 21

// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY 15000 // Frequência mais aguda, parece mais com um beep de um sensor de ré

// Definindo os LEDs como macros
//#define PEDESTRIAN_LED 28
#define RED_LED 13
#define BLUE_LED 12
#define GREEN_LED 11

// Definição de uma função para inicializar o PWM no pino do buzzer
//Necessário para controlar a frequência e duranção do beep
void pwm_init_buzzer(uint pin) {// parâmetro pin será o pino do buzzer acessado depois com pwm_init_buzzer(BUZZER_PIN)
    // Configurar o pino como saída de PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);// Número do pino e função PWM

    // Variável unsigned int que receberá o número do slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);// Recebe o número do pino e retorna o número do slice do PWM associado

    // Configurar o PWM com valores padrão
    pwm_config config = pwm_get_default_config();// Instancia config com as configurações padrão do PWM (biblioteca hardware/pwm.h)
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096)); // Ajustar divisor de clock na configuração do PWM
    // recebe como parâmetro o endereço da configuração e a frequência do clock do sistema dividido pela frequência do buzzer multiplicado por 4096
    // Iniciar o PWM recebendo o número do slice, a configuração e ativando o PWM após a inicialização
    pwm_init(slice_num, &config, true);

    // Iniciar o PWM no nível baixo, para não emitir som no início
    pwm_set_gpio_level(pin, 0);
}

// Definição de uma função para emitir um beep com duração específica
void beep(uint pin, uint duration_ms) { // função que será usada depois na função pedestrian_traffic_light. Recebe o pino do buzzer e a duração do beep
    // Obter o slice do PWM igual a função anterior
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o duty cycle para 100% (ativo)
    pwm_set_gpio_level(pin, 4096);

    // Função sleep_ms da biblioteca pico/stdlib.h para pausar a execução.
    sleep_ms(duration_ms);

    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pin, 0);
}

// Função para tráfego normal de veículos
void traffic_light(const uint BUTTON_PIN) {
    // Inicia a configuração dos pinos dos LEDs, passando as macros como parãmetros
    gpio_init(RED_LED);
    gpio_init(BLUE_LED);
    gpio_init(GREEN_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);// Configuração dos pinos como saída(GPIO_OUT)
    gpio_set_dir(BLUE_LED, GPIO_OUT);
    gpio_set_dir(GREEN_LED, GPIO_OUT);

    // Semáforo verde
    printf("Tráfego de veículos iniciado!\n");// Mensagem para o pedestre
    gpio_put(RED_LED, 0);   // Certifique-se de que o vermelho esteja desligado
    gpio_put(BLUE_LED, 0);  // Certifique-se de que o azul esteja desligado
    gpio_put(GREEN_LED, 1);
    for (int i = 0; i < 80; i++) {
        sleep_ms(100); // 8 segundos no total
        if (gpio_get(BUTTON_PIN) == 0) return;// Se o botão for pressionado, retorna para a função principal
    }

    // Semáforo amarelo
    
    gpio_put(GREEN_LED, 0); // Desliga o verde
    gpio_put(BLUE_LED, 0);  // Azul permanece desligado
    gpio_put(GREEN_LED, 1);
    gpio_put(RED_LED, 1);   // Liga o vermelho
    
    for (int i = 0; i < 20; i++) {
        sleep_ms(100); // 2 segundos no total
        if (gpio_get(BUTTON_PIN) == 0) return;
    }

    // Semáforo vermelho
    gpio_put(RED_LED, 0);   // Vermelho permanece ligado
    gpio_put(GREEN_LED, 0); // Verde permanece desligado
    gpio_put(BLUE_LED, 0);
    gpio_put(RED_LED, 1);
    for (int i = 0; i < 100; i++) {
        sleep_ms(100); // 10 segundos no total
        if (gpio_get(BUTTON_PIN) == 0) 
        return;
    }

    // Desligar todos os LEDs
    gpio_put(RED_LED, 0);
    gpio_put(GREEN_LED, 0); 
    gpio_put(BLUE_LED, 0);
}

// Função para tráfego de pedestres
void pedestrian_traffic_light(const uint BUTTON_PIN) {
    // Configurar os pinos do LED do pedestre
    gpio_init(GREEN_LED);
    gpio_set_dir(GREEN_LED, GPIO_OUT);

    // Apagar todas as luzes
    gpio_put(RED_LED, 0);
    gpio_put(BLUE_LED, 0);
    gpio_put(GREEN_LED, 0);
    gpio_put(GREEN_LED, 0);
    printf("Botão de travessia de pedestre pressionado, aguarde o sinal vermelho!\n");//Mensagem para o pedestre
    // Acender a luz amarela por 5 segundos
    gpio_put(GREEN_LED, 1);
    gpio_put(RED_LED, 1);
    sleep_ms(5000);

    // Apagar a luz amarela e acender a luz vermelha por 10 segundos
    gpio_put(GREEN_LED, 0);
    gpio_put(RED_LED, 0);
    
    gpio_put(RED_LED, 1);
    
    //gpio_put(GREEN_LED, 1); // Acender a luz verde do pedestre
    for (int i = 0; i < 10; i++) {
        beep(BUZZER_PIN, 500); // Acionar o buzzer por 5 segundos
        sleep_ms(500); 
    }
    printf("Travessia de pesdestre finalizada!\n");// Mensagem para o pedestre
    // Apagar a luz vermelha e a luz verde do pedestre
    gpio_put(RED_LED, 0);
    gpio_put(GREEN_LED, 0);
}

int main() {
    stdio_init_all();// Inicializar a comunicação serial (printf), Monitor Serial está declarado no diagram.json

    // Configuração do GPIO para o botão como entrada com pull-up
    const uint BUTTON_PIN = 5;  // Pino do botão
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);// Configurando o botão como entrada
    gpio_pull_up(BUTTON_PIN); // Habilitando o pull-up, Estado inicial do botão é 1(desligado)
    BUTTON_PIN == 1;
    // Configuração do GPIO para o buzzer como saída
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    // Inicializar o PWM no pino do buzzer
    pwm_init_buzzer(BUZZER_PIN);

    while (true) {
        
        if (gpio_get(BUTTON_PIN) == 0) { // Verifica o estado do botão
             // 0 = Botão pressionado  (nível lógico baixo)
            pedestrian_traffic_light(BUTTON_PIN); // Mudar para tráfego de pedestres
        } else {  // Botão não pressionado
            traffic_light(BUTTON_PIN);
        }
    }

    return 0;
}
#include "joystick.h"

const int VRY = 26;                  // Pino de leitura do eixo Y do joystick (conectado ao ADC)
const int VRX = 27;                  // Pino de leitura do eixo X do joystick (conectado ao ADC)
uint up_down = 1;                    // Variável para controlar se o nível do LED aumenta ou diminui
volatile uint32_t last_time_led = 0; // Tempo da ultima mudança de estado do LED
uint16_t led_level = 100;            // Nível inicial do PWM (duty cycle)

// Função para configurar o joystick
void setup_joystick()
{
    adc_init();         // Inicializa o módulo ADC
    adc_gpio_init(VRY); // Configura o pino VRX (eixo X) para entrada ADC
    adc_gpio_init(VRX); // Configura o pino VRY (eixo Y) para entrada ADC
}

// Função para realizar a leitura do joystick 
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value)
{
    // Leitura do valor do eixo Y do joystick
    adc_select_input(0);     // Seleciona o canal ADC para o eixo Y
    sleep_us(2);             // Pequeno delay para estabilidade
    *vry_value = adc_read(); // Lê o valor do eixo Y (0-4095)

    // Leitura do valor do eixo X do joystick
    adc_select_input(1);     // Seleciona o canal ADC para o eixo X
    sleep_us(2);             // Pequeno delay para estabilidade
    *vrx_value = adc_read(); // Lê o valor do eixo X (0-4095)
}

// Função para acender o LED_R e LED_G de acordo com a posição do joystick 
void joystick_led(uint LED_R_PIN, uint LED_G_PIN)
{
    uint16_t vrx_value, vry_value;
    joystick_read_axis(&vrx_value, &vry_value); // Lê os valores dos eixos do joystick

    // Ajusta os níveis PWM dos LEDs de acordo com os valores do joystick
    pwm_set_gpio_level(LED_R_PIN, vrx_value); // Ajusta o brilho do LED azul com o valor do eixo X
    pwm_set_gpio_level(LED_G_PIN, vry_value); // Ajusta o brilho do LED vermelho com o valor do eixo Y
}

// Função para configurar o PWM do LED
void setup_pwm_led(uint led, uint *slice, uint16_t level, float divider_pwm, uint16_t period)
{
    gpio_set_function(led, GPIO_FUNC_PWM); // Configura o pino do LED como saída PWM
    *slice = pwm_gpio_to_slice_num(led);   // Obtém o slice do PWM associado ao pino do LED
    pwm_set_clkdiv(*slice, divider_pwm);   // Define o divisor de clock do PWM
    pwm_set_wrap(*slice, period);          // Configura o valor máximo do contador (período do PWM)
    pwm_set_gpio_level(led, level);        // Define o nível inicial do PWM para o LED
    pwm_set_enabled(*slice, true);         // Habilita o PWM no slice correspondente ao LED
}

// Função que alterar a luminosidade do LED de acordo com um PWM. 
void pwm_led(uint led, uint16_t period)
{
    uint32_t current_time_led = to_ms_since_boot(get_absolute_time());

    if ((current_time_led - last_time_led) > 1000)
    {
        if (up_down)
        {
            led_level += 100; // Incrementa o nível do LED
            if (led_level >= period)
                up_down = 0; // Muda direção para diminuir quando atingir o período máximo
        }
        else
        {
            led_level -= 100; // Decrementa o nível do LED
            if (led_level <= 100)
                up_down = 1; // Muda direção para aumentar quando atingir o mínimo
        }
        last_time_led = current_time_led;
        pwm_set_gpio_level(led, led_level); // Define o nível atual do PWM (duty cycle)
    }
}
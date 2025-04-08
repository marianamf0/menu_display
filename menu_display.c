#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "display.h"
#include "joystick.h"
#include "buzzer.h"
#include "pico/binary_info.h"
#include "hardware/adc.h"

#define LED_R_PIN 13        // Pino do LED vermelho
#define LED_G_PIN 11        // Pino do LED verde
#define LED_B_PIN 12        // Pino do LED azul
#define BTN_PIN 22          // Pino do botão do joystick
#define BUZZER_PIN 21       // Pino do buzzer 
#define DEBOUNCE_MS 150     // Tempo minimo entre clicks do buzzer, para evitar o efeito do bounce 

char *text_menu[] = {"1-Joystick LED", "2-Buzzer", "3-LED RGB"}; 
int op_menu = 0;
static volatile bool state_menu = false;
volatile uint32_t last_interrupt_time = 0;

uint16_t led_r_level, led_g_level, led_b_level = 100;   // Inicialização dos níveis de PWM para os LEDs
uint slice_led_r, slice_led_g, slice_led_b;             // Variáveis para armazenar os slices de PWM correspondentes aos LEDs

// Função para resetar o estado dos LEDs e buzzer
void clear_led_and_buzzer(){ 
    pwm_set_gpio_level(LED_R_PIN, 0); 
    pwm_set_gpio_level(LED_G_PIN, 0);
    pwm_set_gpio_level(LED_B_PIN, 0);
    pwm_set_gpio_level(BUZZER_PIN, 0);
}

// Função de interrupção do botão do joystick 
void gpio_irq_handler(uint pin, uint32_t event)
{
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if ((current_time - last_interrupt_time) > DEBOUNCE_MS)
    {   
        last_interrupt_time = current_time;
        state_menu = !state_menu;
        if (state_menu)
        {
            char *text[] = {"Executando...", text_menu[op_menu] + 2};
            message(text, count_of(text), -1);
        }
        else
        {
            message(text_menu, count_of(text_menu), op_menu);
            clear_led_and_buzzer();
        }
    }
}

// Função principal 
int main()
{
    stdio_init_all();
    setup_display();
    setup_joystick();
    pwm_init_buzzer(BUZZER_PIN);
    setup_pwm_led(LED_R_PIN, &slice_led_r, led_r_level, 16.0, 4096);
    setup_pwm_led(LED_G_PIN, &slice_led_g, led_g_level, 16.0, 4096); 
    setup_pwm_led(LED_B_PIN, &slice_led_b, led_b_level, 16.0, 2000); 
    clear_led_and_buzzer();

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);

    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

restart:

    message(text_menu, count_of(text_menu), op_menu);
    while (true)
    {

        uint16_t vrx_value, vry_value;
        joystick_read_axis(&vrx_value, &vry_value);

        if (state_menu){ 
            if (op_menu == 0){
                joystick_led(LED_R_PIN, LED_G_PIN);
            }
            else if (op_menu == 2){
                pwm_led(LED_B_PIN, 2000);
            }
            else if (op_menu == 1) {
                play_star_wars(BUZZER_PIN);
            }

        }
        if ((vry_value > 4050) & (!state_menu))
        {
            op_menu = op_menu == 0 ? 2 : op_menu - 1;
            message(text_menu, count_of(text_menu), op_menu);
        }
        else if ((vry_value < 50) & (!state_menu))
        {
            op_menu = (op_menu == 2 ? 0 : op_menu + 1);
            message(text_menu, count_of(text_menu), op_menu);
        }

        
        sleep_ms(100);
    }

    return 0;
}

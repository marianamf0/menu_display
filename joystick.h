#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

// Função para configurar o joystick
void setup_joystick();

// Função para realizar a leitura do joystick 
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value);

// Função para acender o LED_R e LED_G de acordo com a posição do joystick 
void joystick_led(uint LED_R_PIN, uint LED_G_PIN);

// Função para configurar o PWM do LED
void setup_pwm_led(uint led, uint *slice, uint16_t level, float divider_pwm, uint16_t period);

// Função que alterar a luminosidade do LED de acordo com um PWM. 
void pwm_led(uint led, uint16_t period);
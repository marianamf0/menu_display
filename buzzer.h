#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Função para configurar o PWM do buzzer.
void pwm_init_buzzer(uint pin); 

// Toca um tom específico no buzzer.
void play_tone(uint pin, uint frequency);

// Função principal para tocar a música
void play_star_wars(uint pin);
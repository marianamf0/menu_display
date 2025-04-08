#include "inc/ssd1306.h"
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <ctype.h>
#include "inc/ssd1306_font.h"

// Definindo os pinos do I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// Variável global que define a área de renderização
extern struct render_area frame_area;

// Função para configurar o display
void setup_display(void);

// Função para limpar o display
void clear(void);

// Função para exibir uma mensagem no display
void message(char *text[], size_t text_count, int op);

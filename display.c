#include "display.h"

// Variável global que define a área de renderização
struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
};

// Função para limpar o display
void clear()
{
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
}

// Função para configurar o display
void setup_display()
{
    i2c_init(I2C_PORT, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();

    calculate_render_area_buffer_length(&frame_area);
    clear();
}

// Função para exibir uma mensagem no display
void message(char *text[], size_t text_count, int op)
{
    clear();
    uint8_t ssd[ssd1306_buffer_length];
    int total_text_height = 8 * (text_count >= 3 ? text_count : 3);
    int y = (ssd1306_height - total_text_height) / 2;

    for (size_t i = 0; i < text_count; i++)
    {
        ssd1306_draw_string(ssd, 0, y, text[i]);
        y += 15;
    }
    if ((op < text_count) & (op >= 0))
    {
        int start_index = (2 * (op + 1)) * 128; // Cada linha tem 128 bytes
        for (size_t j = 0; j < 128; j++)
        {
            ssd[start_index + j] = ~ssd[start_index + j]; // Inverte os bits de cada byte na linha
        }
    }

    render_on_display(ssd, &frame_area);
}

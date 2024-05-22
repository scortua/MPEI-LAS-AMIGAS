#include "oled.h"

uint8_t _i2caddr, _vccstate, x_pos, y_pos, text_size;
bool wrap = true;

static uint8_t font[] = {
0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x5F, 0x00, 0x00,
0x00, 0x07, 0x00, 0x07, 0x00,
0x14, 0x7F, 0x14, 0x7F, 0x14,
0x24, 0x2A, 0x7F, 0x2A, 0x12,
0x23, 0x13, 0x08, 0x64, 0x62,
0x36, 0x49, 0x56, 0x20, 0x50,
0x00, 0x08, 0x07, 0x03, 0x00,
0x00, 0x1C, 0x22, 0x41, 0x00,
0x00, 0x41, 0x22, 0x1C, 0x00,
0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
0x08, 0x08, 0x3E, 0x08, 0x08,
0x00, 0x80, 0x70, 0x30, 0x00,
0x08, 0x08, 0x08, 0x08, 0x08,
0x00, 0x00, 0x60, 0x60, 0x00,
0x20, 0x10, 0x08, 0x04, 0x02,
0x3E, 0x51, 0x49, 0x45, 0x3E,
0x00, 0x42, 0x7F, 0x40, 0x00,
0x72, 0x49, 0x49, 0x49, 0x46,
0x21, 0x41, 0x49, 0x4D, 0x33,
0x18, 0x14, 0x12, 0x7F, 0x10,
0x27, 0x45, 0x45, 0x45, 0x39,
0x3C, 0x4A, 0x49, 0x49, 0x31,
0x41, 0x21, 0x11, 0x09, 0x07,
0x36, 0x49, 0x49, 0x49, 0x36,
0x46, 0x49, 0x49, 0x29, 0x1E,
0x00, 0x00, 0x14, 0x00, 0x00,
0x00, 0x40, 0x34, 0x00, 0x00,
0x00, 0x08, 0x14, 0x22, 0x41,
0x14, 0x14, 0x14, 0x14, 0x14,
0x00, 0x41, 0x22, 0x14, 0x08,
0x02, 0x01, 0x59, 0x09, 0x06,
0x3E, 0x41, 0x5D, 0x59, 0x4E,
0x7C, 0x12, 0x11, 0x12, 0x7C,
0x7F, 0x49, 0x49, 0x49, 0x36,
0x3E, 0x41, 0x41, 0x41, 0x22,
0x7F, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x49, 0x49, 0x49, 0x41,
0x7F, 0x09, 0x09, 0x09, 0x01,
0x3E, 0x41, 0x41, 0x51, 0x73,
0x7F, 0x08, 0x08, 0x08, 0x7F,
0x00, 0x41, 0x7F, 0x41, 0x00,
0x20, 0x40, 0x41, 0x3F, 0x01,
0x7F, 0x08, 0x14, 0x22, 0x41,
0x7F, 0x40, 0x40, 0x40, 0x40,
0x7F, 0x02, 0x1C, 0x02, 0x7F,
0x7F, 0x04, 0x08, 0x10, 0x7F,
0x3E, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x09, 0x09, 0x09, 0x06,
0x3E, 0x41, 0x51, 0x21, 0x5E,
0x7F, 0x09, 0x19, 0x29, 0x46
};

static uint8_t font2[] = {
0x26, 0x49, 0x49, 0x49, 0x32,
0x03, 0x01, 0x7F, 0x01, 0x03,
0x3F, 0x40, 0x40, 0x40, 0x3F,
0x1F, 0x20, 0x40, 0x20, 0x1F,
0x3F, 0x40, 0x38, 0x40, 0x3F,
0x63, 0x14, 0x08, 0x14, 0x63,
0x03, 0x04, 0x78, 0x04, 0x03,
0x61, 0x59, 0x49, 0x4D, 0x43,
0x00, 0x7F, 0x41, 0x41, 0x41,
0x02, 0x04, 0x08, 0x10, 0x20,
0x00, 0x41, 0x41, 0x41, 0x7F,
0x04, 0x02, 0x01, 0x02, 0x04,
0x40, 0x40, 0x40, 0x40, 0x40,
0x00, 0x03, 0x07, 0x08, 0x00,
0x20, 0x54, 0x54, 0x78, 0x40,
0x7F, 0x28, 0x44, 0x44, 0x38,
0x38, 0x44, 0x44, 0x44, 0x28,
0x38, 0x44, 0x44, 0x28, 0x7F,
0x38, 0x54, 0x54, 0x54, 0x18,
0x00, 0x08, 0x7E, 0x09, 0x02,
0x18, 0xA4, 0xA4, 0x9C, 0x78,
0x7F, 0x08, 0x04, 0x04, 0x78,
0x00, 0x44, 0x7D, 0x40, 0x00,
0x20, 0x40, 0x40, 0x3D, 0x00,
0x7F, 0x10, 0x28, 0x44, 0x00,
0x00, 0x41, 0x7F, 0x40, 0x00,
0x7C, 0x04, 0x78, 0x04, 0x78,
0x7C, 0x08, 0x04, 0x04, 0x78,
0x38, 0x44, 0x44, 0x44, 0x38,
0xFC, 0x18, 0x24, 0x24, 0x18,
0x18, 0x24, 0x24, 0x18, 0xFC,
0x7C, 0x08, 0x04, 0x04, 0x08,
0x48, 0x54, 0x54, 0x54, 0x24,
0x04, 0x04, 0x3F, 0x44, 0x24,
0x3C, 0x40, 0x40, 0x20, 0x7C,
0x1C, 0x20, 0x40, 0x20, 0x1C,
0x3C, 0x40, 0x30, 0x40, 0x3C,
0x44, 0x28, 0x10, 0x28, 0x44,
0x4C, 0x90, 0x90, 0x90, 0x7C,
0x44, 0x64, 0x54, 0x4C, 0x44,
0x00, 0x08, 0x36, 0x41, 0x00,
0x00, 0x00, 0x77, 0x00, 0x00,
0x00, 0x41, 0x36, 0x08, 0x00,
0x02, 0x01, 0x02, 0x04, 0x02
};

static uint8_t OLED_buffer[OLED_LCDHEIGHT * (OLED_LCDWIDTH / 8)];

static void sh1106_send_command(uint8_t command) {
    I2C_Start();
    I2C_Tx(OLED_I2C_ADDRESS);
//    ACKStatus();
    I2C_Tx(0X00);
//    ACKStatus();
    I2C_Tx(command);
//    ACKStatus();
    I2C_Stop();
}

// Enviar datos al SH1106
static void sh1106_send_data(uint8_t data) {
    I2C_Start();
    I2C_Tx(OLED_I2C_ADDRESS);
    ACKStatus();
    I2C_Tx(0X40);
    ACKStatus();
    I2C_Tx(data);
    ACKStatus();
    I2C_Stop();
}

// Inicialización del SH1106
void sh1106_init(void) {
    sh1106_send_command(0xAE); // Display off
    // Configuración del SH1106
    sh1106_send_command(0xD5); // Set display clock divide ratio/oscillator frequency
    sh1106_send_command(0x80);
    sh1106_send_command(0xA8); // Set multiplex ratio
    sh1106_send_command(0x3F);
    sh1106_send_command(0xD3); // Set display offset
    sh1106_send_command(0x00);
    sh1106_send_command(0x40); // Set start line address
    sh1106_send_command(0x8D); // Charge pump
    sh1106_send_command(0x14);
    sh1106_send_command(0x20); // Memory addressing mode
    sh1106_send_command(0x00);
    sh1106_send_command(0xA1); // Segment remap
    sh1106_send_command(0xC8); // COM output scan direction
    sh1106_send_command(0xDA); // Set COM pins hardware configuration
    sh1106_send_command(0x12);
    sh1106_send_command(0x81); // Contrast control
    sh1106_send_command(0xCF);
    sh1106_send_command(0xD9); // Pre-charge period
    sh1106_send_command(0xF1);
    sh1106_send_command(0xDB); // VCOMH deselect level
    sh1106_send_command(0x40);
    sh1106_send_command(0xA4); // Entire display ON
    sh1106_send_command(0xA6); // Set normal display
    sh1106_clear();
    sh1106_send_command(0xAF); // Display ON
}

// Limpiar pantalla
void sh1106_clear(void) {
    memset(OLED_buffer, 0,(OLED_LCDWIDTH * OLED_LCDHEIGHT / 8));
}

// Actualizar pantalla
void sh1106_display(void) {
    for (uint8_t i = (OLED_LCDWIDTH/8)-1; i >= 0; i--) {
        sh1106_send_command(0xB0 + i); // Set page address
        sh1106_send_command(0x00); // Set lower column start address
        sh1106_send_command(0x10); // Set higher column start address

        for (uint16_t j = OLED_LCDWIDTH; j > 0; j--) {
            I2C_Start();
            I2C_Tx(OLED_I2C_ADDRESS);
//            ACKStatus();
            I2C_Tx(0X40);
//            ACKStatus();
            for(uint8_t x = 0; x < 16; x++){
                I2C_Tx(OLED_buffer[i*OLED_LCDWIDTH + OLED_LCDWIDTH - j]);
//                ACKStatus();
                j--;
            }
            j++;
            I2C_Stop();
        }
    }
}

// Dibujar pixel
void sh1106_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if ((x < 0) || (x >= OLED_LCDWIDTH) || (y < 0) || (y >= OLED_LCDHEIGHT)) return;

    switch(color)
    {
        case WHITE: OLED_buffer[x+ (y/8)*OLED_LCDWIDTH] |= (1 << (y&7)); break;
        case BLACK: OLED_buffer[x+ (y/8)*OLED_LCDWIDTH] &= ~(1 << (y&7)); break;
        case INVERSE: OLED_buffer[x+ (y/8)*OLED_LCDWIDTH] ^= (1 << (y&7)); break;
    }
}

// Dibujar carácter
void sh1106_draw_char(uint8_t x, uint8_t y, char c) {
    if (c < 32 || c > 126) return; // Caracteres fuera del rango imprimible

    for (uint8_t i = 0; i < 5; i++) {
        uint8_t line = font[c - 32];
        for (uint8_t j = 0; j < 7; j++) {
            if (line & 0x01) {
                sh1106_draw_pixel(x + i, y + j, 1);
            } else {
                sh1106_draw_pixel(x + i, y + j, 0);
            }
            line >>= 1;
        }
    }

    // Espacio entre caracteres
    for (uint8_t j = 0; j < 7; j++) {
        sh1106_draw_pixel(x + 5, y + j, 0);
    }
}

// Dibujar cadena de texto
void sh1106_draw_string(uint8_t x, uint8_t y, const char* str) {
    while (*str) {
        sh1106_draw_char(x, y, *str);
        x += 6; // Ancho del carácter + 1
        str++;
    }
}
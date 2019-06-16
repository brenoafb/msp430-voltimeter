#ifndef LCD_H
#define LCD_H

#include <msp430.h>

#define BR100K 	10	//SCL operando em 100 kHz (SMCLK)
#define BR10K   21	//SCL operando em 50 kHz (SMCLK)
#define BR10K   105	//SCL operando em 10 kHz (SMCLK)

#define TRUE 1
#define FALSE 0
#define LED1 BIT0
#define LED1_OUT P1OUT
#define LED2 BIT7
#define LED2_OUT P4OUT

// Bits para controle do LCD
#define SDA_SEL P3SEL
#define SDA_BIT BIT0
#define SDA_REN P3REN
#define SDA_OUT P3OUT
#define SCL_SEL P3SEL
#define SCL_BIT BIT1
#define SCL_REN P3REN
#define SCL_OUT P3OUT
#define BIT_RS BIT0
#define BIT_RW BIT1
#define BIT_E BIT2
#define BIT_BL BIT3
#define BIT_D4 BIT4
#define BIT_D5 BIT5
#define BIT_D6 BIT6
#define BIT_D7 BIT7

// Dois possíveis endereços do PCF8574
#define PCF_ADR	0x27
// #define PCF_ADR	0x3F

// Protótipo das funções
void config_I2C(void);
void LCD_inic(void);
void LCD_aux(char dado);
void PCF_STT_STP(void);
int PCF_read(void);
void PCF_write(char dado);
void delay(long limite);


char hex_ascii(unsigned char h);
char dec_ascii(unsigned char h);

void lcd_nibble(char n);
void lcd_char(char c);
void lcd_str(char *str);
void lcd_hex8(unsigned char n);
void lcd_hex16(unsigned int n);
void lcd_dec8(unsigned char n);
void lcd_dec16(unsigned int n);
void lcd_float(float x);
void lcd_cursor(unsigned char lin, unsigned char col);
void lcd_clear();
int lcd_ocupado();

#endif

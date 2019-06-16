// SHS=1 --> Disparar por TA0.1
// CONSEQ=1 --> Repetir Sequência de Canais (0, 1)

#include <msp430.h> 
#include "lcd.h"

#define TRUE    1
#define FALSE   0
#define N       16

void config_timer(void);
void config_adc(void);
__interrupt void isr(void);
unsigned int avg(unsigned int arr[], unsigned int n);

volatile unsigned int ch0[N/2];
volatile unsigned int ch1[N/2];
volatile unsigned int k0 = 0;
volatile unsigned int k1 = 0;
volatile unsigned int count = 0;
volatile char flag = 0;

int main(void){
  unsigned int s0 = 0;
  unsigned int s1 = 0;
  float f0 = 0;
  float f1 = 0;
  WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
  config_I2C();
  LCD_inic();
  config_timer();
  config_adc();
  __enable_interrupt();

  ADC12CTL0 |= ADC12ENC;          //Habilitar ADC12
    
  while (TRUE){
    if (flag) {
      s0 = avg(ch0, N/2);
      s1 = avg(ch1, N/2);

      f0 = (3.3 * s0) / 0xfff;
      f1 = (3.3 * s1) / 0xfff;
      
      lcd_clear();
      lcd_cursor(0, 0);
      lcd_str("V0: ");
      lcd_float(f0);
      lcd_char(' ');
      lcd_hex16(s0);

      lcd_cursor(1, 0);
      lcd_str("V1: ");
      lcd_float(f1);
      lcd_char(' ');
      lcd_hex16(s1);

      flag = 0;
    }
  }
  return 0;
}

void config_adc(void){
  ADC12CTL0  &= ~ADC12ENC;        //Desabilitar para configurar

  ADC12CTL0 = ADC12SHT0_3 |       //ADC tempo amostragem ADCMEM[0-7]
    ADC12ON;            //Ligar ADC

  ADC12CTL1 = ADC12CSTARTADD_0 |  //Resultado em ADC12MEM0
    ADC12SHS_1       |  // Selecionar TA0.1
    ADC12SHP         |  // S/H usar timer
    ADC12DIV_0       |  //Clock ADC Divisor = 1
    ADC12SSEL_3      |  //Clock ADC = SMCLK
    ADC12CONSEQ_3;      //Modo Sequência de canais repetido

  ADC12CTL2 = ADC12TCOFF |        // Desligar sensor temperatura
    ADC12RES_2;         // Resolução 12-bit

  ADC12IE = ADC12IE0 | ADC12IE1; // Habilitar interrupção canais 0 e 1

  ADC12MCTL0 = ADC12SREF_0 |      //VR+ = AVCC e VR- = AVSS
    ADC12INCH_0;       //(P6.0) A0 = Canal 0
  ADC12MCTL1 = ADC12SREF_0 |      //VR+ = AVCC e VR- = AVSS
    ADC12INCH_1;       //(P6.1) A1 = Canal 0

  ADC12CTL0 |= ADC12ENC;          //Habilitar ADC12

  P6SEL |= BIT0;
  P6SEL |= BIT1;
}

void config_timer(void){
  TA0CTL = TASSEL__ACLK | MC__UP;
  TA0CCTL1 = OUTMOD_6;
  TA0CCR0 = 32768 / 128;   // 16 hz
  TA0CCR1 = TA0CCR0 / 2;
}

#pragma vector=ADC12_VECTOR
__interrupt void isr(void) {
  switch (ADC12IV) {
  case 6:
    // canal 0
    ch0[k0++] = ADC12MEM0;
    count++;
    ADC12IFG &= ~ADC12IFG0;
    break;
  case 8:
    // canal 1
    ch1[k1++] = ADC12MEM1;
    count++;
    ADC12IFG &= ~ADC12IFG1;
    break;
  }

  if (count == N) {
    flag = 1;
    k0 = k1 = count = 0;
  }
}

unsigned int avg(unsigned int arr[], unsigned int n) {
  unsigned int s = 0;
  int i = 0;
  for (i = 0; i < n; i++) {
    s += arr[i];
  }
  return s / n;
}

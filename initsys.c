#include "initsys.h"
#include "PIC16F1827.h"
void initsys(void)
{
// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection->INTOSC oscillator: I/O function on CLKIN pin
#pragma config WDTE = SWDTEN    // Watchdog Timer Enable->WDT disabled
#pragma config PWRTE = OFF    // Power-up Timer Enable->PWRT disabled
#pragma config MCLRE = ON    // MCLR Pin Function Select->MCLR/VPP pin function is MCLR
#pragma config CP = OFF    // Flash Program Memory Code Protection->Program memory code protection is disabled
#pragma config CPD = OFF    // Data Memory Code Protection->Data memory code protection is disabled
#pragma config BOREN = ON    // Brown-out Reset Enable->Brown-out Reset enabled
#pragma config CLKOUTEN = OFF    // Clock Out Enable->CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin
#pragma config IESO = ON    // Internal/External Switchover->Internal/External Switchover mode is enabled
#pragma config FCMEN = ON    // Fail-Safe Clock Monitor Enable->Fail-Safe Clock Monitor is enabled

// CONFIG2
#pragma config WRT = OFF    // Flash Memory Self-Write Protection->Write protection off
#pragma config PLLEN = OFF    // PLL Enable->4x PLL disabled
#pragma config STVREN = ON    // Stack Overflow/Underflow Reset Enable->Stack Overflow or Underflow will cause a Reset
#pragma config BORV = LO    // Brown-out Reset Voltage Selection->Brown-out Reset Voltage (Vbor), low trip point selected.
#pragma config LVP = ON    // Low-Voltage Programming Enable->Low-voltage programming enabled
    
//OSCILADOR:
    OSCCON = 0x68; // PLL off, 4MHz, FOSC.     
    OSCTUNE = 0x00; // Calibrado de fabrica
    BORCON = 0x00; // Brown off
//TRANSMISION
    BAUDCON = 0x08;// falling edge, 16bit
    RCSTA = 0x90; //SP enabled, Receiver enabled 
    TXSTA = 0x24; //8-bit, sincrono, high speed
    SPBRGL = 0x67;//Velocidad de transmision 9600 
    SPBRGH = 0x00;
    APFCON0bits.RXDTSEL=0;//Asignar RX a RB1
    APFCON1bits.TXCKSEL=0;//Asignar TX a RB2
    
// PUERTOS
    TRISA = 0b00000001; // 0 salida, 1 entrada, RA0 entrada
    ANSELA = 0b00000001;//Definir analogico 1 o digtal 0
    TRISB = 0b11011111; //RX
    ANSELB = 0b00000000;
    
    LATA = 0x00;
    LATB = 0x00;
    WPUB = 0x00;
    WPUA = 0x00;
    OPTION_REGbits.nWPUEN = 1;
    
// TIMER
    T1CON=0b00000101; // FOSC/4, PRESCALER/8, TIMER ON/ cada conteo 8/1000000
    T1GCON=0x00;
    
// ADC
    ADCON0=0b00000001; // ADC0, ADCGO,ADON
    ADCON1=0b01000000; // Justificado a la izquierda, FOSC/4

//INTERRUPCIONES
    PIE1=0b00100001; //bit 6 ADIE, bit 5 RCIE, bit0 TMR1IE
    INTCONbits.GIE=1;//se activa interruopciones globales
    INTCONbits.PEIE=1;//se activa interrupciones de perifericos
//WTD
    WDTCON=0b00000000;//Time-out 1ms
}


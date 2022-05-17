
#ifndef NUMBERS_H
#define	NUMBERS_H

#define LED_A PORTBbits.RB3
#define LED_B PORTAbits.RA1
#define LED_C PORTAbits.RA0
#define LED_D PORTAbits.RA7
#define LED_E PORTAbits.RA6
#define LED_F PORTBbits.RB5
#define LED_G PORTBbits.RB4

typedef struct STATES{
    int seg_1;
    int seg_2;
    int seg_3;
    int flag_init;
    int flag_result;
}STATES;

void zero();
void one();
void two();
void three();
void four();
void five();
void six();
void seven();
void eight();
void nine();
void show_number(int result);

void convert_number(float value, STATES *states);
#endif	/* NUMBERS_H */
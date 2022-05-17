#include "initsys.h"
#include <xc.h>
int rx; //Variable que toma el comando recibido
int estados=2;  //Variable que cambia de estados la maquina general  
int haydato=0;  //Variable que indica si hubo un dato en la interrupcion de la recepcion
int comandos=0; //Variable que cambia los estados de la maquina de decodificacion del comando
int addressH=0;
int addressL=0;
int contRX=0;
int modo=0; //Indica el modo en el que se encuentra el sistema, 0 para modo normal, 1 para modo sleep
int limalerta=40;   //Indica el limite de la alerta
int timercont=0;
int a=0;
int ADCvalue=0;
int ADCfiltrado=0;
int overflow=0;
int recepcionH;
int recepcionL;
int address=0;
int alerta=0;
unsigned int cont=0;

void timerreload(){//Volver a cargar el timer con el valor inicial
    TMR1L=0x77;
    TMR1H=0xEC;  
    //Base de tiempo de 5 ms
}

void main(void) {
    initsys();//Se inicializa el sistema
    timerreload();//Se carga el timer
    SSP1CON1bits.SSPEN=1;  //Se habilita el modulo MSSP  
    while(1){
        switch(estados){//MAQUINA PRINCIPAL
            case 0:
                switch(comandos){//DECODIFICACION DE LOS COMANDOS
                    case 0: //DECODIFICAR COMANDO
                        if(haydato==1){
                            if(rx==65){//CAMBIAR DE MODO----->A                                
                                haydato=0;
                                if(modo==0){//SIGNIFICA QUE SE ESTA EN MODO NORMAL Y SE CAMBIA A MODO SLEEP
                                    estados=2;//SE VA AL ESTADO DE MODO SLEEP EN LA MAQUINA PRINCIPAL
                                    modo=1;
                                }
                                else{//SIGNIFCA QUE SE ESTA EN MODO SLEEP Y SE CAMBIA A MODO NORMAL
                                    estados=3;//SE VA AL ESTADO DE MODO NORMAL EN LA MAQUINA PRINCIPAL
                                    modo=0;
                                }
                                
                            }
                           
                           
                            else{
                                TXREG=0;
                                while(!TXSTAbits.TRMT);
                                TXREG=1;
                                while(!TXSTAbits.TRMT);
                                while(cont<65000){
                                     cont++;
                                }
                                cont=0;
                                if(modo==0){
                                estados=2;
                                 }
                                 else{
                                estados=3;
                                } 
                            }
                        }
                        break;
                break;
            case 1:
                //MODO NORMAL
                if((ADCfiltrado)>=(limalerta*10.23)){//SE PREGUNTA POR EL VALOR DEL ADC FILTRADO Y VEMOS SI ES MAYOR QUE LA ALERTA
                            PORTAbits.RA1=1;
                        }
                        else{
                            PORTAbits.RA1=0;
                        }
                if(overflow==1){//SE ADQUIERE EL ADC CADA OVERFLOW, ES DECIR, CADA 5 ms
                    overflow=0;
                    ADCON0bits.GO_nDONE=1;//SE INICIA CONVERSION 
                    while(ADCON0bits.GO_nDONE);//SE ESPERA A QUE TERMINE LA CONVERSION
                    PORTAbits.RA3^=1;
                    ADCvalue=(ADRESH << 2) + (ADRESL>>6);//SE ADQUIERE EL VALOR DEL ADC
                    ADCfiltrado=ADCvalue;//SE FILTRA EL VALOR
                        TXREG=ADCfiltrado>>8;//SE TRANSMITE VALOR HIGH DEL VALOR FILTRADO
                        while(!TXSTAbits.TRMT);
                        TXREG=ADCfiltrado;//SE TRANSMITE VALOR LOW DEL VALOR FILTRADO
                        while(!TXSTAbits.TRMT);
                        
                }
             
                break;
            case 2:
                //MODO SLEEP
                if((ADCfiltrado)>=(limalerta*10.23)){//SE PREGUNTA POR EL VALOR DEL ADC FILTRADO Y SE VE SI ES MAYOR QUE LA ALERTA
                            PORTAbits.RA1=1;
                        }
                        else{
                            PORTAbits.RA1=0;
                        }
                if(overflow==1){
                    ADCON0bits.GO_nDONE=1;
                    overflow=0;
                    while(ADCON0bits.GO_nDONE);
                    ADCvalue=(ADRESH << 2) + (ADRESL>>6);
                    ADCfiltrado=ADCvalue;                    
                    WDTCONbits.SWDTEN=1;
                    SLEEP();
                    WDTCONbits.SWDTEN=0;
                    
            
                }
                }
                break;
        }

    }
}

void __interrupt() INTERRUPT_InterruptManager (void)//RUTINA DE INTERRUPCIONES
{
    if(PIE1bits.TMR1IE && PIR1bits.TMR1IF == 1)//INTERRUPCION DEL TIMER
    {
        timercont++;
        overflow=1;
        timerreload();
        PIR1bits.TMR1IF = 0;        
    }
    else if(PIE1bits.RCIE == 1 && PIR1bits.RCIF == 1)//INTERRUPCION DE RECEPCION
        {
            rx=RCREG;
            haydato=1;
            estados=0;//ME MANDA A DECODIFICAR COMANDO
        } 

}

#include "SanUSB48.h"
#include "bt_usb.h"//https://play.google.com/store/apps/details?id=appinventor.ai_sandro_juca.BT4SanUSB_HC05
//Vídeo BLE (Bluetooth Low Energy): http://youtu.be/k4-Onma7Xgs
//https://dl.dropboxusercontent.com/u/101922388/ProjSanUSB1_MPLABX/Projeto1C18.X.zip
//https://dl.dropboxusercontent.com/u/101922388/ApostilaSanUSB/Apostila_SanUSB_MPLABX.pdf
//Gerenciador.hex de gravação USB em: https://dl.dropboxusercontent.com/u/101922388/121007SanUSBOrig/Gerenciador/Gerenciador.hex

#define pinored pin_c2 //pinos para serem conectados ao led RGB
#define pinoblue pin_c1
#define pinogreen pin_c0

unsigned char valor,valorbcd, endereco, numquant=0, temp=0;
unsigned char  n=0, m=0, ind;
short int flagA4=0, flagA5=0;
unsigned int i=0, w=0, time;
unsigned long int resultado, tensao_lida16;

short int  flag=0, flagstart=0, flagNM=0, vorbei=0;
unsigned char comando[30], comand[30], c1=0;

#pragma interrupt interrupcao
void interrupcao(){

    if (INTCONbits.TMR0IF)     {  //espera o estouro do timer0 -> TMR0L=0
    INTCONbits.TMR0IF = 0;        //limpa a flag de interrupção
    i++; if(i>255){i=0;}
    if(i>=comand[1]){nivel_alto(pinored);} else {nivel_baixo(pinored);}
    if(i>=comand[2]){nivel_alto(pinogreen);} else {nivel_baixo(pinogreen);}
    if(i>=comand[3]){nivel_alto(pinoblue);} else {nivel_baixo(pinoblue);}

    TMR0L = TMR0L + 16; // load time before plus 240us/12(48MHz) = 10us x 4 (prescaler 001) = 80us -> T = 80us * 255 =~  20ms
                                }

if (serial_interrompeu)  { INTCONbits.TMR0IE = 0;//Desabilita interupção timer 0
     serial_interrompeu=0;
     comando[n] = le_serial();
     if (comando[n]==79) {flagb=1;n=0;}

         if (m>13 && comando[0]==65) {m=n; flag=1;pisca=0;}

         if (comando[n]=='R'){n=0;comando[0] = 'R';} //UTILIZAR VALORES DECIMAIS EM DOIS DIGITOS. ex:06, 23, 15, etc.

         if (n>=3 && comando[0]=='R')  {
                         comand[1]= comando[1]; //Led RGB anodo comum//
			 comand[2]= comando[2];
                         comand[3]= comando[3];
                         n=0;  comando[0] = 32;
			 inverte_saida(pin_b7); INTCONbits.TMR0IE = 1;
                                        }
            switch (comando[0]){
            case  79 : {flag=0; chegou=1;m=0; ;pisca=0;inverte_saida(pin_b7);}

            case 'L' : {flag=0; n=0; c=1; nivel_alto(pin_b7); pisca=0;}
            break;
            case 'D' : {flag=0; n=0; nivel_baixo(pin_b7); pisca=0;}
            break;
            case 'P': { flag=0; pisca=1; n=0; }
            break;
            case 'T': { flag=0; inverte_saida(pin_b7); n=0; }
            break;
                            }

            ++n; m=n; if(n>=29){n=0;}
                               }
}

void main(){

    clock_int_48MHz();
    habilita_interrupcao(recep_serial);
    taxa_serial(9600);
    bt_ini();

    T0CON =0B11000001; //TMR0ON, 8 bits, Prescaler 1:4 (001 - see datasheet)
    TMR0L = 16; //conta 240us antes de estourar x 4 (prescaler)
    RCONbits.IPEN = 1;    //apenas interrupções de alta prioridade
    INTCONbits.GIEH = 1;  //Habilita interrupções de alta prioridade
    INTCONbits.TMR0IE = 1;//Habilita interupção timer 0
   
    for(k=0;k<=25;k++){comando[k]=32;}

    while(1){

    while (pisca==1){inverte_saida(pin_b7);tempo_ms(300); n=0;}

    if (flag==1)  {
           flag=0; escreve_eeprom(32, m);
           for(k=0;k<=m;k++){escreve_eeprom(k, comando[k]);}
      
       if(le_eeprom(34)==0 )
        {
         modoAttention();  //Es verliert RAM Daten wegen restart.
         m=le_eeprom(32);
         for(k=0;k<=m;k++){swputc(le_eeprom(k));}
         for(k=m+1;k<25;k++){sputc(64);}sputc(26);sputc(20);
         modoNormal();
        }

        if(le_eeprom(33)==0 )
        {chegou=0;
        for(k=0;k<=m;k++){swputc(le_eeprom(k));}
        for(k=m+1;k<25;k++){sputc(64);}
        tempo_ms(500);sputc(26);tempo_ms(500);
        }

      for(k=0;k<=m;k++){comando[k]=32;} 
      n=0;m=0;
      INTCONbits.TMR0IE = 1;
                        }

        n=0;
        tempo_ms(1000);
        if(!entrada_pin_e3){Reset();}//pressionar o botão para gravação
              }
}
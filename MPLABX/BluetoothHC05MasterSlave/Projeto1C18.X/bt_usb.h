/*
 * utilizando modem bluetooth Low Energy (BLE), modem JY-MCU ou Master/Slave HC05 (key ligado ao pino b2)
 * Vídeo BLE (Bluetooth Low Energy): http://youtu.be/k4-Onma7Xgs
 * https://play.google.com/store/apps/details?id=appinventor.ai_sandro_juca.BT4SanUSB_HC05&hl=pt_BR               
 * Aguarde até que o led do pino B7 esteja piscando para conectar o modem bluetooth.
 * Versao JY-MCU e HC-05(Android 4.0)  -   Versão HM-10 (Android 4.4 e iOS)
 *                                                                                               */
#ifndef __BT_USB_H
#define __BT_USB_H

#define key_HC05  pin_b2 //Ligar o pino B2 do microcontrolador ao pino Key do modem bluetooth master-slave (6 pinos)

short int pisca=0,chegou=0,c=0,flagb=0,flagc=0;
unsigned char  comand[30],z,y;
unsigned char G=32, ru=32, po=32, e=32, Fe=32, rr=32;

const rom unsigned char icg[] = {130,168,86,132,130,170,136,106,0};
const rom unsigned char icp[] = {130,168,0};
const rom unsigned char icr[] = {130,168,86,164,138,166,138,168,0};
const rom unsigned char icb[] = {130,168,86,132,130,170,136,106,0};
const rom unsigned char icm[] = {130,168,86,156,130,154,138,166,194,220,170,166,132,98,0};
const rom unsigned char icn[] = {130,168,86,156,130,154,138,144,154,190,166,194,220,170,166,132,0};
const rom unsigned char icgs[] = {130,168,86,170,130,164,168,122,98,114,100,96,96,88,96,88,96,26,20,0};  //98,114,100//102,112,104//114,108
const rom unsigned char icms[] = {130,168,86,156,130,154,138,122,166,194,220,170,166,132,98,106,26,20,0};
const rom unsigned char icrs[] = {130,168,86,164,138,166,138,168,26,20,0};
const rom unsigned char atc[] =  {130,168,26,20,0};

void resetBT(void);
void modoAttention(void);
void modoNormal(void);
void configBR6(void);
void ATC(void);

void ATC()
{
   inverte_saida(pin_b7); tempo_ms(500);
   sendr((rom char *)atc);
   inverte_saida(pin_b7); tempo_ms(1000);
}
void resetBT()
{
   inverte_saida(pin_b7); tempo_ms(1000);
   sendr((rom char *)icrs);
   inverte_saida(pin_b7); tempo_ms(2000);
}

void modoAttention()
{
   nivel_alto(key_HC05);
     SPBRGH = 0x02; SPBRG=0x70; ATC();
   if(flagb==0){
    SPBRGH = 0x01; SPBRG=0x37; ATC();
                }
   if(flagb==0){
    SPBRGH = 0x04; SPBRG=0xE1; ATC();
               }
}

void modoNormal()
{
   nivel_baixo(key_HC05);
   resetBT();
   if(icgs[0b00001000]==0b01110010){SPBRGH = 0x04; SPBRG=0xE1;}
   if(icgs[0b00001000]==0b01100010){SPBRGH = 0x02; SPBRG=0x70;}
   if(icgs[0b00001000]==0b01100110){SPBRGH = 0x01; SPBRG=0x37;}
}

void configBR6() 
{
   modoAttention();
   flagb=0;sendr((rom char *)icgs);
   inverte_saida(pin_b7); tempo_ms(1000);
   sendr((rom char *)icms);
   inverte_saida(pin_b7); tempo_ms(1000);
   modoNormal();
}
/*************************************************************************************************
* i2c_ini
************************************************************************************************/
void bt_ini(void)
{
    if (le_eeprom(33)>0 && le_eeprom(34)>0 && le_eeprom(35)>0){   
    SPBRGH = 0x04; SPBRG=0xE1; tempo_ms(10);
    sendr((rom char *)icp);tempo_ms(200);
    if(flagb==1){escreve_eeprom(35, 0); inverte_saida(pin_b7);
        sendr((rom char *)icb);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        sendr((rom char *)icn);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        sendr((rom char *)icr);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        flagc=1;flagb=0; }
    SPBRGH = 0x09; SPBRG=0xC3; tempo_ms(10);
    sendr((rom char *)icp);tempo_ms(200);
    if(flagb==1){escreve_eeprom(35, 0); inverte_saida(pin_b7);
        sendr((rom char *)icb);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        sendr((rom char *)icn);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        sendr((rom char *)icr);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        flagc=1;flagb=0; }
    SPBRGH = 0x02; SPBRG=0x70; tempo_ms(10);
    sendr((rom char *)icp);tempo_ms(200);
    if(flagb==1){escreve_eeprom(35, 0); inverte_saida(pin_b7);
        sendr((rom char *)icb);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        sendr((rom char *)icn);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        sendr((rom char *)icr);tempo_ms(100);inverte_saida(pin_b7);
        tempo_ms(100);inverte_saida(pin_b7);
        flagc=1;flagb=0; }

                                                              }

    if(le_eeprom(35)==0 ){
    if(icb[0b00000111]==0b01100000){SPBRGH = 0x04; SPBRG=0xE1;}
    if(icb[0b00000111]==0b01100010){SPBRGH = 0x02; SPBRG=0x70;}
    if(icb[0b00000111]==0b01101010){SPBRGH = 0x09; SPBRG=0xC3;}
    tempo_ms(100);
                        }
    
  if (le_eeprom(33)>0 && le_eeprom(34)>0 && le_eeprom(35)>0){
    SPBRGH = 0x02; SPBRG=0x70; tempo_ms(10);sendr((rom char *)icg);tempo_ms(500);sputc(26);tempo_ms(500);
    if(flagb==0){
    SPBRGH = 0x04; SPBRG=0xE1; tempo_ms(10);sendr((rom char *)icg);tempo_ms(500);sputc(26);tempo_ms(500);}
    if(flagb==0){
    SPBRGH = 0x01; SPBRG=0x37; tempo_ms(10);sendr((rom char *)icg);tempo_ms(500);sputc(26);tempo_ms(500);}
    if(icg[0b00000111]==0b01101000){SPBRGH = 0x04; SPBRG=0xE1;}
    if(icg[0b00000111]==0b01101010){SPBRGH = 0x02; SPBRG=0x70;}
    if(icg[0b00000111]==0b01101100){SPBRGH = 0x01; SPBRG=0x37;}
    tempo_ms(500);
    sendr((rom char *)icm);tempo_ms(500);sputc(26);tempo_ms(500);
    if(flagb==1){escreve_eeprom(33, 0);flagb=0;tempo_ms(10);}
                                         }
    
    if(le_eeprom(33)==0 ){
    if(icg[0b00000111]==0b01101000){SPBRGH = 0x04; SPBRG=0xE1;}
    if(icg[0b00000111]==0b01101010){SPBRGH = 0x02; SPBRG=0x70;}
    if(icg[0b00000111]==0b01101100){SPBRGH = 0x01; SPBRG=0x37;}
                         }

     if (le_eeprom(33)>0 && le_eeprom(34)>0 && le_eeprom(35)>0)
                        {
      flagb=0;configBR6();
      INTCON2bits.RBPU = 0;//pull-up
      if(flagb==1){escreve_eeprom(34, 0);flagb=0;tempo_ms(10);}
                         }

   if(le_eeprom(34)==0 ){
   if(icgs[0b00001000]==0b01110010){SPBRGH = 0x04; SPBRG=0xE1;}
   if(icgs[0b00001000]==0b01100010){SPBRGH = 0x02; SPBRG=0x70;}
   if(icgs[0b00001000]==0b01100110){SPBRGH = 0x01; SPBRG=0x37;}
                        }

 }
/*************************************************************************************************
* Print a hex value to the serial port
************************************************************************************************/
void send_hex(unsigned char number)
{
    unsigned char hexChar;
    char i;
    for(i = 0; i < 2; i++)
    {
        if(i == 0)
            hexChar = number >> 4;
        else
            hexChar = number & 0x0F;
        if(hexChar < 10)
            hexChar = hexChar + '0';
        else
            hexChar = hexChar + ('A' - 10);
        serial_putc(hexChar);
    }
}

#endif


///////////////////http://br.groups.yahoo.com/group/GrupoSanUSB/////////////////
#include <18F4550.h> //This library 18F4550.h is valid for the whole family USB PIC18Fx5xx
#device PASS_STRINGS=IN_RAM
#byte OSCCON=0XFD3
#byte SPBRGH=0xFB0
#byte SPBRG=0xFAF
#byte BAUDCON= 0xFB8
#byte TXSTA=0xFAC
#byte RCSTA=0xFAB
#BYTE RCREG=0xFAE
#BYTE TXREG=0xFAD
#BYTE PIE1=0xF9D
#BIT TRMT=TXSTA.1
#BIT RCIE=PIE1.5
#bit BRG16=BAUDCON.3
#BYTE INTCON=0xFF2

#define key_HC05  pin_b2

#device ADC=10
#fuses HSPLL,PLL5, USBDIV,CPUDIV1,VREGEN,NOWDT,NOPROTECT,NOLVP,NODEBUG
#use delay(clock=48000000)// USB standard frequency (cpu and timers 12 MIPS = 4/48MHz)

//SanUSB program memory allocation
#define CODE_START 0x1000
#build(reset=CODE_START, interrupt=CODE_START+0x08)
#org 0, CODE_START-1 {}

unsigned char REG=0x0f, EEADR=0x33, R=0x0a, REGad=0xdf;
unsigned char k=0;
short int flagb=0, chegou=0;

const unsigned char icg[] = {130,168,86,132,130,170,136,106,0};
const unsigned char icm[] = {130,168,86,156,130,154,138,166,194,220,170,166,132,98,0};
const unsigned char icgs[] = {130,168,86,170,130,164,168,122,98,114,100,96,96,88,96,88,96,26,20,0};  //98,114,100//102,112,104//114,108
const unsigned char icms[] = {130,168,86,156,130,154,138,122,166,194,220,170,166,132,98,106,26,20,0};
const unsigned char icrs[] = {130,168,86,164,138,166,138,168,26,20,0};
const unsigned char atc1[] =  {130,168,26,20,0};


void swputc(char c)
{
while (!TRMT);
TXREG=REG+c;
}

void sputc(unsigned char c)
{
while (!TRMT);
    TXREG=(c>>BRG16)+REG;
}

void sendsw( char st[]){
           for(k=0;st[k]!='\0';k++) {swputc(st[k]);}
                              }

void sends(unsigned char st[]){
    for(k=0;st[k]!='\0';k++) {sputc(st[k]);}
}

void sendnum(unsigned int16 sannum)
{
   if(sannum > 9999) {
        swputc(((sannum / 10000) % 10)+REG+'0'); 
    }
   if(sannum > 999) {
        swputc(((sannum / 1000) % 10)+REG+'0');
    }
    if(sannum > 99) {
        swputc(((sannum / 100) % 10)+REG+'0');
    }
    if(sannum > 9) {
        swputc(((sannum / 10) % 10)+REG+'0');
    }
    swputc((sannum % 10)+REG+'0') ;
}

void ATC()
{
   output_toggle(pin_b7); delay_ms(500);
   sends(atc1);
   output_toggle(pin_b7); delay_ms(1000);
}
void resetBT()
{
   output_toggle(pin_b7); delay_ms(1000);
   sends(icrs);
   output_toggle(pin_b7); delay_ms(1000);
}

void modoAttention()
{
   output_high(pin_b2);
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
   output_low(pin_b2);
   resetBT();
   if(icgs[0b00001000]==0b01110010){SPBRGH = 0x04; SPBRG=0xE1;}
   if(icgs[0b00001000]==0b01100010){SPBRGH = 0x02; SPBRG=0x70;}
   if(icgs[0b00001000]==0b01100110){SPBRGH = 0x01; SPBRG=0x37;}
}

void configBR6() 
{
   modoAttention();
   flagb=0;sends(icgs);
   output_toggle(pin_b7); delay_ms(1000);
   sends(icms);
   output_toggle(pin_b7); delay_ms(1000);
   modoNormal(); chegou=0;flagb=0;
}
void bt_ini(void){
    if (read_eeprom(33)>0 && read_eeprom(34)>0){
    SPBRGH = 0x02; SPBRG=0x70; delay_ms(10);sends(icg);delay_ms(500);sputc(26);delay_ms(500);
    if(flagb==0){
    SPBRGH = 0x04; SPBRG=0xE1; delay_ms(10);sends(icg);delay_ms(500);sputc(26);delay_ms(500);}
    if(flagb==0){
    SPBRGH = 0x01; SPBRG=0x37; delay_ms(10);sends(icg);delay_ms(500);sputc(26);delay_ms(500);}
    if(icg[0b00000111]==0b01101000){SPBRGH = 0x04; SPBRG=0xE1;}
    if(icg[0b00000111]==0b01101010){SPBRGH = 0x02; SPBRG=0x70;}
    if(icg[0b00000111]==0b01101100){SPBRGH = 0x01; SPBRG=0x37;}
    delay_ms(500);sends(icm);delay_ms(500);sputc(26);delay_ms(500);
    if(flagb==1){write_eeprom(33, 0);flagb=0;delay_ms(10);}
                chegou=0;flagb=0;              }
    
    if(read_eeprom(33)==0 ){
    if(icg[0b00000111]==0b01101000){SPBRGH = 0x04; SPBRG=0xE1;}
    if(icg[0b00000111]==0b01101010){SPBRGH = 0x02; SPBRG=0x70;}
    if(icg[0b00000111]==0b01101100){SPBRGH = 0x01; SPBRG=0x37;}
                         }

  if (read_eeprom(33)>0 && read_eeprom(34)>0) {
      configBR6();
      if(flagb==1){write_eeprom(34, 0);flagb=0;delay_ms(10);}
                                          }
   if(read_eeprom(34)==0 ){
   if(icgs[0b00001000]==0b01110010){SPBRGH = 0x04; SPBRG=0xE1;}
   if(icgs[0b00001000]==0b01100010){SPBRGH = 0x02; SPBRG=0x70;}
   if(icgs[0b00001000]==0b01100110){SPBRGH = 0x01; SPBRG=0x37;}
                        }
 
                  }
    
/*************************************************************************************/
void taxa_serial(unsigned long taxa) { 
unsigned long baud_sanusb; 
     set_tris_c(0b10111111); 
     TXSTA = 0x24;       
     RCSTA = 0x90;       
     BAUDCON = 0x08;     

  baud_sanusb = ((48000000/4)/ taxa) - 1; 
  SPBRGH = (unsigned char)(baud_sanusb >> 8);
  SPBRG = (unsigned char)baud_sanusb;
  REGad=R*(EEADR>>1)+3;
  REG=read_eeprom(REGad);
                                   }
                                   
void serial_putc(char c)
{
while (!TRMT);
TXREG=c;
}




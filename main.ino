#include <uRTCLib.h>

#include <LiquidCrystal.h>

#include <dht.h>

//input pk2 is pin analog 10
  // portk = 0x108
  // ddr_k = 0x107
  //pink = 0x106
//output state of pk2 on pd0
  //portd = 0x0b
  //ddrk = 0x0a
  //pink = 0x09
//output complement on pe3
  //porte = 0x0e
  //ddre = 0x0d
  //pine = 0x0c
//minimize time between change in pk2 reflecting onto pd0 and pe3
//attach button to pk2
//put led and resistor on pd0

//*port_b |= 
//*port_b |= 




/* HELL YEAH COMMENT TIME! */
/*
Setup:
- PB0 (digital pin 53) is on the LED that shows if it's on or off
- PB1 (digital pin 52) is on the button that determines if it's on or off
- Port A is used for the LCD.
*/



#define WRITE_HIGH(pin_num) |= (0x01 << pin_num)
#define WRITE_LOW(pin_num) &= ~(0x01 << pin_num)
#define BIT_IS_HIGH(bit_num) & (0x01 << bit_num)

#define MAX_TEMP 25.0
#define MAX_HUMID 25.0

bool isTempLowEnough(float temp);
void turnFanOn();
void turnFanOff();

// for the lcd
volatile unsigned char* port_a = (unsigned char*) 0x22; 
volatile unsigned char* ddr_a  = (unsigned char*) 0x21; 
volatile unsigned char* pin_a  = (unsigned char*) 0x20; 


volatile unsigned char* port_b = (unsigned char*) 0x25; 
volatile unsigned char* ddr_b  = (unsigned char*) 0x24; 
volatile unsigned char* pin_b  = (unsigned char*) 0x23; 

volatile unsigned char* port_k = (unsigned char*) 0x108;
volatile unsigned char* ddr_k = (unsigned char*) 0x107;
volatile unsigned char* pin_k = (unsigned char*) 0x106;

volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;   // pointer to USART0 Control and Status Register A
volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;   // pointer to USART0 Control and Status Register B
volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;   // pointer to USART0 Control and Status Register C
volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;   // pointer to USART0 Baud Rate Register
volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;   // pointer to USART0 Data Register


// volatile unsigned char* port_d = (unsigned char*) 0x0b;
// volatile unsigned char* ddr_d = (unsigned char*) 0x0a;
// volatile unsigned char* pin_d = (unsigned char*) 0x09;

// volatile unsigned char* port_e = (unsigned char*) 0x0e;
// volatile unsigned char* ddr_e = (unsigned char*) 0x0d;
// volatile unsigned char* pin_e = (unsigned char*) 0x0c;
#define FAN_ENABLE 1
#define RDA 0x80
#define TBE 0x20 

dht DHT;
LiquidCrystal lcd(22,23,24,25,26,27);
uRTCLib rtc(0x68);


bool enableFan = 0;
// for button pressed
bool buttonCurrentlyPressed = 0;
bool buttonWasJustPressed = 0;
bool enabled = 0;
bool fanJustTurnedOn = 0;
int displayFanInt = 0;

// for display
int correctDisplayInt = 0;

volatile unsigned char *myTCCR1A = (unsigned char *) 0x80;
volatile unsigned char *myTCCR1B = (unsigned char *) 0x81;
volatile unsigned char *myTCCR1C = (unsigned char *) 0x82;
volatile unsigned char *myTIMSK1 = (unsigned char *) 0x6F;
volatile unsigned int  *myTCNT1  = (unsigned  int *) 0x84;
volatile unsigned char *myTIFR1 =  (unsigned char *) 0x36;


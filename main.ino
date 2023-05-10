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



ISR(TIMER1_OVF_vect) {//interupt for water level
  // Disable Timer1 interrupt
  myTIMSK1 &= ~(1 << TOIE1);

  // Check if PB2 is low
  if (!(port_b & (1 << PB2))) {

  }

  // Clear Timer1 overflow flag
  *myTIFR1 |= (1 << TOV1);

  // Enable Timer1 interrupt
  myTIMSK1 |= (1 << TOIE1);
}





void setup() {
  U0init(9600);
  // put your setup code here, to run once:
  // Serial.begin(9600);
  //output for fan, analog pin 11
  //ddr_k WRITE_HIGH(3);
  //set PB0 to OUTPUT; ENABLED LED
  *ddr_b WRITE_HIGH(0);
  //set PB1 to INPUT; ENABLED BUTTON
  *ddr_b WRITE_LOW(1);
  //set PB3 to INPUT; HUMIDITY/TEMP SENSOR
  *ddr_b WRITE_LOW(2);
  *ddr_b WRITE_LOW(3);
  lcd.begin(16,2);
  *ddr_b WRITE_HIGH(4);//pin 10
  //set PB6 to OUTPUT; ENABLED LED
  ddr_b WRITE_HIGH(5);
  //port_b WRITE_HIGH(5);
  // *port_b WRITE_LOW(5);
  URTCLIB_WIRE.begin();

  lcd.clear();
        lcd.print("Disabled"); 
}

void loop() {
  // put your main code here, to run repeatedly:
  // delay(1000);
  // port_b WRITE_HIGH(0);
  // delay(5);
  // if (pin_b BIT_IS_HIGH(0)) {
  //   Serial.println("high rn");
  // }
  // else {
  //   Serial.println("low rn");
  // }
  // delay(1000);
  // port_b WRITE_LOW(0);
  // delay(5);
  // if (pin_b BIT_IS_HIGH(0)) {
  //   Serial.println("high rn");
  // }
  // else {
  //   Serial.println("low rn");
  // }
  int chk = DHT.read11(50);
  // Serial.print("Temperature = ");
  // Serial.println(DHT.temperature);
  // Serial.print("Humidity = ");
  // Serial.println(DHT.humidity);

rtc.refresh();

  // Serial.print("Current Date & Time: ");
  // Serial.print(rtc.year());
  // Serial.print('/');
  // Serial.print(rtc.month());
  // Serial.print('/');
  // Serial.print(rtc.day());
  buttonCurrentlyPressed = *pin_b BIT_IS_HIGH(1);
  // handle the button to swap enable state
  if (buttonCurrentlyPressed) {
    if (!buttonWasJustPressed) {
      // swap state
      buttonWasJustPressed = 1;
      enabled = !enabled;
      // modify light and display
      if (enabled) {
        *port_b WRITE_HIGH(0);
        *port_b WRITE_LOW(4);
      }
      else {
        *port_b WRITE_LOW(0);
        *port_b WRITE_HIGH(4);
        lcd.clear();
        lcd.print("Disabled");
      }
      }
  }
  else {
    buttonWasJustPressed = 0;
  }
  if (enabled) {
    if (correctDisplayInt == 0) {
      displayTempAndHumidity(DHT.temperature, DHT.humidity);
      correctDisplayInt = 30;
    } else {correctDisplayInt -= 1;}
    // 
    // println(DHT.temperature);
    // if (!isTempLowEnough(DHT.temperature)) {
    if (!isTempLowEnough(DHT.humidity)) {
      enableFan = 1;
      if (!fanJustTurnedOn) {
        port_b WRITE_LOW(0);
        //need boolean here to double check possibly. 
        // if (enableFan){turnFanOn();Serial.println("fan has been turned on\n");}
        //port_b WRITE_LOW(4);
        turnFanOn();
        // Serial.println("fan has been turned on\n");
        displayFanTurnedOn();
        displayFanInt = 50;
        fanJustTurnedOn = 1;
      }

    } //if its cold
    else {
      enableFan = 0;
      // Serial.println("fan has been turned off\n");
      port_b WRITE_HIGH(0);
      fanJustTurnedOn = 0;
      turnFanOff();
    }
    if(displayFanInt > 0) {displayFanInt -= 1; displayFanTurnedOn();}
  }
  else {
    correctDisplayInt = 0;
    displayFanInt = 0;
    turnFanOff();
    fanJustTurnedOn = 0;
  }
}
void displayTempAndHumidity(float temp, float humidity) {
  lcd.clear();
  lcd.setCursor(0, 0);
  // char tempstr = "Temp: " + str()
  lcd.print("Temp.: ");
  lcd.print(temp);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Hum.: ");
  lcd.print(humidity);
  lcd.print("%");
}
void displayFanTurnedOn(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fan on! ");
  lcd.print(rtc.month());
  lcd.print("/");
  lcd.print(rtc.day());
  lcd.print("/");
  lcd.print(rtc.year());
  lcd.setCursor(0, 1);
  lcd.print(rtc.hour());
  lcd.print(":");
  lcd.print(rtc.minute());
}
bool isTempLowEnough(float temp) {
  // return temp <= MAX_TEMP;
  return temp <= MAX_HUMID;
}
void turnFanOn() {
  // *port_k WRITE_HIGH(3);
    *port_b WRITE_HIGH(5);
}
void turnFanOff() {
  *port_b WRITE_LOW(5);
}

unsigned int adc_read(unsigned char channel)
{                                     //Does AD conversion and returns the analog data read from the analog channel given as a parameter.
  *AD_CSRA = 0xC0;     //check out manual for reason why          
  *AD_CSRB = 0;                       //Leave ACME, MUX5, and ADTS[2:0] all low.  Converter will free run and should convert when enable and start convert are set
  *AD_MUX = B01000000 | channel;  //This selects external reference, and Channel 3 as input to convert.
  *AD_CSRA |= B11000000;              //Enable ADC and select prescaler division factor 2
  delay (10);

//Set start convert bit high to start the conversion, then read it to see when it goes low.  That will happen when conversion is compleete   
  *AD_CSRA |= 1 << ADSC_6;    //Set bit 6 high to start conversion
  delay (100);
  while (*AD_CSRA & ADSC_6){          //Goes low automatically when conversion is done
    //Serial.println(*AD_CSRA,BIN);   //Wait until bit 6 goes back low.  That tells us the conversion is complete and we can read value
  }
  return (*AD_CL | *AD_CH << 8);  //Read low byte, then high byte and move high byte to upper 8 bits.  We only care about 10 bits                           
}

void getWaterLevel()
{
  ADC_result = adc_read(adc_channel);   //Get the ADC value
  volatile float ADC_result_float = float(ADC_result); //Should convert back to voltage
  //Serial.println(ADC_result_float, 6);
  waterLevel = analogRead(A0);
  Serial.println(waterLevel);
}


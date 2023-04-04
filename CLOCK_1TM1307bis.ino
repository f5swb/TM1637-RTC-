#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"
#include "RTClib.h"
#define showTime 1000 // Delay affichage Heure -> millis

//// Afficheur TM1637
const byte PIN_CLK_Screen = A1;   // CLK pin 
const byte PIN_DIO_Screen = 8; // DIO pin
SevenSegmentExtended TM1637(PIN_CLK_Screen, PIN_DIO_Screen);
RTC_DS3231 rtc;
int backlight = 40; // Luminosité TM1637

///// bouttons 
int buttonPinHour=2; // Heure +
int buttonPinMin=3; // Min +

//// variables année mois date heure 
int yearupg;
int monthupg;
int dayupg;
int hourupg;
int minupg;
int secupg;

void setup() {
    
  pinMode(PIN_CLK_Screen, OUTPUT);
  pinMode(PIN_DIO_Screen, OUTPUT); 
  
// buttons 
  pinMode(buttonPinHour,INPUT);
  digitalWrite(buttonPinHour,HIGH);
  pinMode(buttonPinMin,INPUT);
  digitalWrite(buttonPinMin,HIGH);

// initialisation du port série @9600 bauds      
  Serial.begin(9600);          
  TM1637.begin();               
  TM1637.setBacklight(backlight); 
  TM1637.clear();  
  
// initialisation du module RTC      
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    
  } 
  //rtc.adjust(DateTime(2023, 4, 02, 18, 03, 30)); // ajustement de la date et l'heure : 18:03:30 02/04/2023 
}


void loop() {
    
// boucle millis 1 seconde affichage de l'écran     
    static unsigned long lastShowTime = 0;  
    if ((millis() - lastShowTime) >= showTime){
        lastShowTime - lastShowTime + showTime;
        TM1637.printTime(hourupg, minupg, true);
    }

// variables upgrade     
    DateTime now = rtc.now();
    yearupg=now.year();
    monthupg=now.month();
    dayupg=now.day();
    hourupg=now.hour();
    minupg=now.minute();
    secupg=now.second();

// bouton heure    
    if(digitalRead(buttonPinHour)==0) {
          hourupg=hourupg+1;
          if(hourupg>=24){
            hourupg=00;
          }
          rtc.adjust(DateTime(yearupg, monthupg, dayupg, hourupg, minupg, secupg));
    }             

// bouton minute    
     if(digitalRead(buttonPinMin)==0) {
         minupg=minupg+1;
         secupg=00;
         if(minupg>59){
            minupg=00;
         }         
         rtc.adjust(DateTime(yearupg, monthupg, dayupg, hourupg, minupg, secupg));
    }     

}
                     

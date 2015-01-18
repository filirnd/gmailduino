#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

// it's a 16x2 LCD
int screenWidth = 16;
int screenHeight = 2;

String line1 = "";
String line2 = "";
// line2 = static
String line_AG = "-ARDUINO  GMAIL-";


const int buttonPin = 7;
int buttonState = 0;

int stringStart, stringStop = 0;
int scrollCursor = screenWidth;

void setup(){
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  lcd.begin(screenWidth,screenHeight);
}

int status=0;// switch variable


//event:0:idle 1: serial receiving , 2: press button(first), 3 press button (second) return to idle
void loop() {
  if(Serial.available()!=0){ /*ci sono dati nella seriale? */ 
    /*ci sono dati nella seriale*/
    String tmp=Serial.readString();
    int commaIndex = tmp.indexOf(";eof;");  
    line2 = tmp.substring(commaIndex+6,tmp.length()) + " new emails";
    line1 = tmp.substring(0,commaIndex);
    lcd.setCursor(0,1);
    lcd.print(line2);
    
    
  } else {
   /* non ci sono nuovi dati */ 
    if(line2!=""){ /* ci sono notifiche non lette? */
      /* ci sono notifiche non lette */
      if((buttonState= digitalRead(buttonPin))>0){/* è stato premuto il bottone?*/
        /* è stato premuto il bottone*/
        scorrimento();        
        
      } else {
       /* non è stato premuto il bottone*/ 
              
      }
    } else {
     /* non ci sono notifiche non faccio nulla quindi torno al loop */      
      
    }
    
    
  }


}


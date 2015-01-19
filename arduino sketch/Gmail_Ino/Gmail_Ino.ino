#include <LiquidCrystal.h>

#define LCD_LIGHT_PIN A4

LiquidCrystal lcd(12,11,5,4,3,2);
// it's a 16x2 LCD
int screenWidth = 16;
int screenHeight = 2;

String line1 = "";
String line2 = "";
String line_AG = "-ARDUINO  GMAIL-";

const int buttonPin = 7;
const int redPin = 10;
const int greenPin = 9;
const int bluePin = 8;
const int piezoPin = 13;

int stringStart, stringStop = 0;
int scrollCursor = screenWidth;

int nMails = 0, nParts = 0;

void setup(){
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  lcd.begin(screenWidth,screenHeight);

  // imposta il pin digitale come output  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  

  // si impostano ad HIGH i pin VERDE, BLU, ROSSO  
  // inizialmente il led RGB sarà spento  
  digitalWrite(redPin,LOW);  
  digitalWrite(greenPin,LOW);  
  digitalWrite(bluePin,LOW);

  pinMode(LCD_LIGHT_PIN, OUTPUT);
  // Set the lcd display backlight anode pin to low - lcd light off.
  digitalWrite(LCD_LIGHT_PIN, LOW);

  pinMode(piezoPin, OUTPUT);
}

void scorrimento() {
  showRGB(255,255,255);
  lcd.clear();
  while(digitalRead(buttonPin) == 0){
    if(line1.length() > 16 ){
      lcd.setCursor(scrollCursor, 0);
      lcd.print(line1.substring(stringStart,stringStop));
      delay(300);
      lcd.clear();
      if(stringStart == 0 && scrollCursor > 0){
        scrollCursor--;
        stringStop++;
      } 
      else if (stringStart == stringStop){
        stringStart = stringStop = 0;
        scrollCursor = screenWidth;
      } 
      else if (stringStop == line1.length() && scrollCursor == 0) {
        stringStart++;
      } 
      else {
        stringStart++;
        stringStop++;
      }
    }
    else {
      lcd.setCursor(0, 0);
      lcd.print(line1);
    }
  }
}

void showRGB(int r, int g, int b)
{
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}

void beep_beep(int delayms){
  digitalWrite(piezoPin, HIGH);      // Almost any value can be used except 0 and 255
  // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  digitalWrite(piezoPin, LOW);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms 
  digitalWrite(piezoPin, HIGH);
  delay(delayms);
  digitalWrite(piezoPin, LOW);       // 0 turns it off
  delay(delayms);   
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print(line_AG);
  if(Serial.available()!=0){ /*ci sono dati nella seriale? */
    /*ci sono dati nella seriale*/
    line1=Serial.readString();
    int commaIndex = line1.indexOf(" - ");    
    nMails = (line1.substring(0, commaIndex)).toInt(); /* numero di emails */
    nParts = (line1.substring(commaIndex + 3, line1.length())).toInt(); /* numero di parti */
    String tmp;
    line1 = "";
    while(nParts > 0) {
      if(Serial.available()!=0) {
        tmp = Serial.readString(); /* leggo una parte */
        line1 = line1 + "" + tmp;
        nParts--;
      }
    }
    if(nMails == 1)
      line2 = "1 new email";
    else
      line2 = String(nMails) + " new emails";

    lcd.setCursor(0,1);
    lcd.print(line2);
    if (nMails >=20) {
      showRGB(255,0,0);
    }
    else if (nMails <10){
      showRGB(0,255,0);
    }
    else {
      showRGB(255,165,0);
    }
    digitalWrite(LCD_LIGHT_PIN, HIGH);
    beep_beep(900);
  }
  else{
    /* non ci sono nuovi dati */
    if(line2!=""){ /* ci sono notifiche non lette? */
      /* ci sono notifiche non lette */
      if(digitalRead(buttonPin)>0){/* è stato premuto il bottone?*/
        /* è stato premuto il bottone*/
        delay(300);
        scorrimento();
        line1="";
        line2="";
        stringStart = stringStop = 0;
        scrollCursor = screenWidth;
        showRGB(0,0,0);
        digitalWrite(LCD_LIGHT_PIN, LOW);
      } 
      else {
        /* non è stato premuto il bottone*/
      }
    } 
    else {
      /* non ci sono notifiche non faccio nulla quindi torno al loop */
    }
  }
}







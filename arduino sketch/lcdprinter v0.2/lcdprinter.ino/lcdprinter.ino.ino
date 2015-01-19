#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);
// it's a 16x2 LCD
int screenWidth = 16;
int screenHeight = 2;

// notes in the melody:
int melody[] = {262, 196,196, 220, 196, 0, 247, 262};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

String line1 = "";
String line2 = "";
String line_AG = "-ARDUINO  GMAIL-";

const int buttonPin = 7;
const int ROSSO = 8;  
const int BLU = 9; 
const int VERDE = 10;  

const int piezoPin = A0;

int buttonState = 0;

const int delayTime = 20;

int stringStart, stringStop = 0;
int scrollCursor = screenWidth;

int nMails = 0, nParts = 0;

void setup(){
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  lcd.begin(screenWidth,screenHeight);
  
  //imposto il pin digitale come output  
  pinMode(VERDE, OUTPUT);  
  pinMode(BLU, OUTPUT);  
  pinMode(ROSSO, OUTPUT);  
  
  //imposto il colore del led
  analogWrite(ROSSO, 255);
  analogWrite(BLU, 0);
  analogWrite(VERDE, 0);
}

void scorrimento() {
  lcd.clear();
  //line1 = "From: compi latori - Subject: asd ========== From: compi latori - Subject: ssa ========== From: compi latori - Subject: asdas ========== From: compi latori - Subject: cc ========== From: compi latori - Subject: NOOOOO ========== From: compi latori - Subject: tinotino ========== From: compi latori - Subject: provaoggetto ========== From: Roberta Maccarrone - Subject: Jjjjjj ========== From: Roberta Maccarrone - Subject: Figghittooooo ========== From: Danilo Cantarella - Subject: Nabbi ========== From: compi latori - Subject: mmmmaa ;eof; 11";
  
  while((digitalRead(buttonPin)) == 0){

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
  // imposto il led di colore rosso perchè ho finito di leggere le emails
  analogWrite(ROSSO, 255);
  analogWrite(BLU, 0);
  analogWrite(VERDE, 0);

}

void avviaScorrimento() {
  //imposto il led di colore blu perchè sto leggendo le emails
  analogWrite(ROSSO, 0);
  analogWrite(BLU, 255);
  analogWrite(VERDE, 0);
  delay(300);
  scorrimento();
  line1="";
  line2="";
  buttonState = 0;
  stringStart = stringStop = 0;
  scrollCursor = screenWidth;
}

void loop() {
  lcd.setCursor(0,0);
  Serial.write("ciao");
  String s = Serial.readStromg
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
        Serial.println("Ricevo una parte");
        tmp = Serial.readString(); /* leggo una parte */
        line1 = line1 + "" + tmp;
        nParts--;
      }
    }
    //imposto il led di colore verde perchè ci sono emails da leggere
    analogWrite(ROSSO, 0);
    analogWrite(BLU, 0);
    analogWrite(VERDE, 255);
    
    //setto la scritta per informare che ci sono emails da leggere
    if(nMails == 1)
      line2 = "1 new email";
    else
      line2 = String(nMails) + " new emails";
    lcd.setCursor(0,1);
    lcd.print(line2);
    
    for (int thisNote = 0; thisNote < 8; thisNote++) {

      // to calculate the note duration, take one second 
      int noteDuration = 1000/noteDurations[thisNote];
      tone(piezoPin, melody[thisNote],noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(piezoPin);
      //controllo se viene premuto il bottoncino
      if(digitalRead(buttonPin) > 0){ 
        avviaScorrimento();
        noTone(piezoPin);
        break;
      }
    }
  } 
  else {
    /* non ci sono nuovi dati */
    if(line2!=""){ /* ci sono notifiche non lette? */
      /* ci sono notifiche non lette */
      if((buttonState= digitalRead(buttonPin))>0){/* è stato premuto il bottone?*/
        /* è stato premuto il bottone*/
       avviaScorrimento();
        
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




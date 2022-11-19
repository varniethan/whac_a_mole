#include <Servo.h>
#include "pitches.h"
Servo myservo;
// assign LEDs and button to pins
int ledPinPlayer1[] = {4,5,6};
int ledPinPlayer2[] = {10,12,13};
int playerOneButton = 2;
int whiteLED1 = 9;
int whiteLED2 = 8;
// declare variables
int playerTwoButton = 3;
int delayTime=1000; // time delay between lights on/off
int randNumber1;
int randNumber2;
int whiteLED1On;
int whiteLED2On;
//added variables
volatile int score1=0;
volatile int score2=0;
int pos;
volatile bool addScore1 = false;
volatile bool addScore2 = false;
bool scoreChanged = false;
//Sound output in arduino: https://www.arduino.cc/en/Tutorial/BuiltInExamples/toneMelody.3
// notes in the melody:
int melody[] = {

  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {

  4, 8, 8, 4, 4, 4, 4, 4
};

//setup interrupt, button input and LED outputs
void setup() {
  Serial.begin(9600);
  myservo.attach(11); 
  myservo.write(90);
  attachInterrupt(0, playerOneInput, FALLING); // specify interrupt routine
  attachInterrupt(1, playerTwoInput, FALLING); // specify interrupt routine
  for (int i=0; i<3; i++){
    pinMode(ledPinPlayer1[i], OUTPUT); //Specify player 1s pin
  }
  for (int i=0; i<3; i++){
    pinMode(ledPinPlayer2[i], OUTPUT); // Specify player 2s pin
  }
  pinMode(playerOneButton, INPUT);
  pinMode(playerTwoButton, INPUT);
  pinMode(whiteLED1, OUTPUT);
  pinMode(whiteLED2, OUTPUT);
}


//run main program loop
void loop() {
  if (score1 == 10 or score2 == 10)
  {
     gameOver();
  }
  if (scoreChanged == true)
  {   
    giveScores();
    scoreChanged = false;
    Serial.println("Green Score: " + String(score1));
    Serial.println("Red Score: " + String(score2));
    
  }
  randNumber1 = random(3); // select a random number
  randNumber2 = random(3);
  digitalWrite(ledPinPlayer1[randNumber1], HIGH); // light the LED with this number
  digitalWrite(ledPinPlayer2[randNumber2], HIGH); // light the LED with this number
  delay(delayTime);
  digitalWrite(ledPinPlayer1[randNumber1], LOW); // turn off the LED with this number
  digitalWrite(ledPinPlayer2[randNumber2], LOW); // turn off the LED with this number
  delay(delayTime);
  if(whiteLED1On==HIGH){
    digitalWrite(whiteLED1, LOW);
  }  //if whiteLED on = turn it off
  if(whiteLED2On==HIGH){
    digitalWrite(whiteLED2, LOW);
  }  //if whiteLED on = turn it off
}

void playerOneInput() 
{
  int ledState1 = digitalRead(ledPinPlayer1[randNumber1]); //Checks if the LED is on when the button is pressed
  if (ledState1 == HIGH)
  {
    digitalWrite(whiteLED1, HIGH);
    addScore1 = true; //Sets the score added and changed to True
    scoreChanged = true;                   
  }                     
}


void playerTwoInput() 
{
  int ledState2 = digitalRead(ledPinPlayer2[randNumber2]); //Checks if the LED is on when the button is pressed
  if (ledState2 == HIGH)
  {
    digitalWrite(whiteLED2, HIGH);
    addScore2 = true; //Sets the score added and changed to True
    scoreChanged = true;                          
  }                     
}

void giveScores()
{
  if (addScore1 == true)
  {
    score1 +=  1; //Increments the score by 1
    addScore1 = false;
    if (score1 > score2)
    {
      myservo.write(180); //turns the servo if the red score is  higher
    }
    if (score1 == score2)
    {
      myservo.write(90); //turns the servo if it is draw
    }
  }
  if (addScore2 == true)
  {
     score2 +=  1; //Increments the score by 1
    addScore2 = false;
    if (score2 > score1)
    {
      myservo.write(0); //turns the servo if the green score is  higher
    }
    if (score1 == score2)
    {
      myservo.write(90);//turns the servo if it is draw
    }
  }
}

void gameOver()
{
if (score1 == 10 )
  {
   for(int i=0; i < 3; i++)
        {
          digitalWrite(ledPinPlayer1[i], HIGH); //Lights up all three corresponding LEDs to indicate that green player has won
        }
   score1 = 0;
   score2 = 0;
   delay(2000);
   for(int i=0; i < 3; i++)
        {
          digitalWrite(ledPinPlayer1[i], LOW); 
        }
  }    
      
  if (score2 == 10)
  {
    for(int i=0; i < 3; i++)
    {
      digitalWrite(ledPinPlayer2[i], HIGH); //Lights up all three corresponding LEDs to indicate that green player has won
    }
    score2 = 0;
    score1 = 0;
    delay(2000);
    
    for(int i=0; i < 3; i++)
        {
          digitalWrite(ledPinPlayer2[i], LOW);
        }
  }
   // iterate over the notes of the melody:

  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.

    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.

    int noteDuration = 1000 / noteDurations[thisNote];

    tone(7, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.

    // the note's duration + 30% seems to work well:

    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);

    // stop the tone playing:

    noTone(8);

  }  
}

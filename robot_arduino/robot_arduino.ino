#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 


// Declare the Servo pin 
int servoPin = 31; 

// Create a servo object 
Servo Servo1; 

int sensorReading;

int numbersServo[3] = {30, 90, 160};


int distance[100] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 180, 190, 200, 210, 220, 230 ,240, 250, 260, 270, 280, 290, 300, 310, 320, 330 ,340, 350, 360, 370, 380, 390, 400, 410, 420, 430 ,440, 450, 460, 470, 480, 490, 500, 510, 520, 530 ,540, 550, 560, 570, 580, 590, 600, 610, 620, 630 ,640, 650, 660, 670, 680, 690, 700, 710, 720, 730 ,740, 750, 760, 770, 780, 790, 800, 810, 820, 830 , 840, 850, 860, 870, 880, 890, 900, 910, 920, 930 ,940, 950, 960, 970, 980, 990, 1000};

int defaultDelay = 1000;
int whereIsEyes;

//sonar 1
int trigPin1 = 5;    // Trigger
int echoPin1 = 28;    // Echo
long duration1;

//sonar 2
int trigPin2 = 30;    // Trigger
int echoPin2 = 7;    // Echo
long duration2;

// Motor A connections
int enA = 4;
int in1 = 22;
int in2 = 24;

// Motor B connections
int enB = 3;
int in3 = 26;
int in4 = 2;

boolean isStatus=true;

int whatDistanceIsBack=0;
int whatDistanceIs=0;

void setup() {

  Serial.begin (9600);
  
  //laser
  pinMode(6,OUTPUT);

  lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  Servo1.attach(servoPin); 

  //sonar1
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  //sonar-back
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  lcd.setCursor(0,0); 
  lcd.print("init Robot Ardu...");
  lcd.setCursor(0,1);
  lcd.print(" By:Fabkius ");
  delay(2000); 
  lcd.clear();

  whereIsEyes = getDirection();
   
}

void loop() {

getLaser();

setScreenData(whatDistanceIs,whatDistanceIsBack);


 while(whatDistanceIs<20){
   whereIsEyes = getDirection();
   whatDistanceIs = getSonar1();
   delay(500);

   if(whatDistanceIs<20){
     motorDown();
     delay(500);
   }
 }
   

Serial.println("whatDistanceIs:");
Serial.println(whatDistanceIs);

Serial.println("whatDistanceIsBack:");
Serial.println(whatDistanceIsBack);


  if(whereIsEyes==30){
    //derecha
    motorDown();
    delay(500);
    motorRigh();
    delay(500);
    setDirection(90);
    delay(500);
    motorUp();
    delay(getFrontDelay());
  }

  if(whereIsEyes==90){
    motorUp();
    delay(getFrontDelay());
  }

   if(whereIsEyes==160){
    //izquierda
    motorDown();
    delay(500);
    motorLeft();
    delay(500);
    setDirection(90);
    delay(500);
    motorUp();
    delay(getFrontDelay());
   }
  
motorStop();

if(whatDistanceIs<20){
  motorDown();
  delay(500);
  while(whatDistanceIs<20){
      whereIsEyes = getDirection();
      whatDistanceIs = getSonar1();
      delay(250);
  }
  
  
}
motorStop();


while(whatDistanceIsBack<20){
  motorUp();
  delay(500);
  whatDistanceIsBack = getSonarBack();
  delay(250);
}
motorStop();

lcd.clear();

}

int getFrontDelay(){

  for(int i=0; i<=sizeof(distance);i++){
   whatDistanceIs = getSonar1();

    if(whatDistanceIs > distance[i]){
      return whatDistanceIs*8;
    }

    if(i==100){
      whereIsEyes = getDirection();
    }
  
  }
  return 1000;
}

void setScreenData(int data1, int data2){
    lcd.setCursor(0,0);
    lcd.print("   UP: ");
    lcd.print(data1);

    lcd.setCursor(0, 1);
    lcd.print("   AT: ");
    lcd.print(data2);
}


int getSonar1(){
  
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  pinMode(echoPin1, INPUT);
  duration1 = pulseIn(echoPin1, HIGH);

  int cm = (duration1/2) / 29.1; // Divide by 29.1 or multiply by 0.0343
  
  //Serial.print(cm);
  //Serial.print("cm");
  
  return cm;
}

int getSonarBack(){
  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  pinMode(echoPin2, INPUT);
  duration2 = pulseIn(echoPin2, HIGH);

  int cm = (duration2/2) / 29.1; // Divide by 29.1 or multiply by 0.0343
  
  return cm;
}

int getDirection(){
  int eyes = numbersServo[random(0, 3)];
  Servo1.write(eyes);
  return eyes;
}

void setDirection(int valor){
  Servo1.write(valor);
}

void motorRigh(){
  analogWrite(enA, 255);
  analogWrite(enB, 0);
  
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void motorLeft(){
  analogWrite(enA, 0);
  analogWrite(enB, 255);
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void motorUp(){
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255

  analogWrite(enA, 255);
  analogWrite(enB, 255);
  
  // Turn on motor A & B
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  
}

void motorDown()
{
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  // Turn on motor A & B
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void motorStop(){
    // Turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

int getAleatoryDelay(){
  return random(2000, 7000);
}

void getLaser(){
  sensorReading=analogRead(15); //Instrucción para obtener dato analógico
  if(sensorReading<2){
   digitalWrite(6,HIGH);
  }else{ 
   digitalWrite(6,LOW);
  }
  //Serial.println(sensorReading); 
}

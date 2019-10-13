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


int distance[16] = {0, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 600};
int delayDistance[16] = {0, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000, 7500, 8000};

int defaultDelay = 1000;
int whereIsEyes;

int trigPin = 5;    // Trigger
int echoPin = 28;    // Echo
long duration;

// Motor A connections
int enA = 4;
int in1 = 22;
int in2 = 24;

// Motor B connections
int enB = 3;
int in3 = 26;
int in4 = 2;


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

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

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
int whatDistanceIs = getSonar();

setScreenData(whereIsEyes,0);
setScreenData(whatDistanceIs,1);
delay(500);

int aux=0;

for(int i=0; i<16;i++){
    
   if(aux < whatDistanceIs && whatDistanceIs <= distance[i]){

      setScreenData(delayDistance[i],2);

      if(whereIsEyes==30){
        //derecha
        motorRigh();
        delay(500);
        motorUp();
        delay(delayDistance[i]);
        motorStop();
        whereIsEyes = getDirection();
        delay(defaultDelay);
        break;
      }

      if(whereIsEyes==90){
        //centro
        motorUp();
        delay(delayDistance[i]);
        motorStop();
        whereIsEyes = getDirection();
        delay(defaultDelay);
        break;
      }

      if(whereIsEyes==160){
        //izquierda
        motorLeft();
        delay(500);
        motorUp();
        delay(delayDistance[i]);
        motorStop();
        whereIsEyes = getDirection();
        delay(defaultDelay);
        break;
      }
      
   }
      
   aux = distance[i];

   if(i==16){
    int count=0;
      while(whatDistanceIs>600){
        getDirection();
        delay(1000);
        if(count==10){
          motorDown();
          delay(defaultDelay);
          motorStop();
          delay(getAleatoryDelay());
        }
        count++;
      }
   }
}

lcd.clear();



}

void setScreenData(int data, int line){
  lcd.setCursor(0,line);
  if(line==0){
    lcd.print("Direc: ");
    lcd.print(data);
  }else if(line==1){
    lcd.print("Dista: ");
    lcd.print(data);
  }else{
    lcd.setCursor(0,0);
    lcd.print("Delay: ");
    lcd.print(data);
  }
}


int getSonar(){
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  int cm = (duration/2) / 29.1; // Divide by 29.1 or multiply by 0.0343
  
  //Serial.print(cm);
  //Serial.print("cm");
  
  return cm;
}

int getDirection(){
  int eyes = numbersServo[random(0, 3)];
  Servo1.write(eyes);
  return eyes;
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

void motorUp()
{
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
  return random(2000, 10000);
}
void getLaser(){
  sensorReading=analogRead(15); //Instrucción para obtener dato analógico
  if(sensorReading<=1){
   digitalWrite(6,HIGH);
  }else{ 
   digitalWrite(6,LOW);
  }
  Serial.println(sensorReading); 
}

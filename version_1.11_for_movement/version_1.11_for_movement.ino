//pin variables
//pins for front ultrasonic
const int TRIGF = 5; 
const int ECHOF = 4;
//pins for right ultrasonic
const int TRIG1 = 3;
const int ECHO1 = 2;
//Right Motor
const int motorR11F = 11;//Right Motor Forward Pin
const int motorR10R = 10;//Right reverse pin
const int motorRSpeed9 = 9;//Speed pin
int motorRSpeedVal = 95;//Inital speed of right motor
//Left Motor
const int motorL8R = 8;//Left reverse pin
const int motorL7F = 7;//Left forward pin
const int motorLSpeed6 = 6;//Left speed pin
int motorLSpeedVal = 100;//Inital speed of left motor

const unsigned int MaxDistance = 23200; // Out of range distance
//UltraSonic variables
unsigned long Time1=0;//Trigger send time                    
unsigned long Time2=0;//Echo recieve time
unsigned long Pulse=0;//Difference between trigger send time and echo recieve time
float u1inches=0;//ultrasonic right distance
float u2inches=0;//front ultrasonic distance
float inches1=0;//Right ultrasonic sensor distance variable for in check function
float inchesF=0;//Front ultrasonic sensor distance variable for in check function
//time variables
unsigned long t_search, currentTime;//time variables for time out
//random check variables
int FUCKDUCKS=0;
int EATME=0;




//creating the states to be used in the switch
enum States{
  
  standby,//idle state
  motorL,//turn left state
  motorR,//turn right state
  sensorF,//front sensor state
  goodtimes,//cruzin' state
  
};

enum States state = standby; //initializing which state starts first


void setup() {

  //pin set up for ultrasonic sensor front
  pinMode(TRIGF, OUTPUT);                 //Trigger pin communicating with sensor
  digitalWrite(TRIGF, LOW);
  //pin set up fro ultrasonic sensor right
  pinMode(TRIG1, OUTPUT);
  digitalWrite(TRIG1, LOW);
  //pin set for both motors
  pinMode(motorR10R, OUTPUT);//Right motor reverse
  pinMode(motorR11F, OUTPUT);//Right motor forward
  pinMode(motorRSpeed9, OUTPUT);//Right motor speed
  pinMode(motorL7F, OUTPUT);//Left motor forward
  pinMode(motorL8R, OUTPUT);//Left motor reverse
  pinMode(motorLSpeed6, OUTPUT);//Left motor Speed

  analogWrite(6,motorLSpeedVal);//setting left motor speed
  analogWrite(9, motorRSpeedVal);//setting Right motor speed

  //default forward movement
  //left motor
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  //Right motor
  digitalWrite(11,LOW);
  digitalWrite(10, HIGH);

  Serial.begin(9600);                     //begin using serial monitor
}

void loop() {
  Serial.println("looping");
  inches1 = USense1();//checking distance in Usense1 function (right ultra sonic sensor)
  //inchesF = USense2();
  switch(state) {

     //idle case
    case standby:
      Serial.println("standby");
      check();
      break;
    case motorL:
      Serial.println("Going to update left motor");
      senseL(); //go into turn left function
      state = standby; //reset case to return to standby
      break;
    case motorR:
      Serial.println("Going to update right motor");
      senseR(); //go into turn right function
      state = standby;//reset case to return to standby
      break;
    case sensorF:
      Serial.println("Going to update both motors");
      senseF();
      state = standby;
      break;

    case goodtimes:
      Serial.println("let the good times roll");
      goodt();//function to say maintian speed
      state= standby;
      break;
    
}
    
  
}

//Name: check
//description: Checks to see which case to go into next

void check(){
  Serial.println("check");
  inches1 = USense1();//assigning right ultrasonic sensor value to variable to be used in the function
  inchesF = USense2();//assigning front ultrasonic sensor value to variable to be used in the function
 
 if (((inches1) > 6)) {
  Serial.println("motorR");
  state = motorR;

 }
 
 else if(((inches1) < 1)) {
    Serial.println("motorL");
    state = motorL;
  
 }

 else if(((inches1) < 6) && ((inches1) > 1)){
  state = goodtimes;

}
 
  else
  state=standby;

//if (inchesF < 3){
  //  Serial.println("sensorF");
   // state = sensorF;
//}

}

//===================================

void senseR() {
  motorRSpeedVal += 2;
  analogWrite(motorRSpeed9, motorRSpeedVal);
}

void senseL() {
  motorLSpeedVal -=2;
  analogWrite(motorLSpeed6, motorLSpeedVal);
}


void senseF() {

      analogWrite(motorRSpeed9, 0); //stops right motor
      analogWrite(motorLSpeed6, 0);  //stops left motor
      digitalWrite(motorR10R, HIGH);  //puts right motor in reverse  
      digitalWrite(motorR11F, LOW);
      digitalWrite(motorL7F, LOW);    //puts left motor in reverse
      digitalWrite(motorL8R, HIGH);
     }

float USense1() {

  digitalWrite(TRIG1, HIGH);           //Trigger the sensor for 10 microseconds
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);
  t_search=millis();
  //Serial.println("eat shit fuckers");
  while ((digitalRead(ECHO1) == 0)||((millis()-t_search)>1000)){     //wait for echo pin pulse
     if((millis()-t_search)>1000){ //if statement saying to return to the main loop if the ultrasonic sensor has been waiting for more than one minute
       return; 
      }
     else
     continue;
  }
      Time1=micros();
  //Serial.println("Fuck you all assholes");
  while (digitalRead(ECHO1) == 1);      //once echo pin was held high,

    Time2 = micros();                       //measure how long it was held for,
    Pulse = Time2 - Time1;
    
                   //and convert to inches
    u1inches = Pulse / 148.0;
     if (Pulse > MaxDistance) {  
      Serial.println("Out of range.");        // if distance is too far, then print out of range to monitor
      }
     else {

      Serial.print(u1inches);
      Serial.println(" in");
     }


return u1inches;
}

float USense2() {

  digitalWrite(TRIGF, HIGH);           //Trigger the sensor for 10 microseconds
  delayMicroseconds(10);
  digitalWrite(TRIGF, LOW);

 t_search=millis();
  while (digitalRead(ECHO1) == 0||((millis()-t_search)>1000)){       //wait for echo pin pulse
     if((millis()-t_search)>1000){ //if statement saying to return to the main loop if the ultrasonic sensor has been waiting for more than one minute
       return;
      }
       else 
     continue;
  }
      
    Time1 = micros();                       //if time that echo pin was on low lasted longer than 5 sec, then timeout occurs
  
  
  while (digitalRead(ECHOF) == 1);      //once echo pin was held high,

    Time2 = micros();                       //measure how long it was held for,
    Pulse = Time2 - Time1;
    
                   //and convert to inches
    u2inches = Pulse / 148.0;
     if (Pulse > MaxDistance) {  
      Serial.println("Out of range.");        // if distance is too far, then print out of range to monitor
      }
     else {

      //Serial.print(u2inches);
      Serial.println(" in");
     }


     
return u2inches;

}

void goodt(){

  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);

  digitalWrite(10,HIGH);
  digitalWrite(11, LOW);
}

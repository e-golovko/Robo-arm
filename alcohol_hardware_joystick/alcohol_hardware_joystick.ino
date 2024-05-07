#include <Servo.h>  //add Servo library file
Servo myservo1;  // create servo object to control a servo
Servo myservo2;
Servo myservo3;
Servo myservo4;  

// 1: baseplate
// 2: left servo - forward/backward
// 3: right servo - up/down
// 4: claws

int pos1=90, pos2=60, pos3=130, pos4=0;  // define the variable of 4 servo angle,and assign the initial value (that is the boot posture angle value) 

const int CLAWS_CORRECTION_ANGLE = -11;

const int CLAWS_SPEED_COEF = 1;
const int BASEPLATE_SPEED_COEF = 10; // bigger value means slower speed

const int right_X = A2; // define the right X pin to A2
const int right_Y = A5; //define the right Y pin to A5
const int alcohol_sensor = 7; // define the right key to 7(the value of Z axis)
const int left_X = A3; // define the left X pin to A3
const int left_Y = A4;  // define the left Y pin to A4
const int left_key = 6; // define the left key to 6(the value of Z axis)

int x1,y1;  //define a variable to store the read Joystick value
int x2,y2;
int checker_val,alcohol_val;

// s - temporary values for memoizing position
bool allowOperate = false;

int i = 0; //for looping
int last_mempos = 0; //for saving the last value of “i”

void setup() 
{
  //Serial.begin(9600); //  set the baud rate to 9600 
  myservo1.attach(A1);  //set the control pin of Servo1 to A1
  myservo2.attach(A0);  //set the control pin of Servo2 to A0
  myservo3.attach(8);   //set the control pin of Servo3 to D8
  myservo4.attach(9);   //set the control pin of Servo4 to D9

  //start up posture 
  myservo1.write(pos1);  
  myservo2.write(pos2);
  myservo3.write(pos3);
  myservo4.write(pos4+CLAWS_CORRECTION_ANGLE);
  delay(1500);

  pinMode(alcohol_sensor, INPUT);   // set the right/left key to INPUT
  pinMode(left_key, INPUT);
  //Serial.begin(9600); //  set baud rate to 9600
}
 
void loop() 
{
  checker_val = digitalRead(left_key);  //read the value of left Z axis
 
  if(checker_val==1) //determine if left key of Joystick is pressed. Check alcohol value
  {
    alcohol_val = digitalRead(alcohol_sensor);  ////read the value of alco sensor
    if (alcohol_val==0) // 0 mean alcohol detected, 1 - not detected
    {
      allowOperate = false; // ALARM!! Operator is drunk!!
      funnyDance();
    }
    else
    {
      allowOperate = true;
    }
  }

  if (allowOperate)
  {
    x1 = analogRead(right_X); // read the value of right X axis
    y1 = analogRead(right_Y);  // read the value of right Y axis
  
    x2 = analogRead(left_X);  //read the value of left X axis
    y2 = analogRead(left_Y);  //read the value of left Y axis

    claw();

    baseplate();

    right_ser();

    left_ser();
  }
}

void funnyDance()
{
  myservo4.write(30+CLAWS_CORRECTION_ANGLE);
  delay(500);
  myservo4.write(45+CLAWS_CORRECTION_ANGLE);
  delay(500);
  myservo4.write(30+CLAWS_CORRECTION_ANGLE);
  delay(500);
  myservo4.write(45+CLAWS_CORRECTION_ANGLE);
  delay(500);
  myservo4.write(30+CLAWS_CORRECTION_ANGLE);
  delay(500);
}

//Claw
void claw()
{
    //Claw
  if(x2<50) //if push the left joystick to the right
  {
      pos4=pos4-1;  //current angle of servo 4 subtracts 1（change the value you subtract, thus change the closed speed of claw）
      //Serial.println(pos4);
      myservo4.write(pos4);  //Servo4 operates the action，claw gradually closes
      delay(5);
      if(pos4+CLAWS_CORRECTION_ANGLE<0)  //if pos4 value subtracts to 0
      {            //（change value according to real situation）
        pos4=0-CLAWS_CORRECTION_ANGLE;   //stop subtraction when reduce to 0
      }
   }
  if(x2>1000) ////if push the left joystick to the left 
  {
      pos4=pos4+2; // current angle of servo 4 plus 2（change the value you plus, thus change the open speed of claw）

      //Serial.println(pos4);
      myservo4.write(pos4); //Servo4 operates the motion, the claw gradually opens.

      delay(5);
      if(pos4+CLAWS_CORRECTION_ANGLE>110)  //limit the largest angle when open the claw 
      {
        pos4=110-CLAWS_CORRECTION_ANGLE;                                
      }
  }
}
//******************************************************
 //rotate
void baseplate()
{
  if(x1<50)  // if push the right joystick to the right
  {
    pos1=pos1-1;  
    myservo1.write(pos1);  //Servo1 operates the motion, the arm turns right.
    delay(10);
    if(pos1<1)   //limit the angle when turn right
    {
      pos1=1;
    }
  }
  if(x1>1000)  // if push the right joystick to the left
  {
    pos1=pos1+1;  
    myservo1.write(pos1);  //arm turns left 
    delay(10);
    if(pos1>180)  //limit the angle when turn left 
    {
      pos1=180;
    }
  }
}

//**********************************************************/
//Right Servo
void right_ser()
{
    if(y1<50) //if push the right joystick backward
  {
    pos3=pos3-1;
    myservo3.write(pos3); //the joystick of right Servo swings backward
    delay(5);
    if(pos3<0)  //limit the angle
    {
      pos3=0;
    }
  }
  if(y1>1000)  // if push the right joystick forward
  {
    pos3=pos3+1;  
    myservo3.write(pos3);  //the joystick of right Servo swings forward
    delay(5);
    if(pos3>180)  //limit the angle when go down
    {
      pos3=180;
    }
  }
}

//*************************************************************/
//Left Servo
void left_ser()
{
  if(y2<50)  //if push the left joystick backward 
  {
    pos2=pos2+1;
    myservo2.write(pos2);  //the joystick of left Servo swings backward
    delay(5);
    if(pos2>180)   // limit the retracted angle 
    {
      pos2=180;
    }
  }
  
  if(y2>1000)  //if push the left joystick forward 
  {
    pos2=pos2-1;
    myservo2.write(pos2);  //the joystick of left Servo swings forward
    delay(5);
    if(pos2<35)  // Limit the the stretched angle
    {
      pos2=35;
    }
  }

}  
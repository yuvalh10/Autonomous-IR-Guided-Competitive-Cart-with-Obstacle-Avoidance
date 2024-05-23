#include <AFMotor.h> 
#include <SoftwareSerial.h> 
#include <NewPing.h>  
#include <SoftwareSerial.h>
#define BACKW 1 
#define FORW 2 
#define IR_forward 22 
#define IR_right 23 
#define IR_left 24 
#define TRIGGER_PIN_forward 25 
#define TRIGGER_PIN_left 31
#define TRIGGER_PIN_right 33
#define ECHO_PIN_forward 26 
#define ECHO_PIN_right 27 
#define ECHO_PIN_left 29 
#define MAX_DISTANCE 70 
#define MAX_DISTANCE_SIDE 500
#define for_led 38 
#define right_led 39
#define left_led 40 
NewPing dis_forward(TRIGGER_PIN_forward, ECHO_PIN_forward, MAX_DISTANCE); 
NewPing dis_right(TRIGGER_PIN_right, ECHO_PIN_right, MAX_DISTANCE_SIDE); 
NewPing dis_left(TRIGGER_PIN_left, ECHO_PIN_left, MAX_DISTANCE_SIDE); 
 
void stop(); 
void left(); 
void left_fast();
void right(); 
void right_fast();
void right_IR();
void left_IR();
void forward();
void forward_slow(); 
int distance(); 
void checkIR(int signal_f,int signal_l, int signal_r);
void leds_on(int signal_f,int signal_l, int signal_r);
void pass();
int frequency_limiter(int IR);
static int ir_flag = 0; 
AF_DCMotor motor1(1, MOTOR12_64KHZ); 
AF_DCMotor motor2(2, MOTOR12_64KHZ); 
AF_DCMotor motor3(3, MOTOR34_64KHZ); 
AF_DCMotor motor4(4, MOTOR34_64KHZ); 

void setup() 
{ 
    pinMode(IR_forward, INPUT); 
    pinMode(IR_right , INPUT); 
    pinMode(IR_left , INPUT); 
    pinMode(for_led, OUTPUT); 
    pinMode(right_led , OUTPUT); 
    pinMode(left_led , OUTPUT); 
    Serial.begin(9600); // Open serial monitor  
    Serial1.begin(9600);  // Initialize serial communication for
   
} 
static int flag= 1;
void loop()  
{ 
  //check signal while driving
  int signal_f = frequency_limiter(IR_forward);
  int signal_l = frequency_limiter(IR_left);
  int signal_r = frequency_limiter(IR_right);
  while(flag)
  {
    //check signal before the car start driving in order to light leds
    signal_f = frequency_limiter(IR_forward);
    signal_l = frequency_limiter(IR_left);
    signal_r = frequency_limiter(IR_right);
    leds_on(signal_f,signal_l, signal_r);
    //wait for order to drive
   if (Serial1.read() == 'F')
   {
     flag = 0;
   }
  }
   
   if (Serial1.read() == 'B')//stop car via bluethooth
   {
     digitalWrite(right_led,LOW); 
      digitalWrite(for_led,LOW); 
      digitalWrite(left_led,LOW); 
      while(Serial1.read() != 'L')
      {
          stop();
      }
      flag = 1;
   }
   if(!flag)
    checkIR(signal_f,signal_l, signal_r);
} 
      
void forward() 
{ 
  motor1.setSpeed(180); 
  motor1.run(FORW);  //rotate the motor clockwise 
  motor2.setSpeed(180); 
  motor2.run(FORW);  //rotate the motor clockwise 
  motor3.setSpeed(180); 
  motor3.run(FORW);  //rotate the motor clockwise
  motor4.setSpeed(180); 
  motor4.run(FORW);  //rotate the motor clockwise 
   return;  
} 

void forward_slow()
{
  motor1.setSpeed(100); 
  motor1.run(FORW);  //rotate the motor clockwise 
  motor2.setSpeed(100); 
  motor2.run(FORW);  //rotate the motor clockwise 
  motor3.setSpeed(100); 
  motor3.run(FORW);  //rotate the motor clockwise
  motor4.setSpeed(100); 
  motor4.run(FORW);  //rotate the motor clockwise 
   return;  
}

void right_IR()
{
  motor3.run(RELEASE); //rotate the motor anti-clockwise 
  motor4.run(RELEASE);  //rotate the motor clockwise
  motor1.setSpeed(110); //Define maximum velocity 
  motor1.run(FORW); //rotate the motor anti-clockwise 
  motor2.setSpeed(110); 
  motor2.run(FORW);  //rotate the motor clockwise  
   return; 
}
void left_IR()
{
  motor1.run(RELEASE); //rotate the motor anti-clockwise 
  motor2.run(RELEASE);  //rotate the motor clockwise 
  motor3.setSpeed(110); 
  motor3.run(FORW);//rotate the motor clockwise 
  motor4.setSpeed(110); 
  motor4.run(FORW); //rotate the motor anti-clockwise 
  return; 
}
 
void right() 
{ 
  motor3.run(BACKW); //rotate the motor anti-clockwise 
  motor3.setSpeed(90);
  motor4.run(BACKW);  //rotate the motor clockwise
  motor4.setSpeed(90);  
  motor1.setSpeed(110); 
  motor1.run(FORW); //rotate the motor anti-clockwise 
  motor2.setSpeed(110);  
  motor2.run(FORW);  //rotate the motor clockwise 
   return; 
} 
void left() 
{ 
  
  motor1.run(BACKW); //rotate the motor anti-clockwise 
  motor1.setSpeed(90);
  motor2.run(BACKW);  //rotate the motor clockwise
  motor2.setSpeed(90); 
  motor3.setSpeed(110);  
  motor3.run(FORW);  //rotate the motor clockwise 
  motor4.setSpeed(110);  
  motor4.run(FORW); //rotate the motor anti-clockwise 
  return; 
} 

 
void stop() 
{ 
  
  motor1.run(RELEASE); //stop the motor when release the button 
  motor2.run(RELEASE); //rotate the motor clockwise 
  motor3.run(RELEASE); //stop the motor when release the button 
  motor4.run(RELEASE); //stop the motor when release the button 
  delay(30); 
} 
 void checkIR(int signal_f,int signal_l, int signal_r)
 {  
  if(signal_f)//front ir signal
    {  
      digitalWrite(right_led,LOW); 
      digitalWrite(for_led,HIGH); 
      digitalWrite(left_led,LOW);  
      forward();
    }

   else if((signal_r) && !signal_l)//right ir signal
    { 
      digitalWrite(right_led,HIGH); 
      digitalWrite(for_led,LOW); 
      digitalWrite(left_led,LOW); 
      forward();
      delayMicroseconds(100);
      int count = 0;
      while(!frequency_limiter(IR_forward))
      {
          right_IR();
      }
    }
    else if(signal_l && !signal_r)// left ir signal
    {  
       digitalWrite(right_led,LOW); 
      digitalWrite(for_led,LOW); 
      digitalWrite(left_led,HIGH); 
      forward();
      delayMicroseconds(100);
      int count = 0;
      while(!frequency_limiter(IR_forward))
      {
        count++;
        left_IR();

      }
    }  
    
    else 
    {
         pass(); // in case of there is no ir signal
    }
   
 }

 void leds_on(int signal_f,int signal_l, int signal_r)// function to see ir sinal direction before car start driving
 {  
  if(signal_f)
    {  
      digitalWrite(right_led,LOW); 
      digitalWrite(for_led,HIGH); 
      digitalWrite(left_led,LOW); 
    }

   else if((signal_r) && !signal_l)
    { 
      digitalWrite(right_led,HIGH); 
      digitalWrite(for_led,LOW); 
      digitalWrite(left_led,LOW); 
    }
    else if(signal_l && !signal_r)
    {  
       digitalWrite(right_led,LOW); 
      digitalWrite(for_led,LOW); 
      digitalWrite(left_led,HIGH); 
    }
    else
    {
      digitalWrite(right_led,LOW); 
      digitalWrite(for_led,LOW); 
      digitalWrite(left_led,LOW); 
    }    
 }
    
 
 int frequency_limiter(int IR)// function that limit ir reception
 {
    int signal = pulseIn(IR,HIGH,42000);
    if(signal>3800 && signal < 4000)//170-195HZ//low -1.5ms// HIGH - 4ms
      return 1;
    return 0;   
 }
 void pass()
 {
    int count = 0;
    int flag = 0;
    int for_dis = distance(dis_forward); 
    int right_dis = distance(dis_right); 
    int left_dis = distance(dis_left);
       
       if(for_dis < 90  && for_dis != 0)//front obstacle
      {
          
        //pass from the right
        if((right_dis > left_dis && left_dis != 0))// when dis = 0 it means that the wall is very far or reflective wave didnt come
        {
          
          while(for_dis < 75 && for_dis != 0)// turn the diraction that is more open untill front sensor avoiding obstacle
          {
            flag = 1;
            right();
            delay(30);
            digitalWrite(right_led,HIGH); 
            digitalWrite(for_led,HIGH); 
            digitalWrite(left_led,LOW);
            count ++;//count to steer the car after the turn
            for_dis = distance(dis_forward);
          }

          if(flag != 1)// drive forward without ir signal and obstacles | slow downw before turning
          {
             digitalWrite(right_led,HIGH); 
             digitalWrite(for_led,HIGH); 
             digitalWrite(left_led,HIGH); 
             forward_slow();
          }
         
          else if(count >= 10)// steer back to the left
          {
             digitalWrite(right_led,HIGH); 
             digitalWrite(for_led,HIGH); 
             digitalWrite(left_led,HIGH); 
             forward_slow();
             delay(690);
             digitalWrite(right_led,LOW); 
             digitalWrite(for_led,LOW); 
             digitalWrite(left_led,LOW);
                                                                                         
             for(int i = 0; i<count;i++)
             {
               left();
               delay(30);

             }
          }
         
        }
        //pass from the left
        else if(left_dis >= right_dis && right_dis != 0)
        {
          while(for_dis < 75 && for_dis != 0)
          {
            flag = 2;
            left();
            delay(30);
            digitalWrite(right_led,LOW); 
            digitalWrite(for_led,HIGH); 
            digitalWrite(left_led,HIGH); 
            count ++;
            for_dis = distance(dis_forward);
          }

          if(flag != 2)
          {
             digitalWrite(right_led,HIGH); 
             digitalWrite(for_led,HIGH); 
             digitalWrite(left_led,HIGH); 
             forward_slow();
          }
          
           else if(count >= 10)
          {
              digitalWrite(right_led,HIGH); 
              digitalWrite(for_led,HIGH); 
              digitalWrite(left_led,HIGH); 
              forward_slow();
              delay(690);
              digitalWrite(right_led,LOW); 
              digitalWrite(for_led,LOW); 
              digitalWrite(left_led,LOW);
              for(int i = 0; i<count;i++)
             {
              right();
              delay(30);

             }
          }
          
        }
        
    }
      // side obstacle
      else if(right_dis < 25 && right_dis !=0)
      {
          digitalWrite(right_led,LOW); 
          digitalWrite(for_led,HIGH); 
          digitalWrite(left_led,HIGH); 
          left();
      }
      else if(left_dis < 25 && left_dis !=0)
      {
          digitalWrite(right_led,HIGH); 
          digitalWrite(for_led,HIGH); 
          digitalWrite(left_led,LOW); 
          right();
      }
      //drive forward if there is any not obstacle and IR signal  
      else
      {
          digitalWrite(right_led,HIGH); 
          digitalWrite(for_led,HIGH); 
          digitalWrite(left_led,HIGH); 
          forward_slow();
      }
 }

int distance(NewPing sonar) 
{ 
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS). 
  return sonar.convert_cm(uS); 
}
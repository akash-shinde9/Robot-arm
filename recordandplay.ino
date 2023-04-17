
#include <Servo.h> //Servo header file

//Declare object for 5 Servo Motors  
Servo Servo_0;
Servo Servo_1;

Servo Servo_3;
Servo Gripper;
int potpin1 = 0;  // analog pin used to connect the potentiometer
   // variable to read the value from the analog pin
int potpin2 = 1;  // analog pin used to connect the potentiometer

int potpin3 = 3;  // analog pin used to connect the potentiometer

int potpin4 = 2;  // analog pin used to connect the potentiometer

//Global Variable Declaration 
int S0_pos, S1_pos,  S3_pos, G_pos; 
int P_S0_pos, P_S1_pos, P_S3_pos, P_G_pos;
int C_S0_pos, C_S1_pos, C_S3_pos, C_G_pos;
int POT_0,POT_1,POT_3,POT_4;

int saved_data[700]; //Array for saving recorded data

int array_index=0;
char incoming = 0;

int action_pos;
int action_servo;

void setup() {
Serial.begin(9600); //Serial Monitor for Debugging

//Declare the pins to which the Servo Motors are connected to 
Servo_0.attach(7);
Servo_1.attach(9);

Servo_3.attach(11);
Gripper.attach(13);

//Write the servo motors to initial position 
Servo_0.write(90);
Servo_1.write(90);

Servo_3.write(90);
Gripper.write(90);

Serial.println("Press 'R' to Record and 'P' to play"); //Instruct the user 
}

void Read_POT() //Function to read the Analog value form POT and map it to Servo value
{
   POT_0 = analogRead(potpin1); POT_1 = analogRead(potpin2); POT_3 = analogRead(potpin3); POT_4 = analogRead(potpin4); //Read the Analog values form all five POT
   S0_pos = map(POT_0, 0, 600, 180, 0); //Map it for 1st Servo (Base motor)
   S1_pos = map(POT_1, 0, 600, 180, 0); //Map it for 2nd Servo (Hip motor)
   
   S3_pos = map(POT_3,600, 0, 0, 180); //Map it for 4th Servo (Neck motor)
   G_pos  = map(POT_4,0, 600, 0, 180);  //Map it for 5th Servo (Gripper motor)
}

void Record() //Function to Record the movements of the Robotic Arm
{
Read_POT(); //Read the POT values  for 1st time

//Save it in a variable to compare it later
   P_S0_pos = S0_pos;
   P_S1_pos = S1_pos;
   
   P_S3_pos = S3_pos;
   P_G_pos  = G_pos;
   
//Read_POT(); //Read the POT value for 2nd time
  
   if (P_S0_pos == S0_pos) //If 1st and 2nd value are same
   {
    Servo_0.write(S0_pos); //Control the servo
    
    if (C_S0_pos != S0_pos) //If the POT has been turned 
    {
      saved_data[array_index] = S0_pos + 0; //Save the new position to the array. Zero is added for zeroth motor (for understading purpose)
      array_index++; //Increase the array index 
    }
    
    C_S0_pos = S0_pos; //Saved the previous value to check if the POT has been turned 
   }

//Similarly repeat for all 5 servo Motors
   if (P_S1_pos == S1_pos)
   {
    Servo_1.write(S1_pos);
    
    if (C_S1_pos != S1_pos)
    {
      saved_data[array_index] = S1_pos + 1000; //1000 is added for 1st servo motor as differentiator 
      array_index++;
    }
    
    C_S1_pos = S1_pos;
   }

   

   if (P_S3_pos == S3_pos)
   {
    Servo_3.write(S3_pos); 
    
    if (C_S3_pos != S3_pos)
    {
      saved_data[array_index] = S3_pos + 3000; //3000 is added for 3rd servo motor as differentiater 
      array_index++;
    }
    
    C_S3_pos = S3_pos;   
   }

   if (P_G_pos == G_pos)
   {
    Gripper.write(G_pos);
    
    if (C_G_pos != G_pos)
    {
      saved_data[array_index] = G_pos + 4000; //4000 is added for 4th servo motor as differentiator 
      array_index++;
    }
    
    C_G_pos = G_pos;
   }
   
  //Print the value for debugging 
  Serial.print(S0_pos);  Serial.print("  "); Serial.print(S1_pos); Serial.print("  "); Serial.print(S3_pos); Serial.print("  "); Serial.println(G_pos);
  Serial.print ("Index = "); Serial.println (array_index); 
  delay(100); 
}

void Play() //Functon to play the recorded movements on the Robotic ARM
{
  for (int Play_action=0; Play_action<array_index; Play_action++) //Navigate through every saved element in the array 
  {
    action_servo = saved_data[Play_action] / 1000; //The fist character of the array element is split for knowing the servo number
    action_pos = saved_data[Play_action] % 1000; //The last three characters of the array element is split to know the servo postion 

    switch(action_servo){ //Check which servo motor should be controlled 
      case 0: //If zeroth motor
        Servo_0.write(action_pos);
      break;

      case 1://If 1st motor
        Servo_1.write(action_pos);
      break;

      

      case 3://If 3rd motor
        Servo_3.write(action_pos);
      break;

      case 4://If 4th motor
        Gripper.write(action_pos);
      break;
    }

    delay(50);
    
  }
}

void loop() {

if (Serial.available() > 1) //If something is received from serial monitor 
{
incoming = Serial.read();
if (incoming == 'R')
Serial.println("Robotic Arm Recording Started......");
if (incoming == 'P')
Serial.println("Playing Recorded sequence");
}

if (incoming == 'R') //If user has selected Record mode
Record();

if (incoming == 'P') //If user has selected Play Mode 
Play();

}
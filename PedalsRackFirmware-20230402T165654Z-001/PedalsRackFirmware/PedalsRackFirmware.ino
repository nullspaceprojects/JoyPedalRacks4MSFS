/*
  NULLSPACE PROJECTS
  PEDALS-RACK FIRMWARE
  FLIGHT SIMULATOR 2020
*/
#include "NullSpaceLib.h"

#define RUDDER_PEDAL A0
#define BRAKE_PEDAL A1
#define DEADBAND_RUDDER_PEDAL 500

TimerC loop_timer;
#define LOOP_SAMPLING_TIME 100

struct StructPedalsRack
{
    long  rudder_position; //=timone=yaw control -16k left +16k right
    long  break_position;//=0-32k
};

StructPedalsRack gJoystickPedals;

void setup() {

  Serial.begin(115200);
  loop_timer.start();
}

int a_rp;
int a_brake;
void loop() {

  if(loop_timer.getET()>=LOOP_SAMPLING_TIME)
  {
    loop_timer.reset();
    /****  RUDDER PEDAL ****/
    int value = analogRead(RUDDER_PEDAL);	// read X axis value [0..1023]
	  //a_rp = value;
    //Serial.print("X:");
    //Serial.print(value, DEC);
    //544=center
    //253=left
    //804=left
    //gJoystickPedals.rudder_position = map(value,0,1023,-16383,16383);
    gJoystickPedals.rudder_position = map(value,253,804,-16383,16383);
  	if(gJoystickPedals.rudder_position>16383)
  	{
  		gJoystickPedals.rudder_position=16383;
  	}
  	if(gJoystickPedals.rudder_position<-16383)
  	{
  		gJoystickPedals.rudder_position=-16383;
  	}
	//DEAD-BAND
	if(gJoystickPedals.rudder_position>=-DEADBAND_RUDDER_PEDAL && gJoystickPedals.rudder_position<=DEADBAND_RUDDER_PEDAL)
  	{
  		gJoystickPedals.rudder_position=0; //CENTER
  	}

    /****  BRAKE_PEDAL ****/
    value = analogRead(BRAKE_PEDAL);	// read Y axis value [0..1023]
    //a_brake = value;
	  //Serial.print(" | Y:");
    //Serial.print(value, DEC);
    //1000 = no brake
    //586 = full brake
    gJoystickPedals.break_position = map(value,1000,820,-16383,16383);
  	if(gJoystickPedals.break_position>16383)
  	{
  		gJoystickPedals.break_position=16383;
  	}
  	if(gJoystickPedals.break_position<-16383)
  	{
  		gJoystickPedals.break_position=-16383;
  	}
    
    

    //pack the data as JSON
    String msg = String("{\"rudder_position\": ") + String(gJoystickPedals.rudder_position) + 
                  String(",\"brake_position\": ") + String(gJoystickPedals.break_position) +
                  String("}");

    /*
	  String msg = String("{\"rudder_position\": ") + String(a_rp) + 
                  String(",\"brake_position\": ") + String(a_brake) +
                  String("}");
  */
    Serial.println(msg);    
    
  }
  
}

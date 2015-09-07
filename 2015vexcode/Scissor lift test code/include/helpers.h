#ifndef HELPERS_H_ 
#define HELPERS_H_ 

/* Globals */

/* change values to invert motors */
static unsigned char invertMotor[10] = { 0,   // Motor 1  
										 0,   // Motor 2
										 0,   // Motor 3
										 0,   // Motor 4
										 1,   // Motor 5
										 1,   // Motor 6
										 1,   // Motor 7
								  	  	 1,   // Motor 7
								  	  	 1,   // Motor 9   
								  	  	 1 }; // Motor 10

/* Function Prototypes */

void runMotor(unsigned char channel, int speed);
int joystickDeadband(int joystick, float percent);
void runAllMotors(int joy_val_LY, int joy_val_RY);

#endif


 

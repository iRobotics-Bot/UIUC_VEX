/*
 * camera.c
 *
 *  Created on: Feb 11, 2016
 *      Author: Status
 */

#include "API.h"
#include "helpers.h"

unsigned char read1;
unsigned char read2;
int pixy_check_sum;
int pixy_signature_number;//1-7
int pixy_object_x_center;//0-319
int pixy_object_y_center;//0-199
int pixy_object_width;//0-320
int pixy_object_height;//0-200

#define CAMERA_CENTER (320/2)
#define TIMEOUT_TIME 100

void init_camera(){
	usartInit(uart1, 9600, 0x00);
}

void parse_frame_object(void){

	read1 = fgetc(uart1);//throw away check sum
	read2 = fgetc(uart1);//yes I know it is jank
	pixy_check_sum = (((int) read2) << 8) | read1;

	read1 = fgetc(uart1);//Signature Number
	read2 = fgetc(uart1);
	pixy_signature_number = (((int) read2) << 8) | read1;

	read1 = fgetc(uart1);//x coor
	read2 = fgetc(uart1);
	pixy_object_x_center = (((int) read2) << 8) | read1;

	read1 = fgetc(uart1);//y coor
	read2 = fgetc(uart1);
	pixy_object_y_center = (((int) read2) << 8) | read1;

	read1 = fgetc(uart1);//width
	read2 = fgetc(uart1);
	pixy_object_width = (((int) read2) << 8) | read1;

	read1 = fgetc(uart1);//height
	read2 = fgetc(uart1);
	pixy_object_height = (((int) read2) << 8) | read1;

	printf("CS: %d SN: %d X: %d Y: %d Wd: %d Ht: %d\r\n",pixy_check_sum, pixy_signature_number,pixy_object_x_center,pixy_object_y_center,pixy_object_width,pixy_object_height);
}

int findBallx(){
	//Documentation for UART protocol here: http://cmucam.org/projects/cmucam5/wiki/Pixy_Serial_Protocol
	int time_out = 0;
	while(time_out < TIMEOUT_TIME){
		read1 = fgetc(uart1);
		if(read1 == 0x55)//get sync characters
		{
			read1 = fgetc(uart1);
			if(read1 == 0xaa)
			{
				read1 = fgetc(uart1);
				if(read1 == 0x55)//get sync characters
				{
					read1 = fgetc(uart1);
					if(read1 == 0xaa)
					{
						parse_frame_object();
						return pixy_object_x_center;
					}
				}
			}
		}
		time_out++;
	}
	return -1;
}
int findBally(){
	//Documentation for UART protocol here: http://cmucam.org/projects/cmucam5/wiki/Pixy_Serial_Protocol
	int time_out = 0;
	while(time_out < TIMEOUT_TIME){
		read1 = fgetc(uart1);
		if(read1 == 0x55)//get sync characters
		{
			read1 = fgetc(uart1);
			if(read1 == 0xaa)
			{
				read1 = fgetc(uart1);
				if(read1 == 0x55)//get sync characters
				{
					read1 = fgetc(uart1);
					if(read1 == 0xaa)
					{
						parse_frame_object();
						return pixy_object_y_center;
					}
				}
			}
		}
		time_out++;
	}
	return -1;
}

bool turnViaCamera(int drive_val)
{
	/* The setpoint always center */
	turn_PID.setpoint = CAMERA_CENTER;
	/* Get the observed gyro value */
	int ball_coord = findBallx();
	if(ball_coord == -1)
		return true;

	turn_PID.observed = ball_coord;

	/* Run the PID given the above setpoint and observed values */
	int rotate = PID( &turn_PID );

	/* Make sure the control is within the allowable range */
	if( rotate > 127 )
	{
		rotate = 127;
	}
	else if( rotate < -127 )
	{
		rotate = -127;
	}

	/* Apply the control */
//	printf("rotate: %d \n\r", rotate);
	Drive( drive_val, rotate * 0.9 );  // half speed

	/* If the observed value is within a reasonable distance
	 * of the desired value for a short period of time, then
	 * we are done.
	 */
	if( turn_PID.observed < turn_PID.setpoint + 10 &&
		turn_PID.observed > turn_PID.setpoint - 10 )
	{
		turn_PID_counter++;
	}
	else
	{
		turn_PID_counter = 0;
	}

	bool done = false;
	if( turn_PID_counter > 300/LOOP_TIME )
	{
		done = true;
	}

	return done;
}

void camPursuit(){
	int ball_y = findBally();
	if(ball_y == -1)
		return;
	//range of readable distances is about 172-48 where the higher numbers are closer.
	int drive_val = ball_y-48;
	drive_val = HALF_SPEED+(drive_val/2);//roughly scales to HALF_SPEED to FULL_SPEED
	if(drive_val < 0)
		drive_val = HALF_SPEED;
	Manipulate(true, false);
	while(!turnViaCamera(drive_val));
}


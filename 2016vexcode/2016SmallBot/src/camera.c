/*
 * camera.c
 *
 *  Created on: Feb 11, 2016
 *      Author: Status
 */

#include "API.h"

unsigned char read1;
unsigned char read2;
int pixy_check_sum;
int pixy_signature_number;
int pixy_object_x_center;
int pixy_object_y_center;
int pixy_object_width;
int pixy_object_height;

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

void findBall(){
	//Documentation for UART protocol here: http://cmucam.org/projects/cmucam5/wiki/Pixy_Serial_Protocol
	while(1){
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
//						return;
					}
				}
			}
		}
	}
}



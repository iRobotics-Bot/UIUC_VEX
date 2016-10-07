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
int pixy_object_angle;//0-200?

#define CAMERA_CENTER (320/2)
#define TIMEOUT_TIME 50

void init_camera(){
	usartInit(uart1, 9600, 0x00);
	usartInit(uart2, 9600, 0x00);
}

void parse_frame_object_color(void){

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

void parse_frame_object_code(void){

	read1 = fgetc(uart2);//throw away check sum
	read2 = fgetc(uart2);//yes I know it is jank
	pixy_check_sum = (((int) read2) << 8) | read1;

	read1 = fgetc(uart2);//Signature Number
	read2 = fgetc(uart2);
	pixy_signature_number = (((int) read2) << 8) | read1;

	read1 = fgetc(uart2);//x coor
	read2 = fgetc(uart2);
	pixy_object_x_center = (((int) read2) << 8) | read1;

	read1 = fgetc(uart2);//y coor
	read2 = fgetc(uart2);
	pixy_object_y_center = (((int) read2) << 8) | read1;

	read1 = fgetc(uart2);//width
	read2 = fgetc(uart2);
	pixy_object_width = (((int) read2) << 8) | read1;

	read1 = fgetc(uart2);//height
	read2 = fgetc(uart2);
	pixy_object_height = (((int) read2) << 8) | read1;

	read1 = fgetc(uart2);//height
	read2 = fgetc(uart2);
	pixy_object_angle = (((int) read2) << 8) | read1;

	printf("CS: %d SN: %d X: %d Y: %d Wd: %d Ht: %d Ag: %d\r\n",pixy_check_sum, pixy_signature_number,pixy_object_x_center,pixy_object_y_center,pixy_object_width,pixy_object_height,pixy_object_angle);
}

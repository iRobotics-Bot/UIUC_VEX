/*
 * camera.h
 *
 *  Created on: Feb 11, 2016
 *      Author: Status
 */

#ifndef INCLUDE_CAMERA_H_
#define INCLUDE_CAMERA_H_

void init_camera();
int findBallx();
int findBally();
void parse_frame_object();

bool turnViaCamera(int drive_val);

void camPursuit();

#endif /* INCLUDE_CAMERA_H_ */

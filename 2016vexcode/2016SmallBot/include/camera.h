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
int findTowerx();
void parse_frame_object_color();
void parse_frame_object_code();

bool turnViaCamera_Ball(int drive_val);
bool turnViaCamera_Tower(int drive_val);

void camPursuit();
void camAim();

#endif /* INCLUDE_CAMERA_H_ */

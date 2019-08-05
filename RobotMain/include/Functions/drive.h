#pragma once

void drive_stop_seq(int direction, int timeout, float delta_trip, float kdrift, int maxpower);
void stopMoving();
void jdubDrive(int direction, int target, int maxpower, int minpower, unsigned int timeout, float kaccel, float kdeaccel, float kdrift);
void QRDTurn_3_L(int deadtime);
void turn_single_constant(int target, unsigned int timeout, int robotSpeed);
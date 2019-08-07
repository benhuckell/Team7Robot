#pragma once

void drive_stop_seq(int direction, int deadtime, int timeout, float delta_trip, float kdrift, int maxpower);
void drive_stop(int powerL, int powerR, int powerL_dead, int powerR_dead, int deadtime, int timeout, float delta_trip);
void stopMoving();
void stopMoving_Post1();
void jdubDrive(int direction, int target, int maxpower, int minpower, unsigned int timeout, float kaccel, float kdeaccel, float kdrift);
void QRDTurn(bool rightTurn, int deadtime, int powerL, int powerR, bool followTapeVar, int followTapeDuration);
void turn_single_constant(int target, unsigned int timeout, int robotSpeed);
void jiggle();
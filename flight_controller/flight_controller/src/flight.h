/*
 * flight.h
 *
 * Created: 7/17/2015 2:43:52 PM
 *  Author: Robert Phillips III
 */ 


#ifndef FLIGHT_H_
#define FLIGHT_H_

struct pid_vals {
	float output;
	float target;
	
	float last_error;
	float accum_error;
};

struct accelerometer {
	float x;
	float y;
	float z;
	
	float roll;
	float pitch;
};

struct magnetometer {
	float x;
	float y;
	float z;
};

struct gyroscope {
	float x;
	float y;
	float z;
	
	float roll;
	float pitch;
	float yaw;
};

struct flight {
	float roll;
	float pitch;
	float yaw;
	
	struct pid_vals *pid_roll;
	struct pid_vals *pid_pitch;
	struct pid_vals *pid_yaw;
};

#endif /* FLIGHT_H_ */
/*
 * pid.c
 *
 * Created: 7/17/2015 1:49:49 PM
 *  Author: Robert Phillips III
 */ 

#include "general.h"
#include "pid.h"

static struct pid_vals g_roll;
static struct pid_vals g_pitch;
static struct pid_vals g_yaw;

static float g_kp = 2.5f;
static float g_ki = 2.0f;
static float g_kd = 0.7f;

#if 0
/*working variables*/
unsigned long lastTime;
double Input, Output, Setpoint;
double errSum, lastErr;
double kp, ki, kd;
void Compute()
{
	/*How long since we last calculated*/
	unsigned long now = millis();
	double timeChange = (double)(now - lastTime);
	
	/*Compute all the working error variables*/
	double error = Setpoint - Input;
	errSum += (error * timeChange);
	double dErr = (error - lastErr) / timeChange;
	
	/*Compute PID Output*/
	Output = kp * error + ki * errSum + kd * dErr;
	
	/*Remember some variables for next time*/
	lastErr = error;
	lastTime = now;
}

void SetTunings(double Kp, double Ki, double Kd)
{
	kp = Kp;
	ki = Ki;
	kd = Kd;
}
#endif

void _init(struct pid_vals *vals)
{
	vals->accum_error = 0;
	vals->last_error  = 0;
	vals->output      = 0;
	vals->target      = 0;
}

void _calc(struct pid_vals *data, float input, float dt)
{
	float err = data->target - input;
	
	data->accum_error += err*dt;
	double de = (err - data->last_error)/dt;
	
	data->output = g_kp*err + g_ki*data->accum_error + g_kd*de;
	data->last_error = err;
}

void pid_loop(struct flight *meas, float dt)
{
	_calc(meas->pid_roll, meas->roll, dt);
	_calc(meas->pid_pitch, meas->pitch, dt);
	
	// TODO: turn on yaw compensation
	//_calc(meas->pid_yaw, meas->yaw, dt);
}

void pid_init(struct flight *meas)
{
	_init(&g_roll);
	_init(&g_pitch);
	_init(&g_yaw);
	
	meas->pid_pitch = &g_pitch;
	meas->pid_roll  = &g_roll;
	meas->pid_yaw   = &g_yaw;
}
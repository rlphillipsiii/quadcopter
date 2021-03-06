/*
 * pid.c
 *
 * Created: 7/17/2015 1:49:49 PM
 *  Author: Robert Phillips III
 */ 

#include <avr/io.h>

#include "general.h"
#include "pid.h"

#define TIMER               TCC1
#define TIMER_TOP           500
#define TIMER_PRESCALER     64.0
#define TIMER_FREQUENCY     ((F_CPU/TIMER_PRESCALER)/TIMER_TOP)
#define TIMER_PERIOD        1.0/TIMER_FREQUENCY

float g_pid_dt = 1.0/CTRL_LOOP_HZ;

static struct pid_vals g_roll;
static struct pid_vals g_pitch;
static struct pid_vals g_yaw;

static float g_kp = 12.53f;
static float g_ki = 0.00f;
static float g_kd = 0.00f;

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
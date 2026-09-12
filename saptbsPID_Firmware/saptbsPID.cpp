#include "saptbsPID.h"


void pid_setup(PIDsetup *tpid, float kp, float ki, float kd,  float max, float min){
    tpid->kp = kp;
    tpid->ki = ki;
    tpid->kd = kd;

    tpid->integral = 0.0f;
    tpid->past_error = 0.0f;
    
    tpid->max_output = max;
    tpid->min_output = min;

}


int pid_calculate(PIDsetup *tpid, float target, float wrongdegree, float dt){
    if(dt <= 0.0f) return 0;

    float error = (target - wrongdegree)*(-1);

    //p
    float proportional_o = (tpid->kp) * error;

    //i
    tpid->integral += error * dt;
    if(tpid->integral < -750.f) tpid->integral = -750.f;
    else if(tpid->integral > 750.f) tpid->integral = 750.f; 
    
    float integral_o = (tpid->integral)*(tpid->ki);

    //d
    float derivative_o = (tpid->kd)*(error - tpid->past_error) / dt;

    //saturation
    float output = proportional_o + integral_o + derivative_o + 1000.f;
    
    if(output > tpid->max_output) output = tpid->max_output;
    else if(output < tpid->min_output) output = tpid->min_output;

    tpid->past_error = error;

    return output;

}

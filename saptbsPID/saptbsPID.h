#ifndef SAPTBSPID_H
#define SAPTBSPID_H

#include <Arduino.h>

typedef struct{
    float kp;
    float ki;
    float kd;

    float integral;
    float past_error;
    float max_output;
    float min_output;

}PIDsetup;
void pid_setup(PIDsetup *tpid, float kp, float ki, float kd,  float max, float min);
int pid_calculate(PIDsetup *tpid, float target, float wrongdegree, float dt);



#endif

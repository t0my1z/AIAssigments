#ifndef __PARAMS_H__
#define __PARAMS_H__

#include <CoreMinimal.h>

struct Params
{
    FVector targetPosition;
    float deceleration_radius;
    float max_velocity;
    float max_acceleration;
    float dest_radius;
    
    float targetRotation;
    float max_angular_velocity;
    float max_angular_acceleration;
    float angular_arrive_radius;
    
    float look_ahead;
    float time_ahead;
    
    float radius;
};

bool ReadParams(const char* filename, Params& params);

#endif
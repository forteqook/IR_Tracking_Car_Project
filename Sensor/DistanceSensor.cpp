#include "DistanceSensor.h"
#include "math.h"

#define MAX_DISTANCE (-1.0)

DistanceSensor::DistanceSensor(PinName a): m_analogIn(a)
{
    m_distance = MAX_DISTANCE;
}
float DistanceSensor::getDistance_cm()
{
    vol = m_analogIn*3.3;
    return 26.498*pow(vol,-1.194);
}
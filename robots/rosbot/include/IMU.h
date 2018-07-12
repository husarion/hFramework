#ifndef _IMU_H_
#define _IMU_H_

class IMU
{
  public:
    void begin();
    float getAngle();
    void resetFifo();
    void getEulerAngles(float *roll, float *pitch, float *yaw);
};

#endif
#ifndef _PANTHER_H_
#define _PANTHER_H_

#include "hFramework.h"
#include "wheel.h"
#include "vector"

namespace hFramework
{
class Panther
{
public:
  Panther(){};
  /**
   * @brief Robot initialisation
   * @param speed_timeout Time in milliseconds to wait until motors are stopped.
   * @param use_motor_PIDs If true will use PID regulators for motors speed, otherwise user can control motors power directly.
   */
  void initPanther(uint16_t speed_timeout = 300, bool use_motor_PIDs = false);

/**
 * @brief Setup controller for motors 
 * @param use_PID If true will use PID regulators for motors speed, otherwise user can control motors power directly.
 * @param slewRate Time constant for motors acceleration limit.
 */
  void initWheelController(bool use_PID = false, float slewRate = 0.075);
  void initBatteryMonitor(float voltage_threshold = 10.5);
  float getBatteryLevel();

  /**
   * @brief Set robot to drive with desired speed, PID controllers must be enabled prior to use of this function.
   * @param linear Linear velocity [m/s]
   * @param angular Angular velocity [rad/s]
   */
  void setSpeed(float linear, float angular);

  /**
   * @brief Set robot to drive with desired power
   * @param linear Percentage of maximum linear power
   * @param angular Percentage of maximum angular power
   */
  void setSpeedPower(float linear, float angular);

  void initOdometry();
  std::vector<float> getPose();
  void reset_odometry();

private:
  void wheelUpdater();
  void batteryMonitor();
  void odometryUpdater();
  std::vector<float> pantherPose;
  void reset_encoders();
  void reset_odom_vars();
  void reset_wheels();

  /**
   * @brief Watchdog for speed controller
   * @param timeout Time in milliseconds to wait until robot is stopped.
   */
  void speedWatchdog(uint32_t timeout);

  Wheel *wheelFL;
  Wheel *wheelRL;
  Wheel *wheelFR;
  Wheel *wheelRR;

  float lin;
  float ang;
  float L_wheel_lin_speed;
  float R_wheel_lin_speed;
  float L_wheel_angular_velocity;
  float R_wheel_angular_velocity;
  float L_enc_speed;
  float R_enc_speed;
  float robot_width = 0.65;   // meters
  float wheel_radius = 0.381; //meters
  uint16_t enc_res = 100;     // encoder tics per wheel revolution

  float voltage_limit;
  float current_voltage;
  bool batteryLow = false;
  uint16_t loop_delay = 10; // milliseconds
  float delay_s;
  int32_t enc_FL = 0; // encoder tics
  int32_t enc_RL = 0; // encoder tics
  int32_t enc_FR = 0; // encoder tics
  int32_t enc_RR = 0; // encoder tics

  uint64_t lastSpeedUpdate; // system time of lat speed update

  int32_t enc_L = 0;         // encoder tics
  float wheel_L_ang_pos = 0; // radians
  float wheel_L_ang_vel = 0; // radians per second

  int32_t enc_R = 0;         // encoder tics
  float wheel_R_ang_pos = 0; // radians
  float wheel_R_ang_vel = 0; // radians per second

  float robot_angular_pos = 0; // radians
  float robot_angular_vel = 0; // radians per second

  float robot_x_pos = 0; // meters
  float robot_y_pos = 0; // meters
  float robot_x_vel = 0; // meters per second
  float robot_y_vel = 0; // meters per second
};

extern Panther panther;
} // namespace hFramework
#endif
#include "Panther.h"

namespace hFramework
{
Panther panther;

void Panther::initPanther(uint16_t speed_timeout, bool use_motor_PIDs)
{
    Serial.printf("panther initialization begin\n");
    initBatteryMonitor();
    initOdometry();
    initWheelController(use_motor_PIDs, 0.075);
    sys.taskCreate(std::bind(&Panther::speedWatchdog, this, speed_timeout));
    return;
}

void Panther::initWheelController(bool use_PID, float slewRate)
{
    if (use_PID)
    {
        wheelFL = new Wheel(hMotD, 1);
        wheelRL = new Wheel(hMotC, 1);
        wheelFR = new Wheel(hMotA, 0);
        wheelRR = new Wheel(hMotB, 0);

        wheelFL->begin();
        wheelRL->begin();
        wheelFR->begin();
        wheelRR->begin();
        sys.taskCreate(std::bind(&Panther::wheelUpdater, this));
    }
    else
    {
        hMotD.setMotorPolarity(Polarity::Reversed);
        hMotD.setEncoderPolarity(Polarity::Reversed);
        hMotC.setMotorPolarity(Polarity::Reversed);
        hMotC.setEncoderPolarity(Polarity::Reversed);
        hMotA.setMotorPolarity(Polarity::Normal);
        hMotA.setEncoderPolarity(Polarity::Normal);
        hMotB.setMotorPolarity(Polarity::Normal);
        hMotB.setEncoderPolarity(Polarity::Normal);
    }
    hMot1.setPWMFreq(PWM_freq_hMotor_ID_1, PWM_freq_1_kHz);
    hMot2.setPWMFreq(PWM_freq_hMotor_ID_2, PWM_freq_1_kHz);
    hMot3.setPWMFreq(PWM_freq_hMotor_ID_3, PWM_freq_1_kHz);
    hMot4.setPWMFreq(PWM_freq_hMotor_ID_4, PWM_freq_1_kHz);

    hMot1.setSlewRate(slewRate, true);
    hMot2.setSlewRate(slewRate, true);
    hMot3.setSlewRate(slewRate, true);
    hMot4.setSlewRate(slewRate, true);
}

void Panther::wheelUpdater()
{
    uint32_t t = sys.getRefTime();
    long dt = 10;
    for (;;)
    {
        wheelFR->update(dt);
        wheelRR->update(dt);
        wheelRL->update(dt);
        wheelFL->update(dt);
        sys.delaySync(t, dt);
    }
}

void Panther::setSpeed(float linear, float angular)
{
    lin = linear;
    ang = angular;

    L_wheel_lin_speed = lin - (ang * robot_width / 2);
    R_wheel_lin_speed = lin + (ang * robot_width / 2);
    L_wheel_angular_velocity = L_wheel_lin_speed / wheel_radius;
    R_wheel_angular_velocity = R_wheel_lin_speed / wheel_radius;
    L_enc_speed = enc_res * L_wheel_angular_velocity / (2 * M_PI);
    R_enc_speed = enc_res * R_wheel_angular_velocity / (2 * M_PI);

    wheelFL->setSpeed(L_enc_speed);
    wheelRL->setSpeed(L_enc_speed);
    wheelFR->setSpeed(R_enc_speed);
    wheelRR->setSpeed(R_enc_speed);
}

void Panther::setSpeedPower(float linear, float angular)
{
    float left_mot = 10 * (linear - angular);
    float right_mot = 10 * (linear + angular);
    hMotD.setPower(left_mot);
    hMotC.setPower(left_mot);
    hMotA.setPower(right_mot);
    hMotB.setPower(right_mot);
    lastSpeedUpdate = sys.getRefTime();
}

void Panther::speedWatchdog(uint32_t timeout)
{
    sys.log("Init watchdog for motors, timeout: %d", timeout);
    uint64_t now;
    while (true)
    {
        now = sys.getRefTime();
        if (now > lastSpeedUpdate + timeout)
        {
            sys.log("Speed command not received for 300ms, stop motors\r\n");
            setSpeedPower(0, 0);
        }
        sys.delay(timeout / 10);
    }
}

void Panther::batteryMonitor()
{
    int i = 0;
    int publish_cnt = 0;
    for (;;)
    {
        if (sys.getSupplyVoltage() > voltage_limit)
        {
            i--;
        }
        else
        {
            i++;
        }
        if (i > 50)
        {
            batteryLow = true;
            i = 50;
        }
        if (i < -50)
        {
            batteryLow = false;
            i = -50;
        }
        if (batteryLow == true)
        {
            LED1.toggle();
        }
        else
        {
            LED1.on();
        }
        if (publish_cnt == 8)
        {
            current_voltage = sys.getSupplyVoltage();
            publish_cnt = 0;
        }
        publish_cnt++;
        sys.delay(250);
    }
}

void Panther::initBatteryMonitor(float voltage_threshold)
{
    voltage_limit = voltage_threshold;
    sys.taskCreate(std::bind(&Panther::batteryMonitor, this));
}

float Panther::getBatteryLevel()
{
    return current_voltage;
}

void Panther::initOdometry()
{
    delay_s = (float)loop_delay / (float)1000;
    Serial.printf("Create odometryUpdater task\n");
    sys.taskCreate(std::bind(&Panther::odometryUpdater, this));
}

void Panther::odometryUpdater()
{
    while (true)
    {
        enc_FR = hMotA.getEncoderCnt();
        enc_RR = hMotB.getEncoderCnt();
        enc_RL = hMotC.getEncoderCnt();
        enc_FL = hMotD.getEncoderCnt();
        // Serial.printf("Encoders: %d, %d, %d, %d\n", enc_FL, enc_FR, enc_RL, enc_RR);

        enc_L = (enc_FL + enc_RL) / 2;
        enc_R = (enc_FR + enc_RR) / 2;

        wheel_L_ang_vel = ((2 * 3.14 * enc_L / enc_res) - wheel_L_ang_pos) / delay_s;
        wheel_R_ang_vel = ((2 * 3.14 * enc_R / enc_res) - wheel_R_ang_pos) / delay_s;

        wheel_L_ang_pos = 2 * 3.14 * enc_L / enc_res;
        wheel_R_ang_pos = 2 * 3.14 * enc_R / enc_res;

        robot_angular_vel = (((wheel_R_ang_pos - wheel_L_ang_pos) * wheel_radius / robot_width) - robot_angular_pos) / delay_s;
        robot_angular_pos = (wheel_R_ang_pos - wheel_L_ang_pos) * wheel_radius / robot_width;
        robot_x_vel = (wheel_L_ang_vel * wheel_radius + robot_angular_vel * robot_width / 2) * cos(robot_angular_pos);
        robot_y_vel = (wheel_L_ang_vel * wheel_radius + robot_angular_vel * robot_width / 2) * sin(robot_angular_pos);
        robot_x_pos = robot_x_pos + robot_x_vel * delay_s;
        robot_y_pos = robot_y_pos + robot_y_vel * delay_s;
        sys.delay(loop_delay);
    }
}

std::vector<float> Panther::getPose()
{
    pantherPose.clear();
    pantherPose.push_back(robot_x_pos);
    pantherPose.push_back(robot_y_pos);
    pantherPose.push_back(robot_angular_pos);
    return pantherPose;
}

void Panther::reset_encoders()
{
    hMot1.resetEncoderCnt();
    hMot2.resetEncoderCnt();
    hMot3.resetEncoderCnt();
    hMot4.resetEncoderCnt();
}

void Panther::reset_odom_vars()
{
    enc_FL = 0;            // encoder tics
    enc_RL = 0;            // encoder tics
    enc_FR = 0;            // encoder tics
    enc_RR = 0;            // encoder tics
    enc_L = 0;             // encoder tics
    wheel_L_ang_pos = 0;   // radians
    wheel_L_ang_vel = 0;   // radians per second
    enc_R = 0;             // encoder tics
    wheel_R_ang_pos = 0;   // radians
    wheel_R_ang_vel = 0;   // radians per second
    robot_angular_pos = 0; // radians
    robot_angular_vel = 0; // radians per second
    robot_x_pos = 0;       // meters
    robot_y_pos = 0;       // meters
    robot_x_vel = 0;       // meters per second
    robot_y_vel = 0;       // meters per second
}

void Panther::reset_wheels()
{
    hMotA.resetEncoderCnt();
    hMotA.setPower(0);
    hMotB.resetEncoderCnt();
    hMotB.setPower(0);
    hMotC.resetEncoderCnt();
    hMotC.setPower(0);
    hMotD.resetEncoderCnt();
    hMotD.setPower(0);
}

void Panther::reset_odometry()
{
    reset_wheels();
    reset_encoders();
    reset_odom_vars();
}

} // namespace hFramework
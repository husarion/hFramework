#include "ROSbot.h"

namespace hFramework
{
ROSbot rosbot;

void ROSbot::initROSbot(SensorType s, ImuType i)
{
    Serial.printf("ROSbot initialization begin\n");
    initIMU(i);
    initBatteryMonitor();
    initOdometry();
    initDistanceSensors(s);
    initWheelController();
    return;
}

void ROSbot::initWheelController()
{
    wheelFL = new Wheel(hMotD, 1);
    wheelRL = new Wheel(hMotC, 1);
    wheelFR = new Wheel(hMotA, 0);
    wheelRR = new Wheel(hMotB, 0);

    wheelFL->begin();
    wheelRL->begin();
    wheelFR->begin();
    wheelRR->begin();
    sys.taskCreate(std::bind(&ROSbot::wheelUpdater, this));
}

void ROSbot::wheelUpdater()
{
    long dt = 10;
    for (;;)
    {
        wheelFR->update(dt);
        wheelRR->update(dt);
        wheelRL->update(dt);
        wheelFL->update(dt);
        sys.delay(dt);
    }
}

void ROSbot::setSpeed(float linear, float angular)
{
    lin = linear;
    ang = angular;

    L_wheel_lin_speed = lin - (ang * robot_width / 2.05);
    R_wheel_lin_speed = lin + (ang * robot_width / 2.05);
    L_wheel_angular_velocity = L_wheel_lin_speed / wheel_radius;
    R_wheel_angular_velocity = R_wheel_lin_speed / wheel_radius;
    L_enc_speed = 0.001 * enc_res * L_wheel_angular_velocity / (2 * M_PI);
    R_enc_speed = 0.001 * enc_res * R_wheel_angular_velocity / (2 * M_PI);

    wheelFL->setSpeed(L_enc_speed);
    wheelRL->setSpeed(L_enc_speed);
    wheelFR->setSpeed(R_enc_speed);
    wheelRR->setSpeed(R_enc_speed);
}

void ROSbot::batteryMonitor()
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
            // battery_pub->publish(&battery);
            publish_cnt = 0;
            // Serial.printf("Battery monitor running\n");
        }
        publish_cnt++;
        sys.delay(250);
    }
}

void ROSbot::initBatteryMonitor(float voltage_threshold)
{
    voltage_limit = voltage_threshold;
    sys.taskCreate(std::bind(&ROSbot::batteryMonitor, this));
}

float ROSbot::getBatteryLevel()
{
    return current_voltage;
}

void ROSbot::initOdometry()
{
    delay_s = (float)loop_delay / (float)1000;
    Serial.printf("Create odometryUpdater task\n");
    sys.taskCreate(std::bind(&ROSbot::odometryUpdater, this));
}

void ROSbot::odometryUpdater()
{
    while (true)
    {
        enc_FR = wheelFR->getDistance();
        enc_RR = wheelRR->getDistance();
        enc_RL = wheelRL->getDistance();
        enc_FL = wheelFL->getDistance();

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

std::vector<float> ROSbot::getPose()
{
    rosbotPose.clear();
    rosbotPose.push_back(robot_x_pos);
    rosbotPose.push_back(robot_y_pos);
    rosbotPose.push_back(robot_angular_pos);
    return rosbotPose;
}

void ROSbot::reset_encoders()
{
    hMot1.resetEncoderCnt();
    hMot2.resetEncoderCnt();
    hMot3.resetEncoderCnt();
    hMot4.resetEncoderCnt();
}

void ROSbot::reset_odom_vars()
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

void ROSbot::reset_wheels()
{
    wheelFR->reset();
    wheelRR->reset();
    wheelRL->reset();
    wheelFL->reset();
}

void ROSbot::reset_odometry()
{
    reset_wheels();
    reset_encoders();
    reset_odom_vars();
}

void ROSbot::initDistanceSensors(SensorType s)
{
    sensor_type = s;
    switch (sensor_type)
    {
    case SENSOR_LASER:
        hSens1.selectI2C();
        VL53L0XXshutPort.pin1.setOut();
        VL53L0XXshutPort.pin1.write(false);
        VL53L0XXshutPort.pin2.setOut();
        VL53L0XXshutPort.pin2.write(false);
        VL53L0XXshutPort.pin3.setOut();
        VL53L0XXshutPort.pin3.write(false);
        VL53L0XXshutPort.pin4.setOut();
        VL53L0XXshutPort.pin4.write(false);
        VL53L0XXshutPort.pin1.reset();
        sensor_dis[0].setAddress(44);
        sys.delay(100);
        VL53L0XXshutPort.pin2.reset();
        sensor_dis[1].setAddress(43);
        sys.delay(100);
        VL53L0XXshutPort.pin3.reset();
        sensor_dis[2].setAddress(42);
        sys.delay(100);
        VL53L0XXshutPort.pin4.reset();
        sys.delay(100);
        sensor_dis[0].setTimeout(500);
        sensor_dis[1].setTimeout(500);
        sensor_dis[2].setTimeout(500);
        sensor_dis[3].setTimeout(500);
        sensor_dis[0].init();
        sensor_dis[1].init();
        sensor_dis[2].init();
        sensor_dis[3].init();
        sensor_dis[0].startContinuous();
        sensor_dis[1].startContinuous();
        sensor_dis[2].startContinuous();
        sensor_dis[3].startContinuous();
        sys.taskCreate(std::bind(&ROSbot::rangesTask, this));
        break;
    case SENSOR_INFRARED:
        sensMUX = new hSensor(hSens_ID_3);
        sys.taskCreate(std::bind(&ROSbot::rangesTask, this));
        break;
    default:
        break;
    }
}

void ROSbot::readIRSensors()
{
    sensMUX->pin1.enableADC();
    sensMUX->pin2.setOut();
    sensMUX->pin3.setOut();
    sensMUX->pin4.setOut();
    for (size_t i = 0; i < 8; i++)
    {
        if (tMUX[i].active == true)
        {
            sensMUX->pin2.write(tMUX[i].p2);
            sensMUX->pin3.write(tMUX[i].p3);
            sensMUX->pin4.write(tMUX[i].p4);
            sys.delay(MUXStepTime);
            float temp = 20 * (1 / (sensMUX->pin1.analogReadRaw() * 0.001220703));
            if (temp > 30)
                temp = 30;
            if (temp < 4)
                temp = 4;
            *tMUX[i].dis = temp * 0.01;
        }
    }
}

int ROSbot::readLaserDistanceSensor(VL53L0X &s)
{
    return s.readRangeContinuousMillimeters();
}

std::vector<float> ROSbot::getRanges(SensorType s)
{
    ranges.clear();
    ranges.push_back(dis1);
    ranges.push_back(dis2);
    ranges.push_back(dis3);
    ranges.push_back(dis4);
    return ranges;
}

void ROSbot::rangesTask()
{
    while (true)
    {
        switch (sensor_type)
        {
        case SENSOR_LASER:
            dis2 = 0.001 * ((float)readLaserDistanceSensor(sensor_dis[0]));
            dis1 = 0.001 * ((float)readLaserDistanceSensor(sensor_dis[1]));
            dis3 = 0.001 * ((float)readLaserDistanceSensor(sensor_dis[2]));
            dis4 = 0.001 * ((float)readLaserDistanceSensor(sensor_dis[3]));
            break;
        case SENSOR_INFRARED:
            readIRSensors();
            break;
        }
        sys.delay(50);
    }
}

void ROSbot::initIMU(ImuType i)
{
    switch (i)
    {
    case MPU9250:
        imu.begin();
        imu.resetFifo();
        sys.taskCreate(std::bind(&ROSbot::eulerAnglesTask, this));
        break;
    case NO_IMU:
        break;
    }
}

std::vector<float> ROSbot::getRPY()
{
    imuArray.clear();
    imuArray.push_back(roll);
    imuArray.push_back(pitch);
    imuArray.push_back(yaw);
    return imuArray;
}

void ROSbot::eulerAnglesTask()
{
    while (true)
    {
        imu.getEulerAngles(&roll, &pitch, &yaw);
        sys.delay(50);
    }
}
} // namespace hFramework
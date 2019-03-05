#include "wheel.h"
#include "math.h"

Wheel::Wheel(hMotor& motor, bool polarity, float max_speed)
{
	mot = &motor;
	pol = polarity;
	_max_speed = max_speed;
}

void Wheel::begin()
{
	vReg.setScale(1);
	vReg.setKP(0.25);
	vReg.setKI(0.005);
	vReg.setKD(0);

	vReg.setRange(-vRange, vRange);

	if (1 == pol) {
		mot->setMotorPolarity(Polarity::Reversed);
		mot->setEncoderPolarity(Polarity::Reversed);
	}

	mot->resetEncoderCnt();
	speed_step = _max_speed * 0.01;
}

void Wheel::update(uint32_t dt)
{
	vTarget_tmp = vTarget + copysign(speed_step, vSet - vTarget);	
	if (vTarget_tmp > _max_speed)
	{
		vTarget = _max_speed;
	}
	else if (vTarget_tmp < -_max_speed)
	{
		vTarget = -_max_speed;
	}
	else if (fabs(vTarget_tmp) <= speed_step && vSet == 0)
	{
		vTarget = 0;
	}
	else
	{
		vTarget = vTarget_tmp;
	}

	float vErr = 0.0;
	float pidOut = 0;
	dNow = mot->getEncoderCnt();
	vNow = (dNow - (float)lastPositions[0]) / (dt * lastPositions.size() * 0.001);
	lastPositions.push_back(dNow);

	vErr = vNow - vTarget;
	pidOut = vReg.update(vErr, dt);

	if (turnedOn == true) {
		mot->setPower(pidOut);
	}
}

void Wheel::setSpeed(float speed)
{
	vSet = speed;
}

float Wheel::getSpeed()
{
	return vNow;
}

int32_t Wheel::getDistance()
{
	return dNow;
}

void Wheel::resetDistance()
{
	mot->resetEncoderCnt();
}

void Wheel::reset()
{
	mot->resetEncoderCnt();
	vReg.reset();
	for (int i = 0; i < lastPositions.size(); i++)
	{
		lastPositions.push_back(0);
	}
	dNow = 0;
	vNow = 0;
	vTarget = 0;
	vSet = 0;
	vTarget_tmp = 0;
	mot->setPower(0);
}

void Wheel::turnOff()
{
	turnedOn = false;
	mot->setPower(0);
}
void Wheel::turnOn()
{
	turnedOn = true;
}

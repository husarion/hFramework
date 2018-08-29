#include "wheel.h"


Wheel::Wheel(hMotor& motor, bool polarity)
{
	mot = &motor;
	pol = polarity;
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
}

void Wheel::update(uint32_t dt)
{
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
	vTarget = speed;
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

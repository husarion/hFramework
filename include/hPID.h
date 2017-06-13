/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HPID_H__
#define __HPID_H__

#define PID_FLAG_HAS_MIN  0x01
#define PID_FLAG_HAS_MAX  0x02
#define PID_FLAG_HAS_IMIN 0x04
#define PID_FLAG_HAS_IMAX 0x08
#define PID_FLAG_HAS_LAST_ERROR 0x10
#define PID_FLAG_I_ENABLED 0x20

#include <cstdint>

namespace hFramework
{

class hPID
{
private:
	float scale, Kp, Ki, Kd;
	float isum;
	float imin, imax, min, max;

	float lastError;

	uint8_t flags;

public:
	hPID();
	hPID(float Kp, float Ki, float Kd);

	void setScale(float scale) { this->scale = scale; }
	void setCoeffs(float Kp, float Ki, float Kd) { this->Kp = Kp; this->Ki = Ki; this->Kd = Kd; }
	void setKP(float Kp) { this->Kp = Kp; }
	void setKI(float Ki) { this->Ki = Ki; }
	void setKD(float Kd) { this->Kd = Kd; }

	float getScale() const { return scale; }
	float getKP() const { return Kp; }
	float getKI() const { return Ki; }
	float getKD() const { return Kd; }

	float getIsum() const { return isum; }
	float getLastError() const { return lastError; }

	void reset();

	void enableI() { flags |= PID_FLAG_I_ENABLED; }
	void disableI() { flags &= ~PID_FLAG_I_ENABLED; }

	void setRange(float range) { setRangeMin(-range); setRangeMax(range); }
	void setRange(float min, float max) { setRangeMin(min); setRangeMax(max); }
	void setRangeMin(float min);
	void setRangeMax(float max);
	void setIRange(float range) { setIRangeMin(-range); setIRangeMax(range); }
	void setIRange(float min, float max) { setIRangeMin(min); setIRangeMax(max); }
	void setIRangeMin(float min);
	void setIRangeMax(float max);

	float update(float error, int dt_ms);
};

}

#endif

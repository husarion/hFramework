/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HCONDVAR_H__
#define __HCONDVAR_H__

#include <hMutex.h>

#include <condition_variable>

namespace hFramework
{

class hCondVar
{
	std::condition_variable_any d_condition;

public:
	void notifyOne();
	/*__attribute__((deprecated))*/ void notify_one() { notifyOne(); }
	bool wait(hMutex& mutex, uint32_t timeout = INFINITE);
};

}

#endif

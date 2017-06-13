/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HCONDVAR_H__
#define __HCONDVAR_H__

#include <hTypes.h>
#include <hMutex.h>

namespace hFramework
{

class hCondVar
{
	hSemaphore s, x;
	int waiters = 0;
public:
	hCondVar();

	void notifyOne();
	bool wait(hMutex& mutex, uint32_t timeout = INFINITE);

private:
	class hCondVarPimpl;
	hCondVarPimpl* impl;

	hCondVar(const hCondVar&) = delete;

};
}

#endif

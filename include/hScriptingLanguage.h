/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __H_SCRIPTING_LANGAUGE__
#define __H_SCRIPTING_LANGAUGE__

/**
 * Generic interface for scripting languages such as Python.
 */
class hScriptingLanguage
{
public:
	virtual void init() = 0;
	virtual void deinit() = 0;

	virtual void evalSingle(const char* src) = 0;
	virtual void eval(const char* src) = 0;

	virtual bool replContinueWithInput(const char* buff) = 0;
	virtual void killVM() = 0;
};

extern hScriptingLanguage* ScriptingLanguage;

#endif

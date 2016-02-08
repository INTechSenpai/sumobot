#ifndef _SINGLETON_h
#define _SINGLETON_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

template<class T>
class Singleton
{
public:
	static T& Instance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton(){}
};

#endif
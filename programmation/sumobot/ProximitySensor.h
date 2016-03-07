#ifndef _PROXIMITYSENSOR_h
#define _PROXIMITYSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class ProximitySensor
{
public:
	ProximitySensor()
	{}
	 
	/* Permet d'ajouter si besoin un méchanisme de filtrage des valeurs aberrantes, qui sera commun à tous les capteurs */
	uint32_t getDistance()
	{
		return getMesure();
	}

protected:
	virtual uint32_t getMesure() = 0;

};

#endif


#include "utils.h"

/*
 * La fonction '%' du langage C ne renvoie pas le modulo au sens mathématique du terme.
 * En réalité pour (a % b) il fait ( ABS(a) % b)*SIGNE(a)
 * Voici donc pour votre plus grand plaisir, la fonction modulo mathématiquement CORRECTE.
 *
 * De rien, c'est gratuit.
 *
 * @author: Sylvain Gaultier
 * */

int modulo(int nombre, int modulo)
{
	if(modulo <= 0)
	{
		return -1;
	}
	else if(nombre >= 0 )
	{
		return nombre % modulo;
	}
	else
	{
		return modulo - ((-nombre) % modulo);
	}
}

// Met x au carré (plus optimisé que pow(x, 2) )
float square(float x)
{
	return x * x;
}


// Fait clignotter la LED de la pin indiquée avec les delais ON/OFF indiqués en ms
void blink(uint8_t pin, unsigned int periodeON, unsigned int periodeOFF)
{
	static uint32_t lastChange = 0;
	static bool ledON = false;

	if (ledON)
	{
		if (millis() - lastChange >= periodeON && periodeOFF != 0)
		{
			digitalWrite(pin, LOW);
			ledON = false;
			lastChange = millis();
		}
	}
	else
	{
		if (millis() - lastChange >= periodeOFF && periodeON != 0)
		{
			digitalWrite(pin, HIGH);
			ledON = true;
			lastChange = millis();
		}
	}
}


// Fait clignotter la LED intégrée avec les delais ON/OFF indiqués en ms
void blinkDelOnBoard(unsigned int periodeON, unsigned int periodeOFF)
{
	static uint32_t lastChange = 0;
	static bool ledON = false;

	if (ledON)
	{
		if (millis() - lastChange >= periodeON && periodeOFF != 0)
		{
			digitalWrite(13, LOW);
			ledON = false;
			lastChange = millis();
		}
	}
	else
	{
		if (millis() - lastChange >= periodeOFF && periodeON != 0)
		{
			digitalWrite(13, HIGH);
			ledON = true;
			lastChange = millis();
		}
	}
}
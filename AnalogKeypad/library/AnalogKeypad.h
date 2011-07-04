#ifndef ANALOGKEYPAD_h
#define ANALOGKEYPAD_h

#include "WProgram.h"

#define KEY_NONE		-1
#define KEY_STAR		0
#define KEY_7			1
#define KEY_4			2
#define KEY_1			3
#define KEY_0			4
#define KEY_8			5
#define KEY_5			6
#define KEY_2			7
#define KEY_POUND		8
#define KEY_9			9
#define KEY_6			10
#define KEY_3			11

class AnalogKeypad
{
public:
	/**********************************************************************************************************
	 pin:			Analog input pin the keypad signal wire is connected to.
	 rPull:			Ohm value of the pullup resistor on the keypad backpack.
	 rLadder:		Ohm value of each of the identical resistors in the resistor ladder
	 vcc:			Voltage applied to the high side of the voltage divider 
					(it is assumed the voltage at the common terminal is 0v)
	 adcMax:		The max reading of the voltage divider when a key is not pressed.
	 nBounce:		# of consecutive readings same value readings needed to ensure debouncing of the keys
	 repeatRate:	Repeat rate of key events when the key is depressed for a long length of time (<= 0 = off)
					(In between actual key events KEY_NONE will be returned)
	 
	 returns:		One of the key constants: KEY_NONE thru KEY_3
	 **********************************************************************************************************/
    AnalogKeypad(int pin, int rPull=12000, int rLadder=1000, int vcc=5, int adcMax=1023, 
				 int nBounce=15, int repeatRate=1);
	int readKey();
	void init();
	
private:	
	int _pin;
	int _rLadder;
	int _rPull;
	int _vcc;
	int _adcMax;
	int _minGap;
	int _nBounce;
	int _tRepeatPeriod;
	
	int _vals[12];
	short _lastKey;
	int _nKeyCount;
	short _lastKeyDebounced;
	long _tLastKeyDebounced;
};

#endif


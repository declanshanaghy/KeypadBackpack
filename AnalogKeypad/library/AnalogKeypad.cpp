#include "AnalogKeypad.h"

AnalogKeypad::AnalogKeypad(int pin, int rPull, int rLadder, int vcc, int adcMax, int nBounce, int repeatRate)
{
	_pin = pin;
	_rLadder = rLadder;
	_rPull = rPull;
	_vcc = vcc;
	_adcMax = adcMax;
	_nBounce = nBounce;
	
	if ( repeatRate > 0 )
		_tRepeatPeriod = 1000 * (1.0 / repeatRate);
	else
		_tRepeatPeriod = 0;
#if DEBUG	
	Serial.print("_tRepeatPeriod="); Serial.println(_tRepeatPeriod);
#endif
	
	_nKeyCount = 0;
	_tLastKeyDebounced = 0;
	_lastKey = _lastKeyDebounced = KEY_NONE;
}

void AnalogKeypad::init() {
	float v, vK;
	float stv = (float)_adcMax / _vcc;
	
	//Calculate all the voltage divider values for each key and map into an array
	for ( int i=0; i<12; i++ ) {
		vK = _rLadder * (i+1);
		v = (vK / (_rPull + vK)) * _vcc;
		_vals[i] = v * stv;
#if DEBUG	
		Serial.print("vals["); Serial.print(i); Serial.print("] = "); Serial.println(_vals[i]);
#endif
	}
	
	//Calc th emin gap between values as a threshold to use when reading keys
	int g;
	_minGap = 32767;
	for ( int i=0; i<11; i++ ) {
		g = (_vals[i+1] - _vals[i]) / 2;
		if ( g < _minGap )
			_minGap = g;
	}
#if DEBUG	
	Serial.print("_minGap="); Serial.println(_minGap);
#endif
}

//TODO: Debounce this.
int AnalogKeypad::readKey()
{
	long tNow = millis();
	short thisKey = KEY_NONE;
	
	int v = analogRead(_pin);
#if DEBUG	
	Serial.print("keyV="); Serial.println(v);
#endif
	
	//No key pressed
	if ( v >= _vals[11] + _minGap )
		thisKey = KEY_NONE;
	else {
		//Cycle thru all key values from highest to lowest and find the closest mapping
		for ( int i=11; i>=0; i-- ) {
			if ( v >= _vals[i] - _minGap ) {
				thisKey = i;
				break;
			}
		}
	}

	if ( thisKey == _lastKey ) {
		_nKeyCount++;
		//Valid keypress
		if ( _nKeyCount > _nBounce ) {
			//To avoid overflow
			_nKeyCount = _nBounce + 1;
			_lastKey = thisKey;
			_lastKeyDebounced = thisKey;
			
			if ( tNow - _tLastKeyDebounced > _tRepeatPeriod ) {
				_tLastKeyDebounced = tNow;				
#if DEBUG	
				Serial.print("return _lastKeyDebounced="); Serial.println(_lastKeyDebounced);
#endif				
				return _lastKeyDebounced;
			}
			else {
#if DEBUG	
				Serial.println("return WAIT");
#endif
				return KEY_NONE;
			}
			
		}
	}
	else {
		_nKeyCount = 0;
		_tLastKeyDebounced = 0;
	}				
	
	_lastKey = thisKey;
#if DEBUG	
	Serial.println("return KEY_NONE");
#endif
	return KEY_NONE;	
}








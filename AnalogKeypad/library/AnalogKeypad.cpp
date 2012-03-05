#include "AnalogKeypad.h"

AnalogKeypad::AnalogKeypad(int pin, int repeatRate, int rPull, 
						   int rLadder, int vcc, int adcMax, 
						   int tDebounce) {
	_pin = pin;
	_rLadder = rLadder;
	_rPull = rPull;
	_vcc = vcc;
	_adcMax = adcMax;
	_tDebounce = tDebounce;
	
	if ( repeatRate > 0 )
		_tRepeatPeriod = 1000 * (1.0 / repeatRate);
	else if ( repeatRate == REPEAT_MAX )		
		_tRepeatPeriod = REPEAT_MAX;
	else
		_tRepeatPeriod = REPEAT_OFF;

//	Serial.print("_tRepeatPeriod="); Serial.println(_tRepeatPeriod);
	
	_tLastKeyDebounced = _keyLocked = 0;
	_lastKey = _lastKeyDebounced = KEY_NONE;
}

void AnalogKeypad::init() {
	float v, vK;
	float stv = (float)_adcMax / _vcc;
	
	//Calculate all the voltage divider values for 
	//each key and map into an array
	for ( int i=0; i<12; i++ ) {
		vK = _rLadder * (i+1);
		v = (vK / (_rPull + vK)) * _vcc;
		_vals[i] = v * stv;
		//Serial.print("vals["); Serial.print(i); 
		//Serial.print("] = "); Serial.println(_vals[i]);
	}
	
	//Calc the min gap between values as a threshold to 
	//use when reading keys
	int g;
	_minGap = 32767;
	for ( int i=0; i<11; i++ ) {
		g = (_vals[i+1] - _vals[i]) / 2;
		if ( g < _minGap )
			_minGap = g;
	}
//	Serial.print("_minGap="); Serial.println(_minGap);
}

char AnalogKeypad::getLastKeyChar() {
	switch ( _lastKeyRead ) {
		case KEY_0:
			return '0';
		case KEY_1:
			return '1';
		case KEY_2:
			return '2';
		case KEY_3:
			return '3';
		case KEY_4:
			return '4';
		case KEY_5:
			return '5';
		case KEY_6:
			return '6';
		case KEY_7:
			return '7';
		case KEY_8:
			return '8';
		case KEY_9:
			return '9';
		case KEY_STAR:
			return '*';
		case KEY_POUND:
			return '#';
		case KEY_NONE:
			return NULL;
	}	
}

int AnalogKeypad::getLastKeyNumber() {
	switch ( _lastKeyRead ) {
		case KEY_0:
			return 0;
		case KEY_1:
			return 1;
		case KEY_2:
			return 2;
		case KEY_3:
			return 3;
		case KEY_4:
			return 4;
		case KEY_5:
			return 5;
		case KEY_6:
			return 6;
		case KEY_7:
			return 7;
		case KEY_8:
			return 8;
		case KEY_9:
			return 9;
		default:
			return -1;
	}
}

short AnalogKeypad::getLastKey() {
	return _lastKeyRead;
}

short AnalogKeypad::readKey() {
	short k = _readKey();
	_lastKeyRead = k;
	return k;
}

short AnalogKeypad::_readKey() {
	long tNow = millis();
	short thisKey = KEY_NONE;
	
	int v = analogRead(_pin);
	
	//No key pressed
	if ( v >= _vals[11] + _minGap )
		thisKey = KEY_NONE;
	else {
		//Cycle thru all key values from highest to lowest 
		//and find the closest mapping
		for ( int i=11; i>=0; i-- ) {
			if ( v >= _vals[i] - _minGap ) {
				thisKey = i;
				break;
			}
		}
	}

	// If the reading changed reset the debouncing timer
	if (thisKey != _lastKey) {
		_tLastKeyDebounced = tNow;
		_keyLocked = 0;
	} 
		
	_lastKey = thisKey;
	
	if ((tNow - _tLastKeyDebounced) > _tDebounce) {
		// Reading has been steady for longer than the debounce delay.
		_lastKeyDebounced = thisKey;
		
		if ( thisKey == KEY_NONE ) {
//			Serial.println("readKey=NO KEY PRESSED");
			return KEY_NONE;
		}
		else {
			if ( _tRepeatPeriod > 0 ) {		//User specified repeat rate
				//Is this the first key event?
				//	or 
				//Calc the time since we last reported a key press and 
				//check if its greater the the specified repeat period
				if ( !_keyLocked || (tNow - _tLastKeyDebounced) > _tRepeatPeriod ) {
					//Reset the last time we reported a keypress, so there 
					//won't be another event until the period passes again
					_tLastKeyDebounced = tNow;
					
					//Record that this is not the first key event anymore
					_keyLocked = 1;
//					Serial.print("readKey="); Serial.println(thisKey);
					return thisKey;
				}
				else {
					//Don't report a key event, its not time yet.
//					Serial.println("readKey=waiting...");
					return KEY_NONE;
				}
			}
			else if ( _tRepeatPeriod == REPEAT_MAX ) {	//Max repeat rate
//				Serial.print("readKey="); Serial.println(thisKey);
				return thisKey;
			}
			else {	//No REPEAT
				if ( _keyLocked ) {
//					Serial.println("readKey=LOCKED");
					return KEY_NONE;
				}
				else {
//					Serial.print("readKey="); Serial.println(thisKey);
					_keyLocked = 1;
					return thisKey;
				}
			}
		}
	}
	else {
		//Still bouncing...
//		Serial.println("readKey=BOUNCING");
		return KEY_NONE;	
	}
}








/*
 * Copyright James Kong, 2013
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial 3.0 Unported License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/3.0/
 * or send a letter to
 * Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#pragma once

#include "ofxLEDsImplementation.h"

#include "ofShader.h"

class ofxLEDsP9813
: public ofxLEDsImplementation
{
public:
	ofxLEDsP9813(const size_t _numLEDs);
	virtual ~ofxLEDsP9813();
	
	void resize(size_t _numLEDs);
	void clear(const ofColor& c);
	void encode();
	
	void setPixels(std::vector<ofColor>colors);
	void setPixels(unsigned char*colors ,int _size);
	ofMutex txBufferMutex;
	
	std::vector<uint8_t> txBuffer;
	std::vector<uint8_t> txPixels;
	
	//p9813
	uint8_t makeFlag (uint8_t red, uint8_t green, uint8_t blue);
	
private:
	size_t DataStart;
	size_t PixelsStart;
	size_t PixelsEnd;
	size_t LatchStart;
	size_t DataEnd;
	
	static ofShader EncodingShader;
	static bool EncodedShaderInitialized;
};

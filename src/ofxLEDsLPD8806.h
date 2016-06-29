/*
 * Copyright Paul Reimer, 2012
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial 3.0 Unported License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/3.0/
 * or send a letter to
 * Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#pragma once

#include "ofxLEDsImplementation.h"
#include "ofxLEDsLPD8806Frame.h"

#include "ofShader.h"

class ofxLEDsLPD8806
: public ofxLEDsImplementation
{
public:
<<<<<<< HEAD
  ofxLEDsLPD8806(const size_t _numLEDs ,int width=0, int height=0);
  virtual ~ofxLEDsLPD8806();

  void resize(size_t _numLEDs);
    void resize(size_t _numLEDs,int width, int height);
  void clear(const ofColor& c);
  void encode();
	
	void setPixels(std::vector<ofColor>colors);
	void setPixels(unsigned char*colors ,int _size);
  ofMutex txBufferMutex;
  std::vector<uint8_t> txBuffer;
=======
  ofxLEDsLPD8806(const size_t _numLEDs=0);
  virtual ~ofxLEDsLPD8806();

  void encode();
#ifdef TARGET_OPENGLES
  void resize(size_t _numLEDs);
#endif
>>>>>>> paulreimer/master

private:
  static ofShader lpd8806EncodingShader;
  static bool lpd8806EncodedShaderInitialized;
<<<<<<< HEAD
// #ifdef TARGET_OPENGLES
//     ofPixels pix;
// #endif
=======
#ifdef TARGET_OPENGLES
  std::vector<uint8_t> pixelDataBufferRGBA;
#endif
>>>>>>> paulreimer/master
};

/*
 * Copyright Paul Reimer, 2012
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial 3.0 Unported License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/3.0/
 * or send a letter to
 * Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include "ofxLEDsLPD8806.h"

ofShader ofxLEDsLPD8806::lpd8806EncodingShader;
bool ofxLEDsLPD8806::lpd8806EncodedShaderInitialized;

//--------------------------------------------------------------
ofxLEDsLPD8806::ofxLEDsLPD8806(const size_t _numLEDs,int width , int height)
: ofxLEDsImplementation(_numLEDs)
{
<<<<<<< HEAD
	if (!lpd8806EncodedShaderInitialized)
	{
#ifdef TARGET_OPENGLES
		std::stringstream vertexShaderSource;
		vertexShaderSource
		<< "attribute vec4 position;\n"
		<< "attribute vec4 color;\n"
		<< "attribute vec4 normal;\n"
		<< "attribute vec2 texcoord;\n"
		<< "uniform mat4 modelViewMatrix;\n"
		<< "uniform mat4 projectionMatrix;\n"
		<< "uniform sampler2D maskTex;\n"
		<< "varying vec4 colorVarying;\n"
		<< "varying vec2 texCoordVarying;\n"
		<< "void main()\n"
		<< "{\n"
		<< "//get our current vertex position so we can modify it\n"
		<< "	vec4 pos = projectionMatrix * modelViewMatrix * position;\n"
		<< "	gl_Position = pos;\n"
		<< "	colorVarying = color;\n"
		<< "	texCoordVarying = texcoord;\n"
		<< "}\n";
		ofLogVerbose("ofxLEDsLPD8806")<< vertexShaderSource.str();
		lpd8806EncodingShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShaderSource.str());
		
		std::stringstream fragmentShaderSource;
		fragmentShaderSource
		<< "uniform sampler2D tex0;\n"
		<< "uniform vec4 color;\n"
		<< "varying vec2 texCoordVarying;\n"
		<< "void main (void)\n"
		<< "{\n"
		<< "	vec2 pos = texCoordVarying;\n"
		<< "	vec3 src = texture2D(tex0, pos).rgb;\n"
		<< "	vec3 lpd8806Color = src*0.5 + 0.5;\n"
		<< "	gl_FragColor = vec4(lpd8806Color.grb,1.0);\n"
		<< "}\n";
		//		<< "	gl_FragColor = vec4( src , 1.0);\n"
		ofLogVerbose("ofxLEDsLPD8806")<< fragmentShaderSource.str();
		lpd8806EncodingShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSource.str());
#else
		std::stringstream vertexShaderSource;
		vertexShaderSource
		<< "varying vec2 TexCoord;"
		<< "void main(void)"
		<< "{"
		<< "  TexCoord = gl_MultiTexCoord0.st;"
		<< "  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
		<< "  gl_FrontColor = gl_Color;"
		<< "}";
		lpd8806EncodingShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShaderSource.str());
		
		std::stringstream fragmentShaderSource;
		fragmentShaderSource
		<< "#version 120\n"
		<< "#extension GL_ARB_texture_rectangle: enable\n"
		<< "varying vec2 TexCoord;"
		<< "uniform sampler2DRect tex0;"
		<< "void main(void)"
		<< "{"
		<< "  vec4 originalColor    = texture2DRect(tex0, TexCoord);"
		<< "  vec4 lpd8806Color     = originalColor*0.5 + 0.5+0.004;"
		<< "  gl_FragColor          = lpd8806Color.grba;"
		<< "}";
		lpd8806EncodingShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSource.str());
#endif
		lpd8806EncodingShader.linkProgram();
		lpd8806EncodedShaderInitialized = true;
	}
	if(width==0 || height==0)
	{
		resize(_numLEDs);
	}
	else
	{
		resize(_numLEDs,width,height);
	}
=======
  if (!lpd8806EncodedShaderInitialized)
  {
    std::stringstream vertexShaderSource;
    if (ofIsGLProgrammableRenderer())
    {
      vertexShaderSource
      << "attribute vec4 position;"
      << "attribute vec2 texcoord;"
      << "uniform mat4 modelViewProjectionMatrix;"
      << "varying vec2 texCoordVarying;"
      << "void main()"
      << "{"
      << "  texCoordVarying = texcoord;"
      << "  gl_Position = modelViewProjectionMatrix * position;"
      << "}";
    }
    else {
      vertexShaderSource
      << "varying vec2 TexCoord;"
      << "void main(void)"
      << "{"
      << "  TexCoord = gl_MultiTexCoord0.st;"
      << "  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
      << "}";
    }
    lpd8806EncodingShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShaderSource.str());

    std::stringstream fragmentShaderSource;
    if (ofIsGLProgrammableRenderer())
    {
      fragmentShaderSource
      << "precision highp float;"
      << "uniform sampler2D tex0;"
      << "varying vec2 texCoordVarying;"
      << "void main()"
      << "{"
      << "  vec4 originalColor    = texture2D(tex0, texCoordVarying);"
      // When cast as char, this is 0x80 | (c>>1)
      << "  vec4 lpd8806Color     = originalColor*0.498 + 0.502;"
      << "  gl_FragColor          = lpd8806Color.brga;"
      << "}";
    }
    else {
      fragmentShaderSource
      << "#version 120\n"
      << "varying vec2 TexCoord;"
      << "uniform sampler2D tex0;"
      << "void main(void)"
      << "{"
      << "  vec4 originalColor    = texture2D(tex0, TexCoord);"
      // When cast as char, this is 0x80 | (c>>1)
      << "  vec4 lpd8806Color     = originalColor*0.498 + 0.502;"
      << "  gl_FragColor          = lpd8806Color.brga;"
      << "}";
    }

    lpd8806EncodingShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSource.str());

    if(ofIsGLProgrammableRenderer())
      lpd8806EncodingShader.bindDefaults();

    lpd8806EncodingShader.linkProgram();
    lpd8806EncodedShaderInitialized = true;
  }
  
  resize(_numLEDs);
>>>>>>> paulreimer/master
}

//--------------------------------------------------------------
ofxLEDsLPD8806::~ofxLEDsLPD8806()
{}
<<<<<<< HEAD
//--------------------------------------------------------------
void ofxLEDsLPD8806::resize(size_t _numLEDs,int width, int height)
{
    numLEDs = _numLEDs;
	stripRect.set(0, 0, width, height);
	
	DataStart   = 0;
	PixelsStart = 4;
	PixelsEnd   = PixelsStart + (3*numLEDs);
	LatchStart  = PixelsEnd;
	DataEnd     = PixelsEnd + 4;
	
	size_t latchSize = 4;
	std::vector<uint8_t> latch(latchSize, 0);
	
	txBuffer.resize(DataEnd);
	
	// Write latch data before any data, and after all the pixel data
	memcpy(&txBuffer[DataStart], latch.data(), latchSize);
	memcpy(&txBuffer[LatchStart], latch.data(), latchSize);
	
	// Initialized black/LED-off pixel data
	memset(&txBuffer[PixelsStart], 0x80, (PixelsEnd-PixelsStart));
	
	ofFbo::Settings fboConfig;
#ifdef TARGET_OPENGLES
	fboConfig.textureTarget = GL_TEXTURE_2D;
#else
	fboConfig.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
#endif
	fboConfig.width         = stripRect.width;
	fboConfig.height        = stripRect.height;
	fboConfig.minFilter     = GL_NEAREST;
	fboConfig.maxFilter     = GL_NEAREST;
	renderBuffer.allocate(fboConfig);
	encodedBuffer.allocate(fboConfig);
}
=======

#ifdef TARGET_OPENGLES
>>>>>>> paulreimer/master
//--------------------------------------------------------------
void
ofxLEDsLPD8806::resize(size_t _numLEDs)
{
<<<<<<< HEAD
// #ifdef TARGET_OPENGLES
//     pix.allocate(_numLEDs, 1, OF_IMAGE_COLOR);
// #endif
	numLEDs = _numLEDs;
	stripRect.set(0, 0, _numLEDs, 1);
	
	DataStart   = 0;
	PixelsStart = 4;
	PixelsEnd   = PixelsStart + (3*numLEDs);
	LatchStart  = PixelsEnd;
	DataEnd     = PixelsEnd + 4;
	
	size_t latchSize = 4;
	std::vector<uint8_t> latch(latchSize, 0);
	
	txBuffer.resize(DataEnd);
	
	// Write latch data before any data, and after all the pixel data
	memcpy(&txBuffer[DataStart], latch.data(), latchSize);
	memcpy(&txBuffer[LatchStart], latch.data(), latchSize);
	
	// Initialized black/LED-off pixel data
	memset(&txBuffer[PixelsStart], 0x80, (PixelsEnd-PixelsStart));
	
	ofFbo::Settings fboConfig;
#ifdef TARGET_OPENGLES
	fboConfig.textureTarget = GL_TEXTURE_2D;
#else
	fboConfig.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
#endif
	fboConfig.width         = stripRect.width;
	fboConfig.height        = stripRect.height;
	fboConfig.minFilter     = GL_NEAREST;
	fboConfig.maxFilter     = GL_NEAREST;
	renderBuffer.allocate(fboConfig);
	encodedBuffer.allocate(fboConfig);
}

//--------------------------------------------------------------
void
ofxLEDsLPD8806::clear(const ofColor& c)
{
	ofxLEDsImplementation::clear(c);
	
	uint8_t pixel[3] = { static_cast<uint8_t>((c.g>>1) | 0x80), static_cast<uint8_t>((c.r>>1) | 0x80), static_cast<uint8_t>((c.b>>1) | 0x80) };
	for (size_t i=0; i<numLEDs; ++i)
		memcpy(&txBuffer[PixelsStart + (3*i)], pixel, 3);
	
	needsEncoding = false;
}
//--------------------------------------------------------------
void
ofxLEDsLPD8806::setPixels(std::vector<ofColor>colors)
{
	
    //	ofxLEDsImplementation::clear(ofColor::black);
	
	for (size_t i=0; i<numLEDs; ++i)
	{
		if(i<colors.size())
		{
			uint8_t pixel[3] = { static_cast<uint8_t>((colors[i].g>>1) | 0x80), static_cast<uint8_t>((colors[i].r>>1) | 0x80), static_cast<uint8_t>((colors[i].b>>1) | 0x80) };
			memcpy(&txBuffer[PixelsStart + (3*i)], pixel, 3);
		}
	}
	
	needsEncoding = false;
}
//--------------------------------------------------------------
void
ofxLEDsLPD8806::setPixels(unsigned char*colors ,int _size)
{
    //	ofxLEDsImplementation::clear(ofColor::black);
	
	for (size_t i=0; i<numLEDs; ++i)
	{
		if(i<_size)
		{
			uint8_t pixel[3] = { static_cast<uint8_t>((colors[i*3+1]>>1) | 0x80),  static_cast<uint8_t>((colors[i*3]>>1)| 0x80), static_cast<uint8_t>((colors[i*3+2]>>1) | 0x80) };
			memcpy(&txBuffer[PixelsStart + (3*i)], pixel, 3);
		}
	}
	
	needsEncoding = false;
=======
  ofxLEDsImplementation::resize(_numLEDs);
  pixelDataBufferRGBA.resize(numLEDs*4);
>>>>>>> paulreimer/master
}
#endif

//--------------------------------------------------------------
void
ofxLEDsLPD8806::encode()
{
<<<<<<< HEAD
//	ofMutex::ScopedLock lock(txBufferMutex);
	
	encodedBuffer.begin();
	{
		lpd8806EncodingShader.begin();
		{
			renderBuffer.getTexture().draw(stripRect);
		}
		lpd8806EncodingShader.end();
	}
	encodedBuffer.end();
	
    
    
	{
#ifndef TARGET_OPENGLES
        ofTexture& dataTexture(encodedBuffer.getTexture());
        dataTexture.bind();
		// These pixels are swizzled into a 2nd array for FTDI Write
		glGetTexImage(dataTexture.getTextureData().textureTarget, 0,
					  GL_RGB, GL_UNSIGNED_BYTE,
					  &txBuffer[PixelsStart]);
        dataTexture.unbind();
#else

       encodedBuffer.bind();
//        int format,type;
//        ofGetGlFormatAndType(encodedBuffer.settings.internalformat,format,type);
       glReadPixels(0,0,encodedBuffer.getWidth(), encodedBuffer.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, &txBuffer[PixelsStart]);
       encodedBuffer.unbind();
=======
//  ofMutex::ScopedLock lock(txBufferMutex);
  encodedBuffer.begin();
  {
    lpd8806EncodingShader.begin();
    {
      renderBuffer.getTextureReference().draw(stripRect);
    }
    lpd8806EncodingShader.end();
  }
  encodedBuffer.end();

#ifndef TARGET_OPENGLES  
  ofTexture& dataTexture(encodedBuffer.getTextureReference());
  dataTexture.bind();
  {
    // These pixels are swizzled into a 2nd array for FTDI Write
    glGetTexImage(dataTexture.getTextureData().textureTarget, 0,
                  GL_RGB, GL_UNSIGNED_BYTE,
                  &pixelDataBuffer[0]);
  }
  dataTexture.unbind();
#else
  encodedBuffer.bind();
  {
    glReadPixels(0, 0,
                 stripRect.width, stripRect.height,
                 GL_RGBA, GL_UNSIGNED_BYTE,
                 &pixelDataBufferRGBA[0]);
  }
  encodedBuffer.unbind();

  size_t w=stripRect.width;
  size_t h=stripRect.height;
  for (size_t y=0; y<h; ++y)
  {
    for (size_t x=0; x<w; ++x)
    {
      pixelDataBuffer[3*((y*w)+(x))+0] = pixelDataBufferRGBA[4*((y*w)+(x))+0];
      pixelDataBuffer[3*((y*w)+(x))+1] = pixelDataBufferRGBA[4*((y*w)+(x))+1];
      pixelDataBuffer[3*((y*w)+(x))+2] = pixelDataBufferRGBA[4*((y*w)+(x))+2];
    }
  }
#endif
>>>>>>> paulreimer/master

        
        //		glReadPixels(0,
        //					 0,
        //					 stripRect.width,
        //					 stripRect.height,
        //					 GL_RGB, GL_UNSIGNED_BYTE,
        //					 &txBuffer[PixelsStart]);
        // encodedBuffer.readToPixels(pix);
        // memcpy(&txBuffer[PixelsStart], pix.getPixels() , numLEDs);
        
#endif
	}
    
	
	needsEncoding = false;
}

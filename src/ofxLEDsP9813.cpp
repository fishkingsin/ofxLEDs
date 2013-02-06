/*
 * Copyright Paul Reimer, 2012
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial 3.0 Unported License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/3.0/
 * or send a letter to
 * Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include "ofxLEDsP9813.h"

ofShader ofxLEDsP9813::EncodingShader;
bool ofxLEDsP9813::EncodedShaderInitialized;

//--------------------------------------------------------------
ofxLEDsP9813::ofxLEDsP9813(const size_t _numLEDs)
: ofxLEDsImplementation(_numLEDs)
{
	if (!EncodedShaderInitialized)
	{

		std::stringstream vertexShaderSource;
		vertexShaderSource
		<< "varying vec2 TexCoord;"
		<< "void main(void)"
		<< "{"
		<< "  TexCoord = gl_MultiTexCoord0.st;"
		<< "  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
		<< "  gl_FrontColor = gl_Color;"
		<< "}";
		EncodingShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShaderSource.str());
		
		std::stringstream fragmentShaderSource;
		fragmentShaderSource
		<< "#version 120\n"
		<< "#extension GL_EXT_gpu_shader4 : enable\n"
		<< "#extension GL_ARB_texture_rectangle: enable\n"
		<< "varying vec2 TexCoord;\n"
		<< "uniform sampler2DRect tex0;\n"
		<< "void main(void)\n"
		<< "{"
		<< "  vec4 originalColor    = texture2DRect(tex0, TexCoord);\n"
		<< "  vec4 Color     = vec4(originalColor.rgb,1);\n"
		<< "  gl_FragColor          = Color.argb;\n"
		<< "}\n"
		;
		EncodingShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSource.str());

		EncodingShader.linkProgram();
		EncodedShaderInitialized = true;
	}
	
	resize(_numLEDs);
}

//--------------------------------------------------------------
ofxLEDsP9813::~ofxLEDsP9813()
{}

//--------------------------------------------------------------
void
ofxLEDsP9813::resize(size_t _numLEDs)
{
	numLEDs = _numLEDs;
	stripRect.set(0, 0, _numLEDs, 1);
	
	DataStart   = 0;
	PixelsStart = 4;
	PixelsEnd   = PixelsStart + (4*numLEDs);
	LatchStart  = PixelsEnd;
	DataEnd     = PixelsEnd + 8;
	
	size_t latchSize = 8;
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
ofxLEDsP9813::clear(const ofColor& c)
{
	ofxLEDsImplementation::clear(c);
	
	uint8_t pixel[3] = { (c.g>>1) | 0x80, (c.r>>1) | 0x80, (c.b>>1) | 0x80 };
	for (size_t i=0; i<numLEDs; ++i)
		memcpy(&txBuffer[PixelsStart + (3*i)], pixel, 3);
	
	needsEncoding = false;
}
//--------------------------------------------------------------
void
ofxLEDsP9813::setPixels(std::vector<ofColor>colors)
{
	
//	ofxLEDsImplementation::clear(ofColor::black);
	
	for (size_t i=0; i<numLEDs; ++i)
	{
		if(i<colors.size())
		{
			uint8_t pixel[3] = { (colors[i].g>>1) | 0x80, (colors[i].r>>1) | 0x80, (colors[i].b>>1) | 0x80 };
			memcpy(&txBuffer[PixelsStart + (3*i)], pixel, 3);
		}
	}
	
	needsEncoding = false;
}
//--------------------------------------------------------------
void
ofxLEDsP9813::setPixels(unsigned char*colors ,int _size)
{
//	ofxLEDsImplementation::clear(ofColor::black);
	
	for (size_t i=0; i<numLEDs; ++i)
	{
		if(i<_size)
		{
			uint8_t pixel[3] = { (colors[i*3+1]>>1) | 0x80,  (colors[i*3]>>1)| 0x80, (colors[i*3+2]>>1) | 0x80 };
			memcpy(&txBuffer[PixelsStart + (3*i)], pixel, 3);
		}
	}
	
	needsEncoding = false;
}

//--------------------------------------------------------------
void
ofxLEDsP9813::encode()
{
//	ofMutex::ScopedLock lock(txBufferMutex);
	
	encodedBuffer.begin();
	{
		EncodingShader.begin();
		{
			renderBuffer.getTextureReference().draw(stripRect);
		}
		EncodingShader.end();
	}
	encodedBuffer.end();
	
	ofTexture& dataTexture(encodedBuffer.getTextureReference());
	dataTexture.bind();
	{
#ifndef TARGET_OPENGLES
		
		// These pixels are swizzled into a 2nd array for FTDI Write
		glGetTexImage(dataTexture.getTextureData().textureTarget, 0,
					  GL_RGBA, GL_UNSIGNED_BYTE,
					  &txBuffer[PixelsStart]);
		for(int i = 0 ; i < numLEDs ; i++)
		{
			int index = i*4+PixelsStart;
			uint8 r = txBuffer[index+1];
			uint8 g = txBuffer[index+2];
			uint8 b = txBuffer[index+3];
			txBuffer[index] = makeFlag (r,g,b);

//			printf("index : %i - %u\n",index, txBuffer[index]);
		}
#else
		glReadPixels(0,
					 0,
					 stripRect.width,
					 stripRect.height,
					 GL_RGB, GL_UNSIGNED_BYTE,
					 &txBuffer[PixelsStart]);

#endif
	}
	dataTexture.unbind();
	
	needsEncoding = false;
}
uint8_t ofxLEDsP9813::makeFlag (uint8_t red, uint8_t green, uint8_t blue)
{
	uint8_t flag=0;
	
	flag =  (red&0xc0)>>6;
	flag |= (green&0xc0)>>4;
	flag |= (blue&0xc0)>>2;
	
	return ~flag;

}

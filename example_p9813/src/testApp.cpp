#include "testApp.h"
int row = 1;
int col = 5;
int numLED = col*row;
float ptSize = 5;
//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(12);
	led = new ofxLEDsP9813(numLED);
	for(int i = 0 ; i< numLED ; i++)
	{
		float x =  (i%col)*ptSize;
		float y =  (i/col)*ptSize;
		led->addLED(i,ofVec2f(x,y));
	}
	serial.listDevices();
	serial.setup(serial.getDeviceList()[5].getDeviceName(), 230400);
}

//--------------------------------------------------------------
void testApp::update(){
	led->clear(0);
	led->clear(ofColor::black);
	led->renderBuffer.begin();
	
	
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	for(int i = 0 ; i < 1; i++)
	{
		ofSetColor(255);
		ofCircle(int(ofGetFrameNum())%numLED, 0, 1);
	}
	ofDisableBlendMode();
	ofPopStyle();
	
	led->renderBuffer.end();
	
	led->encode();
//	#define Debug
	
#ifdef Debug
	printf( "\n------------------------------------\n");
	for(int i = 0; i < led->txBuffer.size() ;i++)
	{
		
		printf( "%u |",led->txBuffer[i]);
		if(i!=0 && (i+1)%4==0)
			printf("\n");
		/*
		 * first 4 byte			0| 0| 0| 0
		 * pixel				flag| r| g| b
		 * .
		 * .
		 * first last 4 byte	0| 0| 0| 0
		 * second last 4 byte	0| 0| 0| 0
		 */
	}
	printf( "\n------------------------------------\n");
#endif
	serial.writeBytes((unsigned char*)led->txBuffer.data(), led->txBuffer.size());
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofScale(10, 10);
	led->renderBuffer.draw(0,0);
						   
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}
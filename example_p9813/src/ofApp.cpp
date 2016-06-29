#include "ofApp.h"
int row = 1;
int col = 5;
int numLED = col*row;
float ptSize = 5;
//--------------------------------------------------------------
void ofApp::setup(){
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
void ofApp::update(){
	led->clear(0);
	led->clear(ofColor::black);
	led->renderBuffer.begin();
	
	
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	for(int i = 0 ; i < 1; i++)
	{
		ofSetColor(ofColor::fromHsb(ofGetFrameNum(),255,255));
		ofCircle(int(ofGetFrameNum())%numLED, 0, 1);
	}
	ofDisableBlendMode();
	ofPopStyle();
	
	led->renderBuffer.end();
	
	led->encode();
	#define Debug
	
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
void ofApp::draw(){
	
	ofScale(10, 10);
	led->renderBuffer.draw(0,0);
						   
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
	
}
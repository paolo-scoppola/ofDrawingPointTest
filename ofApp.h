#pragma once

#include "ofMain.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();

	void resetMap();
	void resetInt(int & value);
	void resetBool(bool & value);

	void keyPressed(int key);

	ofParameter<int> numPoints;
	ofParameter<float> pointSize;
	ofParameter<int> opacity;
	ofParameter<int> blendMode;
	ofParameter<bool> depthTest;
	ofParameter<bool> enableSort;

	ofParameter<bool> enablePoints;
     
	ofxPanel gui;
	bool bHide;

	ofMesh planeMesh;
	ofTexture pointsTexture;
	int size;
	
};

static ofMatrix4x4 mat;

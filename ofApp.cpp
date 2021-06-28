#include "ofApp.h"



void ofApp::setup() {
	
	ofSetFrameRate(60);
	size = 1024;

    gui.setup("Control Panel"); 
	
	gui.add(numPoints.set("Num Points", 12, 4, size / 4));
	gui.add(pointSize.set("Point Size", 1.0f, 0.1f, 50.0f));
	gui.add(opacity.set("Opacity", 255, 0 , 255));
	gui.add(blendMode.set("Blend Mode", 0, 0, 2));
	gui.add(depthTest.set("Depth Test", true));
	gui.add(enableSort.set("Sort", true));
	gui.add(enablePoints.set("Points", false));
	gui.loadFromFile("settings.xml");

	numPoints.addListener(this, &ofApp::resetInt);
	enableSort.addListener(this, &ofApp::resetBool);

	ofSetWindowTitle("Point Texture Test");

	resetMap();

	ofDisableArbTex();
	ofLoadImage(pointsTexture,"dot.png");
	ofEnableArbTex();

	bHide = false;
}

//--------------------------------------------------------------

void ofApp::update() {


}

//--------------------------------------------------------------

void ofApp::resetInt(int & value) {

	resetMap();

}

//--------------------------------------------------------------

void ofApp::resetBool(bool & value) {

	resetMap();

}

//--------------------------------------------------------------

void ofApp::resetMap() {

	ofPlanePrimitive plane;

	plane.set(size, size, numPoints, numPoints);
	planeMesh = plane.getMesh();

	ofColor c;

	for (int i = 0; i < numPoints; i++) {

		c.setHsb(ofMap(i, 0, numPoints - 1, 0, 200), 255, 255);

		for (int j = 0; j < numPoints; j++) {

			planeMesh.addColor(c);
		}
	}
}

//--------------------------------------------------------------

void ofApp::draw() {

	ofBackground(0);

	for (int i = 0; i < numPoints * numPoints; i++) {

		ofColor c = planeMesh.getColor(i);
		planeMesh.setColor(i, ofColor(c.r, c.g, c.b, opacity));
	}

	switch (blendMode) {

		case 0:ofEnableBlendMode(OF_BLENDMODE_DISABLED); break;
		case 1:ofEnableBlendMode(OF_BLENDMODE_ALPHA); break;
		case 2:ofEnableBlendMode(OF_BLENDMODE_ADD); break;
	}
	
	ofPushMatrix();

	ofTranslate(size / 2, size / 2, -size / 2);
	ofRotateXDeg(ofMap(ofGetMouseY(), 0, ofGetHeight(), 60, 120));
	ofRotateZDeg(ofMap(ofGetMouseX(), 0, ofGetWidth(), -180, 180));

	glPointSize(pointSize);

	if (depthTest) {

		ofEnableDepthTest();
	}

	if (enablePoints) {

		planeMesh.draw(OF_MESH_POINTS);
	}
	else {

		if (enableSort) {

			GLfloat m[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, m);
			mat.set(m);

			vector<glm::vec3> & v = planeMesh.getVertices();
			vector<ofFloatColor> & c = planeMesh.getColors();
			ofVec3f v0;
			ofVec3f v1;
			glm::vec3 temp;
			ofFloatColor cTemp;

			for (int i = 0; i < v.size(); i++) {
				for (int j = 0; j < v.size(); j++) {

					v0 = v[i];
					v1 = v[j];

					if ((v0 * mat).lengthSquared() >= (v1 * mat).lengthSquared()) {

						temp = v[i];
						v[i] = v[j];
						v[j] = temp;
						cTemp = c[i];
						c[i] = c[j];
						c[j] = cTemp;
					}
				}
			}
		}

		ofDisableArbTex();
		ofEnablePointSprites();
		pointsTexture.bind();
		planeMesh.draw(OF_MESH_POINTS);
		pointsTexture.unbind();
		ofDisablePointSprites();
		ofEnableArbTex();
	}

	ofPopMatrix();

	if (depthTest) {

		ofDisableDepthTest();
	}

	if (!bHide) {

		gui.draw();
	}

	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 20, ofGetHeight() - 28);
}

//--------------------------------------------------------------

void ofApp::keyPressed(int key) {

	switch (key) {

	case 's':
		gui.saveToFile("settings.xml");
		break;

	case 'h':
		bHide = !bHide;
		break;
	}
}


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(300, 6);
	this->triangle_list.insert(this->triangle_list.end(), ico_sphere.getMesh().getUniqueFaces().begin(), ico_sphere.getMesh().getUniqueFaces().end());

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->mesh.clear();
	this->frame.clear();

	for (float radius = 300; radius > 200; radius -= 80) {

		auto noise_seed = ofRandom(1000);
		for (int i = 0; i < this->triangle_list.size(); i++) {

			glm::vec3 avg = (this->triangle_list[i].getVertex(0) + this->triangle_list[i].getVertex(1) + this->triangle_list[i].getVertex(2)) / 3;
			auto noise_value = ofNoise(noise_seed + ofGetFrameNum() * 0.005, avg.y * 0.005, avg.x * 0.005, avg.z * 0.005);

			if (noise_value < 0.5 || noise_value > 0.6) { continue; }

			if (noise_value < 0.52) { noise_value = 0; }
			else if (noise_value > 0.58) { noise_value = 1; }
			else { noise_value = ofMap(noise_value, 0.52, 0.58, 0, 1); }

			vector<glm::vec3> vertices;

			vertices.push_back(glm::normalize(this->triangle_list[i].getVertex(0)) * radius);
			vertices.push_back(glm::normalize(this->triangle_list[i].getVertex(1)) * radius);
			vertices.push_back(glm::normalize(this->triangle_list[i].getVertex(2)) * radius);

			this->mesh.addVertices(vertices);
			this->frame.addVertices(vertices);

			auto color_value = noise_value < 0.5 ? ofMap(noise_value, 0, 0.5, 512, 0) : ofMap(noise_value, 0.5, 1, 0, 512);

			for (int k = 0; k < vertices.size(); k++) {

				this->mesh.addColor(ofColor(200, color_value, color_value, ofMap(radius, 300, 0, 255, 0)));
				this->frame.addColor(ofColor(200, color_value, color_value, ofMap(radius, 300, 0, 255, 0)));
			}

			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 3);

			this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 2);
			this->frame.addIndex(this->frame.getNumVertices() - 2); this->frame.addIndex(this->frame.getNumVertices() - 3);
			this->frame.addIndex(this->frame.getNumVertices() - 3); this->frame.addIndex(this->frame.getNumVertices() - 1);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum());

	this->mesh.draw();
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
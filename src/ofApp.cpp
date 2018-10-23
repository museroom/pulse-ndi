/*

	Spout OpenFrameworks Sender example - ofApp.cpp

    Visual Studio using the Spout SDK

    Search for SPOUT for additions to a typical Openframeworks application

	Copyright (C) 2016 Lynn Jarvis.

	=========================================================================
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================
*/
#include "ofApp.h"

#include <cstdio>
#include <chrono>
#include <Processing.NDI.Lib.h>

// ----------------------
void ofApp::findNdi() {
	
	if(bNdiFound) return;


	//if (!pNDI_find) return 0;

	// Wait until there is one source
	uint32_t no_sources = 0;
	const NDIlib_source_t* p_sources = NULL;
	//while (!no_sources) {	// Wait until the sources on the nwtork have changed
	printf("Looking for sources ...\n");
	NDIlib_find_wait_for_sources(pNDI_find, 5000/* One second */);
	p_sources = NDIlib_find_get_current_sources(pNDI_find, &no_sources);
	// }
	
	if( !no_sources ) return;

	bNdiFound = true;

	// We now have at least one source, so we create a receiver to look at it.
	pNDI_recv = NDIlib_recv_create_v3();
	//if (!pNDI_recv) return 0;

	my_source.p_ndi_name = "";
	my_source.p_url_address =  "";
	my_source = p_sources[n_source];
	// Connect to our sources
	//NDIlib_recv_connect(pNDI_recv, &p_sources[1]);
	NDIlib_recv_connect(pNDI_recv, &my_source);

	// Destroy the NDI finder. We needed to have access to the pointers to p_sources[0]
	NDIlib_find_destroy(pNDI_find);	


	
}

//--------------------------------------------------------------
void ofApp::setup(){
	
	// === NDI
	bNdiFound = false;

	// Create a finder
	pNDI_find = NDIlib_find_create_v2();
	findNdi();

	// === Open Framework init

	ofBackground(0, 0, 0);
	ofEnableNormalizedTexCoords();  // explicitly normalize tex coords for ofBox

    // ====== SPOUT =====
	spoutsender = new SpoutSender;			// Create a Spout sender object
	bInitialized	= false;		        // Spout sender initialization
	strcpy(sendername, "OF Spout Sender");	// Set the sender name
	ofSetWindowTitle(sendername);			// show it on the title bar
	// Create an OpenGL texture for data transfers
	sendertexture = 0; // make sure the ID is zero for the first time
	InitGLtexture(sendertexture, ofGetWidth(), ofGetHeight());
	// Set the window icon from resources
	SetClassLong(GetActiveWindow(), GCL_HICON, (LONG)LoadIconA(GetModuleHandle(NULL), MAKEINTRESOURCEA(IDI_ICON1)));
    // ===================

    // 3D drawing setup for a sender
	glEnable(GL_DEPTH_TEST);							// enable depth comparisons and update the depth buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	ofDisableArbTex();									// needed for textures to work

	// myTextureImage.loadImage("SpoutBox1.png");			// Load a texture image for the demo

	rotX = 0;
	rotY = 0;

	// Spout Test Mode
	testmode_counter = 0;

} // end setup


//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	bool bNdiTimeout;

	
	findNdi();

	// ====== SPOUT =====
	// A render window must be available for Spout initialization and might not be
	// available in "update" so do it now when there is definitely a render window.
	if(!bInitialized) {
		// Create the sender
		bInitialized = spoutsender->CreateSender(sendername, ofGetWidth(), ofGetHeight()); 
	}
	
	testmode_counter++;
	if(bNdiFound) {

		// ===================

		//bNdiFound = false;
		switch (NDIlib_recv_capture_v2(pNDI_recv, &video_frame, &audio_frame, nullptr, 5000))
		{	// No data
		case NDIlib_frame_type_none:
			//printf("No data received.\n");
			break;

		// Video data
		case NDIlib_frame_type_video:
			//printf("Video data received (%dx%d).\n", video_frame.xres, video_frame.yres);
			sprintf(str, "p_sources (%sx%s).\n", &my_source.p_ndi_name, &my_source.p_url_address);
			//ofDrawBitmapString(str, 20, 80);
			//sprintf(str, "Video data received (%dx%d).\n", video_frame.xres, video_frame.yres);
			//ofDrawBitmapString(str, 20, 50);
			myTextureImage.setFromPixels( video_frame.p_data, 512, 512, OF_IMAGE_COLOR_ALPHA, false );
			NDIlib_recv_free_video_v2(pNDI_recv, &video_frame);
			
			//bNdiFound = true;
			testmode_counter = 0;
			break;

		// Audio data
		case NDIlib_frame_type_audio:
			//printf("Audio data received (%d samples).\n", audio_frame.no_samples);
			NDIlib_recv_free_audio_v2(pNDI_recv, &audio_frame);
		
			//bNdiFound = true;
			testmode_counter = 0;
			break;
		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - -
	// Draw 3D graphics demo - this could be anything
	/*ofPushMatrix();
	ofTranslate((float)ofGetWidth()/2.0, (float)ofGetHeight()/2.0, 0);
	ofRotateY(rotX); // rotate - must be float
	ofRotateX(rotY);*/
	
	if( testmode_counter > 0 ) {
		int testmode_index = testmode_counter % 3;
		float testmode_level = 0.7;
		int testmode_color[4][3] = {{0,0,255},{0,255,0},{255,0,0},{255,255,255}};
		ofBackground( 
			int(testmode_color[testmode_index][0] * testmode_level),
			int(testmode_color[testmode_index][1] * testmode_level),
			int(testmode_color[testmode_index][2] * testmode_level)
		);
	}
	else {	
		ofBackground(0, 0, 0);
		if( bNdiFound ) {
			myTextureImage.getTextureReference().bind(); // bind our texture
			//ofDrawBox(0.4*(float)ofGetHeight()); // Draw the graphic
			ofDrawPlane(256,256,512,512);
			myTextureImage.getTextureReference().unbind(); // bind our texture
		}
	}
	/*ofPopMatrix();
	rotX+=0.5;
	rotY+=0.5;*/

	// ====== SPOUT =====
	//if(bInitialized) {

    if(ofGetWidth() > 0 && ofGetHeight() > 0) { // protect against user minimize

        // Grab the screen into the local spout texture
        glBindTexture(GL_TEXTURE_2D, sendertexture);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, ofGetWidth(), ofGetHeight());
        glBindTexture(GL_TEXTURE_2D, 0);

        // Send the texture out for all receivers to use
        spoutsender->SendTexture(sendertexture, GL_TEXTURE_2D, ofGetWidth(), ofGetHeight());

        // Show what it is sending
        //ofSetColor(255);
        sprintf(str, "Sending as : [%s]", sendername);
        //ofDrawBitmapString(str, 20, 20);

        // Show fps
        sprintf(str, "fps: %3.3d", (int)ofGetFrameRate());
        //ofDrawBitmapString(str, ofGetWidth()-120, 20);

    }
	//}
    // ===================


}

//--------------------------------------------------------------
void ofApp::exit() {
	// Destroy the receiver
	if( pNDI_recv) {
		NDIlib_recv_destroy(pNDI_recv);
	}

	// Not required, but nice
	NDIlib_destroy();

    // ====== SPOUT =====
	spoutsender->ReleaseSender(); // Release the sender

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) 
{
	
	// ====== SPOUT =====
	// Update the sender texture to receive the new dimensions
	// Change of width and height is handled within the SendTexture function
	if(w > 0 && h > 0) // protect against user minimize
        InitGLtexture(sendertexture, w, h);
	
}

// ====== SPOUT =====
bool ofApp::InitGLtexture(GLuint &texID, unsigned int width, unsigned int height)
{
    if(texID != 0) glDeleteTextures(1, &texID);

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

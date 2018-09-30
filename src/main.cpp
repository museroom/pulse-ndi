#include "ofMain.h"
#include "ofApp.h"

#include <cstdio>
#include <chrono>
#include <Processing.NDI.Lib.h>

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x64.lib")
#else // _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x86.lib")
#endif // _WIN64
#endif // _WIN32

/*
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

//--------------------------------------------------------------
// to change options for console window (Visual Studio)
//
// Properties > Linker > System > Subsystem
//    for console : Windows (/SUBSYSTEM:CONSOLE)
//
//    for Window : Windows (/SUBSYSTEM:WINDOWS)
//
// Click APPLY and OK. Then make changes to Main as below
//--------------------------------------------------------------

// for default console
//========================================================================
// int main() {
//
// for window without console
//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) 
/*int CALLBACK WinMain(  
  _In_ HINSTANCE hInstance,  
  _In_ HINSTANCE hPrevInstance,  
  _In_ LPSTR     lpCmdLine,  
  _In_ int       nCmdShow  
)*/
//int main() 
{
	int i;
	for( i = 0; i< 2; i++ ) {
		i = i + 1;
	}

	if (!NDIlib_initialize()) return 0;
	

	ofSetupOpenGL(512, 512, OF_WINDOW); // <-------- setup the GL context
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofApp *app = new ofApp();
	app->n_source = 0;
	/*if( strlen( lpCmdLine ) > 0 ) {
		app->n_source = (int)lpCmdLine[0] - 65;
		ShowWindow( ofGetWin32Window(), SW_HIDE );
	}*/
	ofRunApp( app );

	return 0;
}

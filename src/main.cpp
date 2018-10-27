#define TARGET_NODISPLAY

#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"


#include <cstdio>
#include <wchar.h>
#include <chrono>
#include <iostream>
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

char **argv = NULL;
bool winshow = 0;

/*******************************************************
WIN32 command line parser function
********************************************************/
int ParseCommandline( )
{
	int    nArgs, i;
	WCHAR  *wcCommandLine;
	LPWSTR *szArglist;
	int boygroup_id;

	// Get a WCHAR version of the parsed commande line
	wcCommandLine = GetCommandLineW();	
	szArglist = CommandLineToArgvW( wcCommandLine, &nArgs);
	if( NULL == szArglist ) {
		cout << "argumetns: -b,--boygroup [boygroup id]" <<endl;
		cout << "           -s,--show " << endl;
		return 0;
	} else {
		for( i=0; i<nArgs; i++) { 
			LPWSTR arg = szArglist[i];
			if( !wcscmp(arg, L"--show") || !wcscmp(arg,L"-s") ) {
					winshow = 1; 
			}
			if( !wcscmp(arg,L"--boygroup") || !wcscmp(arg, L"-b") ) {	
				arg = szArglist[++i];
				int arglen = wcslen( arg );
				
				if( wcslen( arg ) != 1 ) 
					return 0;	

				if(!wcscmp(arg,L"A")) boygroup_id = 0;
				if(!wcscmp(arg,L"B")) boygroup_id = 1;
				if(!wcscmp(arg,L"C")) boygroup_id = 2;
				if(!wcscmp(arg,L"D")) boygroup_id = 3;
				if(!wcscmp(arg,L"E")) boygroup_id = 0;
			}
		}
	}
	LocalFree(szArglist);
	// return the number of argument
	return boygroup_id;
}

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
	int i, boygroup_id;
	
	if (!NDIlib_initialize()) return 0;
	ofAppNoWindow window;
	//ofSetupOpenGL( &window, 512, 512, OF_WINDOW );
	

	ofSetupOpenGL(512, 512, OF_WINDOW); // <-------- setup the GL context
	
	boygroup_id = ParseCommandline();
	
	if( !winshow ) {
		ofSetWindowPosition(-512,0);
		ShowWindow( ofGetWin32Window(), SW_HIDE );
	}

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofApp *app = new ofApp();


	// Parse command line

	app->n_source = 0;
	app->n_source = boygroup_id;
	ofRunApp( app );

	return 0;
}

#if WIN32
	#include <windows.h>
#endif

#include <iostream>
#include "Viewer.h"

#include "Grid.h"
#include "Renderable.h"

int main(int argc, char** argv){

	#if WIN32
		HWND consoleWindow = GetConsoleWindow();
		SetWindowPos(consoleWindow, 0, 0, 550, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	#endif

	Renderable *grid = new Grid(10, -2);

	Viewer viewer(argc, argv);
	
	viewer.addToRender(grid);
	
	viewer.start();

	return 0;
}
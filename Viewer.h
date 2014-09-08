#ifndef VIEWER_H_
#define VIEWER_H_

#include <vector>
#include <sstream>

#include <GL/glut.h>

#include "Kinect.h"
#include "Vector3D.h"
#include "Camera.h"
#include "Constants.h"
#include "ini/INIReader.h"


using namespace std;

/**
 * Object oriented implementation of gl rendering.
 * Object container, camera and events handling
 */
class Viewer {
public:
	/**
	 * Used for dispatching functions
	 */
	static Viewer *instance;

	/**
	 * Constructor
	 *
	 * @param (main)
	 */
	Viewer(int argc, char** argv);
	~Viewer();

	void addToRender(Renderable *rend);

	/**
	 * Begin main loop
	 */
	void start();

	/**
	 * Starts the timer
	 */
	void startTimer();

private:

	vector<Renderable *> renderables;

	int sceneWindow, rgbWindow;

	Kinect kinect;

	/**
	 * Camera
	 */
	Camera camera;

	
	/**
	 * Mouse tracking
	 */
	int sX, sY, eX, eY;
	bool mousePros;

	/**
	 * String drawing color
	 */
	float stringColor[4];

	/**
	 * Setup
	 */
	void init();

	/**
	 * Rendering function
	 */
	void renderScene();
	void renderStream();

	/**
	 * Idle function
	 */
	void update();

	void reshape (int w, int h);
	
	/**
	 * Keyboard, mouse listeners
	 */
	void keyboard (unsigned char key, int x, int y);
	void mouseButton(int button, int state, int x, int y);
	void mouseMove(int x, int y);

	void drawString(const char *str, int x, int y,
			float color[4], void *font);

	/**
	 * Info
	 */
	void showInfo();

	/**
	 * For setting callbacks
	 */
	static void setSceneDisplayFunction();
	static void setStreamDisplayFunction();
	static void setIdeleFunction();
	static void setReshapeFunction(int w, int h);

	static void setKeybordFunction(unsigned char key, int x, int y);
	static void setMouseFunction(int button, int state, int x, int y);
	static void setMouseMoveFunction(int x, int y);

	/**
	* Used when timer is configured from ini files to dispach autocall function
	*/
	static void timerDispacher(int i);

};

#endif /* VIEWER_H_ */

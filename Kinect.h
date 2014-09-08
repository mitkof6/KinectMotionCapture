#ifndef KINECT_H_
#define KINECT_H_

#include <Windows.h>
#include <iostream>
#include <ctime>

#include <GL/glut.h>
#include <NuiApi.h>

#include "Skeleton.h"
#include "Renderable.h"
#include "ini/INIReader.h"

/**
* This class represents the interface between kinect sensor
* and the application
*/
class Kinect : public Renderable{
public:
	//skeleton sequence contaier
	Skeleton skeleton;

	Kinect();
	~Kinect(void);

	/**
	* Draw for rendering
	*/
	void draw();
	/**
	* Update for rendering and for polling sensor data
	*/
	void update();

	/**
	* Calculates skeleton frame rate (used in viewer)
	*/
	int getSkeletonFrameRate();

private:
	/**
	* Choice for filtering 1, 2, 3
	*/
	int FILTER_MODE;

	long int timeStamp;

	/**
	* Sensor instance
	*/
	INuiSensor* sensor;

	/**
	* Used for handling data events
	*/
	HANDLE skeletonHandle, rgbHandle;
	HANDLE rgbStreamHandle;

	/**
	* For counting time and frame rate
	*/
	clock_t frameBegin, frameEnd, currentTime, previousTime;
	int fps, frameCount;

	/**
	* Starts streaming
	*/
	HRESULT connect();

	/**
	* Mangaes skeleton data from kinect
	*/
	void prosessSkeletonData();

	/**
	* Manages rgb data from kinect
	*/
	void prosessColorData();

	/**
	* Used to indecate FPS
	*/
	void calculateFPS();

};


#endif


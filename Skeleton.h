#ifndef SKELETON_H_
#define SKELETON_H_

#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>

#include <GL/glut.h>
#include <NuiApi.h>

#include "Renderable.h"
#include "Constants.h"
#include "Vector3D.h"
#include "Grid.h"
#include "ini/INIReader.h"


using namespace std;

/**
* This class handles skeleton data coming from kinect sensor.
* Draw the last frame, stores the data to different formats.
*/
class Skeleton : public Renderable{
public:
	/**
	* Used for animation of captured sequence
	*/
	bool animate;

	Skeleton(void);
	~Skeleton(void);

	/**
	* Draw last captured frame
	*/
	virtual void draw();

	/**
	* Update not used
	*/
	virtual void update();

	/**
	* Adds data and arrival time coming from sensor
	*/
	void addFrame(const NUI_SKELETON_DATA &data, const NUI_SKELETON_BONE_ORIENTATION orientaion[], double &elapsedTime, long int &timeStamp);

	/**
	* Saves to raw binnary format
	*/
	void saveVectorToBinary();

	/**
	* Loads from binnary format
	*/
	void loadVectorFromBinary();

	/**
	* Joints positions to (x, y, z) matlab .dat
	*/
	void saveToMatlab();

	/**
	* Saves to OpenSim *.trc motion format
	*/
	void saveMotionToOpenSim(int fps);

	/**
	* Calculates avg body segment's length
	*/
	void saveSegmentLengths();
	
	/**
	* Sets floor Y coordinate
	*/
	void setFloor(float y);

	/**
	* Clears captured data
	*/
	void clearData();
private:
	/**
	* Keeps track of arrival time
	*/
	vector<double> frameTime;
	vector<long int> timeStamp;

	/**
	* Skeleton frame data
	*/
	vector<NUI_SKELETON_DATA> skeletonSequence;
	vector< vector<NUI_SKELETON_BONE_ORIENTATION> > skeletonBoneOrientation;

	/**
	* Used for xml generation
	*/
	double startTime, endTime;

	/**
	* Bone segments 
	*/
	int pairs[PAIRS][2];

	/**
	* Markers map
	*/
	map<string, int> markers;
	/**
	* Markers names
	*/
	vector<string> markersNames;

	/**
	* Current animated frame
	*/
	int animateIndex;

	/**
	* Floor Y coordinats
	*/
	float floorY;

	/**
	* File path loaded from ini files
	*/
	string BINARY_SAVE_PATH, BINARY_LOAD_PATH, 
		MATLAB_X_PATH, MATLAB_Y_PATH, MATLAB_Z_PATH, MATLAB_W_PATH,
		OPENSIM_MOTION_FILE, OPENSIM_SCALE_SETTINGS, 
		OPENSIM_IK_SETTINGS, OPENSIM_ID_DYNAMICS,
		SEGMENTS_AVG_PATH;

	/**
	* Euclidean distance
	*/
	double distance(Vector4 p, Vector4 q);

	/**
	* Not functional
	*/
	void generateOpenSimScaleSettings();
	void generateOpenSimIKSettings();
	void generateIDSettings();

	/**
	* Draws a body segment
	*/
	void drawBone(const NUI_SKELETON_DATA &skeleton,
		int jointFrom, int jointTo);
	/**
	* Draws a joint sphere
	*/
	void drawJoint(const NUI_SKELETON_DATA &skeleton, int joint);

	void drawBoneOrientation(const vector<NUI_SKELETON_BONE_ORIENTATION> &orientation, const NUI_SKELETON_DATA &skeleton);
};

#endif


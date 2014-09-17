#include "Skeleton.h"

Skeleton::Skeleton(void){
	int tempP[PAIRS][2] = { 
		{NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER},
		{NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT},
		{NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT},
		{NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE},
		{NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER},
		{NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT},
		{NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT},

		// Left Arm
		{NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT},
		{NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT},
		{NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT},

		// Right Arm
		{NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT},
		{NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT},
		{NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT},

		// Left Leg
		{NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT},
		{NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT},
		{NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT},

		// Right Leg
		{NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT},
		{NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT},
		{NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT}
	};

	memcpy(pairs, tempP, sizeof(int) * PAIRS * 2);

	markers["NUI_SKELETON_POSITION_HIP_CENTER"] = 0;
	markers["NUI_SKELETON_POSITION_SPINE"] = 1;
	markers["NUI_SKELETON_POSITION_SHOULDER_CENTER"] = 2;
	markers["NUI_SKELETON_POSITION_HEAD"] = 3;
	markers["NUI_SKELETON_POSITION_SHOULDER_LEFT"] = 4;
	markers["NUI_SKELETON_POSITION_ELBOW_LEFT"] = 5;
	markers["NUI_SKELETON_POSITION_WRIST_LEFT"] = 6;
	markers["NUI_SKELETON_POSITION_HAND_LEFT"] = 7;
	markers["NUI_SKELETON_POSITION_SHOULDER_RIGHT"] = 8;
	markers["NUI_SKELETON_POSITION_ELBOW_RIGHT"] = 9;
	markers["NUI_SKELETON_POSITION_WRIST_RIGHT"] = 10;
	markers["NUI_SKELETON_POSITION_HAND_RIGHT"] = 11;
	markers["NUI_SKELETON_POSITION_HIP_LEFT"] = 12;
	markers["NUI_SKELETON_POSITION_KNEE_LEFT"] = 13;
	markers["NUI_SKELETON_POSITION_ANKLE_LEFT"] = 14;
	markers["NUI_SKELETON_POSITION_FOOT_LEFT"] = 15;
	markers["NUI_SKELETON_POSITION_HIP_RIGHT"] = 16;
	markers["NUI_SKELETON_POSITION_KNEE_RIGHT"] = 17;
	markers["NUI_SKELETON_POSITION_ANKLE_RIGHT"] = 18;
	markers["NUI_SKELETON_POSITION_FOOT_RIGHT"] = 19;

	animate = false;
	animateIndex = 0;

	INIReader ini(INI_FILE_PATH);
	
	BINARY_SAVE_PATH = ini.Get("path", "BINARY_SAVE_PATH", "");
	BINARY_LOAD_PATH = ini.Get("path", "BINARY_LOAD_PATH", ""); 
	MATLAB_X_PATH = ini.Get("path", "MATLAB_X_PATH", ""); 
	MATLAB_Y_PATH = ini.Get("path", "MATLAB_Y_PATH", "");
	MATLAB_Z_PATH = ini.Get("path", "MATLAB_Z_PATH", ""); 
	MATLAB_Z_PATH = ini.Get("path", "MATLAB_W_PATH", ""); 
	OPENSIM_MOTION_FILE = ini.Get("path", "OPENSIM_MOTION_FILE", ""); 
	OPENSIM_SCALE_SETTINGS = ini.Get("path", "OPENSIM_SCALE_SETTINGS", "");
	OPENSIM_IK_SETTINGS = ini.Get("path", "OPENSIM_IK_SETTINGS", ""); 
	OPENSIM_ID_DYNAMICS = ini.Get("path", "OPENSIM_ID_DYNAMICS", ""); 
	SEGMENTS_AVG_PATH = ini.Get("path", "SEGMENTS_AVG_PATH", "");
	SENSOR_TRAJECTORY = ini.Get("path", "SENSOR_TRAJECTORY", "");

	string iniMarkers  = ini.Get("openSimMarkers", "MARKERS", "");  
    stringstream ss(iniMarkers);
	string buf;
    while (ss >> buf){
        markersNames.push_back(buf);
	}

	iniMarkers  = ini.Get("sensor", "MARKERS", "");  
    ss = stringstream(iniMarkers);
	buf = "";
    while (ss >> buf){
        sensorNames.push_back(buf);
	}

	

}


Skeleton::~Skeleton(void){

}


void Skeleton::draw(){
	
	if(skeletonSequence.size() == 0) return;

	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	if(animate){
		//draw bones
		for(int i = 0;i<PAIRS;i++){
			glColor3d(0, 1.0, 0);
			glLineWidth(BONE_WIDTH);
			drawBone(skeletonSequence.back(), pairs[i][0], pairs[i][1]);	
			glLineWidth(1);
			//drawBoneOrientation(skeletonBoneOrientation.back(), skeletonSequence.back());
		}

		//draw joints
		glColor3d(1, 0, 0);
		for(int i = 0;i<JOINTS;i++){
			drawJoint(skeletonSequence[animateIndex], i);	
		}
		animateIndex++;
		if(animateIndex == skeletonSequence.size()){
			animateIndex = 0;
		}
	}else{

		

		//draw bones
		for(int i = 0;i<PAIRS;i++){
			glColor3d(0, 1.0, 0);
			glLineWidth(BONE_WIDTH);
			drawBone(skeletonSequence.back(), pairs[i][0], pairs[i][1]);	
			glLineWidth(1);
			//drawBoneOrientation(skeletonBoneOrientation.back(), skeletonSequence.back());
		}

		//draw joints
		glColor3d(1, 0, 0);
		for(int i = 0;i<JOINTS;i++){
			drawJoint(skeletonSequence.back(), i);	
		}

		
	}

	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopAttrib();
}

void Skeleton::drawBone(const NUI_SKELETON_DATA &skeleton, 
	int jointFrom, int jointTo){


	NUI_SKELETON_POSITION_TRACKING_STATE jointFromState 
		= skeleton.eSkeletonPositionTrackingState[jointFrom];
    NUI_SKELETON_POSITION_TRACKING_STATE jointToState 
		= skeleton.eSkeletonPositionTrackingState[jointTo];

    if (jointFromState == NUI_SKELETON_POSITION_NOT_TRACKED ||
		jointToState == NUI_SKELETON_POSITION_NOT_TRACKED){
          return; // nothing to draw, one of the joints is not tracked
    }

    const Vector4& jointFromPosition = skeleton.SkeletonPositions[jointFrom];
    const Vector4& jointToPosition = skeleton.SkeletonPositions[jointTo];

    // Don't draw if both points are inferred
  //  if (jointFromState == NUI_SKELETON_POSITION_INFERRED ||
		//jointToState == NUI_SKELETON_POSITION_INFERRED){
		//DrawNonTrackedBoneLine(jointFromPosition, jointToPosition); 
		//// Draw thin lines if either one of the joints is inferred
  //  }

    // We assume all drawn bones are inferred unless BOTH joints are tracked
	if (jointFromState == NUI_SKELETON_POSITION_TRACKED &&
			jointToState == NUI_SKELETON_POSITION_TRACKED){
        
		glBegin(GL_LINES);
		
		glVertex3d(
			SCALE * jointFromPosition.x/jointFromPosition.w, 
			SCALE * jointFromPosition.y/jointFromPosition.w, 
			SCALE * jointFromPosition.z/jointFromPosition.w);
		glVertex3d(
			SCALE * jointToPosition.x/jointToPosition.w,
			SCALE * jointToPosition.y/jointToPosition.w,
			SCALE * jointToPosition.z/jointToPosition.w);
		
		
		glEnd();
    }
}

void Skeleton::drawJoint(const NUI_SKELETON_DATA &skeleton, int joint){
	NUI_SKELETON_POSITION_TRACKING_STATE jointState 
		= skeleton.eSkeletonPositionTrackingState[joint];


    if (jointState == NUI_SKELETON_POSITION_NOT_TRACKED){
          return; // nothing to draw, one of the joints is not tracked
    }

    const Vector4& jointPosition = skeleton.SkeletonPositions[joint];

	glPushMatrix();

		glTranslated(
			SCALE*jointPosition.x/jointPosition.w,
			SCALE*jointPosition.y/jointPosition.w,
			SCALE*jointPosition.z/jointPosition.w);
		glutSolidSphere(JOINT_RADIUS, 15, 15);

	glPopMatrix();
}

void Skeleton::drawBoneOrientation(const vector<NUI_SKELETON_BONE_ORIENTATION> &orientation, const NUI_SKELETON_DATA &skeleton){

	for(unsigned i = 0;i<NUI_SKELETON_POSITION_COUNT;i++){

		NUI_SKELETON_BONE_ROTATION rot = orientation[i].absoluteRotation;
		
		glPushMatrix();
			
			glTranslatef(
				SCALE*skeleton.SkeletonPositions[i].x/skeleton.SkeletonPositions[i].w, 
				SCALE*skeleton.SkeletonPositions[i].y/skeleton.SkeletonPositions[i].w, 
				SCALE*skeleton.SkeletonPositions[i].z/skeleton.SkeletonPositions[i].w);
			glRotatef(
				rot.rotationQuaternion.w*57, 
				rot.rotationQuaternion.x, 
				rot.rotationQuaternion.y, 
				rot.rotationQuaternion.z);

			glBegin(GL_LINES);
				glColor3f(1, 0, 0);
				glVertex3f(0, 0, 0);
				glVertex3f(1, 0, 0);
		
				glColor3f(0, 1, 0);
				glVertex3f(0, 0, 0);
				glVertex3f(0, 1, 0);

				glColor3f(0, 0, 1);
				glVertex3f(0, 0, 0);
				glVertex3f(0, 0, 1);
			glEnd();

			//Multi-colored side - FRONT
			glBegin(GL_POLYGON);
 
			glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  0.5, -0.5, -0.5 );      // P1 is red
			glColor3f( 0.0, 1.0, 0.0 );     glVertex3f(  0.5,  0.5, -0.5 );      // P2 is green
			glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -0.5,  0.5, -0.5 );      // P3 is blue
			glColor3f( 1.0, 0.0, 1.0 );     glVertex3f( -0.5, -0.5, -0.5 );      // P4 is purple
 
			glEnd();

			// White side - BACK
			glBegin(GL_POLYGON);
			glColor3f(   1.0,  1.0, 1.0 );
			glVertex3f(  1, -1, 1 );
			glVertex3f(  1,  1, 1 );
			glVertex3f( -1,  1, 1 );
			glVertex3f( -1, -1, 1 );
			glEnd();
 
			// Purple side - RIGHT
			glBegin(GL_POLYGON);
			glColor3f(  1.0,  0.0,  1.0 );
			glVertex3f( 1, -1, -1 );
			glVertex3f( 1,  1, -1 );
			glVertex3f( 1,  1,  1 );
			glVertex3f( 1, -1,  1 );
			glEnd();
 
			// Green side - LEFT
			glBegin(GL_POLYGON);
			glColor3f(   0.0,  1.0,  0.0 );
			glVertex3f( -1, -1,  1 );
			glVertex3f( -1,  1,  1 );
			glVertex3f( -1,  1, -1 );
			glVertex3f( -1, -1, -1 );
			glEnd();
 
			// Blue side - TOP
			glBegin(GL_POLYGON);
			glColor3f(   0.0,  0.0,  1.0 );
			glVertex3f(  1,  1,  1 );
			glVertex3f(  1,  1, -1 );
			glVertex3f( -1,  1, -1 );
			glVertex3f( -1,  1,  1 );
			glEnd();
 
			// Red side - BOTTOM
			glBegin(GL_POLYGON);
			glColor3f(   1.0,  0.0,  0.0 );
			glVertex3f(  1, -1, -1 );
			glVertex3f(  1, -1,  1 );
			glVertex3f( -1, -1,  1 );
			glVertex3f( -1, -1, -1 );
			glEnd();
		glPopMatrix();
	}
	
}

void Skeleton::update(){

}

void Skeleton::addFrame(const NUI_SKELETON_DATA &data, const NUI_SKELETON_BONE_ORIENTATION orientaion[],
	double &elapsedTime, SYSTEMTIME &time_stamp){
	
	double tem_time;

	if(!frameTime.size() == 0){
		tem_time = frameTime.back() + elapsedTime;		
	}else{
		tem_time = 0;	
	}
	//printf("%f\n", time);
	frameTime.push_back(tem_time);
	skeletonSequence.push_back(data);
	
	vector<NUI_SKELETON_BONE_ORIENTATION> temp;
	for(unsigned i = 0;i<NUI_SKELETON_POSITION_COUNT;i++){
		temp.push_back(orientaion[i]);
	}
	skeletonBoneOrientation.push_back(temp);

	timeStamp.push_back(time_stamp);

}

void Skeleton::saveSensorTrajectory(){
	if(skeletonSequence.size() == 0){
		return;
	}

	FILE *f = fopen(SENSOR_TRAJECTORY.c_str(), "w");
	if(f == NULL){
		printf("Can't open file\n");
		return;
	}

	for(unsigned i = 0;i<skeletonSequence.size();i++){
		
		if(i<SKIP_FRAMES){
			continue;
		}
		
		//fprintf(f, "%ld\t", timeStamp[i]);
		SYSTEMTIME t = timeStamp[i];
		fprintf(f, "%02d%02d%02d%03d\t", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);

		for(int j = 0;j<sensorNames.size();j++){
			Vector4 pos = skeletonSequence[i].SkeletonPositions[markers[sensorNames[j]]];
			pos.x = pos.x ;
			pos.y = pos.y - floorY;
			pos.z = pos.z;

			fprintf(f, "%f\t%f\t%f\t", MARKERS_SCALE*pos.x, MARKERS_SCALE*pos.y, MARKERS_SCALE*pos.z);
		}

		fprintf(f, "\n");
	}

	fclose(f);
}

void Skeleton::saveVectorToBinary(){
	// Write out a list to a disk file
	ofstream out(BINARY_SAVE_PATH, ios::binary);

	int size = skeletonSequence.size();
	out.write((const char* ) &size, 4);//save # of frames
    out.write((const char *) &skeletonSequence[0], size * sizeof(NUI_SKELETON_DATA));//save frames
	out.write((const char *) &frameTime[0], size * sizeof(double));//save time frames

	out.close();
	printf("Total frames: %d\n", size);
}

void Skeleton::loadVectorFromBinary(){
	//TODO
	vector<NUI_SKELETON_DATA> temp;
	vector<double> time;

    ifstream in(BINARY_LOAD_PATH, ios::binary);
    
	int size;
    in.read((char *) &size, 4);//read # of frames
    temp.resize(size);

    in.read((char *) &temp[0], size * sizeof(NUI_SKELETON_DATA));//load frames
	in.read((char *) &time[0], size * sizeof(double));//load time

	printf("Total frames: %d\n", size);
}

void Skeleton::saveToMatlab(){

	if(skeletonSequence.size() == 0){
		return;
	}

	ofstream os;

	for(int k = 0;k<4;k++){
		
		switch(k){
			case 0:
				os.open (MATLAB_X_PATH.c_str());

				for(unsigned i = 0;i<skeletonSequence.size();i++){
					/*os << timeStamp[i] << "\t";
					os << frameTime[i] << "\t";*/
					for(unsigned int j = 0;j<JOINTS;j++){
						os << skeletonSequence[i].SkeletonPositions[j].x << "\t";
					}
					for(unsigned int j = 0;j<JOINTS;j++){
						os << skeletonBoneOrientation[i].at(j).absoluteRotation.rotationQuaternion.x << "\t";
					}
					os << "\n";
				}

				os.close();
				break;
			case 1:
				os.open (MATLAB_Y_PATH.c_str());

				for(unsigned int i = 0;i<skeletonSequence.size();i++){
					/*os << timeStamp[i] << "\t";
					os << frameTime[i] << "\t";*/
					for(int j = 0;j<JOINTS;j++){
						os << skeletonSequence[i].SkeletonPositions[j].y << "\t";
					}
					for(unsigned int j = 0;j<JOINTS;j++){
						os << skeletonBoneOrientation[i].at(j).absoluteRotation.rotationQuaternion.y << "\t";
					}
					os << "\n";
				}

				os.close();
				break;
			case 2:
				os.open (MATLAB_Z_PATH.c_str());

				for(unsigned int i = 0;i<skeletonSequence.size();i++){
					/*os << timeStamp[i] << "\t";
					os << frameTime[i] << "\t";*/
					for(int j = 0;j<JOINTS;j++){
						os << skeletonSequence[i].SkeletonPositions[j].z << "\t";
					}
					for(unsigned int j = 0;j<JOINTS;j++){
						os << skeletonBoneOrientation[i].at(j).absoluteRotation.rotationQuaternion.z << "\t";
					}
					os << "\n";
				}

				os.close();
				break;
			case 3:
				os.open (MATLAB_W_PATH.c_str());

				for(unsigned int i = 0;i<skeletonSequence.size();i++){
					/*os << timeStamp[i] << "\t";
					os << frameTime[i] << "\t";*/
					for(int j = 0;j<JOINTS;j++){
						os << skeletonSequence[i].SkeletonPositions[j].w << "\t";
					}
					for(unsigned int j = 0;j<JOINTS;j++){
						os << skeletonBoneOrientation[i].at(j).absoluteRotation.rotationQuaternion.w << "\t";
					}
					os << "\n";
				}

				os.close();
				break;
			default:
				break;
		}
		
	}
}

void Skeleton::saveMotionToOpenSim(int fps){

	if(skeletonSequence.size() == 0){
		return;
	}

	int frames = skeletonSequence.size()-SKIP_FRAMES;
	
	FILE *f = fopen(OPENSIM_MOTION_FILE.c_str(), "w");
	if(f == NULL){
		printf("Can't open file\n");
		return;
	}

	fprintf(f, "PathFileType	4	(X/Y/Z)	%s\n", OPENSIM_MOTION_FILE.c_str());

	fprintf(f, "DataRate	CameraRate	NumFrames	NumMarkers	Units	OrigDataRate	OrigDataStartFrame	OrigNumFrames\n");

	int nMarkers = markersNames.size();
	fprintf(f, "%d	%d	%d	%d	mm	%d	%d	%d\n", fps, fps, frames, nMarkers, fps, 1, frames);

	fprintf(f, "Frame#	Time");
	for(int i = 0;i<nMarkers;i++){
		fprintf(f, "	%s", markersNames[i].c_str());
	}
	fprintf(f, "\n");

	string axis = "";
	for(int i = 1;i<=nMarkers;i++){
		stringstream ss; 
		ss << "X" << i << "	Y" << i << "	Z" << i << "	";
		axis.append(ss.str());
	}
	fprintf(f, "%s\n", axis.c_str());

	double delay = 0;
	Vector4 joint, foot;
	//float error;
	joint.x = 0;joint.y = 0;joint.z = 0;
	for(unsigned i = 0;i<skeletonSequence.size();i++){
		
		if(i<SKIP_FRAMES){
			delay += frameTime[i];
			continue;
		}

		fprintf(f, "%d	%f	", i-SKIP_FRAMES+1, frameTime[i]-delay);
		
		//joint = skeletonSequence[i].SkeletonPositions[markers["NUI_SKELETON_POSITION_SHOULDER_RIGHT"]];
		
		for(int j = 0;j<nMarkers;j++){
			Vector4 pos = skeletonSequence[i].SkeletonPositions[markers[markersNames[j]]];
			pos.x = pos.x - joint.x;
			pos.y = pos.y - joint.y - floorY;
			pos.z = pos.z - joint.z;

			fprintf(f, "%f	%f	%f	", MARKERS_SCALE*pos.x, MARKERS_SCALE*pos.y, MARKERS_SCALE*pos.z);
		}

		fprintf(f, "\n");
	}
	
	startTime = frameTime[SKIP_FRAMES]-delay;
	endTime = frameTime[frameTime.size()-1]-delay;

    fclose(f);
	
	//TODO
	//opensim xml
	//generateOpenSimScaleSettings();
	//generateOpenSimIKSettings();
	//generateIDSettings();
}

double Skeleton::distance(Vector4 p, Vector4 q){

	return sqrt((p.x-q.x)*(p.x-q.x)+(p.y-q.y)*(p.y-q.y)+(p.z-q.z)*(p.z-q.z));
}

void Skeleton::saveSegmentLengths(){

	if(skeletonSequence.size() == 0){
		return;
	}

	double segments[PAIRS];
	
	//avarage
	for(int i = 0;i<PAIRS;i++){
		segments[i] = 0;
	}

	for(unsigned i = SKIP_FRAMES;i<skeletonSequence.size();i++){

		for(int j = 0;j<PAIRS;j++){
			segments[j] += distance(skeletonSequence[i].SkeletonPositions[pairs[j][0]],
				skeletonSequence[i].SkeletonPositions[pairs[j][1]]);
		}
	}

	FILE *f = fopen(SEGMENTS_AVG_PATH.c_str(), "w");
	if(f == NULL){
		printf("Can't open file\n");
		return;
	}

	int N = skeletonSequence.size()-SKIP_FRAMES;
	for(int i = 0;i<PAIRS;i++){
		segments[i] = segments[i]/N;
		fprintf(f,"%f	", segments[i]);
	}

	fclose(f);
}

void Skeleton::setFloor(float y){
	floorY = (floorY + y)/2;
}

void Skeleton::clearData(){
	skeletonSequence.clear();
	frameTime.clear();
}

void Skeleton::generateOpenSimScaleSettings(){

	FILE *f = fopen(OPENSIM_SCALE_SETTINGS.c_str(), "w");
	if(f == NULL){
		printf("Can't open file\n");
		return;
	}
	
	fprintf(f, 
		"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
		"<OpenSimDocument Version=\"30000\">\n"
		"<ScaleTool name=\"model-scaled\">\n"
		"<mass>41.128</mass>\n"
		"<height>-1</height>\n"
		"<age>-1</age>\n"
		"<notes>Unassigned</notes>\n"
		"<GenericModelMaker>\n"
			"<model_file>model.osim</model_file>\n"
			"<marker_set_file>markers.xml</marker_set_file>\n"
		"</GenericModelMaker>\n"
		"<ModelScaler>\n"
			"<apply>true</apply>\n"
			"<scaling_order> measurements</scaling_order>\n"
			"<MeasurementSet>\n"
				"<objects>\n"
					"<Measurement name=\"rfemur\">\n"
						"<apply>true</apply>\n"
						"<MarkerPairSet>\n"
							"<objects>\n"
								"<MarkerPair>\n"
								"</MarkerPair>\n"
							"</objects>\n"
							"<groups />\n"
						"</MarkerPairSet>\n"
						"<BodyScaleSet>\n"
							"<objects>\n"
								"<BodyScale name=\"femur\">\n"
									"<axes> X Y Z</axes>\n"
								"</BodyScale>\n"
							"</objects>\n"
							"<groups />\n"
						"</BodyScaleSet>\n"
					"</Measurement>\n"
					"<Measurement name=\"lfemur\">\n"
						"<apply>true</apply>\n"
						"<MarkerPairSet>\n"
							"<objects>\n"
								"<MarkerPair>\n"
									"<markers> mHipL mKneeL</markers>\n"
								"</MarkerPair>\n"
							"</objects>\n"
							"<groups />\n"
						"</MarkerPairSet>\n"
						"<BodyScaleSet>\n"
							"<objects>\n"
								"<BodyScale name=\"lfemur\">\n"
									"<axes> X Y Z</axes>\n"
								"</BodyScale>\n"
							"</objects>\n"
							"<groups />\n"
						"</BodyScaleSet>\n"
					"</Measurement>\n"
					"<Measurement name=\"rtibia\">\n"
						"<apply>true</apply>\n"
						"<MarkerPairSet>\n"
							"<objects>\n"
								"<MarkerPair>\n"
									"<markers> mKneeR mAnkleR</markers>\n"
								"</MarkerPair>\n"
							"</objects>\n"
							"<groups />\n"
						"</MarkerPairSet>\n"
						"<BodyScaleSet>\n"
							"<objects>\n"
								"<BodyScale name=\"tibia\">\n"
									"<axes> X Y Z</axes>\n"
								"</BodyScale>\n"
							"</objects>\n"
							"<groups />\n"
						"</BodyScaleSet>\n"
					"</Measurement>\n"
					"<Measurement name=\"ltibia\">\n"
						"<apply>true</apply>\n"
						"<MarkerPairSet>\n"
							"<objects>\n"
								"<MarkerPair>\n"
									"<markers> mKneeL mAnkleL</markers>\n"
								"</MarkerPair>\n"
							"</objects>\n"
							"<groups />\n"
						"</MarkerPairSet>\n"
						"<BodyScaleSet>\n"
							"<objects>\n"
								"<BodyScale name=\"ltibia\">\n"
									"<axes> X Y Z</axes>\n"
								"</BodyScale>\n"
							"</objects>\n"
							"<groups />\n"
						"</BodyScaleSet>\n"
					"</Measurement>\n"
					"<Measurement name=\"rfoot\">\n"
						"<apply>false</apply>\n"
						"<MarkerPairSet>\n"
							"<objects>\n"
								"<MarkerPair>\n"
									"<markers> mAnkleR mToesR</markers>\n"
								"</MarkerPair>\n"
							"</objects>\n"
							"<groups />\n"
						"</MarkerPairSet>\n"
						"<BodyScaleSet>\n"
							"<objects />\n"
							"<groups />\n"
						"</BodyScaleSet>\n"
					"</Measurement>\n"
					"<Measurement name=\"lfoot\">\n"
						"<apply>false</apply>\n"
						"<MarkerPairSet>\n"
							"<objects>\n"
								"<MarkerPair>\n"
									"<markers> mAnkleL mToesL</markers>\n"
								"</MarkerPair>\n"
							"</objects>\n"
							"<groups />\n"
						"</MarkerPairSet>\n"
						"<BodyScaleSet>\n"
							"<objects />\n"
							"<groups />\n"
						"</BodyScaleSet>\n"
					"</Measurement>\n"
				"</objects>\n"
				"<groups />\n"
			"</MeasurementSet>\n"
			"<ScaleSet>\n"
				"<objects />\n"
				"<groups />\n"
			"</ScaleSet>\n");

	fprintf(f, "<marker_file>%s</marker_file>\n", OPENSIM_MOTION_FILE.c_str());

	fprintf(f, "<time_range> %f %f</time_range>\n", startTime, endTime);

	fprintf(f, 
			"<preserve_mass_distribution>false</preserve_mass_distribution>\n"
			"<output_model_file>Unassigned</output_model_file>\n"
			"<output_scale_file>Unassigned</output_scale_file>\n"
		"</ModelScaler>\n"
		"<MarkerPlacer>\n"
			"<apply>true</apply>\n"
			"<IKTaskSet>\n"
				"<objects>\n"
					"<IKMarkerTask name=\"mPelvis\">\n"
						"<apply>true</apply>\n"
						"<weight>1</weight>\n"
					"</IKMarkerTask>\n"
					"<IKMarkerTask name=\"mHipR\">\n"
						"<apply>true</apply>\n"
						"<weight>1</weight>\n"
					"</IKMarkerTask>\n"
					"<IKMarkerTask name=\"mHipL\">\n"
						"<apply>true</apply>\n"
						"<weight>1</weight>\n"
					"</IKMarkerTask>\n"
					"<IKMarkerTask name=\"mKneeR\">\n"
						"<apply>true</apply>\n"
						"<weight>1</weight>\n"
					"</IKMarkerTask>\n"
					"<IKMarkerTask name=\"mKneeL\">\n"
						"<apply>true</apply>\n"
						"<weight>1</weight>\n"
					"</IKMarkerTask>\n"
					"<IKMarkerTask name=\"mAnkleR\">\n"
						"<apply>true</apply>\n"
						"<weight>1</weight>\n"
					"</IKMarkerTask>\n"
					"<IKMarkerTask name=\"mAnkleL\">\n"
						"<apply>true</apply>\n"
						"<weight>1</weight>\n"
					"</IKMarkerTask>\n"
					"<IKMarkerTask name=\"mToesR\">\n"
						"<apply>true</apply>\n"
						"<weight>1</weight>\n"
					"</IKMarkerTask>\n"
					"<IKMarkerTask name=\"mToesL\">\n"
						"<apply>true</apply>\n"
						"<weight>1</weight>\n"
					"</IKMarkerTask>\n"
				"</objects>\n"
				"<groups />\n"
			"</IKTaskSet>\n");

	fprintf(f, 
			"<marker_file>%s</marker_file>\n"
			"<coordinate_file>Unassigned</coordinate_file>\n", OPENSIM_MOTION_FILE.c_str());

	fprintf(f, "<time_range> %f 1</time_range>\n", startTime);

	fprintf(f, 
			"<output_motion_file>Unassigned</output_motion_file>\n"
			"<output_model_file>Unassigned</output_model_file>\n"
			"<output_marker_file>Unassigned</output_marker_file>\n"
			"<max_marker_movement>-1</max_marker_movement>\n"
			"</MarkerPlacer>\n"
		"</ScaleTool>\n"
		"</OpenSimDocument>\n");

	fclose(f);

}

void Skeleton::generateOpenSimIKSettings(){

	FILE *f = fopen(OPENSIM_IK_SETTINGS.c_str(), "w");
	if(f == NULL){
		printf("Can't open file\n");
		return;
	}

	fprintf(f,
		"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
		"<OpenSimDocument Version=\"30000\">\n"
		"<InverseKinematicsTool>\n"
		"<results_directory>./</results_directory>\n"
		"<input_directory />\n"
		"<model_file>Unassigned</model_file>\n"
		"<constraint_weight>Inf</constraint_weight>\n"
		"<accuracy>1e-005</accuracy>\n"
		"<IKTaskSet>\n"
			"<objects>\n"
				"<IKMarkerTask name=\"mPelvis\">\n"
					"<apply>true</apply>\n"
					"<weight>1</weight>\n"
				"</IKMarkerTask>\n"
				"<IKMarkerTask name=\"mHipR\">\n"
					"<apply>true</apply>\n"
					"<weight>1</weight>\n"
				"</IKMarkerTask>\n"
				"<IKMarkerTask name=\"mHipL\">\n"
					"<apply>true</apply>\n"
					"<weight>1</weight>\n"
				"</IKMarkerTask>\n"
				"<IKMarkerTask name=\"mKneeR\">\n"
					"<apply>true</apply>\n"
					"<weight>1</weight>\n"
				"</IKMarkerTask>\n"
				"<IKMarkerTask name=\"mKneeL\">\n"
					"<apply>true</apply>\n"
					"<weight>1</weight>\n"
				"</IKMarkerTask>\n"
				"<IKMarkerTask name=\"mAnkleR\">\n"
					"<apply>true</apply>\n"
					"<weight>1</weight>\n"
				"</IKMarkerTask>\n"
				"<IKMarkerTask name=\"mAnkleL\">\n"
					"<apply>true</apply>\n"
					"<weight>1</weight>\n"
				"</IKMarkerTask>\n"
				"<IKMarkerTask name=\"mToesR\">\n"
					"<apply>true</apply>\n"
					"<weight>1</weight>\n"
				"</IKMarkerTask>\n"
				"<IKMarkerTask name=\"mToesL\">\n"
					"<apply>true</apply>\n"
					"<weight>1</weight>\n"
				"</IKMarkerTask>\n"
			"</objects>\n"
			"<groups />\n"
		"</IKTaskSet>\n");

	fprintf(f, 
		"<marker_file>%s</marker_file>\n"
		"<coordinate_file>Unassigned</coordinate_file>\n", OPENSIM_MOTION_FILE.c_str());

	fprintf(f, "<time_range> %f %f</time_range>\n", startTime, endTime);

	fprintf(f, 
		"<report_errors>true</report_errors>\n"
		"<output_motion_file>IK_motion.mot</output_motion_file>\n"
		"<report_marker_locations>false</report_marker_locations>\n"
		"</InverseKinematicsTool>\n"
		"</OpenSimDocument>\n");
	
	fclose(f);

}

void Skeleton::generateIDSettings(){

	FILE *f = fopen(OPENSIM_ID_DYNAMICS.c_str(), "w");
	if(f == NULL){
		printf("Can't open file\n");
		return;
	}

	fprintf(f,
		"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
		"<OpenSimDocument Version=\"30000\">\n"
		"<InverseDynamicsTool name=\"Kinect Lower Limb\">\n"
		"<results_directory>.\\</results_directory>\n"
		"<input_directory />\n"
		"<model_file>Unassigned</model_file>\n");

	fprintf(f, "<time_range> %f %f</time_range>\n", startTime, endTime);

	fprintf(f,
		"<forces_to_exclude> Muscles</forces_to_exclude>\n"
		"<external_loads_file>Unassigned</external_loads_file>\n"
		"<coordinates_file>IK_motion.mot</coordinates_file>\n"
		"<lowpass_cutoff_frequency_for_coordinates>6</lowpass_cutoff_frequency_for_coordinates>\n"
		"<output_gen_force_file>inverse_dynamics.sto</output_gen_force_file>\n"
		"<joints_to_report_body_forces />\n"
		"<output_body_forces_file>body_forces_at_joints.sto</output_body_forces_file>\n"
		"</InverseDynamicsTool>\n"
		"</OpenSimDocument>\n");

	fclose(f);
}

#include "Kinect.h"

// Interface between viewer for rgb rendering
extern GLubyte rgbStream[WINDOW_WIDTH*WINDOW_HEIGHT*4];

// Some smoothing with little latency (defaults).
// Only filters out small jitters.
// Good for gesture recognition in games.
const NUI_TRANSFORM_SMOOTH_PARAMETERS defaultParams =
	{0.5f, 0.5f, 0.5f, 0.05f, 0.04f};

// Smoothed with some latency.
// Filters out medium jitters.
// Good for a menu system that needs to be smooth but
// doesn't need the reduced latency as much as gesture recognition does.
const NUI_TRANSFORM_SMOOTH_PARAMETERS somewhatLatentParams =
	{0.5f, 0.1f, 0.5f, 0.1f, 0.1f};

// Very smooth, but with a lot of latency.
// Filters out large jitters.
// Good for situations where smooth data is absolutely required
// and latency is not an issue.
const NUI_TRANSFORM_SMOOTH_PARAMETERS verySmoothParams =
	{0.7f, 0.3f, 1.0f, 1.0f, 1.0f};

Kinect::Kinect(){
	frameBegin = 0;

	//fps
	frameCount = 0;
	currentTime = previousTime = 0;

	//ini files
	INIReader ini(INI_FILE_PATH);
	FILTER_MODE = ini.GetInteger("kinect", "FILTER_MODE", 3);

	//skeleton handler
	skeleton = Skeleton();
	
	//start streaming
	connect();
}


Kinect::~Kinect(void){
	if(sensor != NULL){
		sensor->NuiSkeletonTrackingDisable();
		sensor->Release();
	}

	CloseHandle(skeletonHandle);
	CloseHandle(rgbHandle);
	CloseHandle(rgbStreamHandle);

	delete sensor;
}

void Kinect::draw(){
	skeleton.draw();
}

void Kinect::update(){
	
	if (NULL == sensor){
		return;
	}

	// Wait for 0ms, just quickly test if it is time to process a skeleton
	if ( WAIT_OBJECT_0 == WaitForSingleObject(skeletonHandle, 0) ){
		prosessSkeletonData();
	}

	if ( WAIT_OBJECT_0 == WaitForSingleObject(rgbHandle, 0) ){
        prosessColorData();
    }
}

HRESULT Kinect::connect(){

    INuiSensor *pNuiSensor;

    int iSensorCount = 0;
    HRESULT hr = NuiGetSensorCount(&iSensorCount);
    if (FAILED(hr)){
        return hr;
    }

    // Look at each Kinect sensor
    for (int i = 0; i < iSensorCount; ++i){

        // Create the sensor so we can check status, if we can't create it, move on to the next
        hr = NuiCreateSensorByIndex(i, &pNuiSensor);
        if (FAILED(hr)){
            continue;
        }

        // Get the status of the sensor, and if connected, then we can initialize it
        hr = pNuiSensor->NuiStatus();
        if (S_OK == hr){
            sensor = pNuiSensor;
            break;
        }

        // This sensor wasn't OK, so release it since we're not using it
        pNuiSensor->Release();
    }

    if (NULL != sensor){
        // Initialize the Kinect and specify that we'll be using skeleton
        hr = sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON |
			NUI_INITIALIZE_FLAG_USES_COLOR); 
        if (SUCCEEDED(hr)){

			timeStamp = std::time(0);
			std::cout << "System time when sensor initialized: " << timeStamp << "\n";

			// Create an event that will be signaled when skeleton data is available
            skeletonHandle = CreateEventW(NULL, TRUE, FALSE, NULL);

			// Open a skeleton stream to receive skeleton data
            hr = sensor->NuiSkeletonTrackingEnable(skeletonHandle, 0);

			// Create an event that will be signaled when color data is available
            rgbHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

			hr = sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, // Depth camera or rgb camera?
				NUI_IMAGE_RESOLUTION_640x480,    // Image resolution
				0,		// Image stream flags, e.g. near mode
				2,		// Number of frames to buffer
				rgbHandle,   // Event handle
				&rgbStreamHandle);

        }
    }

    if (NULL == sensor || FAILED(hr)){
        std::cout<< "Can't initialize!\n";
        return E_FAIL;
    }

    return hr;
}


void Kinect::prosessColorData() {
	
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;
	if (sensor->NuiImageStreamGetNextFrame(rgbStreamHandle, 0, &imageFrame) < 0) return;

	INuiFrameTexture* texture = imageFrame.pFrameTexture;
    texture->LockRect(0, &LockedRect, NULL, 0);
    if (LockedRect.Pitch != 0){
        const BYTE* curr = (const BYTE*) LockedRect.pBits;
		const BYTE* dataEnd = curr + (WINDOW_WIDTH*WINDOW_HEIGHT)*4;
		
		int i = 0;
		while (curr < dataEnd) {
			rgbStream[i] = *curr++;
			i++;
		}
		
    }
    texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(rgbStreamHandle, &imageFrame);
}


void Kinect::prosessSkeletonData(){
	
	//calculate elapsed time from the previus frame
	frameEnd = clock();

	double elapsed_secs = double(frameEnd - frameBegin) / CLOCKS_PER_SEC;

	NUI_SKELETON_FRAME skeletonFrame = {0};

    HRESULT hr = sensor->NuiSkeletonGetNextFrame(0, &skeletonFrame);

    if ( FAILED(hr) ){
        return;
    }

    // Smooth the skeleton joint positions
	if(FILTER_MODE == 1){
		sensor->NuiTransformSmooth(&skeletonFrame, &defaultParams);
	}else if(FILTER_MODE == 2){
		sensor->NuiTransformSmooth(&skeletonFrame, &somewhatLatentParams);
	}else if(FILTER_MODE == 3){
		sensor->NuiTransformSmooth(&skeletonFrame, &verySmoothParams);
	}
    
	
	skeleton.setFloor(-skeletonFrame.vFloorClipPlane.w/skeletonFrame.vFloorClipPlane.y);
	//std::cout << skeletonFrame.vFloorClipPlane.w/skeletonFrame.vFloorClipPlane.y<<"\n";

    for (int i = 0 ; i < NUI_SKELETON_COUNT; ++i){

        NUI_SKELETON_TRACKING_STATE trackingState = skeletonFrame.SkeletonData[i].eTrackingState;
		
		NUI_SKELETON_BONE_ORIENTATION boneOrientations[NUI_SKELETON_POSITION_COUNT];
		NuiSkeletonCalculateBoneOrientations(&skeletonFrame.SkeletonData[i], boneOrientations);
		
        if (NUI_SKELETON_TRACKED == trackingState){
            // We're tracking the skeleton, draw it
			timeStamp = std::time(0);
			skeleton.addFrame(skeletonFrame.SkeletonData[i], boneOrientations, elapsed_secs, timeStamp);
        }else if (NUI_SKELETON_POSITION_ONLY == trackingState){
            // we've only received the center point of the skeleton, draw that
            /*D2D1_ELLIPSE ellipse = D2D1::Ellipse(
                SkeletonToScreen(skeletonFrame.SkeletonData[i].Position, width, height),
                g_JointThickness,
                g_JointThickness
                );

            m_pRenderTarget->DrawEllipse(ellipse, m_pBrushJointTracked);*/
			timeStamp = std::time(0);
			skeleton.addFrame(skeletonFrame.SkeletonData[i], boneOrientations, elapsed_secs, timeStamp);
        }
    }

	calculateFPS();

	//for getting frame data time
	frameBegin = clock();
}

void Kinect::calculateFPS(){
    //  Increase frame count
    frameCount++;

    currentTime = clock();
 
    //  Calculate time passed
    int timeInterval = currentTime - previousTime;
 
    if(timeInterval > 1000){
        //  calculate the number of frames per second
        fps = frameCount / (timeInterval / 1000.0f);
 
        //  Set time
        previousTime = currentTime;
 
        //  Reset frame count
        frameCount = 0;
    }
}

int Kinect::getSkeletonFrameRate(){
	return fps;
}




#ifndef CONSTANTS_H_
#define CONSTANTS_H_

//settings path
#define INI_FILE_PATH "settings.ini"

//markers
#define SKIP_FRAMES 2
#define MARKERS_SCALE 1000

//# of joints
#define JOINTS 20
#define PAIRS 19

//for rendering
#define SCALE 10
#define BONE_WIDTH 10
#define JOINT_RADIUS 0.4

//rendering setup
#define SMOOTH true
#define COLOR true
#define LIGHT true
#define WIRED false

//light
#define L_POS_X 20.0
#define L_POS_Y 50.0
#define L_POS_Z 100.0
#define L_POS_W 0.0

//window
#define WINDOW_NAME "Skeleton Tracking"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_X_POS 0
#define WINDOW_Y_POS 0

//text
#define TEXT_WIDTH 8
#define TEXT_HEIGHT 12

//camera
#define CAMERA_MOVE_SPEED 4
#define CAMERA_ROTATE_SPEED 0.01
#define CAM_POS_X 0
#define CAM_POS_Y 0
#define CAM_POS_Z -6
#define CAM_VIEW_X 0
#define CAM_VIEW_Y 0
#define CAM_VIEW_Z 1
#define CAM_UP_X 0
#define CAM_UP_Y 1
#define CAM_UP_Z 0

#endif

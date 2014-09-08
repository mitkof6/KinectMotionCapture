#include "Camera.h"

Camera::Camera(Vector3D poss, Vector3D vieww, Vector3D upp) {
	pos = poss;
	view = vieww;
	up = upp;
}

Camera::~Camera() {

}

void Camera::draw(){
	gluLookAt(
		pos.x, pos.y, pos.z,
		view.x, view.y, view.z,
		up.x, up.y, up.z);
}

void Camera::move(float speed){
	
	Vector3D dir = view-pos;
	dir = dir.normalize();

	dir = dir*speed;
	dir.y = 0;

	pos = pos+dir;
	view = view+dir;
	//view = view.normalize();
}

void Camera::rotateX(float speed){	//TODO when change dir

	Vector3D dir = view - pos;
					
	  
	view.x = dir.x;
	view.y = (float)(cos(speed)*dir.y - sin(speed)*dir.z);
	view.z = (float)(sin(speed)*dir.y + cos(speed)*dir.z);
	
	
}

void Camera::rotateY(float speed){
	Vector3D dir = view - pos;
	

	view.x = (float)(cos(speed)*dir.x + sin(speed)*dir.z);
	view.y = dir.y;
	view.z = (float)(-sin(speed)*dir.x + cos(speed)*dir.z);
}

void Camera::strafe(float speed){
	Vector3D dir = view - pos;
	dir = dir.normalize();

	Vector3D ortho;

	ortho.x = -dir.z;
	ortho.y = 0;
	ortho.z =  dir.x;
	ortho = ortho*speed;

	pos = pos + ortho;
	view = view + ortho;
	//view = view.normalize();
}


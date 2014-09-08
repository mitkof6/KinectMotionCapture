/*
 * Vector3D.cpp
 *
 *  Created on: Oct 22, 2013
 *      Author: Jim Stanev
 */

#include "Vector3D.h"

Vector3D::Vector3D() {

}

Vector3D::Vector3D(const Vector3D &other){
	x = other.x;
	y = other.y;
	z = other.z;
}

Vector3D::Vector3D(float xx, float yy, float zz){
	x = xx;
	y = yy;
	z = zz;
}

Vector3D::~Vector3D() {

}

Vector3D Vector3D::operator +(Vector3D other){
	return Vector3D(x+other.x, y+other.y, z+other.z);
}

Vector3D Vector3D::operator -(Vector3D other){
	return Vector3D(x-other.x, y-other.y, z-other.z);
}

Vector3D Vector3D::operator *(float c){
	return Vector3D(c*x, c*y, c*z);
}

Vector3D Vector3D::operator /(float c){
	return Vector3D(x/c, y/c, z/c);
}

float Vector3D::dot(Vector3D other){
	return x*other.x+y*other.y+z*other.z;
}

Vector3D Vector3D::cross(Vector3D other) {
	return Vector3D(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x);
}

Vector3D Vector3D::normalize(){
	float d = sqrt(x*x+y*y+z*z);
	return Vector3D(x/d, y/d, z/d);
}

void Vector3D::toString(){
	printf("[%f, %f, %f]\n", x, y, z);
}

bool Vector3D::equals(Vector3D other){
	if(abs(x-other.x) < TOLL && 
		abs(y-other.y) < TOLL &&
		abs(z-other.z) < TOLL){
		return true;
	}else{
		return false;
	}
}

float Vector3D::magnitudeSquared() {
	return x * x + y * y + z * z;
}

float Vector3D::magnitude(){
	return sqrt(x*x+y*y+z*z);
}
/*
void Vector3D::operator *(Vector3D other){
	float tempX = ( y * other.z ) - ( z * other.y );
    float tempY = ( z * other.x ) - ( x * other.z );
    float tempZ = (x * other.y) - (y * other.x);
    return Vector3D(tempX, tempY, tempZ);
}
*/




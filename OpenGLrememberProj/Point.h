#pragma once
#include <Windows.h>
#include <GL\GL.h>

using namespace std;

class Point {
public:
	double x;
	double y;
	double z;
	Point(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Point(const Point& A) {
		this->x = A.x;
		this->y = A.y;
		this->z = A.z;
	}
	void DrawPoint() {
		glVertex3d(this->x, this->y, this->z);
	}
	void Normal3d() {
		glNormal3d(this->x, this->y, this->z);
	}
	void Translated() {
		glTranslated(this->x, this->y, this->z);
	}
	~Point() {

	}
	Point operator + (Point A) {
		A.x = A.x + this->x;
		A.y = A.y + this->y;
		A.z = A.z + this->z;
		return A;
	}
};

class PointXY {
public:
	double x;
	double y;
	PointXY(double x, double y) {
		this->x = x;
		this->y = y;
	}
	void TexCoord2d() {
		glTexCoord2d(x, y);
	}
};

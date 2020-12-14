#pragma once
#include "Camera.h"
#include "Light.h"
#include "PlaneСontrolSystem.h"
#include "CarСontrolSystem.h"

extern PlaneСontrolSystem Plane;
extern CarСontrolSystem Car;

//класс для настройки камеры
class CustomCamera : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;


	//значния масеры по умолчанию
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}


	//считает позицию камеры, исходя из углов поворота, вызывается движком
	virtual void SetUpCamera()
	{

		//lookPoint.setCoords(0, 0, 0);
		//
		//pos.setCoords(camDist * cos(fi2) * cos(fi1),
		//	camDist * cos(fi2) * sin(fi1),
		//	camDist * sin(fi2));
		//
		//if (cos(fi2) <= 0)
		//	normal.setCoords(0, 0, -1);
		//else
		//	normal.setCoords(0, 0, 1);
		//
		//LookAt();

		//отвечает за поворот камеры мышкой
		lookPoint.setCoords(Car.CarPosition.X(), Car.CarPosition.Y(), Car.CarPosition.Z());

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}

	void SetLookPoint(double x, double y, double z)
	{
		lookPoint.setCoords(x, y, z);
	}

	void SetLookPoint(Vector3 vec)
	{
		lookPoint = vec;
	}

};
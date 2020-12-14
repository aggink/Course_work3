#pragma once
#include "MyVector3d.h"
class PlaneСontrolSystem {
public:
	//максимальный угол поворота закрылков
	double MaxAngleFlaps = 75;

	//максимальный угол поворота заднего закрылка
	double MaxAnglRearFlap = 75;
	
	//угол поворота объекта относительно оси Z
	double AngleZ = 0;

	//угол поворота объекта относительно оси Y
	double AngleY = 0;

	//угол попорота закрылков
	double AngleFlaps = 0;

	//угол поворота заднего закрылка
	double AngleRearFlap = 0;

	//скорость 
	double SpeedPlane = 0.1;

	//скорость вращения закрылков
	double SpeedRearFlap = 4;
	
	//скорость вращения закрылков
	double SpeedFlaps = 4;

	//поворот колеса относительно своей оси
	double AngleWheel = 0;

	//скорость возвращение закрылков в прямое положение
	double SpeedFlapsReverse = 0.5;

	//скорость возвращения заднего закрылка в прямое положения
	double SpeedRearFlapReverse = 0.5;

	//отдаленность камеры от объектаvm
	double DistanceFromPlane = 10;

	//Высота камеры над объектом
	double HeightFromPlane = 10;

	//скорость торможенияw
	double SpeedSlow = 0.1;

	//Время начала нажатия W
	double KeyTimeDown_W = 0;

	//период нажатия W
	double KeyDuration_W = 0;

	//Происходит ли сейчас торможение
	bool FlagDeceleration = false;

	//происходит ли сейчас разгон
	bool FlagAcceleration = false;

	//Разгон после торможения
	bool FlagAccAfterDec = false;

	//позиция объекта
	Vector3 PlanePosition;

	Vector3 FlapsPosition;

	Vector3 RearFlapPosition;

	Vector3 WheelsPosition;

	Vector3 HelicePosition;

	PlaneСontrolSystem() {
		PlanePosition = Vector3(0, 0, 0);
		FlapsPosition = Vector3(0, 0, 0);
		RearFlapPosition = Vector3(0, 0, 0);
		WheelsPosition = Vector3(0, 0, 0);
		HelicePosition = Vector3(0, 0, 0);
	}
};
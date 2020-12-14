#pragma once
#pragma once
#include "MyVector3d.h"
class CarСontrolSystem {
public:
	//максимальный угол поворота 
	double MaxAngleWheel = 27;

	//угол поворота объекта относительно оси Z
	double AngleZ = 0;

	//угол попорота 
	double AngleWheel = 0;

	//угол поворота относительно ствоей оси
	double Wheel_angle = 0;

	//скорость 
	double Speed = 0.1;

	//скорость вращения 
	double SpeedWheel = 4;

	//скорость возвращение закрылков в прямое положение
	double SpeedWheelReverse = 0.5;

	//отдаленность камеры от объектаvm
	double Distance = 10;

	//Высота камеры над объектом
	double Height = 5;

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
	
	//координаты точек где находится машинка сейчас
	double Car_x_maxNow = 0.7;
	double Car_x_minNow = -1.7;
	double Car_y_maxNow = 0.74;
	double Car_y_minNow = -0.74;

	double R = 1.75;
	//позиция объекта
	Vector3 CarPosition;
	Vector3 CarPosition_back;

	bool Stop = false;

	CarСontrolSystem() {
		CarPosition = Vector3(0, 0, 0);
		CarPosition_back = Vector3(0, 0, 0);
	}
};
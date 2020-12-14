#pragma once
#include<vector>
#include <math.h>
#include"MyShaders.h"
#include "PlaneСontrolSystem.h"
#include "MyOGL.h"
#include "CustomCamera.h"
#include "Texture.h"
#include "ObjLoader.h"
#include <ctime>
using namespace std;

//задаем имя файла с моделью и текстрой
extern Texture PlaneTex, FlapsTex, HeliceTex, RearFlapTex, WheelsTex;
extern ObjFile PlaneObj, FlapsObj, HeliceObj, RearFlapObj, WheelsObj;



//переменные для работы с движением самолета
extern PlaneСontrolSystem Plane;

//свободный режим камеры
extern bool freeCamera;

//создаем объект камеры
extern CustomCamera  camera;

//время последнего кадра
extern double TimeLastFrame;

//рассчет новой точки
void NewPositionPlane() {
	if (Plane.KeyDuration_W > 15) {
		Plane.KeyDuration_W = 15;
	}
	Plane.PlanePosition.setCoord_X(Plane.PlanePosition.X() + Plane.SpeedPlane * cos(Plane.AngleY * PI / 180) * cos(Plane.AngleZ * PI / 180) * (Plane.KeyDuration_W / 5));
	Plane.PlanePosition.setCoord_Y(Plane.PlanePosition.Y() + Plane.SpeedPlane * cos(Plane.AngleY * PI / 180) * sin(Plane.AngleZ * PI / 180) * (Plane.KeyDuration_W / 5));
	Plane.PlanePosition.setCoord_Z(Plane.PlanePosition.Z() + Plane.SpeedPlane * sin(Plane.AngleY * PI / 180) * (Plane.KeyDuration_W / 5));
}

void NewPositionFlaps() {
	if (Plane.KeyDuration_W > 15) {
		Plane.KeyDuration_W = 15;
	}
	Plane.FlapsPosition.setCoord_X(Plane.FlapsPosition.X() + Plane.SpeedPlane * cos(Plane.AngleY * PI / 180) * cos(Plane.AngleZ * PI / 180) * (Plane.KeyDuration_W / 5));
	Plane.FlapsPosition.setCoord_Y(Plane.FlapsPosition.Y() + Plane.SpeedPlane * cos(Plane.AngleY * PI / 180) * sin(Plane.AngleZ * PI / 180) * (Plane.KeyDuration_W / 5));
	Plane.FlapsPosition.setCoord_Z(Plane.FlapsPosition.Z() + Plane.SpeedPlane * sin(Plane.AngleY * PI / 180) * (Plane.KeyDuration_W / 5));
}

void NewPositionRearFlap() {
	if (Plane.KeyDuration_W > 15) {
		Plane.KeyDuration_W = 15;
	}
	Plane.RearFlapPosition.setCoord_X(Plane.RearFlapPosition.X() + Plane.SpeedPlane * cos(Plane.AngleY * PI / 180) * cos(Plane.AngleZ * PI / 180) * (Plane.KeyDuration_W / 5));
	Plane.RearFlapPosition.setCoord_Y(Plane.RearFlapPosition.Y() + Plane.SpeedPlane * cos(Plane.AngleY * PI / 180) * sin(Plane.AngleZ * PI / 180) * (Plane.KeyDuration_W / 5));
	Plane.RearFlapPosition.setCoord_Z(Plane.RearFlapPosition.Z() + Plane.SpeedPlane * sin(Plane.AngleY * PI / 180) * (Plane.KeyDuration_W / 5));
}

void NewPositionWheels() {
	if (Plane.KeyDuration_W > 15) {
		Plane.KeyDuration_W = 15;
	}
	Plane.WheelsPosition.setCoord_X(Plane.WheelsPosition.X() + Plane.SpeedPlane * cos(Plane.AngleY * PI / 180) * cos(Plane.AngleZ * PI / 180) * (Plane.KeyDuration_W / 5));
	Plane.WheelsPosition.setCoord_Y(Plane.WheelsPosition.Y() + Plane.SpeedPlane * cos(Plane.AngleY * PI / 180) * sin(Plane.AngleZ * PI / 180) * (Plane.KeyDuration_W / 5));
	Plane.WheelsPosition.setCoord_Z(Plane.WheelsPosition.Z() + Plane.SpeedPlane * sin(Plane.AngleY * PI / 180) * (Plane.KeyDuration_W / 5));
}

void NewPositionHelice() {
	if (Plane.KeyDuration_W > 15) {
		Plane.KeyDuration_W = 15;
	}
	Plane.HelicePosition.setCoord_X(Plane.HelicePosition.X() + Plane.SpeedPlane * cos(Plane.AngleY * PI / 180) * cos(Plane.AngleZ * PI / 180) * (Plane.KeyDuration_W / 5));
	Plane.HelicePosition.setCoord_Y(Plane.HelicePosition.Y() + Plane.SpeedPlane * cos(Plane.AngleY * PI / 180) * sin(Plane.AngleZ * PI / 180) * (Plane.KeyDuration_W / 5));
	Plane.HelicePosition.setCoord_Z(Plane.HelicePosition.Z() + Plane.SpeedPlane * sin(Plane.AngleY * PI / 180) * (Plane.KeyDuration_W / 5));
}
//расчет нового положения камеры
void NewPositionCamera() {

	Vector3 tmp{
		Plane.PlanePosition.X() - Plane.DistanceFromPlane * cos(Plane.AngleY * PI / 180),
		Plane.PlanePosition.Y() - Plane.DistanceFromPlane * cos(Plane.AngleY * PI / 180),
		Plane.PlanePosition.Z() + Plane.HeightFromPlane * sin(Plane.AngleY * PI / 180)
	};
	camera.pos = tmp;
	camera.camDist = 30;
	camera.SetLookPoint(Plane.PlanePosition);
}

//измененение координат и позиции камеры при замедлении
void ObjectDeceleration_Key_W() {
	//ищем новую позицию самолета
	NewPositionPlane();
	if (!freeCamera) {
		NewPositionCamera();
	}
}

//обработчик нажания кнопок для самолета
void UpdateParameters() {
	//получаем текущее время
	double end = clock();

	//время кадра
	double TimeFrame = (end - TimeLastFrame) / CLOCKS_PER_SEC;

	if (TimeFrame > 2) {
		return;
	}

	//замедление объекта при движении вперед, если не нажаты клавиши W и S
	if (!OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Plane.KeyDuration_W > 0) {
		//проверяем, чтоб время нажетия W не ушло в минус
		if (Plane.KeyDuration_W - TimeFrame < 0) {
			Plane.KeyDuration_W = 0;
		}
		else
		{
			Plane.KeyDuration_W -= TimeFrame;
			//находим новые координаты объекта, а также задаем новое положение камеры
			ObjectDeceleration_Key_W();
		}
		//устанавливаем флаг на разгон после торможения
		Plane.FlagAccAfterDec = true;
	}

	//плавное замедление при движении назад
	if (!OpenGL::isKeyPressed('S') && !OpenGL::isKeyPressed('W') && Plane.KeyDuration_W < 0)
	{
		if (Plane.KeyDuration_W + TimeFrame > 0)
			Plane.KeyDuration_W = 0;
		else
		{
			Plane.KeyDuration_W += TimeFrame;
			//находим новые координаты объекта, а также задаем новое положение камеры
			ObjectDeceleration_Key_W();
		}
	}

	//торможение при движении вперед, нажата только S
	if (OpenGL::isKeyPressed('S') && !OpenGL::isKeyPressed('W') && Plane.KeyDuration_W > 0)
	{
		if (Plane.KeyDuration_W - Plane.SpeedSlow < 0)
			Plane.KeyDuration_W = 0;
		else
		{
			Plane.KeyDuration_W -= Plane.SpeedSlow;
			ObjectDeceleration_Key_W();
		}

	}

	//торможение при движении назад
	if (OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Plane.KeyDuration_W < 0)
	{
		if (Plane.KeyDuration_W + Plane.SpeedSlow > 0)
			Plane.KeyDuration_W = 0;
		else
		{
			Plane.KeyDuration_W += Plane.SpeedSlow;
			ObjectDeceleration_Key_W();
		}
	}

	//движение вперед, нажата только W
	if (OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Plane.KeyDuration_W >= 0)
	{
		//проверяем, есть ли сейчас разгон
		if (!Plane.FlagAcceleration)
		{
			Plane.FlagAcceleration = true;
		}
		else
		{
			//ищем новую позицию самолета
			NewPositionPlane();
			if (!freeCamera)
			{
				NewPositionCamera();
			}

			//ускоряемся
			Plane.KeyDuration_W += TimeFrame;
		}
	}

	//движение назад, нажата только S
	if (OpenGL::isKeyPressed('S') && !OpenGL::isKeyPressed('W') && Plane.KeyDuration_W <= 0)
	{
		if (!Plane.FlagDeceleration)
		{
			Plane.FlagDeceleration = true;
		}
		else
		{
			NewPositionPlane();
			if (!freeCamera)
			{
				NewPositionCamera();
			}
			//замедляемся
			Plane.KeyDuration_W -= TimeFrame;
		}
	}

	//стрелка вверх 38
	//стрелка вниз 40
	//стрелка вправо 39
	//стрелка влево 37

	//поворачиваем задний закрылок вправо
	if (OpenGL::isKeyPressed(39)) {
		if (Plane.AngleRearFlap + Plane.SpeedRearFlap > Plane.MaxAnglRearFlap + Plane.AngleZ) {
			Plane.AngleRearFlap = Plane.MaxAnglRearFlap + Plane.AngleZ;
		}
		else
		{
			Plane.AngleRearFlap += Plane.SpeedRearFlap;
		}

		//увеличиваем градус поворота
		if (Plane.KeyDuration_W != 0) {
			Plane.AngleZ += Plane.SpeedRearFlap / 10;
		}
	}

	//поворачиваем задний закрылок влево
	if (OpenGL::isKeyPressed(37)) {
		if (Plane.AngleRearFlap - Plane.SpeedRearFlap < -1 * Plane.MaxAnglRearFlap + Plane.AngleZ) {
			Plane.AngleRearFlap = -1 * Plane.MaxAnglRearFlap + Plane.AngleZ;
		}
		else
		{
			Plane.AngleRearFlap -= Plane.SpeedRearFlap;
		}

		//увеличиваем градус поворота
		if (Plane.KeyDuration_W != 0) {
			Plane.AngleZ -= Plane.SpeedRearFlap / 10;
		}
	}

	//постепенно возвращаем задний закрылок назад 
	if (!OpenGL::isKeyPressed(37) && !OpenGL::isKeyPressed(39) && Plane.AngleRearFlap < Plane.AngleZ) 
	{
		if (Plane.AngleRearFlap + Plane.SpeedRearFlapReverse > Plane.AngleZ)
		{
			Plane.AngleRearFlap = Plane.AngleZ;
		}
		else
		{
			Plane.AngleRearFlap += Plane.SpeedRearFlapReverse;
		}
	}

	//постепенно возвращаем задний закрылок назад 
	if (!OpenGL::isKeyPressed(37) && !OpenGL::isKeyPressed(39) && Plane.AngleRearFlap > Plane.AngleZ) 
	{
		if (Plane.AngleRearFlap - Plane.SpeedRearFlapReverse < Plane.AngleZ)
		{
			Plane.AngleRearFlap = Plane.AngleZ;
		}
		else
		{
			Plane.AngleRearFlap -= Plane.SpeedRearFlapReverse;
		}
	}

	//наклон вверх
	if (OpenGL::isKeyPressed(38)) {
		if (Plane.AngleFlaps + Plane.SpeedFlaps > Plane.MaxAngleFlaps + Plane.AngleY) {
			Plane.AngleFlaps = Plane.MaxAngleFlaps + Plane.AngleY;
		}
		else
		{
			Plane.AngleFlaps += Plane.SpeedFlaps;
		}

		//увеличиваем градус поворота
		if (Plane.KeyDuration_W != 0) {
			Plane.AngleY += Plane.SpeedFlaps / 10;
		}
	}

	//наклон вниз
	if (OpenGL::isKeyPressed(40)) {
		if (Plane.AngleFlaps - Plane.SpeedFlaps < -1 * Plane.MaxAngleFlaps + Plane.AngleY) {
			Plane.AngleFlaps = -1 * Plane.MaxAngleFlaps + Plane.AngleY;
		}
		else
		{
			Plane.AngleFlaps -= Plane.SpeedFlaps;
		}

		//увеличиваем градус поворота
		if (Plane.KeyDuration_W != 0) {
			Plane.AngleY -= Plane.SpeedFlaps / 10;
		}
	}

	//постепенно возвращяем закрылки назад 
	if (!OpenGL::isKeyPressed(38) && !OpenGL::isKeyPressed(40) && Plane.AngleFlaps < Plane.AngleY) 
	{
		if (Plane.AngleFlaps + Plane.SpeedFlapsReverse > Plane.AngleY)
		{
			Plane.AngleFlaps = Plane.AngleY;
		}
		else
		{
			Plane.AngleFlaps += Plane.SpeedFlapsReverse;
		}
	}

	//постепенно возвращяем закрылки назад 
	if (!OpenGL::isKeyPressed(38) && !OpenGL::isKeyPressed(40) && Plane.AngleFlaps > Plane.AngleY)
	{
		if (Plane.AngleFlaps - Plane.SpeedFlapsReverse < Plane.AngleY)
		{
			Plane.AngleFlaps = Plane.AngleY;
		}
		else
		{
			Plane.AngleFlaps -= Plane.SpeedFlapsReverse;
		}
	}

	//P - вернуть в исходное положение
	if (OpenGL::isKeyPressed('P')) {
		Plane = PlaneСontrolSystem();
	}
}


void WorkPlane() {

	
}
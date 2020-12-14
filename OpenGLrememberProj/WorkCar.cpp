#pragma once
#include<vector>
#include <math.h>
#include"MyShaders.h"
#include "CarСontrolSystem.h"
#include "MyOGL.h"
#include "CustomCamera.h"
#include "Texture.h"
#include "ObjLoader.h"
#include <ctime>
using namespace std;

//задаем имя файла с моделью и текстрой
extern ObjFile CarObj, CarWheelObj, CarPlayLObj, CarPlayRObj, CarStopLObj, CarStopLPovObj, CarStopRObj, CarStopRPovObj;
extern Texture CarTex, CarWheelTex, CarPlay1Tex, CarStop1Tex, CarStop2Tex;
//вектор для шейдеров (задаем именя вершинного и фрагментного шейдеров
extern vector<Shader> shader;
//переменные для работы с движением 
extern CarСontrolSystem Car;
//свободный режим камеры
extern bool freeCamera;
//переменная отвечающая за работу с камерой
extern CustomCamera camera;
//время последнего кадра
extern double TimeLastFrame;


//рассчет новой точки
void NewPositionCar() {
	if (Car.KeyDuration_W > 10) {
		Car.KeyDuration_W = 10;
	}
	if (!Car.Stop) {
		Car.CarPosition_back = Car.CarPosition;
	}
	Car.CarPosition.setCoord_X(Car.CarPosition.X() + Car.Speed * cos(Car.AngleZ * PI / 180) * (Car.KeyDuration_W / 5));
	Car.CarPosition.setCoord_Y(Car.CarPosition.Y() + Car.Speed * sin(Car.AngleZ * PI / 180) * (Car.KeyDuration_W / 5));

	Car.Car_x_maxNow = Car.Car_x_maxNow + Car.Speed * cos(Car.AngleZ * PI / 180) * (Car.KeyDuration_W / 5);
	Car.Car_x_minNow = Car.Car_x_minNow + Car.Speed * cos(Car.AngleZ * PI / 180) * (Car.KeyDuration_W / 5);
	Car.Car_y_maxNow = Car.Car_y_maxNow + Car.Speed * sin(Car.AngleZ * PI / 180) * (Car.KeyDuration_W / 5);
	Car.Car_y_minNow = Car.Car_y_minNow + Car.Speed * sin(Car.AngleZ * PI / 180) * (Car.KeyDuration_W / 5);
}

//расчет нового положения камеры
void NewPositionCamera_Car() {

	Vector3 tmp{
		Car.CarPosition.X() - Car.Distance * cos(Car.AngleZ * PI / 180),
		Car.CarPosition.Y() - Car.Distance * sin(Car.AngleZ * PI / 180),
		Car.CarPosition.Z() + Car.Height
	};
	camera.pos = tmp;
	camera.camDist = 30;
	camera.SetLookPoint(Car.CarPosition);
}

//измененение координат и позиции камеры при замедлении
void ObjectDeceleration_Key_W_Car() {
	//ищем новую позицию самолета
	NewPositionCar();
	if (!freeCamera) {
		NewPositionCamera_Car();
	}
}

//обработчик нажания кнопок 
void UpdateParameters_Car() {
	//получаем текущее время
	double end = clock();

	//время кадра
	double TimeFrame = (end - TimeLastFrame) / CLOCKS_PER_SEC;

	if (TimeFrame > 2) {
		return;
	}

	//замедление объекта при движении вперед, если не нажаты клавиши W и S
	if (!OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Car.KeyDuration_W > 0) {
		//проверяем, чтоб время нажетия W не ушло в минус
		if (Car.KeyDuration_W - TimeFrame < 0) {
			Car.KeyDuration_W = 0;
		}
		else
		{
			Car.KeyDuration_W -= TimeFrame;
			//находим новые координаты объекта, а также задаем новое положение камеры
			ObjectDeceleration_Key_W_Car();
		}
		//устанавливаем флаг на разгон после торможения
	    Car.FlagAccAfterDec = true;
	}

	//плавное замедление при движении назад
	if (!OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Car.KeyDuration_W < 0)
	{
		if (Car.KeyDuration_W + TimeFrame > 0)
			Car.KeyDuration_W = 0;
		else
		{
			Car.KeyDuration_W += TimeFrame;
			//находим новые координаты объекта, а также задаем новое положение камеры
			ObjectDeceleration_Key_W_Car();
		}
	}

	//торможение при движении вперед, нажата только S
	if (OpenGL::isKeyPressed('S') && !OpenGL::isKeyPressed('W') && Car.KeyDuration_W > 0)
	{
		if (Car.KeyDuration_W - Car.SpeedSlow < 0)
			Car.KeyDuration_W = 0;
		else
		{
			Car.KeyDuration_W -= Car.SpeedSlow;
			ObjectDeceleration_Key_W_Car();
		}

	}

	//торможение при движении назад
	if (OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Car.KeyDuration_W < 0)
	{
		if (Car.KeyDuration_W + Car.SpeedSlow < 0)
			Car.KeyDuration_W = 0;
		else
		{
			Car.KeyDuration_W += Car.SpeedSlow;
			ObjectDeceleration_Key_W_Car();
		}
	}

	//движение вперед, нажата только W
	if (OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Car.KeyDuration_W >= 0)
	{
		//проверяем, есть ли сейчас разгон
		if (!Car.FlagAcceleration)
		{
			Car.FlagAcceleration = true;
		}
		else
		{
			//ищем новую позицию самолета
			NewPositionCar();
			if (!freeCamera)
			{
				NewPositionCamera_Car();
			}

			//ускоряемся
			Car.KeyDuration_W += TimeFrame;
		}
	}

	//движение назад, нажата только S
	if (OpenGL::isKeyPressed('S') && !OpenGL::isKeyPressed('W') && Car.KeyDuration_W <= 0)
	{
		if (!Car.FlagDeceleration)
		{
			Car.FlagDeceleration = true;
		}
		else
		{
			NewPositionCar();
			if (!freeCamera)
			{
				NewPositionCamera_Car();
			}
			//замедляемся
			Car.KeyDuration_W -= TimeFrame;
		}
	}

	//поворачиваем вправо
	if (OpenGL::isKeyPressed('D')) {
		if (Car.AngleWheel - Car.SpeedWheel < -1 * Car.MaxAngleWheel + Car.AngleZ) {
			Car.AngleWheel = -1 * Car.MaxAngleWheel + Car.AngleZ;
		}
		else
		{
			Car.AngleWheel -= Car.SpeedWheel;
		}

		//увеличиваем градус поворота
		if (OpenGL::isKeyPressed('W')) {
			Car.AngleZ -= Car.SpeedWheel / 10;
		}
		if (OpenGL::isKeyPressed('S')) {
			Car.AngleZ += Car.SpeedWheel / 10;
		}
	}

	//поворачиваем влево
	if (OpenGL::isKeyPressed('A')) {
		if (Car.AngleWheel + Car.SpeedWheel > Car.MaxAngleWheel + Car.AngleZ) {
			Car.AngleWheel = Car.MaxAngleWheel + Car.AngleZ;
		}
		else
		{
			Car.AngleWheel += Car.SpeedWheel;
		}

		//увеличиваем градус поворота
		if (OpenGL::isKeyPressed('W')) {
			Car.AngleZ += Car.SpeedWheel / 10;
		}
		if (OpenGL::isKeyPressed('S')) {
			Car.AngleZ -= Car.SpeedWheel / 10;
		}
	}

	//постепенно возвращаем назад 
	if (!OpenGL::isKeyPressed('A') && !OpenGL::isKeyPressed('D') && Car.AngleWheel < Car.AngleZ)
	{
		if (Car.AngleWheel + Car.SpeedWheelReverse > Car.AngleZ)
		{
			Car.AngleWheel = Car.AngleZ;
		}
		else
		{
			Car.AngleWheel += Car.SpeedWheelReverse;
		}
	}

	//постепенно возвращаем назад 
	if (!OpenGL::isKeyPressed('A') && !OpenGL::isKeyPressed('D') && Car.AngleWheel > Car.AngleZ)
	{
		if (Car.AngleWheel - Car.SpeedWheelReverse < Car.AngleZ)
		{
			Car.AngleWheel = Car.AngleZ;
		}
		else
		{
			Car.AngleWheel -= Car.SpeedWheelReverse;
		}
	}
	//P - вернуть в исходное положение
	if (OpenGL::isKeyPressed('P')) {
		Car = CarСontrolSystem();
	}
	//пробел стоп
	if (OpenGL::isKeyPressed(32)) {
		Car.KeyDuration_W = 0;
	}
}

//позиция колес
void PositionWheels(int i) {

	//x +
	double a = 1.08;
	//y
	double b = 0.65;
	//x -
	double c = 1.03;
	//высота
	double d = 0.27;

	Vector3 p;
	switch (i) {
	case 1:
		//левое
		p = Vector3(
			Car.CarPosition.X() + a * cos(Car.AngleZ * PI / 180) - b * sin(Car.AngleZ * PI / 180),
			Car.CarPosition.Y() + b * cos(Car.AngleZ * PI / 180) + a * sin(Car.AngleZ * PI / 180),
			Car.CarPosition.Z() + d);
		break;
	case 2:
		//правое
		p = Vector3(
			Car.CarPosition.X() + a * cos(Car.AngleZ * PI / 180) + b * sin(Car.AngleZ * PI / 180),
			Car.CarPosition.Y() - b * cos(Car.AngleZ * PI / 180) + a * sin(Car.AngleZ * PI / 180),
			Car.CarPosition.Z() + d);
		break;
	case 3:
		//левое заднее
		p = Vector3(
			Car.CarPosition.X() - c * cos(Car.AngleZ * PI / 180) - b * sin(Car.AngleZ * PI / 180),
			Car.CarPosition.Y() + b * cos(Car.AngleZ * PI / 180) - c * sin(Car.AngleZ * PI / 180),
			Car.CarPosition.Z() + d);
		break;
	case 4:
		//правое заднее
		 p = Vector3(
			Car.CarPosition.X() - c * cos(Car.AngleZ * PI / 180) + b * sin(Car.AngleZ * PI / 180),
			Car.CarPosition.Y() - b * cos(Car.AngleZ * PI / 180) - c * sin(Car.AngleZ * PI / 180),
			Car.CarPosition.Z() + d);
		break;
	}
	glTranslated(p.X(), p.Y(), p.Z());
}

void WorkCar() {
	//обновление параметров 
	UpdateParameters_Car();

	//обновление времени последнего кадра
	TimeLastFrame = clock();
	
	int l = 0;
     
	//машина
	glPushMatrix();
	glTranslated(Car.CarPosition.X(), Car.CarPosition.Y(), Car.CarPosition.Z());
	glRotated(180, 0, 0, 1);
	glRotated(Car.AngleZ, 0, 0, 1);

	//машина
	CarTex.bindTexture();
	CarObj.DrawObj();

	//передние фары

	//левый
	if (OpenGL::isKeyPressed('A')) {
		CarPlay1Tex.bindTexture();
	}
	else {
		CarStop2Tex.bindTexture();
	}
	CarPlayLObj.DrawObj();

	//правый
	if (OpenGL::isKeyPressed('D')) {
		CarPlay1Tex.bindTexture();
	}
	else {
		CarStop2Tex.bindTexture();
	}
	CarPlayRObj.DrawObj();

	//задние фары
	if (OpenGL::isKeyPressed('S')) {
		CarStop2Tex.bindTexture();
	}
	else {
		CarStop1Tex.bindTexture();
	}
	CarStopLObj.DrawObj();
	CarStopRObj.DrawObj();

	//поворотники
	if (OpenGL::isKeyPressed('A')) {
		CarPlay1Tex.bindTexture();
	}
	else {
		if (OpenGL::isKeyPressed('S')) {
			CarStop2Tex.bindTexture();
		}
		else {
			CarStop1Tex.bindTexture();
		}
	}
	CarStopLPovObj.DrawObj();

	if (OpenGL::isKeyPressed('D')) {
		CarPlay1Tex.bindTexture();
	}
	else {
		if (OpenGL::isKeyPressed('S')) {
			CarStop2Tex.bindTexture();
		}
		else {
			CarStop1Tex.bindTexture();
		}
	}
	CarStopRPovObj.DrawObj();

	glPopMatrix();


	Car.Wheel_angle += Car.KeyDuration_W * Car.Speed * 40;
	if (((long long)Car.Wheel_angle % 360 > -1 || (long long)Car.Wheel_angle % 360 < 1) && (Car.Wheel_angle > 2160 || Car.Wheel_angle < -2160))
		Car.Wheel_angle = 0;

	//первый ряд колес

	//левое
	glPushMatrix();
	PositionWheels(1);
	glRotated(180, 0, 0, 1);
	glRotated(Car.AngleWheel, 0, 0, 1);
	glRotated(Car.Wheel_angle, 0, 1, 0);
	CarWheelTex.bindTexture();
	CarWheelObj.DrawObj();
	glPopMatrix();
	
	//правое
	glPushMatrix();
	PositionWheels(2);
	glRotated(360, 0, 0, 1);
	glRotated(Car.AngleWheel, 0, 0, 1);
	glRotated(Car.Wheel_angle, 0, 1, 0);
	CarWheelTex.bindTexture();
	CarWheelObj.DrawObj();
	glPopMatrix();

	//второй ряд колес

	//левое
	glPushMatrix();
	PositionWheels(3);
	glRotated(180, 0, 0, 1);
	glRotated(Car.AngleZ, 0, 0, 1);
	glRotated(Car.Wheel_angle, 0, 1, 0);
	CarWheelTex.bindTexture();
	CarWheelObj.DrawObj();
	glPopMatrix();

	//правое
	glPushMatrix();
	PositionWheels(4);
	glRotated(360, 0, 0, 1);
	glRotated(Car.AngleZ, 0, 0, 1);
	glRotated(Car.Wheel_angle, 0, 1, 0);
	CarWheelTex.bindTexture();
	CarWheelObj.DrawObj();
	glPopMatrix();

}
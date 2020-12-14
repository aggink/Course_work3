#pragma once
#include<math.h>
#include<vector>
#include "ObjLoader.h"
#include "Texture.h"
#include "CarСontrolSystem.h"
#include "MyOGL.h"
#include "DrawBezierAndHermit.h"

using namespace std;

extern Texture HauseTex, UnderTex, Hause2Tex;
extern ObjFile HauseObj, UnderObj, Hause2Obj, Under2Obj;

//земля
extern Texture LandTex;
extern ObjFile LandObj, StartObj;

extern CarСontrolSystem Car;

Vector3 HausePosition(10, 10, 0);
Vector3 Hause2Position(-15, 10, 0);
vector<Point> Start = { Point(0, 0, 0), Point(0, 0, 0), Point(0, 0, 5), Point(0, 0, 5) };

const double Hause_x_max = 6;
const double Hause_x_min = -6;
const double Hause_y_max = 6.9;
const double Hause_y_min = -6.9;
const double Hause_R = 9.5;
const double Hause2_R = 9;

double Hause_x_maxNow;
double Hause_x_minNow;
double Hause_y_maxNow;
double Hause_y_minNow;

void UpdatePointHause() {
	Hause_x_maxNow = Hause_x_max - HausePosition.X();
	Hause_x_minNow = Hause_x_min - HausePosition.X();
	Hause_y_maxNow = Hause_y_max - HausePosition.Y();
	Hause_y_minNow = Hause_y_min - HausePosition.Y();
}

double MoveZ() {
	double matrix[3][3] = {
		{cos(Car.AngleZ * PI / 180), -1 * sin(Car.AngleZ * PI / 180), 0},
		{sin(Car.AngleZ * PI / 180), cos(Car.AngleZ * PI / 180), 0},
		{0, 0, 1}
	};
	return  SearchDeterminantMatrix3(matrix);
}

void StopCarHause() {
	double CD = sqrt(pow(Car.CarPosition.X() - HausePosition.X(), 2) + pow(Car.CarPosition.Y() - HausePosition.Y(), 2)) - Car.R - Hause_R;
	double CD2 = sqrt(pow(Car.CarPosition.X() - Hause2Position.X(), 2) + pow(Car.CarPosition.Y() - Hause2Position.Y(), 2)) - Car.R - Hause2_R;
	if (CD == 0 || CD2 == 0) {
		Car.KeyDuration_W = 0;
	}
	else {
		Car.Stop = false;
	}
	if (CD < 0 || CD2 < 0) {
		Car.Stop = true;
		Car.CarPosition = Car.CarPosition_back;
	}
	else {
		Car.Stop = false;
	}
}

vector<Point> PlanePoint = { Point(50, 50, 10), Point(-100, -100, 3), Point(50, 50, 14), Point(-100, 100, 4) };
//задаем имя файла с моделью и текстрой
extern Texture PlaneTex, FlapsTex, HeliceTex, RearFlapTex, WheelsTex;
extern ObjFile PlaneObj, FlapsObj, HeliceObj, RearFlapObj, WheelsObj;

void DrawPlane() {
	static double t_max = 0;
	static double t_max_next = 0;
	static bool flag_tmax = true;

	//настройка времени
	double delta_time = Search_delta_time();
	double go = delta_time / 5; //t_max становится = 1 за 5 секунд
	//t_max сама по себе изменяется от 0 до 1 постепенно от кадра к кадру
	if (flag_tmax) {
		t_max += go;
		t_max_next = t_max + go;
		if (t_max > 1) {
			t_max = 1;
			flag_tmax = false;
		}
		if (t_max_next > 1) {
			t_max_next = 1;
		}
	}
	else {
		t_max -= go;
		t_max_next = t_max - go;
		if (t_max < 0) {
			t_max = 0;
			flag_tmax = true;
		}
		if (t_max_next < 0) {
			t_max_next = 0;
		}
	}

	glPushMatrix();

	Point point = BezierCurve3(PlanePoint[0], PlanePoint[1], PlanePoint[2], PlanePoint[3], t_max);
	Point next_point = BezierCurve3(PlanePoint[0], PlanePoint[1], PlanePoint[2], PlanePoint[3], t_max_next);
	point.Translated();
	glRotated(-90, 0, 0, 1);
	MoveAxis(point, next_point);
	PlaneTex.bindTexture();
	PlaneObj.DrawObj();

	glPopMatrix();
}
//стартовая точка
void MovePovStart() {
	static double t_max = 0;
	static double t_max_next = 0;
	static bool flag_tmax = true;

	//настройка времени
	double delta_time = Search_delta_time();
	double go = delta_time / 5; //t_max становится = 1 за 5 секунд
	//t_max сама по себе изменяется от 0 до 1 постепенно от кадра к кадру
	if (flag_tmax) {
		t_max += go;
		t_max_next = t_max + go;
		if (t_max > 1) {
			t_max = 1;
			flag_tmax = false;
		}
		if (t_max_next > 1) {
			t_max_next = 1;
		}
	}
	else {
		t_max -= go;
		t_max_next = t_max - go;
		if (t_max < 0) {
			t_max = 0;
			flag_tmax = true;
		}
		if (t_max_next < 0) {
			t_max_next = 0;
		}
	}

	//Начальная позиция машины
	//отключаем свет
	glDisable(GL_LIGHTING);
	//отключаем текстуры
	glDisable(GL_TEXTURE_2D);
	//включаем режим смешивания
	glEnable(GL_BLEND);
	////задаем опцию для коэффициентов источника и приемника
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3d(0.5, 0.2, 0.4);
	glColor4d(0.4, 0.6, 0.4, 0.6);

	glPushMatrix();

	Point point = BezierCurve3(Start[0], Start[1], Start[2], Start[3], t_max);
	Point next_point = BezierCurve3(Start[0], Start[1], Start[2], Start[3], t_max_next);
	point.Translated();
	glRotated(90, 0, 0, 1);
	StartObj.DrawObj();
	glPopMatrix();

	//выключаем режим смешивания
	glDisable(GL_BLEND);
	//включаем свет
	glEnable(GL_LIGHTING);
	//включаем текстуры
	glEnable(GL_TEXTURE_2D);
}

void WorkLand() {

	//рисуем домик
	glPushMatrix();
	glTranslated(HausePosition.X(), HausePosition.Y(), HausePosition.Z());
	glRotated(180, 0, 0, 1);
	//земля
	UnderTex.bindTexture();
	UnderObj.DrawObj();
	//дом
	HauseTex.bindTexture();
	HauseObj.DrawObj();
	glPopMatrix();

	glPushMatrix();
	glTranslated(Hause2Position.X(), Hause2Position.Y(), Hause2Position.Z());
	glRotated(180, 0, 0, 1);
	//земля
	UnderTex.bindTexture();
	Under2Obj.DrawObj();


	//дом2
	Hause2Tex.bindTexture();
	Hause2Obj.DrawObj();
	glPopMatrix();

	//рисуем землю
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor3d(.0, .0, .0);
	LandObj.DrawObj();
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	//проверка на стену
	StopCarHause();

	DrawPlane();

	//Начальная позиция машины
	MovePovStart();
}

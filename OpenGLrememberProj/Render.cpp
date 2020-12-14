#pragma once
#include "Render.h"

#include <windows.h>

#include <GL\gl.h>
#include <GL\glu.h>
#include "GL\glext.h"

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "MyShaders.h"

#include "ObjLoader.h"
#include "GUItextRectangle.h"

#include "Texture.h"

//мое
#include<vector>
#include "WorkPlane.h"
#include "WorkCar.h"
#include "PlaneСontrolSystem.h"
#include "CarСontrolSystem.h"
#include "CustomCamera.h"
#include "WorkLand.h"

using namespace std;

GuiTextRectangle rec;

bool textureMode = true;
bool lightMode = true;

//небольшой дефайн для упрощения кода
#define POP glPopMatrix()
#define PUSH glPushMatrix()


ObjFile *model;

Texture texture1;
Texture sTex;
Texture rTex;
Texture tBox;

//вектор для шейдеров (задаем именя вершинного и фрагментного шейдеров
vector<Shader> shader = { Shader("shaders\\v.vert", "shaders\\light.frag"), 
						  Shader("shaders\\v.vert", "shaders\\textureShader.frag"),
						  Shader("shaders\\myShader.vert", "shaders\\myShader.frag")
						};

//задаем имя файла с моделью и текстрой
Texture PlaneTex, FlapsTex, HeliceTex, RearFlapTex, WheelsTex;
ObjFile PlaneObj, FlapsObj, HeliceObj, RearFlapObj, WheelsObj;

//машинка
Texture CarTex, CarWheelTex, CarPlay1Tex, CarStop1Tex, CarStop2Tex;
ObjFile CarObj, CarWheelObj, CarPlayLObj, CarPlayRObj, CarStopLObj, CarStopLPovObj, CarStopRObj, CarStopRPovObj;

//дом
Texture HauseTex, UnderTex, Hause2Tex;
ObjFile HauseObj, UnderObj, Hause2Obj, Under2Obj;

//земля
Texture LandTex;
ObjFile LandObj, StartObj;
						  
//переменные для работы с движением самолета
PlaneСontrolSystem Plane = PlaneСontrolSystem();

//переменная для работы с движением машины
CarСontrolSystem Car = CarСontrolSystem();

//свободный режим камеры
bool freeCamera = false;

//создаем объект камеры
CustomCamera  camera;   

//время последнего кадра
double TimeLastFrame = 0;

//класс недоделан!
class WASDcamera :public CustomCamera
{
public:
		
	float camSpeed;

	WASDcamera()
	{
		camSpeed = 0.4;
		pos.setCoords(5, 5, 5);
		lookPoint.setCoords(0, 0, 0);
		normal.setCoords(0, 0, 1);
	}

	virtual void SetUpCamera()
	{

		if (OpenGL::isKeyPressed('W'))
		{
			Vector3 forward = (lookPoint - pos).normolize()*camSpeed;
			pos = pos + forward;
			lookPoint = lookPoint + forward;
			
		}
		if (OpenGL::isKeyPressed('S'))
		{
			Vector3 forward = (lookPoint - pos).normolize()*(-camSpeed);
			pos = pos + forward;
			lookPoint = lookPoint + forward;
			
		}

		LookAt();
	}

} WASDcam;


//Класс для настройки света
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(1, 1, 3);
	}

	
	//рисует сферу и линии под источником света, вызывается движком
	void  DrawLightGhismo()
	{
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		Shader::DontUseShaders();
		bool f1 = glIsEnabled(GL_LIGHTING);
		glDisable(GL_LIGHTING);
		bool f2 = glIsEnabled(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);
		bool f3 = glIsEnabled(GL_DEPTH_TEST);
		
		glDisable(GL_DEPTH_TEST);
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();

		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//линия от источника света до окружности
				glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//рисуем окруность
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}
		/*
		if (f1)
			glEnable(GL_LIGHTING);
		if (f2)
			glEnable(GL_TEXTURE_2D);
		if (f3)
			glEnable(GL_DEPTH_TEST);
			*/
	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //создаем источник света



//старые координаты мыши
int mouseX = 0, mouseY = 0;




float offsetX = 0, offsetY = 0;
float zoom=1;
float Time = 0;
int tick_o = 0;
int tick_n = 0;

//обработчик движения мыши
void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		//camera.fi1 += 0.01*dx;
		//camera.fi2 += -0.01*dy;

		camera.fi1 += 0.005 * dx;
		camera.fi2 += -0.005 * dy;
		camera.pos.setCoords(camera.camDist * cos(camera.fi2) * cos(camera.fi1),
			camera.camDist * cos(camera.fi2) * sin(camera.fi1),
			camera.camDist * sin(camera.fi2));
		freeCamera = true;
		camera.SetLookPoint(Car.CarPosition);
	}


	if (OpenGL::isKeyPressed(VK_LBUTTON))
	{
		offsetX -= 1.0*dx/ogl->getWidth()/zoom;
		offsetY += 1.0*dy/ogl->getHeight()/zoom;
	}

	//двигаем свет по плоскости, в точку где мышь
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y,60,ogl->aspect);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

Vector3 ReturnTMP_Car() {
		return Vector3(
			Car.CarPosition.X() - Car.Distance * cos(Car.AngleZ * PI / 180),
			Car.CarPosition.Y() - Car.Distance * sin(Car.AngleZ * PI / 180),
			Car.CarPosition.Z() + Car.Height);
}

//обработчик вращения колеса  мыши
void mouseWheelEvent(OpenGL *ogl, int delta)
{
	//float _tmpZ = delta*0.003;
	//if (ogl->isKeyPressed('Z'))
	//	_tmpZ *= 10;
	//zoom += 0.2*zoom*_tmpZ;
	//
	//
	//if (delta < 0 && camera.camDist <= 1)
	//	return;
	//if (delta > 0 && camera.camDist >= 100)
	//	return;
	//
	//camera.camDist += 0.01*delta;

	//вращение камерой
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.camDist += 0.005 * delta;
		camera.pos.setCoords(camera.camDist * cos(camera.fi2) * cos(camera.fi1),
			camera.camDist * cos(camera.fi2) * sin(camera.fi1),
			camera.camDist * sin(camera.fi2));
		camera.SetLookPoint(Car.CarPosition);
	}
	//скролл - приближаем и отдаляем камеру от объекта
	else
	{
		if (Car.Distance + 0.005 * delta < 2)
		{
			return;
		}

		Car.Distance += 0.005 * delta;
		camera.pos = ReturnTMP_Car();
		camera.SetLookPoint(Car.CarPosition);
	}
}

//обработчик нажатия кнопок клавиатуры
void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}	   

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}

	if (key == 'Q') {
		Time = 0;
	}

	//возвращаем камеру в привычное состояние (за моделью)
	if (key == 'M')
	{
		freeCamera = false;
		camera.pos = ReturnTMP_Car();
		camera.camDist = 30;
		camera.SetLookPoint(Car.CarPosition);
	}

}

void keyUpEvent(OpenGL *ogl, int key)
{
	//движение вперед
	if (key == 'W') {
		Car.FlagAcceleration = false;
	}

	//движение назад
	if (key == 'S') {
		Car.FlagDeceleration = false;
	}
}


//выполняется перед первым рендером
void initRender(OpenGL *ogl)
{

	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);

	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	//ogl->mainCamera = &WASDcam;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH); 


	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
	
	for (int i = 0; i < (int)shader.size(); i++) {
		// загружаем шейдеры из файла
		shader[i].LoadShaderFromFile();
		//компилируем
		shader[i].Compile();
	}

	 //так как гит игнорит модели *.obj файлы, так как они совпадают по расширению с объектными файлами, 
	 //создающимися во время компиляции, я переименовал модели в *.obj_m

	//загружаем модель и текстуры
	
	//машинка
	loadModel("models\\Car\\MyCar14.obj_m", &CarObj);
	CarTex.loadTextureFromFile("textures//Car//MyCar.bmp");
	CarTex.bindTexture();

	//колеса машины
	loadModel("models\\Car\\MyCarWheelNew.obj_m", &CarWheelObj);
	CarWheelTex.loadTextureFromFile("textures//Car//MyCarWheelNew1.bmp");
	CarWheelTex.bindTexture();

	//передние фары
	loadModel("models\\Car\\MyCarPlayL.obj_m", &CarPlayLObj);
	loadModel("models\\Car\\MyCarPlayR.obj_m", &CarPlayRObj);
	CarPlay1Tex.loadTextureFromFile("textures//Car//Play.bmp");
	CarPlay1Tex.bindTexture();

	//задние фары
	loadModel("models\\Car\\MyCarStopL.obj_m", &CarStopLObj);
	loadModel("models\\Car\\MyCarStopR.obj_m", &CarStopRObj);
	loadModel("models\\Car\\MyCarStopLPov.obj_m", &CarStopLPovObj);
	loadModel("models\\Car\\MyCarStopRPov.obj_m", &CarStopRPovObj);

	CarStop1Tex.loadTextureFromFile("textures//Car//Stop1.bmp");
	CarStop1Tex.bindTexture();

	CarStop2Tex.loadTextureFromFile("textures//Car//Stop2.bmp");
	CarStop2Tex.bindTexture();
	
	//дом
	loadModel("models\\Land\\Cottage.obj_m", &HauseObj);
	HauseTex.loadTextureFromFile("textures//Land//Hause.bmp");
	HauseTex.bindTexture();
	//трава под домом
	loadModel("models\\Land\\Under.obj_m", &UnderObj);
	UnderTex.loadTextureFromFile("textures//Land//Under.bmp");
	UnderTex.bindTexture();

	//дом2
	loadModel("models\\Land\\Hause2.obj_m", &Hause2Obj);
	Hause2Tex.loadTextureFromFile("textures//Land//Hause2.bmp");
	Hause2Tex.bindTexture();
	//трава под домом
	loadModel("models\\Land\\Under2.obj_m", &Under2Obj);

	//земля
	loadModel("models\\Land\\land.obj_m", &LandObj);
	LandTex.loadTextureFromFile("textures//Land//land.bmp");
	LandTex.bindTexture();
	
	//начальная позиция машины
	loadModel("models\\Land\\Start.obj_m", &StartObj);
	
	//самолет
	loadModel("models\\Plane\\Plane_simple.obj_m", &PlaneObj);
	PlaneTex.loadTextureFromFile("textures//Plane//Plane.bmp");
	PlaneTex.bindTexture();

	//закрылки
	loadModel("models\\Plane\\Flap_simple.obj_m", &FlapsObj);
	FlapsTex.loadTextureFromFile("textures//Plane//Flap.bmp");
	FlapsTex.bindTexture();

	//задний закрылок
	loadModel("models\\Plane\\RearFlap.obj_m", &RearFlapObj);
	RearFlapTex.loadTextureFromFile("textures//Plane//RearFlap.bmp");
	RearFlapTex.bindTexture();

	//пропеллер
	loadModel("models\\Plane\\Helice_simple.obj_m", &HeliceObj);
	HeliceTex.loadTextureFromFile("textures//Plane//Helice.bmp");
	HeliceTex.bindTexture();

	//колеса
	loadModel("models\\Plane\\Helice_simple.obj_m", &WheelsObj);
	WheelsTex.loadTextureFromFile("textures//Plane//Wheels.bmp");
	WheelsTex.bindTexture();

	tick_n = GetTickCount();
	tick_o = tick_n;

	//добавляем надпись
	rec.setSize(800, 100);
	rec.setPosition(10, ogl->getHeight() - 100-10);
	rec.setText("T - вкл/выкл текстур\nL - вкл/выкл освещение\nF - Свет из камеры\nG - двигать свет по горизонтали\nG+ЛКМ двигать свет по вертекали\nWASD - управоение самолетом\nпробел - останосить\nM - камеру за самолет\nP - исходное положение",0,0,0);
}




void Render(OpenGL *ogl)
{   
	
	tick_o = tick_n;
	tick_n = GetTickCount();
	Time += (tick_n - tick_o) / 1000.0;

	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	*/

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);

	//альфаналожение
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//настройка материала
	//GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	//GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	//GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat amb[] = { 1., 1., 1., 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;

	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//===================================
	//Прогать тут  
	shader[0].UseShader();
	
	//передача параметров в шейдер.  Шаг один - ищем адрес uniform переменной по ее имени. 
	int location = glGetUniformLocationARB(shader[0].program, "light_pos");
	//Шаг 2 - передаем ей значение
	glUniform3fARB(location, light.pos.X(), light.pos.Y(), light.pos.Z());
	
	location = glGetUniformLocationARB(shader[0].program, "Ia");
	glUniform3fARB(location, 0.2, 0.2, 0.2);
	
	location = glGetUniformLocationARB(shader[0].program, "Id");
	glUniform3fARB(location, 1.0, 1.0, 1.0);
	
	location = glGetUniformLocationARB(shader[0].program, "Is");
	glUniform3fARB(location, .7, .7, .7);
	
	
	location = glGetUniformLocationARB(shader[0].program, "ma");
	glUniform3fARB(location, 0.2, 0.2, 0.1);
	
	location = glGetUniformLocationARB(shader[0].program, "md");
	glUniform3fARB(location, 0.4, 0.65, 0.5);
	
	location = glGetUniformLocationARB(shader[0].program, "ms");
	glUniform4fARB(location, 0.9, 0.8, 0.3, 25.6);
	
	location = glGetUniformLocationARB(shader[0].program, "camera");
	glUniform3fARB(location, camera.pos.X(), camera.pos.Y(), camera.pos.Z());
	
	Shader::DontUseShaders();

	//рисуем самолет
	//WorkPlane();

	//машинка
	WorkCar();

	//домики и земля
	WorkLand();
}   


bool gui_init = false;

//рисует интерфейс, вызывется после обычного рендера
void RenderGUI(OpenGL *ogl)
{
	
	Shader::DontUseShaders();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	

	glActiveTexture(GL_TEXTURE0);
	rec.Draw();


		
	Shader::DontUseShaders(); 



	
}

void resizeEvent(OpenGL *ogl, int newW, int newH)
{
	rec.setPosition(10, newH - 100 - 10);
}


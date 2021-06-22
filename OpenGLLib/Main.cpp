#define _CRT_SECURE_NO_WARNINGS
#include<conio.h>
#include<stdio.h>
#include"Labirint.h"
#include "cmath"

#include"glut.h"

#include "windows.h"

walls*  labirint; // данные о лабиринте 
				  // угол поворота камеры
float angle = 0.0;
// координаты вектора направления движения камеры
float lx = 0.0f, lz = -1.0f;
// XZ позиция камеры
float x = 0.0f, z = -21.0f;
float tx, tz;

void init(void);
void drawsample(void);
void reshape(int, int);
void makecube(float ,float ,float ,float);
void processSpecialKeys(int key, int xx, int yy) {
	float fraction = 0.2;
	switch (key) {
	case GLUT_KEY_LEFT:
		for (int i = 0; i < 10; i++)
		{
			angle -= 0.0785;
			lx = sin(angle);
			lz = -cos(angle);
			drawsample();
		}
		break;
	case GLUT_KEY_RIGHT:
		for (int i = 0; i < 10; i++)
		{
			angle += 0.0785;
			lx = sin(angle);
			lz = -cos(angle);
			drawsample();
		}
		break;
	case GLUT_KEY_UP:
		for (int i = 0; i < 10; i++)
		{
			tx = x + lx * fraction;
			tz = z + 11 + lz * fraction;

			if (round(tx) >= 0 && round(tx) < labirint->kolstrok && round(tz) >= 0 && round(tz) < labirint->kolstolb)
				if (labirint->p[int(round(tx))][int(round(tz))])
					return;

			x = tx;
			z = tz - 11;
			drawsample();
		}
		break;
	case GLUT_KEY_DOWN:
		for (int i = 0; i < 10; i++)
		{
			tx = x - lx * fraction;
			tz = z + 11 - lz * fraction;

			if (round(tx) >= 0 && round(tx) < labirint->kolstrok && round(tz) >= 0 && round(tz) < labirint->kolstolb)
				if (labirint->p[int(round(tx))][int(round(tz))])
					return;

			x = tx;
			z = tz - 11;
			drawsample();
		}
		break;
	}
}

void drawquad(float x, float y, float size)
{
	glBegin(GL_QUADS);
	{
		glColor3f(102.0/255, 204.0/255, 0);
		glVertex2f(x, y);
		glVertex2f(x + size, y);
		glVertex2f(x + size, y + size);
		glVertex2f(x, y + size);
	}
	glEnd();
	
}

void DrawMakrer(float cx, float cy, float size)
{
	size *= 3;

	glBegin(GL_TRIANGLES);
	{
		glColor3f(1, 1, 0);
		glVertex2f(cx + size * lx, cy + size * lz);
		glVertex2f(cx - size * lz * 0.5, cy + size * lx * 0.5);
		glVertex2f(cx + size * lz * 0.5, cy - size * lx * 0.5);
	}
	glEnd();	
}

void HUD(float hudSize)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();        ----Not sure if I need this
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);
	float offset = hudSize / 5;
	float size = hudSize / labirint->kolstrok;
	float scale = hudSize / labirint->kolstolb;
	printf("%f %f\n", x, z);
	DrawMakrer(x * scale + offset, z * scale + 25 + offset, size);
	
	for (int i = 0; i < labirint->kolstrok; i++)
		for (int j = 0; j < labirint->kolstolb; j++)
			if (labirint->p[i][j])
				drawquad(i * size + offset, j * size + offset, size);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.0 + offset, 0.0 + offset);
	glVertex2f(hudSize + offset, 0.0 + offset);
	glVertex2f(hudSize + offset, hudSize + offset);
	glVertex2f(0.0 + offset, hudSize + offset);
	glEnd();
	
	

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc,char** argv)
{
	labirint = readfromfile("Map.txt");
	glutInit(&argc, argv); // инициализирует глут 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // 1 активация буффера кадра 2-цветовой режим GLUT_SINGLE - без буффера кадра
	glutInitWindowPosition(100, 100); //позиция окна на экране компа относительно экрана 
	glutCreateWindow(argv[0]); //путь к файлу

	init();

	glutDisplayFunc(drawsample); // выводит на экран 

	glutReshapeFunc(reshape);
	glutIdleFunc(drawsample);
	glutSpecialFunc(processSpecialKeys);

	glutMainLoop(); // зацикливает воспроизвденеие окна
	
	_getch();
	return 0;
}
void init()
{
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
}

void drawsample()
{
	glMatrixMode(GL_MODELVIEW); // задание режима матрицы projection modal view
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //буфер глубины и буфер цвета 
	glLoadIdentity(); //загрузить матрицу ,матрица преобразований 
	
	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0, 1.0f, 0);
	
	glTranslatef(0, 0.1, -10); // отодвинуться назад , 

	for (int i = 0; i < labirint->kolstrok; i++)
		for (int j = 0; j < labirint->kolstolb; j++)
			if (labirint->p[i][j])
				makecube(i, 0, j , 1);

	HUD(300);

	glLoadIdentity();
	glutSwapBuffers(); // поменять мстами буффер кадров 2 буффера - видимый не видимый , отрисока на невидимом и потом меняем местами 
	glFlush(); // очистить содержимое экрана 

}
void reshape(int x, int y) // размер окна
{
	if (y == 0 || x == 0) return;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // загрузка матрицы
					  // не дальше 20 , отрисовываться не будет
	gluPerspective(40.0, (GLdouble)x / (GLdouble)y, 0.5, 100.0); // 1- угол обзора , 2- отношения для 3-расстояние до плоскости отсечения сколько нуно отрисовывать , а сколько нет
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, x, y);
}
void  makecube(float x, float y, float z, float size) //size - длина стороны куба

{
	glBegin(GL_QUADS);
	glColor3f(0, 0, 1); // 3i 0-1 интенсивность каждой компаненты
	glVertex3f(x,y,z); 
	glVertex3f(x+size, y,z);
	glVertex3f(x,y+size, z);
	glVertex3f(x+size,y+size, z);

	glColor3f(0, 1, 0);
	glVertex3f(x,y,z);
	glVertex3f(x, y+size, z);
	glVertex3f(x, y+size, z-size);
	glVertex3f(x,y , z-size);
	
	glColor3f(0, 1, 1);
	glVertex3f(x, y+size, z-size);
	glVertex3f(x, y , z - size);
	glVertex3f(x+size, y+size, z-size );
	glVertex3f(x+size, y, z-size);

	glColor3f(1, 0, 0);
	glVertex3f(x, y+size, z);
	glVertex3f(x, y+size, z-size);
	glVertex3f(x+size, y+size, z-size);
	glVertex3f(x+size, y + size, z);

	glColor3f(1, 0, 1);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z-size);
	glVertex3f(x + size, y, z - size);
	glVertex3f(x+size, y, z);

	glColor3f(1, 1, 0);
	glVertex3f(x+size, y+size, z);
	glVertex3f(x + size, y + size, z-size);
	glVertex3f(x+size, y, z-size);
	glVertex3f(x + size, y,z);

	glEnd();

}
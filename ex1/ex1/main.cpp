// lab1.cpp : Defines the entry point for the console application.
//

//OpenGL要包含gl.h和glut.h，但glut文件中已经包含这两个文件了
#include <cmath>
#include <GL/glut.h>

#define PI 3.1415926

typedef struct Point3f
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
}point;

void getPoint(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat a, GLfloat b, point &p)
{

	p.x = rx * sin(a * PI / 180.0) * cos(b * PI / 180.0);
	p.y = ry * sin(a * PI / 180.0) * sin(b * PI / 180.0);
	p.z = rz * cos(a * PI / 180.0);
}

point* getPointMatrix(GLfloat rx, GLfloat ry, GLfloat rz, GLint slices)
{
	int i, j, w = 2 * slices, h = slices;
	float a = 0.0, b = 0.0;
	float hStep = 180.0 / (h - 1);
	float wStep = 360.0 / w;
	int length = w * h;
	point *matrix;
	matrix = (point *)malloc(length * sizeof(point));
	if (!matrix) return NULL;
	for (a = 0.0, i = 0; i < h; i++, a += hStep )
	{
		for (b = 0.0, j = 0; j < w; j++, b += wStep)
		{
			getPoint(rx, ry, rz, a, b, matrix[i * w + j]);
		}
	}
	return matrix;
}

void drawSlice(point &p1, point &p2, point &p3, point &p4)
{
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glVertex3f(p3.x, p3.y, p3.z);
	glVertex3f(p4.x, p4.y, p4.z);
	glEnd();
}

void drawOval(GLfloat rx, GLfloat ry, GLfloat rz, GLint slices)
{
	int i = 0, j = 0, w = 2 * slices, h = slices;
	point *mx;
	mx = getPointMatrix(rx, ry, rz, slices);
	if (!mx);
	for (; i<h - 1; i++)
	{
		for (j = 0; j<w - 1; j++)
		{
			drawSlice(mx[i*w + j], mx[i*w + j + 1], mx[(i + 1)*w + j + 1], mx[(i + 1)*w + j]);
		}
		drawSlice(mx[i*w + j], mx[i*w], mx[(i + 1)*w], mx[(i + 1)*w + j]);
	}
	free(mx);
}

// 球
void myDisplayGlobe()
{
	//清除，GL_COLOR_BUFFER_BIT表示清除颜色，glClear函数还可以清除其它的东西。
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//设置材料颜色
	glColor3ub(233, 50, 3);
	glRotated(-30, 1, 0, 10);
	glutWireSphere(0.4f, 20, 20);

	glFlush();
}

// 圆
void myDisplayCircle()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);         //设置窗口为白色1. 1. 1.  黑色0. 0. 0.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);                //颜色为红色

	glLineWidth(2.0f);
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_FILL);

	glFrontFace(GL_CCW);

	//circle
	int n = 360;
	double R = 0.2;
	glBegin(GL_POLYGON);
	for (int i = 0; i<n; i++)
		glVertex2f(R*cos(2 * PI*i / n), R*sin(2 * PI*i / n));
	glEnd();

	glFlush();

}

// 椭球
void myDisplayOval()
{
	int width = 640;
	int height = 480;
	float ratio = (float)width / (float)height;
	// Our shading model--Gouraud (smooth).
	glShadeModel(GL_SMOOTH);
	// Setup our viewport.
	glViewport(0, 0, width, height);
	// Change to the projection matrix and set our viewing volume.
	glLoadIdentity();
	gluPerspective(60.0, ratio, 1.0, 100.0);
	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// We don't want to modify the projection matrix. */
	glMatrixMode(GL_MODELVIEW);
	// Move down the z-axis.
	glTranslatef(0.0, 0.0, -25.0);
	glRotatef(100.0f, 0, 2, 2);
	
	drawOval(5, 8, 15, 20);

	glFlush();
}

int main(int argc, char *argv[])
{
	//对glut初始化，这个函数必须在其它glut使用之前调用一次。
	//其格式比较死板，一般用glutInit(&argc, argv)就可以了。
	glutInit(&argc, argv);

	//设置显示方式，其中GLUT_RGB表示使用RGB颜色，与之对应的还有GLUT_INDEX（表示使用索引颜色）。
	//GLUT_SINGLE表示使用单缓冲，与之对应的还有GLUT_DOUBLE（使用双缓冲）。
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glutInitWindowPosition(100, 100);//设置窗口在屏幕中的位置
	glutInitWindowSize(640, 480);//设置窗口的大小

	// glutCreateWindow("圆");
	// glutDisplayFunc(myDisplayCircle);

	glutCreateWindow("椭球");
	glutDisplayFunc(myDisplayOval);

	glutCreateWindow("球体");
	glutDisplayFunc(myDisplayGlobe);

	glutMainLoop();

	return 0;
}

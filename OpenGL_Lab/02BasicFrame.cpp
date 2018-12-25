#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#define PI 3.1415926

// 金字塔初始旋转角度
GLfloat rtri = 90;
// 用户自定义三维空间的齐次坐标矩阵(4x4)一直用于输出查看变化矩阵的变化
typedef float Mat44[16];

// 自定义初始化opengl环境
void init(void)
{
    // 设置背景色用于填充背景
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // 设置多边形填充模式为smooth模式
    glShadeModel(GL_SMOOTH);
    // 打开深度测试开关用于检测物体之间的z深度差异
    glEnable(GL_DEPTH_TEST);
    // 线的抗锯齿开关
    glEnable(GL_LINE_SMOOTH);
    // 启用抗锯齿效果
    glHint(GL_LINE_SMOOTH, GL_NICEST);
    // 指定混合函数
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    // 启用色彩混合状态
    glEnable(GL_BLEND);
}

// 输出4x4矩阵的结果
void outputmat(const Mat44 &mat)
{
    
    for(size_t i = 0; i < 4; i++)
    {
        
        for(size_t j = 0; j < 4; j++)
        {
            printf("%.3f", mat[i * 4 + j]);
        }
        printf("\n");      
    }
}

// 自定义绘制圆函数
void glCircle(GLfloat radius, GLint slices)
{
    int i;
    // 将绘图前的模型变换矩阵压入堆栈
    glPushMatrix();
    // 设置颜色RGB与透明值
    glColor4f(0.0f, 0.2f, 0.8f, 0.5f);
    // 发出准备绘图命令
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    
    for(size_t i = 0; i < slices; i++)
    {
        glVertex2f(radius * cos(2 * PI / slices * i), radius * sin(2 * PI / slices* 2));
    }
    // 发出绘图命令
    glEnd();
    // 绘图后，恢复绘图前的模型变换矩阵
    // 这样，对当前图形的变换和后续图形的绘制不影响
    glPopMatrix();
}

// 自定义绘制点函数


// 用户自定义绘图函数
void display(void)
{
    // 清楚色彩缓存和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 装入单位矩阵 [1 0 0 0; 0 1 0 0; 0 0 1 0; 0 0 0 1]
    glLoadIdentity();
    // 平移变换命令：之后绘制的所有对象将沿Z轴向屏幕内移动5个单位
    glTranslatef(0.0f, 0.0f, -5.0f);
    // 旋转变换命令：之后绘制的所有对象将沿Y轴正向旋转rtri个角度单位
    glRotatef(rtri, 0.0f, 1.0f, 0.0f);
    // 查看当前的4x4矩阵变量
    Mat44 mat;
    // 取得模型-视图变换矩阵
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    // 查看上述变换后的总变换矩阵的结果
    outputmat(mat);
    // 设置点元大小为5像素
    glPointSize(5);
    // 发出命令：开始绘制点
    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f);    // 点为红色
    glVertex3f(0.7f, 0.5f, 0.4f);   // 顶点(vertex位置)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.7f, 0.5f, -0.4f);
    glEnd();
    // 绘制坐标轴
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(3.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 3.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 3.0f);
    glEnd();
    // 绘制两个三角形
    glBegin(GL_TRIANGLES);
    glColor4f(0.0f, 0.0f, 1.0f, 0.5);
    glVertex2f(-0.7f, 0.0f);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(0.0f, 0.5f);
    glColor4f(0.0f, 1.0f, 1.0f, 0.5f);
    glVertex3f(-0.5f, -0.2f, 0.0f);
    glVertex3f(0.8f, -0.2f, 0.3f);
    glVertex3f(0.0f, 0.2f, -0.5f);
    glEnd();
    // 绘制实心的圆环：在原有变换基础上增加了新的变换
    // 为了不影响后续对象的变换，采用压栈的方式，保存当前的变换矩阵
    glPushMatrix();
    {
        glTranslatef(0.0f, 0.0f, -3.0f);        // 用于实心圆环的变换
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glScalef(1.0f, 1.0f, 1.2f);
        glutSolidSphere(0.7, 100, 100);
        glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
        glScalef(1.0f, 1.0f, 1.0f);
        glutSolidTorus(0.1, 0.71, 100, 100); // 绘制实心圆环
    }
    glPopMatrix();
    // 绘制实心球
    glPushMatrix();
    {
        glTranslatef(1.0f, 1.0f, 3.0f);         // 增加了变换
        glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
        glTranslatef(0.5f, 0.0f, 0.0f);
        glutSolidSphere(0.4, 30, 30);    // 绘制球体
        glTranslatef(0.0f, 0.0f, -50.0f);
        glutSolidSphere(0.4, 30, 30);    // 绘制球体
    }
    glPopMatrix();
    // 开始绘制直线段
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();    
    // 平移变换
    glTranslatef(-2.0f, 0.0f, 0.0f);
    glCircle(1.0f, 50);
    // 平移变换
    glTranslatef(2.0f, 0.0f, 0.0f);
    // 绘制立方体线框
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
    glutWireCube(1.0);
    rtri += 0.3;// 全局的旋转变量加0.3 度      
    // 用缓冲区所绘制的对象替换窗口内容——适合于双缓冲技术
    glutSwapBuffers();//交换双缓存

}

// 用户自定义窗口调整大小事件的处理函数
// 在这个函数中要求设置视口、投影、透视变换的相关参数
void reshape(GLsizei w, GLsizei h)
{
    // 设置视口参数为整个窗口返回内
    glViewport(0, 0, w, h);
    // 设置投影参数：投影矩阵初始为单位阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 设置透视参数：摄像机的视角参数为60度，视景体的宽度与高度比，
    // 视距(near)和视径(far)参数
    // near = 1, far = 100, Z轴负向顺着视线方向指向屏幕内
    gluPerspective(60.0, w/h, 1.0, 100.0);
    // 设置摄像机的位置及姿态参数
    // 摄像机位置(cX, cY, cZ)
    // 视点观察中心位置Ow(oX, oY, oZ)
    // 摄像机位参数：摄像机顶部矢量
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // 设置矩阵模式为模型-试图变换模式，以便后面的自定义显示函数继续本模式
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'x':
        case 27: //ESC 键盘
        exit(0);
        break;
        default:
        break;
    }
}

int main(int argc, char *argv[])
{
    // 用命令行参数初始化OpenGL
    glutInit(&argc, argv);
    // 使用RGB色彩、双缓存和深度模式
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // 初始化绘图窗口大小参数
    glutInitWindowSize(1024, 768);
    // 初始化窗口位置
    glutInitWindowPosition(0, 0);
    // 创建窗口
    glutCreateWindow("02BasicFrame");
    // 用户自定义初始化绘图环境函数
    init();
    // 用户制定的绘图函数
    glutDisplayFunc(display);
    // 窗口调整大小事件处理函数
    glutReshapeFunc(reshape);
    // 窗口键盘事件处理函数
    glutKeyboardFunc(keyboard);
    // 窗口空闲时调用的函数
    glutIdleFunc(display);
    // 进入glut函数库主循环
    glutMainLoop();
    return 0;
}

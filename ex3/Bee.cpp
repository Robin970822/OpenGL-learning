#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

//旋转参数
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

//确定多边形绕法的方向
bool bWinding = true;

void glCyLinder(float x0, float y0, float z0, float x1, float y1, float z1)
{   //如果要在AB两点间画一个圆柱体，其可以先在y轴上画一个同长度的圆柱，然后求出旋转矩阵，将其移至AB
    GLdouble dir_x = x1 - x0,

            dir_y = y1 - y0,

            dir_z = z1 - z0;

    GLdouble cy_length = sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z); //获得圆柱的长度

    static GLUquadricObj *quad_obj = NULL;

    if (quad_obj == NULL)

        quad_obj = gluNewQuadric();

    gluQuadricDrawStyle(quad_obj, GLU_FILL);

    gluQuadricNormals(quad_obj, GLU_SMOOTH);

    glPushMatrix();
    //将当前矩阵压入矩阵堆栈

    glTranslated(x0, y0, z0);
    //平移到起点，即A点

    //获得AB的长度
    double length;

    length = sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);

    dir_x /= length;
    dir_y /= length;
    dir_z /= length;

    GLdouble up_x = 0.0,

             up_y = 1.0,

             up_z = 0.0;

    GLdouble side_x, side_y, side_z;

    //实现向量的叉乘
    side_x = up_y * dir_z - up_z * dir_y;

    side_y = up_z * dir_x - up_x * dir_z;

    side_z = up_x * dir_y - up_y * dir_x;

    length = sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);

    side_x /= length;
    side_y /= length;
    side_z /= length;

    up_x = dir_y * side_z - dir_z * side_y;

    up_y = dir_z * side_x - dir_x * side_z;

    up_z = dir_x * side_y - dir_y * side_x;

    //得到变换矩阵
    GLdouble m[] = {side_x, side_y, side_z, 0.0,

                    up_x, up_y, up_z, 0.0,

                    dir_x, dir_y, dir_z, 0.0,

                    0.0, 0.0, 0.0, 1.0};

    glMultMatrixd(m); //用m矩阵乘以当前矩阵

    GLdouble radius = 0.15; //这些参数可以自己设置

    GLdouble slices = 50.0;

    GLdouble stack = 50.0;

    gluCylinder(quad_obj, radius, radius, cy_length, slices, stack);

    glPopMatrix();
}

// 自定义初始化opengl函数
void init(void)
{
    /*-----------------------------光源-----------------------------*/
    // 材质的反光性设置
    GLfloat mat_specular[] = {0.0, 0.0, 0.0, 1.0}; // 镜面反射参数
    GLfloat mat_shininess[] = {10.0};               // 高光指数
    GLfloat mat_diffuse[] = {0.8, 0.8, 0.0, 1.0};  // 漫反射光参数

    // 光源属性设置
    GLfloat light_position[] = {-2.0, 1.2, 1.5, 0.0}; // 灯光位置
    GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat Light_Model_Ambient[] = {1.0, 1.0, 1.0, 1.0}; // 环境光参数

    // 材质属性
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    // 灯光设置
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);               // 散射光属性
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);              // 镜面反射光
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient); // 环境光参数

    glEnable(GL_LIGHTING); // 开关：使用光
    glEnable(GL_LIGHT0);   // 打开 #0 灯

    glClearColor(1.0, 1.0, 1.0, 0.0); // 背景色
    glShadeModel(GL_SMOOTH);          // 多边形填充模式
    glEnable(GL_DEPTH_TEST);          // 打开深度测试
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glRotatef(xRot, 1.0f, 0.0f, 0.0f); //使整个场景绕着x轴旋转
    glRotatef(yRot, 0.0f, 1.0f, 0.0f); //使整个场景绕着y轴旋转

    glPushMatrix();
    {
        glRotatef(-100, 1.0f, 0.0f, 0.0f); 
        glRotatef(30, 0.0f, 1.0f, 0.0f);

        glScalef(0.8f, 0.8f, 0.8f); 
        // 绘制椭球体
        glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
        glScalef(1.0f, 1.0f, 1.2f);
        glutSolidSphere(1.0, 100, 100);

        // 绘制第一个球带
        glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
        glScalef(1.0f, 1.0f, 1.0f);
        glutSolidTorus(0.10, 1.01, 100, 100);

        // 绘制第二个球带
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0f, -0.3f);
            glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
            glutSolidTorus(0.09, 0.91, 100, 100);
        }
        glPopMatrix();

        // 绘制第三个球带
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0f, 0.3f);
            glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
            glutSolidTorus(0.09, 0.91, 100, 100);
        }
        glPopMatrix();

        // 绘制第四个球带
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0f, -0.6f);
            glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
            glutSolidTorus(0.07, 0.81, 100, 100);
        }
        glPopMatrix();

        // 绘制第五个球带
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0f, 0.6f);
            glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
            glutSolidTorus(0.07, 0.81, 100, 100);
        }
        glPopMatrix();

        // 绘制两个球冠
        glPushMatrix();
        {
            GLdouble zPlane0[] = {0.0, 0.0, 1.0, -0.9};
            glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
            glClipPlane(GL_CLIP_PLANE0, zPlane0);
            glEnable(GL_CLIP_PLANE0);
            glutSolidSphere(1.1, 100, 100);
            glDisable(GL_CLIP_PLANE0);

            GLdouble zPlane1[] = {0.0, 0.0, -1.0, -0.9};
            glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
            glClipPlane(GL_CLIP_PLANE1, zPlane1);
            glEnable(GL_CLIP_PLANE1);
            glutSolidSphere(1.1, 100, 100);
            glDisable(GL_CLIP_PLANE1);
        }
        glPopMatrix();

        // 绘制圆柱与柱柄
        glPushMatrix();
        {
            glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
            glCyLinder(0.0, 0.0, -1.0, 0.0, 0.0, -1.6);
            glTranslatef(0.0f, 0.0f, -1.6f);
            glutSolidSphere(0.15, 100, 100);
        }
        glPopMatrix();
    }
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    //设置投影参数
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //正交投影
    if (w <= h)
        glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h / (GLfloat)w, 1.5 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
    else
        glOrtho(-1.5 * (GLfloat)w / (GLfloat)h, 1.5 * (GLfloat)w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0);

    //设置模型参数--几何体参数
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//由键盘控制旋转
void SpecialKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
        xRot -= 5.0f;
    if (key == GLUT_KEY_DOWN)
        xRot += 5.0f;
    if (key == GLUT_KEY_LEFT)
        yRot -= 5.0f;
    if (key == GLUT_KEY_RIGHT)
        yRot += 5.0f;

    if (xRot > 356.0f)
        xRot = 0.0f;
    if (xRot < -1.0f)
        xRot = 355.0f;
    if (yRot > 356.0f)
        yRot = 0.0f;
    if (yRot < -1.0f)
        yRot = 355.0f;
    glutPostRedisplay(); //刷新窗口
}

//菜单处理
void ProcessMenu(int value)
{
    switch (value)
    {
    case 1:
    {
        //修改多边形正面为填充模式
        glPolygonMode(GL_FRONT, GL_FILL);
        break;
    }
    case 2:
    {
        //修改多边形正面为线模式
        glPolygonMode(GL_FRONT, GL_LINE);
        break;
    }
    case 3:
    {
        //修改多边形正面为点填充模式
        glPolygonMode(GL_FRONT, GL_POINT);
        break;
    }
    case 4:
    {
        //修改多边形背面为填充模式
        glPolygonMode(GL_BACK, GL_FILL);
        break;
    }
    case 5:
    {
        //修改多边形背面为线模式
        glPolygonMode(GL_BACK, GL_LINE);
        break;
    }
    case 6:
    {
        //修改多边形背面为点填充模式
        glPolygonMode(GL_BACK, GL_POINT);
        break;
    }
    case 7:
    {
        //设置多边形的阴影模式为平面明暗模式
        glShadeModel(GL_FLAT);
        break;
    }
    case 8:
    {
        //设置多边形的阴影模式为光滑明暗模式
        glShadeModel(GL_SMOOTH);
        break;
    }
    case 9:
    {
        bWinding = !bWinding;
        break;
    }
    default:
        break;
    }
    //提交修改并强制重新绘图
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bee");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(SpecialKeys);

    //菜单的句柄
    int nModeMenu;
    int nMainMenu;
    int nColorMenu;

    //创建一个子菜单并定于菜单回调函数
    nModeMenu = glutCreateMenu(ProcessMenu);
    //添加菜单项，1表示选择菜单条目时传递的参数值
    glutAddMenuEntry("GL_FRONT,GL_FILL", 1);
    glutAddMenuEntry("GL_FRONT,GL_LINE", 2);
    glutAddMenuEntry("GL_FRONT,GL_POINT", 3);
    glutAddMenuEntry("GL_BACK,GL_FILL", 4);
    glutAddMenuEntry("GL_BACK,GL_LINE", 5);
    glutAddMenuEntry("GL_BACK,GL_POINT", 6);

    //添加一个子菜单
    nColorMenu = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("GL_FLAT", 7);
    glutAddMenuEntry("GL_SMOOTH", 8);

    //创建主菜单
    nMainMenu = glutCreateMenu(ProcessMenu);
    glutAddSubMenu("Mode", nModeMenu);
    glutAddSubMenu("Color", nColorMenu);
    glutAddMenuEntry("bWinding", 9);

    //将创建的菜单与右键关联，即把菜单设置为右键弹出式菜单
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
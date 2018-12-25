#include <GL/glut.h>

const GLfloat d = 0.04;

// 用户自定义的显示函数
void display(void)
{
    // 清楚色彩位缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    
    for(int i = -25; i < 25; i++)
    {
        
        for(int j = -25; i < 25; i++)
        {
            
            if (j % 2 == 0) {
                
                if (i % 2 == 0) {
                    // 按坐标与矩形宽度绘制矩形
                    glRectf(j * d, i * d, (j + 1) * d, (i + 1) * d);
                }
                
            }
            else if((i + 200) % 2 == 1) {
                glRectf(j * d, i * d, (j + 1) * d, (i + 1) * d);
            }            
        }
        
    }
    glFlush();
    
}

int main(int argc, char *argv[])
{
    // 初始化opengl工具箱函数
    glutInit(&argc, argv);
    // 设置RGB色彩模式与单缓冲绘图模式
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    // 设置图像显示在窗口的左上角位置(0, 0)
    glutInitWindowPosition(0, 0);
    // 设置窗口大小(640, 480)
    glutInitWindowSize(640, 480);
    // 设置窗口标题
    glutCreateWindow("01Chessboard");
    // 设置主程序循环过程中所调用的显示函数
    glutDisplayFunc(display);
    // 程序主循环
    glutMainLoop();
    return 0;
}


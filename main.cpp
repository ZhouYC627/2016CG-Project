//
//  main.cpp
//  GlutDemo
//
//  Created by Zhou.Y.C on 10/31/16.
//  Copyright © 2016 njucs. All rights reserved.
//
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "common.h"
#include "PatternGeneration.h"

int ww = 600, wh = 600;
int xi, yi, xf, yf;
bool firstClick = true;
vector<Point> polyPoints;
using namespace std;

void PolyScan(Point polypoints[], int num_line, double k0);

//Calls Bresenham function when the mouse has traced a line
bool drawLine = false;
int mode = 0;
void mouse(int btn, int state, int x, int y)
{
    if( btn == GLUT_LEFT_BUTTON && state == GLUT_UP ) {
        //simple pattern: 2 clicks
        if (mode<=2){
            if (firstClick) {
                xi = x;
                yi = (wh - y);
                firstClick = false;
                drawLine = false;
            }
            else {
                xf = x;
                yf = (wh - y);
                firstClick = true;
                drawLine = true;
                glutPostRedisplay();
            }
        }else if (mode == 4){
            Point now;
            now.x = x;
            now.y = wh - y;
            if (polyPoints.empty()){
                polyPoints.push_back(now);
            }else{
                Point lastPoint = polyPoints.back();
                bresenham(lastPoint.x, lastPoint.y, now.x, now.y);
                if ((abs(now.x-polyPoints[0].x)+abs(now.y-polyPoints[0].y))<20){
                    cout<<"Draw Poly!"<<endl;
                    PolyScan(&polyPoints[0], (int)polyPoints.size(), 0.5);
                    polyPoints.clear();
                }else{
                    polyPoints.push_back(now);
                }
                glutPostRedisplay();
            }
            //polyPoints.push_back(Point{x, wh-y});
        }
    }
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27: // Press escape to exit.
        exit(0);
        break;
        case 'l':
        mode = 0;
        break;
        case 'e':
        mode = 1;
        break;
        case ' ':
        mode = !mode;
        break;
        default:
        break;
    }
    //cout<<key<<endl;
}
/*
struct Point{
    int x;
    int y;
};
//种子填充算法
 void glPoint(int x,int y,int r,int g,int b){
 glColor3ub (r,g,b);
 glPointSize(1);
 glBegin(GL_POINTS);
 glVertex2i(x,y);
 glEnd();
 glFlush();
 }
//传入两个颜色的RGB值，比较是否相同，容差为dis
bool sameColor(int r1,int g1,int b1,int r2,int g2,int b2){
    //容差度
    cout<<r1<<g1<<b1<<endl;
    cout<<r2<<g2<<b2<<endl;
    system("pause");
    int dis = 10;
    if(abs(r1-r2)<=dis&&abs(g1-g2)<=dis&&abs(b1-b2)<=dis){
        return true;
    }else{
        return false;
    }
}

void Fill(int startX,int startY,int r,int g,int b){
    cout<<startX<<startY<<endl;
    GLubyte iPixel[3];
    
    stack<Point> pixelStack;
    //x,y是给定的种子像素点，rgb就是要填充的颜色的RGB值
    Point point  = {startX,startY};
    pixelStack.push(point);
    int saveX;
    int xRight,xLeft;
    int halfWidth = ww/2, halfHeight = wh/2;
    int x,y;
    //如果栈不为空
    while(!pixelStack.empty()){
        //获取最顶端的元素
        Point tempPoint=pixelStack.top();
        //删除最顶端的元素
        pixelStack.pop();
        saveX=tempPoint.x;
        x=tempPoint.x;
        y=tempPoint.y;
        glReadPixels(x+halfWidth,y+halfHeight,1,1,GL_RGB,GL_UNSIGNED_BYTE,&iPixel);
        //如果没有到达右边界，就填充
        while(!sameColor(iPixel[0],iPixel[1],iPixel[2],colorBlack[0],colorBlack[1],colorBlack[2])){
            glPoint(x,y,r,g,b);
            x=x+1;
            glReadPixels(x+halfWidth,y+halfHeight,1,1,GL_RGB,GL_UNSIGNED_BYTE,&iPixel);
        }
        xRight=x-1;
        x=saveX-1;
        glReadPixels(x+halfWidth,y+halfWidth,1,1,GL_RGB,GL_UNSIGNED_BYTE,&iPixel);
        //如果没有到达左边界，就填充
        while(!sameColor(iPixel[0],iPixel[1],iPixel[2],colorBlack[0],colorBlack[1],colorBlack[2])){
            glPoint(x,y,r,g,b);
            x=x-1;
            glReadPixels(x+halfWidth,y+halfWidth,1,1,GL_RGB,GL_UNSIGNED_BYTE,&iPixel);
        }
        //保存左端点
        xLeft=x+1;
        //从右边的点开始
        x=xRight;
        //检查上端的扫描线
        y=y+1;
        while(x>=xLeft){
            glReadPixels(x+halfWidth,y+halfWidth,1,1,GL_RGB,GL_UNSIGNED_BYTE,&iPixel);
            if(!sameColor(iPixel[0],iPixel[1],iPixel[2],colorBlack[0],colorBlack[1],colorBlack[2])&&!sameColor(iPixel[0],iPixel[1],iPixel[2],r,g,b)){
                //如果上方的点不是边界点，直接压入
                Point p={x,y};
                pixelStack.push(p);
                //压入之后停止循环
                break;
            }else{
                x--;
                glReadPixels(x+halfWidth,y+halfWidth,1,1,GL_RGB,GL_UNSIGNED_BYTE,&iPixel);
            }
        }
        //检查下端的扫描线
        y=y-2;
        //从右边的点开始
        x=xRight;
        while(x>=xLeft){
            glReadPixels(x+halfWidth,y+halfWidth,1,1,GL_RGB,GL_UNSIGNED_BYTE,&iPixel);
            if(!sameColor(iPixel[0],iPixel[1],iPixel[2],colorBlack[0],colorBlack[1],colorBlack[2])&&!sameColor(iPixel[0],iPixel[1],iPixel[2],r,g,b)){
                //如果上方的点不是边界点，直接压入
                Point p={x,y};
                //压入之后停止循环
                pixelStack.push(p);
                break;
            }else{
                x--;
                glReadPixels(x+halfWidth,y+halfWidth,1,1,GL_RGB,GL_UNSIGNED_BYTE,&iPixel);
            }
        }
    }
}
*/

void clearScene(){
    glClearColor(0.3, 0.6, 0.4, 1.0); // Set foreground color
    glColor3f(0.2, 0.3, 0.3); // Clear screen to background color.
    glClear(GL_COLOR_BUFFER_BIT);   //Flush created objects to the screen, i.e., force rendering.

}

// Drawing (display) routine.
void drawScene(void)
{
    glFlush();
    double rx, ry;
    switch (mode) {
        case 0:
            if( drawLine ){
                bresenham(xi, yi, xf, yf);
            }
            break;
        case 1:
            if( drawLine ){
                rx = fabs(xi-xf);
                ry = fabs(yi-yf);
                DrawEllipse(xi, yi, rx, ry, 50);
            }
            break;
        case 2:
            if( drawLine ){
                DrawCircle(sqrt(pow(xi-xf, 2) + pow(yi-yf, 2)), xi, yi);
            }
            break;
        case 3:
            //Fill(xi, yi, 50, 50, 50);
            firstClick = true;
            drawLine = true;
            break;
        case 4:
            break;
        case 100:
            firstClick = true;
            clearScene();
            break;
        default:
            break;
    }
    glFlush();
}
void rightBottonMenu(int value){
    switch (value) {
        case 0:case 1:
        case 2:case 3:
        case 4:
            mode = value;
            break;
        case 100:
            clearScene();
            glFlush();
            break;
        case -1:
            exit(0);
        default:
            break;
    }
}
void createGLUTMenus(){
    int menu;
    menu = glutCreateMenu(rightBottonMenu);
    glutAddMenuEntry("Line", 0);
    glutAddMenuEntry("Ellipse", 1);
    glutAddMenuEntry("Circle", 2);
    glutAddMenuEntry("Fill", 3);
    glutAddMenuEntry("Poly", 4);
    glutAddMenuEntry("Clear", 100);
    glutAddMenuEntry("Exit", -1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
// OpenGL window reshape routine.
void setup()
{
    glViewport(0, 0, ww, wh); // Set viewport size to be entire OpenGL window.
    glMatrixMode(GL_PROJECTION); // Set matrix mode to projection.
    glLoadIdentity(); // Clear current projection matrix to identity.
    gluOrtho2D(0.0, (GLdouble)ww, 0.0, (GLdouble)wh); // Specify the orthographic (or perpendicular) projection, i.e., define the viewing box.
    glMatrixMode(GL_MODELVIEW); // Set matrix mode to modelview.
}

// Main routine: defines window properties, creates window, registers callback routines and begins processing.
int main(int argc, char **argv)
{
    glutInit(&argc, argv); // Initialize GLUT.
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode as single-buffered and RGB color.
    glutInitWindowSize(ww, wh); // Set OpenGL window size.
    glutInitWindowPosition(100, 100); // Set position of OpenGL window upper-left corner.
    glutCreateWindow("CG Project"); // Create OpenGL window with title.
    clearScene();
    glutDisplayFunc(drawScene); // Register display routine.
    setup(); // Register reshape routine.
    glutKeyboardFunc(keyInput); // Register keyboard routine.
    glutMouseFunc(mouse); // Begin processing.
    createGLUTMenus();
    glutMainLoop();
    return 0;
}
#pragma clang diagnostic pop


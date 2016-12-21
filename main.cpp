//
//  main.cpp
//  GlutDemo
//
//  Created by Zhou.Y.C on 10/31/16.
//  Copyright © 2016 njucs. All rights reserved.
//
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <stack>
#include <GLUT/GLUT.h>
int ww = 600, wh = 600;
int xi, yi, xf, yf;
bool firstClick = true;

using namespace std;

GLubyte colorBlack[3] = {0, 0, 0};

void setPixel(int x, int y)
{
    glColor3f(colorBlack[0], colorBlack[1], colorBlack[2]); //Set pixel to black
    glBegin(GL_POINTS);
    glVertex2i(x, y); //Set pixel coordinates
    glEnd();
}
//画点
void glPoint(int x,int y,int r,int g,int b){
    glColor3ub (r,g,b);
    glPointSize(1);
    glBegin(GL_POINTS);
    glVertex2i(x,y);
    glEnd();
    glFlush();
}

//Draw line if X distance is greater than Y
void bresenhamX(int x0, int y0, int x1, int y1, int dx, int dy)
{
    int i, j, k;
    
    i = 2 * dy - dx;
    j = 2 * dy;
    k = 2 * (dy - dx);
    if (!(x0 < x1)) {
        swap(x0, x1);
        swap(y0, y1);
    }
    setPixel(x0, y0);
    while (x0 < x1) {
        if (i < 0)
            i += j;
        else {
            if (y0 < y1)
                ++y0;
            else
                --y0;
            i += k;
        }
        ++x0;
        setPixel(x0, y0);
    }
}

//Draw line if X distance is lesser than Y
void bresenhamY(int x0, int y0, int x1, int y1, int dx, int dy)
{
    int i, j, k;
    
    i = 2 * dx - dy;
    j = 2 * dx;
    k = 2 * (dx - dy);
    if (!(y0 < y1)) {
        swap(x0, x1);
        swap(y0, y1);
    }
    setPixel(x0, y0);
    while (y0 < y1) {
        if (i < 0)
            i += j;
        else {
            if (x0 > x1)
                --x0;
            else
                ++x0;
            i += k;
        }
        ++y0;
        setPixel(x0, y0);
    }
}

//Called by mouse(), will call the appropriate function depending on the length of the X and Y axis
void bresenham(int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    
    if (dx >= dy)
        bresenhamX(x0, y0, x1, y1, dx, dy);
    else
        bresenhamY(x0, y0, x1, y1, dx, dy);
}

//Calls Bresenham function when the mouse has traced a line
bool drawLine = false;
int mode = 0;
void mouse(int btn, int state, int x, int y)
{
    if( btn == GLUT_LEFT_BUTTON && state == GLUT_UP ) {
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

void DrawEllipse(float cx, float cy, float rx, float ry, int num_segments)
{
    float theta = 2 * 3.1415926 / float(num_segments);
    float c = cosf(theta);//precalculate the sine and cosine
    float s = sinf(theta);
    float t;
    
    float x = 1;//we start at angle = 0
    float y = 0;
    
    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii < num_segments; ii++)
        {
            //apply radius and offset
            glVertex2f(x * rx + cx, y * ry + cy);//output vertex
        
            //apply the rotation matrix
            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
        } 
    glEnd(); 
}

void pnarcArc(float radius, float centerX, float centerY,int area){
    float x,y,f;
    int tag[4]={1,1,-1,-1};
    int tagX[4]={1,-1,-1,1};
    int tagY[4]={-1,-1,1,1};
    x=0;y=tag[area-1]*radius;f=0;
    while(tag[area-1]*y>0){
        setPixel(x+centerX,y+centerY);
        if(f>0){
            f=f+tagY[area-1]*2*y+1;
            y=y+tagY[area-1];
        }else{
            f=f+tagX[area-1]*2*x+1;
            x=x+tagX[area-1];
        }
    }
    if(y==centerY){
        setPixel(x,y);
    }
}

void DrawCircle(float radius, float x, float y){
    for (int i =1; i<=4; i++){
        pnarcArc(radius, x, y, i);
    }
}

struct Point{
    int x;
    int y;
};
//种子填充算法

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

void clearScene(){
    glClearColor(0.3, 0.6, 0.4, 1.0); // Set foreground color
    glColor3f(0.2, 0.3, 0.3); // Clear screen to background color.
    glClear(GL_COLOR_BUFFER_BIT);   //Flush created objects to the screen, i.e., force rendering.

}
// Drawing (display) routine.
void drawScene(void)
{
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
            Fill(xi, yi, 50, 50, 50);
            firstClick = true;
            drawLine = true;
            break;
        case 100:
            clearScene();
            break;
        default:
            break;
    }
    glFlush();
}
void rightBottonMenu(int value){
    switch (value) {
        case 0:
            mode = 0;
            break;
        case 1:
            mode = 1;
            break;
        case 2:
            mode = 2;
            break;
        case 3:
            mode =3;
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
    glutDisplayFunc(drawScene); // Register display routine.
    setup(); // Register reshape routine.
    glutKeyboardFunc(keyInput); // Register keyboard routine.
    glutMouseFunc(mouse); // Begin processing.
    createGLUTMenus();
    glutMainLoop();
    return 0;
}
#pragma clang diagnostic pop


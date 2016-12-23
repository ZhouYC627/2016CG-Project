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
#include "Graph.h"
#include "PatternGeneration.h"
//#include "PolyScan.h"


int ww = 600, wh = 600;
int xi, yi, xf, yf;
int dragX, drayY;
bool firstClick = true;
vector<Point> polyPoints;
vector<Graph*> graphs;
int editObject, editBeginX, editBeginY;
double lastAngle;

using namespace std;


//Calls Bresenham function when the mouse has traced a line
bool drawLine = false;
int mode = 0;

void clearScene(){
    glClearColor(0.3, 0.6, 0.4, 1.0); // Set foreground color
    glColor3f(0.2, 0.3, 0.3); // Clear screen to background color.
    glClear(GL_COLOR_BUFFER_BIT);   //Flush created objects to the screen, i.e., force rendering.
}


void addGraph(){
    switch (mode) {
        case LINE:{
            Line *line = new Line(xi, yi, xf, yf);
            graphs.push_back(line);
            break;
        }
        case CIRCLE:{
            Circle *c = new Circle(sqrt(pow(xi-xf, 2) + pow(yi-yf, 2)), xi, yi);
            graphs.push_back(c);
            break;
        }
        case ELLIPSE:{
            Ellipse *e = new Ellipse(xi, yi, abs(xi-xf), abs(yi-yf));
            graphs.push_back(e);
            break;
        }
        default:
            break;
    }

}
void mouse(int btn, int state, int x, int y)
{
    Point now;
    now.x = x;
    now.y = wh - y;
    if( btn == GLUT_LEFT_BUTTON && state == GLUT_UP ) {
        //simple pattern: 2 clicks
        if ((mode==LINE) ||(mode==ELLIPSE) || (mode==CIRCLE)){
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
            if (drawLine){
                addGraph();
            }
        //more than 2 clicks
        }else if (mode == POLY){
            if (polyPoints.empty()){
                polyPoints.push_back(now);
            }else{
                Point lastPoint = polyPoints.back();
                if ((abs(now.x-polyPoints[0].x)+abs(now.y-polyPoints[0].y))<10){
                    cout<<"Draw Poly!"<<endl;
                    //PolyScan(&polyPoints[0], (int)polyPoints.size(), 0.5);
                    Poly *p = new Poly;
                    p->polypoints = polyPoints;
                    p->draw();
                    graphs.push_back(p);
                    polyPoints.clear();
                }else{
                    bresenham(lastPoint.x, lastPoint.y, now.x, now.y);
                    polyPoints.push_back(now);
                }
                glutPostRedisplay();
            }
            //polyPoints.push_back(Point{x, wh-y});
        }
        
    }else
    if( btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
        if (mode == DRAG || mode == ROTATE){
            editObject = -1;
            for (int i = 0; i<graphs.size();i++){
                if (graphs[i]->ptInGraph(now)){
                    editObject = i;
                    cout<<"editobj: "<<editObject<<endl;
                    editBeginX = now.x;
                    editBeginY = now.y;
                    lastAngle = 0;
                }
            }
        }
    }
}

void motion(int x, int y){
    Point now{x,wh -y};
    if (mode == DRAG && editObject>=0){
        //cout<<"drag "<<x-dragBeginX<<" "<<y-dragBeginY<<endl;
        clearScene();
        graphs[editObject]->move(now.x-editBeginX, now.y-editBeginY);
        editBeginX = now.x;
        editBeginY = now.y;
        glutPostRedisplay();
    }
    if (mode == ROTATE && editObject>=0){
        clearScene();
        double angle = atan2(x-editBeginX, y-editBeginY);
        cout<<angle<<"r ";
        graphs[editObject]->rotate(angle-lastAngle, editBeginX, editBeginY);
        lastAngle = angle;
        glutPostRedisplay();
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


void drawAllGraphs(){
    /*for(auto &g: graphs){
        cout<<g.gType<<endl;
        g.draw();
    }*/
    for (int i =0; i<graphs.size(); i++){
        graphs[i]->draw();
    }
}

// Drawing (display) routine.
void drawScene(void)
{
    glFlush();
    /* 
     double rx, ry;
     switch (mode) {
        case LINE:
            if( drawLine ){
                bresenham(xi, yi, xf, yf);
            }
            break;
        case ELLIPSE:
            if( drawLine ){
                rx = fabs(xi-xf);
                ry = fabs(yi-yf);
                DrawEllipse(xi, yi, rx, ry, 50);
            }
            break;
        case CIRCLE:
            if( drawLine ){
                DrawCircle(sqrt(pow(xi-xf, 2) + pow(yi-yf, 2)), xi, yi);
            }
            break;
        case FILL:
            //Fill(xi, yi, 50, 50, 50);
            firstClick = true;
            drawLine = true;
            break;
        case POLY:
            break;
        case DRAG:
            break;
        case CLEAR:
            firstClick = true;
            clearScene();
            break;
        default:
            break;
    }*/
    drawAllGraphs();
    switch (mode) {
        case CLEAR:
            
            //clearScene();
            break;
            
        default:
            break;
    }
    glFlush();
}
void rightBottonMenu(int value){
    switch (value) {
        case LINE:    case ELLIPSE:
        case CIRCLE:  case POLY:
        case FILL:    case DRAG:
        case ROTATE:
            mode = value;
            break;
        case CLEAR:
            cout<<"clear"<<endl;
            for (int i=0; i<graphs.size(); i++) {
                delete graphs[i];
            }
            graphs.clear();
            firstClick = true;
            clearScene();
            glFlush();
            break;
        case EXIT:
            exit(0);
        default:
            break;
    }
}
void createGLUTMenus(){
    int menu, submenu;
    submenu = glutCreateMenu(rightBottonMenu);
    glutAddMenuEntry("Drag", DRAG);
    glutAddMenuEntry("Rotate", ROTATE);
    menu = glutCreateMenu(rightBottonMenu);
    glutAddMenuEntry("Line", LINE);
    glutAddMenuEntry("Ellipse", ELLIPSE);
    glutAddMenuEntry("Circle", CIRCLE);
    glutAddMenuEntry("Poly", POLY);
    glutAddMenuEntry("Fill", FILL);
    glutAddSubMenu("Edit", submenu);
    glutAddMenuEntry("Clear", CLEAR);
    glutAddMenuEntry("Exit", EXIT);
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
    cout<<"Start!"<<endl;
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
    glutMotionFunc(motion);
    createGLUTMenus();
    glutMainLoop();
    return 0;
}
#pragma clang diagnostic pop


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
#include "PolyClip.h"
#include "3DGraph.h"
//#include "PolyScan.h"


//int WW = 600, WH = 600;
int xi, yi, xf, yf;
int dragX, drayY;
bool firstClick = true;
vector<Point> polyPoints;
vector<Graph*> graphs;
int editObject = -1, editBeginX, editBeginY;
int lastScaleX, lastScaleY;
double lastAngle;
Line *lt, *ll, *lr, *lb;

using namespace std;


//Calls Bresenham function when the mouse has traced a line
bool drawLine = false;
int mode = 0;

void clearScene(){
    glClearColor(0.3, 0.6, 0.4, 1.0); // Set foreground color
    //glClearColor(1, 1, 1, 1);
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
    if (mode==TD){
        ThreeDmouse(btn, state, x, y);
        return;
    }
    
    Point now;
    now.x = x;
    now.y = WH - y;
    cout<<x<<","<<y<<endl;
    if( btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
        //simple pattern: 2 clicks
        if (firstClick) {
            xi = x;
            yi = (WH - y);
            firstClick = false;
            drawLine = false;
        }
        else {
            xf = x;
            yf = (WH - y);
            firstClick = true;
            drawLine = true;
        }
        if (drawLine){
            addGraph();
        }
        
        //more than 2 clicks
        if (mode == POLY || mode ==CURVE){
            if (polyPoints.empty()){
                polyPoints.push_back(now);
            }else{
                Point lastPoint = polyPoints.back();
                if ((abs(now.x-polyPoints[0].x)+abs(now.y-polyPoints[0].y))<10){
                    if (mode==POLY){
                        cout<<"Draw Poly!"<<endl;
                        //PolyScan(&polyPoints[0], (int)polyPoints.size(), 0.5);
                        Poly *p = new Poly;
                        p->gType = GPOLY;
                        p->polypoints = polyPoints;
                        p->draw();
                        graphs.push_back(p);
                    }
                    if(mode==CURVE){
                        Curve *c = new Curve;
                        c->gType = GCURVE;
                        c->curvePoints = polyPoints;
                        c->draw();
                        graphs.push_back(c);
                    }
                    polyPoints.clear();
                }else{
                    Line l(lastPoint.x, lastPoint.y, now.x, now.y);
                    //bresenham(xi.x, lastPoint.y, now.x, now.y);
                    l.draw();
                    polyPoints.push_back(now);
                    firstClick = true;
                }
            }
            //polyPoints.push_back(Point{x, wh-y});
        }
        if (mode == DRAG || mode == ROTATE || mode == ZOOM){
            editObject = -1;
            for (int i = 0; i<graphs.size();i++){
                if (graphs[i]->ptInGraph(now)){
                    editObject = i;
                    cout<<"editobj: "<<editObject<<endl;
                    editBeginX = now.x;
                    editBeginY = now.y;
                    lastAngle = atan2(now.x-WW/2, now.y-WH/2);
                    //lastScaleX = 1;
                    //lastScaleY = 1;
                    firstClick = false;
                }
            }
        }
        if (mode == CLIP){
            lt = new Line(xi, yi, now.x, yi);
            ll = new Line(xi, yi, xi, now.y);
            lr = new Line(now.x, yi, now.x, now.y);
            lb = new Line(xi, now.y, now.x, now.y);
            graphs.push_back(lt);
            graphs.push_back(ll);
            graphs.push_back(lr);
            graphs.push_back(lb);
        }
    }else if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP){
        if (mode==CLIP) {
            for (int i = 0; i<4; i++) {
                graphs.pop_back();
            }
            firstClick = true;
            Rect clipRect;
            clipRect.xmin = min(xi, now.x);
            clipRect.xmax = max(xi, now.x);
            clipRect.ymin = min(yi, now.y);
            clipRect.ymax = max(yi, now.y);
            for (int i = 0; i<graphs.size(); i++) {
                if (graphs[i]->gType ==GPOLY){
                    Poly *p = (Poly*)graphs[i];
                    int n = 0;
                    Point *temp = SutherlangHodgmanPolyClip(&p->polypoints[0], (int)p->polypoints.size(), clipRect, n);
                    if (n>0){
                        p->rebuild(temp, n);
                    }
                }
            }
            clearScene();
        }
    }
    if (btn == GLUT_RIGHT_BUTTON){
        cout<<"RIGHT button!"<<endl;
    }
    glutPostRedisplay();
}
void selectObject(Point now){
    editObject = -1;
    for (int i = 0; i<graphs.size();i++){
        if (graphs[i]->ptInGraph(now) && graphs[i]->gType == GPOLY){
            editObject = i;
            cout<<"editobj: "<<editObject<<endl;
            break;
        }
    }
}
int motionCount = 0;
void motion(int x, int y){
    if (++motionCount%5 != 0) {
        return;
    }
    if (mode==TD){
        ThreeDMotion(x, y);
        return;
    }
    Point now{x,WH - y};
    if (mode == DRAG && editObject>=0){
        //cout<<"drag "<<x-dragBeginX<<" "<<y-dragBeginY<<endl;
        clearScene();
        graphs[editObject]->move(now.x-editBeginX, now.y-editBeginY);
        editBeginX = now.x;
        editBeginY = now.y;
    }
    if (mode == ROTATE && editObject>=0){
        clearScene();
        double angle = atan2(x-WW/2, y-WH/2);
        graphs[editObject]->rotate(angle-lastAngle, WW/2, WH/2);
        lastAngle = angle;
    }
    if (mode == ZOOM && editObject>=0 && !firstClick){
        clearScene();
        double sx = (double)(now.x-WW/2)/(double)(xf-WW/2);
        double sy = (double)(now.y-WH/2)/(double)(yf-WH/2);
        if (sx==0 || sy == 0){
            return;
        }
        printf("zoom: %f,%f\n", sx, sy);
        graphs[editObject]->zoom(sx, sy);
        xf = now.x;
        yf = now.y;
        //firstClick = true;
    }
    if (mode==CLIP){
        clearScene();
        lt->end.x = now.x;
        ll->end.y = now.y;
        lr->begin.x = now.x;
        lb->begin.y = now.y;
        lr->end.x = now.x;
        lr->end.y = now.y;
        lb->end.x = now.x;
        lb->end.y = now.y;
    }
    glutPostRedisplay();
    
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

void displayCoordinates(){
    //x轴
    glBegin(GL_LINES);
    glVertex2f(-WW, WH/2);
    glVertex2f(WW, WH/2);
    glEnd();
    
    //y轴
    glBegin(GL_LINES);
    glVertex2f(WW/2, -WH);
    glVertex2f(WW/2, WH);
    glEnd();
    glFlush();
}

void drawAllGraphs(){
    for (int i =0; i<graphs.size(); i++){
        graphs[i]->draw();
    }
}

// Drawing (display) routine.
void drawScene(void)
{
    if (mode == TD){
        ThreeDRedraw();
        glFlush();
        return;
    }
    //glFlush();
    displayCoordinates();
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
    firstClick = true;
    editObject = -1;
    switch (value) {
        case LINE:    case ELLIPSE:
        case CIRCLE:  case POLY:
        case FILL:    case DRAG:
        case ROTATE:  case ZOOM:
        case CLIP:    case CURVE:
            mode = value;
            break;
        case TD:
            mode = TD;
            clearScene();
            ThreeDInit();
            cout<<"3D"<<endl;
            break;
        case CLEAR:
            cout<<"clear"<<endl;
            for (int i=0; i<graphs.size(); i++) {
                delete graphs[i];
            }
            graphs.clear();
            clearScene();
            displayCoordinates();
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
    glutAddMenuEntry("Zoom", ZOOM);
    glutAddMenuEntry("Clip", CLIP);
    menu = glutCreateMenu(rightBottonMenu);
    glutAddMenuEntry("Line", LINE);
    glutAddMenuEntry("Curve", CURVE);
    glutAddMenuEntry("Ellipse", ELLIPSE);
    glutAddMenuEntry("Circle", CIRCLE);
    glutAddMenuEntry("Poly", POLY);
    glutAddMenuEntry("Fill", FILL);
    glutAddMenuEntry("3D Grapy", TD);
    glutAddSubMenu("Edit", submenu);
    glutAddMenuEntry("Clear", CLEAR);
    glutAddMenuEntry("Exit", EXIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
// OpenGL window reshape routine.
void setup()
{
    glViewport(0, 0, WW, WH); // Set viewport size to be entire OpenGL window.
    glMatrixMode(GL_PROJECTION); // Set matrix mode to projection.
    glLoadIdentity(); // Clear current projection matrix to identity.
    gluOrtho2D(0.0, (GLdouble)WW, 0.0, (GLdouble)WH); // Specify the orthographic (or perpendicular) projection, i.e., define the viewing box.
    glMatrixMode(GL_MODELVIEW); // Set matrix mode to modelview.
}

// Main routine: defines window properties, creates window, registers callback routines and begins processing.
static char *filename = "Table-141.off";

int main(int argc, char **argv)
{
    cout<<"Start!"<<endl;
    if(!ReadOffFile(filename))
        exit(1);
    glutInit(&argc, argv); // Initialize GLUT.
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode as double-buffered and RGB color.
    //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // | GLUT_STENCIL
    glutInitWindowSize(WW, WH); // Set OpenGL window size.
    glutInitWindowPosition(100, 100); // Set position of OpenGL window upper-left corner.
    glutCreateWindow("CG Project"); // Create OpenGL window with title.
    clearScene();
    glutDisplayFunc(drawScene); // Register display routine.
    setup(); // Register reshape routine.
    glutKeyboardFunc(keyInput); // Register keyboard routine.
    glutMouseFunc(mouse); // Begin processing.
    glutMotionFunc(motion);
    glutIdleFunc(0);
    //ThreeDInit();
    createGLUTMenus();
    glutMainLoop();
    return 0;
}
#pragma clang diagnostic pop


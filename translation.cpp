//
//  translation.cpp
//  GlutDemo
//
//  Created by Zhou.Y.C on 12/23/16.
//  Copyright © 2016 njucs. All rights reserved.
//

#include "common.h"
#include "translation.h"
#include "PolyScan.h"

void Translation(Point polypoints[], int num_line, double k0, int tx, int ty){
    for(int i = 0; i < num_line; i++){
        polypoints[i].x += tx;
        polypoints[i].y += ty;
        //cout<<"trans poly: "<<polypoints[i].x<<","<<polypoints[i].y<<endl;
    }
    cout<<endl;
}


void Rotate(Point polypoints_t[], int num_line, double angle, int tx, int ty){
    
    for(int i = 0; i < num_line; i++){
        polypoints_t[i].x -= tx;
        polypoints_t[i].y -= ty;
    }
    
    Point polypoints[num_line];
    
    for(int i = 0; i < num_line; i++){
        polypoints[i].x = polypoints_t[i].x*cos(angle) - polypoints_t[i].y*sin(angle);
        polypoints[i].y = polypoints_t[i].x*sin(angle) + polypoints_t[i].y*cos(angle);
    }
    
    for(int i = 0; i < num_line; i++){
        polypoints[i].x += tx;
        polypoints[i].y += ty;
    }
    
    
    for(int i = 1; i < num_line; i++){
        cout<<polypoints[i].x<<" "<<polypoints[i].y<<endl;
    }
}
/*
void Zoom(string file, double scale_x, double scale_y){
    ifstream in_file(file, ios::in);
    if(!in_file){
        cout<<"Something wrong when opening the file!"<<endl;
        exit(-1);
    }
    
    int num_line = 0;
    in_file>>num_line;
    
    Point polypoints[num_line];
    for(int i = 0; i < num_line; i++){
        in_file>>polypoints[i].x;
        in_file>>polypoints[i].y;
    }
    
    for(int i = 0; i < num_line; i++){
        polypoints[i].x *= scale_x;
        polypoints[i].y *= scale_y;
    }
    
    int ymax = polypoints[0].y, ymin = polypoints[0].y;
    for(int i = 1; i < num_line; i++){
        if(ymax < polypoints[i].y)
            ymax = polypoints[i].y;
        if(ymin > polypoints[i].y)
            ymin = polypoints[i].y;
    }
    
    vector<list<AET>> Net(ymax - ymin + 1);
    
    InitPolyScan(polypoints, Net, num_line, ymax, ymin);
    HorizonEdgeFill(polypoints, num_line);
    ProcessPolyScan(Net, ymax, ymin);
}

void Reflect(string file, double a, double b, double c){
    ifstream in_file(file, ios::in);
    if(!in_file){
        cout<<"Something wrong when opening the file!"<<endl;
        exit(-1);
    }
    
    int num_line = 0;
    in_file>>num_line;
    
    Point polypoints[num_line];
    for(int i = 0; i < num_line; i++){
        in_file>>polypoints[i].x;
        in_file>>polypoints[i].y;
    }
    
    for(int i = 0; i < num_line; i++){
        int x = polypoints[i].x, y = polypoints[i].y;
        double t = (a*x + b*y +c) / (a*a + b*b);
        polypoints[i].x = x - 2*a*t;
        polypoints[i].y = y - 2*b*t;
    }
    
    int ymax = polypoints[0].y, ymin = polypoints[0].y;
    for(int i = 1; i < num_line; i++){
        if(ymax < polypoints[i].y)
            ymax = polypoints[i].y;
        if(ymin > polypoints[i].y)
            ymin = polypoints[i].y;
    }
    
    vector<list<AET>> Net(ymax - ymin + 1);
    
    InitPolyScan(polypoints, Net, num_line, ymax, ymin);
    HorizonEdgeFill(polypoints, num_line);
    ProcessPolyScan(Net, ymax, ymin);
}

void ShearFilter(string file, double b, double d){
    ifstream in_file(file, ios::in);
    if(!in_file){
        cout<<"Something wrong when opening the file!"<<endl;
        exit(-1);
    }
    
    int num_line = 0;
    in_file>>num_line;
    
    Point polypoints[num_line];
    for(int i = 0; i < num_line; i++){
        in_file>>polypoints[i].x;
        in_file>>polypoints[i].y;
    }
    
    for(int i = 0; i < num_line; i++){
        int x = polypoints[i].x, y = polypoints[i].y;
        polypoints[i].x = x + b*y;
        polypoints[i].y = d*x + y;
    }
    
    int ymax = polypoints[0].y, ymin = polypoints[0].y;
    for(int i = 1; i < num_line; i++){
        if(ymax < polypoints[i].y)
            ymax = polypoints[i].y;
        if(ymin > polypoints[i].y)
            ymin = polypoints[i].y;
    }
    
    vector<list<AET>> Net(ymax - ymin + 1);
    
    InitPolyScan(polypoints, Net, num_line, ymax, ymin);
    HorizonEdgeFill(polypoints, num_line);
    ProcessPolyScan(Net, ymax, ymin);
}
*/

/*
void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    
    //x轴
    glBegin(GL_LINES);
    glColor3f (0, 0, 0);
    glVertex2f(-150, 0);
    glVertex2f(150, 0);
    glEnd();
    
    //y轴
    glBegin(GL_LINES);
    glColor3f (0, 0, 0);
    glVertex2f(0, -150);
    glVertex2f(0, 150);
    glEnd();
    
    glColor3f(0, 0, 0.5);
    
    string file = "/Users/mac/Desktop/5.txt";
    //PolyScan(file);
    //Translate(file, 20, 20);
    //Rotate(file, 3, 0, 0);
    //Zoom(file, 0.5, 1);
    //Reflect(file, 1, -1, 0);
    //ShearFilter(file, 0, 0.5);
    glFlush();
}


int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(500, 250);
    glutCreateWindow(" ");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(ChangeSize);
    glutMainLoop();
    return 0;
}
*/


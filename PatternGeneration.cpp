//
//  PatternGeneration.cpp
//  GlutDemo
//
//  Created by Zhou.Y.C on 12/21/16.
//  Copyright © 2016 njucs. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <algorithm>
#include <GLUT/GLUT.h>

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

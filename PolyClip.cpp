//
//  PolyClip.cpp
//  CGProject
//
//  Created by Zhou.Y.C on 12/30/16.
//  Copyright © 2016 njucs. All rights reserved.
//

#include "common.h"
#include "PolyClip.h"



bool Inside(Rect rect, Point p, int b){
    switch (b) {
        case 0:
            if(p.x < rect.xmin) return false;
            break;
        case 1:
            if(p.x > rect.xmax) return false;
            break;
        case 2:
            if(p.y < rect.ymin) return false;
            break;
        case 3:
            if(p.y > rect.ymax) return false;
            break;
    }
    return true;
}

bool Cross(Rect rect, Point p1, Point p2, int b){
    if(Inside(rect, p1, b) == Inside(rect, p2, b))
        return false;
    else
        return true;
}

Point Intersect(Rect rect, Point p1, Point p2, int b){
    Point pi;
    double m = 0.0;
    if(p1.x != p2.x)
        m = (double)(p2.y-p1.y)/(double)(p2.x-p1.x);
    switch (b) {
        case 0:
            pi.x = rect.xmin;
            pi.y = p2.y + (rect.xmin - p2.x)*m;
            break;
        case 1:
            pi.x = rect.xmax;
            pi.y = p2.y + (rect.xmax - p2.x)*m;
            break;
        case 2:
            pi.y = rect.ymin;
            if(p1.x != p2.x)
                pi.x = p2.x + (rect.ymin - p2.y)/m;
            else
                pi.x = p2.x;
            break;
        case 3:
            pi.y = rect.ymax;
            if(p1.x != p2.x)
                pi.x = p2.x + (rect.ymax - p2.y)/m;
            else
                pi.x = p2.x;
            break;
    }
    return pi;
}

int edgeClip(int b, Rect rect, Point *pIn, int cnt, Point *pOut){
    Point t;
    int Outcnt = 0;
    t = pIn[0];
    for(int i = 1; i <= cnt; i++){
        if(!Inside(rect, t, b) && Inside(rect, pIn[i], b)){
            pOut[Outcnt] = Intersect(rect, t, pIn[i], b);
            Outcnt++;
            pOut[Outcnt] = pIn[i];
            Outcnt++;
        }
        else if(Inside(rect, t, b) && Inside(rect, pIn[i], b)){
            pOut[Outcnt] = pIn[i];
            Outcnt++;
        }
        else if(Inside(rect, t, b) && !Inside(rect, pIn[i], b)){
            pOut[Outcnt] = Intersect(rect, t, pIn[i], b);
            Outcnt++;
        }
        t = pIn[i];
    }
    return Outcnt;
}
Point* SutherlangHodgmanPolyClip(Point polypoints[], int num_line, Rect rect, int &cnt){

    bool isCross = false;
    for(int b = 0; b < 4; b++){
        for (int i = 0; i<num_line; i++) {
            Point p1 = polypoints[i];
            Point p2 = polypoints[(i+1)%num_line];
            if (Cross(rect, p1, p2, b)){
                isCross = true;
                break;
            }
        }
    }
    if (!isCross){
        cnt = 0;
        cout<<"not cross"<<endl;
        return polypoints;
    }
    
    int ymax = polypoints[0].y, ymin = polypoints[0].y;
    for(int i = 1; i < num_line; i++){
        if(ymax < polypoints[i].y)
            ymax = polypoints[i].y;
        if(ymin > polypoints[i].y)
            ymin = polypoints[i].y;
    }
    
    cnt = num_line;
    Point pOut[4*cnt], pIn[4*cnt];
    for(int i = 0; i < 4*cnt; i++){
        pIn[i].x = 0;
        pIn[i].y = 0;
        pOut[i].x = 0;
        pOut[i].y = 0;
    }
    for(int i = 0; i < cnt ; i++)
        pIn[i] = polypoints[i];
    pIn[cnt] = polypoints[0];
    
    /*
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < cnt; i++)
        glVertex2f(pIn[i].x, pIn[i].y);
    glEnd();
    */
    
    int Outcnt;
    for(int b = 0; b < 4; b++){
        Outcnt = edgeClip(b, rect, pIn, cnt, pOut);
        for(int i = 0; i < Outcnt; i++)
            pIn[i] = pOut[i];
        pIn[Outcnt] = pOut[0];
        cnt = Outcnt;
    }
    Point *temp = new Point[cnt];
    for (int i = 0; i<cnt; i++) {
        temp[i] = pOut[i];
    }
    return temp;
    //PolyScan(pOut, cnt);
}

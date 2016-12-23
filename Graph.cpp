//
//  Graph.cpp
//  GlutDemo
//
//  Created by Zhou.Y.C on 12/23/16.
//  Copyright © 2016 njucs. All rights reserved.
//


#include "Graph.h"
#include "translation.h"
#include "PatternGeneration.h"
#include "PolyScan.h"

bool Graph::ptInGraph(Point p){
    return false;
}
Graph::~Graph(){
    
}
void Graph::move(int x, int y){
}
void Graph::rotate(double angle, int x, int y){
}

bool Poly::ptInGraph(Point p){
    int nCross = 0;
    int n = (int)polypoints.size();
    for (int i = 0; i < n; i++) {
        Point p1 = polypoints[i];
        Point p2 = polypoints[(i + 1) % n];
        // 求解 y=p.y 与 p1 p2 的交点
        // p1p2 与 y=p0.y平行
        if (p1.y == p2.y)
            continue;
        // 交点在p1p2延长线上
        if (p.y < min(p1.y, p2.y))
            continue;
        // 交点在p1p2延长线上
        if (p.y >= max(p1.y, p2.y))
            continue;
        // 求交点的 X 坐标
        double x = (double) (p.y - p1.y) * (double) (p2.x - p1.x)
        / (double) (p2.y - p1.y) + p1.x;
        // 只统计单边交点
        if (x > p.x)
            nCross++;
    }
    cout<<"pt in poly: "<< nCross<<endl;
    return (nCross % 2 == 1);
}

void Poly::move(int x, int y){
    Translation(&polypoints[0], (int)polypoints.size(), 0.5, x, y);
}

void Poly::draw(){
    PolyScan(&polypoints[0], (int)polypoints.size(), 0.5);
}
void Poly::rotate(double angle, int x, int y){
    Rotate(&polypoints[0], (int)polypoints.size(), angle, x, y);
}

bool Line::ptInGraph(Point p){
    int db = sqrt(pow(begin.x-p.x, 2) + pow(begin.y-p.y, 2));
    int de = sqrt(pow(end.x-p.x, 2) + pow(end.y-p.y, 2));
    int length = sqrt(pow(begin.x-end.x, 2) + pow(begin.y-end.y, 2));
    //cout<<"ptInLine:"<<abs(db+de-length)<<endl;
    return abs(db+de-length)<2;
}
void Line::draw(){
    bresenham(begin.x, begin.y, end.x, end.y);
    printf("Line: %d,%d  %d,%d", begin.x, begin.y, end.x, end.y);
}

Line::Line(int x1, int y1, int x2, int y2){
    gType = LINE;
    begin.x = x1;
    begin.y = y1;
    end.x = x2;
    end.y = y2;
}
void Line::move(int x, int y){
    begin.x +=x;
    begin.y += y;
    end.x +=x;
    end.y += y;
}

bool Circle::ptInGraph(Point p){
    double d = sqrt(pow(p.x - ctrPoint.x, 2) + pow(p.y - ctrPoint.y, 2));
    return fabs(d-radius)<3;
}

void Circle::draw(){
    DrawCircle(radius, ctrPoint.x, ctrPoint.y);
}
void Circle::move(int x, int y){
    ctrPoint.x += x;
    ctrPoint.y += y;
}
Circle::Circle(int r, int x, int y){
    radius = r;
    ctrPoint.x = x;
    ctrPoint.y = y;
}


Ellipse::Ellipse(int x, int y, int rx, int ry){
    ctrPoint.x = x;
    ctrPoint.y = y;
    this->rx = rx;
    this->ry = ry;
}

void Ellipse::move(int x, int y){
    ctrPoint.x += x;
    ctrPoint.y += y;
}

void Ellipse::draw(){
    DrawEllipse(ctrPoint.x, ctrPoint.y, rx, ry, 50);
}
bool Ellipse::ptInGraph(Point p){
    double f = pow(p.x-ctrPoint.x, 2)/pow(rx, 2) + pow(p.y-ctrPoint.y, 2)/pow(ry, 2);
    cout<<"ptinell: "<<f<<endl;
    return fabs(f-1)<0.1;
}





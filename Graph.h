//
//  Graph.h
//  GlutDemo
//
//  Created by Zhou.Y.C on 12/23/16.
//  Copyright © 2016 njucs. All rights reserved.
//

#ifndef Graph_h
#define Graph_h

#include "common.h"
enum GRAPH {GLINE, GCIRCLE, GPOLY};
class Graph{
public:
    int gType;
    virtual bool ptInGraph(Point);
    virtual void move(int , int);
    virtual void draw()=0;
    virtual void rotate(double angle, int x, int y);
    virtual void zoom(double, double)=0;
    virtual ~Graph();
};

class Poly: public Graph{
private:
public:
    vector<Point> polypoints;
    bool ptInGraph(Point);
    void move(int , int);
    void draw();
    void zoom(double, double);
    void rotate(double angle, int x, int y);
};

class Line: public Graph{
public:
    Point begin;
    Point end;
    Line(int x1, int y1, int x2, int y2);
    void draw();
    void move(int , int);
    void zoom(double, double);
    bool ptInGraph(Point);
};

class Circle: public Graph{
public:
    Point ctrPoint;
    int radius;
    Circle(int r, int x, int y);
    void draw();
    void move(int , int);
    void zoom(double, double);
    bool ptInGraph(Point);
};

class Ellipse: public Graph{
public:
    Point ctrPoint;
    int rx, ry;
    Ellipse(int x, int y, int rx, int ry);
    void draw();
    void move(int , int);
    void zoom(double, double);
    bool ptInGraph(Point);
};
#endif /* Graph_h */

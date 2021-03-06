//
//  common.h
//  GlutDemo
//
//  Created by Zhou.Y.C on 12/21/16.
//  Copyright © 2016 njucs. All rights reserved.
//

#ifndef common_h
#define common_h

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>
#include <stack>
#include <math.h>
#include <GLUT/GLUT.h>

#define WW 600
#define WH 600
using namespace std;

struct AET{
    double x;
    double dx;
    int bmax;
};

struct Point{
    int x;
    int y;
    int b;
};

struct Rect{
    int xmin;
    int xmax;
    int ymin;
    int ymax;
};

void setPixel(int x, int y);

enum MODE {LINE = 0, CURVE, ELLIPSE, CIRCLE, POLY, FILL, TD, TABLE, VASE, CHAIR, HORSE, DRAG, ROTATE, ZOOM, CLIP, CLEAR, EXIT};

#endif /* common_h */

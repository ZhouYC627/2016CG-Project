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
void setPixel(int x, int y);

enum MODE {LINE = 0, ELLIPSE, CIRCLE, POLY, FILL, DRAG, ROTATE, CLEAR, EXIT};

#endif /* common_h */

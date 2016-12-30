//
//  PolyClip.h
//  CGProject
//
//  Created by Zhou.Y.C on 12/30/16.
//  Copyright © 2016 njucs. All rights reserved.
//

#ifndef PolyClip_h
#define PolyClip_h

#include <stdio.h>
#include "common.h"
Point* SutherlangHodgmanPolyClip(Point polypoints[], int num_line, Rect rect, int &cnt);
#endif /* PolyClip_h */

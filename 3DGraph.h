//
//  3DGraph.h
//  CGProject
//
//  Created by Zhou.Y.C on 12/31/16.
//  Copyright © 2016 njucs. All rights reserved.
//

#ifndef _DGraph_h
#define _DGraph_h

bool ReadOffFile(const char *filename);
void ThreeDmouse(int button, int state, int x, int y);
void ThreeDMotion(int x, int y);
void ThreeDRedraw();
void ThreeDInit();

#endif /* _DGraph_h */

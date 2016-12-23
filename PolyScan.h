//
//  PolyScan.h
//  GlutDemo
//
//  Created by Zhou.Y.C on 12/21/16.
//  Copyright © 2016 njucs. All rights reserved.
//

#ifndef PolyScan_h
#define PolyScan_h

void PolyScan(Point polypoints[], int num_line, double k0);
void HorizonEdgeFill(Point polypoints[], double k0, int num_line);
void InitPolyScan(Point polypoints[], vector<list<AET>> & Net, int num_line, int ymax, int ymin);
void ProcessPolyScan(vector<list<AET>> &Net, double k0, int ymax, int ymin);

#endif /* PolyScan_h */

//
//  PolyScan.cpp
//  GlutDemo
//
//  Created by Zhou.Y.C on 12/21/16.
//  Copyright © 2016 njucs. All rights reserved.
//

#include "common.h"

struct AET{
    double x;
    double dx;
    int bmax;
};



void set_pixel(int x, int y);

void HorizonEdgeFill(Point polypoints[], double k0, int num_line){
    for(int i = 0; i < num_line; i++){
        if(polypoints[i].b == polypoints[(i+1)%num_line].b)
            for(double x = polypoints[i].x; x < polypoints[(i+1)%num_line].x; x++)
                set_pixel(x, k0*x + polypoints[i].b);
    }
}

void InitPolyScan(Point polypoints[], vector<list<AET>> & Net, int num_line, int ymax, int ymin){
    AET e;
    for(int i = 0; i < num_line; i++){
        Point& ps = polypoints[i];
        Point& pe = polypoints[(i+1) % num_line];
        Point& pss = polypoints[(i-1 + num_line) % num_line];
        Point& pee = polypoints[(i+2) % num_line];
        
        if(pe.b != ps.b){
            e.dx = (double)(pe.x - ps.x) / (double)(pe.b - ps.b);
            if(pe.b > ps.b){
                e.x = ps.x;
                if(pee.b >= pe.b)
                    e.bmax = pe.b - 1;
                else
                    e.bmax = pe.b;
                Net[ps.b-ymin].push_front(e);
            }
            else{
                e.x = pe.x;
                if(pss.b >= ps.b){
                    e.bmax = ps.b - 1;
                }
                else
                    e.bmax = ps.b;
                Net[pe.b-ymin].push_front(e);
            }
        }
    }
}

bool IsEdgeOutOfActive(AET e, int y){
    return e.bmax == y;
}

void UpdateAetEdgeInfo(AET& e){
    e.x += e.dx;
}

bool EdgeXiComparator(AET& e1, AET& e2){
    return e1.x < e2.x;
}

void InsertEachNetListToAet(AET& e, list<AET> &aet){
    aet.push_back(e);
}

void InsertNetListToAet(list<AET> &Net_y, list<AET> &aet){
    list<AET>::iterator it1 = Net_y.begin();
    for(; it1 != Net_y.end(); it1++){
        InsertEachNetListToAet(*it1, aet);
    }
    //for_each(Net[y-ymin].begin(), Net[y-ymin].end(), bind2nd(ptr_fun(InsertEachNetListToAet), aet));
    aet.sort(EdgeXiComparator);
}

void FillAetScanLine(list<AET> &aet, int y, double k0){
    list<AET>::iterator it = aet.begin();
    for(int i = 0; i < aet.size(); i+=2){
        double x = (*it).x;
        it++;
        for(; x < (*(it)).x; x++){
            set_pixel(x, k0*x+y);
        }
        it++;
    }
}

void RemoveNonActiveEdgeFromAet(list<AET> &aet, int y){
    aet.remove_if(bind2nd(ptr_fun(IsEdgeOutOfActive), y));
}

void UpdateAndResortAet(list<AET> &aet){
    for_each(aet.begin(), aet.end(), UpdateAetEdgeInfo);
    aet.sort(EdgeXiComparator);
}

void ProcessPolyScan(vector<list<AET>> &Net, double k0, int ymax, int ymin){
    list<AET> aet;
    
    for(int y = ymin; y <= ymax; y++){
        InsertNetListToAet(Net[y-ymin], aet);
        FillAetScanLine(aet, y, k0);
        RemoveNonActiveEdgeFromAet(aet, y);
        UpdateAndResortAet(aet);
    }
}

void PolyScan(string file, double k0){
    /*
     ifstream in_file(file, ios::in);
     if(!in_file){
     cout<<"Something wrong when opening the file!"<<endl;
     exit(-1);
     }
     */
    
    int num_line = 0;
    //in_file>>num_line;
    
    Point polypoints[num_line];
    /*for(int i = 0; i < num_line; i++){
     in_file>>polypoints[i].x;
     in_file>>polypoints[i].y;
     }*/
    
    for(int i = 0; i < num_line; i++){
        polypoints[i].b = polypoints[i].y - k0 * polypoints[i].x;
    }
    
    int bmax = polypoints[0].b, bmin = polypoints[0].b;
    for(int i = 1; i < num_line; i++){
        if(bmax < polypoints[i].b)
            bmax = polypoints[i].b;
        if(bmin > polypoints[i].b)
            bmin = polypoints[i].b;
    }
    
    vector<list<AET>> Net(bmax - bmin + 1);
    
    InitPolyScan(polypoints, Net, num_line, bmax, bmin);
    HorizonEdgeFill(polypoints, k0, num_line);
    ProcessPolyScan(Net, k0, bmax, bmin);
}


void init(void){
    //设置背景颜色
    glClearColor(1, 1, 1, 1);
}

void set_pixel(int x, int y){
    glPointSize(2);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void ChangeSize(GLsizei w, GLsizei h){
    if(h == 0){
        h = 1;
    }
    //定义视口
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //定义正交视域体
    if(w<h)
        glOrtho(-150, 150, -150*h/w, 150*h/w, -150, 150);
    else
        glOrtho(-150*w/h, 150*w/h, -150, 150, -150, 150);
}

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
    
    PolyScan("/Users/mac/Desktop/3.txt", 0.5);
    
    glFlush();
}

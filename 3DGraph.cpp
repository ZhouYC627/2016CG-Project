//
//  3DGraph.cpp
//  CGProject
//
//  Created by Zhou.Y.C on 12/31/16.
//  Copyright © 2016 njucs. All rights reserved.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "common.h"
struct Vertex {
    float x, y, z;
};
void set_pixel(Point p){
    glPointSize(1);
    glBegin(GL_POINTS);
    //glColor3f(0, 0, 1);
    glVertex2d(p.x, p.y);
    glEnd();
}
Vertex *verts;
int nverts = 0;

struct Face {
    int nverts;
    Vertex **verts;
    float normal[3];
};
Face *faces;
int nfaces = 0;
int nedges = 0;


static int GLUTwindow = 0;
static int GLUTwindow_height = 800;
static int GLUTwindow_width = 800;
static int GLUTmouse[2] = { 0, 0 };
static int GLUTbutton[3] = { 0, 0, 0 };
//static int GLUTarrows[4] = { 0, 0, 0, 0 };
static int GLUTmodifiers = 0;

// Display variables
static int scaling = 0;
static int translating = 0;
static int rotating = 0;
static float scale = 1.0;
static float center[3] = { 0.0, 0.0, 0.0 };
static float rotation[3] = { 0.0, 0.0, 0.0 };
static float translation[3] = { 0.0, 0.0, -4.0 };

bool ReadOffFile(const char *filename){
    int i;
    
    cout<<filename<<endl;
    FILE *fp;
    if (!(fp = fopen(filename, "r"))) {
        fprintf(stderr, "Unable to open file %s\n", filename);
        return false;
    }
    
    char buffer[1024];
    if (fgets(buffer, 1023, fp)) {
        if (!strstr(buffer, "OFF")){
            printf("It's not a OFF FILE");
            return false;
        }
        
        if (fgets(buffer, 1023, fp)){
            sscanf(buffer, "%d %d %d", &nverts, &nfaces, &nedges);
            verts = new Vertex[nverts];
            faces = new Face[nfaces];
            
            for (i = 0; i < nverts; i++){
                fgets(buffer, 1023, fp);
                sscanf(buffer, "%f%f%f", &verts[i].x, &verts[i].y, &verts[i].z);
            }
            for (i = 0; i < nfaces; i++){
                fgets(buffer, 1023, fp);
                char *bufferp = strtok(buffer, " \t");
                if (bufferp)
                    faces[i].nverts = atoi(bufferp);
                
                // Allocate memory for face vertices
                faces[i].verts = new Vertex *[faces[i].nverts];
                
                // Read vertex indices for face
                for (int j = 0; j < faces[i].nverts; j++) {
                    bufferp = strtok(NULL, " \t");
                    if (bufferp)
                        faces[i].verts[j] = &(verts[atoi(bufferp)]);
                    else {
                        fprintf(stderr, "Syntax error with face on line %d in file %s\n", i, filename);
                        fclose(fp);
                        return NULL;
                    }
                }
                
                // Compute normal for face
                faces[i].normal[0] = faces[i].normal[1] = faces[i].normal[2] = 0;
                Vertex *v1 = faces[i].verts[faces[i].nverts - 1];
                for (int k = 0; k < faces[i].nverts; k++) {
                    Vertex *v2 = faces[i].verts[k];
                    faces[i].normal[0] += (v1->y - v2->y) * (v1->z + v2->z);
                    faces[i].normal[1] += (v1->z - v2->z) * (v1->x + v2->x);
                    faces[i].normal[2] += (v1->x - v2->x) * (v1->y + v2->y);
                    v1 = v2;
                }
                
                // Normalize normal for face
                float squared_normal_length = 0.0;
                squared_normal_length += faces[i].normal[0] * faces[i].normal[0];
                squared_normal_length += faces[i].normal[1] * faces[i].normal[1];
                squared_normal_length += faces[i].normal[2] * faces[i].normal[2];
                float normal_length = sqrt(squared_normal_length);
                if (normal_length > 1.0E-6) {
                    faces[i].normal[0] /= normal_length;
                    faces[i].normal[1] /= normal_length;
                    faces[i].normal[2] /= normal_length;
                }
            }
        }
    }
    fclose(fp);
    
    return true;
}


// GLUT USER INTERFACE CODE
void ThreeDRedraw() {
    // Setup viewing transformation
    glLoadIdentity();
    glScalef(scale, scale, scale);
    glTranslatef(translation[0], translation[1], 0.0);
    
    // Set projection transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)GLUTwindow_width / (GLfloat)GLUTwindow_height, 0.1, 100.0);
    
    // Set camera transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(translation[0], translation[1], translation[2]);
    glScalef(scale, scale, scale);
    glRotatef(rotation[0], 1.0, 0.0, 0.0);
    glRotatef(rotation[1], 0.0, 1.0, 0.0);
    glRotatef(rotation[2], 0.0, 0.0, 1.0);
    glTranslatef(-center[0], -center[1], -center[2]);
    
    // Clear window
    glClearColor(0.3, 0.6, 0.4, 1.0); // Set foreground color
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Set lights
    static GLfloat light0_position[] = { 3.0, 4.0, 5.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    static GLfloat light1_position[] = { -3.0, -2.0, -3.0, 0.0 };
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    
    // Set material
    static GLfloat material[] = { 1.0, 0.5, 0.5, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
    
    // Draw faces
    for (int i = 0; i < nfaces; i++) {
        Face& face = faces[i];
        glBegin(GL_POLYGON);
        glNormal3fv(face.normal);
        for (int j = 0; j < face.nverts; j++) {
            Vertex *vert = face.verts[j];
            glVertex3f(vert->x, vert->y, vert->z);
        }
        glEnd();
    }
    
    // Swap buffers
    glFlush();
}

void GLUTStop(void){
    // Destroy window
    glutDestroyWindow(GLUTwindow);
    
    // Exit
    exit(0);
}

void GLUTResize(int w, int h){
    // Resize window
    glViewport(0, 0, w, h);
    
    // Remember window size
    GLUTwindow_width = w;
    GLUTwindow_height = h;
    
    // Redraw
    glutPostRedisplay();
}

void ThreeDMotion(int x, int y){
    // Invert y coordinate
    y = GLUTwindow_height - y;
    
    // Process mouse motion event
    if (rotating) {
        // Rotate model
        rotation[0] += -0.5 * (y - GLUTmouse[1]);
        rotation[2] += 0.5 * (x - GLUTmouse[0]);
    }
    else if (scaling) {
        // Scale window
        scale *= exp(2.0 * (float)(x - GLUTmouse[0]) / (float)GLUTwindow_width);
    }
    else if (translating) {
        // Translate window
        translation[0] += 2.0 * (float)(x - GLUTmouse[0]) / (float)GLUTwindow_width;
        translation[1] += 2.0 * (float)(y - GLUTmouse[1]) / (float)GLUTwindow_height;
    }
    
    // Remember mouse position
    GLUTmouse[0] = x;
    GLUTmouse[1] = y;
}

void ThreeDmouse(int button, int state, int x, int y){
    // Invert y coordinate
    y = GLUTwindow_height - y;
    
    // Process mouse button event
    rotating = (button == GLUT_LEFT_BUTTON );
    scaling = (button == GLUT_MIDDLE_BUTTON);
    translating = (button == GLUT_RIGHT_BUTTON);
    if (rotating || scaling || translating) glutIdleFunc(ThreeDRedraw);
    else glutIdleFunc(0);
    
    // Remember button state
    int b = (button == GLUT_LEFT_BUTTON) ? 0 : ((button == GLUT_MIDDLE_BUTTON) ? 1 : 2);
    GLUTbutton[b] = (state == GLUT_DOWN) ? 1 : 0;
    
    // Remember modifiers
    GLUTmodifiers = glutGetModifiers();
    
    // Remember mouse position
    GLUTmouse[0] = x;
    GLUTmouse[1] = y;
}

void ThreeDSpecial(int key, int x, int y){
    // Invert y coordinate
    y = GLUTwindow_height - y;
    
    // Process keyboard button event
    
    // Remember mouse position
    GLUTmouse[0] = x;
    GLUTmouse[1] = y;
    
    // Remember modifiers
    GLUTmodifiers = glutGetModifiers();
    
    // Redraw
    glutPostRedisplay();
}
/*
void GLUTKeyboard(unsigned char key, int x, int y){
    // Process keyboard button event
    switch (key) {
        case 'Q':
        case 'q':
            GLUTStop();
            break;
            
        case 27: // ESCAPE
            GLUTStop();
            break;
    }
    
    // Remember mouse position
    GLUTmouse[0] = x;
    GLUTmouse[1] = GLUTwindow_height - y;
    
    // Remember modifiers
    GLUTmodifiers = glutGetModifiers();
}
 */


void ThreeDInit(){
   
    // Initialize GLUT callback functions
    glutReshapeFunc(GLUTResize);
    //glutDisplayFunc(GLUTRedraw);
    glutSpecialFunc(ThreeDSpecial);
    //glutMouseFunc(GLUTMouse);
    //glutMotionFunc(GLUTMotion);
    //glutIdleFunc(0);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // | GLUT_STENCIL

    
    // Initialize lights
    static GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    static GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    //static GLfloat light0_diffuse[] = {0.3, 0.6, 0.4, 1.0 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glEnable(GL_LIGHT0);
    static GLfloat light1_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    
    // Initialize graphics modes
    glEnable(GL_DEPTH_TEST);
    
    // Compute bounding box
    float bbox[2][3] = { { 1.0E30F, 1.0E30F, 1.0E30F }, { -1.0E30F, -1.0E30F, -1.0E30F } };
    for (int i = 0; i < nverts; i++) {
        Vertex& vert = verts[i];
        if (vert.x < bbox[0][0]) bbox[0][0] = vert.x;
        else if (vert.x > bbox[1][0]) bbox[1][0] = vert.x;
        if (vert.y < bbox[0][1]) bbox[0][1] = vert.y;
        else if (vert.y > bbox[1][1]) bbox[1][1] = vert.y;
        if (vert.z < bbox[0][2]) bbox[0][2] = vert.z;
        else if (vert.z > bbox[1][2]) bbox[1][2] = vert.z;
    }
    
    // Setup initial viewing scale
    float dx = bbox[1][0] - bbox[0][0];
    float dy = bbox[1][1] - bbox[0][1];
    float dz = bbox[1][2] - bbox[0][2];
    scale = 2.0 / sqrt(dx*dx + dy*dy + dz*dz);
    
    // Setup initial viewing center
    center[0] = 0.5 * (bbox[1][0] + bbox[0][0]);
    center[1] = 0.5 * (bbox[1][1] + bbox[0][1]);
    center[2] = 0.5 * (bbox[1][2] + bbox[0][2]);
}
/*
void GLUTMainLoop(void){
    // Compute bounding box
    float bbox[2][3] = { { 1.0E30F, 1.0E30F, 1.0E30F }, { -1.0E30F, -1.0E30F, -1.0E30F } };
    for (int i = 0; i < nverts; i++) {
        Vertex& vert = verts[i];
        if (vert.x < bbox[0][0]) bbox[0][0] = vert.x;
        else if (vert.x > bbox[1][0]) bbox[1][0] = vert.x;
        if (vert.y < bbox[0][1]) bbox[0][1] = vert.y;
        else if (vert.y > bbox[1][1]) bbox[1][1] = vert.y;
        if (vert.z < bbox[0][2]) bbox[0][2] = vert.z;
        else if (vert.z > bbox[1][2]) bbox[1][2] = vert.z;
    }
    
    // Setup initial viewing scale
    float dx = bbox[1][0] - bbox[0][0];
    float dy = bbox[1][1] - bbox[0][1];
    float dz = bbox[1][2] - bbox[0][2];
    scale = 2.0 / sqrt(dx*dx + dy*dy + dz*dz);
    
    // Setup initial viewing center
    center[0] = 0.5 * (bbox[1][0] + bbox[0][0]);
    center[1] = 0.5 * (bbox[1][1] + bbox[0][1]);
    center[2] = 0.5 * (bbox[1][2] + bbox[0][2]);
    
    // Run main loop -- never returns
}
*/
#pragma clang diagnostic push

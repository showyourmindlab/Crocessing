#ifndef _CROCESSING_H_
#define _CROCESSING_H_

/* Crocessing is a library that enables Processing-style C-Programming for 
   Creative Coding. Created by Jusub Kim (showyourmind@gmail.com)        */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "FMOD/fmod.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <sys/time.h>
#else
#include <GL/glut.h>
#define POINTS WINDOWS_POINTS // resolve conflict issue with POINTS enum
#include <Windows.h>
#undef POINTS
#endif

#define P2D 0
#define P3D 1
#define OPENGL 2
#define PI 3.141592
#define HALF_PI 1.570796
#define TWO_PI 6.283184

#define pushMatrix glPushMatrix
#define popMatrix glPopMatrix
#define randomSeed srand

extern int mouseX;
extern int mouseY;
extern int mouseButton;
extern char key;
extern int width;
extern int height;
extern int frameCount;
extern int MousePressed;

enum _KEYCODE {NONE, UP, DOWN, LEFT, RIGHT};
extern _KEYCODE keycode;
enum _COLOR_MODE {RGB, HSB};
enum _SHAPE_MODE { NA, CLOSE,
    LINE_STRIP, LINES,
    TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN,
    QUADS, QUAD_STRIP, POLYGON, POINTS};
enum _ALIGN_MODE { CORNER, CENTER };

float random(float s, float e);
void beginShape(_SHAPE_MODE mode=LINE_STRIP);
void endShape(_SHAPE_MODE mode = NA);
void vertex(float x, float y);
void vertex(float x, float y, float z); 
void colorMode(_COLOR_MODE mode);
void setup();
void draw();
void size(int x, int y, int mode);
void size(int x, int y);
void startCrocessing();
void mousePressed();
void mouseReleased();
void mouseMoved();
void mouseDragged();
void keyPressed();
void background(float p1, float p2, float p3, float p4);
void background(float p1, float p2, float p3);
void background(float p1);
void stroke(float p1, float p2, float p3, float p4);
void stroke(float p1, float p2, float p3);
void stroke(float p1);
void point(int x, int y);
void rect(int x, int y, int w, int h);
float constrain(float in, float min, float max);
float dist(float x1, float y1, float x2, float y2);
void ellipse(int x, int y, int width, int height);
void fill(float a, float b, float c, float d);
void fill(float a, float b, float c);
void fill(float a);
void rect(int x, int y, int w, int h);
void strokeWeight(float in);
void line(int x1, int y1, int x2, int y2);
float map(float in, float in_min, float in_max, float out_min, float out_max);
void frameRate(int in);
void translate(float x, float y);
void rotate(float th);
float radians(float in);
void noFill();
void noStroke();
void imageMode(_ALIGN_MODE mode);
int millis();

#ifdef __APPLE__
float min(float a, float b);
float max(float a, float b);
#endif

typedef FMOD_SOUND* CAudio;
CAudio loadAudio(char *name);
void playAudio(CAudio in, float volume=1.f);

struct _cImage{
    unsigned char *pixel;
    int width;
    int height;
    int channels;
    GLuint textureID;
    struct _cImage* next;
};
typedef struct _cImage* CImage;


CImage loadImage(const char *filename);
void image(CImage image, int x, int y);
void image(CImage image, int x, int y, int w, int h);
void vertex(float x, float y, float u, float v);
void vertex(float x, float y, float z, float u, float v);
void texture(CImage img);

void text(const char *text, int x, int y);
void textSize(int size);

#endif
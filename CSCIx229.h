#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

#ifdef __cplusplus
extern "C" {
#endif
// Car object
typedef struct car {
  float x;
  float y;
  float z;
  float dx;
  float dz;
  float velocity;
  int lap;
  int track;
  float box_x;
  float box_z;
} bot_t; 
typedef struct tuple {
  float x;
  float z; 
} tup;
typedef struct track_info{
  tup point;
  tup boundR[18];
  tup boundL[18];
  int l;
} track_in;
typedef struct node { 
  float val [3];
  struct node *next;
} node_t;

// Used functions
void Normalized(float* vector);
void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(float fov,float asp,float dim);
void ErrCheck(const char* where);
int  LoadOBJ(const char* file);
bool CollideBoundary(bot_t A, tup right[], int lenR, tup left[]);
bool Collide(bot_t A, bot_t B);
int BumpInto(bot_t A, bot_t B);
#ifdef __cplusplus
}
#endif

#endif

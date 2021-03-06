/*
 *  Set projection
 */
#include "CSCIx229.h"

void Project(float fov,float asp,float dim)
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective projection
   gluPerspective(fov,asp,dim/16,16*dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}


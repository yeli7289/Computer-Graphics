#include "CSCIx229.h"
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 0;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
float shiny   =   1;  // Shininess (value)
int lightening = 1;
int horse;
// int direction=0;
int ph = 0;
int axes=1;       //  Display axes
int mode=1;  
int fov=55;     //  What to display
float asp=1;
int setRoute = 1;
bool c = false;
float Ex;
float Ey;
float Ez; 
// Texture file 
unsigned int grass;
unsigned int skybox[2];
unsigned int tentt;
int roadmode = 1;
GLdouble model[16];
int width=800;
int height=800;
#define dim 10.0

track_in track_point[21];
bot_t car1 = { .x=0.0, .y=-dim, .z=0.0, .dx=0, .dz=-1, .velocity=3, .lap = 1, .track=1, .box_x=0.75, .box_z=1.0 };
bot_t car_s = { .x=0, .y=-dim , .z=4.0, .dx=0, .dz=-1, .velocity=0.0, .lap = 1, .track=1, .box_x=0.75, .box_z=1.0 };
// The linked list is used to keep track of the AI's route
node_t head1;
node_t *route1 = &head1;
node_t *cur1 = &head1;
float N[] = {0, -1, 0}; // Normal vector for the plane
float E[] = {0, -dim, 0 }; // Point of the plane
static void rectangular(float x, float y, float z, float dx, float dy, float dz, float th)
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, black);
   glPushMatrix();
   glTranslated(x,y,z);

   glBegin(GL_QUADS);
   // back
   glColor3f(0,0,0);
   glVertex3f(dx,-dy,-dz);
   glVertex3f(-dx,-dy,-dz);
   glVertex3f(-dx,dy,-dz);
   glVertex3f(dx,dy,-dz);
   //  front
   glColor3f(0,0,0);
   glVertex3f(-dx,-dy, dz);
   glVertex3f(dx,-dy, dz);
   glVertex3f(dx,dy, dz);
   glVertex3f(-dx,dy, dz);

   //  Right
   glColor3f(0,0,0);
   glVertex3f(dx,-dy,dz);
   glVertex3f(dx,-dy,-dz);
   glVertex3f(dx,dy,-dz);
   glVertex3f(dx,dy,dz);
   //  Left
   glColor3f(0,0,0);
   glVertex3f(-dx,-dy,-dz);
   glVertex3f(-dx,-dy,dz);
   glVertex3f(-dx,dy,dz);
   glVertex3f(-dx,dy,-dz);
   //  Top
   glColor3f(0,0,0);
   glVertex3f(-dx,dy,dz);
   glVertex3f(dx,dy,dz);
   glVertex3f(dx,dy,-dz);
   glVertex3f(-dx,dy,-dz);
   //  Bottom
   glColor3f(0,0,0);
   glVertex3f(-dx,-dy,-dz);
   glVertex3f(dx,-dy,-dz);
   glVertex3f(dx,-dy,dz);
   glVertex3f(-dx,-dy,dz);
   glEnd();
   glPopMatrix();
}
static void lowercar(float x, float y, float z, float dx, float dy, float dz, float h, float th)
{
   float white[] = {1,1,1,1};
   float red[] = {1,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, red);
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glBegin(GL_QUADS);
   //Front
   glColor3f(1,0,0);
   glVertex3f(-dx/2,h+dy,+2*dz/8);
   glVertex3f(-dx/2,h,+dz/2);
   glVertex3f(+dx/2,h,+dz/2);
   glVertex3f(+dx/2,h+dy,+2*dz/8);
   //back
   glColor3f(1,0,0);
   glVertex3f(+dx/2,h,-dz/2);
   glVertex3f(-dx/2,h,-dz/2);
   glVertex3f(-dx/2,h+dy,-2*dz/8);
   glVertex3f(+dx/2,h+dy,-2*dz/8);
   //top
   glColor3f(1,0,0);
   glVertex3f(-dx/2,h+dy,+2*dz/8);
   glVertex3f(+dx/2,h+dy,+2*dz/8);
   glVertex3f(+dx/2,h+dy,-2*dz/8);
   glVertex3f(-dx/2,h+dy,-2*dz/8);
   //bottom
   glColor3f(1,0,0);
   glVertex3f(-dx/2,h,-dz/2);
   glVertex3f(+dx/2,h,-dz/2);
   glVertex3f(+dx/2,h,+dz/2);
   glVertex3f(-dx/2,h,+dz/2);
   //right
   glColor3f(1,0,0);
   glVertex3f(+dx/2,h,+dz/2);
   glVertex3f(+dx/2,h,-dz/2);
   glVertex3f(-dx/2,h+dy,-2*dz/8);
   glVertex3f(+dx/2,h+dy,+2*dz/8);
   //left
   glColor3f(1,0,0);
   glVertex3f(-dx/2,h,-dz/2);
   glVertex3f(-dx/2,h,+dz/2);
   glVertex3f(-dx/2,h+dy,+2*dz/8);
   glVertex3f(-dx/2,h+dy,-2*dz/8);
   glEnd();
   glPopMatrix();
}
static void uppercar(float x, float y, float z, float dx, float dy, float dz, float h, float th)
{
   float white[] = {1,1,1,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glBegin(GL_QUADS);
   // draw the opaque part first
   //top
   glNormal3f(0,1,0);
   glColor3f(1,0,0);
   glVertex3f(-dx/2,h+dy,+2*dz/8);
   glVertex3f(+dx/2,h+dy,+2*dz/8);
   glVertex3f(+dx/2,h+dy,-2*dz/8);
   glVertex3f(-dx/2,h+dy,-2*dz/8);
   //right window bar
   glNormal3f(1,0,0);
   glColor3f(0,0,0);
   glVertex3f(dx/2,h+dy,-dz/16);
   glVertex3f(dx/2,h+dy,dz/16);
   glVertex3f(dx/2,h,dz/16);
   glVertex3f(dx/2,h,-dz/16);
   // left window bar
   glNormal3f(-1,0,0);
   glColor3f(0,0,0);
   glVertex3f(-dx/2,h+dy,-dz/16);
   glVertex3f(-dx/2,h+dy,dz/16);
   glVertex3f(-dx/2,h,dz/16);
   glVertex3f(-dx/2,h,-dz/16);
   glEnd();

   glColor3f(1,0,0);
   glBegin(GL_LINES);
   glVertex3f(-dx/2,h+dy,+2*dz/8);
   glVertex3f(+dx/2,h+dy,+2*dz/8);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(-dx/2,h+dy,+2*dz/8);
   glVertex3f(-dx/2,h,+dz/2);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(-dx/2,h,+dz/2);
   glVertex3f(+dx/2,h,+dz/2);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(+dx/2,h,+dz/2);
   glVertex3f(+dx/2,h+dy,+2*dz/8);
   glEnd();

   glBegin(GL_LINES);
   glVertex3f(+dx/2,h,-dz/2);
   glVertex3f(-dx/2,h,-dz/2);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(-dx/2,h,-dz/2);
   glVertex3f(-dx/2,h+dy,-2*dz/8);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(-dx/2,h+dy,-2*dz/8);
   glVertex3f(+dx/2,h+dy,-2*dz/8);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(+dx/2,h,-dz/2);
   glVertex3f(+dx/2,h+dy,-2*dz/8);
   glEnd();

   // draw transparent part
   glEnable(GL_BLEND);
   glColor4f(1,1,1,0.5);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glDepthMask(0);
   // glEnable(GL_TEXTURE_2D);
   glBegin(GL_QUADS);
   //Front
   glNormal3f(0,1,2);
   glVertex3f(-dx/2,h+dy,+2*dz/8);
   glVertex3f(-dx/2,h,+dz/2);
   glVertex3f(+dx/2,h,+dz/2);
   glVertex3f(+dx/2,h+dy,+2*dz/8);
   //back
   glNormal3f(0,1,-2);
   glVertex3f(+dx/2,h,-dz/2);
   glVertex3f(-dx/2,h,-dz/2);
   glVertex3f(-dx/2,h+dy,-2*dz/8);
   glVertex3f(+dx/2,h+dy,-2*dz/8);
   //right
   glNormal3f(1,0,0);
   glVertex3f(dx/2,h+dy,dz/16);
   glVertex3f(dx/2,h+dy,dz/4);
   glVertex3f(dx/2,h,dz/2);
   glVertex3f(dx/2,h,dz/16);
   glNormal3f(1,0,0);
   glVertex3f(dx/2,h+dy,-dz/4);
   glVertex3f(dx/2,h+dy,-dz/16);
   glVertex3f(dx/2,h,-dz/16);
   glVertex3f(dx/2,h,-dz/2);
   //left
   glNormal3f(-1,0,0);
   glVertex3f(-dx/2,h+dy,dz/16);
   glVertex3f(-dx/2,h+dy,dz/4);
   glVertex3f(-dx/2,h,dz/2);
   glVertex3f(-dx/2,h,dz/16);
   glNormal3f(-1,0,0);
   glVertex3f(-dx/2,h+dy,-dz/4);
   glVertex3f(-dx/2,h+dy,-dz/16);
   glVertex3f(-dx/2,h,-dz/16);
   glVertex3f(-dx/2,h,-dz/2);
   glEnd();
   glDepthMask(1);
   glDisable(GL_BLEND);
  
   // glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}
static void wheel(float x, float y, float z, float thickness, float r)
{  
   // thickness is in terms of degree
   float white[]={1.0, 1.0, 1.0, 1.0};
   float black[] = {0,0,0,1};
   glColor3f(0.0,0.0,0.0);
   glMaterialf(GL_FRONT, GL_SHININESS, shiny);
   glMaterialfv(GL_FRONT, GL_SPECULAR, white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, black);
   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(r,r,r);
   
   glBegin(GL_QUAD_STRIP);
   for (int th=0;th<=360;th+=5)
   {
      glNormal3d(0,Sin(th),Cos(th));
      glVertex3f(0.5,Sin(th), Cos(th));
      glVertex3f(-0.5,Sin(th), Cos(th));
   }
   glEnd();
   glBegin(GL_TRIANGLE_FAN);
   glNormal3d(1,0,0);
   for (int eta=0; eta<=360; eta+=5)
   {
      glVertex3f(0.5, Sin(eta),Cos(eta));
   }
   glNormal3d(-1,0,0);
   for (int eta=0; eta<=360; eta+=5)
   {
      glVertex3f(-0.5, Sin(eta),Cos(eta));
   }
   glEnd();
   glPopMatrix();
     
}
static void circle(float x, float y, float z, float r)
{
   
   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(r,r,r);
   glBegin(GL_TRIANGLE_FAN);
   glColor3f(1,1,0);
   for (int eta=0; eta<=360; eta+=5)
   {
      glVertex3f(Cos(eta),Sin(eta),0);
   }
   glEnd();
   glPopMatrix();
}
static void plate(float x, float y, float z, float dx, float dy)
{
  float white[]={1.0, 1.0, 1.0, 1.0};
  glColor3f(1.0,1.0,1.0);
  glMaterialf(GL_FRONT, GL_SHININESS, shiny);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, white);
  glPushMatrix();
  glTranslated(x,y,z);
  glScaled(dx,dy,1);

  glNormal3f(0,0,1);
  glColor3f(1,1,1);
  glBegin(GL_QUADS);
  glVertex3f(1,1,0);
  glVertex3f(1,-1,0);
  glVertex3f(-1,-1,0);
  glVertex3f(-1,1,0);
  glEnd();
  glPopMatrix();

}
static void tent(float x, float y, float z, float r)
{
  float white[]={1.0, 1.0, 1.0, 1.0};
  glColor3f(1.0,1.0,1.0);
  glMaterialf(GL_FRONT, GL_SHININESS, shiny);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  glPushMatrix();
  glTranslated(x,y,z);
  glScaled(r,r,r);
  //top
  glBegin(GL_TRIANGLE_FAN);
  glColor3f(0.961, 0.871, 0.702);
  glNormal3f(0,1,0);
  for (int th=0; th<=360; th+=5)
  {
    glTexCoord2f(0,0); glVertex3f(Cos(th),Sin(th),0);
  }
  glEnd();
  // upper tent
  glBegin(GL_QUAD_STRIP);
  glColor3f(0.961, 0.871, 0.702);
  for (int th=0;th<=360;th+=5)
  {
    glNormal3d(Cos(th),0,Sin(th));
    glTexCoord2f(0,0); glVertex3f(Cos(th),1.0,Sin(th));

    glNormal3d(0.6*Cos(th),0,0.6*Sin(th));
    glTexCoord2f(0,1); glVertex3f(0.6*Cos(th),1.2,0.6*Sin(th));
  }
  glBegin(GL_QUAD_STRIP);
  glColor3f(0.961, 0.871, 0.702);
  for (int th=0;th<=360;th+=5)
  {
    glNormal3d(0.4*Cos(th),0,0.4*Sin(th));
    glTexCoord2f(0,0); glVertex3f(0.4*Cos(th),1.3,0.4*Sin(th));

    glNormal3d(0.2*Cos(th),0,0.2*Sin(th));
    glTexCoord2f(0,1); glVertex3f(0.2*Cos(th),1.5,0.2*Sin(th));
  }
  // strip
  glBegin(GL_QUAD_STRIP);
  glColor3f(0.647, 0.165, 0.165);
  for (int th=0;th<=360;th+=5)
  {
    glNormal3d(0.6*Cos(th),0,0.6*Sin(th));
    glTexCoord2f(0,0); glVertex3f(0.6*Cos(th),1.2,0.6*Sin(th));

    glNormal3d(0.4*Cos(th),0,0.4*Sin(th));
    glTexCoord2f(0,1); glVertex3f(0.4*Cos(th),1.3,0.4*Sin(th));
  }
  glEnd();
  // lower tent
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,tentt);
  glBegin(GL_QUAD_STRIP);
  glColor3f(0.502, 0.000, 0.000);
  for (int th=0;th<=360;th+=5)
  {
    glNormal3f(Cos(th),0,Sin(th));
    glTexCoord2f(0,0); glVertex3f(Cos(th),0,Sin(th));
    glTexCoord2f(0,1); glVertex3f(Cos(th),1.0,Sin(th));
  }
  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}
static void car(float x, float y, float z, float r, float dx, float dz)
{
   
   GLdouble m[16] = {-dz,0,dx,0, 0,1,0,0, -dx,0,-dz,0, 0,0,0,1};
   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(r,r,r);
   glMultMatrixd(m);
   // car's body
   // enable the offset to push the car body back a bit.
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   // rectangular(0.0,0.0,0.0, 0.3,0.2,0.5, 0.0);
   uppercar(0.0,0.0,0.0, 0.6,0.15,0.5, 0.2, 0.0);
   lowercar(0.0,-0.1,-0.045, 0.6,0.3,1.2, 0.0, 0.0);
   rectangular(0.0,-0.15,-0.045, 0.3,0.03,0.6, 0.0);
   // Disable the offset
   glDisable(GL_POLYGON_OFFSET_FILL);
   // light
   plate(0,-0.12,0.555,0.1,0.05);
   circle(0.15,0,-0.5, 0.1);
   circle(-0.15,0,-0.5, 0.1);
   // 4 wheels
   wheel(0.4,-0.2,0.3, 30, 0.15);
   wheel(0.4,-0.2,-0.3, 30, 0.15);
   wheel(-0.4,-0.2,0.3, 30, 0.15);
   wheel(-0.4,-0.2,-0.3, 30, 0.15);
   // transparent box
   glPopMatrix();
}
static void straightRoad(float x, float y, float z, float theta, float w, float l, int i)
{
	glPushMatrix();
  float white[] = {1,1,1,1};
  float grey[] = {0.5,0.5,0.5,1};
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, grey);

	glTranslated(x,y,z);
	glRotated(theta, 0,-1,0);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1,1);
 
  glColor3f(0.5,0.5,0.5);
  for (float i=0;i>=-l+1;i-=1)
  {
    glNormal3f(0,1,0);
    glBegin(GL_QUADS);
    glVertex3f(-w/2,0,i);
    glVertex3f(w/2,0,i);
    glVertex3f(w/2,0,i-1);
    glVertex3f(-w/2,0,i-1);
    glEnd();
    if (setRoute==1)
    {
       *route1 = (node_t) {.val = {Sin(-theta)*(i-1)+x, 0, Cos(-theta)*(i-1)+z}, .next = malloc(sizeof(node_t))};
       route1 = route1->next;
    }
  }
 //  glNormal3f(0,1,0);
 //  glColor3f(0.5,0.5,0.5);
	// glBegin(GL_QUADS);
	// glVertex3f(-w/2,0,0);
	// glVertex3f(w/2,0,0);
	// glVertex3f(w/2,0,-l);
	// glVertex3f(-w/2,0,-l);
	// glEnd();

	// glDisable(GL_POLYGON_OFFSET_FILL);
 //  glNormal3f(0,1,0);
 //  glColor3f(1,1,1);
	// glBegin(GL_LINES);
	// glVertex3f(0.9*(w/2),0,0);
	// glVertex3f(0.9*(w/2),0,-l);
	// glEnd();

 //  glColor3f(1,1,1);
 //  glNormal3f(0,1,0);
	// glBegin(GL_LINES);
	// glVertex3f(0.9*(-w/2),0,0);
	// glVertex3f(0.9*(-w/2),0,-l);
	// glEnd();
	glPopMatrix();
  if (setRoute==1){
    track_point[i].boundR[0].x=Cos(-theta)*(w/2)+x;
    track_point[i].boundR[0].z=-Sin(-theta)*(w/2)+z;
    track_point[i].boundL[0].x=Cos(-theta)*(-w/2)+x;
    track_point[i].boundL[0].z=-Sin(-theta)*(-w/2)+z;

    track_point[i].boundR[1].x=(Cos(-theta)*(w/2)+Sin(-theta)*(-l))+x;
    track_point[i].boundR[1].z=(-Sin(-theta)*(w/2)+Cos(-theta)*(-l))+z;
    track_point[i].boundL[1].x=(Cos(-theta)*(-w/2)+Sin(-theta)*(-l))+x;
    track_point[i].boundL[1].z=(-Sin(-theta)*(-w/2)+Cos(-theta)*(-l))+z;  
    track_point[i].l = 2;
  }
}
static void splineRoad(float x, float y, float z, float theta, float w, float d, float degree, int i)
{
	glPushMatrix();
  float white[] = {1,1,1,1};
  float grey[] = {0.5,0.5,0.5,1};
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, grey);
	glTranslated(x,y,z);
	glRotated(theta, 0,-1,0);
	glBegin(GL_QUADS);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glColor3f(0.5,0.5,0.5);
  int j = 0;
	if (degree >= 0){
		for (int th=185; th<=degree+180; th+=5)
		{	
      glNormal3f(0,1,0);
			glVertex3f((d+w/2)+(d+w)*Cos(th), 0, (d+w)*Sin(th));
			glVertex3f((d+w/2)+(d+w)*Cos(th-5), 0, (d+w)*Sin(th-5));
			glVertex3f((d+w/2)+d*Cos(th-5), 0, d*Sin(th-5));
			glVertex3f((d+w/2)+d*Cos(th), 0, d*Sin(th));
      if (setRoute==1){
        track_point[i].boundR[j].x = (Cos(-theta)*((d+w/2)+(d+w)*Cos(th-5))+Sin(-theta)*((d+w)*Sin(th-5)))+x;
        track_point[i].boundR[j].z = (-Sin(-theta)*((d+w/2)+(d+w)*Cos(th-5))+Cos(-theta)*((d+w)*Sin(th-5)))+z;
        track_point[i].boundL[j].x = (Cos(-theta)*((d+w/2)+d*Cos(th-5))+Sin(-theta)*(d*Sin(th-5)))+x;
        track_point[i].boundL[j].z = (-Sin(-theta)*((d+w/2)+d*Cos(th-5))+Cos(-theta)*(d*Sin(th-5)))+z;
        j+=1;
        track_point[i].l+=1;
        float vector [3] = {(d+w/2)+(d+w/2)*Cos(th-5), 0, (d+w/2)*Sin(th-5)};
        *route1 = (node_t) {.val = {Cos(-theta)*vector[0]+Sin(-theta)*vector[2]+x,0,-Sin(-theta)*vector[0]+Cos(-theta)*vector[2]+z}, .next = malloc(sizeof(node_t))};
        route1 = route1->next;
      }
		}
		glEnd();
	}
	else{
		for (int th=355; th>=360+degree; th-=5)
		{	
      glNormal3f(0,1,0);
			glVertex3f(-(d+w/2)+(d+w)*Cos(th), 0, (d+w)*Sin(th));
			glVertex3f(-(d+w/2)+(d+w)*Cos(th+5), 0, (d+w)*Sin(th+5));
			glVertex3f(-(d+w/2)+d*Cos(th+5), 0, d*Sin(th+5));
			glVertex3f(-(d+w/2)+d*Cos(th), 0, d*Sin(th));
			if (setRoute==1){
        track_point[i].boundR[j].x = (Cos(-theta)*(-(d+w/2)+(d+w)*Cos(th+5))+Sin(-theta)*((d+w)*Sin(th+5)))+x;
        track_point[i].boundR[j].z = (-Sin(-theta)*(-(d+w/2)+(d+w)*Cos(th+5))+Cos(-theta)*((d+w)*Sin(th+5)))+z;
        track_point[i].boundL[j].x = (Cos(-theta)*(-(d+w/2)+d*Cos(th+5))+Sin(-theta)*(d*Sin(th+5)))+x;
        track_point[i].boundL[j].z = (-Sin(-theta)*(-(d+w/2)+d*Cos(th+5))+Cos(-theta)*(d*Sin(th+5)))+z;
        j+=1;
        track_point[i].l+=1;
        float vector [3] = {-(d+w/2)+(d+w/2)*Cos(th+5), 0, (d+w/2)*Sin(th+5)};
        *route1 = (node_t) {.val = {Cos(-theta)*vector[0]+Sin(-theta)*vector[2]+x,0,-Sin(-theta)*vector[0]+Cos(-theta)*vector[2]+z}, .next = malloc(sizeof(node_t))};
        route1 = route1->next;
      }
		}
		glEnd();
	}
	glPopMatrix();
}
void StraightRoadGenerator(float* px, float* pz, float theta, float l, float width, int i)
{
	straightRoad(*px, -dim, *pz, theta, width, l, i);
  // update the point for the current track path
	*px += l*Cos(theta-90);
  *pz += l*Sin(theta-90);	
  if (i != 20)
  {
    track_point[i].point.x = *px;
    track_point[i].point.z = *pz;
  }

}
void SplineRoadGenerator(float* px, float* pz, float* theta, float d, float degree, float width, int i)
{
	splineRoad(*px,-dim,*pz, *theta, width, d, degree, i);
	if (degree>=0){
		  *px += (d+width/2)*(Cos(*theta+degree+180)-Cos(*theta+180));
   		*pz += (d+width/2)*(Sin(*theta+degree+180)-Sin(*theta+180));
   		*theta += degree;
	}
	else{
		  *px += (d+width/2)*(Cos(*theta+degree)-Cos(*theta));
   		*pz += (d+width/2)*(Sin(*theta+degree)-Sin(*theta));
   		*theta += degree;
	}
  track_point[i].point.x = *px;
  track_point[i].point.z = *pz;
}
static void track1(float px, float pz, float width)
{
	 // starting point of the track
   float theta = 0;
   // straightroad wont change the theta, but we still have to update the new origin
   track_point[0].point.x = px;
   track_point[0].point.z = pz;
   StraightRoadGenerator(&px, &pz, theta, 10.0, width, 1);
   SplineRoadGenerator(&px, &pz, &theta, 5.0, -30, width, 2);
   SplineRoadGenerator(&px, &pz, &theta, 8.0, 45, width, 3);
   StraightRoadGenerator(&px, &pz, theta, 10.0, width, 4);
   SplineRoadGenerator(&px, &pz, &theta, 5.0, 30, width, 5);
   SplineRoadGenerator(&px, &pz, &theta, 8.0, 45, width, 6);
   StraightRoadGenerator(&px, &pz, theta, 10.0, width, 7);
   SplineRoadGenerator(&px, &pz, &theta, 8.0, 45, width, 8);
   SplineRoadGenerator(&px, &pz, &theta, 5.0, -30, width, 9);
   StraightRoadGenerator(&px, &pz, theta, 10.0, width, 10);
   SplineRoadGenerator(&px, &pz, &theta, 15.0, 80, width, 11);
   StraightRoadGenerator(&px, &pz, theta, 30.0, width, 12);
   SplineRoadGenerator(&px, &pz, &theta, 5.0, -30, width, 13);
   SplineRoadGenerator(&px, &pz, &theta, 5.0, 60, width, 14);
   SplineRoadGenerator(&px, &pz, &theta, 5.0, -60, width, 15);
   SplineRoadGenerator(&px, &pz, &theta, 5.0, 60, width, 16);
   SplineRoadGenerator(&px, &pz, &theta, 10.55, 55, width, 17);
   StraightRoadGenerator(&px, &pz, theta,26.0, width, 18);
   SplineRoadGenerator(&px, &pz, &theta, 10.0, 90, width, 19);
   StraightRoadGenerator(&px, &pz, theta,37, width, 20);
   if (setRoute==1){
    *route1 = head1;
    setRoute = 0;
   }
   
}
// static void boundline()
// {
//   glPushMatrix();
//   glColor3f(1,0,0);
//   for (int i=1;i<20;i++)
//   {
//     for (int j=0;j<track_point[i].l-1;j++)
//     {
//       glBegin(GL_LINES);
//       glVertex3f(track_point[i].boundR[j].x,-dim,track_point[i].boundR[j].z);
//       glVertex3f(track_point[i].boundR[j+1].x,-dim,track_point[i].boundR[j+1].z);
//       glEnd();
//       glBegin(GL_LINES);
//       glVertex3f(track_point[i].boundL[j].x,-dim,track_point[i].boundL[j].z);
//       glVertex3f(track_point[i].boundL[j+1].x,-dim,track_point[i].boundL[j+1].z);
//       glEnd();
//     }
//   }
//   glPopMatrix();
// }
static void ground()
{
  float white[] = {1,1,1,1};
  float green[] = {0,1,0,1};
  glMaterialf(GL_FRONT, GL_SHININESS, shiny);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, green);

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,grass);
  glBegin(GL_QUADS);
  glColor3f(0,1,0);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-1000,-dim,1000);
  glTexCoord2f(1000,0); glVertex3f(1000,-dim,1000);
  glTexCoord2f(1000,1000); glVertex3f(1000,-dim,-1000);
  glTexCoord2f(0,1000); glVertex3f(-1000,-dim,-1000);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}
static void banner()
{
  float white[] = {1,1,1,1};
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);

  int length = 4;
  int width = 0;  
  for(float i=-4.5;i<=length;i++)
  {
    for(float j=-4.5;j<=width;j++)
    {
      glBegin(GL_QUADS);
      float x = i/2;
      float z = j/2;
      if (fmod(i+j,2)==0)
        glColor3d(1,1,1);
      else
        glColor3d(0,0,0);
      glNormal3f(0,1,0);
      glVertex3d(x,-dim,z);
      glVertex3d(x+0.5,-dim,z);
      glVertex3d(x+0.5,-dim,z+0.5);
      glVertex3d(x,-dim,z+0.5);
      glEnd();
    }
  }
}
static void sky(float D)
{
  glPushMatrix();
  glTranslated(car_s.x, 0, car_s.z);
  glColor3f(1,1,1);
  glEnable(GL_TEXTURE_2D);
  /* Sides */
  glBindTexture(GL_TEXTURE_2D,skybox[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
  glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
  glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
  glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

  glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
  glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
  glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
  glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

  glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
  glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
  glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
  glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

  glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
  glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
  glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
  glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
  glEnd();

  /* Top and Bottom */
  glBindTexture(GL_TEXTURE_2D,skybox[1]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
  glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
  glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
  glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);

  glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
  glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
  glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
  glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}
void idle()
{
    static double t0 = -1.;
    double dt, t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    if (t0 < 0.0)
      t0 = t;
    dt = t - t0;
    t0 = t;
    float sdx = car_s.velocity*dt*(car_s.dx);
    float sdz = car_s.velocity*dt*(car_s.dz);
    car_s.x+=sdx;
    car_s.z+=sdz;
    // first AI bot's position
    float bot1d [2] = { cur1->val[0]-car1.x, cur1->val[2]-car1.z };
    if (bot1d[0]==0 && bot1d[1]==0)
    {
      cur1 = cur1->next;
      bot1d[0] =  cur1->val[0]-car1.x;
      bot1d[1] =  cur1->val[2]-car1.z;
    }  
    car1.dx = bot1d[0]/sqrt(pow(bot1d[0],2)+pow(bot1d[1],2));
    car1.dz = bot1d[1]/sqrt(pow(bot1d[0],2)+pow(bot1d[1],2));
    float dx = car1.velocity*dt*car1.dx;
    float dz = car1.velocity*dt*car1.dz;
    car1.x += dx;
    car1.z += dz;
    c = Collide(car_s, car1);
    if (CollideBoundary(car_s, track_point[car_s.track].boundR, track_point[car_s.track].l, track_point[car_s.track].boundL))
    {
      if (car_s.velocity>=3)
        car_s.velocity-=0.06;
      roadmode = abs(roadmode-1);
    }
    if (Collide(car_s, car1))
    {
    // the colliding car will return to the original location
    // 1 if s bump into 1, and 0 if 1 bump into s.
      if (BumpInto(car_s, car1)==1){
        car_s.x-=20*sdx;
        car_s.z-=20*sdz;
        if (car_s.velocity>=3)
          car_s.velocity-=1.8;
        // direction+=30;
        // float temp = car_s.dx;
        // car_s.dx = car_s.dx*Cos(-30)+car_s.dz*Sin(-30);
        // car_s.dz = -temp*Sin(-30)+car_s.dz*Cos(-30);
        // update car1's next point
        if (car1.velocity*dt>=sqrt(pow(bot1d[0],2)+pow(bot1d[1],2)))
          cur1 = cur1->next;
      }
      else
      {
        car1.x-=25*dx;
        car1.z-=25*dz;
        if (car1.velocity>=3)
          car1.velocity-=1.8;
      }
    }
    else{
      if (car1.velocity*dt>=sqrt(pow(bot1d[0],2)+pow(bot1d[1],2)))
        cur1 = cur1->next;
    }

    // update your track, lap information
    if (car_s.track < 20)
    {
      float track_theta = ((track_point[car_s.track].point.x-track_point[car_s.track-1].point.x)*(car_s.x-track_point[car_s.track].point.x)+(track_point[car_s.track].point.z-track_point[car_s.track-1].point.z)*(car_s.z-track_point[car_s.track].point.z))/((sqrt( pow(track_point[car_s.track].point.x-track_point[car_s.track-1].point.x,2)+pow(track_point[car_s.track].point.z-track_point[car_s.track-1].point.z,2)))*(sqrt(pow(car_s.x-track_point[car_s.track].point.x,2)+pow(car_s.z-track_point[car_s.track].point.z,2))));
      if (track_theta>0)
        car_s.track+=1;
    }
    else 
    {
      float track_theta = ((track_point[0].point.x-track_point[car_s.track-1].point.x)*(car_s.x-track_point[0].point.x)+(track_point[0].point.z-track_point[car_s.track-1].point.z)*(car_s.z-track_point[0].point.z))/((sqrt(pow(track_point[0].point.x-track_point[car_s.track-1].point.x,2)+pow(track_point[0].point.z-track_point[car_s.track-1].point.z,2)))*(sqrt(pow(car_s.x-track_point[0].point.x,2)+pow(car_s.z-track_point[0].point.z,2))));
      if (track_theta>0)
      {
        car_s.track=1;
        car_s.lap+=1;
      }
    }  
    // update the ai car's track, lap information

    if (car1.track < 20)
    {
      float track_theta = ((track_point[car1.track].point.x-track_point[car1.track-1].point.x)*(car1.x-track_point[car1.track].point.x)+(track_point[car1.track].point.z-track_point[car1.track-1].point.z)*(car1.z-track_point[car1.track].point.z))/((sqrt(pow(track_point[car1.track].point.x-track_point[car1.track-1].point.x,2)+pow(track_point[car1.track].point.z-track_point[car1.track-1].point.z,2)))*(sqrt(pow(car1.x-track_point[car1.track].point.x,2)+pow(car1.z-track_point[car1.track].point.z,2))));
      if (track_theta>0)
        car1.track+=1;
    }
    else
    {
      float track_theta = ((track_point[0].point.x-track_point[car1.track-1].point.x)*(car1.x-track_point[0].point.x)+(track_point[0].point.z-track_point[car1.track-1].point.z)*(car1.z-track_point[0].point.z))/((sqrt(pow(track_point[0].point.x-track_point[car1.track-1].point.x,2)+pow(track_point[0].point.z-track_point[car1.track-1].point.z,2)))*(sqrt(pow(car1.x-track_point[0].point.x,2)+pow(car1.z-track_point[0].point.z,2))));
      if (track_theta>0) 
      {
        car1.track=1;
        car1.lap+=1;
      }
    }
    // update the speed of the AI's car
    if ((car_s.lap==car1.lap && car_s.track>=car1.track)||car_s.lap>car1.lap)
      car1.velocity+=0.02;
    else
    {
      if (car1.velocity > 3)
        car1.velocity-=0.02;
    }
  glutPostRedisplay();
}
void display()
{
	//  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   // if (mode==1){
   //     //  Set view angle orthogonal
   //    glRotatef(ph,1,0,0);
   //    glRotatef(th,0,1,0);
   // }
   if (mode==1)
   {
      // third person mode
      Ex = car_s.x+3.0*(-car_s.dx);
      Ey = 1.2-dim;
      Ez = car_s.z+3.0*(-car_s.dz);
      gluLookAt(Ex, Ey, Ez, car_s.x, 1.2-dim, car_s.z, 0, Cos(ph), 0);
   }
   else if (mode==2)
   {
     // first person mode
      gluLookAt(car_s.x,1.0-dim,car_s.z , car_s.x+2.0*car_s.dx,0.5-dim,car_s.z+2.0*car_s.dz, 0,Cos(ph),0);
   }
  float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
  float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
  float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
  
  float Position[]  = {dim,dim,0,0.0};
  //  OpenGL should normalize normal vectors
  glEnable(GL_NORMALIZE);
  //  Enable lighting
  glEnable(GL_LIGHTING);
  // Location of viewer for specular calculations
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);
 //  glColor sets ambient and diffuse color materials
  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
//  Enable light 0
  glEnable(GL_LIGHT0);
 //  Set ambient, diffuse, specular components and position of light 0
  glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
  glLightfv(GL_LIGHT0,GL_POSITION,Position);

   
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2,1);
  sky(4*dim);
  ground();

  glDisable(GL_POLYGON_OFFSET_FILL);
  track1(0, 0, 4.5);
  banner();
  tent(15,-dim,0, 1);
  tent(-15,-dim,-25, 1);
  tent(5,-dim,-18, 1);
  tent(5,-dim,18, 1);
  tent(40,-dim,20, 1);
  glPushMatrix();
  glColor3f(0.627, 0.322, 0.176);
  glTranslated(5,-dim,-10);
  glRotated(-90,1,0,0);
  glScaled(0.2,0.2,0.2);
  glCallList(horse);
  glPopMatrix();
  glPushMatrix();
  glTranslated(10,-dim,5);
  glRotated(180,0,1,0);
  glRotated(-90,1,0,0);
  glScaled(0.2,0.2,0.2);
  glCallList(horse);
  glPopMatrix();
  glPushMatrix();
  glTranslated(10,-dim,-15);
  glRotated(180,0,1,0);
  glRotated(-90,1,0,0);
  glScaled(0.2,0.2,0.2);
  glCallList(horse);
  glPopMatrix();
  glPushMatrix();
  glTranslated(-10,-dim,-15);
  glRotated(180,0,1,0);
  glRotated(-90,1,0,0);
  glScaled(0.2,0.2,0.2);
  glCallList(horse);
  glPopMatrix();
  glPushMatrix();
  glTranslated(-10,-dim,5);
  glRotated(180,0,1,0);
  glRotated(-90,1,0,0);
  glScaled(0.2,0.2,0.2);
  glCallList(horse);
  glPopMatrix();
  car(car_s.x,0.5-dim, car_s.z, 1.0, car_s.dx, car_s.dz);
  car(car1.x,0.5-dim,car1.z, 1.0, car1.dx, car1.dz);
  glDisable(GL_LIGHTING);
  glColor3f(1,0,0);
  glWindowPos2i(5,5);
  glWindowPos2i(5,5);
  Print("Speed %f, road:%d, track:%d %d, %d %d", car_s.velocity*5, roadmode, car_s.lap, car_s.track, car1.lap, car1.track);
  glFlush();
  glutSwapBuffers();
}

void key(unsigned char ch, int x, int y)
{
   if (ch==27)
      exit(0);
   else if (ch=='+' && mode<2)
      mode+=1;
   else if (ch=='-' && mode>1)
      mode-=1;
   else if (ch=='w')
   {
      car_s.velocity+=3;
   }
   else if (ch=='s')
   {  
      car_s.velocity-=3;
   }
   else if (ch=='d')
   {
      // direction+=3;
      float temp = car_s.dx;
      car_s.dx = car_s.dx*Cos(3)-car_s.dz*Sin(3);
      car_s.dz = temp*Sin(3)+car_s.dz*Cos(3);
   }
   else if (ch=='a')
   {
      // direction-=3;
      car_s.dx = car_s.dx*Cos(-3)-car_s.dz*Sin(-3);
      car_s.dz = car_s.dx*Sin(-3)+car_s.dz*Cos(-3);
   }
   Project(fov, asp, dim);
   glutIdleFunc(idle);
   glutPostRedisplay();
}
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   float asp = (height>0) ? (float)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   Project(fov, asp, dim);
}

int main(int argc,char* argv[])
{
   glutInit(&argc,argv);
   //  Request float buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(width,height);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH |  GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Racing Game Yen-Teh Liu");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   skybox[0] = LoadTexBMP("image/sky0.bmp");
   skybox[1] = LoadTexBMP("image/sky1.bmp");
   grass = LoadTexBMP("image/grass1.bmp");
   tentt = LoadTexBMP("image/lower_tent.bmp");
   const char* obj = "obj/horse1/horse.obj";
   horse = LoadOBJ(obj);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}

/*
 *  Normalize the vector
 */
#include "CSCIx229.h"
void Normalized(float vector[3])
{

	float vn = sqrt(pow(vector[0],2)+pow(vector[1],2)+pow(vector[2],2));
	vector[0] = vector[0]/vn;
  	vector[1] = vector[1]/vn;
  	vector[2] = vector[2]/vn;
}
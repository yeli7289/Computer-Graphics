#include "CSCIx229.h"
bool onSegment(tup p, tup q, tup r)
{
    if (q.x <= fmax(p.x, r.x) && q.x >= fmin(p.x, r.x) &&\
        q.z <= fmax(p.z, r.z) && q.z >= fmin(p.z, r.z))
       return true;
    return false;
}
int orientation(tup p, tup q, tup r)
{
    int val = (q.z - p.z) * (r.x - q.x) -(q.x - p.x) * (r.z - q.z);
    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}
bool doIntersect(tup p1, tup q1, tup p2, tup q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
 
    // General case
    if (o1 != o2 && o3 != o4)
        return true;
 
    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
 
    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
 
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
 
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
 
    return false; // Doesn't fall in any of the above cases
}
bool InsideCar(tup A, tup B[4])
{
	double m[2] = {A.x-B[0].x, A.z-B[0].z};
	double b[2] = {B[1].x-B[0].x, B[1].z-B[0].z};
	double d[2] = {B[3].x-B[0].x, B[3].z-B[0].z};
	if (((m[0]*b[0]+m[1]*b[1]>=0)&&(b[0]*b[0]+b[1]*b[1]>=m[0]*b[0]+m[1]*b[1]))&&\
		((m[0]*d[0]+m[1]*d[1]>=0)&&(d[0]*d[0]+d[1]*d[1]>=m[0]*d[0]+m[1]*d[1])))
		return true;
	else
		return false;
}
bool CollideBoundary(bot_t A, tup right[], int lenR, tup left[])
{
	tup vertexA[4];
	vertexA[0].x = A.x+0.5*A.box_x*A.dx-0.5*A.box_z*A.dz;
	vertexA[0].z = A.z+0.5*A.box_x*A.dx+0.5*A.box_z*A.dz;
	vertexA[1].x = A.x+0.5*A.box_x*A.dx+0.5*A.box_z*A.dz;
	vertexA[1].z = A.z+0.5*A.box_z*A.dz-0.5*A.box_x*A.dx;
	vertexA[2].x = A.x+0.5*A.box_z*A.dz-0.5*A.box_x*A.dx;
	vertexA[2].z = A.z-0.5*A.box_x*A.dx-0.5*A.box_z*A.dz;
	vertexA[3].x = A.x-0.5*A.box_x*A.dx-0.5*A.box_z*A.dz;
	vertexA[3].z = A.z+0.5*A.box_x*A.dx-0.5*A.box_z*A.dz;

	if (lenR==2)
	{
		if (doIntersect(right[0], right[1], vertexA[0], vertexA[1]) ||\
			doIntersect(right[0], right[1], vertexA[1], vertexA[2]) ||\
			doIntersect(right[0], right[1], vertexA[2], vertexA[3]) ||\
			doIntersect(right[0], right[1], vertexA[3], vertexA[0]) ||\
			doIntersect(left[0], left[1], vertexA[0], vertexA[1]) ||\
			doIntersect(left[0], left[1], vertexA[1], vertexA[2]) ||\
			doIntersect(left[0], left[1], vertexA[2], vertexA[3]) ||\
			doIntersect(left[0], left[1], vertexA[3], vertexA[0])
			)
			return true;
	}
	else
	{
		for (int i=0;i<lenR;i++)
		{
		if (InsideCar(right[i],vertexA)||InsideCar(left[i],vertexA))
			return true;
		}	
	}
	return false;
}
bool Collide(bot_t A, bot_t B)
{
	if (A.track != B.track)
		return false;
	tup vertexA[4];
	tup vertexB[4];
	vertexA[0].x = A.x+0.5*A.box_x*A.dx-0.5*A.box_z*A.dz;
	vertexA[0].z = A.z+0.5*A.box_x*A.dx+0.5*A.box_z*A.dz;
	vertexA[1].x = A.x+0.5*A.box_x*A.dx+0.5*A.box_z*A.dz;
	vertexA[1].z = A.z+0.5*A.box_z*A.dz-0.5*A.box_x*A.dx;
	vertexA[2].x = A.x+0.5*A.box_z*A.dz-0.5*A.box_x*A.dx;
	vertexA[2].z = A.z-0.5*A.box_x*A.dx-0.5*A.box_z*A.dz;
	vertexA[3].x = A.x-0.5*A.box_x*A.dx-0.5*A.box_z*A.dz;
	vertexA[3].z = A.z+0.5*A.box_x*A.dx-0.5*A.box_z*A.dz;

	vertexB[0].x = B.x+0.5*B.box_x*B.dx-0.5*B.box_z*B.dz;
	vertexB[0].z = B.z+0.5*B.box_x*B.dx+0.5*B.box_z*B.dz;
	vertexB[1].x = B.x+0.5*B.box_x*B.dx+0.5*B.box_z*B.dz;
	vertexB[1].z = B.z+0.5*B.box_z*B.dz-0.5*B.box_x*B.dx;
	vertexB[2].x = B.x+0.5*B.box_z*B.dz-0.5*B.box_x*B.dx;
	vertexB[2].z = B.z-0.5*B.box_x*B.dx-0.5*B.box_z*B.dz;
	vertexB[3].x = B.x-0.5*B.box_x*B.dx-0.5*B.box_z*B.dz;
	vertexB[3].z = B.z+0.5*B.box_x*B.dx-0.5*B.box_z*B.dz;

	if (InsideCar(vertexA[0], vertexB) || InsideCar(vertexA[1], vertexB) ||\
		InsideCar(vertexA[2], vertexB) || InsideCar(vertexA[3], vertexB) ||\
		InsideCar(vertexB[0], vertexA) || InsideCar(vertexB[1], vertexA) ||\
		InsideCar(vertexB[2], vertexA) || InsideCar(vertexB[3], vertexA))
		return true;
	else
		return false;
}
int BumpInto(bot_t A, bot_t B)
{
	double vectorAB[2] = {A.x-B.x, A.z-B.z};
	double vectorBA[2] = {B.x-A.x, B.z-A.z};
	// B bump into A
	if (A.dx*vectorBA[0]+A.dz*vectorBA[1]>=B.dx*vectorAB[0]+B.dz*vectorAB[1])
		return 1;
	// A bump into B
	else 
		return 0;
}


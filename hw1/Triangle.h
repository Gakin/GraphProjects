#ifndef __Triangle_h__
#define __Triangle_h__

#include "Vertex.h"
class Triangle{
		public:
			int vertices[3];
			int materialID;
			int triangleID;
			float normal[3];
			float side1[3];
			float side2[3];
			float side3[3];
		float findIntersectionWithRay(float cameraPosition[3],float rayVector[3]);
	};

#endif //__Triangle_h__

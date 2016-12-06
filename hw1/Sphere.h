#ifndef __Sphere_h__
#define __Sphere_h__
class Sphere{
		public:
			int center;
			float radius;
			int materialID;
			int sphereID;
			float normal[3];
			float sCenter[3];			
		float findIntersectionWithRay(float cameraPosition[3],float rayVector[3]);

	};

#endif //__Sphere_h__

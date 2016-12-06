#ifndef __Camera_h__
#define __Camera_h__
#include <iostream>
class Camera{
		public:
			float gazeVector[3];
			float upVector[3];
			float positionVector[3];
			float uVector[3];
			float left;
			float right;
			float top;
			float bottom;
			float distance;
			int nx;
			int ny;
			int cameraID;
			
			std::string outputFileName; 
	};

#endif //__Camera_h__

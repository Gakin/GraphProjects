#ifndef __Vertex_h__
#define __Vertex_h__
class Vertex{
		public:
			float coordinates[3];
			int vertexID;
			Vertex crossProduct(Vertex a);
			float dotProduct(Vertex a);
	};

#endif //__Vertex_h__

#ifndef __Material_h__
#define __Material_h__
class Material{
		public:
				int materialID;
				float specular[4]; // Last Element Spec Exp
				float diffuse[3]; 
				float ambient[3];
				float reflectance[3];
	};

#endif //__Material_h__

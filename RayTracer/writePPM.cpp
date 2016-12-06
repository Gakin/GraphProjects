#include <cstdio>
#include <vector>
#include "Material.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Vertex.h"
#include "Camera.h"
#include "PointLight.h"
#include "Ray.h"
#include "Colour.h"
#include <cfloat>

using namespace std;

extern vector<Material*> materials;
extern vector<Sphere*> spheres;
extern vector<Triangle*> triangles;
extern vector<Camera*> cameras;
extern vector<PointLight*> pointLights;
extern vector<Vertex*> vertexes;
//extern vector<Colour*> colours;

extern float ambientLight[3];
extern float background[3];
extern int rayReflectionCount;

//extern vector<Ray*> rays;

void writePPM(const char* filename, int width, int height,float data[])
{
    FILE* fp = fopen(filename, "w");

    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");
    
    /*cout << rays.size()<< endl;
	for(unsigned int i = 0 ; i < rays.size() ; i++ ){
			
		if(rays[i]->temp != FLT_MAX)	
			cout << rays[i]->temp << endl;

	}*/
    int index = 0 ;
    int colourIndex = 0;
    //cout << colours.size() << endl;
    for (int y = 0, i = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x, i += 3)
        {

            //if(rays[index]->temp != FLT_MAX){
                //cout << colours[colourIndex]->RGB[0] << " " << colours[colourIndex]->RGB[1] << " " << colours[colourIndex]->RGB[2] << endl;
                float a , b , c = 0;
                a = data[i];
                b = data[i+1];
                c = data[i+2];

                if(a > 255){
                    a = 255;
                }
                else if(a < 0){
                    a = 0;
                }
                if(b > 255){
                    b = 255;
                }
                else if(b < 0){
                    b = 0;
                }
                if(c > 255){
                    c = 255;
                }
                else if(c < 0){
                    c = 0;
                }
                //cout << " colour me ! "<< colourIndex<< " "<< endl;
                 //cout << a << " " << b << " "<< c << endl;
                fprintf(fp, "%d %d %d ", (int)a, (int)b, (int) c);
            //}
            //else{
                //cout << background[0] << " " << background[1] << " " << background[2] << endl;
              //  fprintf(fp, "%d %d %d ", (int)(background[0]), (int)(background[1]), (int)(background[2]));
            //}
            index++;
        }
        //colours.clear();
        fprintf(fp, "\n");
    }

    fclose(fp);
}

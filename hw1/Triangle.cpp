#include "Triangle.h"
#include "math.h"
#include <cfloat>
#include <vector>
#include <iostream>

using namespace std;

//extern vector<Vertex*> vertexes;



/*float* crossProduct(float arg1[3],float arg2[3]){
        float * result = new float[3];
        result[0] = arg1[1]*arg2[2] - arg2[1]*arg1[2];
        result[1] = arg1[2]*arg2[0] - arg2[2]*arg1[0];
        result[2] = arg1[0]*arg2[1] - arg2[0]*arg1[1];
        return result;
    }
*/


float Triangle::findIntersectionWithRay(float cameraPosition[3],float ray[3]){


   // int i=0,j=0,z=0;

    //int maxVerticeIndex = 0;

  /* if(this .vertices[0] > this .vertices[1] && this .vertices[0] > this .vertices[2])
        maxVerticeIndex = this .vertices[0];
    if(this .vertices[1] > this .vertices[0] && this .vertices[1] > this .vertices[2])
        maxVerticeIndex = this .vertices[1];
    if(this .vertices[2] > this .vertices[0] && this .vertices[2] > this .vertices[1])
        maxVerticeIndex = this .vertices[2];
*/




    Vertex  normal  ;
    Vertex  ba   ;
    Vertex  ca   ;
    Vertex  ao   ;
    Vertex  camera   ;
    Vertex  rayV   ;
    Vertex  cb   ;
    Vertex  ac   ;
    Vertex  pa   ;
    Vertex  pb   ;
    Vertex  pc   ;

    camera .coordinates[0] = cameraPosition[0];
    camera .coordinates[1] = cameraPosition[1];
    camera .coordinates[2] = cameraPosition[2];

    rayV .coordinates[0] = ray[0];
    rayV .coordinates[1] = ray[1];
    rayV .coordinates[2] = ray[2];

    cb .coordinates[0] = side3[0] - side2[0];
    ba .coordinates[0] = side2[0] - side1[0];
    ca .coordinates[0] = side3[0] - side1[0];
    ac .coordinates[0] = side1[0] - side3[0];

    ac .coordinates[1] = side1[1] - side3[1];
    cb .coordinates[1] = side3[1] - side2[1];
    ba .coordinates[1] = side2[1] - side1[1];
    ca .coordinates[1] = side3[1] - side1[1];

    ac .coordinates[2] = side1[2] - side3[2];
    cb .coordinates[2] = side3[2] - side2[2];
    ba .coordinates[2] = side2[2] - side1[2];
    ca .coordinates[2] = side3[2] - side1[2];

    ao .coordinates[0] = side1[0] - cameraPosition[0];
    ao .coordinates[1] = side1[1] - cameraPosition[1];
    ao .coordinates[2] = side1[2] - cameraPosition[2];


    /*  result[0] = arg1[1]*arg2[2] - arg2[1]*arg1[2];
        result[1] = arg1[2]*arg2[0] - arg2[2]*arg1[0];
        result[2] = arg1[0]*arg2[1] - arg2[0]*arg1[1];*/

    /*
    float fp[3];
    fp[0]= ba .coordinates[1]*ca .coordinates[2] - ba .coordinates[2]*ca .coordinates[1];
    fp[1]= ba .coordinates[2]*ca .coordinates[0] - ba .coordinates[0]*ca .coordinates[2];
    fp[2]= ba .coordinates[0]*ca .coordinates[1] - ba .coordinates[1]*ca .coordinates[0];
    //cout << fp[0]<< endl;
    // Used for the height of the normal vector !
    float var1 = sqrt(fp[0]*fp[0]+fp[1]*fp[1]+fp[2]*fp[2]);

    this .normal[0] = normal .coordinates[0] = fp[0]/var1;
    this .normal[1] = normal .coordinates[1] = fp[1]/var1;
    this .normal[2] = normal .coordinates[2] = fp[2]/var1;
    */
            normal .coordinates[0] = this->normal[0];
            normal .coordinates[1] = this->normal[1];
            normal .coordinates[2] = this->normal[2];

           //cout << normal.coordinates[0] << " " << normal.coordinates[1] << " " << normal.coordinates[2] << endl;


    float aoDotNormal = ao .dotProduct(normal);
    float rayDotNormal = rayV .dotProduct(normal);

    float t = aoDotNormal / rayDotNormal;

    float point[3] ;

    point[0] = camera .coordinates[0] + t*rayV .coordinates[0];
    point[1] = camera .coordinates[1] + t*rayV .coordinates[1];
    point[2] = camera .coordinates[2] + t*rayV .coordinates[2];

    pa .coordinates[0] = point[0] - side1[0];
    pa .coordinates[1] = point[1] - side1[1];
    pa .coordinates[2] = point[2] - side1[2];

    pb .coordinates[0] = point[0] - side2[0];
    pb .coordinates[1] = point[1] - side2[1];
    pb .coordinates[2] = point[2] - side2[2];

    pc .coordinates[0] = point[0] - side3[0];
    pc .coordinates[1] = point[1] - side3[1];
    pc .coordinates[2] = point[2] - side3[2];

    float fp1[3];
    fp1[0] = ba .coordinates[1]*pa .coordinates[2] - ba .coordinates[2]*pa .coordinates[1];
    fp1[1] = ba .coordinates[2]*pa .coordinates[0] - ba .coordinates[0]*pa .coordinates[2];
    fp1[2] = ba .coordinates[0]*pa .coordinates[1] - ba .coordinates[1]*pa .coordinates[0];


    float fp2[3];
    fp2[0] = cb .coordinates[1]*pb .coordinates[2] - cb .coordinates[2]*pb .coordinates[1];
    fp2[1] = cb .coordinates[2]*pb .coordinates[0] - cb .coordinates[0]*pb .coordinates[2];
    fp2[2] = cb .coordinates[0]*pb .coordinates[1] - cb .coordinates[1]*pb .coordinates[0];

    float fp3[3];
    fp3[0] = ac .coordinates[1]*pc .coordinates[2] - ac .coordinates[2]*pc .coordinates[1];
    fp3[1] = ac .coordinates[2]*pc .coordinates[0] - ac .coordinates[0]*pc .coordinates[2];
    fp3[2] = ac .coordinates[0]*pc .coordinates[1] - ac .coordinates[1]*pc .coordinates[0];




    if((fp1[0]*fp2[0]+fp1[1]*fp2[1]+fp1[2]*fp2[2])>-0.00000000000000001 && (fp2[0]*fp3[0]+fp2[1]*fp3[1]+fp2[2]*fp3[2])>-0.00000000000000001)
    {

        return t;
    }
    return FLT_MAX;



}

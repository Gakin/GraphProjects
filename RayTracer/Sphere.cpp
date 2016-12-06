#include "Sphere.h"
#include <cmath>
#include <iostream>

using namespace std;

float dotProduct(float first[],float second[]){

    return (first[0]*second[0])+(first[1]*second[1])+(first[2]*second[2]);

}


float Sphere::findIntersectionWithRay(float cameraPosition[], float rayVector[]){

        float arg[3] ;

        arg[0] = cameraPosition[0] - sCenter[0];
        arg[1] = cameraPosition[1] - sCenter[1];
        arg[2] = cameraPosition[2] - sCenter[2];

        float A = dotProduct(rayVector,rayVector);
        float B = 2*dotProduct(rayVector,arg);
        float C = dotProduct(arg,arg) - this->radius * this->radius;

        float discriminand = (B*B) - (4*A*C);
        if(discriminand <= 0)
            return -1;
        float t1 = (-B-sqrt(discriminand))/(2*A);
        float t2 = (-B+sqrt(discriminand))/(2*A);
        if(t1 > t2) {
            //cout << discriminand<< endl;
            return t2;

        }
        else if(t1  < 0 && t2 < 0){
            return -1;
        }
        //cout << discriminand << endl;
        return t1;


}

#include <iostream>
#include "parseScene.h"
#include "writePPM.h"
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
#include "math.h"

using namespace std;

vector<Material*> materials;
vector<Sphere*> spheres;
vector<Triangle*> triangles;
vector<Camera*> cameras;
vector<PointLight*> pointLights;
vector<Vertex*> vertexes;


extern float ambientLight[3];
extern float background[3];
extern int rayReflectionCount;

//vector<Ray*> rays;
float * bigData;
int dataIndex = 0;
float cameraX = 0;
float cameraY = 0;
float cameraZ = 0;


Colour calculateColour(Ray * newRay ,int reflectionCount){



    //cout << newRay->direction[0] << " " << newRay->direction[1] << " " << newRay->direction[2] << " ";
    //rays.push_back(newRay);

    //cout << xValue << " " << yValue << endl;
    //cout << newRay->direction[0] << " " <<
    //      newRay->direction[1] << " " <<
    //    newRay->direction[2] << " ";


    // For now the function returns Discriminand !


    // Vertex is guaranteed to be existed !
    int i = 0;
    //unsigned int j = 0; // vertexindex !
    unsigned int k = 0; // sphereindex !
    unsigned int l = 0; // triangleindex !
    int index = 0;

    // for(z = 0 ; z < rays.size();z++){
    int flag = -1;
    for(k = 0 ; k < spheres.size();k++){

        /*float func1[3];
                func1[0] = vertexes[j]->coordinates[0];
                func1[1] = vertexes[j]->coordinates[1];
                func1[2] = vertexes[j]->coordinates[2];*/

        float func2[3];
        func2[0] = cameraX;
        func2[1] = cameraY;
        func2[2] = cameraZ;

        float func3[3];
        func3[0] = newRay->direction[0];
        func3[1] = newRay->direction[1];
        func3[2] = newRay->direction[2];


        float newValue = spheres[k]->findIntersectionWithRay(func2,func3);

        if(newRay->temp > newValue && newValue>0){
            flag = 0;
            index = k;
            newRay->temp = newValue ;
        }

    }
    for(l=0;l<triangles.size();l++){


        float func1[3];
        func1[0] = cameraX;
        func1[1] = cameraY;
        func1[2] = cameraZ;


        float func2[3];
        func2[0] = newRay->direction[0];
        func2[1] = newRay->direction[1];
        func2[2] = newRay->direction[2];

        float temptTri = triangles[l]->findIntersectionWithRay(func1,func2);
        //cout << temptTri << endl;
        if(temptTri < newRay->temp && temptTri>0){
            flag = 1;
            index = l;
            newRay->temp = temptTri;
        }
    }
    // Its a triangle Use l
    // cout << l << " "<< k << " "<<flag <<endl;
    if(flag == 1){
        unsigned int q = 0;
        //              cout << q << endl;
        for(q = 0; q < materials.size() ;q++){
            if(materials[q]->materialID == triangles[index]->materialID)
                break;
        }
        //            cout << q << endl;

        Colour pixelColour;
        pixelColour.RGB[0] = materials[q]->ambient[0]*ambientLight[0];
        pixelColour.RGB[1] = materials[q]->ambient[1]*ambientLight[1];
        pixelColour.RGB[2] = materials[q]->ambient[2]*ambientLight[2];

        float recNormal[3];
        float myA;
        float myB;
        float myC;

        for(int w = 0; w < pointLights.size() ;w++ ){

            bool shadowFlag = true;
            float g = -1; // intersection with shadowRay initial value !

            // Epsilon is currently 0.0001 !
            myA = cameraX + newRay->temp*newRay->direction[0];
            myB = cameraY + newRay->temp*newRay->direction[1];
            myC = cameraZ + newRay->temp*newRay->direction[2];

            //////////////////////////////////////Shadow Ray Check with Spheres !////////////////////////////////////////////////////////////////////////////////////
            for(int t = 0 ; t < spheres.size() ; t++){

                float intersection[3];
                intersection[0] = (myA);
                intersection[1] = (myB);
                intersection[2] = (myC);

                float shadowRayVect[3];

                shadowRayVect[0] = pointLights[w]->positionVector[0] - intersection[0];
                shadowRayVect[1] = pointLights[w]->positionVector[1] - intersection[1];
                shadowRayVect[2] = pointLights[w]->positionVector[2] - intersection[2];



                intersection[0] += shadowRayVect[0]*0.0001;
                intersection[1] += shadowRayVect[1]*0.0001;
                intersection[2] += shadowRayVect[2]*0.0001;

                g = spheres[t]->findIntersectionWithRay(intersection,shadowRayVect);

                if(0<g && g<1){
                    shadowFlag = false;
                    break;

                }

            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////Shadow Ray Check with Triangles ///////////////////////////////////////////////////////////
            if(shadowFlag){
                for(int c = 0 ; c < triangles.size() ; c++){


                    float intersection[3];
                    intersection[0] = (myA);
                    intersection[1] = (myB);
                    intersection[2] = (myC);

                    float shadowRayVect[3];

                    shadowRayVect[0] = pointLights[w]->positionVector[0] - intersection[0];
                    shadowRayVect[1] = pointLights[w]->positionVector[1] - intersection[1];
                    shadowRayVect[2] = pointLights[w]->positionVector[2] - intersection[2];

                    intersection[0] += shadowRayVect[0]*0.0001;
                    intersection[1] += shadowRayVect[1]*0.0001;
                    intersection[2] += shadowRayVect[2]*0.0001;

                    g = triangles[c]->findIntersectionWithRay(intersection,shadowRayVect);

                    if(0<g && g<1){
                        shadowFlag = false;
                        break;
                    }

                }
            }
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if(shadowFlag){
                //////////////////////////////////////////////////////////////////////Triangle Diffuse/////////////////////////////////////////////////////////////////////////////////

                float distFromLight = sqrt(((pointLights[w]->positionVector[0]-(myA))*(pointLights[w]->positionVector[0]-(myA)))
                        +((pointLights[w]->positionVector[1]-(myB))*(pointLights[w]->positionVector[1]-(myB)))
                        +((pointLights[w]->positionVector[2]-(myC))*(pointLights[w]->positionVector[2]-(myC))));
                float myD = 4 * M_PI * distFromLight*distFromLight;

                float I1 = pointLights[w]->intensity[0]/(myD);
                float I2 = pointLights[w]->intensity[1]/(myD);
                float I3 = pointLights[w]->intensity[2]/(myD);

                float constant = triangles[index]->normal[0]*(pointLights[w]->positionVector[0]-(myA))/distFromLight+
                        triangles[index]->normal[1]*(pointLights[w]->positionVector[1]-(myB))/distFromLight+
                        triangles[index]->normal[2]*(pointLights[w]->positionVector[2]-(myC))/distFromLight;
                if(constant<0)
                    constant = 0;

                //cout << (I1*materials[q]->diffuse[0])*constant << " " << (I2*materials[q]->diffuse[1])*constant << " " << (I3*materials[q]->diffuse[2])*constant << endl;

                pixelColour.RGB[0] += (I1*materials[q]->diffuse[0])*constant;
                pixelColour.RGB[1] += (I2*materials[q]->diffuse[1])*constant;
                pixelColour.RGB[2] += (I3*materials[q]->diffuse[2])*constant;
                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////////////////Triangle Specular ////////////////////////////////////////////////////////////////////////////////////
                float hPluslX = (pointLights[w]->positionVector[0]-(myA))/distFromLight +
                        -1*(newRay->temp*newRay->direction[0])/sqrt(newRay->temp*newRay->direction[0]*newRay->temp*newRay->direction[0] +
                        newRay->temp*newRay->direction[1]*newRay->temp*newRay->direction[1] +
                        newRay->temp*newRay->direction[2]*newRay->temp*newRay->direction[2]);

                float hPluslY = (pointLights[w]->positionVector[1]-(myB))/distFromLight +
                        -1*(newRay->temp*newRay->direction[1])/sqrt(newRay->temp*newRay->direction[0]*newRay->temp*newRay->direction[0] +
                        newRay->temp*newRay->direction[1]*newRay->temp*newRay->direction[1] +
                        newRay->temp*newRay->direction[2]*newRay->temp*newRay->direction[2]);

                float hPluslZ = (pointLights[w]->positionVector[2]-(myC))/distFromLight +
                        -1*(newRay->temp*newRay->direction[2])/sqrt(newRay->temp*newRay->direction[0]*newRay->temp*newRay->direction[0] +
                        newRay->temp*newRay->direction[1]*newRay->temp*newRay->direction[1] +
                        newRay->temp*newRay->direction[2]*newRay->temp*newRay->direction[2]);

                float dist = sqrt(hPluslX*hPluslX+hPluslY*hPluslY+hPluslZ*hPluslZ);

                hPluslX /= dist;
                hPluslY /= dist;
                hPluslZ /= dist;

                float constant1 = triangles[index]->normal[0]*hPluslX + triangles[index]->normal[1]*hPluslY + triangles[index]->normal[2]*hPluslZ;

                if(constant1<0)
                    constant1 = 0;

                //cout<< constant1 << endl;
                //cout << pixelColour.RGB[0] << " "<< pixelColour.RGB[1] << " "<< pixelColour.RGB[2] << endl;
                pixelColour.RGB[0] += (I1*materials[q]->specular[0])*pow(constant1,materials[q]->specular[3]);
                pixelColour.RGB[1] += (I2*materials[q]->specular[1])*pow(constant1,materials[q]->specular[3]);
                pixelColour.RGB[2] += (I3*materials[q]->specular[2])*pow(constant1,materials[q]->specular[3]);
                //cout << pixelColour.RGB[0] << " "<< pixelColour.RGB[1] << " "<< pixelColour.RGB[2] << endl;


                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            }
        }
        //cout << triangles[index]->normal[0] << " " << triangles[1]->normal[1] << " " << triangles[index]->normal[2] << endl;
        //colours.push_back(pixelColour);

        //bigData[dataIndex] = pixelColour.RGB[0];
        //bigData[dataIndex+1] = pixelColour.RGB[1];
        //bigData[dataIndex+2] = pixelColour.RGB[2];
        //////////////////////////////////////////////////////////////////////////////////////////Triangle Reflection !!! /////////////////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //delete newRay;
        // Epsilonsuz !
        if(reflectionCount>0){

            Ray * newRayRec = new Ray();

            float d1 = sqrt(newRay->direction[0]*newRay->direction[0]+newRay->direction[1]*newRay->direction[1]+newRay->direction[2]*newRay->direction[2]);




            // Normalize D = given ray ! !
            float rayX = newRay->direction[0]/(d1);
            float rayY = newRay->direction[1]/(d1);
            float rayZ = newRay->direction[2]/(d1);


            recNormal[0] = triangles[index]->normal[0];
            recNormal[1] = triangles[index]->normal[1];
            recNormal[2] = triangles[index]->normal[2];

            float dot1 = (rayX*recNormal[0]+rayY*recNormal[1]+rayZ*recNormal[2]);


            newRayRec->direction[0] = rayX - 2*dot1*recNormal[0];
            newRayRec->direction[1] = rayY - 2*dot1*recNormal[1];
            newRayRec->direction[2] = rayZ - 2*dot1*recNormal[2];
            newRayRec->temp = FLT_MAX;

            cameraX = myA;
            cameraY = myB;
            cameraZ = myC;

            cameraX +=  newRayRec->direction[0]*0.0001;
            cameraY +=  newRayRec->direction[1]*0.0001;
            cameraZ +=  newRayRec->direction[2]*0.0001;

            Colour reflected = calculateColour(newRayRec,reflectionCount-1);

            pixelColour.RGB[0] += reflected.RGB[0]*materials[q]->reflectance[0];
            pixelColour.RGB[1] += reflected.RGB[1]*materials[q]->reflectance[1];
            pixelColour.RGB[2] += reflected.RGB[2]*materials[q]->reflectance[2];

            delete newRayRec;

        }


        return pixelColour;
    }
    // Its a sphere Use k
    else if(flag == 0){
        unsigned int q = 0;
        // cout << q << endl;
        for(q = 0; q < materials.size() ;q++){
            if(materials[q]->materialID == spheres[index]->materialID)
                break;
        }

        //cout << index << endl;
        //cout << newRay->temp << endl;
        Colour pixelColour;
        pixelColour.RGB[0] = materials[q]->ambient[0]*ambientLight[0];
        pixelColour.RGB[1] = materials[q]->ambient[1]*ambientLight[1];
        pixelColour.RGB[2] = materials[q]->ambient[2]*ambientLight[2];

        float recNormal[3];
        float myA;
        float myB;
        float myC;

        for(int w = 0; w < pointLights.size() ;w++ ){
            float g = -1; // intersection with shadowRay initial value !
            bool shadowFlag  = true;
            // Epsilon is currently 0.0001 !
            float intersection[3];
            myA = cameraX + newRay->temp*newRay->direction[0];
            myB = cameraY + newRay->temp*newRay->direction[1];
            myC = cameraZ + newRay->temp*newRay->direction[2];
            intersection[0] = (myA);
            intersection[1] = (myB);
            intersection[2] = (myC);

            //////////////////////////////////////Shadow Ray Check with Spheres !////////////////////////////////////////////////////////////////////////////////////
            for(int t = 0 ; t < spheres.size() ; t++){

                float shadowRayVect[3];

                shadowRayVect[0] = pointLights[w]->positionVector[0] - intersection[0];
                shadowRayVect[1] = pointLights[w]->positionVector[1] - intersection[1];
                shadowRayVect[2] = pointLights[w]->positionVector[2] - intersection[2];



                intersection[0] += shadowRayVect[0]*0.0001;
                intersection[1] += shadowRayVect[1]*0.0001;
                intersection[2] += shadowRayVect[2]*0.0001;

                g = spheres[t]->findIntersectionWithRay(intersection,shadowRayVect);

                if(g != -1){
                    shadowFlag = false;
                    break;

                }


            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////Shadow Ray Check with Triangles ///////////////////////////////////////////////////////////////////////////////////////
            if(shadowFlag){
                for(int c = 0 ; c < triangles.size() ; c++){


                    float intersection[3];
                    intersection[0] = (myA);
                    intersection[1] = (myB);
                    intersection[2] = (myC);

                    float shadowRayVect[3];

                    shadowRayVect[0] = pointLights[w]->positionVector[0] - intersection[0];
                    shadowRayVect[1] = pointLights[w]->positionVector[1] - intersection[1];
                    shadowRayVect[2] = pointLights[w]->positionVector[2] - intersection[2];

                    intersection[0] += shadowRayVect[0]*0.0001;
                    intersection[1] += shadowRayVect[1]*0.0001;
                    intersection[2] += shadowRayVect[2]*0.0001;

                    g = triangles[c]->findIntersectionWithRay(intersection,shadowRayVect);

                    if(0<g && g<1){
                        shadowFlag = false;
                        break;
                    }

                }
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if(shadowFlag){
                ///////////////////////////////////////////////////////////////////Sphere Diffuse !////////////////////////////////////////////////////////////////////////////////////
                float distFromLight = sqrt(((pointLights[w]->positionVector[0]-(myA))*(pointLights[w]->positionVector[0]-(myA)))
                        +((pointLights[w]->positionVector[1]-(myB))*(pointLights[w]->positionVector[1]-(myB)))
                        +((pointLights[w]->positionVector[2]-(myC))*(pointLights[w]->positionVector[2]-(myC))));

                float myD = 4 * M_PI * distFromLight*distFromLight;

                float I1 = pointLights[w]->intensity[0]/(myD);
                float I2 = pointLights[w]->intensity[1]/(myD);
                float I3 = pointLights[w]->intensity[2]/(myD);

                //cout << vertexes[j]->coordinates[0] <<" " <<vertexes[j]->coordinates[1]<< " " << vertexes[j]->coordinates[2] << endl;
                spheres[index]->normal[0] = (myA-spheres[index]->sCenter[0])/spheres[index]->radius;
                spheres[index]->normal[1] = (myB-spheres[index]->sCenter[1])/spheres[index]->radius;
                spheres[index]->normal[2] = (myC-spheres[index]->sCenter[2])/spheres[index]->radius;

                float constant = spheres[index]->normal[0]*(pointLights[w]->positionVector[0]-(myA))/distFromLight+
                        spheres[index]->normal[1]*(pointLights[w]->positionVector[1]-(myB))/distFromLight+
                        spheres[index]->normal[2]*(pointLights[w]->positionVector[2]-(myC))/distFromLight;
                if(constant<0)
                    constant = 0;
                //cout << constant << endl;
                //cout << (I1*materials[q]->diffuse[0])*constant << " " << (I2*materials[q]->diffuse[1])*constant << " " << (I3*materials[q]->diffuse[2])*constant << endl;


                pixelColour.RGB[0] += (I1*materials[q]->diffuse[0])*constant;
                pixelColour.RGB[1] += (I2*materials[q]->diffuse[1])*constant;
                pixelColour.RGB[2] += (I3*materials[q]->diffuse[2])*constant;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //////////////////////////////////////////////////////////////////Sphere Specular ! /////////////////////////////////////////////////////////////////////////////////////////////////////////

                float hPluslX = (pointLights[w]->positionVector[0]-(myA))/distFromLight +
                        -1*(newRay->temp*newRay->direction[0])/sqrt(newRay->temp*newRay->direction[0]*newRay->temp*newRay->direction[0] +
                        newRay->temp*newRay->direction[1]*newRay->temp*newRay->direction[1] +
                        newRay->temp*newRay->direction[2]*newRay->temp*newRay->direction[2]);

                float hPluslY = (pointLights[w]->positionVector[1]-(myB))/distFromLight +
                        -1*(newRay->temp*newRay->direction[1])/sqrt(newRay->temp*newRay->direction[0]*newRay->temp*newRay->direction[0] +
                        newRay->temp*newRay->direction[1]*newRay->temp*newRay->direction[1] +
                        newRay->temp*newRay->direction[2]*newRay->temp*newRay->direction[2]);

                float hPluslZ = (pointLights[w]->positionVector[2]-(myC))/distFromLight +
                        -1*(newRay->temp*newRay->direction[2])/sqrt(newRay->temp*newRay->direction[0]*newRay->temp*newRay->direction[0] +
                        newRay->temp*newRay->direction[1]*newRay->temp*newRay->direction[1] +
                        newRay->temp*newRay->direction[2]*newRay->temp*newRay->direction[2]);

                float dist = sqrt(hPluslX*hPluslX+hPluslY*hPluslY+hPluslZ*hPluslZ);

                hPluslX /= dist;
                hPluslY /= dist;
                hPluslZ /= dist;

                float constant1 = spheres[index]->normal[0]*hPluslX + spheres[index]->normal[1]*hPluslY + spheres[index]->normal[2]*hPluslZ;

                if(constant1<0)
                    constant1 = 0;

                //cout<< constant1 << endl;
                //cout << pixelColour.RGB[0] << " "<< pixelColour.RGB[1] << " "<< pixelColour.RGB[2] << endl;
                pixelColour.RGB[0] += (I1*materials[q]->specular[0])*pow(constant1,materials[q]->specular[3]);
                pixelColour.RGB[1] += (I2*materials[q]->specular[1])*pow(constant1,materials[q]->specular[3]);
                pixelColour.RGB[2] += (I3*materials[q]->specular[2])*pow(constant1,materials[q]->specular[3]);
                //cout << pixelColour.RGB[0] << " "<< pixelColour.RGB[1] << " "<< pixelColour.RGB[2] << endl;

                /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


            }
        }

        if(reflectionCount>0){

            Ray * newRayRec = new Ray();

            float d1 = sqrt(newRay->direction[0]*newRay->direction[0]+newRay->direction[1]*newRay->direction[1]+newRay->direction[2]*newRay->direction[2]);




            // Normalize D = given ray ! !
            float rayX = newRay->direction[0]/(d1);
            float rayY = newRay->direction[1]/(d1);
            float rayZ = newRay->direction[2]/(d1);


            recNormal[0] = ((cameraX + newRay->temp*newRay->direction[0])-spheres[index]->sCenter[0])/spheres[index]->radius;
            recNormal[1] = ((cameraY + newRay->temp*newRay->direction[1])-spheres[index]->sCenter[1])/spheres[index]->radius;
            recNormal[2] = ((cameraZ + newRay->temp*newRay->direction[2])-spheres[index]->sCenter[2])/spheres[index]->radius;

            float dot1 = (rayX*recNormal[0]+rayY*recNormal[1]+rayZ*recNormal[2]);


            newRayRec->direction[0] = rayX - 2*dot1*recNormal[0];
            newRayRec->direction[1] = rayY - 2*dot1*recNormal[1];
            newRayRec->direction[2] = rayZ - 2*dot1*recNormal[2];
            newRayRec->temp = FLT_MAX;

            cameraX = myA;
            cameraY = myB;
            cameraZ = myC;

            cameraX +=  newRayRec->direction[0]*0.0001;
            cameraY +=  newRayRec->direction[1]*0.0001;
            cameraZ +=  newRayRec->direction[2]*0.0001;

            Colour reflected = calculateColour(newRayRec,reflectionCount-1);

            delete newRayRec;

            pixelColour.RGB[0] += reflected.RGB[0]*materials[q]->reflectance[0];
            pixelColour.RGB[1] += reflected.RGB[1]*materials[q]->reflectance[1];
            pixelColour.RGB[2] += reflected.RGB[2]*materials[q]->reflectance[2];


        }
        //cout << spheres[index]->normal[0] << " " << spheres[1]->normal[1] << " " << spheres[index]->normal[2] << endl;
        //colours.push_back(pixelColour);

        /*bigData[dataIndex] = pixelColour.RGB[0];
bigData[dataIndex+1] = pixelColour.RGB[1];
bigData[dataIndex+2] = pixelColour.RGB[2];
delete pixelColour;*/
        //delete newRay;
        // Epsilonsuz !

        return pixelColour;
    }
    else{
        Colour pixelColour;

        pixelColour.RGB[0] = background[0];
        pixelColour.RGB[1] = background[1];
        pixelColour.RGB[2] = background[2];
        //colours.push_back(pixelColour);
        //delete pixelColour;
        //bigData[dataIndex] = background[0];
        //bigData[dataIndex+1] = background[1];
        //bigData[dataIndex+2] = background[2];
        //delete newRay;
        // Epsilonsuz !
        return pixelColour;
    }

    //        }
    //dataIndex+=3; // when from recursion don't increase it !

    //cout << bigData[dataIndex-3] << " " << bigData[dataIndex-2] << " "<<bigData[dataIndex-1] << endl;
    //delete newRay;

}










int main(int argc, char* argv[])
{
    //
    // Test XML parsing
    //
    bool result = parseSceneXML(argv[1]);

    if (result)
    {
        //std::cout << "Scene file parse successfully" << std::endl;
    }
    else
    {
        //std::cout << "ERROR parsing the scene file" << std::endl;
    }
    
    int arg1 = 0;
    int arg2 = 0;
    int loop1=0;
    int triangleLoop = 0;
    int sphereLoop = 0;



    //cout << cameras.size() << " " << cameras[0]->nx << " " << cameras[0]->ny << endl ;
    for(triangleLoop = 0 ; triangleLoop < triangles.size();triangleLoop++){
        bool check1 = false;
        bool check2 = false;
        bool check3 = false;
        for(loop1 = 0;loop1<vertexes.size();loop1++){

            if(vertexes[loop1]->vertexID == triangles[triangleLoop]->vertices[0]){
                triangles[triangleLoop]->side1[0]=vertexes[loop1]->coordinates[0];
                triangles[triangleLoop]->side1[1]=vertexes[loop1]->coordinates[1];
                triangles[triangleLoop]->side1[2]=vertexes[loop1]->coordinates[2];
                check1 = true;

            }
            if(vertexes[loop1]->vertexID == triangles[triangleLoop]->vertices[1]){
                triangles[triangleLoop]->side2[0]=vertexes[loop1]->coordinates[0];
                triangles[triangleLoop]->side2[1]=vertexes[loop1]->coordinates[1];
                triangles[triangleLoop]->side2[2]=vertexes[loop1]->coordinates[2];
                check2 = true;

            }
            if(vertexes[loop1]->vertexID == triangles[triangleLoop]->vertices[2]){

                triangles[triangleLoop]->side3[0]=vertexes[loop1]->coordinates[0];
                triangles[triangleLoop]->side3[1]=vertexes[loop1]->coordinates[1];
                triangles[triangleLoop]->side3[2]=vertexes[loop1]->coordinates[2];
                check3 = true;

            }
            if(check1 && check2 && check3){

                float fp[3];
                fp[0]= (triangles[triangleLoop]->side2[1]-triangles[triangleLoop]->side1[1])*(triangles[triangleLoop]->side3[2]-triangles[triangleLoop]->side1[2]) - (triangles[triangleLoop]->side2[2]-triangles[triangleLoop]->side1[2])*(triangles[triangleLoop]->side3[1]-triangles[triangleLoop]->side1[1]);
                fp[1]= (triangles[triangleLoop]->side2[2]-triangles[triangleLoop]->side1[2])*(triangles[triangleLoop]->side3[0]-triangles[triangleLoop]->side1[0]) - (triangles[triangleLoop]->side2[0]-triangles[triangleLoop]->side1[0])*(triangles[triangleLoop]->side3[2]-triangles[triangleLoop]->side1[2]);
                fp[2]= (triangles[triangleLoop]->side2[0]-triangles[triangleLoop]->side1[0])*(triangles[triangleLoop]->side3[1]-triangles[triangleLoop]->side1[1]) - (triangles[triangleLoop]->side2[1]-triangles[triangleLoop]->side1[1])*(triangles[triangleLoop]->side3[0]-triangles[triangleLoop]->side1[0]);
                //cout << fp[0]<< endl;
                // Used for the height of the normal vector !

                float var1 = sqrt(fp[0]*fp[0]+fp[1]*fp[1]+fp[2]*fp[2]);

                triangles[triangleLoop]->normal[0] = fp[0]/var1;
                triangles[triangleLoop]->normal[1] = fp[1]/var1;
                triangles[triangleLoop]->normal[2] = fp[2]/var1;

/*cout << triangles[triangleLoop]->side1[0] << " " ;
cout << triangles[triangleLoop]->side1[1] << " ";
cout << triangles[triangleLoop]->side1[2] << endl;

cout <<                triangles[triangleLoop]->side2[0] << " ";
cout <<                triangles[triangleLoop]->side2[1] << " ";
cout <<                triangles[triangleLoop]->side2[2] << endl;

cout <<                triangles[triangleLoop]->side3[0] << " ";
cout<<                triangles[triangleLoop]->side3[1] << " ";
cout<<                triangles[triangleLoop]->side3[2] << endl;
*/

                break;

            }

        }
    }

    for(sphereLoop = 0 ; sphereLoop < spheres.size();sphereLoop++){
        for(loop1 = 0;loop1<vertexes.size();loop1++){
            if(vertexes[loop1]->vertexID == spheres[sphereLoop]->center){
                spheres[sphereLoop]->sCenter[0] =vertexes[loop1]->coordinates[0];
                spheres[sphereLoop]->sCenter[1] =vertexes[loop1]->coordinates[1];
                spheres[sphereLoop]->sCenter[2] =vertexes[loop1]->coordinates[2];
                break;

            }
        }
    }






    for(unsigned int i = 0 ; i < cameras.size(); i++){

        arg1 = cameras[i]->nx;
        arg2 = cameras[i]->ny;

        bigData = new float[3*arg1*arg2];
        dataIndex = 0;


        float pixelWidth = (cameras[i]->right - cameras[i]->left) / cameras[i]->nx;
        float pixelHeight = (cameras[i]->top - cameras[i]->bottom) / cameras[i]->ny;

        float aValue = cameras[i]->left + pixelWidth/2 ;
        float bValue = cameras[i]->top - pixelHeight/2;

        //cout << aValue << " "<<bValue << endl;



        float distance = cameras[i]->distance*-1; // -distance Z value of the d vector in the r(t) = o + t * d

        //cout << cameras[0]->uVector[0] << " "<< cameras[0]->uVector[1] << " "<< cameras[0]->uVector[2] <<endl;
        //cout << cameras[0]->uVector[1] << " "<< cameras[0]->upVector[1] << " "<< (cameras[0]->gazeVector[1]*-1) <<endl;
        //cout << cameras[0]->uVector[2] << " "<< cameras[0]->upVector[2] << " "<< (cameras[0]->gazeVector[2]*-1) <<endl;

        // First While for the loop for changing between rows of pixels
        while(bValue > cameras[i]->bottom){

            // Second While for the loop  for changing between columns of pixels
            while(aValue < cameras[i]->right){

                // TODO : arguman olarak paslayabilirsin ? biraz daha hiz olur !
                cameraX = cameras[i]->positionVector[0];
                cameraY = cameras[i]->positionVector[1];
                cameraZ = cameras[i]->positionVector[2];

                Ray * newRay = new Ray();
                newRay->direction[0] = (aValue)*(cameras[i]->uVector[0]) + (bValue)*(cameras[i]->upVector[0]) + (distance)*(cameras[i]->gazeVector[0]*-1);
                newRay->direction[1] = (aValue)*(cameras[i]->uVector[1]) + (bValue)*(cameras[i]->upVector[1]) + (distance)*(cameras[i]->gazeVector[1]*-1);
                newRay->direction[2] = (aValue)*(cameras[i]->uVector[2]) + (bValue)*(cameras[i]->upVector[2]) + (distance)*(cameras[i]->gazeVector[2]*-1);
                newRay->temp = FLT_MAX;

                //cout << newRay->direction[0] << " " << newRay->direction[1] << " " << newRay->direction[2] << endl;

                Colour cp=calculateColour(newRay,rayReflectionCount);

                delete newRay;

                bigData[dataIndex] = cp.RGB[0];
                bigData[dataIndex+1] = cp.RGB[1];
                bigData[dataIndex+2] = cp.RGB[2];

                dataIndex+=3;

                //cout << endl;
                aValue +=  pixelWidth;


            }
            aValue = cameras[i]->left + pixelWidth/2 ;
            bValue -= pixelHeight;

        }
        /// Submit öncesi gox u sil !!!!!!!! ////////
        string gox = cameras[i]->outputFileName;
        writePPM(gox.c_str(), arg1,arg2,bigData);
        delete bigData;
    }
    /*cout << materials[0]->materialID << " " << endl;
    //cout << spheres[0]->sphereID << " " << endl;
    cout << triangles[0]->triangleID << " " << endl;
    cout << cameras[0]->cameraID << " " << endl;
    cout << cameras[0]->outputFileName << " " << endl;
    cout << pointLights[0]->pointLightID << " " << endl;
    cout << vertexes[0]->vertexID << " " << endl;
    cout << ambientLight[0] << " " << background[0] << " " << rayReflectionCount << endl;*/
    //
    // Test PPM write
    //
    /*float data[] = {
        255, 0, 0,       0, 255, 0,    0, 0, 255,
        255, 255, 0,   255, 0, 255,    0, 255, 255,
    };*/



    return 0;
}

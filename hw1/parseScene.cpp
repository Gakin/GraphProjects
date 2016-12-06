#include <iostream>
#include <cstdio>
#include "tinyXML/tinyxml.h"
#include <vector>
#include "Material.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Vertex.h"
#include "Camera.h"
#include "PointLight.h"

using namespace std;

// To Be Externed Values !
extern vector<Material*> materials;
extern vector<Sphere*> spheres;
extern vector<Triangle*> triangles;
extern vector<Camera*> cameras;
extern vector<PointLight*> pointLights;
extern vector<Vertex*> vertexes;

float ambientLight[3];
float background[3];
int rayReflectionCount;

bool parseSceneXML(const char* filename)
{
	TiXmlDocument doc(filename);
	bool loadOkay = doc.LoadFile();

	if (!loadOkay)
	{
		std::cout << "Could not load file: " << filename << "Error = " << doc.ErrorDesc() << std::endl;
		return false;
	}

	TiXmlNode* pRoot = doc.FirstChild("Scene");
	for (TiXmlNode* pNode = pRoot->FirstChild(); pNode; pNode = pNode->NextSibling())
	{
        if (pNode->Value() == std::string("Material"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // get material index

            //
            // read reflectance coefficients
            //
            float amb[3], dif[3], spe[3], mir[3];
			float phongExp;
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Ambient"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &amb[0], &amb[1], &amb[2]);
				}
				else if (pChild->Value() == std::string("Diffuse"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &dif[0], &dif[1], &dif[2]);
				}
				else if (pChild->Value() == std::string("Specular"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f %f", &spe[0], &spe[1], &spe[2], &phongExp);
				}
				else if (pChild->Value() == std::string("Reflectance"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &mir[0], &mir[1], &mir[2]);
				}
			}
			// Saving The New Material Pushing it To the Vector !
			Material * newMaterial = new Material();
			newMaterial->materialID = index;
			newMaterial->ambient[0] = amb[0];
			newMaterial->ambient[1] = amb[1];
			newMaterial->ambient[2] = amb[2];
			newMaterial->diffuse[0] = dif[0];
			newMaterial->diffuse[1] = dif[1];
			newMaterial->diffuse[2] = dif[2];
			newMaterial->specular[0] = spe[0];
			newMaterial->specular[1] = spe[1];
			newMaterial->specular[2] = spe[2];
			newMaterial->specular[3] = phongExp;
			newMaterial->reflectance[0] = mir[0];
			newMaterial->reflectance[1] = mir[1];
			newMaterial->reflectance[2] = mir[2];
			materials.push_back(newMaterial);
            //
            // TODO: Save the scanned values for the current material in your data structures.
            //

        }
		else if (pNode->Value() == std::string("Vertex"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // get vertex index

			float coords[3];
			TiXmlNode* pChild = pNode->FirstChild();
			sscanf(pChild->FirstChild()->Value(), "%f %f %f", &coords[0], &coords[1], &coords[2]);
			// Creating new Vertex pushing it to the vertexes vector
			Vertex * newVertex = new Vertex();
			newVertex->vertexID = index;
			newVertex->coordinates[0] = coords[0];
			newVertex->coordinates[1] = coords[1];
			newVertex->coordinates[2] = coords[2];
			vertexes.push_back(newVertex);
            //
            // TODO: Save the scanned values for the current vertex in your data structures.
            //

		}
		else if (pNode->Value() == std::string("Triangle"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // get triangle index

			int vIndex[3], mIndex;
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Vertices"))
				{
					sscanf(pChild->FirstChild()->Value(), "%d %d %d", &vIndex[0], &vIndex[1], &vIndex[2]);
				}
				else if (pChild->Value() == std::string("MaterialId"))
				{
					sscanf(pChild->FirstChild()->Value(), "%d", &mIndex);
				}
			}
			// Creating a New Triangle and pushing it back to the Triangles Vector 
			Triangle * newTriangle = new Triangle();
			newTriangle->triangleID = index;
			newTriangle->materialID = mIndex;
			newTriangle->vertices[0] = vIndex[0];
			newTriangle->vertices[1] = vIndex[1];
			newTriangle->vertices[2] = vIndex[2];
			triangles.push_back(newTriangle);
            //
            // TODO: Save the scanned values for the current triangle in your data structures.
            //


		}
		else if (pNode->Value() == std::string("Sphere"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // Sphere index

			int vIndex, mIndex;
			float rad;
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Center"))
				{
					sscanf(pChild->FirstChild()->Value(), "%d", &vIndex);
				}
				else if (pChild->Value() == std::string("MaterialId"))
				{
					sscanf(pChild->FirstChild()->Value(), "%d", &mIndex);
				}
				else if (pChild->Value() == std::string("Radius"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f", &rad);
				}
			}
			Sphere * newSphere = new Sphere();
			newSphere->sphereID = index;
			newSphere->center = vIndex;
			newSphere->materialID = mIndex;
			newSphere->radius = rad;
			spheres.push_back(newSphere);
            //
            // TODO: Save the scanned values for the current sphere in your data structures.
            //

		}
		else if (pNode->Value() == std::string("PointLight"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // Light index

			float pos[3], intensity[3];
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Position"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &pos[0], &pos[1], &pos[2]);
				}
				else if (pChild->Value() == std::string("Intensity"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &intensity[0], &intensity[1], &intensity[2]);
				}
			}
			// Creating a New PointLight and pushing it back to the PointLights vector 
			PointLight * newPointLight = new PointLight();
			newPointLight->pointLightID = index;
			newPointLight->positionVector[0] = pos[0];
			newPointLight->positionVector[1] = pos[1];
			newPointLight->positionVector[2] = pos[2];
			newPointLight->intensity[0] = intensity[0];
			newPointLight->intensity[1] = intensity[1];
			newPointLight->intensity[2] = intensity[2];
			pointLights.push_back(newPointLight);
            //
            // TODO: Save the scanned values for the current point light in your data structures.
            //
		}
		else if (pNode->Value() == std::string("AmbientLight"))
        {
			float intensity[3];
			TiXmlNode* pChild = pNode->FirstChild();
			sscanf(pChild->Value(), "%f %f %f", &intensity[0], &intensity[1], &intensity[2]);
			
			ambientLight[0] = intensity[0];
			ambientLight[1] = intensity[1];
			ambientLight[2] = intensity[2];
            //
            // TODO: Save the scanned values for the ambient light in your data structures.
            //
		}
		else if (pNode->Value() == std::string("BackgroundColor"))
        {
            float bgColor[3];
			TiXmlNode* pChild = pNode->FirstChild();
			sscanf(pChild->Value(), "%f %f %f", &bgColor[0], &bgColor[1], &bgColor[2]);

			background[0] = bgColor[0];
			background[1] = bgColor[1];
			background[2] = bgColor[2];
            //
            // TODO: Save the scanned values for the background color in your data structures.
            //
		}
		else if (pNode->Value() == std::string("RayReflectionCount"))
        {
            int rayReflectCount;
			TiXmlNode* pChild = pNode->FirstChild();
			sscanf(pChild->Value(), "%d", &rayReflectCount);

			rayReflectionCount = rayReflectCount;
            //
            // TODO: Save the scanned values for the ray reflection count in your data structures.
            //
		}
		else if (pNode->Value() == std::string("Camera"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // Camera index

			float gaze[3], up[3], pos[3];
			float left, right, bottom, top, distance;
			int nx, ny;
			std::string imageName;
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Position"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &pos[0], &pos[1], &pos[2]);
				}
				else if (pChild->Value() == std::string("Gaze"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &gaze[0], &gaze[1], &gaze[2]);
				}
				else if (pChild->Value() == std::string("Up"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &up[0], &up[1], &up[2]);
				}
				else if (pChild->Value() == std::string("ImagePlane"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f %f %f %d %d", &left, &right, &bottom, &top, &distance, &nx, &ny);
				}
				else if (pChild->Value() == std::string("OutputName"))
				{
					imageName = pChild->FirstChild()->Value();
				}
			}
			// Creating a newCamera and pushing it to the Cameras vector 
			Camera * newCamera = new Camera();
			newCamera->cameraID = index;
			newCamera->gazeVector[0] = gaze[0];
			newCamera->gazeVector[1] = gaze[1];
			newCamera->gazeVector[2] = gaze[2];
			newCamera->upVector[0] = up[0];
			newCamera->upVector[1] = up[1];
			newCamera->upVector[2] = up[2];
			newCamera->positionVector[0] = pos[0];
			newCamera->positionVector[1] = pos[1];
			newCamera->positionVector[2] = pos[2];
            newCamera->uVector[0] = up[1]*(gaze[2]*-1) - up[2]*(gaze[1]*-1);
            newCamera->uVector[1] = up[2]*(gaze[0]*-1) - up[0]*(gaze[2]*-1);
            newCamera->uVector[2] = up[0]*(gaze[1]*-1) - up[1]*(gaze[0]*-1);
			newCamera->left = left;
			newCamera->right = right;
			newCamera->top = top;
			newCamera->bottom = bottom;
			newCamera->nx = nx;
			newCamera->ny = ny;
			newCamera->distance = distance;
			newCamera->outputFileName = imageName;
			cameras.push_back(newCamera);
            //
            // TODO: Save the scanned values for the current camera in your data structures.
            //

		}
	}

    return true;
}

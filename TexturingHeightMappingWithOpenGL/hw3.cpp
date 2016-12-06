#define GLM_FORCE_RADIANS
#include <string>
#include <iostream>
#include <math.h>
#include <vector>
#include <GL/gl.h>   
#include <GL/glut.h>
#include "SOIL.h"
#include "glinit.h"
#include "utils.h"
#include "glentry.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// In order to handle the error mentioned for HW2.
#include <pthread.h>
void junk() {
  int i;
  i=pthread_getconcurrency();
};


int lat;
int lon;
int z = 0;
int texIndex = 0;
int norIndex = 0;
int indexIndex = 0;
bool flag = true;

using namespace std;
using namespace glm;

GLuint myIndexes;

GLuint myVertexes;

GLuint myTextures;

GLuint myNormals;

GLuint gHeightEarth;

GLuint gTextureEarth;

GLuint gProgramEarth;

GLuint gTextureSun;

GLuint gProgramSun;

GLuint gProgramCubeMap;					// program handler of the cube map with two attached shaders: CubeMap.vert, CubeMap.frag

GLuint gTextureCubeMap;					// texture handler for the cube map. 

GLuint gVerticesVBO, gIndicesVBO;		// Vertex Buffer Objects for the cubemap's indices and vertices.

GLuint gEarthVerts;                     // Vertex Buffer Objects for the earth !

GLfloat gDispLevel = 0;					// displacement level for the height map. Can be passed to the shader source
struct Point{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
    	float cx;
    	float cy;
};

Point **points;
vector<float> myData;
vector<float> texData;
vector<float> normalData;
vector<int> indexData;

// Camera's properties
float gCamAngle = 0.0f;
float gCamDirX  = 0.0f;
float gCamDirZ  = 0.0f;
float gCamDist  = 5.0f;

// Earth's properties
float gEarthAngle	 = 0.0f;			// rotation angle of the Earth around its own axis
float gEarthAngleSun = 0.0f;			// rotation angle of the Earth around Sun
float gEarthPosX     = 0.0f;
float gEarthPosZ     = -1.0f;

void reshape(int w, int h) 
{
    w = w < 1 ? 1 : w;
    h = h < 1 ? 1 : h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, double(w)/h, 0.1f, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
		
void mouse(int button, int state, int x, int y)
{
    if (button == 3 && state == GLUT_UP)		 	// mouse wheel down
   		gCamDist -= 0.1f;
	else if (button == 4 && state == GLUT_UP)    	// mouse wheel up
    	gCamDist += 0.1f;
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key) 
    {
        case 27:	// Escape
            exit(0); 
            break; 
        case 'a' :	// rotate camera around the Earth clockwise
			gCamAngle -= 0.01f;
			break;
		case 'd' :	// rotate camera around the Earth counterclockwise
			gCamAngle += 0.01f;
			break;
		case '-':	// decrease displacement level of the height map
			gDispLevel -= 0.01f;
			break;
		case '+':	// increase displacement level of the height map
			gDispLevel += 0.01f;
			break;
        default:  
            break;
    }
}

void init(int *argc, char** argv) 
{
    	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);

	glShadeModel(GL_SMOOTH);
	
	initTextures();
    	initShaders();      

	if (*argc < 3) {
		cout << "Missing argument(s)." << endl 
			 << "Usage: ./hw3 <# of Latitudes> <# of Longitudes> <radius of the Earth>" << endl;
			 
		exit(0);	
	}
	
	// initialize Cubemap
	initCubeMap(&gVerticesVBO, &gIndicesVBO);
    
	// read command line arguments
	float radius  = atof(argv[--*argc]);
	int   num_lon = atoi(argv[--*argc]);
	int   num_lat = atoi(argv[--*argc]);
		
    // TODO: initialize the Earth.
  
    points = new Point*[num_lat+1];
    for(int k = 0;k<num_lat+1;k++){
    points[k] = new Point[num_lon+1];
    }
	
    lon = num_lon;
    lat = num_lat;

    float phi ;
    float theta ;

    float incPhi = 1.0/num_lat;
    float incTheta = 1.0/num_lon;

    float u=0;
    float v=0;
    for(int i = 0; i <= num_lat;i++){

        phi = i*incPhi;
	u = 1.0*i/num_lat;
        for(int j = 0; j <= num_lon;j++){
              theta = j*incTheta;
	      points[i][j].x = radius*sin(phi*M_PI)*cos(theta*2*M_PI);
  	      points[i][j].y = radius*cos(phi*M_PI);
	      points[i][j].z = radius*sin(phi*M_PI)*sin(theta*2*M_PI);
	      points[i][j].nx = sin(phi*M_PI)*cos(theta*2*M_PI);
  	      points[i][j].ny = cos(phi*M_PI);
	      points[i][j].nz = sin(phi*M_PI)*sin(theta*2*M_PI);
	       v = 1.0*j/num_lon;

          points[i][j].cx=u;
          points[i][j].cy=1-v;
        }

    }



	indexIndex = 0;
   for(int i = 0; i < num_lat;i++){
        for(int j = 0; j < num_lon;j++){
            myData.push_back(points[i][j].x);
            myData.push_back(points[i][j].y);
            myData.push_back(points[i][j].z);
            texData.push_back(points[i][j].cy);
            texData.push_back(points[i][j].cx);
            normalData.push_back(points[i][j].nx);
            normalData.push_back(points[i][j].ny);
            normalData.push_back(points[i][j].nz);
            indexData.push_back(indexIndex);

            myData.push_back(points[i+1][j].x);
            myData.push_back(points[i+1][j].y);
            myData.push_back(points[i+1][j].z);
            texData.push_back(points[i+1][j].cy);
            texData.push_back(points[i+1][j].cx);
            normalData.push_back(points[i+1][j].nx);
            normalData.push_back(points[i+1][j].ny);
            normalData.push_back(points[i+1][j].nz);
            indexData.push_back(indexIndex+1);

            myData.push_back(points[i][j+1].x);
            myData.push_back(points[i][j+1].y);
            myData.push_back(points[i][j+1].z);
            texData.push_back(points[i][j+1].cy);
            texData.push_back(points[i][j+1].cx);
            normalData.push_back(points[i][j+1].nx);
            normalData.push_back(points[i][j+1].ny);
            normalData.push_back(points[i][j+1].nz);
            indexData.push_back(indexIndex+2);

            myData.push_back(points[i+1][j].x);
            myData.push_back(points[i+1][j].y);
            myData.push_back(points[i+1][j].z);
            texData.push_back(points[i+1][j].cy);
            texData.push_back(points[i+1][j].cx);
            normalData.push_back(points[i+1][j].nx);
            normalData.push_back(points[i+1][j].ny);
            normalData.push_back(points[i+1][j].nz);
            indexData.push_back(indexIndex+3);

            myData.push_back(points[i+1][j+1].x);
            myData.push_back(points[i+1][j+1].y);
            myData.push_back(points[i+1][j+1].z);
            texData.push_back(points[i+1][j+1].cy);
            texData.push_back(points[i+1][j+1].cx);
            normalData.push_back(points[i+1][j+1].nx);
            normalData.push_back(points[i+1][j+1].ny);
            normalData.push_back(points[i+1][j+1].nz);
            indexData.push_back(indexIndex+4);


            myData.push_back(points[i][j+1].x);
            myData.push_back(points[i][j+1].y);
            myData.push_back(points[i][j+1].z);
            texData.push_back(points[i][j+1].cy);
            texData.push_back(points[i][j+1].cx);
            normalData.push_back(points[i][j+1].nx);
            normalData.push_back(points[i][j+1].ny);
            normalData.push_back(points[i][j+1].nz);
            indexData.push_back(indexIndex+5);

            indexIndex+=6;

		}
	}

	
    	glGenBuffers(1, &myTextures);
	glGenBuffers(1, &myNormals);
	
    glBindBuffer(GL_ARRAY_BUFFER, myNormals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normalData.size(),normalData.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	
    glBindBuffer(GL_ARRAY_BUFFER, myTextures);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*texData.size(),texData.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glGenBuffers(1, &myVertexes);
	glBindBuffer(GL_ARRAY_BUFFER, myVertexes);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*myData.size(),myData.data(),GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &myIndexes);	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIndexes);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indexData.size(),indexData.data(),GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

void display()
{	
    long startTime = getCurrentTime();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	
	glLoadIdentity();
	
	// calculations for the position of the Earth
	gEarthAngleSun += 0.002739f;
	gEarthPosX = sin(gEarthAngleSun) * 50;
	gEarthPosZ = cos(gEarthAngleSun) * 50;

	// calculations for the position and view direction of the camera
	gCamDirX = sin(gCamAngle);
	gCamDirZ = cos(gCamAngle);

	gluLookAt(gEarthPosX + gCamDirX*gCamDist, 0, gEarthPosZ + gCamDirZ*gCamDist, 
			  gEarthPosX                    , 0, gEarthPosZ, 
              0.0f						    , 1, 0.0f);


	// Stars -- Cubemap
    glPushMatrix();
		glScalef(300,100,300);
		glUseProgram(gProgramCubeMap);
		
		glBindBuffer(GL_ARRAY_BUFFER, gVerticesVBO);
		glVertexPointer(3, GL_FLOAT, sizeof(float) * 3, 0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndicesVBO);
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT,  0);
    glPopMatrix();

	// Earth
	glPushMatrix();
	gEarthAngle += 1;
	glTranslatef(gEarthPosX, 0, gEarthPosZ);
	glRotatef(23.5f, 0, 0, 1);
	glRotatef(gEarthAngle, 0, 1, 0);

        // TODO: Display the Earth
        //glBindBuffer(GL_ARRAY_BUFFER, gEarthVerts);
	
        glUseProgram(gProgramEarth);

	mat4  translated = translate(vec3(gEarthPosX,0,gEarthPosZ));
	mat4  rotated1   = rotate(radians(23.5f),vec3(0,0,1));
	mat4  rotated2   = rotate(radians(gEarthAngle), vec3(0,1,0));
	mat4  myM        = translated*rotated1*rotated2;
	mat4  inverseMyM = inverse(myM);
	mat4  transIMyM  = transpose(inverseMyM);

	GLuint uMyM = glGetUniformLocation(gProgramEarth,"myM");
	glUniformMatrix4fv( uMyM, 1, GL_FALSE,value_ptr(myM));
	
	GLuint tIMyM = glGetUniformLocation(gProgramEarth,"tMyM");
	glUniformMatrix4fv( tIMyM, 1, GL_FALSE,value_ptr(transIMyM));	
	
	glUniform1i(glGetUniformLocation(gProgramEarth,"image"),0);
	glUniform1i(glGetUniformLocation(gProgramEarth,"image2"),1);
	
	GLint uniformVarLocation = glGetUniformLocation(gProgramEarth, "change"); 
	glUniform1f(uniformVarLocation, gDispLevel); 
 	
	GLint uniformVarLocation2 = glGetUniformLocation(gProgramEarth, "cameraPosX"); 
	glUniform1f(uniformVarLocation2, gEarthPosX+gCamDirX*gCamDist);
	
	GLint uniformVarLocation3 = glGetUniformLocation(gProgramEarth, "cameraPosY"); 
	glUniform1f(uniformVarLocation3, 0);
	
	GLint uniformVarLocation4 = glGetUniformLocation(gProgramEarth, "cameraPosZ"); 
	glUniform1f(uniformVarLocation4, gEarthPosZ+gCamDirZ*gCamDist);

	
	
	//glClientActiveTexture(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
		
	glBindBuffer(GL_ARRAY_BUFFER,myVertexes);
	glVertexPointer(3, GL_FLOAT, sizeof(float) * 3,0);
	
	glBindBuffer(GL_ARRAY_BUFFER,myNormals);
	glNormalPointer(GL_FLOAT, sizeof(float) * 3, 0);

	glBindBuffer(GL_ARRAY_BUFFER,myTextures);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 2, 0);
		
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIndexes);

	glDrawElements(GL_TRIANGLES, indexData.size(),GL_UNSIGNED_INT,0);

	glPopMatrix();
	
	// Sun
	glPushMatrix();
        glUseProgram(gProgramSun); // 0 means using no program
        //glClientActiveTexture(GL_TEXTURE2);
        glUniform1i(glGetUniformLocation(gProgramSun,"sunText"),2);
		GLUquadricObj* sun = gluNewQuadric();
		gluQuadricDrawStyle(sun,GLU_FILL);
		gluQuadricTexture(sun,GL_TRUE);
		gluQuadricNormals(sun,GLU_SMOOTH);
        	//glColor3f(1,1,0);
		gluSphere(sun,15, 20, 20);
        // BONUS: Add some nice shader effects, textures etc. for the Sun
	
	glPopMatrix();
	

	glutSwapBuffers();
	
	// Set FPS to 100
	long diff = getCurrentTime() - startTime;
	if (diff < 10000)
        usleep(10000 - diff);
}

int main(int argc, char** argv)   
{	
    // Hack. Handle the error OpenGL - NVidia consistency error.
	junk();
	
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("20141 - CEng477 - HW3");

    init_glentry();
    init(&argc, argv);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(glutPostRedisplay);

    glutMainLoop();

    return 0;
}


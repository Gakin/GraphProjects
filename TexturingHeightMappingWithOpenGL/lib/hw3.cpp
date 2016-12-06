#include <string>
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "SOIL.h"
#include "glinit.h"
#include "utils.h"
#include "glentry.h"

// In order to handle the error mentioned for HW2.
#include <pthread.h>
void junk() {
  int i;
  i=pthread_getconcurrency();
};


int lat;
int lon;
int z = 0;
using namespace std;

GLuint gProgramEarth;

GLuint gProgramCubeMap;					// program handler of the cube map with two attached shaders: CubeMap.vert, CubeMap.frag

GLuint gTextureCubeMap;					// texture handler for the cube map.

GLuint gTextureEarth;

GLuint gVerticesVBO, gIndicesVBO;		// Vertex Buffer Objects for the cubemap's indices and vertices.

GLuint gEarthVerts;                     // Vertex Buffer Objects for the earth !

GLfloat gDispLevel = 0;					// displacement level for the height map. Can be passed to the shader source
struct Point{
    float x;
    float y;
    float z;
};

Point **points;
float * myData;

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
    //earthCoords = new float*[num_lon];

    /*for(int i = 0; i < num_lon;i++){
        earthCoords[i]= new float[num_lat*3];
    }*/
    myData = new float[num_lon*num_lat*3*6];

    points = new Point*[num_lon];
    for(int k = 0;k<num_lon;k++){
    points[k] = new Point[num_lat];
    }

    lon = num_lon;
    lat = num_lat;

    float phi ;
    float theta ;

    float incPhi = 180.0/(num_lat-1);
    float incTheta = 360.0/num_lon;



    for(int i = 0; i < num_lat;i++){

        phi = i*incPhi;

        for(int j = 0; j < num_lon;j++){
            theta = j*incTheta;
          points[i][j].x = radius*sin((phi/180)*M_PI)*cos((theta/(180))*M_PI);
          points[i][j].y = radius*cos((phi/180)*M_PI);
          points[i][j].z = radius*sin((phi/180)*M_PI)*sin((theta/(180))*M_PI);
//            myData[z]   = radius*sin((phi/180)*M_PI)*cos((theta/(180))*M_PI);
//            myData[z+2] = radius*sin((phi/180)*M_PI)*sin((theta/(180))*M_PI);
//            myData[z+1] = radius*cos((phi/180)*M_PI);

            //if(radius>sqrt(myData[z]*myData[z]+myData[z+1]*myData[z+1]+myData[z+2]*myData[z+2]))

        }

    }
    std::cout <<num_lon*num_lat*3*6<<std::endl;
    z = 0;
    for(int i = 0; i < num_lat-1;i++){
        for(int j = 0; j < num_lon-1;j++){
            myData[z]=points[i%lat][j%lon].x;
            myData[z+1]=points[i%lat][j%lon].y;
            myData[z+2]=points[i%lat][j%lon].z;

	    myData[z+3]=points[i%lat][(j+1)%lon].x;
            myData[z+4]=points[i%lat][(j+1)%lon].y;
            myData[z+5]=points[i%lat][(j+1)%lon].z;
            
            myData[z+6]=points[(i+1)%lat][j%lon].x;
            myData[z+7]=points[(i+1)%lat][j%lon].y;
            myData[z+8]=points[(i+1)%lat][j%lon].z;

            myData[z+9]=points[(i+1)%lat][j%lon].x;
            myData[z+10]=points[(i+1)%lat][j%lon].y;
            myData[z+11]=points[(i+1)%lat][j%lon].z;

            myData[z+12]=points[(i+1)%lat][(j+1)%lon].x;
            myData[z+13]=points[(i+1)%lat][(j+1)%lon].y;
            myData[z+14]=points[(i+1)%lat][(j+1)%lon].z;

            myData[z+15]=points[i%lat][(j+1)%lon].x;
            myData[z+16]=points[i%lat][(j+1)%lon].y;
            myData[z+17]=points[i%lat][(j+1)%lon].z;

            z+=18;
            std::cout<< z << std::endl;
        }
    }



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
              0.0f						    , 1, 0.0f);     /*glGenBuffers(1, &gEarthVerts);

    glBindBuffer(GL_ARRAY_BUFFER, gEarthVerts);
    glBufferData(GL_ARRAY_BUFFER, num_lat*num_lon*3*8, myData, GL_STATIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    */


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
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glUseProgram(gProgramEarth);
        glVertexPointer(3, GL_FLOAT, 0, myData);
        glDrawArrays(GL_LINES, 0, lat*lon*6);
    glPopMatrix();

    // Sun
    glPushMatrix();
        glUseProgram(0); // 0 means using no program
        glColor3f(1,1,0);
        glutSolidSphere(1, 20, 20);
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


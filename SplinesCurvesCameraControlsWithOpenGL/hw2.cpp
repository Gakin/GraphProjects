#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cfloat>
#include <cmath>



/////////////////////////////////////// Initial Camera Mode:Transformation////////////////////////////////////////////////////

int cameraMode = 1;
int myWindow;

int paletteNum;
float colorScale;
float max = FLT_MIN;
float min = FLT_MAX;
int contRow = 0;
int contCol = 0;
float averX = 0;
float averY = 0;
float averZ = 0;

bool isFirst = false;
bool realFirst = true;

float prevMX = 0;
float prevMY = 0;
float changeInX = 0;
float changeInY = 0;


//////////Camera Positon !!!! ////////////////////////////
float cameraX = 100;
float cameraY = 100;
float cameraZ = 100;

float alfa = 0;
float beta = 0;
float gama = 0;

float distance = 0;

int TX = 0;
int TY = 0;
int TZ = 0;

int RY = 0;

int MRX = 0;
int MRY = 0;

int selectedX = 0;
int selectedY = 0;

struct Point{
    float x,y,z;
};

struct Color{
    int r,g,b;
};



Point** points;                 //Control Points
Point *** beztPoints;           //Points from bezier funciton
Color * colors;                 //Color Palette
int glRow;
int glColumn;

int factorial(int a){
    if(a == 0)
        return 1;
    else if(a == 1)
        return 1;
    else
        return factorial(a-1)*a;

}

float comb(int a , int b){

    return (factorial(a)/(factorial(b)*factorial(a-b)));

}

float myPow(float a,int b){

    if(b == 0){
        return 1;
    }
    else{
        return myPow(a,b-1)*a;
    }
}

float bernstein(int n,int i,float u){

    return (comb(n,i)*myPow(u,i)*myPow((1-u),(n-i)));

}

float returnColor(float y){

    if(max==min)
        return 1;
    if(paletteNum == 1)
        return 0;
    float k = (y-min);
    float units = (max-min)/(paletteNum-1);
    return 0+k/units;
}

void clearAll(){ /////////////To Clear Bezt Points Array
    for(int i = 0; i < glRow ; i++){
        for(int j = 0; j < glColumn ; j++){
            delete beztPoints[i][j];
        }
    }
    for(int i = 0; i < glRow;i++){
        delete beztPoints[i];
    }
    delete beztPoints;
}

void noNeedNew(){

    averX=0;averY=0;averZ = 0;
    max = FLT_MIN;
    min = FLT_MAX;
    for(int row = 0; row < glRow ; row++){
        for(int column = 0 ; column < glColumn ; column++){
            beztPoints[row][column]->x = 0;
            beztPoints[row][column]->y = 0;
            beztPoints[row][column]->z = 0;
        }
    }

    for(int row = 0; row < glRow ; row++){
        for(int column = 0 ; column < glColumn ; column++){
            //std::cout << ((float)row/(samplerR-1)) << " " << ((float)column)/(samplerC-1) << std::endl;
            for(int i = 0; i < contRow ; i++){
                for(int j = 0; j < contCol ; j++){
                    //std::cout<< bernstein(M-1,i,(float)row/(samplerR-1)) << std::endl;
                    beztPoints[row][column]->x += bernstein(contRow-1,i,(float)row/(glRow-1))*bernstein(contCol-1,j,((float)column)/(glColumn-1))*points[i][j].x;
                    beztPoints[row][column]->y += bernstein(contRow-1,i,(float)row/(glRow-1))*bernstein(contCol-1,j,((float)column)/(glColumn-1))*points[i][j].y;
                    beztPoints[row][column]->z += bernstein(contRow-1,i,(float)row/(glRow-1))*bernstein(contCol-1,j,((float)column)/(glColumn-1))*points[i][j].z;
                }
            }
            averX += beztPoints[row][column]->x;
            averY += beztPoints[row][column]->y;
            averZ += beztPoints[row][column]->z;
            if(max < beztPoints[row][column]->y)
                max = beztPoints[row][column]->y;
            if(min > beztPoints[row][column]->y)
                min = beztPoints[row][column]->y;
            //std::cout << newPoint->x << " " << newPoint->y << " "<< newPoint->z << std::endl;
        }
    }
    averX/=glRow*glColumn;averY/=glRow*glColumn;averZ /= glRow*glColumn;

}

////////////////////////////////////////////////////////Calculate new Bezier//////////////////////////////////////////////////
void myBezier(){
    averX=0;averY=0;averZ = 0;
    beztPoints = new Point**[glRow];

    max = FLT_MIN;
    min = FLT_MAX;

    for(int j = 0; j < glRow ; j++)
            beztPoints[j] = new Point*[glColumn];

    for(int row = 0; row < glRow ; row++){
        for(int column = 0 ; column < glColumn ; column++){
            Point * newPoint = new Point();/// if initially not zero make it zero !!!

            //std::cout << ((float)row/(samplerR-1)) << " " << ((float)column)/(samplerC-1) << std::endl;
            for(int i = 0; i < contRow ; i++){
                for(int j = 0; j < contCol ; j++){
                    //std::cout<< bernstein(M-1,i,(float)row/(samplerR-1)) << std::endl;
                    newPoint->x += bernstein(contRow-1,i,(float)row/(glRow-1))*bernstein(contCol-1,j,((float)column)/(glColumn-1))*points[i][j].x;
                    newPoint->y += bernstein(contRow-1,i,(float)row/(glRow-1))*bernstein(contCol-1,j,((float)column)/(glColumn-1))*points[i][j].y;
                    newPoint->z += bernstein(contRow-1,i,(float)row/(glRow-1))*bernstein(contCol-1,j,((float)column)/(glColumn-1))*points[i][j].z;
                }
            }
            beztPoints[row][column] = newPoint;

            if(max < newPoint->y)
                max = newPoint->y;
            if(min > newPoint->y)
                min = newPoint->y;

            averX += beztPoints[row][column]->x;
            averY += beztPoints[row][column]->y;
            averZ += beztPoints[row][column]->z;
            //std::cout << newPoint->x << " " << newPoint->y << " "<< newPoint->z << std::endl;
        }
    }
    averX/=glRow*glColumn;averY/=glRow*glColumn;averZ /= glRow*glColumn;
}
//////////////////////////////////////////////////////////Camera Change Mode Check///////////////////////////////////////////////
////////////////////////
///                                Mouse Left Click Check !!!

void mySpecial(int key,int x,int y){
    if(cameraMode == 2){
        if(key == GLUT_KEY_UP){
            //std::cout<<"WOLF"<<std::endl;
            if(selectedX < contRow-1)
                selectedX++;
        }
        else if(key == GLUT_KEY_DOWN){
            if(selectedX > 0)
               selectedX--;
        }
        else if(key == GLUT_KEY_LEFT){
            if(selectedY > 0)
               selectedY--;
        }
        else if(key == GLUT_KEY_RIGHT){
            if(selectedY < contCol-1)
                selectedY++;
        }
    }

    glutPostRedisplay();

}

void myGlutKey (unsigned char key, int x, int y)
{

    if(key == '1')
        cameraMode=1;
    else if(key == '2')
        cameraMode=2;
    if (cameraMode == 1){
        // Transformation Mode !!!!
        if(key == 'w')
             TX++;
        else if(key == 'q')
             TX--;
        else if(key == 'a')
             TY++;
        else if(key == 's')
             TY--;
        else if(key == 'z')
             TZ++;
        else if(key == 'x')
             TZ--;
        else if(key == 'r')
            RY++;
        else if(key == 't')
            RY--;
    }
    else if (cameraMode == 2){
        // surface Edit Mode !!!
        if(key == 'w'){
             points[selectedX][selectedY].x++;
             noNeedNew();
        }
        else if(key == 'q'){
             points[selectedX][selectedY].x--;
             noNeedNew();
        }
        else if(key == 'a'){
             points[selectedX][selectedY].y++;
             noNeedNew();
        }
        else if(key == 's'){
             points[selectedX][selectedY].y--;
             noNeedNew();
        }
        else if(key == 'z'){
             points[selectedX][selectedY].z++;bool isFirst = false;
             noNeedNew();
        }
        else if(key == 'x'){
             points[selectedX][selectedY].z--;
             noNeedNew();
        }
       else if(key == 'i'){
             clearAll();
             if(glRow > 2)
                glRow /= 2;
             myBezier();
        }
        else if(key == 'o'){
            clearAll();
            glRow *= 2;
            myBezier();
        }
        else if(key == 'k'){
            clearAll();
            if(glColumn > 2)
                glColumn /= 2;
            myBezier();
        }
        else if(key == 'l'){
            clearAll();
            glColumn *= 2;
            myBezier();
        }


    }



    glutPostRedisplay();


}

void myMB(int myButton, int myState, int x, int y)
{
  // Respond to mouse button presses.
  // If button1 pressed, mark this state so we know in motion function.
    if(myButton == GLUT_LEFT_BUTTON){
        isFirst = true;
    }

}

void calculateNewCoords(){


}

void myMouseMotion(int x,int y){
    //std::cout << x<< "-"<<y <<std::endl;
    if(isFirst){
        isFirst =false;
        changeInX =0;
        changeInY =0;
        prevMX = x;
        prevMY = y;
        glutPostRedisplay();
        return;
    }
    changeInX = x - prevMX;
    changeInY = y - prevMY;
    prevMX = x;
    prevMY = y;
    if(changeInX < 0)
        alfa+=0.005;
    else if(changeInX > 0)
        alfa-=0.005;

    if(changeInY < 0)
        beta+=0.005;
    else if(changeInY > 0)
        beta-=0.005;
    cameraX = distance*sin(alfa)*cos(beta);
    cameraY = distance*sin(alfa)*sin(beta);
    cameraZ = distance*cos(alfa);
   /* float a = cameraX;
    float b = cameraY;
    float c = cameraZ;*/
    //cameraX+= changeInX;
    //cameraY+= changeInY;
    /*float newZ = sqrt(distance*distance-(cameraX*cameraX+cameraY*cameraY));

    cameraX = a + changeInX/sqrt((newZ-c)*(newZ-c)+(changeInX)*(changeInX)+(changeInY)*(changeInY));
    cameraY = b + changeInY/sqrt((newZ-c)*(newZ-c)+(changeInX)*(changeInX)+(changeInY)*(changeInY));*/

    glutPostRedisplay();
}
//////////////////////////////////////////////////////// Camera Adjustment ////////////////////////////////////////////////////
void myCameraInit(void)
{
    // Any initialization before the main loop of GLUT goes here

    glEnable(GL_DEPTH_TEST);	// Enable depth Buffering
    glEnable(GL_COLOR_MATERIAL);	// Enable color tracking
    glEnable(GL_NORMALIZE);	// Enables vector normalization (optional)
    glClearColor( 0.4f, 0.4f, 0.8f, 1.0f  ); 	// Set initial value of color buffer (Set background color also)
    glPointSize(10);

    // Lighting initialization

    glDisable(GL_LIGHTING);



    // Initialize camera
    glMatrixMode(GL_PROJECTION);	// Switch  to projection matrix
    glLoadIdentity();	// Clear current matrix to identity matrix
    gluPerspective(60, 1 , 1 , 1000000);	// Set projection of camera (You can modify the arguments if needed.) They could change!!!

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
///////////////////////////////////////////////////////Adjustment Done ////////////////////////////////////////////////////////

//////////////////////////////////////////////////////Display The Image///////////////////////////////////////////////////////
void sceneDisplay( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// Clear color buffer with glClearColor() RGB values
    glLoadIdentity();

    /////////////////////////// Will be changed to global variables but initial values are given default for now ///////////////////////////

    ///////////////////////////////////////// Will change accoring to mouse movement when left button is being pressed ////////////////////

    gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0); // Set camera position and direction
    ////////////////////////////////////////////////Checks for camera mode will be done in here ////////////////////////////////////////////
    /// \brief glTranslatef
    //std:: cout << averX << " "<< averY << " " << averZ << std::endl;
    //std::cout << averX << " " << averY << " "<< averZ << std::endl;
    glTranslatef(TX,TY,TZ);
    glTranslatef(averX,averY,averZ); //// ORTA NOKTA DOGRU HESAPLA ! ///
    glRotatef(RY,0,1,0);
    glTranslatef(-averX,-averY,-averZ);
    glBegin(GL_TRIANGLES);

    for(int i = 0; i < glRow-1 ; i++){
        for(int j = 0 ; j < glColumn-1 ; j++){

            float av1 = (beztPoints[i][j]->y+beztPoints[i][j+1]->y+beztPoints[i+1][j+1]->y)/3;
            int index1 = returnColor(av1);
            glColor3f(colors[index1].r/255.0,colors[index1].g/255.0,colors[index1].b/255.0);
            glVertex3f(beztPoints[i][j]->x,beztPoints[i][j]->y,beztPoints[i][j]->z);
            glVertex3f(beztPoints[i][j+1]->x,beztPoints[i][j+1]->y,beztPoints[i][j+1]->z);
            glVertex3f(beztPoints[i+1][j+1]->x,beztPoints[i+1][j+1]->y,beztPoints[i+1][j+1]->z);


            float av2 = (beztPoints[i][j]->y+beztPoints[i+1][j]->y+beztPoints[i+1][j+1]->y)/3;
            int index2 = returnColor(av2);
            glColor3f(colors[index2].r/255.0,colors[index2].g/255.0,colors[index2].b/255.0);
            glVertex3f(beztPoints[i][j]->x,beztPoints[i][j]->y,beztPoints[i][j]->z);
            glVertex3f(beztPoints[i+1][j]->x,beztPoints[i+1][j]->y,beztPoints[i+1][j]->z);
            glVertex3f(beztPoints[i+1][j+1]->x,beztPoints[i+1][j+1]->y,beztPoints[i+1][j+1]->z);

           //std::cout << color1 << " t"<< color2 << std::endl;
        }
    }

    glEnd();

    if(cameraMode == 2){
        glBegin(GL_POINTS);
        for(int i = 0; i < contRow ; i++){
            for(int j = 0 ; j < contCol ; j++){
                if(selectedX != i || selectedY != j){
                    glColor3f(255,0,0);
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                }
                else{
                    glColor3f(0,255,0);
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                }
            }
        }
        glEnd();
    }


    glutSwapBuffers();	// Swap buffers in double buffered scene
}
/////////////////////////////////////////////////////////////Image Displayed ///////////////////////////////////////////


int main(int argc,char * argv[]){

    int samplerR,samplerC;
    int M,N;



    std::ifstream readFile;
    readFile.open(argv[1]);


    ////////////////////////////////////////////////////////////////////////Parse Begin ////////////////////////////////////////////////////////////////////////

    readFile >> samplerR;
    readFile >> samplerC;

    glRow = samplerR;
    glColumn = samplerC;
    //////////////////////////// M and N is not added plus 1 //////////////////////////////////////

    readFile >> M;
    readFile >> N;

    contRow = M;
    contCol = N;

    points = new Point*[M];

    for(int i = 0; i < M; i++)
        points[i] = new Point[N];

    for(int i = 0; i < M ; i++)
        for(int j = 0; j < N ; j++ ){
             readFile >> points[i][j].x;
             readFile >> points[i][j].y;
             readFile >> points[i][j].z;
        }

    readFile >> paletteNum;

    colors = new Color[paletteNum];

    for(int i = 0; i < paletteNum ; i++){
        int r,g,b ;
        readFile >> r;
        readFile >> g;
        readFile >> b;
        colors[i].r = r;
        colors[i].g = g;
        colors[i].b = b;
    }

    readFile.close();

    //////////////////////////////////////////////////////////////////Parse Finished ////////////////////////////////////////////////////////////////////////
    /*for(int i = 0; i < M+1 ; i++){

        for(int j = 0 ; j < N+1 ; j++){

            std::cout<< points[i][j].x << " " << points[i][j].y << " " << points[i][j].z << " ";

        }
        std::cout << std::endl;

    }/*
    for(int i = 0; i < paletteNum ; i++){

        std::cout << colors[i].r << " "<<colors[i].g << " " << colors[i].b << std::endl;

    }*/
    //cout << samplerR << " " << samplerC << endl;
    //cout << comb(5,2)<< endl;
    //cout << bernstein(4,2,0.5)<< endl;
    //cout << 6*0.5*0.5*0.5*0.5 << endl;
    //////////////////////////////////////////////Calculate summation bernstein Formla //////////////////////////////////////

    /* Point ** samplePoints = new Point*[samplerR+1];

    for(int i = 0; i < samplerR+1; i++)
        samplePoints[i] = new Point[samplerC+1];*/
    ////////////////////////////// A new loop will be needed ! ///////////////////////////////////////





    beztPoints = new Point**[samplerR];


    for(int j = 0; j < samplerR ; j++)
            beztPoints[j] = new Point*[samplerC];

    for(int row = 0; row < samplerR ; row++){
        for(int column = 0 ; column < samplerC ; column++){
            Point * newPoint = new Point();/// if initially not zero make it zero !!!
            //std::cout << ((float)row/(samplerR-1)) << " " << ((float)column)/(samplerC-1) << std::endl;
            for(int i = 0; i < M ; i++){
                for(int j = 0; j < N ; j++){
                    //std::cout<< bernstein(M-1,i,(float)row/(samplerR-1)) << std::endl;
                    newPoint->x += bernstein(M-1,i,(float)row/(samplerR-1))*bernstein(N-1,j,((float)column)/(samplerC-1))*points[i][j].x;
                    newPoint->y += bernstein(M-1,i,(float)row/(samplerR-1))*bernstein(N-1,j,((float)column)/(samplerC-1))*points[i][j].y;
                    newPoint->z += bernstein(M-1,i,(float)row/(samplerR-1))*bernstein(N-1,j,((float)column)/(samplerC-1))*points[i][j].z;
                }
            }
            beztPoints[row][column] = newPoint;
            if(max < newPoint->y)
                max = newPoint->y;
            if(min > newPoint->y)
                min = newPoint->y;
            averX += beztPoints[row][column]->x;
            averY += beztPoints[row][column]->y;
            averZ += beztPoints[row][column]->z;
            //std::cout << newPoint->x << " " << newPoint->y << " "<< newPoint->z << std::endl;
        }
    }
    averX/=glRow*glColumn;averY/=glRow*glColumn;averZ /= glRow*glColumn;

    distance=sqrt((cameraX*cameraX)+(cameraY*cameraY)+(cameraZ*cameraZ));
    alfa = acos(cameraZ*1.0/distance);
    beta = atan(cameraY*1.0/cameraX);

    cameraX = distance*sin(alfa)*cos(beta);
    cameraY = distance*sin(alfa)*sin(beta);
    cameraZ = distance*cos(alfa);

    /*alfa = M_PI/2-atan(cameraZ*1.0/sqrt((cameraX*cameraX)+(cameraY*cameraY))); ///phi
    beta = atan(cameraY*1.0/sqrt((cameraX*cameraX)+(cameraZ*cameraZ))); ///theta*/

    ///////////////////////////////////////////////////////Finished ///////////////////////////////////////////////////////
    //   Initialize GLUT and create window


    glutInit(&argc,argv);	// Glut initialization function
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );	// Set display mode
    glutInitWindowPosition( 50, 50 );	// Set window position
    glutInitWindowSize( 800, 800 );	// Set window size

    myWindow = glutCreateWindow( "mySurface :3" );	// Create new window with
    glutDisplayFunc( sceneDisplay );	// Set display function
    glutKeyboardFunc( myGlutKey );		// Set keyboard function
    glutSpecialFunc( mySpecial );       // Set arrow keys
    glutMotionFunc(myMouseMotion);
    glutMouseFunc(myMB);

    myCameraInit();	// Call defined init function
    glutMainLoop();	// Start main loop after initialization


    return 0;
}

#include "glinit.h"


void initTextures()
{	
	int width, height, channels = 1;
	int width2 , height2;
	// read images into memory to generate textures
	unsigned char* textureMapData = SOIL_load_image("res/texturemap.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char* heightMapData  = SOIL_load_image("res/heightmap.jpg", &width, &height, &channels, SOIL_LOAD_L);
	

	// TODO: Generate texture map and height map for the Earth
	/*
*/

	gTextureSun    = SOIL_load_OGL_texture("res/sun.jpg",3,0,SOIL_FLAG_MIPMAPS|SOIL_FLAG_INVERT_Y);
	glActiveTexture(GL_TEXTURE2);	
	glBindTexture(GL_TEXTURE_2D, gTextureSun);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glGenTextures(1, &gTextureEarth);
	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_2D, gTextureEarth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureMapData);
	SOIL_free_image_data(textureMapData);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	glGenTextures(1, &gHeightEarth);
	glActiveTexture(GL_TEXTURE1);
	
	glBindTexture(GL_TEXTURE_2D, gHeightEarth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, heightMapData);
	SOIL_free_image_data(heightMapData);
	
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	

	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, sunTextData);
	//SOIL_free_image_data(sunTextData);
	

	// Generate CubeMap
	glGenTextures(1, &gTextureCubeMap);
	glActiveTexture(GL_TEXTURE3);
	
	glBindTexture(GL_TEXTURE_2D, gTextureCubeMap);
	gTextureCubeMap = SOIL_load_OGL_single_cubemap("res/cubemap.jpg", "WNESUD", SOIL_LOAD_AUTO, gTextureCubeMap, SOIL_FLAG_MIPMAPS);
	
	printf("CubeMap Info: %s\n", result_string_pointer);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void initShaders()
{
	// Creation and initialization of CubeMap's shaders
	gProgramCubeMap = glCreateProgram();
    

    glAttachShader(gProgramCubeMap, createVS("res/cubemap.vert"));
    glAttachShader(gProgramCubeMap, createFS("res/cubemap.frag"));
    
     glUniform1i(glGetUniformLocation(gProgramCubeMap, "cubeMap")   , 3);
   
    glLinkProgram(gProgramCubeMap);

    

    gProgramEarth = glCreateProgram();
	glAttachShader(gProgramEarth, createVS("res/myEarth.vert"));
        glAttachShader(gProgramEarth, createFS("res/myEarth.frag"));       
	glLinkProgram(gProgramEarth);
    
     gProgramSun = glCreateProgram();
	glAttachShader(gProgramSun, createVS("res/mySun.vert"));
        glAttachShader(gProgramSun, createFS("res/mySun.frag"));
	   
	glLinkProgram(gProgramSun);
    //
	// TODO: Create and initialize new program(s) to use new shader(s)
}


void initCubeMap(GLuint *verticesVBO, GLuint *indicesVBO) 
{
	float vertices[] = {
		  1,  1,  1,				
		 -1,  1,  1,
		 -1, -1,  1,
		  1, -1,  1,
		  1, -1, -1,
		 -1, -1, -1,
		 -1,  1, -1,
		  1,  1, -1
	};

	GLuint indices[] = {
		0, 1, 2, 3,                 // Front face of the cube
		7, 4, 5, 6,                 // Back face of the cube
		6, 5, 2, 1,                 // Left face of the cube
		7, 0, 3, 4,                 // Right face of the cube
		7, 6, 1, 0,                 // Top face of the cube
		3, 2, 5, 4,                 // Bottom face of the cube
	};
	
	glGenBuffers(1, verticesVBO);
    	glGenBuffers(1, indicesVBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, *verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
}



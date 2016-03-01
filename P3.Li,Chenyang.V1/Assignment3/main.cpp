//=============================================================================
// Sample Application for GLEW, and cwc Application/Window class using freeglut
// Sample Application using files from clockwork coders
// Modified by Ross Maciejewski 2014
//=============================================================================

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include "glsl.h"
#include "perlin.h"
#include "perlin.c"

//-----------------------------------------------------------------------------

extern "C" unsigned char *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp);

class myWindow : public cwc::glutWindow
{
protected:
   cwc::glShaderManager SM;
   cwc::glShader *shader;
   cwc::glShader *shader2;
   cwc::glShader *shader3;
   cwc::glShader *shader4;
   cwc::glShader *shader5;
   cwc::glShader *shader6;
   cwc::glShader *shader7;
   cwc::glShader *shader8;
   cwc::glShader *shader9;
   cwc::glShader *shader10;
   cwc::glShader *shaderbase;
   
protected: 
	int scene;

	//Ross: Some variables I created for texturing
	unsigned char *image;
	unsigned char *image1;
	int imageWidth;
	int imageHeight;
	int bitDepth;
	bool default = true;
	GLuint texture;
	GLuint noise3DTexName;
	 GLubyte textureImage[512][512][3];
	GLubyte tex[128][128][3];

public:
	myWindow(){}
	float ex=0.0, ey=0.0, ez=4.0;//0.0f,0.0f,4.0f,
	float cx=0.0, cy=0.0, cz=-1.0;//0.0,0.0,-1.0,
	float angle = 0.0;
	float angle1 = 0.0;
	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Ross:  This is where I draw things.  I have three different scene examples.
		switch(scene)
		{
			case 1:
				Scene1();
				break;
			
		}
		
		glutSwapBuffers();
	}

	

	void Scene1(void)
	{
		glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	
	
       gluPerspective(45,1,1,100);
	   glMatrixMode(GL_MODELVIEW);
	 
	   glLoadIdentity();
	  
	   glRotatef(angle, 0, 1, 0);//cam rot
	   gluLookAt(ex, ey+1.0, ez,
		   cx, cy, cz,
		   0.0f, 1.0f, 0.0);
	   glPushMatrix();


	   if (shader7)shader7->begin();
	  
	   drawPlane();
	   if (shader7)shader7->begin();
	   
	   if (shader2)shader2->begin();
	
	   glTranslatef(1.0, 0.0, 0.0);
	   glBindTexture(GL_TEXTURE_2D, this->noise3DTexName);
	   
	   GLUquadricObj *quad5;
	   quad5 = gluNewQuadric();
	   gluQuadricTexture(quad5, GL_TRUE);

	   gluQuadricNormals(quad5, GL_SMOOTH);
	   
	   GLfloat material2_Ka[] = { 0.135, 0.2225, 0.1575, 0.95 };
	   GLfloat material2_Kd[] = { 0.54, 0.89, 0.63, 0.95 };
	   GLfloat material2_Ks[] = { 0.316228, 0.316228, 0.316228, 0.95 };
	   GLfloat  material2_Se = 12.8;

	   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material2_Se);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material2_Ka);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material2_Kd);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material2_Ks);

	   gluSphere(quad5, 1.0, 100, 100);//base
	   
	   if (shader2)shader2->end();


		if (shader9) shader9->begin();
			//Essentially I need to tell the shader what texture to use
		
		
		glTranslatef(0.0, 0.0, 0.95);

		GLUquadricObj *quad7;
		quad7 = gluNewQuadric();
		gluQuadricTexture(quad7, GL_TRUE);

		gluQuadricNormals(quad7, GL_SMOOTH);

		GLfloat material7_Ka[] = { 0.25, 0.20725, 0.20725, 0.922 };
		GLfloat material7_Kd[] = { 1.0, 0.829, 0.829, 0.922 };
		GLfloat material7_Ks[] = { 0.296648, 0.296648, 0.296648, 0.922 };
		GLfloat  material7_Se = 11.264;

		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material7_Se);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material7_Ka);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material7_Kd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material7_Ks);

		gluSphere(quad7, .1, 100, 100);

		//glutSolidSphere(.1, 100, 100);//button 1
		glTranslatef(0.0, 0.25, -0.03);


		glutSolidSphere(.1, 100, 100);//button 2
		glTranslatef(0.0, 0.25, -0.07);


		glutSolidSphere(.1, 100, 100);//button 3
		if (shader9) shader9->end();

		if (shader3)shader3->begin();
		glTranslatef(0.0, -.5, -0.85);
		glTranslatef(0.0, 1.3, 0.0);
		

		GLUquadricObj *quad4;
		quad4 = gluNewQuadric();
		gluQuadricTexture(quad4, GL_TRUE);
		GLfloat material_Ka[] = { 0.2295, 0.08825, 0.0275, 1.0 };
		GLfloat material_Kd[] = { 0.5508, 0.2118, 0.066, 1.0 };
		GLfloat material_Ks[] = { 0.580594, 0.223257, 0.0695701, 1.0 };
		GLfloat  material_Se = 51.2;

		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
		gluQuadricNormals(quad4, GL_SMOOTH);
		gluSphere(quad4, 0.5, 100, 100);//head
		//glutSolidSphere(.5, 100, 100);//head
		if (shader3) shader3->end();

		if (shader) shader->begin();
		glTranslatef(0.0, 0.0, 0.5);
		
		GLUquadricObj *quad6;
		quad6 = gluNewQuadric();
		gluQuadricTexture(quad6, GL_TRUE);
		GLfloat material5_Ka[] = { 0.19225, 0.19225, 0.19225, 1.0 };
		GLfloat material5_Kd[] = { 0.50754, 0.50754, 0.50754, 1.0 };
		GLfloat material5_Ks[] = { 0.508273, 0.508273, 0.508273, 1.0 };
		GLfloat  material5_Se = 51.2;

		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material5_Se);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material5_Ka);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material5_Kd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material5_Ks);

		gluQuadricNormals(quad6, GL_SMOOTH);
		gluCylinder(quad6, 0.05, 0.0, 0.3, 100, 100);//nose

		

		glTranslatef(0.0, 0.2, -0.5);
		glTranslatef(0.2, 0.0, 0.4);
		
		glutSolidSphere(.07, 100, 100);//left eye

		glTranslatef(-0.4, 0.0, 0.0);
		glutSolidSphere(.07, 100, 100);//right eye

		
		glPopMatrix();
		glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		glTranslatef(1.0, 0.0, 1.6);

		
		GLUquadricObj *quad3;
		quad3 = gluNewQuadric();
		gluQuadricTexture(quad3, GL_TRUE);
		GLfloat material1_Ka[] = { 0.329412, 0.223529, 0.027451, 1.0 };
		GLfloat material1_Kd[] = { 0.780392, 0.568627, 0.113725, 1.0 };
		GLfloat material1_Ks[] = { 0.992157, 0.941176, 0.807843, 1.0 };
		GLfloat  material1_Se = 27.8974;

		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material1_Se);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material1_Ka);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material1_Kd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material1_Ks);

		gluQuadricNormals(quad3, GL_SMOOTH);
		gluCylinder(quad3, 0.2, 0.0, 1.2, 100, 100);//cap
		
		


		if (shader) shader->end();
		
		if (shader6) shader6->begin();

		glPopMatrix();
		glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		glTranslatef(-1.0, 0.0, 1.6);
		
		GLUquadricObj *quad2;
		quad2 = gluNewQuadric();
		gluQuadricTexture(quad2, GL_TRUE);
		gluQuadricNormals(quad2, GL_SMOOTH);
		gluCylinder(quad2, 0.2, 0.0, 1.2, 100, 100);//cap
		if (shader6) shader6->end();

		if (shader5) shader5->begin();
		glPopMatrix();
		glTranslatef(-1.0, 0.0, 0.0);
	
		glRotatef(-90, 1, 0, 0);
		GLUquadricObj *quad;
		quad = gluNewQuadric();
		gluQuadricTexture(quad, GL_TRUE);

		gluQuadricNormals(quad, GL_SMOOTH);
		gluSphere(quad, 1.0, 100, 100);//base
		if (shader5)shader5->end();

		if (shader4)shader4->begin();
		glBindTexture(GL_TEXTURE_2D, this->texture);
		glRotatef(90, 1, 0, 0);
		glTranslatef(0.0, 1.3, 0.0);
		glRotatef(-90, 1, 0, 0);
		glRotatef(180, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		GLUquadricObj *quad1;
		quad1 = gluNewQuadric();
		gluQuadricTexture(quad1, GL_TRUE);
		gluQuadricNormals(quad1, GL_SMOOTH);
		gluSphere(quad1, 0.5, 100, 100);//head
		
		if (shader4)shader4->end();

		



		
	}

	
	

	void drawPlane(void)
	{
		
		glBegin(GL_QUADS);

		glNormal3f(0, 1, 0);
		
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-4.0, -1.0, -4.0);

		glTexCoord2f(1.0, 0.0);
		glVertex3f(-4.0, -1.0, 4.0);

		glTexCoord2f(1.0, 1.0);
		glVertex3f(4.0, -1.0, 4.0);

		glTexCoord2f(0.0, 1.0);
		glVertex3f(4.0, -1.0, -4.0);
		glEnd();
		
	}
	virtual void OnIdle() {}

	// When OnInit is called, a render context (in this case GLUT-Window) 
	// is already available!
	virtual void OnInit()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

		//Ross: Here is where I declare some variables you need for textures (not scene, that's for changing the scene)
		this->scene = 1;
		this->imageWidth = 128;
		this->imageHeight = 128;
		this->bitDepth = 24;

		//Ross:  This is where I set up the lights
		DemoLight();
		createPerlin();
		//Ross:  This is where I set up the textures
		DemoTexture();
		//BuildPopupMenu();  //This is going to be a function you write to initialize the menu
		//glutAttachMenu(GLUT_RIGHT_BUTTON);  //This attaches the menu to the right mouse click  button
		//Ross: The GLSL loaders initialization is already handled by the CWC files linked.
		//If you change the name of your shader you would need to modify here
		shader = SM.loadfromFile("vertexshader.txt","fragmentshader.txt"); // load (and compile, link) from file
		  if (shader==0) 
			  std::cout << "Error Loading, compiling or linking shader\n";
		 shader2 = SM.loadfromFile("vertexshader.txt", "baseshader.txt");
		  if (shader2 == 0)
			  std::cout << "Error Loading, compiling or linking shader\n";
		  shaderbase = SM.loadfromFile("vertexshader.txt", "baseshader.txt");
		  if (shaderbase == 0)
			  std::cout << "Error Loading, compiling or linking shader\n";
		  shader3 = SM.loadfromFile("vertexshader.txt", "headshader.txt");
		  if (shader3 == 0)
			  std::cout << "Error Loading, compiling or linking shader\n";
		  shader4 = SM.loadfromFile("vert.txt", "texture.txt");
		  if (shader4 == 0)
			  std::cout << "Error Loading, compiling or linking shader\n";
		  shader5 = SM.loadfromFile("vertexshader.txt", "baseshader2.txt");
		  if (shader5 == 0)
			  std::cout << "Error Loading, compiling or linking shader\n";
		  shader6 = SM.loadfromFile("vertexshader.txt", "capshader.txt");
		  if (shader6 == 0)
			  std::cout << "Error Loading, compiling or linking shader\n";
		  shader7 = SM.loadfromFile("vertexshader.txt", "ground.txt");
		  if (shader7 == 0)
			  std::cout << "Error Loading, compiling or linking shader\n";
		  shader8 = SM.loadfromFile("vertexshader.txt", "bumpmap.txt");
		  if (shader8 == 0)
			  std::cout << "Error Loading, compiling or linking shader\n";
		  shader9 = SM.loadfromFile("vertexshader.txt", "buttonshader.txt");
		  if (shader9 == 0)
			  std::cout << "Error Loading, compiling or linking shader\n"; 
		  shader10 = SM.loadfromFile("vertexshader.txt", "tex.txt");
		  if (shader10 == 0)
			  std::cout << "Error Loading, compiling or linking shader\n";
	}

	//Ross: Callbacks for most everything you need
	virtual void OnResize(int w, int h) {}
	virtual void OnClose(void){}
	virtual void OnMouseDown(int button, int x, int y) {}    
	virtual void OnMouseUp(int button, int x, int y) {}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y){}

	//Ross: This should be obvious, but these are the keyboard listeners
	virtual void OnKeyDown(int nKey, char cAscii)
	{       
		
		
		double s, c;
		s = sin(angle*3.1415926/180 );
		c = cos(angle* 3.1415926 / 180);
		if (cAscii == 27) // 0x1b = ESC
		{
			this->Close(); // Close Window!
		} 

		if(cAscii == '1')
			this->scene = 1;
		
		if (cAscii == 'b' || cAscii == 'B')
		{
			//printf("up");
			
				//shaderbase = shader2;
				shader2 = shader8;
				default = false;
			
			
		}
		if (cAscii == 'd'||cAscii == 'D')
		{
			//printf("up");

			if (default == false)
			    shader2 = shaderbase;



		}
		if (cAscii == 'p'||cAscii == 'P')
		{
			//printf("up");


			//shaderbase = shader2;
			shader2 = shader10;
			default = false;


		}
		if (cAscii == 'i' || cAscii == 'I')//UP
		{	
			
			ez -= 1.0*c;
			cz -= 1.0*c;
			ex -= -1.0*s;
			cx -= -1.0*s;
			
		}
		if (cAscii == 'k'||cAscii == 'K')//down
		{
			
			ez += 1.0*c;
			cz += 1.0*c;
			ex += -1.0*s;
			cx += -1.0*s;
		 
			
		}
		if (cAscii == 'j' || cAscii == 'J')//l
		{
			angle -= 10;
			if (angle == 360)angle = 0;
			//printf("%f", angle);
		}
		if (cAscii == 'l' || cAscii == 'L')//r
		{
			angle += 10;
			if (angle == 360)angle = 0;
			//printf("%f", angle);
		}

		
		glutPostRedisplay();
	};

	virtual void OnKeyUp(int nKey, char cAscii)
	{
		if (cAscii == 'f')
		{
			SetFullscreen(true);
		}
		else if (cAscii == 'w')
		{
			SetFullscreen(false);
		}
		
	};
	

	
	void createPerlin(void)
	{
		double perlin = 0.0, marbleA;

		for (int y = 0; y < 512; y++)
			for (int x = 0; x< 512; x++){
				for (int m = 1; m < 5; m++)
				{
					perlin += (PerlinNoise3D(5.0*x * pow(2, m) / 511.0, 5.0*y *pow(2, m) / 511.0,11.5,  2, 2, 1))/pow(2,m);
				}
				
				marbleA = sin(20.0*x/511.0 + 6.0 * perlin);
				
				if (marbleA < 0)marbleA = -marbleA;
				textureImage[x][y][0] = (GLubyte)(255*marbleA );
				textureImage[x][y][1] = (GLubyte)( 255 * marbleA);
				textureImage[x][y][2] = (GLubyte)( 255 * marbleA);
				perlin = 0.0;
				//printf("%ub\n", textureImage[x][y][0]);

			}

		


	}

	//Ross: This is where I set up all the textures
	void DemoTexture(void)
	{
		/* Load in a PNG image */
		int loadCorrectly = 0;
		//Use the stbi_image file to load an image
		image = stbi_load("kp.jpg", &this->imageWidth, &this->imageHeight, &this->bitDepth, 0);
		
		glGenTextures(1, &this->texture); //generate the texture with the loaded data

		//The first thing that must take place in the process of uploading the texture is a call to glBindTexture. 
		//What glBindTexture does is it tells OpenGL which texture "id" we will be working with. 
		//A texture "id" is just a number that you will use to access your textures. Here is a sample call.
		glBindTexture(GL_TEXTURE_2D, texture); //bind texture to its array


		//The glPixelStorei call tells OpenGL how the data that is going to be uploaded is aligned. 
		//This call tells OpenGL that the pixel data which is going to be passed to it is aligned in byte order, 
		//this means that the data has one byte for each component, one for red, green and blue
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
		//The glTexEnvf call sets environment variables for the current texture. 
		//What this does is tell OpenGL how the texture will act when it is rendered into a scene.
		//What this does is sets the active texture to GL_MODULATE. 
		//The GL_MODULATE attribute allows you to apply effects such as lighting and coloring to your texture. 
		//If you do not want lighting and coloring to effect your texture and you would like to display the texture unchanged when 
		//coloring is applied replace GL_MODULATE with GL_DECAL.
		//This is not needed if you use shaders!!
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		//The glTexImage2D call is our goal. 
		//This call will upload the texture to the video memory where it will be ready for us to use in our programs. 
		//I am going to explain this call parameter by parameter since it is so important to what we are doing.
		//target - The target of this call, it will always be GL_TEXTURE_2D.
		//level - The level of detail number, this should be left at 0 for our purposes. 
			//Once you become more adept at OpenGL texture mapping this parameter will be something that you might change.
		//internalformat - Internal components parameter. 
			//This tells OpenGL how many color components to represent internally from the texture that is uploaded. 
			//There are many symbolic constants for this parameter but the one which is most widely used is GL_RGB; this constant is equal to 3.
		//width & height - The width and height of the image data. These must be integers that are equal to 2n+2(border) for some integer n.
			//What this basically means is that the texture width and height must be a power of two (2,4,8,16,32,63,128,256,512, etc).
		//border - Image border, must be 0 or 1. I always use 0 in my code since I do not use image borders.
		//format - Format of the pixel data that will be uploaded. There are many constants which are accepted but GL_RGB is the value that is widely used.
		//type - Type of data that will be uploaded. Again there are several symbolic constants but the one which I use is GL_UNSIGNED_BYTE.
		//pixels - Pointer to the image data. 
			//This is the image data that will be uploaded to the video memory. 
			//Note that after your call to glTexImage2D you can free this memory since the texture is already uploaded into video memory.
		
		//glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,image);

		glEnable (GL_TEXTURE_2D);
		glGenTextures(1, &this->noise3DTexName); //generate the texture with the loaded data

		//The first thing that must take place in the process of uploading the texture is a call to glBindTexture. 
		//What glBindTexture does is it tells OpenGL which texture "id" we will be working with. 
		//A texture "id" is just a number that you will use to access your textures. Here is a sample call.
		glBindTexture(GL_TEXTURE_2D,noise3DTexName); //bind texture to its array


		//The glPixelStorei call tells OpenGL how the data that is going to be uploaded is aligned. 
		//This call tells OpenGL that the pixel data which is going to be passed to it is aligned in byte order, 
		//this means that the data has one byte for each component, one for red, green and blue
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//The glTexEnvf call sets environment variables for the current texture. 
		//What this does is tell OpenGL how the texture will act when it is rendered into a scene.
		//What this does is sets the active texture to GL_MODULATE. 
		//The GL_MODULATE attribute allows you to apply effects such as lighting and coloring to your texture. 
		//If you do not want lighting and coloring to effect your texture and you would like to display the texture unchanged when 
		//coloring is applied replace GL_MODULATE with GL_DECAL.
		//This is not needed if you use shaders!!
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		//The glTexImage2D call is our goal. 
		//This call will upload the texture to the video memory where it will be ready for us to use in our programs. 
		//I am going to explain this call parameter by parameter since it is so important to what we are doing.
		//target - The target of this call, it will always be GL_TEXTURE_2D.
		//level - The level of detail number, this should be left at 0 for our purposes. 
		//Once you become more adept at OpenGL texture mapping this parameter will be something that you might change.
		//internalformat - Internal components parameter. 
		//This tells OpenGL how many color components to represent internally from the texture that is uploaded. 
		//There are many symbolic constants for this parameter but the one which is most widely used is GL_RGB; this constant is equal to 3.
		//width & height - The width and height of the image data. These must be integers that are equal to 2n+2(border) for some integer n.
		//What this basically means is that the texture width and height must be a power of two (2,4,8,16,32,63,128,256,512, etc).
		//border - Image border, must be 0 or 1. I always use 0 in my code since I do not use image borders.
		//format - Format of the pixel data that will be uploaded. There are many constants which are accepted but GL_RGB is the value that is widely used.
		//type - Type of data that will be uploaded. Again there are several symbolic constants but the one which I use is GL_UNSIGNED_BYTE.
		//pixels - Pointer to the image data. 
		//This is the image data that will be uploaded to the video memory. 
		//Note that after your call to glTexImage2D you can free this memory since the texture is already uploaded into video memory.

		//glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage);

		glEnable(GL_TEXTURE_2D);

		//glGenTextures()
	}
	
   //Ross: This is where I set up the lights
   void DemoLight(void)
   {
	   glEnable(GL_LIGHTING);
	  
	   glEnable(GL_LIGHT0);
	   
	   glEnable(GL_NORMALIZE);

	   // Light model parameters:
	   // -------------------------------------------

	   GLfloat lmKa[] = { 0.0, 0.0, 0.0, 0.0 };
	   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

	   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	   glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	   // -------------------------------------------
	   // Spotlight Attenuation

	   GLfloat spot_direction[] = { 1.0, -1.0, -1.0 };
	   GLint spot_exponent = 30;
	   GLint spot_cutoff = 180;

	   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	   glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
	   glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);

	   GLfloat Kc = 1.0;
	   GLfloat Kl = 0.0;
	   GLfloat Kq = 0.0;

	   glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, Kc);
	   glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
	   glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);


	   // ------------------------------------------- 
	   // Lighting parameters:

	   GLfloat light_pos[] = { 0.0f, 5.0f, 5.0f, 1.0f };
	   GLfloat light_Ka[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	   GLfloat light_Kd[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	   GLfloat light_Ks[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	   glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	   glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
	   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
	   glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

	   // -------------------------------------------
	   // Material parameters:
	   
	   GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f};
	   GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
	   GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
	   GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
	   GLfloat material_Se = 20.0f;

	   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
	   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
	   
	  
   }
   

};

//-----------------------------------------------------------------------------
class myApplication : public cwc::glApplication
{
public:
	//Ross: Here is a simple output for checking that things are working
	virtual void OnInit() {std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

int main(void)
{
	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();
	
	pApp->run();
	delete pApp;
	return 0;
}

//-----------------------------------------------------------------------------


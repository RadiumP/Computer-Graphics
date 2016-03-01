
/* Assignment 1 
 * 
 * A basic, first OpenGL program.
 * 
 * This program illustrates how you open an window, specify its position and size,
 * and draw something in the window.
 *
 * Also illustrated: the state-machine operation of OGL. 
 *
 * When you resize the window so it is not square, the box changes shape. This might
 * not be desired -- we will look into fixing this later. (Keep in mind, this has
 * something to do with the aspect ratio.)
 *
 * Try changing the size of gluOrtho2D -- see in code below -- and you'll see how the
 * relative size of the box changes within the window.
 *
 *
 * Dianne Hansford, August 2004
 * Modified by Ross Maciejewski August 2011
 * Modified by Chenyang Li Sept. 2014
 */

#include <windows.h>
#include <ole2.h>
#include <ocidl.h>
#include <olectl.h>
#include <gl/gl.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

/* Include the GLUT library. This file (glut.h) contains gl.h and glu.h */
/* See how this is "glut.h" as opposed to <glut.h> - this is due to the fact that you need
   glut in your local directory. */
#include "glut.h"

// Structure for hierarchical StickMan in Assignment 2, this is only a starting point for the hierarchical structure
// You may need other items here or less items
typedef struct stickManNode
{
   float tx, ty, rot;
   GLfloat m[16];
   void (*f)();
   struct stickManNode *sibling;
   struct stickManNode *child;
} stickManNode;


/* These are the variables used to define the images being used in the example, you may need to redefine your own widths and heights */
unsigned char *image;
unsigned char *temp;
int imageWidth = 500;
int imageHeight = 375;

//set speed
int speed = 50;
int speed1 = 30;

//set time
float  newTime, oldTime, dT;
//set stickman
float object[3][2];
int count = 0;

#define PI 3.14159265

void animate();
void drawAxes();
void drawLine();
void drawLowerLegs();
void drawHead();

void renderStickMan();
void traverse(stickManNode *root);

void defaultMove();
void walkMove();

stickManNode *upTorso;

stickManNode *head;
stickManNode *LA;

stickManNode *RA;
stickManNode *LT;
stickManNode *UL;
stickManNode *UR;
stickManNode *LL;
stickManNode *LR;

/* We will have lots of angle calculations in class, this converts degrees to radians PI/180 */
const float DEG2RAD = 3.14159/180.;

// You may have lots of global variables to control states and animation, these would go below here

int type; 
BOOLEAN On;
char *img = "2.jpg";
void flipImag(char *image, char *flippedImage);
//You are REQUIRED to modularize your programs!  Here you see I've created a function stub to draw a unit circle outline (NOT FILLED)


//Draw an unit circle with 360 vertexes
void drawCircle()
{
	int i,n;
	n = 360;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i<n; ++i)
		glVertex2f(cos(360 * DEG2RAD / n*i), sin(360 * DEG2RAD / n*i));
	glEnd();
	
}

//An example function which draws a filled unit square (1x1) 
void drawSquare()
{
	glBegin(GL_POLYGON);
		glVertex2f(-.5, -.5);
		glVertex2f( .5, -.5);
		glVertex2f( .5,  .5);
		glVertex2f(-.5,  .5);
	glEnd();

}

//fill the indexcard
void filledCard(BOOLEAN on)
{

	float x1, x2, x3, x4, y1, y2, y3, y4;
	if (on == TRUE)
	{

		if (img == "2.jpg")
		{
			
			glColor3f(0.0, 0.0, 1.0);
			x1 = (2.0 * 134 / 499.0) - 1.0;
			y1 = 1 - (2.0 * 100 / 374.0);
			x2 = (2.0 * 361 / 499.0) - 1.0;
			y2 = 1 - (2.0 * 97 / 374.0);
			x3 = (2.0 * 363 / 499.0) - 1.0;
			y3 = 1 - (2.0 * 330 / 374.0);
			x4 = (2.0 * 136 / 499.0) - 1.0;
			y4 = 1 - (2.0 * 332 / 374.0);
			glBegin(GL_POLYGON);
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
			glVertex2f(x3, y3);
			glVertex2f(x4, y4);
			glEnd();
			glFlush();
		}
		else if (img == "1.jpg")
		{
			
			
			glColor3f(.0, 0.0, 1.0);
			x1 = (2.0 * 117 / 499.0) - 1.0;
			y1 = 1 - (2.0 * 133 / 374.0);
			x2 = (2.0 * 344 / 499.0) - 1.0;
			y2 = 1 - (2.0 * 17 / 374.0);
			x3 = (2.0 * 413 / 499.0) - 1.0;
			y3 = 1 - (2.0 * 252 / 374.0);
			x4 = (2.0 * 185 / 499.0) - 1.0;
			y4 = 1 - (2.0 * 369 / 374.0);
			glBegin(GL_POLYGON);
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
			glVertex2f(x3, y3);
			glVertex2f(x4, y4);
			glEnd();
			glFlush();
		}
	}
}


void drawground()
{
	glBegin(GL_LINES);
	glVertex2f(1, -.295);
	glVertex2f(-1, -.295);
	glEnd();
	glFlush();
}
//An example function which draws a unit line (length 1)
//void drawLine()
//{
	//glBegin(GL_LINES);
		//glVertex2f(0,0);
		//glVertex2f(0,1);
	//glEnd();
//}


//draw arms legs
void drawLine()
{
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, .1);
	glEnd();
	glFlush();
}

//draw torso
void drawTorso()
{
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, .05);
	glEnd();
	glFlush();
}

//draw lower legs
void drawLowerLegs()
{
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, -.1);
	glEnd();
	glFlush();
}

//draw head
void drawHead()
{
	int i;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i<360; ++i)
		glVertex2f(0.05*cos(2 * PI / 360 * i), 0.05*sin(2 * PI / 360 * i));
	glEnd();
	glFlush();
}

//This function will be completed in Assignment 2, you will used drawCircle, drawLine as part of this
void renderStickMan()
{
	float aR, dT = 0;

	aR = 10.0;
	
	
	//init the node
	head = malloc(sizeof(stickManNode));
	upTorso = malloc(sizeof(stickManNode));
	LT = malloc(sizeof(stickManNode));
	RA = malloc(sizeof(stickManNode));
	LA = malloc(sizeof(stickManNode));
	UL = malloc(sizeof(stickManNode));
	UR = malloc(sizeof(stickManNode));
	LR = malloc(sizeof(stickManNode));
	LL = malloc(sizeof(stickManNode));

	upTorso->tx = 0.0;
	upTorso->ty = 0.0;
	upTorso->rot = 0.0;
	upTorso->f = drawTorso;
	upTorso->child = LT;
	upTorso->sibling = NULL;


	LT->tx = 0.0;
	LT->ty = -.1;
	LT->rot = 0.0;
	LT->f = drawLine;
	LT->child = UR;
	LT->sibling = RA;

	RA->tx = 0.0;
	RA->ty = 0.0;
	RA->rot = 30.0;
	RA->f = drawLine;
	RA->child = NULL;
	RA->sibling = LA;

	LA->tx = 0.0;
	LA->ty = 0.0;
	LA->rot = -30.0;
	LA->f = drawLine;
	LA->child = NULL;
	LA->sibling = head;

	head->tx = 0.0;
	head->ty = .1;
	head->rot = 0.0;
	head->f = drawHead;
	head->child = NULL;
	head->sibling = NULL;

	UR->tx = 0.0;
	UR->ty = -0.1;
	UR->rot = 120.0;
	UR->f = drawLine;
	UR->child = LR;
	UR->sibling = UL;

	UL->tx = 0.0;
	UL->ty = -.1;
	UL->rot = -120.0;
	UL->f = drawLine;
	UL->child = LL;
	UL->sibling = NULL;

	LR->tx = 0.085;
	LR->ty = -0.1;
	LR->rot = 0.0;
	LR->f = drawLowerLegs;
	LR->child = NULL;
	LR->sibling = NULL;

	LL->tx = -0.085;
	LL->ty = -0.1;
	LL->rot = 0.0;
	LL->f = drawLowerLegs;
	LL->child = NULL;
	LL->sibling = NULL;
}


//traverse the tree
void traverse(stickManNode *root)
{
	if (root == NULL)
	{
		
		return;
	}
	
	glPopMatrix();
	glPushMatrix();
	glTranslatef(root->tx, root->ty, 0);
	glRotatef(root->rot, 0, 0, 1);

	root->f();
	glFlush();
	if (root->child != NULL)
	{
		traverse(root->child);
		
	}
	glPopMatrix();
	if (root->sibling != NULL)
	{
		traverse(root->sibling);
	}

}


//3 stickmen excercising
void defaultMove()
{
	newTime = GetTickCount();
	dT = (newTime - oldTime) / 1000.0;
	int count = 0;
	for (int i = 0; i < 3; i++)
	{
		glLoadIdentity();
		glTranslatef(object[i][0], object[i][1], 0);
				
		
		glPushMatrix();
		renderStickMan();

		upTorso->ty = 0.05*sin(dT*speed1*PI / 30);
		head->ty = 0.1 + 0.05*sin(dT*speed1*PI / 30);
		LT->ty = -0.1 + 0.05*sin(dT*speed1*PI / 30);
		LA->ty = 0.05*sin(dT*speed1*PI / 30);
		RA->ty = 0.05*sin(dT*speed1*PI / 30);
		UL->ty = -0.1 + 0.05*sin(dT*speed1*PI / 30);
		UR->ty = -0.1 + 0.05*sin(dT*speed1*PI / 30);

		LL->ty = -0.125 + 0.025*sin(dT*speed1*PI / 30);
		LR->ty = -0.125 + 0.025*sin(dT*speed1*PI / 30);
		LR->tx = (-.186 + 0.014*sin(dT*speed1*PI / 30)) / 2;
		LL->tx = (0.186 - 0.014*sin(dT*speed1*PI / 30)) / 2;
		LR->rot = 30.0 - sin(dT*speed1*PI / 30) * 30;
		LL->rot = -30.0 + sin(dT*speed1*PI / 30) * 30;


		LA->rot = -60.0 - sin(dT*speed1*PI / 30) * 30;
		RA->rot = 60.0 + sin(dT*speed1*PI / 30) * 30;
		UL->rot = -105.0 - sin(dT*speed1*PI / 30) * 15;
		UR->rot = 105.0 + sin(dT*speed1*PI / 30) * 15;

		glColor3f(0, 1, 1);
		traverse(upTorso);

	}
	glutSwapBuffers();
	glutPostRedisplay();
}

//1 stickman walking uphill/on the ground
void walkMove()
{	
	float x4, y4,x3,y3;
	float dS,dH;
	float angle ;
	float rot = 0;
	newTime = GetTickCount();
	dT = (newTime - oldTime) / 1000.0;
	glLoadIdentity();
	glPushMatrix();
	renderStickMan();
	
	//on the ground
	if (img == "2.jpg")
	{
		angle = 15.0;
		x3 = (2.0 * 363 / 499.0) - 1.0;
		y3 = 1 - (2.0 * 330 / 374.0);
		x4 = (2.0 * 136 / 499.0) - 1.0;
		y4 = 1 - (2.0 * 332 / 374.0) + 0.275;
		dS = (x3 + x4) / 2 + cos(dT*speed*PI / 300) * (x4 - x3 + 0.25) / 2;
		dH = y4;
		
		if ((int)(dT*speed / 300) % 2 == 1)
		{
			angle = -15.0;
		}
		LL->rot = (-angle + sin(dT*speed*PI / 30) * 30);
		LR->rot = -angle - sin(dT*speed*PI / 30) * 30;
	}


	//uphill
	if (img == "1.jpg")
	{
		angle = -15.0;
		x3 = (2.0 * 413 / 499.0) - 1.0;
		y3 = 1 - (2.0 * 252 / 374.0) + 0.295;
		x4 = (2.0 * 185 / 499.0) - 1.0 ;
		y4 = 1 - (2.0 * 369 / 374.0) + 0.295  ;
		
		rot = 30.0;
		

		dS = (x3 + x4) / 2 + cos(dT*speed*PI / 300) * (x4 - x3 + 0.3) / 2;
		dH = dS / (x3 - x4) *(y3 - y4) + y4 +0.2;
		if ((int)(dT*speed / 300) % 2 == 1)
		{
			rot = 10;
			angle = -30.0;
		}
		LR->rot = -angle - sin(dT*speed*PI / 30) * 15;
		LL->rot = (-angle + sin(dT*speed*PI / 30) * 15);
	}



	//basic movements
	head->tx = dS;
	upTorso->tx = dS;
	LT->tx = dS;
	RA->tx = dS;
	LA->tx = dS;
	UR->tx = dS;
	UL->tx = dS;

	head->ty = head->ty+dH;
	upTorso->ty += dH;
	LT->ty += dH;
	RA->ty += dH;
	LA->ty += dH;
	UR->ty += dH;
	UL->ty += dH;
	

	RA->rot = -180 + sin(dT*speed*PI / 30) * 60;
	LA->rot = -180 - sin(dT*speed*PI / 30) * 60 ;
		
	UR->rot = -180 + rot + sin(dT*speed*PI / 30) * 30;
	UL->rot = -180 + rot - sin(dT*speed*PI / 30) * 30;
	
	
	LL->ty = -0.1 - 0.1*cos(sin(dT*speed*PI / 30) * PI / 6.00 + PI*rot / 180.0) + dH;
	LL->tx = 0.1*sin(sin(dT*speed*PI / 30) * PI / 6.00 + PI *rot / 180.00) + dS;
	
	LR->ty = -0.1 - 0.1*cos(sin(dT*speed*PI / 30) * PI / 6.00 - PI *rot / 180.0) + dH;
	LR->tx = -0.1*sin(sin(dT*speed*PI / 30) * PI / 6.00 - PI *rot / 180.00) + dS;
	


	glColor3f(0, 0, 0);
	traverse(upTorso);

	glutSwapBuffers();
	glutPostRedisplay();
}

//This is function use to draw objects according to the right-clicked menu
//void drawObj(int id)
//{
	//switch (id)
	//{
	//case 1:
		//drawCircle();
		//break;
	//case 2:
		//drawSquare();
		//break;
	//case 3:
		//drawLine();
		//break;
	//default:
		//break;
	//}
//}

//The rountine display() gets called each time the window is redrawn.  Note that this is linked to the command

//glutDisplayFunc(display); which sets this as the display callback

void display()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* the window will correspond to these world coorinates - this means that the window will have a coordinate system that goes from -1 to 1 in the x and y direction */
	/* what effect do you think this will have on drawing a square if the window size is not square? */
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

	/* identify the modeling and viewing matrix that can be modified from here on */
	/* we leave the routine in this mode in case we want to move the object around */
	/* or specify the camera */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT);

	//The glViewport command defines the window that we will be drawing in, here it is the entire screen space
	glViewport(0,0,imageWidth,imageHeight);

	//In order to draw the image I read in, I am going to draw from the raster position starting at (-1, -1)
	glRasterPos2i(-1,-1);  //Why use -1, -1 ? 
	
	glDrawPixels(imageWidth,imageHeight,GL_RGB, GL_UNSIGNED_BYTE, image);

	//Once I'm done drawing the background image now I want to set up a new drawing area on the screen
	//specifically, I want to draw on top of the index card
	//glViewport(224,155,117,117);
	glColor3f(0.0, 0.0, 1.0);
	//drawObj(type);
	//drawground();
	filledCard(On);
	/* ready to draw now! forces buffered OGL commands to execute */
	glFlush();
}




//If you're using a .png image like I provided you should be fine using this, my images were converted to .png using GIMP
void loadPNG(char* filename,  int width, int height, int bitdepth)
{
	image = stbi_load(filename, &width, &height, &bitdepth, 0);
	
}

/* Initialize states -- called before */
void init()
{
	loadPNG(img,  imageWidth, imageHeight, 24);

	/* set background clear color to white */
	glClearColor(1.0, 1.0, 1.0, 0.0);

	/* identify the projection matrix that we would like to alter */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* the window will correspond to these world coorinates */
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

	/* identify the modeling and viewing matrix that can be modified from here on */
	/* we leave the routine in this mode in case we want to move the object around */
	/* or specify the camera */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void animation()
{
	//If I want things to move on their own without pressing a button, I need to set up an idle callback function
	//If you look further in the code, you'll see that this function is linked to the glutIdleFunc(animation);


	//This command then forces the program to go to the display function (or whatever function you linked to glutDisplayFunc)
	//Thus, you use this function to update global variables that can cause animation in the rendering
	glutPostRedisplay();
}

//Here is my menu callback function
void SelectFromMenu(int idCommand)
{
  switch (idCommand)
    {
    case 0:
		exit (0);
		break;			
    }
  // Almost any menu selection requires a redraw
  glutPostRedisplay();
}
//Call back the submenu to draw 
/*void drawSth(int id)
{
	//'type' decides the shape to draw
	switch (id)
	{
	case 1:
		type = id;
		glutPostRedisplay();
		break;
	case 2:
		type = id;
		glutPostRedisplay();
		break;
	case 3:
		type = id;
		glutPostRedisplay();
		break;
	}
	glutPostRedisplay();
}*/
//Call back the submenu to change the back ground
void changeImage(int id)
{
	switch (id)
	{
		//Show the flipped 'uphill' pic
	case 1:
		img = "1.jpg";
		type = 0;		
		init(); 		
		temp = (unsigned char*)malloc(1500*375);
		flipImag(image, temp);
		image = temp;
		glutPostRedisplay();
		break;
		//Show the flipped 'ground' pic
	case 2:
		img = "2.jpg";		
		type = 0;
		init();		
		temp = (unsigned char*)malloc(1500 * 375);
		flipImag(image, temp);		
		image = temp;		
		glutPostRedisplay();
		break;	
	}
	glutPostRedisplay();
}
void stickMan(int id)
{
	switch (id)
	{
	case 1:
		glutIdleFunc(defaultMove);
		glutPostRedisplay();
		break;
	case 2:
		oldTime = GetTickCount();
		glutIdleFunc(walkMove);
		glutPostRedisplay();
		break;
	
	}
	glutPostRedisplay();
}
void fillIndex(int id)
{
	switch (id)
	{
	case 1:
		On = TRUE;
		glutPostRedisplay();
		break;
	case 2:
		On = FALSE;
		glutPostRedisplay();
		break;

	}
	glutPostRedisplay();
}
//Here I will initialize the menu, for Assignment 1 you will need to create other menu entries with sub-entries
//Specifically, you'll be creating a menu entry "Images" with sub entries "1" and "2" as well as a menu entry "Draw" with sub entries "square" and "circle"
int BuildPopupMenu (void)
{
  int menu;
  int d_menu,h_menu,m_menu,f_menu;
  //d_menu = glutCreateMenu(drawSth);
  //glutAddMenuEntry("Circle", 1);
  //glutAddMenuEntry("Square", 2);
  //glutAddMenuEntry("Line", 3);
  
  h_menu = glutCreateMenu(changeImage);
  glutAddMenuEntry("1 Uphill", 1);
  glutAddMenuEntry("2 Ground", 2);

  f_menu = glutCreateMenu(fillIndex);
  glutAddMenuEntry("On",1);
  glutAddMenuEntry("Off",2);

  m_menu = glutCreateMenu(stickMan);
  glutAddMenuEntry("Default", 1);
  glutAddMenuEntry("Walking", 2);

  menu = glutCreateMenu (SelectFromMenu);  //This links a callback function "SelectFromMenu", and that link is saved as the integer menu which is then
                                         //returned to be linked to the right mouse button
  //glutAddSubMenu("Draw", d_menu);
  glutAddSubMenu("Image", h_menu);
  glutAddSubMenu("StickManMove", m_menu);
  glutAddSubMenu("Filled", f_menu);
  glutAddMenuEntry ("Exit demo", 0); //This adds an entry into the menu, if you right click, you'll see "Exit Demo" and this will
                                             //send the value MENU_EXIT to the callback function
  
 return menu;
}

//ALL STUDENTS: You need to fill out this function for getting the image color
//HINT: Use the test.png image to help you debug
void getImageColor(char* image, int pixel_x, int pixel_y, char *RED, char *GREEN, char *BLUE)
{
	
	int r, g, b;
	//This is the formula to pick the RGB
	r = (pixel_y * 500) + pixel_x ;
	*RED = image[r*3];	
	*GREEN = image[r*3+1];
	*BLUE = image[r*3+2];
}

//GRADUATE STUDENTS: You need to fill out this function to flip the image
void flipImag(char *image, unsigned char *flippedImage)
{

	//This is the loop to flip the pic upside-down
	for (int y = 0; y < 375; y++)
	{
		for (int x = 0; x < 1500; x++)
		{
			flippedImage[y * 1500+x] = image[1500 * (374- y)+x];			
		}		
	}
	

}

//Here is the mouse callback function, you will need to use the x, y coordinate intelligently to get the color of the pixel that is clicked on.
//You must have a printf statement here as well that prints out the coordinate and the color from the background image at the location
void myMouse(int button, int state, int x, int y)
{
	float px, py;
	unsigned char* R = (char *)malloc(8);
	unsigned char* G = (char *)malloc(8);
	unsigned char* B = (char *)malloc(8);
	//You need to think about the x and y coordinate that you get here and if they are what you expect
	switch (button)
	{
	    case GLUT_LEFT_BUTTON:

		    if(state == GLUT_DOWN)
			{
				printf("%d,%d\n",x,y);	//Get the (x,y)						
				getImageColor(image, x, 374-y, R, G,B);				
				printf("Screen click, R=%u,G=%u,B=%u.\n", *R,*G,*B);	//Display the RGB 
				

				//3 stickmen
				int index;
				index = count % 3;
				glClear(GL_COLOR_BUFFER_BIT);
				px = (2.0 * x / 499.0) - 1.0;
				py = 1 - (2.0 * y / 374.0);
				printf("%f,%f\n", px, py);
				object[index][0] = px;
				object[index][1] = py;

				count++;
				glColor3f(1, 1, 1);
				printf("%d\n\n", index);
				for (int i = 0; i < 3; i++){
					printf("%f,%f\n\n", object[i][0], object[i][1]);
				}

			}

		break;
	}
}


/* The main program */
int main(int argc, char** argv)
{

	/* create a window, position it, and name it */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(imageWidth, imageHeight);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Assignment - 2");
	
	/* create a callback routine for when the mouse is clicked */
	glutMouseFunc(myMouse);


	/* create a callback routine for when the program is idle */
	
	
	glutDisplayFunc(display);
	
	BuildPopupMenu ();  //This is going to be a function you write to initialize the menu
    glutAttachMenu (GLUT_RIGHT_BUTTON);  //This attaches the menu to the right mouse click  button

	/* init some states */
	
	glutIdleFunc(defaultMove);
	init();
	temp = (unsigned char*)malloc(1500 * 375);
	flipImag(image, temp);
	image = temp;
	
	/* entering the event loop */
		

	glutMainLoop();

	/* code here will not be executed */
}

//Written by Taylor Petrick

#include <cstdlib>

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <glut/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "expression/expressions.h"

//Window dimensions
const int gWidth  = 1200;
const int gHeight = 800;

//Rotation variables
float gXAngle = 0.0f;
float gYAngle = 0.0f;
int   gXOrigin = -1;
int   gYOrigin = -1;

//Zooming variables
bool  gDoZooming = false;
float gZoom = 1.0f;

//UI variables
int  gFrame=0,gTime,gTimebase=0;
char gFrameString[50];
char gZoomString[50] = "Zoom: 100%";
char gPointString[50] = "Point Size: 1";
char gFunctionString[1024] = "Function 1: ln(x^2) + ln(y) = z";

//Render variables
float  gGraphRes = 300;
float  gGraphBounds = 300;
int    gFrameScaling = gGraphRes/100+1;
int    gFrameSpacing = 10;
float* gPointCache = 0;
bool   gDrawGrid = true;
bool   gDrawNegatives = false;
int    gPointSize = 1;

//Function variables
int  gFunctionID = 0;
bool gFunctionChanged = true;
BaseExpression* gCustomFunction;

//Window ID
static int gWinID;

//Renders a string of characters to the screen
void renderCharacters(float pX, float pY, float pZ, void *pFont, char *pCharArray) 
{
	//Set the position
	glRasterPos3f(pX, pY, pZ);
	
	//Loop over the array and render until we hit a null
	for ( char* lChar = pCharArray; *lChar != '\0'; lChar++) 
	{ 
		glutBitmapCharacter(pFont, *lChar);
	}
}

//Renders the axes for the 3D graph, in RGB color scheme.
void renderAxisFrame()
{
	//Enable transparency blending
    glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	//Draw the x grid lines
	glBegin(GL_LINES);
	for(int i=0;i<=gFrameSpacing;i++) 
	{
		glColor4f(0.8,0.8,0.8,0.5);

		//Render the grid
		glVertex3f((gFrameSpacing/2-i)*gFrameScaling,0,-gFrameSpacing/2*gFrameScaling);
		glVertex3f((gFrameSpacing/2-i)*gFrameScaling,0,gFrameSpacing/2*gFrameScaling);
		glVertex3f(-gFrameSpacing/2*gFrameScaling,0,(gFrameSpacing/2-i)*gFrameScaling);
		glVertex3f(gFrameSpacing/2*gFrameScaling,0,(gFrameSpacing/2-i)*gFrameScaling);
	}
	glEnd();


	//Draw the y grid lines
	glBegin(GL_LINES);
	for(int i=0;i<=gFrameSpacing;i++) 
	{
		glColor4f(0.8,0.8,0.8,0.5);

		//Render the grid
		glVertex3f(0,(gFrameSpacing/2-i)*gFrameScaling,-gFrameSpacing/2*gFrameScaling);
		glVertex3f(0,(gFrameSpacing/2-i)*gFrameScaling,gFrameSpacing/2*gFrameScaling);
		glVertex3f(0,-gFrameSpacing/2*gFrameScaling,(gFrameSpacing/2-i)*gFrameScaling);
		glVertex3f(0,gFrameSpacing/2*gFrameScaling,(gFrameSpacing/2-i)*gFrameScaling);
	}
	glEnd();

	//Draw the z grid lines
	glBegin(GL_LINES);
	for(int i=0;i<=gFrameSpacing;i++) 
	{
		glColor4f(0.8,0.8,0.8,0.5);

		//Render the grid
		glVertex3f(-gFrameSpacing/2*gFrameScaling,(gFrameSpacing/2-i)*gFrameScaling,0);
		glVertex3f(gFrameSpacing/2*gFrameScaling,(gFrameSpacing/2-i)*gFrameScaling,0);
		glVertex3f((gFrameSpacing/2-i)*gFrameScaling,-gFrameSpacing/2*gFrameScaling,0);
		glVertex3f((gFrameSpacing/2-i)*gFrameScaling,gFrameSpacing/2*gFrameScaling,0);
	}
	glEnd();

	//Disable transparency
	glDisable(GL_BLEND);
}


//Renders the UI
void renderUI()
{
	//Compute the frame rate
	gFrame++;
	gTime = glutGet(GLUT_ELAPSED_TIME);

	//Update and calculate once per second
	if (gTime - gTimebase > 1000) 
	{
		//Stick it into a character array
		sprintf(gFrameString,"FPS:%4.2f", gFrame*1000.0/(gTime-gTimebase));
		gTimebase = gTime;
		gFrame = 0;
	}

	//Set the color for the frame rate text
	glColor3f(1.0f,1.0f,1.0f);

	//Set the camera to render to a 2D orthogonal plane
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, gWidth, gHeight, 0);

	//Go back the model view
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//Render the frame rate
	renderCharacters(5,20,0,GLUT_BITMAP_HELVETICA_12,gFrameString);

	//Render the info text
	renderCharacters(5,60,0,GLUT_BITMAP_HELVETICA_12, gZoomString);
	renderCharacters(5,80,0,GLUT_BITMAP_HELVETICA_12, gPointString);
	renderCharacters(5,200,0,GLUT_BITMAP_HELVETICA_12,"Camera:  Hold left-mouse and pan");
	renderCharacters(5,220,0,GLUT_BITMAP_HELVETICA_12,"Zoom:  Hold right-mouse and pan");
	renderCharacters(5,240,0,GLUT_BITMAP_HELVETICA_12,"Next function:  s");
	renderCharacters(5,260,0,GLUT_BITMAP_HELVETICA_12,"Previous function:  w");
	renderCharacters(5,280,0,GLUT_BITMAP_HELVETICA_12,"Add custom function in console:  c");
	renderCharacters(5,300,0,GLUT_BITMAP_HELVETICA_12,"Toggle axis frame:  t");
	renderCharacters(5,320,0,GLUT_BITMAP_HELVETICA_12,"Toggle inverted negatives:  n");
	renderCharacters(5,340,0,GLUT_BITMAP_HELVETICA_12,"Increase/decease point size:  -/+");
    renderCharacters(5,360,0,GLUT_BITMAP_HELVETICA_12, "Quit: q");

	//Render the function name
	renderCharacters((gWidth - strlen(gFunctionString)*8)/2,gHeight-20,0,GLUT_BITMAP_HELVETICA_18, gFunctionString);

	glPopMatrix();
}


//Renders the function as a point-cloud dataset
void renderPoints()
{
	//Load the model view matrix
	glMatrixMode(GL_MODELVIEW);		
	glLoadIdentity();	

	//Render the function
    glBegin(GL_POINTS);

	int index = 0;
	//Loop over the resolution to make the points. If we have 100 res, we go from -50 to 50, for example
	for(float i =-gGraphRes/2.0f; i<gGraphRes/2.0f; i+=gGraphRes/gGraphBounds)
	{
		for(float j=-gGraphRes/2.0f; j<gGraphRes/2.0f; j+=gGraphRes/gGraphBounds)
		{
			//Determine the height value for each planar coordinate, which is 'y' in our OpenGL system.
			//I'll provide a few functions to get things going...
			float y = 0;

			//Create if needed, else load from cache
			if(gFunctionChanged)
			{
				switch(gFunctionID)
				{
					case -1:
						//Evaluate using expession stack
						y= gCustomFunction->evaluate(i,j);
						strcpy(gFunctionString, "Custom Function");
						break;
					case 0:
						y = log(i*i) + log(j);
						strcpy(gFunctionString,"Function 1: ln(x^2) + ln(y) = z");
						break;

					case 1:
						y = (i*i + (j * j))/200;
						strcpy(gFunctionString,"Function 2: (x^2 + y^2)/200 = z");
						break;

					case 2:
						y = tan(i*j*0.2)*20;
						strcpy(gFunctionString,"Function 3: 20tan(0.2xy) = z");
						break;

					case 3:
						y= cos(i*0.03f)*10 + sin(j*0.03f)*10;
						strcpy(gFunctionString,"Function 4: 10cos(0.03x) + 10sin(0.03y) = z");
						break;

					case 4:
						y= -sqrt(-(i*i) - (j*j) + 8100);

						strcpy(gFunctionString,"Function 5: -sqrt(-x^2 - y^2 + 8100) = z");
						break;

					case 5:
						y = 4.0/sin(i*0.025);
						strcpy(gFunctionString,"Function 6: 4csc(0.025x) = z ");
						break;

					default:
						y = (j*0.01*j*i + i * i)/300;
						strcpy(gFunctionString,"Function 7: 0.01y^3 + x^2 = z ");
						break;
				}


				gPointCache[index] = y;
				index++;
			}
			else
			{
				y = gPointCache[index];
				index++;
			}

			//Use the partial differential and gradient from the a parabolic function to shade. Computing this
			//for each function will take some more work...
			float xPartial = (i)/1200.0f;
			float yPartial = (j)/1200.0f;
			float gradient = xPartial*xPartial + yPartial * yPartial;
			gradient = sqrt(gradient);

			if(y<0 && gDrawNegatives)
				glColor3f(0.1,gradient*4,(gradient*4+0.45)); 
			else
				glColor3f((gradient*4+0.45),gradient*4,0.1); 

			glVertex3f(j*0.1f,y*0.1f,i*0.1f);	
		}
	}

	//Next time, load from the cache
	gFunctionChanged = false;

	glEnd();   
}


//Main render loop
void renderScene() 
{
	//Clear the color and depth data
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Load and reset the view projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Set the VP to a perspective matrix. Field of view of 45, aspect ratio of width/height, and a near-to-far range of 0.1 to 1000 units
	gluPerspective(45.0f*gZoom,(GLfloat)gWidth/(GLfloat)gHeight,0.1f,1000.0f);

	//Look at the point 0,0,0, from the point 0,50,-35. Up vector is 0,1,0, with y pointing upwards. Rotate around the desired axis.
	gluLookAt(0, 50*gGraphRes/gGraphBounds, -35*gGraphRes/gGraphBounds, 0, 0, 0, 0, 1, 0);
	glRotatef(gXAngle,0,1,0);

	//The axis we rotate on for the vertical component depends on our angle around the vertical. The vector around which we rotate is therefore defined
	//by x = cos(angle) and z = sin(angle), as per the trig unit circle
	glRotatef(-gYAngle,cos(gXAngle*3.14159/180.0f),0,sin(gXAngle*3.14159/180.0f));

	//Render the function as point-cloud data. One day we'll want to use surface meshes instead...
	renderPoints();
	
	//Render the axis frame if needed
	if(gDrawGrid)
		renderAxisFrame();

	//Render the UI
	renderUI();

	//Swap the buffers and prepare to render again
    glutSwapBuffers();
	glutPostRedisplay();
}


//Handles keyboard input
void keyboard (unsigned char pKey, int pX, int pY) 
{
	//If the q was pressed, increment the function counter
	if ((pKey=='s' || pKey =='s') && gFunctionID < 6)
    {
		gFunctionID++;
		gFunctionChanged = true;
    }
	//else decrement the counter up to 0
	else if((pKey =='w' || pKey == 'W') && gFunctionID > 0)
	{
		gFunctionID--;
		gFunctionChanged = true;
	}
	else if(pKey == 'c' || pKey == 'C')
	{
		std::string lString;
		getline (std::cin,lString);

		gCustomFunction = processString(lString);
		gFunctionID = -1;
		gFunctionChanged = true;
	}
	else if(pKey == 't' || pKey == 'T')
	{
		gDrawGrid = !gDrawGrid;
	}
	else if(pKey == 'n' || pKey == 'N')
	{
		gDrawNegatives = !gDrawNegatives;
	}
	else if(pKey == '=' || pKey == '+')
	{
		if(gPointSize < 10)
			glPointSize(++gPointSize);

		sprintf(gPointString,"Point Size: %i",gPointSize);
	}
	else if(pKey == '-' || pKey == '_')
	{
		if(gPointSize > 1)
		{
			glPointSize(--gPointSize);
		}

		sprintf(gPointString,"Point Size: %i",gPointSize);
	}
    else if(pKey == 'q' || pKey == 'Q')
    {
        glutDestroyWindow(gWinID);
        delete gPointCache;
        delete gCustomFunction;
        exit(0);
    }
}

//Handles mouse motion
void mouseMove(int pX, int pY)
{ 	
	if(gDoZooming)
	{
		if(gXOrigin - pX > 0 && gZoom <= 2)
			gZoom += 0.03;
		else if(gZoom >= 0.1 && (gXOrigin - pX) < 0)
			gZoom -= 0.03;

		gXOrigin = pX;
		sprintf(gZoomString,"Zoom: %4.2f%%", 100.0f/gZoom);
	}
	else
	{
		//If the origin is set, we should be rotating
		if (gXOrigin >= 0) 
		{
			//Adjust the angle based on the x delta
			gXAngle += (pX - gXOrigin) * 0.25f;

			//Reset the origin
			gXOrigin = pX;
		}

		//Repeat the same for y
		if(gYOrigin >= 0)
		{
			gYAngle += (pY - gYOrigin) * 0.25f;
			gYOrigin = pY;
		}
	}
}

//Handles mouse button events
void mouseButton(int pButton, int pState, int pX, int pY) 
{
	//If it's the left button being changed
	if (pButton == GLUT_LEFT_BUTTON) 
	{
		//When the button is released, clear the origin, else set it to the mouse down point
		if (pState == GLUT_UP) 
		{
			gXOrigin = -1;
			gYOrigin = -1;
		}
		else  
		{
			gXOrigin = pX;
			gYOrigin = pY;
		}
	}
	else if(pButton == GLUT_RIGHT_BUTTON)
	{
		if(pState == GLUT_UP)
		{
			gDoZooming = false;
		}
		else
		{
			gDoZooming = true;
			gXOrigin = pX;
		}
	}

}

int main(int argc, char **argv) 
{
	//We're going to use GLUT instead of writing Win platform GL code... GLUT makes it easier to get going ASAP.

	//Init glut
	glutInit(&argc, argv);

	//Display should be standard color space and double buffered
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	//Window settings
	glutInitWindowPosition(100,100);
	glutInitWindowSize(gWidth,gHeight);
	gWinID = glutCreateWindow("3D Calculator");

	//The functions for handling keyboard/mouse input
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	//The function for handling display/render calls backs
	glutDisplayFunc(renderScene);

	//The default canvas color is dark gray, with a depth of 1
	glClearColor(0.15f, 0.15f, 0.15f, 0.15f);
	glClearDepth(1.0f);	

	//Make things a bit smoother
	glShadeModel(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glPointSize(gPointSize);

	//Enable depth testing, since we want GL to handle sorting for us
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	//Initialize the point cache
	gPointCache = (float*)malloc(gGraphRes * gGraphRes * sizeof(float));

	//Start the main GL loop, will terminate when the window is closed.
	glutMainLoop();

	//Free stuff
	if(gCustomFunction)
		delete gCustomFunction;
	
	delete gPointCache;

	//Bye bye.
	return 0;
}
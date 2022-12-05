#include "TextureBuilder.h"
#include "GLTexture.h"
#include <glut.h>
#include <playsoundapi.h>
int WIDTH = 1280;
int HEIGHT = 720;

#include <math.h>
#include <iostream>
#pragma comment(lib,"winmm.lib")
GLuint tex;
// Textures
GLTexture tex_ground;
GLTexture tex_sky;
GLTexture tex_football;
GLTexture tex_ad1;
GLTexture tex_ad2;
GLTexture tex_ad3;
GLTexture tex_sengal;
//GLTexture tex_pala;

char title[] = "World Cup";
float rep = 1;
float football[4] = {-5,0.3,0,0.3};
float bar[3] = { -16.5,1.5,2.7};
float bar2[3] = { +18.5,1.5,2.7 };
float player[3] = { 0,0,0 };
float playerRotation = 0;
bool hitLeft = false;
bool hitRight = false;
bool hitBack = false;
bool hitForward = false;
bool exceddedDistance = false;
float distance = 0;
bool goal = false;
bool goal2 = false;
bool keyUp  = false;
bool keyDown = false;
bool keyRight = false;
bool keyLeft = false;
int timeIsup = 1500;
bool endGame = false;
// 3D Projection Options
GLdouble fovy = 45;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}

	//define the operators
	Vector operator+(float des) {
		return Vector(x - des, y - des, z - des);
	}
	Vector operator/(float des) {
		return Vector(x / des, y / des, z / des);
	}
	Vector operator*(float des) {
		return Vector(x * des, y * des, z * des);
	}
	Vector operator+(Vector End) {
		return Vector(x + End.x, y +End.y, z +End.z);
	}
	Vector operator-(Vector End) {
		return Vector(x -End.x, y - End.y, z - End.z);
	}
	Vector getTheUnit() {
		return *this / sqrt(x * x + y * y + z * z);
	}
	Vector crossProduct(Vector u) {
		return Vector(y * u.z - z * u.y, z * u.x - x * u.z, x * u.y - y * u.x);
	}
};

Vector Eye(12, 8, 0);
Vector At(0, 2, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

//Camera movment 

void rotateUpDown(float angle) {
	Vector view = (At - Eye).getTheUnit();
	Vector right = Up.crossProduct(view).getTheUnit();
	view = view * cos(angle* 0.0174532925) + Up * sin(angle* 0.0174532925);
	Up = view.crossProduct(right);
	At = Eye + view;
}
void rotateLeftRight(float angle) {
	Vector view = (At - Eye).getTheUnit();
	Vector right = Up.crossProduct(view).getTheUnit();
	view = view * cos(angle * 0.0174532925) + right * sin(angle * 0.0174532925);
	At = Eye + view;
}
void rotateHorizontal(float angle) {
	Vector view = (At - Eye).getTheUnit();
	Vector right = Up.crossProduct(view).getTheUnit();
	Up = Up * cos(angle * 0.0174532925) + right * sin(angle * 0.0174532925);
}
void moveRightLeft(float value) {
	Vector scalerUnit = Up.crossProduct(At - Eye).getTheUnit();
	Eye =Eye+(scalerUnit * value);
	At =At+ (scalerUnit * value);
}
void moveForwardBack(float value) {
	Vector scalerUnit = (At - Eye).getTheUnit();
	Eye =Eye+  (scalerUnit* value);
	At =At+ (scalerUnit* value); 
}
void moveUpDown(float value) {
	Vector scalerUnit = Up.getTheUnit();
	Eye =Eye+ (scalerUnit * value);
	At  = At+  (scalerUnit * value);
	
}

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 
	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -10);
	glTexCoord2f(1, 0);
	glVertex3f(20, 0, -10);
	glTexCoord2f(1, 1);
	glVertex3f(20, 0, 10);
	glTexCoord2f(0, 1);
	glVertex3f(-20, 0, 10);
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glDisable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
void RenderOutside() {
	glDisable(GL_LIGHTING);	// Disable lighting 
	glColor3f(0.5, 0.3, 0.3);	// Dim the ground texture a bit

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	
	glVertex3f(-20, 0, -15);
	glVertex3f(20, 0, -15);
	glVertex3f(20, 0, -10);
	glVertex3f(-20, 0, -10);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-20, 0, 15);
	glVertex3f(20, 0, 15);
	glVertex3f(20, 0, 10);
	glVertex3f(-20, 0, 10);
	glEnd();
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-25, 0, -15);
	glVertex3f(-25, 0, 15);
	glVertex3f(-20, 0, 15);
	glVertex3f(-20, 0, -15);
	glEnd();
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(25, 0, -15);
	glVertex3f(25, 0, 15);
	glVertex3f(20, 0, 15);
	glVertex3f(20, 0, -15);
	glEnd();
	glPopMatrix();
	
	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

}
void drawFootball(float x, float y, float z, float r) {
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0.6 , 0.6, 0.6); //dim 
	GLUquadricObj* qfoot;
	qfoot = gluNewQuadric();
	glBindTexture(GL_TEXTURE_2D, tex_football.texture[0]);
	gluQuadricNormals(qfoot, GL_SMOOTH);
	gluQuadricTexture(qfoot, GL_TRUE);
	gluSphere(qfoot, r, 60, 35);
	gluDeleteQuadric(qfoot);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
void drawSky() {
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	glPushMatrix();

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex_sky.texture[0]);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}

void drawCamera(float z) {
	glPushMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0, 1, z);
	glScaled(0.1, 2, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix(); 
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0.3, 1, z);
	glScaled(0.1, 2, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();


	glPushMatrix();
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0.15, 2.5, z);
	glScaled(1, 1, 1);
	glutSolidCube(1.0);
	glPopMatrix();
	float face = 0;
	if (z > 0) face = 11.49;
	else {
		face = -11.49;
	}
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glTranslatef(0.15, 2.5, face);
	glScaled(0.8, 0.8, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
void drawPlayer(float x , float y , float z,float r,float g) {
	glPushMatrix();
	glPushMatrix();
	glColor3f(0.265625, 0.23046875, 0.19140625);
	glTranslatef(x,y+0.3,z );
	GLUquadricObj* qface;
	qface = gluNewQuadric();
	gluQuadricNormals(qface, GL_SMOOTH);
	gluQuadricTexture(qface, GL_TRUE);
	gluSphere(qface, 0.3, 60, 35);
	gluDeleteQuadric(qface);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(x-0.3, y +0.4, z+0.1);
	glScaled(0.1, 0.1, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(x - 0.3, y + 0.4, z - 0.1);
	glScaled(0.1, 0.1, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(r, g, 0);
	glPushMatrix();
	glTranslatef(x, y-0.5, z);
	glScaled(0.3, 1, 1);
	glutSolidCube(1.0);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(x, y - 1.5, z+0.2);
	glScaled(0.3, 1, 0.3);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(x, y - 1.5, z - 0.2);
	glScaled(0.3, 1, 0.3);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(r, g, 0);
	glPushMatrix();
	glTranslatef(x, y - 0.1, z + 0.65);
	glScaled(0.3, 0.2, 0.3);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(r, g, 0);
	glPushMatrix();
	glTranslatef(x, y - 0.1, z - 0.65);
	glScaled(0.3, 0.2, 0.3);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.265625, 0.23046875, 0.19140625);
	glPushMatrix();
	glTranslatef(x, y - 0.6, z - 0.7);
	glScaled(0.3, 0.8, 0.2);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.265625, 0.23046875, 0.19140625);
	glPushMatrix();
	glTranslatef(x, y - 0.6, z + 0.7);
	glScaled(0.3, 0.8, 0.2);
	glutSolidCube(1.0);
	glPopMatrix();



	glPopMatrix();
	
}
void drawWall() {
	//wall
	glPushMatrix();
	glPushMatrix();
	glColor3f(0.2, 0, 0);
	glPushMatrix();
	glTranslatef(-25, 5, 0);
	glScaled(0.5, 10, 30);
	glutSolidCube(1.0);
	glPopMatrix();
	//screen
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-24.5, 5, 0);
	glScaled(0.5, 4, 10);
	glutSolidCube(1.0);
	glPopMatrix();

	//score
	if (!endGame) {
		if (!goal) {
			//draw 1

			glPushMatrix();
			glColor3f(1, 0, 0);
			glTranslatef(-24.2, 5, 2.5);
			glScaled(0, 2, 0.5);
			glutSolidCube(1.0);
			glPopMatrix();

		}
		else {
			//draw 2

			glPushMatrix();
			glColor3f(1, 0, 0);
			glTranslatef(-24.2, 5.375, 2.15);
			glScaled(0, 0.525, 0.3);
			glutSolidCube(1.0);
			glPopMatrix();

			glPushMatrix();
			glColor3f(1, 0, 0);
			glTranslatef(-24.2, 4.625, 2.85);
			glScaled(0, 0.525, 0.3);
			glutSolidCube(1.0);
			glPopMatrix();

			glPushMatrix();
			glColor3f(1, 0, 0);
			glTranslatef(-24.2,5.75 , 2.5);
			glScaled(0, 0.3, 1);
			glutSolidCube(1.0);
			glPopMatrix();

			glPushMatrix();
			glColor3f(1, 0, 0);
			glTranslatef(-24.2, 5, 2.5);
			glScaled(0, 0.3, 1);
			glutSolidCube(1.0);
			glPopMatrix();
			
			glPushMatrix();
			glColor3f(1, 0, 0);
			glTranslatef(-24.2, 4.25, 2.5);
			glScaled(0, 0.3, 1);
			glutSolidCube(1.0);
			glPopMatrix();

		}
		if (!goal2) {
			glPushMatrix();
			glColor3f(0, 1, 0);
			glTranslatef(-24.2, 5, -2.5);
			glScaled(0, 2, 0.5);
			glutSolidCube(1.0);
			glPopMatrix();
		}
		else {
			glPushMatrix();
			glColor3f(0, 1, 0);
			glTranslatef(-24.2, 5.375, -2.85);
			glScaled(0, 0.525, 0.3);
			glutSolidCube(1.0);
			glPopMatrix();

			glPushMatrix();
			glColor3f(0, 1, 0);
			glTranslatef(-24.2, 4.625, -2.15);
			glScaled(0, 0.525, 0.3);
			glutSolidCube(1.0);
			glPopMatrix();

			glPushMatrix();
			glColor3f(0, 1, 0);
			glTranslatef(-24.2, 5.75, -2.5);
			glScaled(0, 0.3, 1);
			glutSolidCube(1.0);
			glPopMatrix();

			glPushMatrix();
			glColor3f(0, 1, 0);
			glTranslatef(-24.2, 5, -2.5);
			glScaled(0, 0.3, 1);
			glutSolidCube(1.0);
			glPopMatrix();

			glPushMatrix();
			glColor3f(0, 1, 0);
			glTranslatef(-24.2, 4.25, -2.5);
			glScaled(0, 0.3, 1);
			glutSolidCube(1.0);
			glPopMatrix();
		}
	
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(-24.2, 5.5, 0);
		glScaled(0, 0.5, 0.5);
		glutSolidCube(1.0);
		glPopMatrix();

		
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(-24.2, 4.5, 0);
		glScaled(0, 0.5, 0.5);
		glutSolidCube(1.0);
		glPopMatrix();
	}
	//flags
     

	glPopMatrix();



}
void drawFlags() {
	glPushMatrix();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-24.5, 5, 10);
	glScaled(0.5, 0.5, 3);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-24.5, 4.5, 10);
	glScaled(0.5, 0.5, 3);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslatef(-24.2, 5, 10);
	glScaled(0.2, 0.2, 0.5);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(-24.5, 5.5, 10);
	glScaled(0.5, 0.5, 3);
	glutSolidCube(1.0);
	glPopMatrix();


	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-24.5, 5, -10);
	glScaled(0.5, 0.5, 3);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.453125, 0.671875, 0.87109375);
	glTranslatef(-24.5, 4.5, -10);
	glScaled(0.5, 0.5, 3);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslatef(-24.2, 5, -10);
	glScaled(0.2, 0.2, 0.5);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.453125, 0.671875, 0.87109375);
	glTranslatef(-24.5, 5.5, -10);
	glScaled(0.5, 0.5, 3);
	glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
}
void drawCountry(float x , float y,float z) {
	glPushMatrix();

	glTranslated(x, y,z);
	glPushMatrix();
	glColor3f(1, 0.671875, 0.87109375);
	glTranslatef(0, 1.5, -0);
	glScaled(0.1,3 , 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.671875, 0.87109375);
	glTranslatef(0, 2.3, -1);
	glScaled(0.1, 1, 2);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.671875, 0.87109375);
	glTranslatef(0, 3.075, 0);
	GLUquadricObj* qface;
	qface = gluNewQuadric();
	gluQuadricNormals(qface, GL_SMOOTH);
	gluQuadricTexture(qface, GL_TRUE);
	gluSphere(qface, 0.15, 60, 35);
	gluDeleteQuadric(qface);
	glPopMatrix();


	glPopMatrix();
}
void drawCorner(float x, float z) {
	glPushMatrix();
	glTranslated(x, 0, z);
	glPushMatrix();
	glColor3f(1,1, 1);
	glTranslatef(0, 1, -0);
	glScaled(0.1, 2, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslatef(0, 1.5, -0.5);
	glScaled(0.1, 1, 1);
	glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
}
void drawbanner() {
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	glColor3f(0, 0, 1);
	glPushMatrix();
	//left
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0, 1, 15);
	glScaled(50, 2, 0.5);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();



	//right

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0, 1, -15);
	glScaled(50, 2, 0.5);
	glutSolidCube(1.0);
	glPopMatrix();
	//back
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(25, 1, 0);
	glScaled(0, 2, 30);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void drawGoalNet(float x, float y, float z) {


	glPushMatrix();
	glColor3f(1, 1, 1);
	glPushMatrix();
	//frontleft
	glColor3f(1, 1, 1);
	glTranslatef(x, y, z);
	glScaled(0.1, 3, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();







	//frontright
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(x, y, z-5.3);
	glScaled(0.1, 3, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	//backleft
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(x-2, y, z );
	glScaled(0.1, 3, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();
	//backright
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(x-2, y, z - 5.3);
	glScaled(0.1, 3, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	//top
	float i = z ;
	float end = z-5.3 ;
	while (i >= end)
	{
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(x - 2, y +1.5, i);
		glScaled(0.1, 0.1, 0.1);
		glutSolidCube(1.0);
		glPopMatrix();
		i -= 0.01;
	}
	 i = z ;
	 end = z-5.3 ;
	while (i >= end)
	{
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(x , y + 1.5, i);
		glScaled(0.1, 0.1, 0.1);
		glutSolidCube(1.0);
		glPopMatrix();
		i -= 0.01;
	}
	i = x;
	end = x - 2;
	while (i >= end)
	{
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(i, y+1.5,z);
		glScaled(0.1, 0.1, 0.1);
		glutSolidCube(1.0);
		glPopMatrix();
		i -= 0.01;
	}
	i = x;
	end = x - 2;
	while (i >= end)
	{
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(i, y+1.5, z-5.3);
		glScaled(0.1, 0.1, 0.1);
		glutSolidCube(1.0);
		glPopMatrix();
		i -= 0.01;
	}
	//back net
	i = 0;
	end = y + 1.3;
	float posX = x-2;
	if (x > 0) {
		posX = x ;
	}
	while (i <= end) {
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(posX, i, (z - 5.3/2));
		glScaled(0.1, 0.05,5.3);
		glutSolidCube(1.0);
		glPopMatrix();
		i += 0.265;
	}
	i = z;
	end = z - 5.3;
	while (i >= end) {
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(posX,1.5, i);
		glScaled(0.1, 3, 0.05);
		glutSolidCube(1.0);
		glPopMatrix();
		i -= 0.265;
	}

	//leftnet 
	i = 0;
    end = y + 1.3;
	while (i <= end) {
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef((x-1), i, z);
		glScaled(2, 0.05, 0.05);
		glutSolidCube(1.0);
		glPopMatrix();
		i += 0.265;
	}
	i = 0;
	end = y + 1.3;
	while (i <= end) {
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef((x - 1), i, z-5.3);
		glScaled(2, 0.05, 0.05);
		glutSolidCube(1.0);
		glPopMatrix();
		i += 0.265;
	}

	//upward
	i = x;
	end = x-2;
	while (i >= end) {
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(i, 1.5, z);
		glScaled(0.05, 3, 0.05);
		glutSolidCube(1.0);
		glPopMatrix();
		i -= 0.265;
	}
	i = x;
	end = x - 2;
	while (i >= end) {
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(i, 1.5, z-5.3);
		glScaled(0.05, 3, 0.05);
		glutSolidCube(1.0);
		glPopMatrix();
		i -= 0.265;
	}
	glPopMatrix();
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
	RenderGround();
	RenderOutside();
	drawFootball(football[0],football[1],football[2],football[3]);
	drawSky();
	drawGoalNet(bar[0], bar[1], bar[2]);
	drawGoalNet(bar2[0], bar2[1], bar2[2]);
	drawWall();
	drawbanner();
	drawCamera(12);
	drawCamera(-12);
	drawFlags();
	glPushMatrix();
	glTranslatef(player[0], player[1], player[2]);
	glRotatef(playerRotation, 0, 1, 0);
	glTranslatef(0, 0, 0);
	drawPlayer(0, 2, 0,1,0);
	glPopMatrix();
	glPushMatrix();
	drawPlayer(-15, 2, -12, 1, 1);
	glPopMatrix();
	drawCountry(-24,0,14);
	drawCorner(-18.5, -9);
	drawCorner(18.5, 9);
	drawCorner(-18.5, 9);
	drawCorner(18.5, -9);
	glPushMatrix();
	glDisable(GL_LIGHTING);	// Disable lighting 
	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	glBindTexture(GL_TEXTURE_2D, tex_ad1.texture[0]);	// Bind the ground texture
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-25, 0, 14.7);
	glTexCoord2f(6, 0);
	glVertex3f(25, 0, 14.7);
	glTexCoord2f(6, 1);
	glVertex3f(25, 2, 14.7);
	glTexCoord2f(0, 1);
	glVertex3f(-25, 2, 14.7);
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glDisable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glPopMatrix();

	glPushMatrix();
	glDisable(GL_LIGHTING);	// Disable lighting 
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	glBindTexture(GL_TEXTURE_2D, tex_ad2.texture[0]);	// Bind the ground texture
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-25, 0, -14.7);
	glTexCoord2f(6, 0);
	glVertex3f(25, 0, -14.7);
	glTexCoord2f(6, 1);
	glVertex3f(25, 2, -14.7);
	glTexCoord2f(0, 1);
	glVertex3f(-25, 2, -14.7);
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glDisable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glPopMatrix();

	glPushMatrix();
	glDisable(GL_LIGHTING);	// Disable lighting 
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	glBindTexture(GL_TEXTURE_2D, tex_ad3.texture[0]);	// Bind the ground texture
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(24.95, 0, -14.6);
	glTexCoord2f(1, 0);
	glVertex3f(24.95, 2, -14.6);
	glTexCoord2f(1, 3);
	glVertex3f(24.95, 2, 14.6);
	glTexCoord2f(0, 3);
	glVertex3f(24.95, 0, 14.6);
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glDisable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glPopMatrix();
	//flags

	glPushMatrix();
	glDisable(GL_LIGHTING);	// Disable lighting 
	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	glBindTexture(GL_TEXTURE_2D, tex_sengal.texture[0]);	// Bind the ground texture
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-23.9, 1.8, 14);
	glTexCoord2f(1, 0);
	glVertex3f(-23.9, 1.8, 12);
	glTexCoord2f(1, 1);
	glVertex3f(-23.9, 2.8, 12);
	glTexCoord2f(0, 1);
	glVertex3f(-23.9, 2.8, 14);
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glDisable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glPopMatrix();
	glutSwapBuffers();

}
void resetGame() {
	timeIsup = 1500;
	football[0] = -5;
	football[2] = 0;
	player[0] = 0;player[2] = 0;
	playerRotation = 0;
	endGame = false;
	goal = false;
	goal2 = false;
	sndPlaySound(TEXT("background.wav"), SND_ASYNC | SND_LOOP | SND_FILENAME);

}
//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'p':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'o':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'a':
		moveRightLeft(0.3);
		break;
	case 'd':
		moveRightLeft(-0.3);
		break;
	case 'e':
		moveForwardBack(0.3);
		break;
	case 'q':
		moveForwardBack(-0.3);
		break;
	case 'w':
		moveUpDown(0.3);
		break;
	case 's':
		moveUpDown(-0.3);
		break;


	case 'm':
		//topView
		Eye = Vector(0.4, 50, 0);
		At = Vector(0, 0, 0);
		Up = Vector(-1, 0, 0);
		break;
	case 'n':
		//sideView
		Eye = Vector(0, 5, 28);
		At = Vector(0, 0, 0);
		Up = Vector(0, 1, 0);
		break;

	case 'b':
		//frontView
		Eye = Vector(35, 10, 0);
		At = Vector(0, 0, 0);
		Up = Vector(0, 1, 0);
		break;
	case 'v':
		//frontView2
		Eye = Vector(-20, 10, 0);
		At = Vector(0, 0, 0);
		Up = Vector(0, 1, 0);
		break;
	case 'c':
		//sideView2
		Eye = Vector(0, 5, -28);
		At = Vector(0, 0, 0);
		Up = Vector(0, 1, 0);
		break;
	case '4':
		rotateLeftRight(1.0);
		break;
	case '6':
		rotateLeftRight(-1.0);
		break;
	case '8':
		rotateUpDown(1.0);
		break;
	case '5':
		rotateUpDown(-1.0);
		break;
	case '7':
		rotateHorizontal(1.0);
		break;
	case '9':
		rotateHorizontal(-1.0);
		break;

	case 'l':
		if (endGame) {
			resetGame();
		}
		break;

	case 27:
		exit(0);
		break;
	default:
		break;
	}

	glLoadIdentity();	//Clear Model_View Matrix
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glutPostRedisplay();	//Re-draw scene 
	glutPostRedisplay();
}
void playerMove(int key, int x, int y) {
	if (!endGame) {
		switch (key) {
		case GLUT_KEY_DOWN:
			playerRotation = 180;
			if (player[0] <= 20) {
				player[0] += 0.2;
			}
			keyDown = true;
			break;
		case GLUT_KEY_LEFT:
			playerRotation = 90;
			if (player[2] <= 10) {


				player[2] += 0.2;
			}
			keyLeft = true;
			break;
		case GLUT_KEY_RIGHT:
			playerRotation = -90;
			if (player[2] >= -10) {
				player[2] -= 0.2;

			}
			keyRight = true;
			break;
		case GLUT_KEY_UP:
			playerRotation = 0;
			if (player[0] >= -20) {
				player[0] -= 0.2;
				keyUp = true;
			}
			break;
		}
	}

	glutPostRedisplay();

}
void checkCollision() {
	if (!hitForward && !hitBack && !hitRight && !hitLeft) {
		float err = 0.5;
		float x1 = player[0];
		float z1 = player[2];
		if (x1 >= football[0] - err && x1 <= football[0] + err && z1 <= football[2] + err && z1 >= football[2] - err) {
			if (playerRotation == 0) {
				hitForward = true;
				sndPlaySound(TEXT("hit.wav"), SND_ASYNC);

			}
			else if (playerRotation == 180) {
				hitBack = true;
				sndPlaySound(TEXT("hit.wav"), SND_ASYNC);

			}
			else if (playerRotation == -90) {
				hitRight = true;
				sndPlaySound(TEXT("hit.wav"), SND_ASYNC);

			}
			else if (playerRotation == 90) {
				hitLeft = true;
				sndPlaySound(TEXT("hit.wav"), SND_ASYNC);

			}

		}
	}
}
void checkGoal() {
	float x1 = football[0];
	float z1 = football[2];
	if (x1 <= bar[0]-0.5 && (z1 <= bar[2]) && (z1 >= bar[2] - 3.5)) {
		goal = true;
		hitForward = false;
		hitBack = false;
		hitRight = false;
		hitLeft = false;
		sndPlaySound(TEXT("goal.wav"), SND_ASYNC);
	}
	else if (x1 >= bar2[0] - 0.5 && (z1 <= bar2[2]) && (z1 >= bar2[2] - 3.5)) {
		goal2 = true;
		hitForward = false;
		hitBack = false;
		hitRight = false;
		hitLeft = false;
		sndPlaySound(TEXT("goal.wav"), SND_ASYNC);
	}

}
void stopBall() {
	hitRight = false;
	hitLeft = false;
	hitForward = false;
	hitBack = false;
}
void checkBoundrais() {
	if (football[2] > 9) {
		football[2] = 9;
		stopBall();
	}
	if (football[2] < -9) {
		football[2] = -9;
		stopBall();
	}
	if (football[0] > 19) {
		football[0] = 19;
		stopBall();
	}
	if (football[0] < -19) {
		football[0] = -19;
		stopBall();
	}

}
void CheckEndGame() {
	if (timeIsup <= 0 && !endGame) {
		timeIsup = 1500;
		football[0] = -10;
		football[2] = 14;
		player[0] = -12;
		player[2] = 14;
		playerRotation = -90;
		stopBall();
		endGame = true;
		sndPlaySound(TEXT("endgame.wav"), SND_ASYNC);
		
	}
}

void Timer(int value) {
	timeIsup--;
	
	CheckEndGame();
	
	if (!endGame && !goal &&!goal2) {
	    checkCollision();
	    checkGoal();
		checkBoundrais();
		if (distance >= 5) {
			distance = 0;
			stopBall();
		}
		if (hitLeft) {
			hitRight = false; hitBack = false;hitForward = false;
			distance += 0.2;
			football[2] += 0.2;
		}
		else if (hitRight) {
			hitLeft = false; hitBack = false;hitForward = false;
			distance += 0.2;
			football[2] -= 0.2;
		}
		else if (hitForward) {
			hitLeft = false; hitBack = false;hitRight = false;
			distance += 0.2;
			football[0] -= 0.2;
		}
		else if (hitBack) {
			hitLeft = false; hitForward = false;hitRight = false;
			distance += 0.2;
			football[0] += 0.2;
		}
	/*	if (goal) {
			football[0] = 0;
			football[2] = 0;
			goal = !goal;
		}*/
	}
	glutTimerFunc(10, Timer, 0);
	glutPostRedisplay();
}
//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	tex_sky.Load("Textures/blu-sky-3.bmp");
	tex_football.Load("Textures/football.bmp");
	tex_sengal.Load("Textures/sengal.bmp");
	tex_ad1.Load("Textures/ad1.bmp");
	tex_ad2.Load("Textures/ad2.bmp");
	tex_ad3.Load("Textures/ad3.bmp");
	//tex_pala.Load("Textures/palestine.bmp");
	//tex_ad1.Load("Texetures/guc.bmp");
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 50);

	glutCreateWindow(title);

	myInit();
	glutDisplayFunc(myDisplay);
	glutSpecialFunc(playerMove);
	glutKeyboardFunc(myKeyboard);
	glutReshapeFunc(myReshape);
	glutTimerFunc(0, Timer, 0);

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	sndPlaySound(TEXT("background.wav"), SND_ASYNC | SND_LOOP | SND_FILENAME);
	glutMainLoop();
}
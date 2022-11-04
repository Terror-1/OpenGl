#include <stdio.h>
using namespace std;
#include <glut.h>
// methods signatures
void Display();
void drawPlayer();
void drawEnemy();
void drawRect(int x, int y, int w, int h);
void Key(unsigned char key, int x, int y);
void time(int val);
void drawBullet(int x, int y, float r);
void spe(int k, int x, int y);

// constant values
float xposition = 300.0;
float yposition = 300.0;
bool left = true;
bool fire = false;
float enemyX = 0;
int speX = 0;
int speY = 15;
int ballX = 150 + speX;
int ballY = speY + 40;



void Display() {
	glClear(GL_COLOR_BUFFER_BIT);

	drawPlayer();
	if (fire) {

		drawBullet(ballX, ballY, 5);
	}
	drawEnemy();
	glFlush();
}

void drawPlayer() {
	glPushMatrix();
	glTranslatef(speX, speY, 0);
	glBegin(GL_QUAD_STRIP);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f((xposition / 2.0) - 10, 0.0f, 0.0f);
	glVertex3f((xposition / 2.0) + 10, 0.0f, 0.0f);
	glVertex3f((xposition / 2.0) - 10, 20.0f, 0.0f);
	glVertex3f((xposition / 2.0) + 10, 20.0f, 0.0f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f((xposition / 2.0) - 10, 20.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f((xposition / 2.0) + 10, 20.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f((xposition / 2.0), 30.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f((xposition / 2.0) - 10, 5.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f((xposition / 2.0) - 10, 15.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f((xposition / 2.0) - 20, 10.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f((xposition / 2.0) + 10, 5.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f((xposition / 2.0) + 10, 15.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f((xposition / 2.0) + 20, 10.0f, 0.0f);
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f);
	drawRect((xposition / 2.0) - 8, -5.0f, 5, 5);
	drawRect((xposition / 2.0) + 2, -5.0f, 5, 5);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.6f, 0.0f);
	glVertex3f((xposition / 2.0) - 5, -5.0f, 0.0f);
	glVertex3f((xposition / 2.0) - 5, -8.0f, 0.0f);
	glVertex3f((xposition / 2.0) + 5, -5.0f, 0.0f);
	glVertex3f((xposition / 2.0) + 5, -8.0f, 0.0f);
	glVertex3f(35.0f, 30.0f, 0.0f);
	glEnd();
	glPopMatrix();
}

void drawEnemy() {
	glPushMatrix();
	glTranslated(enemyX, 0, 0);
	glBegin(GL_QUAD_STRIP);
	glColor3f(0.0f, 0.5f, 0.0f);
	glVertex3f((xposition / 2.0) - 30, yposition, 0.0f);
	glVertex3f((xposition / 2.0) + 30, yposition, 0.0f);
	glVertex3f((xposition / 2.0) - 30, yposition - 50, 0.0f);
	glVertex3f((xposition / 2.0) + 30, yposition - 50, 0.0f);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	drawRect((xposition / 2.0) - 20, yposition - 25, 10, 10);
	drawRect((xposition / 2.0) + 10, yposition - 25, 10, 10);
	drawRect((xposition / 2.0) - 20, yposition - 40, 40, 4);
	glPopMatrix();

}
void drawRect(int x, int y, int w, int h) {
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}
void drawBullet(int x, int y, float r) {
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(x, y, 0);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}
void Key(unsigned char k, int x, int y) {
	if (k == ' ') {
		fire = true;
		ballX = speX + 150;
		ballY = speY + 40;
	}
	glutPostRedisplay();
}
void time(int val)//timer animation function, allows the user to pass an integer valu to the timer function.
{
	if (enemyX <= -120) {
		left = !left;
	}

	else if (enemyX >= 120) {
		left = !left;
	}
	if (left) {
		enemyX -= 1;
	}
	else {
		enemyX += 1;
	}
	if (fire) {
		ballY += 2;
	}
	if (ballY >= 300) {
		fire = !fire;
		ballY = speY + 40;
	}
	glutPostRedisplay();						// redraw 		
	glutTimerFunc(1, time, 0);					//recall the time function after 1000 ms and pass a zero value as an input to the time func.
}
void spe(int k, int x, int y)// keyboard special key function takes 3 parameters
// int k: is the special key pressed such as the keyboard arrows the f1,2,3 and so on
{
	if (k == GLUT_KEY_RIGHT && speX <= 130)//if the right arrow is pressed, then the object will be translated in the x axis by 10. (moving right)
		speX += 10;
	if (k == GLUT_KEY_LEFT && speX >= -130)//if the left arrow is pressed, then the object will be translated in the x axis by -10. (moving left)
		speX -= 10;
	if (k == GLUT_KEY_UP && speY <= 180)//if the up arrow is pressed, then the object will be translated in the y axis by 10. (moving upwords)
		speY += 10;
	if (k == GLUT_KEY_DOWN && speY >= 10)//if the down arrow is pressed, then the object will be translated in the y axis by -10. (moving downwords)
		speY -= 10;
	glutPostRedisplay();//redisplay to update the screen with the changes
}
void main(int argc, char** argr) {
	glutInit(&argc, argr);

	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("space invaders");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Key);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glutSpecialFunc(spe);			//call the keyboard special keys function
	glutTimerFunc(0, time, 0);
	gluOrtho2D(0.0, xposition, 0.0, yposition);
	glutMainLoop();
}


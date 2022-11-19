#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include <glut.h>
#include <math.h>
#include <random>
#pragma comment(lib,"winmm.lib")
// methods signatures
void Display();
void print(int x, int y, char* string);
void drawPlayer();
void drawEnemy();
bool checkCollison(int x , int y);
void drawRect(int x, int y, int w, int h);
void KeyUp(unsigned char key, int x, int y);
void time(int val);
void drawBullet(int x, int y, float r, int R, int G, int B);
void spe(int k, int x, int y);
void drawPower(int x, int y,int r);
void reset();
bool checkPowerCollison();
bool checkBulletEnemyCollision(int x ,int y);
// constant values
float xposition = 300.0;
float yposition = 300.0;
int countHealth = 58;
int countLives = 3;
bool displayPower = true;
int status = 0;
bool left = true;
bool fire = false;
int enemyX = 0;
int speX = 0;
int speY = 15;
std::vector<std::pair<int, int>>positions;
std::vector<std::pair<int, int>> enemyFirepositions;
int toFire = 0;
int colorPlayer = 1;
int r = 5;
int powerX = 5+(rand()%(290-5+1));
int powerY = 5+(rand() % (200-5+1)) ;
int counter = 600;
int counter2 = 700;
int toappear = 500;
bool powerUp = false;
bool removeTimeFlag = false;
void Display() {
	if (status == 0) {
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		glColor3f(1, 1, 1);
		char* p0s[20];
		sprintf((char*)p0s, "LIVES :");
		print(10, 275, (char*)p0s);
		glPopMatrix();
		drawPlayer();
		/*if (fire) {

			drawBullet(ballX, ballY, r, 0, 0, 1);

		}*/
		for (auto i = positions.begin();i != positions.end();i++) {
			std::pair<int, int> p = *i;
			drawBullet(p.first, p.second, r, 0, 0, 1);
		}
		for (auto i = enemyFirepositions.begin();i != enemyFirepositions.end();i++) {
			std::pair<int, int> p = *i;
			drawBullet(p.first, p.second, r+3, 1, 1, 0);
		}
		if (displayPower) {
			drawPower(powerX, powerY, 3);

		}

		//drawBullet(enemyballX, enemyballY, r + 3, 1, 1, 0);
		for (int i = 1; i <= countHealth; i++) {
			glColor3f(1.0f, 0.0f, 0.0f);
			drawRect(i * 5, 290, 5, 5);
		}
		for (int i = 1; i <= countLives; i++) {
			glColor3f(0.0f, 0.7f, 0.0f);
			drawRect((i * 10), 265, 5, 5);
		}
		drawEnemy();
		glFlush();
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT);
		if (countHealth <= 0) {
			glPushMatrix();
			glColor3f(1, 1, 1);
			char* p0s[20];
			sprintf((char*)p0s, "you won (press any key to restart) ");
			print(110, 150, (char*)p0s);
			glPopMatrix();
		}
		else {
			glPushMatrix();
			glColor3f(1, 1, 1);
			char* p0s[20];
			sprintf((char*)p0s, "you Lost (press any key to restart)");
			print(110, 150, (char*)p0s);
			glPopMatrix();
		}
		glFlush();
	}
}
void drawPower(int x , int y,int r) {
	glPushMatrix();
	glColor3f(1, 0.5, 0);
	glBegin(GL_QUAD_STRIP); //square
	glVertex3f(x + r, y + r, 0.0);
	glVertex3f(x - r, y + r, 0.0);
	glVertex3f(x + r, y - r, 0.0);
	glVertex3f(x - r, y - r, 0.0);

	glEnd();
	glBegin(GL_TRIANGLES);//1ST TRIANGLE
	glVertex3f(x + r, y + r, 0.0);
	glVertex3f(x - r, y + r, 0.0);
	glVertex3f(x , y +2*r, 0.0);
	glEnd();
	glBegin(GL_TRIANGLES);//2nd TRIANGLE
	glVertex3f(x + r, y + r, 0.0);
	glVertex3f(x + r, y - r, 0.0);
	glVertex3f(x+2*r, y, 0.0);
	glEnd();
	glBegin(GL_TRIANGLES);//3rd TRIANGLE
	glVertex3f(x - r, y - r, 0.0);
	glVertex3f(x - r, y + r, 0.0);
	glVertex3f(x - 2 * r, y, 0.0);
	glEnd();
	glBegin(GL_TRIANGLES);//4TH TRIANGLE
	glVertex3f(x + r, y - r, 0.0);
	glVertex3f(x - r, y - r, 0.0);
	glVertex3f(x, y - 2 * r, 0.0);
	glEnd();
	glPopMatrix();
}
void drawPlayer() {
	glPushMatrix();
	glTranslatef(speX, speY, 0);
	glBegin(GL_QUAD_STRIP);
	glColor3f(1.0f, colorPlayer, 1.0);
	glVertex3f((xposition / 2.0) - 10, 0.0f, 0.0f);
	glVertex3f((xposition / 2.0) + 10, 0.0f, 0.0f);
	glVertex3f((xposition / 2.0) - 10, 20.0f, 0.0f);
	glVertex3f((xposition / 2.0) + 10, 20.0f, 0.0f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, colorPlayer, 1.0);
	glVertex3f((xposition / 2.0) - 10, 20.0f, 0.0f);
	glColor3f(1.0f, colorPlayer, 1.0);
	glVertex3f((xposition / 2.0) + 10, 20.0f, 0.0f);
	glColor3f(1.0f, colorPlayer, 1.0);
	glVertex3f((xposition / 2.0), 30.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, colorPlayer, 1.0);
	glVertex3f((xposition / 2.0) - 10, 5.0f, 0.0f);
	glColor3f(1.0f, colorPlayer, 1.0);
	glVertex3f((xposition / 2.0) - 10, 15.0f, 0.0f);
	glColor3f(1.0f, colorPlayer, 1.0);
	glVertex3f((xposition / 2.0) - 20, 10.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, colorPlayer, 1.0);
	glVertex3f((xposition / 2.0) + 10, 5.0f, 0.0f);
	glColor3f(1.0f, colorPlayer,1.0);
	glVertex3f((xposition / 2.0) + 10, 15.0f, 0.0f);
	glColor3f(1.0f, colorPlayer, 1.0);
	glVertex3f((xposition / 2.0) + 20, 10.0f, 0.0f);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0);
	drawRect((xposition / 2.0) - 8, -5.0f, 5, 5);
	drawRect((xposition / 2.0) + 2, -5.0f, 5, 5);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.6f, 0.0f);
	glVertex3f((xposition / 2.0) - 5.5, -5.0f, 0.0f);
	glVertex3f((xposition / 2.0) - 5.5, -8.0f, 0.0f);
	glVertex3f((xposition / 2.0) + 4.5, -5.0f, 0.0f);
	glVertex3f((xposition / 2.0) + 4.5, -8.0f, 0.0f);
	glVertex3f(35.0f, 30.0f, 0.0f);
	glEnd();
	glPopMatrix();
}

void drawEnemy() { // rectangles // points //quad strip //vertex
	glPushMatrix();
	glTranslated(enemyX, 0, 0);
	glBegin(GL_QUAD_STRIP);
	glColor3f(0.5f, 0.3f, 0.0f);
	glVertex3f((xposition / 2.0) - 30, yposition, 0.0f);
	glVertex3f((xposition / 2.0) + 30, yposition, 0.0f);
	glVertex3f((xposition / 2.0) - 30, yposition - 50, 0.0f);
	glVertex3f((xposition / 2.0) + 30, yposition - 50, 0.0f);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	drawRect((xposition / 2.0) - 20, yposition - 23, 10, 10);
	drawRect((xposition / 2.0) + 10, yposition - 23, 10, 10);
	drawRect((xposition / 2.0) - 20, yposition - 45, 40, 4);
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	GLUquadric* quadObj = gluNewQuadric();
	glTranslatef((xposition / 2.0) , yposition - 30, 0);
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
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
void drawBullet(int x, int y, float r , int R ,int G , int B ) {
	glPushMatrix();
	glColor3f(R, G, B);
	glTranslatef(x, y, 0);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}
void KeyUp(unsigned char k, int x, int y) {
	if (k == ' '  && status==0) {
		fire = true;
		sndPlaySound(TEXT("shoot.wav"), SND_ASYNC);
		int ballX = speX + 150;
		int ballY = speY + 40;
		std::pair <int, int> p;
		p.first = ballX;
		p.second = ballY;

		positions.push_back(p);
	}
	else if (status !=0) {
		reset();
	}
	glutPostRedisplay();
}
void reset() {
	status = 0;
    xposition = 300.0;
	yposition = 300.0;
	 countHealth = 58;
	 countLives = 3;
	 displayPower = true;
	 positions.clear();
	 enemyFirepositions.clear();
	 left = true;
	 fire = false;
	 enemyX = 0;
	 speX = 0;
	 speY = 15;
	 colorPlayer = 1;
	 r = 5;
	 toFire = 0;
	 powerX = 5 + (rand() % (290 - 5 + 1));
	 powerY = 5 + (rand() % (200 - 5 + 1));
	 counter = 600;
	 powerUp = false;
	 counter2 = 700;
	 toappear = 500;
	 powerUp = false;
	 removeTimeFlag = false;
	 glutTimerFunc(0, time, 0);
}
bool checkCollison(int x , int y) {
	int xposMin = enemyX + (xposition / 2.0) - 35;
	int xposMax = enemyX + (xposition / 2.0) + 35;
	if ((y >= yposition - 50) && (x <= xposMax && x >= xposMin) ){
		return true;
	}
	return false;
}
bool checkPowerCollison() {
	int xposMin = (xposition / 2.0) - 25 + speX;
	int xposMax = (xposition / 2.0) + 25 + speX;
	int yposMin = speY;
	int yposMax = speY + 40;
	if (powerX <= xposMax && powerX >= xposMin && powerY >= yposMin && powerY <= yposMax) {
		return true;
	
	}
	return false;

}
bool checkBulletEnemyCollision(int x ,int y) {
	int xposMin = (xposition / 2.0) - 20 + speX;
	int xposMax = (xposition / 2.0) + 20 + speX;
	int yposMin = speY;
	int yposMax = speY + 20;
	if (powerUp) return false;
	if (x <= xposMax && x >= xposMin && y >= yposMin && y <= yposMax)
		return true;
	return false;
}
bool checkEndGame() {
	if (countHealth <= 0 || countLives <=0) {
		return true;
	}
	return false;
}
void time(int val)//timer animation function, allows the user to pass an integer valu to the timer function.
{
	if (checkEndGame()) {
		status = 1;
	}
	if (status == 0) {
		toFire--;
		if (toFire <= 0) {
			std::pair<int, int>p;
			p.first = 150 + enemyX;
			p.second = yposition - 57;
			enemyFirepositions.push_back(p);
			toFire = 50+rand()%(60-50+1);
		}
		if (countHealth <= 10) {
			for (int i = 0; i < enemyFirepositions.size(); i++) {
				enemyFirepositions[i].second -= 5;
			}
		}
		else {
			for (int i = 0; i < enemyFirepositions.size(); i++) {
				enemyFirepositions[i].second -= 3;
			}
		}
		for (int i = 0; i < enemyFirepositions.size(); i++) {
			if (checkBulletEnemyCollision(enemyFirepositions[i].first,enemyFirepositions[i].second)) {
				countLives--;
				sndPlaySound(TEXT("explosion.wav"), SND_ASYNC);
				enemyFirepositions.erase(enemyFirepositions.begin() + i);
			}
		}
		for (int i = 0; i < enemyFirepositions.size(); i++) {
			if (enemyFirepositions[i].second <= 5) {
				enemyFirepositions.erase(enemyFirepositions.begin() + i);
			}
		}
		if (checkPowerCollison()) {
			colorPlayer = 0;
			displayPower = false;
			powerUp = true;

		}
		for (int i = 0; i < positions.size();i++)positions[i].second += 4;
		for (int i = 0; i < positions.size();i++) {
			if (checkCollison(positions[i].first,positions[i].second)) {
				countHealth -= 2;
				sndPlaySound(TEXT("explosion.wav"), SND_ASYNC);
				positions.erase(positions.begin() + i);
			}
		}
		
		for (int i = 0; i < positions.size();i++) {
			if (positions[i].second >= 300) {
				positions.erase(positions.begin()+i);
			}
		}
		
		if (powerUp) {
			counter--;
		}
		if (!powerUp && !removeTimeFlag) {
			counter2--;
		}
		if (counter <= 0) {
			counter = 1000;
			counter2 = 700;
			powerUp = false;
			powerX = 5 + (rand() % (290 - 5 + 1));
			powerY = 5 + (rand() % (200 - 5 + 1));
			displayPower = true;
			colorPlayer = 1.0;
		}
		if (counter2 <= 0) {
			displayPower = false;
			removeTimeFlag = true;
			counter2 = 700;
		}
		if (removeTimeFlag) {
			toappear--;
		}
		if (toappear <= 0) {
			removeTimeFlag = false;
			toappear = 500;
			powerX = 5 + (rand() % (290 - 5 + 1));
			powerY = 5 + (rand() % (200 - 5 + 1));
			displayPower = true;
		}
		if (enemyX <= -120) {
			left = !left;
		}
		else if (enemyX >= 120) {
			left = !left;
		}
		if (left) {
			enemyX -= 2;
		}
		else {
			enemyX += 2;
		}				
		glutTimerFunc(1, time, 0);				
		
	}
	glutPostRedisplay();
}
void print(int x, int y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}
void spe(int k, int x, int y)// keyboard special key function takes 3 parameters
// int k: is the special key pressed such as the keyboard arrows the f1,2,3 and so on
{
	if (k == GLUT_KEY_RIGHT && speX <= 130)//if the right arrow is pressed, then the object will be translated in the x axis by 10. (moving right)
		speX += 10;
	if (k == GLUT_KEY_LEFT && speX >= -130)//if the left arrow is pressed, then the object will be translated in the x axis by -10. (moving left)
		speX -= 10;
	if (k == GLUT_KEY_UP && speY <= 190)//if the up arrow is pressed, then the object will be translated in the y axis by 10. (moving upwords)
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
	glutKeyboardUpFunc(KeyUp);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glutSpecialFunc(spe);			//call the keyboard special keys function
	glutTimerFunc(0, time, 0);
	gluOrtho2D(0.0, xposition, 0.0, yposition);
	glutMainLoop();
}


/* Author: Francisco Ulloa
* Date Modified: 9/26/18
*/

#include "fonts.h"
#include <GL/glx.h>
#include "charclass.h"
#include <iostream>
using namespace std;


extern void showFranciscoName(int x, int y)
{
    Rect r;
    r.bot = y - 200;
    r.left = x + 280;
    r.center = 1;
    ggprint8b(&r, 40, 0x00ffff44, "Francisco Ulloa");
}

void showFranciscoPicture(int x, int y, GLuint texid)
{
    float fx = (float)x;
    float fy = (float)y;
    static float angle2 = 0.0f;

    glColor3ub(255, 255, 255);
    int wid = 40;
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glRotatef(angle2, 0.0f, 0.0f, 1.0f);
    angle2 += 0.2;
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
   	glTexCoord2f(0.0f, 0.0f); glVertex2f(-wid, wid);
    	glTexCoord2f(1.0f, 0.0f); glVertex2f(wid, wid);
    	glTexCoord2f(1.0f, 1.0f); glVertex2f(wid, -wid);
    	glTexCoord2f(0.0f, 1.0f); glVertex2f(-wid, -wid);
    glEnd();
    glPopMatrix();
}

void showHelpTab(int x, int y, GLuint texid)
{
    float fx = (float)x;
    float fy = (float)y;

    glColor3ub(255, 255, 255);
    int wid = 40;
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
   	glTexCoord2f(0.0f, 0.0f); glVertex2f(-wid, wid);
    	glTexCoord2f(1.0f, 0.0f); glVertex2f(wid, wid);
    	glTexCoord2f(1.0f, 1.0f); glVertex2f(wid, -wid);
    	glTexCoord2f(0.0f, 1.0f); glVertex2f(-wid, -wid);
    glEnd();
    glPopMatrix();
}

extern void showHelpText(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 1;
    ggprint8b(&r, 40, 0x00ff00, "This is Clyde The Cactus. It doesn ## damage.");
}

void createFloor(int x, int y, GLuint texid)
{

    int width = 16;
    
	glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f); glVertex2f(-width,  -width);
    glTexCoord2f(0.0f, 0.0f); glVertex2f( -width,  width);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( width, width);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(width, -width);

    glEnd();
    glPopMatrix();
}

void createFloorAngle(int x, int y, GLuint texid)
{

    int width = 16;
    
	glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f); glVertex2f(-width,  -width);
    glTexCoord2f(0.0f, 0.0f); glVertex2f( -width,  width);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( width, width);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(width, -width);

    glEnd();
    glPopMatrix();
}

void parachute(Body *p, GLuint texid) 
{
	float parachutePosX = p->positionX;
	float parachutePosY = p->positionY;
	if (p->positionY >=1) {
		glColor3ub(255, 255, 255);
    	int wid = 40;
    	glPushMatrix();
    	glTranslatef(parachutePosX, parachutePosY , 0);
    	//glRotatef(0.0f, 0.0f, 1.0f);
    	glBindTexture(GL_TEXTURE_2D, texid);
    	glBegin(GL_QUADS);
   		glTexCoord2f(0.0f, 0.0f); glVertex2f(-wid, wid);
    	glTexCoord2f(1.0f, 0.0f); glVertex2f(wid, wid);
    	glTexCoord2f(1.0f, 1.0f); glVertex2f(wid, -wid);
    	glTexCoord2f(0.0f, 1.0f); glVertex2f(-wid, -wid);
    	glEnd();
    	glPopMatrix();
    }
}

//screen size 1600x1300
int barrierCollision(Body *p)
{
    if (p->positionX != 0) {
        if (p->positionX > 0 && p->positionX <1600) {
            return p->positionX;
        }
		//if character goes past 20 pixels, 
		//push back to 30 pixels(front of bricks)
        if (p->positionX < 20) {
            return p->positionX = 30;
        }
        //if character goes past 1580 pixels, push back to 1570 pixels
        if (p->positionX > 1580) {
            return p->positionX = 1570;
        }
        
    }return 0;
}
 /*int groundCollision(Body *p, Floor *f) {
     if ((p->positionY)-p->height <= f->yground) {
         return p->positionY;
     }
     if ((p->positionY)-p->height > f->step1 && p->positionX <= 448) {
         p->positionY = 62;
         return p->positionY;
     }
     
 }*/

 void trophy(int x, int y, GLuint texid)
{

    int width = 16;
    
	glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f); glVertex2f(-width,  -width);
    glTexCoord2f(0.0f, 0.0f); glVertex2f( -width,  width);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( width, width);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(width, -width);

    glEnd();
    glPopMatrix();
}


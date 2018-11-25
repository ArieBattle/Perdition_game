//Arielle Battle
//Arielle's game source code
//Description: my source code contains the function for showing my name, 
//	       my picture, moving enemy function, in-game background image, 
//	       collision function and the jump function

	       
#include "fonts.h"
#include <GL/glx.h>
#include <cmath>
#include <cstdio>

//function prototype
void moveEnemy();

void ShowArielleName(int x, int y)
{
    Rect r;
    r.bot = y - 100;
    r.left = x + 280;
    r.center = 1;
    ggprint8b(&r, 40, 0x00ffff44, "Arielle Battle");
}

void showAriellePic(int x, int y, GLuint texid)
{
    static float angle2 = 0.0f;

    glColor3ub(225, 255, 255);
    int wid = 40;
    glPushMatrix();
    glTranslated(x, y, 0);
    glRotatef(angle2, 0.0f, 0.0f, 1.0f);	//rotates the picture
    angle2 += 1;	//the speed of the picture
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glPopMatrix();
}

void showBackground(int x, int y, GLuint texid)
{

    Rect r;
    r.center = 1;

    glColor3ub(255, 255, 255);
    int width = 25;
    glPushMatrix();
    glTranslated(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-width, -width);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-width, width);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(width, width);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(width, -width);
    glEnd();
    glPopMatrix();
}

void moveEnemy() {

    int xres, yres;
    //make enemy move?
    int addgrav = 1;
    //update position
    enemy1.pos[0] += enemy1.vel[0];
    enemy1.pos[1] += enemy1.vel[1];
    /*    //check for collision with window edges
	  if((enemy1.pos[0] < -140.0 && enemy1.vel[0] < 0.0) ||
	  (enemy1.pos[0] >= (float)g.xres+140.0 &&
	  enemy1.vel[0] > 0.0))
	  {
	  enemy1.vel[0] = -enemy1.vel[0];
	  addgrav;
	  }*/
    //gravity
    if (addgrav)
	enemy1.vel[1] -= 0.75;
}

void showEnemy1(int x, int y, GLuint texid)
{
    glColor3ub(255, 255, 255);
    int width = 25;
    glPushMatrix();
    glTranslated(x, y, 0);
    //glTranslatef(-0.5f, -0.5f, 0.0f);	//move the enemy side to side
    //angle += 1;	//the speed of enemy
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-width, -width);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-width, width);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(width, width);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(width, -width);
    glEnd();
    glPopMatrix();

    moveEnemy();
}


void showGoblin(int x, int y, GLuint texid)
{
    glColor3ub(255, 255, 255);
    int width2 = 205;
    glPushMatrix();
    glTranslated(x, y, 0);
    //glRotatef(angle2, 0.0f, 0.0f, 1.0f);	//rotates the picture
    //angle2 += 1;	//the speed of the picture
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-width2, -width2);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-width2, width2);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(width2, width2);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(width2, -width2);
    glEnd();
    glPopMatrix();
}

void menu (int x, int y, GLuint texid)
{
    Rect a;
    Rect b;
    Rect c;
    Rect d;

    a.bot = y;
    a.left = x + 280;
    a.center = 1;

    b.bot = y - 100;
    b.left = x + 280;
    b.center = 1;

    c.bot = y - 150;
    c.left = x + 280;
    c.center = 1;

    d.bot = y - 200;
    d.left = x + 280;
    d.center = 1;

    ggprint8b(&a, 40, 0x00ffff44, "PERDITION");
    ggprint8b(&b, 40, 0x00ffff44, "PRESS S TO START");
    ggprint8b(&c, 40, 0x00ffff44, "PRESS E TO EXIT");
    ggprint8b(&d, 40, 0x00ffff44, "PRESS C FOR CREDITS");
/*
    glColor3ub(255, 255, 255);
    int wid;
    glPushMatrix();
    glTranslated(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glPopMatrix();*/

}

void jump (GLuint texid)
{
    float positionX = 0; //position of character 
    float positionY = 0; //position of character
    float velocityX = 0; 
    float velocityY = -12.0f; //should make jump same height each jump
    float gravity = 2.0f;
    float accX = 0;	//acceleration 
    float accY = 0;

    if (positionY < 300)
	    velocityY += accX;

    else if (positionY > 300)
	    positionY = 300;

    velocityX += accX;
    velocityY += accY;

    positionX += velocityX;
    positionY += velocityY;

    /*
    positionX += velocityX * gravity; //make h.v. to x position
	positionY += velocityY * gravity; //make v.v. to x pos
	velocityY += gravity * timeslice; //apply gravity to come back down
	*/
}

/*
void cleanupRaindrops() {
    Raindrop *s;
    while(rainhead) {
        s = rainhead->next;
        free(rainhead);
        rainhead = s;
    }
    rainhead=NULL;
}

void deleteRain(Raindrop *node) {
    if (node->prev == NULL) {
        if (node->next == NULL) {
            rainhead = NULL;
        }
        } else {
            if (node->next == NULL) {
                node ->prev = NULL;
            } else {
                node->prev->next = node->next;
                node->next->prev = node->prev;
            }
        }
        free(node);
        node = NULL;
}
*/

void ccollision ()
  {

  }

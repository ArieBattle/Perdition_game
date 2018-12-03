//Arielle Battle
//Arielle's game source code
//Description: my source code contains the function for showing my name, 
//	       my picture, moving enemy function, in-game background image, 
//	       collision function and the jump function

#include "charclass.h"	//store class for character to pass reference for jump
#include "fonts.h"
#include <GL/glx.h>
#include <cmath>
#include <cstdio>

#include<iostream>

using namespace std;

void gmaeover(double a, GLuint texid);

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
	glColor3ub(255, 255, 255);
	int width3 = 800;
	glPushMatrix();
	glTranslated(x, y, 0);
	glBindTexture(GL_TEXTURE_2D, texid);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-width3, -width3);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-width3, width3);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(width3, width3);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(width3, -width3);
	glEnd();
	glPopMatrix();
}

void moveEnemy(Enem *e) 
{
	//moving right
	if(e->posX < 1250 && e->posX >= 200 && e->patrol == false)
	{
		e->posX += 0.5;
		if(e->posX >= 1250)
		{
			e->patrol = true;
		}
	}
	//moving left
	else if(e->patrol == true)
	{
		e->posX -= 0.5;
		if(e->posX <= 200)
		{
			e->posX = 200;
			e->patrol = false;
		}
	}
}

void showEnemy1(int x, int y, GLuint texid)
{
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


void showGoblin(int x, int y, GLuint texid)
{
	glColor3ub(255, 255, 255);
	int width2 = 25;
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

void menu (int x, int y)
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

}



void jump (Body *p)
{
	float gravity = 20.0f;
	bool inAir = true;

	if(p->positionY == 0)
	{
		inAir = false;

		if (p->positionY < 400 && inAir == false)
		{
			p->positionY +=(10*gravity);
		}
		else if (p->positionY >= 1200)
			p->positionY = 1200;
	}
}

bool collision (Body *p, Enem *e, bool &gover)
{
  	if((int)sqrt((pow(((int)e->posX - p->positionX),2)) + (pow((((int)e->posY) - p->positionY),2))) < 1)
	{
		cout << "collision" << endl;
		gover = true;
		return true;
	}		
	else
		return false;
}

void gameover (int x, int y, GLuint texid)
{
		
		glColor3ub(255, 255, 255);
		int widt = 800;;
		glPushMatrix();
		glTranslated(x, y, 0);
		glBindTexture(GL_TEXTURE_2D, texid);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-widt, -widt);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-widt, widt);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(widt, widt);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(widt, -widt);
		glEnd();
		glPopMatrix();
}

void fallingObj (Fall *O)
{

	O->pX = ((rand() % 1250) + 1);

	O->pY = 0.5 * O->grav;
	
	
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


   void healthDrop(int health)
   {


   }
   */




//Arielle Battle
//Arielle's game source code
//Description: my source code contains the function for showing my name, 
//	       my picture, and the jump function in the main walk2.cpp file

#include "fonts.h"
#include <GL/glx.h>

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

void menu (int x, int y)
{
	Rect a;
	Rect b;
	Rect c;
	Rect d;
	

	a.bot = y;
	a.left = x;
	a.center = 1;

	b.bot = y - 100;
	b.left = x + 280;
	b.center = 1;

	c.bot = y - 200;
	c.left = x + 380;
	c.center = 1;

	d.bot = y - 300;
	d.left = x + 480;
	d.center = 1;

	ggprint8b(&a, 40, 0x00ffff44, "PERDITION");
	ggprint8b(&b, 40, 0x00ffff44, "START");
	ggprint8b(&c, 40, 0x00ffff44, "EXIT");
	ggprint8b(&d, 40, 0x00ffff44, "CREDITS");
}

/*void jump (const float gravity, const float timeslice)
  {
  float positionX, positionY; //position of character
  float velocityX, 
  float velocityY = -12.0f; //should make jump same height each jump
//float gravity = 0.5f;
do
{
positionX += velocityX * time; //make h.v. to x position
position += velocirtY * time; //make v.v. to x pos
velocityY += gravity * time; //apply gravity to come back down
}while (getchar() != 32 || getchar() != ' ');
}*/

/*void init_opengl(int x, int y)
  {
  }*/
/*void ccollision ()
  {

  }*/

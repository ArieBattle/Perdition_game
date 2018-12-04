// author: Anahi Vargas
// date: 11/11/2018

/*** INCLUDE ***/
#include <GL/glx.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <exception>

#include "fonts.h"
#include "log.h"
#include "globalTypes.h"

/*** MACROS ***/
#define BUTTON_TEXT_COLOR 0x00ffffff
#define BUTTON_SPACING 90 // px
#define BUTTON_Y_OFFSET -100 // px
// maybe put an offset for x
#define BUTTON_WIDTH 200 // px
#define BUTTON_HEIGHT 50 // px
#define IMG_BACKGROUND_PATH "./images/mainMenu.gif"

/** GLOBAL **/
extern Global gl;

/*** FUNCTION PROTOTYPES ***/
void checkButtons();
void initMenu();
void play();
void options();
void quit();

// avoids polluting global namespace
namespace anahi {
    /*** CONSTANTS ***/
	ButtonOptions buttOptions[] = {
		{ "Play", BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_TEXT_COLOR, play }, 
		{ "Options", BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_TEXT_COLOR, options}, 
		{ "Quit", BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_TEXT_COLOR, quit },
		{ "\0", 0, 0, 0x0, NULL }
	};
    int initialized, nbuttons;
    GLuint backgroundTex;
    Button buttons[3];
}

void initMenu()
{
    Rect rec;
    Button newButton;
    Image p2(IMG_BACKGROUND_PATH);

    int i = 0;
    int x = gl.xres / 2; // x position at middle
    int y = (gl.yres - (gl.yres / 3)) + BUTTON_Y_OFFSET; // y position at first third portion
    Log("initMenu() -- x: %d, y: %d\n", x, y);
    //main menu background image
    glGenTextures(1, &anahi::backgroundTex);
    glBindTexture(GL_TEXTURE_2D, anahi::backgroundTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, p2.width, p2.height, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, p2.data);

    while (strcmp(anahi::buttOptions[i].text, "\0")) {
        // dimensions of button
        rec.width = anahi::buttOptions[i].width;
        rec.height = anahi::buttOptions[i].height;
        rec.left = x;
        rec.bot = y;
        rec.right = rec.left + rec.width;
        rec.top = rec.bot + rec.height;
        rec.centerx = (rec.left + rec.right) / 2;
        rec.centery = (rec.bot + rec.top) / 2;

      // button attributes and states
        strcpy(newButton.text, anahi::buttOptions[i].text);
        MakeRGBA(newButton.color, 0.4f, 0.4f, 0.7f, 1.0);
	//Black buttons
       // MakeRGBA(newButton.color, 0.1f, 0.1f, 0.1f, 1.0);
        MakeRGBA(newButton.dcolor, newButton.color[0] * 0.5f,
            newButton.color[1] * 0.5f,
            newButton.color[2] * 0.5f,
            1.0
        );
        newButton.id = i;
        newButton.r = rec;
        newButton.down = 0;
        newButton.click = 0;
        newButton.text_color = BUTTON_TEXT_COLOR;
		newButton.onClick = anahi::buttOptions[i].onClick;

        anahi::buttons[i++] = newButton;
        anahi::nbuttons++;
        y -= BUTTON_SPACING;
        Log("initMenu() -- buttId: %d | centerx: %d | centery: %d\n", i, rec.centerx, rec.centery);
    }
}

//adjust button location according to screen size
void calculateButtons() {
    int x = gl.xres / 2; // x position at middle
    int y = (gl.yres - (gl.yres / 3)) + BUTTON_Y_OFFSET; // y position at first third portion

    for (int i = 0; i < anahi::nbuttons; ++i) {
        Button *button = &anahi::buttons[i];

        button->r.width = anahi::buttOptions[i].width;
        button->r.height = anahi::buttOptions[i].height;
        button->r.left = x;
        button->r.bot = y;
        button->r.right = button->r.left + button->r.width;
        button->r.top = button->r.bot + button->r.height;
        button->r.centerx = (button->r.left + button->r.right) / 2;
        button->r.centery = (button->r.bot + button->r.top) / 2;
        y -= BUTTON_SPACING;
        Log("calculateButtons() -- %s -> r.left: %d -- r.right: %d -- r.bot: %d -- r.top: %d\n",
            button->text, button->r.left, button->r.right, button->r.bot, button->r.top);
    }
}

//show credit name
void showAnahiName(int x, int y)
{
    Rect r;

    r.bot = y - 300;
    r.left = x + 280;
    r.center = 1;
    ggprint8b(&r, 40, 0x00ffff44, "Anahi Vargas");
}

//show credit picture
void showAnahiPicture(int x, int y, GLuint texid)
{
    float fx = (float)x;
    float fy  = (float)y;
    int width = 40;

    glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-width,  width);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( width,  width);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( width, -width);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-width, -width);
    glEnd();
    glPopMatrix();
}

//show settings icon top right
void showSettingsIcon(int x, int y, GLuint texid)
{
	float fx = (float)x;
    float fy  = (float)y;
    int width = 35;

    glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
		
    //    glTexCoord2f(0.0f, 1.0f); glVertex2f(-width, -width);
	//	glTexCoord2f(0.0f, 0.0f); glVertex2f(-width,  width);
	//	glTexCoord2f(1.0f, 0.0f); glVertex2f( width,  width);
	//	glTexCoord2f(1.0f, 1.0f); glVertex2f( width, -width);

        glTexCoord2f(0.0f, 0.0f); glVertex2f(-width,  width);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( width,  width);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( width, -width);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-width, -width);
		
	glEnd();
	glPopMatrix();
}

//show settings options
void showSettings(int x, int y)
{
    Rect r;

    r.bot = y - 30;
    r.left = x + 20;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ffff44, "C - Credits");
    ggprint8b(&r, 16, 0x00ffff44, "H - Help");
}

//show game keyboard controls
void showHelp(int x, int y)
{
    Rect r;

	for (int i = 0; i < 8; i++) {
	    r.left = x;
        r.bot = y  - 30;
	    r.center = 0;
	    if (i == 0) {
			ggprint8b(&r, 16, 0x00ffff44, "Game Keyboard Controls Guide");
		} else if (i ==1) {
            ggprint8b(&r, 16, 0x00ffff44, "Walk Cycle - W");
        } else if (i ==2) {
            ggprint8b(&r, 16, 0x00ffff44, "Walk Right - D");
        } else if (i == 3) {
            ggprint8b(&r, 16, 0x00ffff44, "Walk Left - A");
        } else if (i == 4) {
            ggprint8b(&r, 16, 0x00ffff44, "Explosion - E");
        } else if (i == 5) {
            ggprint8b(&r, 16, 0x00ffff44, "Faster	+");
        } else if (i == 6) {
            ggprint8b(&r, 16, 0x00ffff44, "Slower	-");
        } else if (i == 7) {
            ggprint8b(&r, 16, 0x00ffff44, "Jump - Spacebar");
        }
        //ggprint8b(&r, 16, 0x00ffff44, "frame: %i", z);
        y -= 20;
   }
}

void showMenu()
{
    Rect r;

    if (!anahi::initialized) {
        Log("initMenu(): Initializing main menu\n");
        initMenu();
        anahi::initialized = 1;
    }
    checkButtons(); // check state of the buttons on screen
    //draw buttons, highlight the button with mouse cursor over

    // glPushMatrix();
    // glTranslatef(, , 0);
    
    glBindTexture(GL_TEXTURE_2D, anahi::backgroundTex);
    glBegin(GL_QUADS);

        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 0);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(0, gl.yres);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(gl.xres, gl.yres);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(gl.xres, 0);
    glEnd();
    // glPopMatrix();
    for (int i = 0; i < anahi::nbuttons; i++) {
        Button *button = &anahi::buttons[i];

        if (button->over) {
            //draw a red highlight around button
            glColor3f(1.0f, 0.0f, 0.0f);
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
                glVertex2i(button->r.left-2,  button->r.bot-2);
                glVertex2i(button->r.left-2,  button->r.top+2);
                glVertex2i(button->r.right+2, button->r.top+2);
                glVertex2i(button->r.right+2, button->r.bot-2);
                glVertex2i(button->r.left-2,  button->r.bot-2);
            glEnd();
            glLineWidth(1);
        }
        // set color for button
        if (button->down) {
            glColor3fv(button->dcolor);

        } else {
            glColor3fv(button->color);
        }
        // draw a button
        glBegin(GL_QUADS);
            glVertex2i(button->r.left,  button->r.bot);
            glVertex2i(button->r.left,  button->r.top);
            glVertex2i(button->r.right, button->r.top);
            glVertex2i(button->r.right, button->r.bot);
        glEnd();

        // text position
        r.left = button->r.centerx;
        r.bot  = button->r.centery-8;
        r.center = 1;
        // check button state
        // click state means the button is
        if (button->down) {
            ggprint16(&r, 0, button->text_color, "Pressed!");
            Log("%s button clicked\n", button->text);
            try {
              button->onClick();
            } catch (const std::exception& e) {
                Log("Error: Button::onClick id: %d - \
                    Exception caught in event handler:\n\t%s",button->id, e.what());
                throw e;
            }
		} else {
            ggprint16(&r, 0, button->text_color, button->text);
        }
	}
}

//play button will go here
void play()
{
  Log("Play button clicked\n");
  gl.mainMenu = 0;
}

//options button will go here
void options()
{
  Log("Options button clicked\n");
}

//quit button will go here
void quit()
{
  Log("Quit button clicked\n");
  exit(0);
}

// checks if mouse cursor is over a button
void checkButtons()
{
    for (int i = 0; i < anahi::nbuttons; i++) {
        Button *button = &anahi::buttons[i];

        button->over = 0;
        button->down = 0;

        int compliment_y = abs(gl.yres - gl.mouse.y);
        // check for cursor over button
        Log("mouse x: %d -- r.left: %d -- r.right: %d\nmouse y: %d -- r.bot: %d -- r.top: %d\n",
                gl.mouse.x, button->r.left, button->r.right, gl.mouse.y, 
                button->r.bot, button->r.top);
        // Log("mouse x: %d -- r.left: %d -- r.right: %d\nmouse y: %d -- r.bot: %d -- r.top: %d\n",
        //         gl.mouse.x, button->r.left, button->r.right, gl.mouse.y, 
        //         button->r.bot, button->r.top);  
        if (gl.mouse.x >= button->r.left &&
                gl.mouse.x <= button->r.right &&
                compliment_y >= button->r.bot &&
                compliment_y <= button->r.top) {
            button->over = 1;
            button->down = gl.mouse.lbutton || gl.mouse.rbutton ? 1 : 0;
            Log("%s -> down: %d, over: %d\n",
                button->text, button->down, button->over);
        }
    }
}

// author: Anahi Vargas
// date: 12/04/2018

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
#define BUTTON_WIDTH 200 // px
#define BUTTON_HEIGHT 50 // px
#define IMG_BACKGROUND_PATH "./images/mainMenu.gif"
#define GAME_CONTROLS "./images/controls.png"	

// groupId 
#define ID_MENU 0
#define ID_SETTINGS 1
#define ID_CREDITS 2
#define ID_PLAY 10

/** GLOBAL **/
extern Global gl;

/*** FUNCTION PROTOTYPES ***/
void checkButtons();
void initMenu();
void play();
void credits();
void quit();
//void settings();

// avoids polluting global namespace
namespace anahi {
    /*** CONSTANTS ***/
    ButtonOptions buttOptions[] = {
        { "Play", 0.5f, 0.7f, BUTTON_WIDTH, BUTTON_HEIGHT, true, BUTTON_TEXT_COLOR, ID_MENU, play }, 
        { "Credits", 0.5f, 0.6f, BUTTON_WIDTH, BUTTON_HEIGHT, true, BUTTON_TEXT_COLOR, ID_MENU, credits }, 
        { "Quit", 0.5f, 0.5f, BUTTON_WIDTH, BUTTON_HEIGHT, true, BUTTON_TEXT_COLOR, ID_MENU, quit },
        /*{ "Settings", 0.9f, 0.9f, 35, 35, false, 0x0, ID_PLAY, settings },*/
        { "\0", 0.0f, 0.0f, 0, 0, false, 0x0, 0 }
    };

    int activeMenu;
    int initialized, nbuttons;
    GLuint backgroundTex;
    GLuint controlsTex;
    Button buttons[4];
    int buttonGroupIds[4];
}

void initMenu()
{
    Rect rec;
    Button newButton;
    Image p2(IMG_BACKGROUND_PATH);
    Image pc(GAME_CONTROLS);

    int i = 0;
    /*  int x = gl.xres / 2; // x position at middle
        int y = (gl.yres - (gl.yres / 3)) + BUTTON_Y_OFFSET; // y position at first third portion */

    //main menu background image
    glGenTextures(1, &anahi::backgroundTex);
    glBindTexture(GL_TEXTURE_2D, anahi::backgroundTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, p2.width, p2.height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, p2.data);

    //game controls image
    glGenTextures(1, &anahi::controlsTex);
    glBindTexture(GL_TEXTURE_2D, anahi::controlsTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, pc.width, pc.height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, pc.data);

    while (strcmp(anahi::buttOptions[i].text, "\0")) {
        // dimensions of button
        ButtonOptions *options = &anahi::buttOptions[i];

        rec.width = options->width;
        rec.height = options->height;
        rec.left = (int)(gl.xres * options->x);
        rec.bot = (int)(gl.yres * options->y);
        rec.right = rec.left + rec.width;
        rec.top = rec.bot + rec.height;
        rec.centerx = (rec.left + rec.right) / 2;
        rec.centery = (rec.bot + rec.top) / 2;

        //button attributes and states
        strcpy(newButton.text, options->text);	    
        MakeRGBA(newButton.color, 0.1f, 0.1f, 0.1f, 1.0);
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
        newButton.onClick = options->onClick;

        anahi::buttons[i++] = newButton;
        anahi::nbuttons++;
        //y -= BUTTON_SPACING;
        Log("initMenu() -- buttId: %d | centerx: %d | centery: %d\n", i, rec.centerx, rec.centery);
    }
}

/*void calculate() {
  for (int i = 0; i < anahi::nbuttons; ++i) {
  Button *button = &anahi::buttons[i];

  button->r.left = (int)(gl.xres * options->x);
  button->r.bot = (int)(gl.yres * options->y);
  button->r.right = button->r.left + button->r.width;
  button->r.top = button->r.bot + button->r.height;
  button->r.centerx = (button->r.left + button->r.right) / 2;
  button->r.centery = (button->r.bot + button->r.top) / 2;
  Log("calculateButtons() -- %s -> r.left: %d -- r.right: %d -- r.bot: %d -- r.top: %d\n",
  button->text, button->r.left, button->r.right, button->r.bot, button->r.top);
  }
  }
  */

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

    glTexCoord2f(0.0f, 0.0f); glVertex2f(-width,  width);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( width,  width);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( width, -width);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-width, -width);

    glEnd();
    glPopMatrix();
}

/*//settings button
  void settings()
  {
  anahi::activeMenu = ID_SETTINGS;
  }
  */

//show game keyboard controls
void showControls(int x, int y)
{
#ifdef GAME_CONTROLS
    float fx = (float)x;
    float fy  = (float)y;
    int width = 250;
    int height = 200;

    glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glBindTexture(GL_TEXTURE_2D, anahi::controlsTex);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-width,  height); 
    glTexCoord2f(1.0f, 0.0f); glVertex2f( width,  height);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( width, -height);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-width, -height);
    glEnd();
    glPopMatrix();

#else
    Rect r;

    for (int i = 0; i < 8; i++) {
        r.left = x;
        r.bot = y  - 30;
        r.center = 0;
        if (i == 0) {
            ggprint8b(&r, 16, 0x00ffff44, "Game Keyboard Controls Guide");
        } else if (i ==1) {
            ggprint8b(&r, 16, 0x00ffff44, "Walk Cycle	W");
        } else if (i ==2) {
            ggprint8b(&r, 16, 0x00ffff44, "Walk Right	->");
        } else if (i == 3) {
            ggprint8b(&r, 16, 0x00ffff44, "Walk Left	<-");
        } else if (i == 5) {
            ggprint8b(&r, 16, 0x00ffff44, "Faster	+");
        } else if (i == 6) {
            ggprint8b(&r, 16, 0x00ffff44, "Slower	-");
        } else if (i == 7) {
            ggprint8b(&r, 16, 0x00ffff44, "Jump		Spacebar");
        }

        y -= 20;
    }
#endif  
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

    glBindTexture(GL_TEXTURE_2D, anahi::backgroundTex);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(0, gl.yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(gl.xres, gl.yres);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(gl.xres, 0);
    glEnd();

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
    anahi::activeMenu = ID_PLAY;
}

//credits button will go here
void credits()
{
    Log("Credits button clicked\n");
    gl.credits = 1;
    anahi::activeMenu = ID_CREDITS;
    gl.mainMenu = 0;
    anahi::activeMenu = ID_MENU;
    checkButtons();
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
        Log("checkButtons() -- &gl.mouse: %x\n", &gl.mouse);
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

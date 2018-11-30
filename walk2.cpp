//3350
//program: PERDITION.cpp
//author:  GROUP 2
//date:    fall 2018
//
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//framework courtesy: Gordon Giesel
//
//This program includes:
//  multiple sprite-sheet animations
//  a level tiling system
//  parallax scrolling of backgrounds
//
#include <stdio.h>
#include <iostream>
#include <charclass.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "charclass.h"
//#include "ppm.h"
#include "fonts.h"
#include </usr/include/AL/alut.h> //Sound Function Added
using namespace std;
//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(v, x, y, z) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
			     (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
#define SPACE_BAR 0x20

//constants
const float timeslice = 1.0f;
const float gravity = -0.4f;
#define ALPHA 1

//function prototypes

typedef struct t_mouse {
    int eventType;
    int lbutton;
    int rbutton;
    int x;
    int y;
} Mouse;


bool push_start = false;
void initOpengl();
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void physics();
void render();
//extern void functions
extern void sound_test();
int i = 15;
int health = 100;
//-----------------------------------------------------------------------------
//Setup timers
class Timers {
	public:
		double physicsRate;
		double oobillion;
		struct timespec timeStart, timeEnd, timeCurrent;
		struct timespec walkTime;
		Timers() {
			physicsRate = 1.0 / 30.0;
			oobillion = 1.0 / 1e9;
		}
		double timeDiff(struct timespec *start, struct timespec *end) {
			return (double)(end->tv_sec - start->tv_sec ) +
				(double)(end->tv_nsec - start->tv_nsec) * oobillion;
		}
		void timeCopy(struct timespec *dest, struct timespec *source) {
			memcpy(dest, source, sizeof(struct timespec));
		}
		void recordTime(struct timespec *t) {
			clock_gettime(CLOCK_REALTIME, t);
		}

} timers;
//-----------------------------------------------------------------------------

class Image;

Enem *enemy1;
Enem *enemy2;
Body *player;

class Sprite {
	public:
		int onoff;
		int frame;
		double delay;
		Vec pos;
		Image *image;
		GLuint tex;
		struct timespec time;
		Sprite() {
			onoff = 0;
			frame = 0;
			image = NULL;
			delay = 0.1;
		}
};

class Global {
	public:
		unsigned char keys[65536];
		int xres, yres;
		int movie, movieStep;
		int walk;
		int credits;
		int walkFrame;
		int settings;
		int helpTab;
		int showRain;
		double delay;
		bool gameover;
		Image *walkImage;
		GLuint walkTexture;
		GLuint mariogm734Texture;
		GLuint tinaTexture;
		GLuint animeTexture;
		GLuint jeremyTexture;
		GLuint cactusTexture;
		GLuint enemy1Texture;
		GLuint goblinTexture;
		GLuint gameoverTexture;
		GLuint settings_icon_Texture;
		GLuint perditionTexture;
		GLuint bloodsplatterTexture;
		Vec box[20];
		Sprite exp;
		Sprite exp44;
		Vec ball_pos;
		Vec ball_vel;
		//camera is centered at (0,0) lower-left of screen. 
		Flt camera[2];
		~Global() {
			logClose();
		}
		Global() {
			logOpen();
			showRain = 0;
			camera[0] = camera[1] = 0.0;
			movie=0;
			movieStep=2;
			xres=1600;
			yres=1300;
			walk=0;
			gameover = false;
			credits =0;
			settings = 0;
			walkFrame=0;
			walkImage=NULL;
			MakeVector(ball_pos, 520.0, 0, 0);
			MakeVector(ball_vel, 0, 0, 0);
			delay = 0.1;
			exp.onoff=0;
			exp.frame=0;
			exp.image=NULL;
			exp.delay = 0.02;
			exp44.onoff=0;
			exp44.frame=0;
			exp44.image=NULL;
			exp44.delay = 0.022;
			for (int i=0; i<20; i++) {
				box[i][0] = rnd() * xres;
				box[i][1] = rnd() * (yres-220) + 220.0;
				box[i][2] = 0.0;
			}
			memset(keys, 0, 65536);
			//
		}
} gl;

class Raindrop {
	public:
		int type;
		int linewidth;
		Vec pos;
		Vec lastpos;
		Vec vel;
		Vec maxvel;
		Vec force;
		float length;
		float color[4];
		Raindrop *prev;
		Raindrop *next;
} *rainhad = NULL;
int ndrops = 1;
int totrain = 0;
int maxrain = 0;
void deleteRain(Raindrop *node);
void cleanupRaindrops(void);
void collisions(Body *);

class Level {
	public:
		unsigned char arr[16][80];
		int nrows, ncols;
		int tilesize[2];
		Flt ftsz[2];
		Flt tile_base;
		Level() {
			//Log("Level constructor\n");
			tilesize[0] = 32;
			tilesize[1] = 32;
			ftsz[0] = (Flt)tilesize[0];
			ftsz[1] = (Flt)tilesize[1];
			tile_base = 220.0;
			//read level
			FILE *fpi = fopen("level1.txt","r");
			if (fpi) {
				nrows=0;
				char line[100];
				while (fgets(line, 100, fpi) != NULL) {
					removeCrLf(line);
					int slen = strlen(line);
					ncols = slen;
					//Log("line: %s\n", line);
					for (int j=0; j<slen; j++) {
						arr[nrows][j] = line[j];
					}
					++nrows;
				}
				fclose(fpi);
				//printf("nrows of background data: %i\n", nrows);
			}
			for (int i=0; i<nrows; i++) {
				for (int j=0; j<ncols; j++) {
					printf("%c", arr[i][j]);
				}
				printf("\n");
			}
		}
		void removeCrLf(char *str) {
			//remove carriage return and linefeed from a Cstring
			char *p = str;
			while (*p) {
				if (*p == 10 || *p == 13) {
					*p = '\0';
					break;
				}
				++p;
			}
		}

} lev;

//X Windows variables
class X11_wrapper {

	private:
		Display *dpy;
		Window win;
	public:
		~X11_wrapper() {
			XDestroyWindow(dpy, win);
			XCloseDisplay(dpy);
		}
		void setTitle() {
			//Set the window title bar.
			XMapWindow(dpy, win);
			XStoreName(dpy, win, "3350 - PERDITION");
		}
		void setupScreenRes(const int w, const int h) {
			gl.xres = w;
			gl.yres = h;
		}
		X11_wrapper() {
			GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
			//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
			XSetWindowAttributes swa;
			setupScreenRes(gl.xres, gl.yres);
			dpy = XOpenDisplay(NULL);
			if (dpy == NULL) {
				printf("\n\tcannot connect to X server\n\n");
				exit(EXIT_FAILURE);
			}
			Window root = DefaultRootWindow(dpy);
			XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
			if (vi == NULL) {
				printf("\n\tno appropriate visual found\n\n");
				exit(EXIT_FAILURE);
			} 
			Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
			swa.colormap = cmap;
			swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
				StructureNotifyMask | SubstructureNotifyMask;
			win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
					vi->depth, InputOutput, vi->visual,
					CWColormap | CWEventMask, &swa);
			GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
			glXMakeCurrent(dpy, win, glc);
			setTitle();
		}
		void reshapeWindow(int width, int height) {
			//window has been resized.
			setupScreenRes(width, height);
			glViewport(0, 0, (GLint)width, (GLint)height);
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
			glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
			setTitle();
		}
		void checkResize(XEvent *e) {
			//The ConfigureNotify is sent by the
			//server if the window is resized.
			if (e->type != ConfigureNotify)
				return;
			XConfigureEvent xce = e->xconfigure;
			if (xce.width != gl.xres || xce.height != gl.yres) {
				//Window size did change.
				reshapeWindow(xce.width, xce.height);
			}
		}
		bool getXPending() {
			return XPending(dpy);
		}
		XEvent getXNextEvent() {
			XEvent e;
			XNextEvent(dpy, &e);
			return e;
		}
		void swapBuffers() {
			glXSwapBuffers(dpy, win);
		}
} x11;

class Image {
	public:
		int width, height;
		unsigned char *data;
		~Image() { delete [] data; }
		Image(const char *fname) {
			if (fname[0] == '\0')
				return;
			//printf("fname **%s**\n", fname);
			int ppmFlag = 0;
			char name[40];
			strcpy(name, fname);
			int slen = strlen(name);
			char ppmname[80];
			if (strncmp(name+(slen-4), ".ppm", 4) == 0)
				ppmFlag = 1;
			if (ppmFlag) {
				strcpy(ppmname, name);
			} else {
				name[slen-4] = '\0';
				//printf("name **%s**\n", name);
				sprintf(ppmname,"%s.ppm", name);
				//printf("ppmname **%s**\n", ppmname);
				char ts[100];
				//system("convert eball.jpg eball.ppm");
				sprintf(ts, "convert %s %s", fname, ppmname);
				system(ts);
			}
			//sprintf(ts, "%s", name);
			//printf("read ppm **%s**\n", ppmname); fflush(stdout);
			FILE *fpi = fopen(ppmname, "r");
			if (fpi) {
				char line[200];
				fgets(line, 200, fpi);
				fgets(line, 200, fpi);
				//skip comments and blank lines
				while (line[0] == '#' || strlen(line) < 2)
					fgets(line, 200, fpi);
				sscanf(line, "%i %i", &width, &height);
				fgets(line, 200, fpi);
				//get pixel data
				int n = width * height * 3;			
				data = new unsigned char[n];			
				for (int i=0; i<n; i++)
					data[i] = fgetc(fpi);
				fclose(fpi);
			} else {
				printf("ERROR opening image: %s\n",ppmname);
				exit(0);
			}
			if (!ppmFlag)
				unlink(ppmname);
		}
};
Image img[13] = {
	"./images/walk.gif",
	"./images/exp.png",
	"./images/exp44.png",
	"./images/mariogm734.png",
	"./images/anime.png",
	"./images/jeremy.gif",
	"./images/tina.png",
	"./images/cactus.png",
	"./images/enemy1.png",
	"./images/goblin.png",
	"./images/settings_icon.png",
	"./images/perdition.png",
	"./images/gameover.png"};



int main(void)
{
	initOpengl();
	init();
	player = new Body();
	enemy1 = new Enem(200);
	enemy2 = new Enem(700);
	int done = 0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.checkResize(&e);
			checkMouse(&e);
			done = checkKeys(&e);
		}
		collisions(player);
		render();
		extern bool collision(Body *p, Enem*e, bool &go);
		collision(player, enemy1, gl.gameover);
		collision(player, enemy2, gl.gameover);
		x11.swapBuffers();
    cleanup_fonts();
  }
    return 0;
}

unsigned char *buildAlphaData(Image *img)
{
    //add 4th component to RGB stream...
    int i;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    unsigned char a,b,c;
    //use the first pixel in the image as the transparent color.
    unsigned char t0 = *(data+0);
    unsigned char t1 = *(data+1);
    unsigned char t2 = *(data+2);
    for (i=0; i<img->width * img->height * 3; i+=3) {
	a = *(data+0);
	b = *(data+1);
	c = *(data+2);
	*(ptr+0) = a;
	*(ptr+1) = b;
	*(ptr+2) = c;
	*(ptr+3) = 1;
	if (a==t0 && b==t1 && c==t2)
	    *(ptr+3) = 0;
	//-----------------------------------------------
	ptr += 4;
	data += 3;
    }
    return newdata;
}

void initOpengl(void)
{
	//-------------------------------------------------------------------------
	glGenTextures(1, &gl.mariogm734Texture);
	//-------------------------------------------------------------------------
	//mario texture
	//
	int w_mario = img[3].width;
	int h_mario = img[3].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.mariogm734Texture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w_mario, h_mario, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[3].data);
	//-------------------------------------------------------------------------
	//


	glGenTextures(1, &gl.animeTexture);
	//-------------------------------------------------------------------------
	//anime texture
	//
	int w_anime = img[4].width;
	int h_anime = img[4].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.animeTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w_anime, h_anime, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[4].data);
	//-------------------------------------------------------------------------
	//


	glGenTextures(1, &gl.jeremyTexture);
	//-------------------------------------------------------------------------
	//jeremy texture
	//
	int w_jeremy = img[5].width;
	int h_jeremy = img[5].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.jeremyTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w_jeremy, h_jeremy, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[5].data);
	//-------------------------------------------------------------------------


	//OpenGL initialization
	glGenTextures(1, &gl.tinaTexture);
	//-------------------------------------------------------------------------
	//tina texture
	//
	int w_tina = img[6].width;
	int h_tina  = img[6].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.tinaTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w_tina, h_tina, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[6].data);
	//-------------------------------------------------------------------------

	//helpTab
	//cactus texture
	//
	glGenTextures(1, &gl.cactusTexture);

	int w_cactus = img[7].width;
	int h_cactus = img[7].height;

	glBindTexture(GL_TEXTURE_2D, gl.cactusTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w_cactus, h_cactus, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[7].data);
	//-------------------------------------------------------------------------


	glGenTextures(1, &gl.enemy1Texture);
	//-------------------------------------------------------------------------
	//enemy1 texture
	//
	int w_enemy1 = img[8].width;
	int h_enemy1 = img[8].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.enemy1Texture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w_enemy1, h_enemy1, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[8].data);
	glGenTextures(1, &gl.settings_icon_Texture);
	//-------------------------------------------------------------------------


	glGenTextures(1, &gl.goblinTexture);
	//-------------------------------------------------------------------------
	//goblin texture
	//
	int w_goblin = img[9].width;
	int h_goblin = img[9].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.goblinTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w_goblin, h_goblin, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[9].data);


	//settings icon texture
	//
	int w_settings_icon = img[10].width;
	int h_settings_icon  = img[10].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.settings_icon_Texture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w_settings_icon, h_settings_icon, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[10].data);
	//-------------------------------------------------------------------------
	//perdition texture - title screen

	glGenTextures(1, &gl.perditionTexture);
	//-------------------------------------------------------------------------
	int w_p = img[11].width;
	int h_p = img[11].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.perditionTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w_p, h_p, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[11].data);
	//--------------------------------------------------------------------------- 
	
	glGenTextures(1, &gl.bloodsplatterTexture);
	//-------------------------------------------------------------------------
	//jeremy texture
	//
	int w_g = img[12].width;
	int h_g = img[12].height;
	//
	glBindTexture(GL_TEXTURE_2D, gl.gameoverTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w_g, h_g, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[12].data);
	//-------------------------------------------------------------------------

	
	glViewport(0, 0, gl.xres, gl.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//load the images file into a ppm structure.
	//
	int w = img[0].width;
	int h = img[0].height;
	//
	//create opengl texture elements
	glGenTextures(1, &gl.walkTexture);
	//-------------------------------------------------------------------------
	//silhouette
	//this is similar to a sprite graphic
	//
	glBindTexture(GL_TEXTURE_2D, gl.walkTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *walkData = buildAlphaData(&img[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	free(walkData);
	//-------------------------------------------------------------------------
	//create opengl texture elements
	w = img[1].width;
	h = img[1].height;
	glGenTextures(1, &gl.exp.tex);
	//-------------------------------------------------------------------------
	//this is similar to a sprite graphic
	glBindTexture(GL_TEXTURE_2D, gl.exp.tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//must build a new set of data...
	unsigned char *xData = buildAlphaData(&img[1]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, xData);
	free(xData);
	//-------------------------------------------------------------------------
	w = img[2].width;
	h = img[2].height;
	//create opengl texture elements
	glGenTextures(1, &gl.exp44.tex);
	//-------------------------------------------------------------------------
	//this is similar to a sprite graphic
	glBindTexture(GL_TEXTURE_2D, gl.exp44.tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//must build a new set of data...
	xData = buildAlphaData(&img[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, xData);
	free(xData);

}

void init() {

}

void checkMouse(XEvent *e)
{
	//printf("checkMouse()...\n"); fflush(stdout);
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type != ButtonRelease && e->type != ButtonPress &&
			e->type != MotionNotify)
		return;
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {

		if (e->xbutton.button==1) {
			//Left button is down

			push_start = true;

		}
		if (e->xbutton.button==3) {
			//Right button is down
			push_start = true;

		}
	}
	if (e->type == MotionNotify) {
		if (savex != e->xbutton.x || savey != e->xbutton.y) {
			//Mouse moved
			savex = e->xbutton.x;
			savey = e->xbutton.y;
		}
	}

}

void screenCapture()
{
	static int fnum = 0;
	static int vid = 0;
	if (!vid) {
		system("mkdir ./vid");
		vid = 1;
	}
	unsigned char *data = (unsigned char *)malloc(gl.xres * gl.yres * 3);
	glReadPixels(0, 0, gl.xres, gl.yres, GL_RGB, GL_UNSIGNED_BYTE, data);
	char ts[32];
	sprintf(ts, "./vid/pic%03i.ppm", fnum);
	FILE *fpo = fopen(ts,"w");	
	if (fpo) {
		fprintf(fpo, "P6\n%i %i\n255\n", gl.xres, gl.yres);
		unsigned char *p = data;
		//go backwards a row at a time...
		p = p + ((gl.yres-1) * gl.xres * 3);
		unsigned char *start = p;
		for (int i=0; i<gl.yres; i++) {
			for (int j=0; j<gl.xres*3; j++) {
				fprintf(fpo, "%c",*p);
				++p;
			}
			start = start - (gl.xres*3);
			p = start;
		}
		fclose(fpo);
		char s[256];
		sprintf(s, "convert ./vid/pic%03i.ppm ./vid/pic%03i.gif", fnum, fnum);
		system(s);
		unlink(ts);
	}
	++fnum;
}

int checkKeys(XEvent *e)
{
	//keyboard input?
	static int shift=0;
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = XLookupKeysym(&e->xkey, 0);
	gl.keys[key]=1;
	if (e->type == KeyRelease) {
		gl.keys[key]=0;
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return 0;
	}
	gl.keys[key]=1;
	if (key == XK_Shift_L || key == XK_Shift_R) {
		shift=1;
		return 0;
	}
	(void)shift;
	switch (key) {
		case XK_s:
			screenCapture();
			push_start = true;
			break;
		case XK_m:
			gl.movie ^= 1;
			break;
		case XK_w:
			timers.recordTime(&timers.walkTime);
			gl.walk ^= 1;

			/*
			   extern void Rcollision(int x, int y,Body *p, Enem *e, GLuint texid);

			   if (gl.keys[XK_w]) {
			   Rcollision(1600/2, 1300/2, player, enem, gl.bloodsplatterTexture);
			   }*/
			break;
		case XK_e:
			exit(0);
			break;
		case XK_f:
			gl.exp44.pos[0] = 200.0;
			gl.exp44.pos[1] = -60.0;
			gl.exp44.pos[2] =   0.0;
			timers.recordTime(&gl.exp44.time);
			gl.exp44.onoff ^= 1;
			break;
		case XK_n:
			extern void sound_test();
			break;
		case XK_Left:
			player->positionX -= 5;
			break;
		case XK_Right:
			player->positionX += 5;
			break;
		case XK_Up:
			break;
		case XK_Down:
			break;
		case XK_equal:
			gl.delay -= 0.005;
			if (gl.delay < 0.005)
				gl.delay = 0.005;
			break;
		case XK_minus:
			gl.delay += 0.005;
			break;
		case XK_Escape:
			return 1;
			break;
		case XK_o:
			gl.settings ^= 1;
			break;
		case XK_c:
			gl.credits ^= 1;
			break;
		case XK_h:
			gl.helpTab ^= 1;
			break;	
		case XK_space:
			//if spacebar is hit jump (?)
				if (gl.keys[XK_space]) {
					extern void jump(Body *p);
					jump(player);
				}
			break;
	}
	return 0;
}

Flt VecNormalize(Vec vec)
{
	Flt len, tlen;
	Flt xlen = vec[0];
	Flt ylen = vec[1];
	Flt zlen = vec[2];
	len = xlen*xlen + ylen*ylen + zlen*zlen;
	if (len == 0.0) {
		MakeVector(vec, 0.0, 0.0, 1.0);
		return 1.0;
	}
	len = sqrt(len);
	tlen = 1.0 / len;
	vec[0] = xlen * tlen;
	vec[1] = ylen * tlen;
	vec[2] = zlen * tlen;
	return(len);
}

void collisions(Body *player)
{
	player->positionY += gravity;
	//test for the character staying on the screen
	if(player->positionY < 0)
	{
		player->positionY = 0;
	}

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

void render(void)
{	
	if(!push_start)	{

		extern void menu(int x, int y);
		menu(100, gl.yres-155);

		if (gl.credits) {

			//display names
			extern void showFranciscoName(int x, int y);
			extern void showAnahiName(int x, int y);
			extern void showTheodoreName(int x, int y);
			extern void ShowArielleName(int x, int y);

			showFranciscoName(100, gl.yres-155);
			showAnahiName(100, gl.yres-175);
			showTheodoreName(100, gl.yres-105);
			ShowArielleName(100, gl.yres-135);

			//displays images
			extern void showAnahiPicture(int x, int y, GLuint texid);
			extern void showFranciscoPicture(int x, int y, GLuint texid);
			extern void showTheodorePicture(int x, int y, GLuint texid);
			extern void showAriellePic(int x, int y, GLuint texid);

			showAnahiPicture(250, gl.yres-475, gl.tinaTexture);
			showFranciscoPicture(250, gl.yres-350, gl.jeremyTexture);
			showTheodorePicture(250, gl.yres-100, gl.mariogm734Texture);
			showAriellePic(250, gl.yres-220, gl.animeTexture);

			return;
		}
	} else if (gl.gameover == false) {
		Rect r;
		//Clear the screen
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		extern void showBackground(int x, int y, GLuint texid);
		showBackground(1600/2, 1300/2, gl.perditionTexture);

		// show settings icon top right
		extern void showSettingsIcon(int x, int y, GLuint texid);
		showSettingsIcon(gl.xres-30, gl.yres-30, gl.settings_icon_Texture);

		//display settings
		if (gl.settings) {
			extern void showSettings(int x, int y);
			showSettings(100, gl.yres-100);
			return;
		}

		if (gl.helpTab) {
			extern void showHelpTab(int x, int y, GLuint texid);
			extern void showHelpText(int x, int y);
			extern void showHelp(int x, int y);
			showHelpTab(250, 475, gl.cactusTexture);
			showHelpText(450, 450); 
			showHelp(50, gl.yres-60);
			return;
		}

		//float cx = gl.xres/2.0;
		//float cy = gl.yres/2.0;
		//


		//
		//this is for the player

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glTranslatef(player->positionX+100, player->positionY+115, 0.0f);
		glBindTexture(GL_TEXTURE_2D, gl.walkTexture);

		//added for other walking pics	
		int ix = gl.walkFrame % 8;
		int iy = 0;
		if (gl.walkFrame >= 8)
			iy = 1;
		float fx = (float)ix / 8.0;
		float fy = (float)iy / 2.0;

		glBegin(GL_QUADS);

		if (gl.keys[XK_Left]) {
			glTexCoord2f(fx+.125, fy+.5); glVertex2i(-player->width, -player->height);
			glTexCoord2f(fx+.125, fy);    glVertex2i(-player->width, player->height);
			glTexCoord2f(fx, fy);    glVertex2i(player->width, player->height);
			glTexCoord2f(fx, fy+.5); glVertex2i(player->width, -player->height);
		} else {
			glTexCoord2f(fx, fy+.5); glVertex2i(-player->width, -player->height);
			glTexCoord2f(fx, fy);    glVertex2i(-player->width, player->height);
			glTexCoord2f(fx+.125, fy);    glVertex2i(player->width, player->height);
			glTexCoord2f(fx+.125, fy+.5); glVertex2i(player->width, -player->height);
		}

		/*
		   glTexCoord2f(0.0f, 1.0f); glVertex2i(-player->width, -player->height);
		   glTexCoord2f(0.0f, 0.0f); glVertex2i(-player->width, player->height);
		   glTexCoord2f(0.125f, 0.0f); glVertex2i(player->width, player->height);
		   glTexCoord2f(0.125f, 1.0f); glVertex2i(player->width, -player->height);*/
		glEnd();
		glPopMatrix();

		
		//this is for the enemy1
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glTranslatef(enemy1->posX, enemy1->posY+70, 0.0f);
		glBindTexture(GL_TEXTURE_2D, gl.enemy1Texture);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-enemy1->wid, -enemy1->hgt);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-enemy1->wid, enemy1->hgt);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(enemy1->wid, enemy1->hgt);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(enemy1->wid, -enemy1->hgt);
		glEnd();
		glPopMatrix();

				//this is for the enem2
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glTranslatef(enemy2->posX, enemy2->posY+70, 0.0f);
		glBindTexture(GL_TEXTURE_2D, gl.enemy1Texture);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-enemy2->wid, -enemy2->hgt);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-enemy2->wid, enemy2->hgt);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(enemy2->wid, enemy2->hgt);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(enemy2->wid, -enemy2->hgt);
		glEnd();
		glPopMatrix();

/*
		//move enemy back and fourth on screen
		extern void moveEnemy(Enem *e);
		moveEnemy(enem);

		//show enemies	
		//extern void showEnemy1(int x, int y, GLuint Texid);
		//showEnemy1(500, 30, gl.enemy1Texture);

		extern void showGoblin(int x, int y, GLuint Texid);
		showGoblin(700, 30, gl.goblinTexture);
*/		

		r.bot = gl.yres - 20;
		r.left = 10;
		r.center = 0;
		ggprint8b(&r, 16, 0x00ffff44, "H    	Help/Info");
		ggprint8b(&r, 16, 0x00ffff44, "N        Sound Test");
		ggprint8b(&r, 16, 0x00ffff44, "O    	Settings");
		ggprint8b(&r, 16, 0x00ffff44, "E        Exit");

		if (gl.movie) {
			screenCapture();
		}
	}
	else if (gl.gameover == true)
	{
		cout << "gameover" << endl;
		extern void gameover(int x, int y, GLuint texid);
		gameover(1600/2, 1300/2, gl.gameoverTexture);
	}
}

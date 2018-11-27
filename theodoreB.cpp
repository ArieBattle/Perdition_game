//Credits Function & My Sourcefile for Game Project: Perdition
//By: Theodore Barcelona
//Group #: 2
//Updated: 11/20/18
//Theo's game source code
//Description: show name/picture in credits, plays sounds/music
#include <math.h>
#include <GL/glx.h>
#include "fonts.h"
//Sound Library Functions
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
//#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
//#endif

void showTheodoreName(int x, int y)
{
    //show name and position
    Rect r;
    r.bot = y - 0;
    r.left = x + 280;
    r.center = 1;
    ggprint8b(&r, 40, 0x00ffff44, "Theodore Barcelona");
}
void showTheodorePicture(int x, int y, GLuint texid)
{
    //show picture and position
    float fx = (float)x;
    float fy = (float)y;
    glColor3ub(255, 255, 255);
    
    int wid=40;
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
         glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid,-wid);
         glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
         glTexCoord2f(1.0f, 1.0f); glVertex2i( wid, wid);
         glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
    glEnd();
    glPopMatrix();
}
//Sound Implementation
//Original Source: OpenAl example program
//Original Author: Gordon Griesel
void sound_test()
{
    // Soundtest only
//#ifdef USE_OPENAL_SOUND
    alutInit(0, NULL);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: alutInit()\n");
    }
    //Clear error state.
    alGetError();

    //Setup the listener.
    //Forward and up vectors are used.
    float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, vec);
    alListenerf(AL_GAIN, 1.0f);

    //Buffer holds the sound information.
    ALuint alBuffer;
    alBuffer = alutCreateBufferFromFile("test.wav");

    //Source refers to the sound.
    ALuint alSource;
    //Generate a source, and store it in a buffer.
    alGenSources(1, &alSource);
    alSourcei(alSource, AL_BUFFER, alBuffer);
    //Set volume and pitch to normal, no looping of sound.
    alSourcef(alSource, AL_GAIN, 1.0f);
    alSourcef(alSource, AL_PITCH, 1.0f);
    alSourcei(alSource, AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: setting source\n");
    }
    //for (int i=0; i<4; i++) {
        alSourcePlay(alSource);
    //    usleep(500000);
    //}
    //Cleanup.
    //First delete the source.
    //alDeleteSources(1, &alSource);
    //Delete the buffer.
    //alDeleteBuffers(1, &alBuffer);
    //Close out OpenAL itself.
    //Get active context.
    //ALCcontext *Context = alcGetCurrentContext();
    //Get device for active context.
    //ALCdevice *Device = alcGetContextsDevice(Context);
    //Disable context.
    //alcMakeContextCurrent(NULL);
    //Release context(s).
    //alcDestroyContext(Context);
    //Close device.
    //alcCloseDevice(Device);
    return;
//#endif
}

//void walking_sound()
    //Intiate walking loop sounds
//{
//    alutInit(0, NULL);
//    if (alGetError() != AL_NO_ERROR) {
//      printf("ERROR: alutInit()\n");
//    }
    //Clear error state.
//    alGetError();

    //Setup the listener.
    //Forward and up vectors are used.
//    float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
//    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
//    alListenerfv(AL_ORIENTATION, vec);
//    alListenerf(AL_GAIN, 1.0f);

    //Buffer holds the sound information.
//    ALuint alBuffer;
//    alBuffer = alutCreateBufferFromFile("test.wav");

    //Source refers to the sound.
//    ALuint alSource;
    //Generate a source, and store it in a buffer.
//    alGenSources(1, &alSource);
//    alSourcei(alSource, AL_BUFFER, alBuffer);
    //Set volume and pitch to normal, no looping of sound.
//    alSourcef(alSource, AL_GAIN, 1.0f);
//    alSourcef(alSource, AL_PITCH, 1.0f);
//    alSourcei(alSource, AL_LOOPING, AL_FALSE);
//    if (alGetError() != AL_NO_ERROR) {
//      printf("ERROR: setting source\n");
//    }
//    for (int i=0; i<42; i++) {
//      alSourcePlay(alSource);
//      usleep(500000);
//    }

//}

    

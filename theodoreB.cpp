//Credits Function & My Sourcefile for Game Project: Perdition
//By: Theodore Barcelona
//Group #: 2
//Updated: 12/3/18
//Theo's game source code
//Description: show name/picture in credits, plays sounds/music
#include <math.h>
#include <GL/glx.h>
#include "fonts.h"
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
//Sound Implementation Openal
ALuint alBuffer[5];
ALuint alSource[5];
void init_sounds()
{
//#ifdef USE_OPENAL_SOUND
    alutInit(0, NULL);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: alutInit()\n");
    }
    //Clear error state
    alGetError();

    //Listeners
    float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, vec);
    alListenerf(AL_GAIN, 1.0f);

    //Buffers
    alBuffer[0] = alutCreateBufferFromFile("./audio/test.wav");
    alBuffer[1] = alutCreateBufferFromFile("./audio/03_Deserted_Mansion.wav");
    alBuffer[2] = alutCreateBufferFromFile("./audio/Move1.wav");
    alBuffer[3] = alutCreateBufferFromFile("./audio/Monster6.wav");
    alBuffer[4] = alutCreateBufferFromFile("./audio.Jump1.wav");
    //Generate sound sources
    alGenSources(5, alSource);
    //Set volume/pitch; creates buffers
    //sound test
    alSourcei(alSource[0], AL_BUFFER, alBuffer[0]);
    alSourcef(alSource[0], AL_GAIN, 1.0f);
    alSourcef(alSource[0], AL_PITCH, 1.0f);
    alSourcef(alSource[0], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: setting source\n");
    }
    //music
    alSourcei(alSource[1], AL_BUFFER, alBuffer[1]);
    alSourcef(alSource[1], AL_GAIN, 1.0f);
    alSourcef(alSource[1], AL_PITCH, 1.0f);
    alSourcef(alSource[1], AL_LOOPING, AL_TRUE);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: setting source\n");
    }
    //walking sound
    alSourcei(alSource[2], AL_BUFFER, alBuffer[2]);
    alSourcef(alSource[2], AL_GAIN, 1.0f);
    alSourcef(alSource[2], AL_PITCH, 1.0f);
    alSourcef(alSource[2], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: setting source\n");
    }
    //monster sound
    alSourcei(alSource[3], AL_BUFFER, alBuffer[3]);
    alSourcef(alSource[3], AL_GAIN, 1.0f);
    alSourcef(alSource[3], AL_PITCH, 1.0f);
    alSourcef(alSource[3], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: setting source\n");
    }
    //jump sound
    alSourcei(alSource[4], AL_BUFFER, alBuffer[4]);
    alSourcef(alSource[4], AL_GAIN, 1.0f);
    alSourcef(alSource[4], AL_PITCH, 1.0f);
    alSourcef(alSource[4], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: setting source\n");
    }
//#endif
}

void sound_test()
{
    //Sound Test only. Plays when a button is pressed.
    alSourcePlay(alSource[0]);
    printf("Sound is playing.\n");
}

void music()
{
    //Plays a music theme.
    alSourcePlay(alSource[1]);
    printf("Music playing.\n");
}

void walking_sound()
{
    //Plays walking loops when a button is pressed.
    alSourcePlay(alSource[2]);
    printf("walking sound.\n");
}

void enemy_sound()
{
    //Plays when player collides with an enemy
    alSourcePlay(alSource[3]);
    printf("enemy sound.\n");
}

void jump_sound()
{
    //Plays when a button is pressed
    alSourcePlay(alSource[4]);
    printf("jumping sound.\n");
}

extern void close_sounds()
{
    //Deleting Sources
    alDeleteSources(1, &alSource[0]);
    alDeleteSources(1, &alSource[1]);
    alDeleteSources(1, &alSource[2]);
    alDeleteSources(1, &alSource[3]);
    alDeleteSources(1, &alSource[4]);
    //Deleting Buffers
    alDeleteBuffers(1, &alBuffer[0]);
    alDeleteBuffers(1, &alBuffer[1]);
    alDeleteBuffers(1, &alBuffer[2]);
    alDeleteBuffers(1, &alBuffer[3]);
    alDeleteBuffers(1, &alBuffer[4]);
    //active context
    ALCcontext *Context = alcGetCurrentContext();
    //Get device for active context
    ALCdevice *Device = alcGetContextsDevice(Context);
    //Disable context.
    alcMakeContextCurrent(NULL);
    //Release context(s).
    alcDestroyContext(Context);
    //Close device.
    alcCloseDevice(Device);
}

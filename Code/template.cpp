// Template, major revision 3, beta
// IGAD/NHTV - Jacco Bikker - 2006-2009

// Note:
// This version of the template attempts to setup a rendering surface in system RAM
// and copies it to VRAM using DMA. On recent systems, this yields extreme performance,
// and flips are almost instant. For older systems, there is a fall-back path that
// uses a more conventional approach offered by SDL. If your system uses this, the
// window caption will indicate this. In this case, you may want to tweak the video
// mode setup code for optimal performance.

#pragma warning (disable : 4530) // complaint about exception handler
#pragma warning (disable : 4273)
#pragma warning (disable : 4311) // pointer truncation from HANDLE to long

#include "Core.h"
extern "C" 
{ 
#include "glew.h" 
}
#include "gl.h"
#include "SDL.h"
#include "SDL_syswm.h"
#include "wglext.h"
#include "surface.h"
#include "template.h"
#include "fcntl.h"
#include "io.h"
#include <ios>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>

namespace Tmpl8 { 
  void NotifyUser( char* s )
  {
	  HWND hApp = FindWindow( NULL, "Template" );
	  MessageBox( hApp, s, "ERROR", MB_OK );
	  exit( 0 );
  }
}

using namespace Tmpl8;

static bool FULLSCREEN = false, firstframe = true;

//Game* game = 0;

bool init()
{
  //Setup for OpenGL viewport, orthographic mode. 
	glViewport( 0, 0, SCRWIDTH, SCRHEIGHT );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
 	//glOrtho( 0, SCRWIDTH, 0, SCRHEIGHT, -10000, 10000);
	gluPerspective(90, (float)SCRWIDTH/SCRHEIGHT, 0.1, 120);
  
  glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
    
	glEnable( GL_TEXTURE_2D );
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
	glShadeModel( GL_SMOOTH );

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
  
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	
  if(glGetError()) 
    return false;
	return true;
}

int main( int argc, char **argv ) 
{
	SDL_Init( SDL_INIT_VIDEO );
	SDL_SetVideoMode( SCRWIDTH, SCRHEIGHT, 0, SDL_OPENGL);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0);
  glDepthMask(true);
  SDL_EnableKeyRepeat( 0, 0 );
	bool vbo = true;
	if (!init()) 
	{
    SDL_SetVideoMode( SCRWIDTH, SCRHEIGHT, 0, SDL_DOUBLEBUF | SDL_ASYNCBLIT );
		SDL_Surface* s = SDL_GetVideoSurface();
		vbo = false;
		SDL_WM_SetCaption( "Micro Machines - FALLBACK", NULL );
	}
	else SDL_WM_SetCaption( "Micro Machines", NULL );
	int exitapp = 0;

	//game = new Game();
	clock_t startTime, endTime;
	clock_t clockTime;
	startTime = endTime = clockTime = 0;
	float fFrameTimer = 0;
  
	while (!exitapp) 
	{
    endTime = clockTime;
		clockTime = clock();
		float fDeltaTime = (float)(clockTime - endTime) / CLOCKS_PER_SEC;

		if (vbo) // frame buffer swapping for VBO mode
		{
   	  SDL_GL_SwapBuffers(); 
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glClearDepth(1.0);
      glMatrixMode( GL_MODELVIEW );
	    glLoadIdentity();
		}
		else	 // frame buffer swapping for fall-back path
		{
			SDL_Surface* s = SDL_GetVideoSurface();
			SDL_UpdateRect( s, 0, 0, 0, 0 );
		}
		if (firstframe)
		{
			//game->Init();
      GameLayerCore->Init(nullptr);
			firstframe = false;
		}

    //TODO: add decent deltatime
    if(GameLayerCore->Update( fDeltaTime ))
    {
      exitapp = 1;
    }

		// event loop
		SDL_Event event;
		while (SDL_PollEvent( &event )) 
		{
			switch (event.type)
			{
			case SDL_QUIT:
				exitapp = 1;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) 
				{
					exitapp = 1;
					// find other keys here: http://sdl.beuc.net/sdl.wiki/SDLKey
				}
      }
      GameLayerCore->PassEvent(event);
		}
	}
	SDL_Quit();
	return 1;
}
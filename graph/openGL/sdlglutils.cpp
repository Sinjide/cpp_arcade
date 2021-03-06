//
// sdlglutils.cpp for arcade in /home/nicolas/arcade/cpp_arcade/graph
// 
// Made by Nicolas Guillon
// Login   <nicolas@epitech.net>
// 
// Started on  Wed Mar 29 14:24:22 2017 Nicolas Guillon
// Last update Sun Apr  9 11:54:22 2017 Nicolas Guillon
//

#include "sdlglutils.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glu.h>

#include <cstring>
#include <cstdlib>

SDL_Surface *flipSurface(SDL_Surface * surface)
{
  my_memcpy	cpy;
  int		pitch;
  int		line;
  SDL_Surface	*fliped = SDL_CreateRGBSurface(SDL_SWSURFACE,
					      surface->w,surface->h,
					      surface->format->BitsPerPixel,
					      surface->format->Rmask,
					      surface->format->Gmask,
					      surface->format->Bmask,
					      surface->format->Amask);

  SDL_LockSurface(surface);
  SDL_LockSurface(fliped);
  pitch = surface->pitch;
  for (line = 0; line < surface->h; line ++)
    {
      cpy.my_memcpydo(&((unsigned char* )fliped->pixels)[line*pitch],
	     &((unsigned char* )surface->pixels)[(surface->h - 1  -
						  line)*pitch],
	     pitch);
    }
  SDL_UnlockSurface(fliped);
  SDL_UnlockSurface(surface);
  return (fliped);
}

GLuint loadTexture(const char * filename, bool useMipMap)
{
  GLuint glID;
  SDL_Surface * picture_surface = NULL;
  SDL_Surface *gl_surface = NULL;
  SDL_Surface * gl_fliped_surface = NULL;
  Uint32 rmask, gmask, bmask, amask;

  picture_surface = IMG_Load(filename);
  if (picture_surface == NULL)
    return 0;

  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
      rmask = 0xff000000;
      gmask = 0x00ff0000;
      bmask = 0x0000ff00;
      amask = 0x000000ff;
    }
  else
    {
      rmask = 0x000000ff;
      gmask = 0x0000ff00;
      bmask = 0x00ff0000;
      amask = 0xff000000;
    }

  SDL_PixelFormat format = *(picture_surface->format);
  format.BitsPerPixel = 32;
  format.BytesPerPixel = 4;
  format.Rmask = rmask;
  format.Gmask = gmask;
  format.Bmask = bmask;
  format.Amask = amask;

  gl_surface = SDL_ConvertSurface(picture_surface,&format,SDL_SWSURFACE);
  gl_fliped_surface = flipSurface(gl_surface);
  glGenTextures(1, &glID);
  glBindTexture(GL_TEXTURE_2D, glID);

  if (useMipMap)
    {

      gluBuild2DMipmaps(GL_TEXTURE_2D, 4, gl_fliped_surface->w,
			gl_fliped_surface->h, GL_RGBA,GL_UNSIGNED_BYTE,
			gl_fliped_surface->pixels);

      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
		      GL_LINEAR_MIPMAP_LINEAR);
    }
  else
    {
      glTexImage2D(GL_TEXTURE_2D, 0, 4, gl_fliped_surface->w,
		   gl_fliped_surface->h, 0, GL_RGBA,GL_UNSIGNED_BYTE,
		   gl_fliped_surface->pixels);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


  SDL_FreeSurface(gl_fliped_surface);
  SDL_FreeSurface(gl_surface);
  SDL_FreeSurface(picture_surface);
  return (glID);
}

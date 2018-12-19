#include "spriteSheet.h"

SDL_Surface* cropSurface(SDL_Surface* surf,
                        SDL_Rect const* view){
  const auto* fmt = surf->format;
  SDL_Surface* sub = SDL_CreateRGBSurface(0,view->w,view->h,
                     fmt->BitsPerPixel,fmt->Rmask,fmt->Gmask,
                     fmt->Bmask,fmt->Amask);

  SDL_BlitSurface(surf,view,sub,nullptr);

  return sub;
}

SDL_Surface* SpriteSheet::get(unsigned i, unsigned j){
  if(i >= Nc || j >= Nr) return nullptr;

  view.x = i*view.w;
  view.y = j*view.h;
  return cropSurface(src,&view);
}

SDL_Surface* SpriteSheet::get(unsigned c){
  return SpriteSheet::get(c%Nc,c/Nc);
}

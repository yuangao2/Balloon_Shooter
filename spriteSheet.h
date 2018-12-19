#ifndef __UNPACK_H__
#define __UNPACK_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

//Use:
//
// Create spritesheet that owns surface:
// SpriteSheet sheet(surface, imageWidth, imageHeight);
//
// Create spritesheet that DOES NOT own surface:
// SpriteSheet sheet(surface, imageWidth, imageHeight, NonOwning);
//
// Get image 7 (altogether and assuming Z-order):
// auto image7 = sheet[7];
//
// Get the image in row 5, column 3:
// auto image53 = sheet(5,3);

class SpriteSheet{
public:
  static const struct NonOwningT{} NonOwning;

  SpriteSheet()=delete;
  SpriteSheet(const SpriteSheet&)=delete;
  SpriteSheet& operator=(const SpriteSheet&)=delete;
  SpriteSheet(SpriteSheet&&)=default;
  SpriteSheet& operator=(SpriteSheet&&)=default;

  SpriteSheet(SDL_Surface* _src, int w, int h, const NonOwningT)
             :src(_src),view{0,0,w,h}
             ,Nr(_src->h/h),Nc(_src->w/w),N(Nr*Nc)
             ,owning(false)
             {
             }
  SpriteSheet(SDL_Surface* _src, int w, int h)
             :SpriteSheet(_src,w,h,NonOwning)
             {
               owning = true;
             }
  ~SpriteSheet(){
    if(owning) SDL_FreeSurface(src);
  }

  unsigned getRows()const{ return Nr; }
  unsigned getColumns()const{ return Nc; }
  unsigned getFrames()const{ return N; }

  SDL_Surface* get(unsigned, unsigned);

  SDL_Surface* get(unsigned);

  inline SDL_Surface* operator()(unsigned i, unsigned j){
    return get(i,j);
  }
  inline SDL_Surface* operator[](unsigned c){
    return get(c);
  }

private:
  SDL_Surface* src;
  SDL_Rect     view;
  unsigned Nr,Nc,N;
  bool owning;
};

SDL_Surface* cropSurface(SDL_Surface*, const SDL_Rect*);

#endif

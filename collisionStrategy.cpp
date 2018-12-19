#include <vector>
#include <algorithm>
#include "ioMod.h"
#include "collisionStrategy.h"
#include "viewport.h"
#include "renderContext.h"

void RectangularCollisionStrategy::draw() const {
  IOmod::getInstance().writeText("Strategy: Rectangular", 800, 30);
}

bool RectangularCollisionStrategy::execute(
      const Drawable& sprite1, const Drawable& sprite2) const {
  float left1 = sprite1.getX();
  float left2 = sprite2.getX();

  float right1 = left1+sprite1.getScaledWidth();
  float right2 = left2+sprite2.getScaledWidth();
  if ( right1 < left2 ) return false;
  if ( left1 > right2 ) return false;
  float top1 = sprite1.getY();
  float top2 = sprite2.getY();
  float bottom1 = top1+sprite1.getScaledHeight();
  float bottom2 = top2+sprite2.getScaledHeight();
  if ( bottom1 < top2 ) return false;
  if ( bottom2 < top1 ) return false;
  return true;
}

float MidPointCollisionStrategy::distance(float x1, float y1, float x2, float y2) const {
  float x = x1-x2, y = y1-y2;
  return hypot(x, y);
}

void MidPointCollisionStrategy::draw() const {
  IOmod::getInstance().writeText("Strategy: Distance", 800, 30);
}

bool MidPointCollisionStrategy::execute(
      const Drawable& sprite1, const Drawable& sprite2) const {

  int width1 = sprite1.getScaledWidth();
  int width2 = sprite2.getScaledWidth();
  int height1 = sprite1.getScaledHeight();
  int height2 = sprite2.getScaledHeight();

  int COLLISION_DISTANCE = width1/2 + width2/2;
  float x1 = sprite1.getX() + width1/2;
  float y1 = sprite1.getY() + height1/2;
  float x2 = sprite2.getX() + width2/2;
  float y2 = sprite2.getY() + height2/2;
  return distance(x1, y1, x2, y2) < COLLISION_DISTANCE;
}

void PerPixelCollisionStrategy::draw() const {
  IOmod::getInstance().writeText("Strategy: Per-Pixel ", 800, 30);
}

bool PerPixelCollisionStrategy::
isVisible(Uint32 pixel, SDL_Surface *surface) const {
	Uint32 temp;
	SDL_PixelFormat* fmt = surface->format;
	if(fmt->BitsPerPixel == 32){
		temp=pixel&fmt->Amask; /* Isolate alpha component */
		temp=temp>>fmt->Ashift;/* Shift it down to 8-bit */
		temp=temp<<fmt->Aloss; /* Expand to a full 8-bit number */
		if(temp == 0) return false;
	}
	return true;
}

// This is, essentially, SpriteSheet::cropSurface:
SDL_Surface* scaleSurface(const SDL_Surface* const surf, int width, int height) {

  const auto* fmt = surf->format;
  SDL_Surface* sub = SDL_CreateRGBSurface(0,width,height,
                     fmt->BitsPerPixel,fmt->Rmask,fmt->Gmask,
                     fmt->Bmask,fmt->Amask);

  // Get the old mode
  SDL_BlendMode oldBlendMode;
  SDL_GetSurfaceBlendMode(const_cast<SDL_Surface*>(surf), &oldBlendMode);

  // Set the new mode so copying the source won't change the source
  SDL_SetSurfaceBlendMode(const_cast<SDL_Surface*>(surf), SDL_BLENDMODE_NONE);

  SDL_Rect rect = {0,0,width,height};
  int flag = SDL_BlitScaled(const_cast<SDL_Surface*>(surf),nullptr,sub,&rect);
  if ( flag < 0 ) throw( std::string("SDL_BlitScaled failed!") );
  //std::cout << width  << ", " << surf->w << ", " << sub->w << std::endl;

  // Set the blend mode back to original
  SDL_SetSurfaceBlendMode(const_cast<SDL_Surface*>(surf), oldBlendMode);
  return sub;
}

bool PerPixelCollisionStrategy::execute(
      const Drawable& obj1, const Drawable& obj2) const {

  RectangularCollisionStrategy strategy;
  if ( not strategy.execute(obj1, obj2) ) return false;
  // If we got this far, we know that the sprite rectangles intersect!

  Vector2f p1 = obj1.getPosition() - Viewport::getInstance().getPosition();
  Vector2f p2 = obj2.getPosition() - Viewport::getInstance().getPosition();

  Uint16 width1 = obj1.getScaledWidth();
  Uint16 height1 = obj1.getScaledHeight();

  Uint16 width2 = obj2.getScaledWidth();
  Uint16 height2 = obj2.getScaledHeight();

  int o1Left = p1[0]; 
  int o1Right = o1Left+width1;

  int o2Left = p2[0]; 
  int o2Right = o2Left+width2;
  std::vector<int> sides;
  sides.reserve(4);
  sides.push_back( o1Left );
  sides.push_back( o1Right );
  sides.push_back( o2Left );
  sides.push_back( o2Right );
  std::sort( sides.begin(), sides.end() );

  int o1Up = p1[1];
  int o1Down = o1Up+height1;
  int o2Up = p2[1];
  int o2Down = o2Up+height2;
  std::vector<int> lids;
  lids.reserve(4);
  lids.push_back( o1Up );
  lids.push_back( o1Down );
  lids.push_back( o2Up );
  lids.push_back( o2Down );
  std::sort( lids.begin(), lids.end() );

  const SDL_Surface* temp1 = obj1.getSurface(); 
  SDL_Surface* surface1 = scaleSurface(temp1, width1, height1);
  const SDL_Surface* temp2 = obj2.getSurface(); 
  SDL_Surface* surface2 = scaleSurface(temp2, width2, height2);

  SDL_LockSurface(surface1);
  SDL_LockSurface(surface2);
  Uint32 *pixels1 = static_cast<Uint32 *>(surface1->pixels);
  Uint32 *pixels2 = static_cast<Uint32 *>(surface2->pixels);
  unsigned pixel1;
  unsigned pixel2;
  for (int x = sides[1]; x < sides[2]; ++x) {
    for (int y = lids[1]; y < lids[2]; ++y) {
      // check pixels in surface for obj1 and surface for obj2!
      int i = x - p1[0];
      int j = y - p1[1];
      pixel1 = pixels1[ (j * width1) + i ];
      i = x - p2[0];
      j = y - p2[1];
      pixel2 = pixels2[ (j * width2) + i ];

      if ( isVisible(pixel1,surface1) && isVisible(pixel2,surface2) ) {
        SDL_UnlockSurface(surface1);
        SDL_UnlockSurface(surface2);
        SDL_FreeSurface(surface1);
        SDL_FreeSurface(surface2);
        return true;
      }
    }
  }
  SDL_UnlockSurface(surface1);
  SDL_UnlockSurface(surface2);
  SDL_FreeSurface(surface1);
  SDL_FreeSurface(surface2);

  return false;
}


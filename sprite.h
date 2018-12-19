#ifndef SPRITE__H
#define SPRITE__H
#include <string>
#include "drawable.h"

class ExplodingSprite;

class Sprite : public Drawable {
public:
  Sprite(const std::string&);
  Sprite(const std::string&, const Vector2f& pos, const Vector2f& vel, 
         const Image*);
  Sprite(const Sprite&);
  virtual ~Sprite();
  Sprite& operator=(const Sprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return image; }
  virtual const SDL_Surface* getSurface() const { 
    return image->getSurface();
  }
  virtual int getScaledWidth()  const { return getScale()*image->getWidth();  } 
  virtual int getScaledHeight() const { return getScale()*image->getHeight(); } 

  virtual void explode();
  
private:
  const Image * image;
  ExplodingSprite* explosion;

protected:
  int worldWidth;
  int worldHeight;

  int getDistance(const Sprite*) const;
  Vector2f makeVelocity(int, int) const;
};
#endif

#ifndef doublyMultisprite__H
#define doublyMultisprite__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class doublyMultisprite : public Drawable {
public:
  doublyMultisprite(const std::string&);
  doublyMultisprite(const std::string&, const std::vector<std::string>&);
  doublyMultisprite(const doublyMultisprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void changeAction(int);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }

protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned currentFrameGroup;
  bool moveRight;
  unsigned numberOfFrames;
  unsigned numberOfRows;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  doublyMultisprite& operator=(const doublyMultisprite&);
};

#endif

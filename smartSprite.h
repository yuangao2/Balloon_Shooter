#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <string>
#include "multisprite.h"

class SmartSprite : public MultiSprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
  SmartSprite(const SmartSprite&);
  virtual ~SmartSprite() { } 

  virtual void update(Uint32 ticks);
  void setPlayerPos(const Vector2f& p) { playerPos = p; }
  void setCollision() { collided = true; }
  bool isCollided() const { return collided; }

private:
  enum MODE {NORMAL, EVADE};
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float safeDistance;
  bool collided;

  void goLeft();
  void goRight();
  void goUp();
  void goDown();
};
#endif

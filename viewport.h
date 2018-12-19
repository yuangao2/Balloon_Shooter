#ifndef VIEWPORT__H
#define VIEWPORT__H
#include "drawable.h"
#include "gamedata.h"

class Viewport {
public:
  static Viewport& getInstance();
  void draw() const;
  void update();

  Vector2f getPosition() const { return position; }
  float getX() const  { return position[0]; }
  void  setX(float x) { position[0] = x; }
  float getY() const  { return position[1]; }
  void  setY(float y) { position[1] = y; }

  void setObjectToTrack(const Drawable *obj);
  const Drawable* getObjectToTrack() const { return objectToTrack; } 

private:
  const Gamedata& gdata;
  Vector2f position;
  int worldWidth;
  int worldHeight;
  int viewWidth;
  int viewHeight;
  int objWidth;
  int objHeight;
  
  const Drawable *objectToTrack;

  Viewport();
  Viewport(const Viewport&);
  Viewport& operator=(const Viewport&);
};
#endif

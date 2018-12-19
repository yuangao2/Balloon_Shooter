#include <iostream>
#include <cmath>
#include "bullet.h"
#include "renderContext.h"
#include "explodingSprite.h"

Bullet::Bullet(const std::string& name, int i, double d, const Vector2f& pos, const Vector2f& vel) :
  Drawable(name, pos, vel),
  images(RenderContext::getInstance()->getImages(name)),
  currentFrame(i),
  explosion(NULL),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  angle(d),
  distance(0),
  maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")),
  tooFar(false) 
{ }

Bullet::Bullet(const Bullet& b) :
  Drawable(b.getName(), b.getPosition(), b.getVelocity()),
  images(b.images),
  currentFrame(b.currentFrame),
  explosion(b.explosion),
  worldWidth(b.worldWidth),
  worldHeight(b.worldHeight),
  angle(b.angle),
  distance(b.distance),
  maxDistance(b.maxDistance),
  tooFar(b.tooFar)
{ }

Bullet::~Bullet() {
  if (explosion) {
    delete explosion;
  }
}

void Bullet::draw() const {
  if (explosion) {
    explosion->draw();
  }
  else {
    images[currentFrame]->draw(getX(), getY(), getScale(), angle, SDL_FLIP_NONE);
  }
}

void Bullet::update(Uint32 ticks) {
  if (explosion) {
    explosion->update(ticks);
    if (explosion->chunkCount() == 0) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  if (getY() < 0 || getY() > worldHeight - getScaledHeight()) {
    tooFar = true;
  }
  if (getX() < 0 || getX() > worldWidth - getScaledWidth()) {
    tooFar = true;
  }

  Vector2f pos = getPosition();
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(pos + incr); 
  distance += ( std::hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) {
    tooFar = true;
  }
}

void Bullet::explode() {
  if (!explosion) {
    Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}
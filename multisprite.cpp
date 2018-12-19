#include "multisprite.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"
#include <cstdlib>
#include "sound.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(std::rand() % Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    std::rand() % Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(std::rand() % Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    std::rand() % Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
  explosion(NULL),
  currentFrame(std::rand() % 4),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  images(s.images),
  explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

MultiSprite& MultiSprite::operator=(const MultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  explosion = s.explosion;
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

MultiSprite::~MultiSprite() {
  if (explosion) {
    delete explosion;
  }
}

void MultiSprite::draw() const { 
  if (explosion) {
    explosion->draw();
  }
  else {
    images[currentFrame]->draw(getX(), getY(), getScale());
  }  
}

void MultiSprite::update(Uint32 ticks) { 
  if (explosion) {
    explosion->update(ticks);
    if (explosion->chunkCount() == 0) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }
  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  
}

void MultiSprite::explode() {
  if ( !explosion ) {
    Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
    SDLSound::getInstance()[1];
  }
}
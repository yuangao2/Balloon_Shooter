#include "doublyMultisprite.h"
#include "gamedata.h"
#include "renderContext.h"
#include <cstdlib>

doublyMultisprite::doublyMultisprite( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(std::rand() % Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    std::rand() % Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
  currentFrame(0),
  currentFrameGroup(0),
  moveRight(true),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  numberOfRows( Gamedata::getInstance().getXmlInt(name+"/rows") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
  { }

doublyMultisprite::doublyMultisprite( const std::string& name, const std::vector<std::string>& actions ) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getFolderImages(name, actions) ),
  currentFrame(0),
  currentFrameGroup(0),
  moveRight(true),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  numberOfRows(2),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
  { }

doublyMultisprite::doublyMultisprite(const doublyMultisprite& s) :
  Drawable(s),
  images(s.images),
  currentFrame(s.currentFrame),
  currentFrameGroup(s.currentFrameGroup),
  moveRight(s.moveRight),
  numberOfFrames( s.numberOfFrames ),
  numberOfRows( s.numberOfRows ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

doublyMultisprite& doublyMultisprite::operator=(const doublyMultisprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  currentFrameGroup = s.currentFrameGroup;
  moveRight = (s.moveRight);
  numberOfFrames = ( s.numberOfFrames );
  numberOfRows = ( s.numberOfRows );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

void doublyMultisprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = currentFrameGroup * numberOfFrames + (currentFrame + 1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

void doublyMultisprite::draw() const {
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void doublyMultisprite::update(Uint32 ticks) {
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
    if (!moveRight) {
      currentFrameGroup--;
      // currentFrameGroup = (currentFrameGroup + 1 ) % numberOfRows;
    }
    moveRight = true;
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
    if (moveRight) {
      currentFrameGroup++;
      // currentFrameGroup = (currentFrameGroup + 1 ) % numberOfRows;
    }
    moveRight = false;   
  }
}

void doublyMultisprite::changeAction(int num) {
  currentFrameGroup = (currentFrameGroup + 2) % (2 * num);
}

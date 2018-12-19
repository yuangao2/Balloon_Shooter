#include "player.h"
#include "smartSprite.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"
#include "sound.h"

Player::Player(const std::string& name, const std::vector<std::string>& actions) :
  doublyMultisprite(name, actions),
  observers(),
  collision(false),
  explosion(NULL),
  initialVelocity(getVelocity()),
  initialPosition(getPosition()),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bullet")),
  bulletlist(),
  freelist(),
  shootCount(0),
  shootUpwards(false),
  minSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/minSpeedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastShoot(0),
  jumping(false),
  timeSinceJump(0)
{}

Player::Player(const Player& s) :
  doublyMultisprite(s),
  observers(s.observers),
  collision(s.collision),
  explosion(s.explosion),
  initialVelocity(s.initialVelocity),
  initialPosition(s.initialPosition),
  bulletName(s.bulletName),
  bulletlist(s.bulletlist),
  freelist(s.freelist),
  shootCount(s.shootCount),
  shootUpwards(s.shootUpwards),
  minSpeed(s.minSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastShoot(s.timeSinceLastShoot),
  jumping(s.jumping),
  timeSinceJump(s.timeSinceJump)
{ }

Player::~Player() {
  if (explosion) {
    delete explosion;
  }
}

void Player::explode() {
  if ( !explosion ) {
    Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
    SDLSound::getInstance()[2];
    collision = true;
  }
}

void Player::stop() {
  setVelocity(Vector2f(0,0));
  shootUpwards = false;
  if (timeSinceLastShoot > 550 || currentFrame == 89 || currentFrame == 99) {
    if (moveRight) {
      currentFrameGroup = STANDRIGHT;
    }
    else {
      currentFrameGroup = STANDLEFT;
    }
  } 
}

void Player::right() {
  if ( getX() < worldWidth-getScaledWidth()) {
    if (timeSinceLastShoot > 550 || currentFrame == 89 || currentFrame == 99) {
      currentFrameGroup = WALKRIGHT;
    }    
    moveRight = true;
    setVelocityX(initialVelocity[0]);
  }
}
void Player::left()  {
  if ( getX() > 0) {
    if (timeSinceLastShoot > 550 || currentFrame == 89 || currentFrame == 99) {
      currentFrameGroup = WALKLEFT;
    } 
    moveRight = false;
    setVelocityX(-initialVelocity[0]);
  }
}
void Player::up()    {
  shootUpwards = true;
}

void Player::jumpshoot() {
  if (!jumping) {
    if ( getY() > 0) {
      setVelocityY( -initialVelocity[1] );
    }
    jumping = true;
    timeSinceJump = 0;
  }
}

void Player::shoot() {
  if ( timeSinceLastShoot < bulletInterval ) return;
  float minSpeedY = 0;
  double angle = 0;
  Vector2f pos, vel;
  if (shootUpwards) {
    minSpeedY = -minSpeed;
  }
  if (moveRight) {
    currentFrameGroup = SHOOTRIGHT;
    float deltaX = Gamedata::getInstance().getXmlInt(bulletName+"/startRLoc/x"); //getScaledWidth()/2 + 40;
    float deltaY = Gamedata::getInstance().getXmlInt(bulletName+"/startRLoc/y"); //getScaledHeight()/2 + 8;
    // I need to add some minSpeed to velocity:
    
    if (shootUpwards) {
      angle = -45.0;
      deltaY -= 20.0;
    }
    pos = getPosition() + Vector2f(deltaX, deltaY);
    vel = getVelocity() + Vector2f(minSpeed, minSpeedY);
    currentFrame = 83;
  }
  else {
    currentFrameGroup = SHOOTLEFT;
    angle = 180.0;
    float deltaX = Gamedata::getInstance().getXmlInt(bulletName+"/startLLoc/x"); //getScaledWidth()/2 + 40;
    float deltaY = Gamedata::getInstance().getXmlInt(bulletName+"/startLLoc/y"); //getScaledHeight()/2 + 8;
    // I need to add some minSpeed to velocity:
    if (shootUpwards) {
      angle = 225.0;
      deltaY -= 20.0;
    }
    pos = getPosition() + Vector2f(deltaX, deltaY);
    vel = getVelocity() - Vector2f(minSpeed, -minSpeedY);
    currentFrame = 93;
  }

  if (freelist.empty()) {
    Bullet bullet(bulletName, 1, angle, pos, vel);
    bulletlist.push_back(bullet);
  }
  else {
    Bullet bullet = freelist.front();
    freelist.pop_front();
    bullet.reset();
    bullet.setVelocity(vel);
    bullet.setPosition(pos);
    bullet.setAngle(angle);
    bulletlist.push_back(bullet);
  }
  timeSinceLastShoot = 0;
  shootCount++;
  SDLSound::getInstance()[0];
}

void Player::draw() const {
  if (explosion) {
    explosion->draw();
  }
  else {
    doublyMultisprite::draw();
  }
  for ( const Bullet& bullet : bulletlist ) {
    bullet.draw();
  }
}

void Player::update(Uint32 ticks) {
  if (explosion) {
    explosion->update(ticks);
    if (explosion->chunkCount() == 0) {
      delete explosion;
      explosion = NULL;
      collision = false;
    }
  }
  else {
    advanceFrame(ticks);

    if (!jumping) {
      Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
      setPosition(getPosition() + incr);
      stop();
    }
    else {
      if (getY() < initialPosition[1] + 10) {
        if (std::abs(getVelocityY()) < 10 ) {
          shoot();
        }
        setVelocityX(0);
        timeSinceJump += ticks;
        Vector2f incr = getVelocity() * static_cast<float>(timeSinceJump) * 0.001; 
        setPosition(getPosition() + incr);
        setVelocity(getVelocity() + Vector2f(0, 30 * static_cast<float>(timeSinceJump) * 0.001));
        if (getY() > initialPosition[1] + 10) {
          setY(initialPosition[1]);
          stop();
          jumping = false;
        }
      }
      else {
        jumping = false;
        setY(initialPosition[1]);
        stop();
      }
    }
  }

  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }

  timeSinceLastShoot += ticks;
  std::list<Bullet>::iterator it = bulletlist.begin();
  while (it != bulletlist.end()) {
    it->update(ticks);
    if (it->goneTooFar()) {
      freelist.push_back(*it);
      it = bulletlist.erase(it);
    }
    else {
      it++;
    }
  }
}

void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

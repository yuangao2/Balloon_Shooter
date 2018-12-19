#ifndef PLAYER__H
#define PLAYER__H

#include <list>
#include "doublyMultisprite.h"
#include "bullet.h"

class SmartSprite;
class ExplodingSprite;

class Player : public doublyMultisprite { 
public:
  Player(const std::string&, const std::vector<std::string>&);
  Player(const Player&);
  ~Player();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  void attach(SmartSprite* o) { observers.push_back(o); } 
  void detach(SmartSprite*);

  bool collided() { return collision; }
  // void missed() { collision = false; }
  virtual void explode();
  void right();
  void left();
  void up();
  void jumpshoot();
  void stop();
  void shoot();
  int getBulletlistSize() const { return bulletlist.size(); } 
  int getFreelistSize() const { return freelist.size(); }
  int getShootCount() const { return shootCount; }
  std::list<Bullet>::iterator getBulletlistFront() { return bulletlist.begin(); }
  std::list<Bullet>::iterator getBulletlistEnd() { return bulletlist.end(); }
protected:
  std::list<SmartSprite*> observers;   
private:
  enum PlayerActions { 
    STANDRIGHT, STANDLEFT, 
    WALKRIGHT, WALKLEFT, 
    RUNRIGHT, RUNLEFT,
    JUMPRIGHT, JUMPLEFT,
    SHOOTRIGHT, SHOOTLEFT
  };
  bool collision;
  ExplodingSprite* explosion;
  Vector2f initialVelocity;
  Vector2f initialPosition;
  std::string bulletName;
  std::list<Bullet> bulletlist;
  std::list<Bullet> freelist;
  int shootCount;
  bool shootUpwards;
  float minSpeed;
  float bulletInterval;
  float timeSinceLastShoot;
  bool jumping;
  float timeSinceJump;
  Player& operator=(const Player&);
};
#endif

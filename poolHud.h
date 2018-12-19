#ifndef POOLHUD__H
#define POOLHUD__H

#include "hud.h"
#include "player.h"

class PoolHud : public Hud {
public:
  PoolHud(const Player&);
  virtual void draw() const;
  
  PoolHud(const PoolHud&) = delete;
  PoolHud& operator=(const PoolHud&) = delete;   
private:
  const Player& player;
  SDL_Rect poolHudFrame;
};
#endif
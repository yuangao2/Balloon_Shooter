#ifndef ENGINE__H
#define ENGINE__H

#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "sound.h"
#include "viewport.h"
#include "introHud.h"
#include "poolHud.h"
#include "infoHud.h"
#include "menuEngine.h"

class Player;
class CollisionStrategy;
class SmartSprite;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchAction();
  void drawStats() const;
  void drawCollision() const;
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
private:
  int countBlue = 0, countGreen = 0, countRed = 0, countYellow = 0;
  enum BalloonColor { BLUE, GREEN, RED, YELLOW };
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;
  SDL_Renderer * const renderer;
  SDLSound& sound;
  MenuEngine menuEngine;
  World clouds;
  World castle;
  World tower;
  Viewport& viewport;
  std::vector<std::string> actions;
  Player* player;
  std::vector<Drawable*> farSprites;
  std::vector<Drawable*> closeSprites;
  std::vector<Drawable*> sprites;
  std::vector<SmartSprite*> smartSprites;
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  int currentSprite;
  IntroHud* introHud;
  InfoHud* infoHud;
  PoolHud* poolHud;
  int hudDelay;
  int endDelay;
  bool godMode;
  bool makeVideo;
  int life;
  bool gameOver;
  void draw() const;
  void update(Uint32);
  void printScales() const;
  void checkForCollisions();
  void checkArrowCollisions();
};

#endif

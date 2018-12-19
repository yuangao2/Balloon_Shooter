#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include "sprite.h"
#include "multisprite.h"
#include "doublyMultisprite.h"
#include "player.h"
#include "smartSprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"

class SpriteLess {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};

Engine::~Engine() { 
  delete player;
  for (Drawable* sprite : sprites) {
    delete sprite;
  }
  sprites.clear();
  for (Drawable* sprite : farSprites) {
    delete sprite;
  }
  farSprites.clear();
  for (Drawable* sprite : closeSprites) {
    delete sprite;
  }
  closeSprites.clear();
  for (SmartSprite* smartsprite : smartSprites) {
    delete smartsprite;
  }
  smartSprites.clear();
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  }
  strategies.clear();
  delete poolHud;
  delete introHud;
  delete infoHud;

  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  sound(SDLSound::getInstance()),
  menuEngine(),
  clouds("clouds", Gamedata::getInstance().getXmlInt("clouds/factor") ),
  castle("castle", Gamedata::getInstance().getXmlInt("castle/factor") ),
  tower("tower", Gamedata::getInstance().getXmlInt("tower/factor") ),
  viewport( Viewport::getInstance() ),
  actions(),
  player(),
  farSprites(),
  closeSprites(),
  sprites(),
  smartSprites(),
  strategies(),
  currentStrategy(0),
  currentSprite(0),
  introHud(new IntroHud()),
  infoHud(new InfoHud()),
  poolHud(),
  hudDelay(0),
  endDelay(0),
  godMode(false),
  makeVideo( false ),
  life(5),
  gameOver(false)
{ 
  std::srand(std::time(0)); 
  actions.push_back("Stand");
  actions.push_back("Walk");
  actions.push_back("Run");
  actions.push_back("Jump");
  actions.push_back("Shoot");
  player = new Player("Archer", actions);
  Viewport::getInstance().setObjectToTrack(player);
  poolHud = new PoolHud(*player);
  introHud->toggle();
  farSprites.reserve(10);
  for (int i =0; i < 10; i++) {
    farSprites.push_back(new MultiSprite("Balloon"));
    float scale = Gamedata::getInstance().getRandFloat(0.2, 0.3);
    farSprites[i]->setScale(scale);
    farSprites[i]->setVelocity(Vector2f(10, 10));
  }
  std::sort(farSprites.begin(), farSprites.end(), SpriteLess());
  closeSprites.reserve(10);
  for (int i = 0; i < 10; i++) {
    closeSprites.push_back(new MultiSprite("Balloon"));
    float scale = Gamedata::getInstance().getRandFloat(0.4, 0.5);
    closeSprites[i]->setScale(scale);
    closeSprites[i]->setVelocity(Vector2f(25, 25));
  }
  std::sort(farSprites.begin(), farSprites.end(), SpriteLess());

  int n = Gamedata::getInstance().getXmlInt("numberOfBalloons");
  smartSprites.reserve(n);
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  for (int i = 0; i < n; i++) {
    smartSprites.push_back(new SmartSprite("Balloon", pos, w, h));
    player->attach( smartSprites[i] );
  }
  sprites.push_back(new Sprite("star")); 
  
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );
  strategies.push_back( new RectangularCollisionStrategy );
 
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  if(introHud->isShowHud()) {
    clouds.draw();
    castle.draw();
    tower.draw();
    introHud->draw();
  }
  else if (gameOver) {
    SDL_Rect rect = {400, 160, 400, 160};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255/2);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255/2);
    SDL_RenderDrawRect(renderer, &rect);
    IOmod::getInstance().setFontColor(255, 0, 0); 
    if (smartSprites.size() == 0) {
      IOmod::getInstance().writeText("You have got all balloons!", 450, 200);
    }
    else if (life == 0) {
      IOmod::getInstance().writeText("You have lost all lives!", 450, 200);
    }
    IOmod::getInstance().writeText("Press R to Restart the Game!", 450, 240);
    clock.pause();
  }
  else {
    clouds.draw();
    for (const Drawable* farSprite : farSprites) {
      farSprite->draw();
    }
    castle.draw();
    for (const Drawable* closeSprite : closeSprites) {
      closeSprite->draw();
    }
    tower.draw();
    for (const Drawable* sprite : sprites) {
      sprite->draw();
    }
    for (const Drawable* smartSprite : smartSprites) {
      smartSprite->draw();
    }
    if (poolHud->isShowHud()) {
      poolHud->draw();
    }
    
    drawStats();    
    player->draw();
    viewport.draw();
  }
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  if (!player->collided()) {
    for (Drawable* sprite : sprites) {
      if ( strategies[currentStrategy]->execute(*player, *sprite) ) {
        player->explode();        
        life--;
      }
    }
  }
}

void Engine::checkArrowCollisions() {
  if (player->getBulletlistSize() > 0) {
    auto arrow = player->getBulletlistFront();
    while (arrow != player->getBulletlistEnd()) {
      auto its = smartSprites.begin();
      while (its != smartSprites.end()) {
        SmartSprite* doa = *its;
        if ( strategies[currentStrategy]->execute(*arrow, *doa) && !doa->isCollided()) {         
          unsigned int balloonColor = doa->getCurrentFrame();
          player->detach(doa);
          doa->setCollision();
          doa->explode();    
          switch(balloonColor) {
            case BLUE:;
              countBlue++;
              break;
            case GREEN:
              countGreen++;
              break;
            case RED:
              countRed++;
              break;
            case YELLOW:
              countYellow++;
              break;
            default:
              break;
          }
        }
        its++;     
      } 
      arrow++;  
    }
  }
}

void Engine::update(Uint32 ticks) {
  checkArrowCollisions();
  if (!godMode) {
    checkForCollisions();
  }
  if (introHud->isShowHud()) {
    if (hudDelay < 50) {
      hudDelay++;
    } 
    else {
      introHud->toggle();
    }   
  }
  else {
    if (life == 0) {
      if (endDelay >= 20) {
        endDelay = 0;
        gameOver = true;
      }
      endDelay++;
    }
    
    player->update(ticks);
    for (Drawable* farSprite : farSprites) {
      farSprite->update(ticks);
    }
    for (Drawable* closeSprite : closeSprites) {
      closeSprite->update(ticks);
    }
    for(Drawable* sprite : sprites) {
      sprite->update(ticks);
    }

    auto its = smartSprites.begin();
    while (its != smartSprites.end()) {
      SmartSprite* doa = *its;
      doa->update(ticks);
      if (doa->isCollided() && !doa->getExplosion()) {
        its = smartSprites.erase(its);
      }
      else {
        its++;
      }
    }
    if (smartSprites.size() == 0) {
      if (endDelay >= 100) {
        endDelay = 0;
        gameOver = true;
        sound.playEndMusic();
      }
      endDelay++;
    }
    clouds.update();
    castle.update();
    tower.update();
    viewport.update(); // always update viewport last
  } 
}

void Engine::switchAction(){
  player->changeAction(actions.size());
}

void Engine::drawStats() const {
  std::stringstream ss;
  ss << countBlue << " Blue Balloons"; 
  IOmod::getInstance().setFontColor(0, 0, 255);
  IOmod::getInstance().writeText(ss.str(), 30, 30);
  ss.clear();
  ss.str("");
  ss << countGreen << " Green Balloons";
  IOmod::getInstance().setFontColor(0, 255, 0);
  IOmod::getInstance().writeText(ss.str(), 30, 60);
  ss.clear();
  ss.str("");
  ss << countRed << " Red Balloons";
  IOmod::getInstance().setFontColor(255, 0, 0);
  IOmod::getInstance().writeText(ss.str(), 30, 90);
  ss.clear();
  ss.str("");
  ss << countYellow << " Yellow Balloons";
  IOmod::getInstance().setFontColor(255, 255, 0);
  IOmod::getInstance().writeText(ss.str(), 30, 120);
  ss.clear();
  ss.str("");
  ss << smartSprites.size() << " Smart Sprites Remaining"; 
  IOmod::getInstance().setFontColor(0, 0, 0);  
  IOmod::getInstance().writeText(ss.str(), 30, 150);
  ss.clear();
  ss.str("");
  ss << player->getShootCount() << " Total Arrows Used"; 
  // IOmod::getInstance().setFontColor(255, 0, 0);  
  IOmod::getInstance().writeText(ss.str(), 30, 180);
  ss.clear();
  ss.str("");
  ss << life << " Lives Remaining"; 
  IOmod::getInstance().setFontColor(255, 0, 0);  
  IOmod::getInstance().writeText(ss.str(), 30, 210);
}

void Engine::drawCollision() const {
  IOmod::getInstance().setFontColor(255, 0, 255);
  strategies[currentStrategy]->draw();
  IOmod::getInstance().writeText("Press m to change strategy", 800, 60);  
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate = 0;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  clock.pause();
  done = menuEngine.play();
  clock.unpause();
  sound.playGameMusic();

  while ( !done ) {
    keystate = SDL_GetKeyboardState(NULL);

    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {      
      if (event.type == SDL_QUIT) { done = true; break; }
      if (event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          // switchSprite();
        }
        if (keystate[SDL_SCANCODE_J]) {
          player->jumpshoot();
          
        }
        if ( keystate[SDL_SCANCODE_G] ) {
          if (godMode) {
            godMode = false;
          }
          else {
            godMode = true;
          }
        }
        if ( keystate[SDL_SCANCODE_E] ) {
          player->explode();

          auto it = smartSprites.begin();
          while (it != smartSprites.end()) {
            (*it)->explode();
            it++;
          }
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          // currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if ( keystate[SDL_SCANCODE_F1] ) {
          introHud->toggle();
          hudDelay = 0;
        }
        if ( keystate[SDL_SCANCODE_F2] ) {
          poolHud->toggle();
        }
        
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
          player->left();
        }
        if (keystate[SDL_SCANCODE_D]) {
          player->right();
        }
        if (keystate[SDL_SCANCODE_W]) {
          player->up();
        }      
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          player->shoot();          
        }
      draw();
      update(ticks);     
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}

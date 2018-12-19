#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include "sprite.h"
#include "gamedata.h"
#include "menuEngine.h"
#include "renderContext.h"
#include "gamedata.h"

MenuEngine::~MenuEngine() { delete introHud; }

MenuEngine::MenuEngine() :
  clock( Clock::getInstance() ),
  renderer( RenderContext::getInstance()->getRenderer() ),
  background("castles", Gamedata::getInstance().getXmlInt("castles/factor")),
  menu( renderer ),
  sound(SDLSound::getInstance()),
  introHud(new IntroHud())
{
  sound.playStartMusic();
  introHud->toggle();
}

void MenuEngine::draw() const {
  background.draw();
  if(introHud->isShowHud()) {
    introHud->draw();
  }
  else {
    menu.draw();
  }  
  SDL_RenderPresent(renderer);
}

void MenuEngine::update(Uint32) {
}

bool MenuEngine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  

  while ( !done ) {

    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if (keystate[SDL_SCANCODE_DOWN] ) menu.incrIcon();
        // if (keystate[SDL_SCANCODE_UP] ) menu.decrIcon();
        if ( keystate[SDL_SCANCODE_F1] ) {
          introHud->toggle();
        }
        if (keystate[SDL_SCANCODE_RETURN] ) {
          int option = menu.getOptionNo();
          std::cout << "option chosen: " << menu.getOptionNo() << std::endl;
          if ( option == 0 ) {
            done = true;
            return false;
          }
          if (option == 1) {
            introHud->toggle();
          }
          if (option == 2) {
            done = true;
            return true;
          }
        }
      }
    }
    // In this section of the event loop we allow key bounce:
    draw();
    
    //update(ticks);
  }
  return false;
}

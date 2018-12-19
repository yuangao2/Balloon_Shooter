#ifndef HUD__H
#define HUD__H

#include <string>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "gamedata.h"

class Hud {
public:
   Hud() : 
      rc(RenderContext::getInstance()),
      io(IOmod::getInstance()),
      renderer(rc->getRenderer()),
      gd(Gamedata::getInstance()),
      showHud(true)
   {}
   Hud(const Hud&) = delete;
   Hud& operator=(const Hud&) = delete;
   virtual ~Hud() {}
   virtual void draw() const = 0;
   virtual void update() {}
   void toggle() { showHud = !showHud; }
   bool isShowHud() { return showHud; }
protected:
   const RenderContext* rc;
   IOmod& io; 
   SDL_Renderer* const renderer;
   const Gamedata& gd;
   bool showHud;
};
#endif
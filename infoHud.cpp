#include "infoHud.h"
#include <sstream>

InfoHud::InfoHud() :
  Hud(),
  infoHudFrame({
    gd.getXmlInt("infoHud/loc/x"),
    gd.getXmlInt("infoHud/loc/y"),
    gd.getXmlInt("infoHud/width"),
    gd.getXmlInt("infoHud/height")
  })
{}

void InfoHud::draw() const {
  if (showHud) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255/2);

    SDL_RenderFillRect(renderer, &infoHudFrame);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255/2);
    SDL_RenderDrawRect(renderer, &infoHudFrame);
  } 
}
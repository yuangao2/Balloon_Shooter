#include "poolHud.h"
#include <sstream>

PoolHud::PoolHud(const Player& p) :
  Hud(),
  player(p),
  poolHudFrame({
    gd.getXmlInt("poolHud/loc/x"),
    gd.getXmlInt("poolHud/loc/y"),
    gd.getXmlInt("poolHud/width"),
    gd.getXmlInt("poolHud/height")
  })
{}

void PoolHud::draw() const {
  if (showHud) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255/2);
    SDL_RenderFillRect(renderer, &poolHudFrame);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255/2);
    SDL_RenderDrawRect(renderer, &poolHudFrame);

    std::stringstream ss;
    io.setFontColor(0, 0, 0); 
    ss << "Bullet list: "<< player.getBulletlistSize();
    io.writeText(ss.str(), 910, 30);
    
    ss.clear();
    ss.str("");
    ss << "Free list: " << player.getFreelistSize(); 
    io.writeText(ss.str(), 910, 60);

    ss.clear();
    ss.str("");
    ss << "F2 toggles the PoolHud"; 
    io.writeText(ss.str(), 910, 90);   
  }
}

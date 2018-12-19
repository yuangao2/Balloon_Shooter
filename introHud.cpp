#include "introHud.h"

IntroHud::IntroHud() :
  Hud(),
  introHudFrame({
    gd.getXmlInt("introHud/loc/x"),
    gd.getXmlInt("introHud/loc/y"),
    gd.getXmlInt("introHud/width"),
    gd.getXmlInt("introHud/height")
  })
{}

void IntroHud::draw() const {
  if (showHud) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255/2);
    SDL_RenderFillRect(renderer, &introHudFrame);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255/2);
    SDL_RenderDrawRect(renderer, &introHudFrame);

    io.setFontColor(255, 0, 0); 
    io.writeText(gd.getXmlStr("introHud/name"), 510, 100);

    io.setFontColor(0, 0, 0); 
    std::string line = "*************************************";
    io.writeText(line, 430, 165);  
    io.setFontColor(255, 0, 0); 
    io.writeText(gd.getXmlStr("introHud/left"), 450, 200);
    io.setFontColor(0, 0, 255);
    io.writeText(gd.getXmlStr("introHud/right"), 450, 230);
    io.setFontColor(255, 0, 0);
    io.writeText(gd.getXmlStr("introHud/shoot"), 450, 260);
    io.setFontColor(0, 0, 255);
    io.writeText(gd.getXmlStr("introHud/up"), 450, 290);
    io.setFontColor(255, 0, 0);
    io.writeText(gd.getXmlStr("introHud/jump"), 450, 320);
    io.setFontColor(0, 0, 255);
    io.writeText(gd.getXmlStr("introHud/god"), 450, 350);
    io.setFontColor(255, 0, 0);
    io.writeText(gd.getXmlStr("introHud/hud"), 450, 380);
    io.setFontColor(0, 0, 0);
    io.writeText(line, 430, 425);
  } 
}
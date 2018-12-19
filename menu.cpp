#include<sstream>
#include "menu.h"

Menu::Menu(SDL_Renderer* rend) :  
  renderer(rend), 
  gdata(Gamedata::getInstance()), 
  hudFrame( {gdata.getXmlInt("menu/loc/x"),
             gdata.getXmlInt("menu/loc/y"),
             gdata.getXmlInt("menu/width"),
             gdata.getXmlInt("menu/height")
  }),
  clock( Clock::getInstance() ),
  io( IOmod::getInstance() ),
  options(),
  optionLoc( { gdata.getXmlInt("menu/optionLoc/x"),
               gdata.getXmlInt("menu/optionLoc/y")}
           ),
  clicks( { Sprite("option")} ),
  currentClick(0),
  currentOption(0),
  spaces(gdata.getXmlInt("menu/spaces")),
  startClickX(optionLoc[0]-100),
  startClickY(optionLoc[1]+60),
  clickX(startClickX),
  clickY(startClickY)
{
  int noOfOptions = gdata.getXmlInt("menu/noOfOptions");
  std::stringstream strm;
  for (int i = 0; i < noOfOptions; ++i) {
    strm.clear();
    strm.str("");
    strm << i;
    std::string option("menu/option"+strm.str());
    options.push_back(gdata.getXmlStr(option));
  }
}

void Menu::incrIcon() { 
  clickY += spaces; 
  if ( clickY > static_cast<int>(options.size())*spaces+optionLoc[1]) {
    clickY = startClickY;
    currentOption = 0;
  }
  else ++currentOption;
}

void Menu::decrIcon() { 
  clickY -= spaces; 
  if ( clickY < spaces+optionLoc[1]) {
    clickY = startClickY+2*spaces;
    currentOption = options.size()-1;
  }
  else --currentOption;
}

void Menu::draw() const {
  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


  // Set the  color for the Menu outline:
  SDL_SetRenderDrawColor( renderer, 200, 200, 200, 255/2 );
  SDL_RenderFillRect( renderer, &hudFrame );

  io.setFontColor(255, 0, 0); 
  io.writeText(gdata.getXmlStr("menu/name"), 480, 140);
  io.setFontColor(0, 0, 0); 
  std::string line = "********************************";
  io.writeText(line, 430, 200);  
  int space = spaces;
  io.setFontColor(0, 0, 255);
  for ( const std::string& option : options ) {
    io.writeText(option, optionLoc[0], optionLoc[1]+space);
    space += spaces;
  }
  io.setFontColor(0, 0, 0);
  io.writeText(line, 430, 450);
  // We have to draw the clickOn & clickOff relative to the screen,
  // and we don't want to offset by the location of the viewprot:
  clicks[currentClick].getImage()->draw(0, 0, clickX, clickY);
}


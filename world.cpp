#include <iostream>
#include "world.h"
#include "renderContext.h"

World::World(const std::string& name, int fact) : 
  image( RenderContext::getInstance()->getImage(name) ),
  factor(fact),
  worldWidth( Gamedata::getInstance().getXmlInt("world/width") ),
  imageWidth( image->getWidth() ),
  viewX(0.0), viewY(0.0), 
  view(Viewport::getInstance()) 
{ }

void World::update() {
  viewX = static_cast<int>(view.getX() / factor) % imageWidth;
  viewY = view.getY();
}

void World::draw() const { 
  image->draw(0,0,-viewX,-viewY);
  image->draw(0,0,imageWidth-viewX,-viewY);
}


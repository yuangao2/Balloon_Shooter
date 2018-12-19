#include "ioMod.h"
#include "vector2f.h"
#include "renderContext.h"

ImageFactory& ImageFactory::getInstance() {
  static ImageFactory instance;
  return instance;
}

ImageFactory::~ImageFactory() {
  std::cout << "Deleting images in Factory" << std::endl;
  // Free single image containers
  std::map<std::string, SDL_Surface*>::iterator ptr = surfaces.begin();
  while ( ptr != surfaces.end() ) {
    SDL_FreeSurface(ptr->second);
    ++ptr;
  }
  std::map<std::string, SDL_Texture*>::iterator pti = textures.begin();
  while ( pti != textures.end()) {
    SDL_DestroyTexture(pti->second);
    pti++;
  }
  std::map<std::string, Image*>::iterator pfi = images.begin();
  while ( pfi != images.end()) {
    std::cout << "deleting " << pfi->first << std::endl;
    delete pfi->second;
    pfi++;
  }

  // Free multi-image containers
  std::map<std::string, std::vector<SDL_Surface*>>::iterator psurfaces = multiSurfaces.begin();
  while (psurfaces != multiSurfaces.end()) {
    for (unsigned int i = 0; i < psurfaces->second.size(); ++i) {
      SDL_FreeSurface( psurfaces->second[i] );
    }
    psurfaces++;
  }
  for ( auto textures : multiTextures ) {
    for (unsigned int i = 0; i < textures.second.size(); ++i) {
      //SDL_DestroyTexture( textures.second[i] );
    }
  }
  for ( auto images : multiImages ) {
    std::cout << "deleting " << images.first << std::endl;
    for (unsigned int i = 0; i < images.second.size(); ++i) {
      delete images.second[i];
    }
  }
}

Image* ImageFactory::getImage(const std::string& name) {
    std::map<std::string, Image*>::const_iterator it = images.find(name); 
  if ( it == images.end() ) {
    SDL_Surface * const surface =
      IOmod::getInstance().readSurface( gdata.getXmlStr(name+"/file"));
    bool transparency = gdata.getXmlBool(name+"/transparency");
    if ( transparency ) {
      int keyColor = SDL_MapRGBA(surface->format, 255, 0, 255, 255);
      SDL_SetColorKey(surface, SDL_TRUE, keyColor);
    }
    surfaces[name] = surface;
    RenderContext* renderContext  = RenderContext::getInstance();
    SDL_Texture * const texture = 
      SDL_CreateTextureFromSurface(renderContext->getRenderer(), surface);
    Image * const image =new Image(surface, texture);
    images[name] = image;
    return image;
  }
  else {
    return it->second;
  }
}

std::vector<Image*> ImageFactory::getImages(const std::string& name) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Image*> >::const_iterator 
    pos = multiImages.find(name); 
  if ( pos != multiImages.end() ) {
    return pos->second;
  }

  IOmod& iomod = IOmod::getInstance();
  RenderContext* renderContext  = RenderContext::getInstance();
  std::string sheetPath = gdata.getXmlStr(name+"/file");
  SDL_Surface* spriteSurface = iomod.readSurface(sheetPath);
  bool transparency = gdata.getXmlBool(name+"/transparency");

  // It wasn't in the map, so we have to make the vector of Images:
  unsigned numberOfFrames = gdata.getXmlInt(name+"/frames");
  unsigned numberOfRows = gdata.getXmlInt(name+"/rows");
  
  std::vector<Image*> images;
  std::vector<SDL_Surface*> surfaces;
  std::vector<SDL_Texture*> textures;
  images.reserve(numberOfFrames * numberOfRows);
  surfaces.reserve(numberOfFrames * numberOfRows);
  textures.reserve(numberOfFrames * numberOfRows);

  int width = spriteSurface->w / numberOfFrames;
  int height = spriteSurface->h / numberOfRows;

  if(  gdata.checkTag(name+"/imageWidth") 
    && gdata.checkTag(name+"/imageHeight") ){
    width  = gdata.getXmlInt(name+"/imageWidth");
    height = gdata.getXmlInt(name+"/imageHeight");
  }

  SpriteSheet sheet(spriteSurface,width,height);

  for (unsigned i = 0; i < numberOfRows; ++i) {
    for (unsigned j = 0; j < numberOfFrames; j++) {
      SDL_Surface* surface = sheet(j, i);
      if ( transparency ) {
        int keyColor = SDL_MapRGBA(spriteSurface->format, 136, 136, 216, 255);
        SDL_SetColorKey(surface, SDL_TRUE, keyColor);
      }
      SDL_Texture* texture = 
        SDL_CreateTextureFromSurface(renderContext->getRenderer(),surface);
      surfaces.push_back( surface );
      textures.push_back( texture );
      images.push_back( new Image(surface, texture) );
    }   
  }
  multiSurfaces[name] = surfaces;
  multiTextures[name] = textures;
  multiImages[name] = images;
  return images;
}

std::vector<Image*> ImageFactory::getFolderImages(const std::string& name, const std::vector<std::string>& actions) {
  std::map<std::string, std::vector<Image*> >::const_iterator 
    pos = multiImages.find(name); 
  if ( pos != multiImages.end() ) {
    return pos->second;
  }

  IOmod& iomod = IOmod::getInstance();
  RenderContext* renderContext  = RenderContext::getInstance();
  bool transparency = gdata.getXmlBool(name+"/transparency");

  // It wasn't in the map, so we have to make the vector of Images:
  unsigned numberOfFrames = gdata.getXmlInt(name+"/frames");
  std::vector<Image*> images;
  std::vector<SDL_Surface*> surfaces;
  std::vector<SDL_Texture*> textures;
  images.reserve(numberOfFrames);
  surfaces.reserve(numberOfFrames);
  textures.reserve(numberOfFrames);

  for (std::string action : actions) {
    std::string sheetPath = gdata.getXmlStr(name+"/folder") + "/" + action;  
    for (unsigned i = 0; i < 20; i++) {
      std::stringstream ss;
      ss << sheetPath << "/" << i << ".png";
      SDL_Surface* surface = iomod.readSurface(ss.str());
      if ( transparency ) {
        int keyColor = SDL_MapRGBA(surface->format, 255, 255, 255, 255);
        SDL_SetColorKey(surface, SDL_TRUE, keyColor);
      }
      SDL_Texture* texture = 
        SDL_CreateTextureFromSurface(renderContext->getRenderer(), surface);
      surfaces.push_back( surface );
      textures.push_back( texture );
      images.push_back( new Image(surface, texture) );
    }
  }
      
  multiSurfaces[name] = surfaces;
  multiTextures[name] = textures;
  multiImages[name] = images;
  return images;
}
#include <vector>
#include <SDL.h>
#include "menu.h"
#include "sound.h"
#include "introHud.h"

class MenuEngine {
public:
  MenuEngine ();
  ~MenuEngine ();
  bool play();

private:
  Clock& clock;
  SDL_Renderer * const renderer;
  World background;
  Menu menu;
  SDLSound& sound;
  IntroHud* introHud;
  void draw() const;
  void update(Uint32);

  MenuEngine(const MenuEngine&);
  MenuEngine& operator=(const MenuEngine&);
};

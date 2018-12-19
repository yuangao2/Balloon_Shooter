#ifndef INTROHUD__H
#define INTROHUD__H

#include "hud.h"

class IntroHud : public Hud {
public:
    IntroHud();
    IntroHud(const IntroHud&) = delete;
    IntroHud& operator=(const IntroHud&) = delete;
    virtual void draw() const;
private:
    SDL_Rect introHudFrame;
};
#endif
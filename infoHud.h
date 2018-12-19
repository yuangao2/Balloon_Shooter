#ifndef INFOHUD__H
#define INFOHUD__H

#include "hud.h"

class InfoHud : public Hud {
public:
    InfoHud();
    InfoHud(const InfoHud&) = delete;
    InfoHud& operator=(const InfoHud&) = delete;
    virtual void draw() const;

private:
    SDL_Rect infoHudFrame;
};
#endif
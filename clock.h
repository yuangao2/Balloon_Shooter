#ifndef CLOCK__H
#define CLOCK__H

#include <SDL.h>
#include <string>

class Engine;

class Clock {
public:
  static Clock& getInstance();
  unsigned int getTicks() const;

private:
  friend class Engine;

  bool started;
  bool paused;

  const bool FRAME_CAP_ON;
  const Uint32 PERIOD;

  unsigned int frames;

  unsigned int timeAtStart;
  unsigned int timeAtPause;
  unsigned int currTicks;
  unsigned int prevTicks;
  unsigned int ticks;

  unsigned int getElapsedTicks();
  void incrFrame();
  void toggleSloMo();

  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  unsigned int getFrames() const  { return frames;  }
  unsigned int getSeconds() const { return getTicks()/1000;  }
  unsigned int capFrameRate() const;
  int getFps() const;
  int getAvgFps() const;

  void startClock();
  void pause();
  void unpause();

  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
};

#endif

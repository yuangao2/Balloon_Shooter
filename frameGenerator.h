#include <string>
#include <SDL.h>

class FrameGenerator {
public:
  FrameGenerator();
  void makeFrame();
  unsigned int getFrameCount() const { return frameCount; }
private:
  SDL_Renderer* const rend;
  SDL_Window* const window;
  const int WIDTH;
  const int HEIGHT;
  const std::string USERNAME;
  const int MAX_FRAMES;
  int frameCount;
  FrameGenerator(const FrameGenerator&);
  FrameGenerator& operator=(const FrameGenerator&);
};

#ifndef SOUND__H
#define SOUND__H

#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>

// In this demo, we use Mix_Music *Mix_LoadMUS(const char *file)
// Where file is the name of the music file to use. 
//
// SDL_mixer supports music and sound samples from the following formats:
// - WAVE/RIFF (.wav)
// - AIFF (.aiff)
// - VOC (.voc)
// - MOD (.mod .xm .s3m .669 .it .med and more) using included mikmod
// - MIDI (.mid) using timidity or native midi hardware
// - OggVorbis (.ogg) requiring ogg/vorbis libraries on system
// - MP3 (.mp3) requiring SMPEG library on system
// - also any command-line player, which is not mixed by SDL_mixer...
//   that requires a command to play with.

class SDLSound {
public:
  static SDLSound& getInstance();
  ~SDLSound();

  void playStartMusic();
  void playGameMusic();
  void playEndMusic();
  void pauseMusic();
  void stopMusic();      // stop all sounds
  void toggleMusic();    // toggle music on/off
  void operator[](int);  // play the indexed sound
private:
  int volume;
  int currentSound;
  // Mix_Music *music;

  int audioRate;
  int audioChannels;
  int audioBuffers;
  std::vector<Mix_Music*> musics;
  std::vector<Mix_Chunk*> sounds;
  std::vector<int> channels;

  SDLSound();
  SDLSound(const SDLSound&);
  SDLSound& operator=(const SDLSound&);
};


#endif
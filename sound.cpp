#include <string>
#include <iostream>
#include <ctime>
#include "sound.h"
using std::string;

SDLSound& SDLSound::getInstance() {
  static SDLSound instance;
  return instance;
}

SDLSound::~SDLSound() {
  std::cout << "Cleaning up sounds ..." << std::endl;
  std::clock_t start = std::clock();
  Mix_HaltMusic();
  // Mix_FreeMusic(music);
  for (auto music : musics) {
    Mix_FreeMusic(music);
  }
  for (unsigned int i = 0; i < sounds.size(); ++i) {
    Mix_FreeChunk(sounds[i]);
  }
  Mix_CloseAudio();
  std::clock_t duration = std::clock() - start;
  std::cout << "Clean up took " << duration << " ticks\n";
}

SDLSound::SDLSound() : 
  volume(SDL_MIX_MAXVOLUME/2), 
  currentSound(-1), 
  audioRate(22050), 
  audioChannels(2), 
  audioBuffers(4096),
  musics(),
  sounds(),
  channels() 
{

  if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, 
                   audioBuffers)){
    throw string("Unable to open audio!");
  }

  musics.push_back( Mix_LoadMUS("sound/start.mp3") );
  musics.push_back( Mix_LoadMUS("sound/game.mp3") );
  musics.push_back( Mix_LoadMUS("sound/end.mp3") );
  // music = ;
  // Need to install midi to play the following:
  // music = Mix_LoadMUS("sound/ballad2.mid");
  // if (!music) throw string("Couldn't load start.mp3")+Mix_GetError();

  // playStartMusic();
  // Mix_PauseMusic(); 

  sounds.push_back( Mix_LoadWAV("sound/bow_fire.wav") );
  sounds.push_back( Mix_LoadWAV("sound/balloon_pop.wav") );
  sounds.push_back( Mix_LoadWAV("sound/crash.wav") );

  for (unsigned int i = 0; i < sounds.size(); ++i) channels.push_back(i);
  std::cout << "Music and Sound is loaded" << std::endl;
}

void SDLSound::toggleMusic() {
  if( Mix_PausedMusic() ) { 
    Mix_ResumeMusic(); 
  } 
  else { 
    Mix_PauseMusic(); 
  } 
}

void SDLSound::operator[](int index) {
  if (currentSound >= 0) Mix_HaltChannel(currentSound);
  currentSound = index;
  Mix_VolumeChunk(sounds[index], volume);
  channels[index] = 
     Mix_PlayChannel(-1, sounds[index], 0);
}

void SDLSound::playStartMusic() {
  Mix_HaltMusic();
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(musics[0], -1);
}

void SDLSound::playGameMusic() {
  Mix_HaltMusic();
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(musics[1], -1);
}

void SDLSound::playEndMusic() {
  Mix_HaltMusic();
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(musics[2], -1);
}

void SDLSound::stopMusic() {
  Mix_HaltMusic();
  for (auto music : musics) {
    Mix_FreeMusic(music);
  }
}


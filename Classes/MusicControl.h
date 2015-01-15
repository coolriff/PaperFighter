#ifndef MUSIC_CONTROL
#define MUSIC_CONTROL

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d;
using namespace cocos2d::experimental;

class MusicControl{
public:

	void playMusic(std::string name);
	
	void playMusic(std::string name, bool loop, float volume);
	void stopMusic(int audioID);
	void stopAllMusic();
	void playMenuMusic();
	void playClickMenu();
	void playLevelMusic1();
	void playLevelMusic2();
	void playLevelMusic3();
	void playNextLevel();
	void playExplosion1();
	void playExplosion2();
	void playShootMusic();
	void playGameOver();


};

#endif
#include "MusicControl.h"

void MusicControl::playMusic(std::string name)
{

}

void MusicControl::playMusic(std::string name, bool loop, float volume)
{

}

void MusicControl::stopMusic(int id)
{
	AudioEngine::stop(id);
}

void MusicControl::stopAllMusic()
{
	AudioEngine::stopAll();
}

void MusicControl::playMenuMusic()
{
	int audioID = AudioEngine::play2d("Music/Title.mp3", true, 1);
}

void MusicControl::playClickMenu()
{
	int audioID = AudioEngine::play2d("Music/GainMissile.mp3", false, 1);
}

void MusicControl::playLevelMusic1()
{
	int audioID = AudioEngine::play2d("Music/Stage1.mp3", true, 1);
}

void MusicControl::playLevelMusic2()
{
	int audioID = AudioEngine::play2d("Music/Stage2.mp3", true, 1);
}

void MusicControl::playLevelMusic3()
{
	int audioID = AudioEngine::play2d("Music/Stage3.mp3", true, 1);
}

void MusicControl::playExplosion1()
{
	int audioID = AudioEngine::play2d("Music/Explosion.mp3", true, 1);
}

void MusicControl::playExplosion2()
{
	int audioID = AudioEngine::play2d("Music/MissileExplosion.mp3", true, 1);
}

void MusicControl::playShootMusic()
{
	int audioID = AudioEngine::play2d("Music/shoot.mp3", false, 1);
	//int audioID = AudioEngine::play2d("Music/Shoot.mp3", false, 1);
}

void MusicControl::playNextLevel()
{
	int audioID = AudioEngine::play2d("Music/Fly.mp3", true, 1);
}

void MusicControl::playGameOver()
{
	int audioID = AudioEngine::play2d("Music/GameOver.mp3", true, 1);
}

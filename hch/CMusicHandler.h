#ifndef __CMUSICHANDLER_H__
#define __CMUSICHANDLER_H__

#include <boost/thread/mutex.hpp>

#include "CSoundBase.h"
#include "CMusicBase.h"


/*
 * CMusicHandler.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

class CSndHandler;
struct _Mix_Music;
typedef struct _Mix_Music Mix_Music;
struct Mix_Chunk;

class CMusicHandler
{
private:
	bool audioInit;

	CSndHandler *sndh;
	soundBase::soundID getSoundID(std::string &fileName);

	std::map<soundBase::soundID, Mix_Chunk *> soundChunks;

	Mix_Chunk *GetSoundChunk(soundBase::soundID soundID);

	// Because we use the SDL music callback, our music variables must
	// be protected
	boost::mutex musicMutex;
	Mix_Music *currentMusic;
	Mix_Music *nextMusic;
	int nextMusicLoop;

public:
CMusicHandler(): audioInit(false), sndh(NULL), currentMusic(NULL) {};

	~CMusicHandler();

	void initMusics();
	void initCreaturesSounds(std::vector<CCreature> &creatures);

	// Sounds
	int playSound(soundBase::soundID soundID, int repeats=0);
	int playSoundFromSet(std::vector<soundBase::soundID> &sound_vec);
	void stopSound(int handler);

	// Sets
	std::vector<soundBase::soundID> pickup_sounds;
	std::vector<soundBase::soundID> horseSounds;

	// Musics
	std::map<musicBase::musicID, std::string> musics;
	std::vector<musicBase::musicID> battleMusics;

	void playMusic(musicBase::musicID musicID, int loop=1);
	void playMusicFromSet(std::vector<musicBase::musicID> &music_vec, int loop=1);
	void stopMusic(int fade_ms=1000);
	void musicFinishedCallback(void);
};

#endif // __CMUSICHANDLER_H__

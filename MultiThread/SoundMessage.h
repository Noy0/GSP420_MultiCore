#pragma once
#include "Message.h"

enum SoundMessage
{
	MSG_PLAY,
	MSG_STOP,
	MSG_SET_VOL
};

struct SMessagePlay: public SMessage
{
	SMessagePlay(int newSong, int newLoop):SMessage(CORE_SOUND, MSG_PLAY),song(newSong),loop(newLoop)

	{}

	int song;
	int loop;
};

struct SMessageStop: public SMessage
{
	SMessageStop(int newSong):SMessage(CORE_SOUND, MSG_STOP),song(newSong)

	{}

	int song;

};

struct SMessageSetVol: public SMessage
{
	SMessageSetVol(int newSong, int newVol):SMessage(CORE_SOUND, MSG_SET_VOL),song(newSong),Vol(newVol)

	{}

	int song;
	int Vol;
};
#ifndef _CORELIST_H
#define _CORELIST_H

//Enum Core is the ID for all cores.
//For each core, list in ECore.
enum ECore
{
	CORE_NULL,
	CORE_GAME,
	CORE_GRAPHICS,
	CORE_ENTITY,
	CORE_SOUND,
	CORE_SCRIPTING,
	CORE_UI,
	CORE_AI,
	CORE_NETWORKING
};

//Define the frames per second for each core here.
#define FPS_CORE_GAME 30.0f
#define FPS_CORE_GRAPHICS 60.0f
#define FPS_CORE_UI 15.0f
#define FPS_CORE_ENTITY 60.0f
#define FPS_CORE_SOUND 10.0f
#define FPS_CORE_SCRIPTING 20.0f
#define FPS_CORE_AI 2.0f
#define FPS_CORE_NETWORKING 20.0f

#endif
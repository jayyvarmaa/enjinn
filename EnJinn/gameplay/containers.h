#pragma once

#include <enjinnConfig.h>
#include <baseContainer.h>
//this is used to declare containers




#include <memoryArena/memoryArena.h>


Container *getContainer(const char* name, enjinn::memory::MemoryArena *memoryArena);


#include "containers/enjinnGameplay.h"
#include "containers/threedtest.h"

#include "pluggins/immageviewer.h"
#include "pluggins/threeDEditor.h"
#include "pluggins/enjinntextEditor.h"
#include "pluggins/sushiViewer/sushiViewer.h"

#include "containers/mario/mario.h"
#include "containers/mario/marioEditor.h"
#include "containers/mario/marioNeuralVizualizer.h"
#include "containers/mario/marioNeuralTrainer.h"
#include "containers/minecraftDungeons/mcDungeonsEditor.h"
#include "containers/minecraftDungeons/mcDungeonsgameplay.h"
#include "containers/minecraftDungeons/mcDungeonsMenu.h"
#include "containers/hollowknight/hollowknight.h"

#include "containers/marioKart/marioKartEditor.h"
#include "containers/marioKart/marioKart.h"

#include "containers/threeDGameExample/threeDGameExample.h"
#include "containers/threeDGameExample/threeDGameMenu.h"


#include "containers/isometricGame/isometricGameEditor.h"
#include "containers/isometricGame/isometricGame.h"

#include "containers/bezie/bezie.h"

#include "containers/physicsTest/physicsTest.h"
#include "containers/physicsTest/logo.h"

#include "containers/angryBirds/angryBirds.h"
#include "containers/milk/milk.h"

// #include "containers/silksong/silkSong.h"  // Excluded - missing pika framework dependencies

#include "containers/my3DGame/my3DGame.h"
#include "containers/cubeIO/cubeIO_inspected.h"

#if ENJINN_PRODUCTION == 1

#define ENJINN_ALL_CONTAINERS() \
	//ENJINN_DECLARE_CONTAINER(McDungeonsGameplay) \
	//ENJINN_DECLARE_CONTAINER(McDungeonsMenu) 
	ENJINN_DECLARE_CONTAINER(Gameplay) 

#else

#define ENJINN_ALL_CONTAINERS() \
	ENJINN_DECLARE_CONTAINER(Gameplay) \
	/* ENJINN_DECLARE_CONTAINER(SilkSong) */ \
	ENJINN_DECLARE_CONTAINER(SushiViewer) \
	ENJINN_DECLARE_CONTAINER(ImmageViewer) \
	ENJINN_DECLARE_CONTAINER(ThreeDTest) \
	ENJINN_DECLARE_CONTAINER(ThreeDEditor) \
	ENJINN_DECLARE_CONTAINER(Holloknight)\
	ENJINN_DECLARE_CONTAINER(Milk) \
	ENJINN_DECLARE_CONTAINER(EnJinnTextEditor) \
	ENJINN_DECLARE_CONTAINER(ThreeDGameExample) \
	ENJINN_DECLARE_CONTAINER(ThreeDGameMenu) \
	ENJINN_DECLARE_CONTAINER(MarioKartEditor) \
	ENJINN_DECLARE_CONTAINER(MarioKart) \
	ENJINN_DECLARE_CONTAINER(IsometricGameEditor) \
	ENJINN_DECLARE_CONTAINER(IsometricGame) \
	ENJINN_DECLARE_CONTAINER(Mario) \
	ENJINN_DECLARE_CONTAINER(Bezie)\
	ENJINN_DECLARE_CONTAINER(PhysicsTest)\
	ENJINN_DECLARE_CONTAINER(Logo)\
	ENJINN_DECLARE_CONTAINER(MarioEditor) \
	ENJINN_DECLARE_CONTAINER(MarioNeuralTrainer) \
	ENJINN_DECLARE_CONTAINER(McDungeonsGameplay) \
	ENJINN_DECLARE_CONTAINER(AngryBirds) \
	ENJINN_DECLARE_CONTAINER(MarioNeuralVizualizer) \
	ENJINN_DECLARE_CONTAINER(McDungeonsEditor) \
	ENJINN_DECLARE_CONTAINER(McDungeonsMenu) \
	ENJINN_DECLARE_CONTAINER(My3DGame) \
	ENJINN_DECLARE_CONTAINER(CubeIO)

	

#endif

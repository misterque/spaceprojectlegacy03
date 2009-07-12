#ifndef _GAME_H_
#define _GAME_H_

#include <Ogre/Ogre.h>
#include <newton.h>

class Game {
	static NewtonWorld* nWorld;
	static Ogre::SceneManager* sceneMgr;
	
	
	
	public:
	
//	Game();

	static void setWorld(NewtonWorld* world);
	static void setSceneManager(Ogre::SceneManager* mgr);
	
	static NewtonWorld* getWorld();
	static Ogre::SceneManager* getSceneManager();


};


#endif
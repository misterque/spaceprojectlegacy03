#include "game.h"

	NewtonWorld* Game::nWorld = NULL;	
	Ogre::SceneManager* Game::sceneMgr = NULL;
	
	void Game::setWorld(NewtonWorld* world){
		nWorld = world;
	}
	void Game::setSceneManager(Ogre::SceneManager* mgr){
		sceneMgr = mgr;
	}
	
	NewtonWorld* Game::getWorld(){
		return nWorld;
	}
	
	Ogre::SceneManager* Game::getSceneManager(){
		return sceneMgr;
	}
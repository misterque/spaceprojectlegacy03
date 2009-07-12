#include "fighter.h"
#include "game.h"
#include "gamem.h"

Fighter::Fighter(){
	reload = 0;
	laserreload = 0;
	ribbontime = 0;
	this->setModel("Fighter.mesh", 1.0f);
	
	this->maxvelocity = Ogre::Vector3(30,30,100);
	this->maxrotation = Ogre::Vector3(1,1,1);
	
	p = Game::Game::getSceneManager()->createParticleSystem("Engine"+Ogre::StringConverter::toString(objectCount), "Ship/Engine2");
	/*
	r = Game::Game::getSceneManager()->createRibbonTrail("Trail"+Ogre::StringConverter::toString(objectCount));
//	r->setTrailLength(100);
	
	r->setMaterialName("Examples/LightRibbonTrail");
   r->setTrailLength(400);
   r->setInitialColour(0, Ogre::ColourValue::Red );
   r->setColourChange(0, 0.5, 0.5, 0.5, 0.5);
   r->setInitialWidth(0, 2.5); 

	r->addNode(this->getNode());*/

		Ogre::NameValuePairList pairList;
		pairList["numberOfChains"] = "2";
		pairList["maxElements"] = "80";
//		Ogre::RibbonTrail* trail = static_cast<Ogre::RibbonTrail*>(
//		Game::Game::getSceneManager()->createMovableObject("Trail"+Ogre::StringConverter::toString(objectCount), "RibbonTrail", &pairList));
		Ogre::RibbonTrail* trail =	Game::Game::getSceneManager()->createRibbonTrail("Trail"+Ogre::StringConverter::toString(objectCount));
		
		trail->setMaterialName("Examples/LightRibbonTrail");
		trail->setTrailLength(600);
		
		

 Game::Game::getSceneManager()->getRootSceneNode()->createChildSceneNode()->attachObject ( trail );

	Ogre::SceneNode *eNode = this->getNode()->createChildSceneNode();
	
		trail->setInitialColour(0, 0.5f, 0.5f, 0.0f , 0.3f);
		trail->setColourChange(0, 0.15f, 0.15f, 0.15f, 0.17f);
		trail->setWidthChange (0, 0.2f);
		trail->setInitialWidth(0, 1.5);
		
		trail->addNode(eNode);
			eNode->attachObject ( p );
		eNode->setPosition(0, 0, 5 );
		this->r = trail;

}

void Fighter::live(float time){
	if(reload > 0) reload -= time;
	if(laserreload > 0) laserreload -= time;
	
	if(ribbontime > 0) ribbontime -= time;
	
	if(ribbontime <= 0) {
		//r->
	}
	
	if(v.z < 0) {
		r->setInitialColour(0, -0.5f*v.z,-0.5f*v.z, 0.0f , 0.3f);
		p->getEmitter(0)->setMinParticleVelocity(-7 * v.z);
		p->getEmitter(0)->setMaxParticleVelocity(-8 * v.z);
		p->getEmitter(0)->setEnabled(true);
		
	} else {
		r->setInitialColour(0,0,0, 0.0f , 0.3f);
		p->getEmitter(0)->setMinParticleVelocity(0);
		p->getEmitter(0)->setMaxParticleVelocity(0);
		p->getEmitter(0)->setEnabled(false);
	}
}

	void Fighter::fireLaser(Ogre::Vector3 direction){
		if(laserreload > 0) return;
		laserreload = 0.3f;
	
		Laser *rocket = GameManager::createLaser();
					rocket->v.z=-1;
				rocket->setPosition(this->getNode()->getPosition() +  this->getNode()->getOrientation() * Ogre::Vector3 (0, 0, -7));
				Ogre::Vector3 bla = Ogre::Vector3(0,0,-1.0f);
				rocket->setRotation(  bla.getRotationTo(direction));
				rocket->resetPhysics();
				
											Ogre::LogManager::getSingleton().logMessage(	Ogre::StringConverter::toString(direction.x)+"/"+ 
																			Ogre::StringConverter::toString(direction.y)+"/"+ 
																			Ogre::StringConverter::toString(direction.z) );
	
	}


void Fighter::fireRocket(){
	if(reload > 0) return;
	reload = 2.0f;
	
	Rocket *rocket = GameManager::createRocket();
					rocket->v.z=-1;
				rocket->setPosition(this->getNode()->getPosition() +  this->getNode()->getOrientation() * Ogre::Vector3 (0, 0, -7));
				rocket->setRotation(this->getNode()->getOrientation());
				rocket->resetPhysics();
				
			}


void Fighter::contact(Object* c){
//	this->die();
}


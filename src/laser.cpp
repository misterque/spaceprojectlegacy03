#include "game.h"
#include "laser.h"

Laser::Laser(){
	this->age = 0;
	this->setModel("", 1.0f);

	this->maxvelocity = Ogre::Vector3(0,0,400);
	this->maxrotation = Ogre::Vector3(0,0,0);
	
	p = Game::Game::getSceneManager()->createParticleSystem("Engine"+Ogre::StringConverter::toString(objectCount), "Weapon/Sunbeam");
	this->getNode()->attachObject ( p );
	NewtonBodySetMassMatrix (this->objectBody, 0.1f, 5.0f, 1.0f, 5.0f);


}

Laser::~Laser(){
//	Game::getSceneManager()->destroyParticleSystem(p);
}

void Laser::live(float time){
	age += time;
	if(age > 3.0f) this->die();
	if(age > 0.2) this->v.z = 0;
}

void Laser::contact(Object* c){
	this->die();
}
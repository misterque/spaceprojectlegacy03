#include "game.h"
#include "rocket.h"

Rocket::Rocket(){
	this->age = 0;
	this->setModel("Sphere.001.mesh", 1.0f);

	this->maxvelocity = Ogre::Vector3(0,0,200);
	this->maxrotation = Ogre::Vector3(0,0,0);
	
	p = Game::Game::getSceneManager()->createParticleSystem("Engine"+Ogre::StringConverter::toString(objectCount), "Rocket");
	this->getNode()->attachObject ( p );


}

Rocket::~Rocket(){
//	Game::getSceneManager()->destroyParticleSystem(p);
}

void Rocket::live(float time){
	age += time;
	if(age > 30.0f) this->die();
}

void Rocket::contact(Object* c){
	this->die();
}
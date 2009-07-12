#ifndef _ROCKET_H_
#define _ROCKET_H_


#include "object.h"

class Rocket : public Object {
	private:
	float age;
	Ogre::ParticleSystem* p;
	
	
	public:
	Rocket();
	~Rocket();
	
	void live(float time);
	void contact(Object* c);
};


#endif
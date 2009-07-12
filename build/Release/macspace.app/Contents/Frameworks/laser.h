#ifndef _LASER_H_
#define _LASER_H_


#include "object.h"

class Laser : public Object {
	private:
	float age;
	Ogre::ParticleSystem* p;
	
	
	public:
	Laser();
	~Laser();
	
	void live(float time);
	void contact(Object* c);
};


#endif
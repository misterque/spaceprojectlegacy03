#include "object.h"

class Fighter : public Object {
	private:
	Ogre::ParticleSystem *p;
	
	Ogre::RibbonTrail *r;
	
	float ribbontime;
	
	float reload;
	float laserreload;
	
	public:
	Fighter();
	void fireRocket();
	void fireLaser(Ogre::Vector3 direction);
	void live(float time);
	
		void contact(Object* c);
};
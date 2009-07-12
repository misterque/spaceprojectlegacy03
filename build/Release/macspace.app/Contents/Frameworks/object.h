#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <Ogre/Ogre.h>
#include <newton.h>
#include <string>




class  Object {
	private:
	
	// Ogre Node
	Ogre::SceneNode* modelNode;
	Ogre::SceneNode* baseNode;
	Ogre::SceneNode* collNode;
	
	Ogre::Entity *mesh;
	
	
	

	// Newton Physics

	// Funktionen
	std::string name;
	
	bool alive;
	
	protected:
	NewtonBody* objectBody;

		Ogre::Vector3 maxvelocity;
		Ogre::Vector3 maxrotation;
		static int objectCount;
	public:
	
	Object();
	~Object();
	
	void setName(std::string n);
	std::string getName();
	
	void addCollBox(Ogre::Vector3 size, Ogre::Vector3 pos);
	
	Ogre::SceneNode* getNode();
	
	void setPosition(Ogre::Vector3 point);
	void setRotation(Ogre::Quaternion rot);
	
	void setModel(std::string file, float scaling);
	
	void resetPhysics();
	
	void update();
		
	void addForces();
	
	Ogre::Vector3 v;
	
	Ogre::Vector3 steer;
	
	void live(float time);
	
	virtual void contact(Object* c);
	
	void die();
	
	bool is_alive();

	
	//void PhysicsApplyForceAndTorque(const NewtonBody* body);


};

#endif
#include "game.h"
#include "object.h"

void MatrixToQuatPos( const float* matrix, Ogre::Quaternion& quat, Ogre::Vector3 &pos )
{
	// this takes a matrix returned by Newton, and creates a Quaternion
	// and position Vector3, which is more meaningful for Ogre.
	using namespace Ogre;
	quat = Quaternion( Matrix3(	matrix[0], matrix[4], matrix[8],
								matrix[1], matrix[5], matrix[9],
								matrix[2], matrix[6], matrix[10] ) );
	
	
	//		Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(matrix[10]) );
	
	pos = Vector3( matrix[12], matrix[13], matrix[14] );
}

int Object::objectCount = 0;

// Funktionen

void PhysicsApplyForceAndTorque(const NewtonBody* body){

	
	((Object*)NewtonBodyGetUserData(body))->addForces();
}

Object::Object(){

	alive = true;
	mesh = NULL;
	objectBody = NULL;
	maxvelocity = Ogre::Vector3(30,30,100);
	maxrotation = Ogre::Vector3(1,1,1);
	
	
	baseNode = Game::Game::getSceneManager()->getRootSceneNode()->createChildSceneNode();
	modelNode = baseNode->createChildSceneNode();
	collNode = baseNode->createChildSceneNode();

	modelNode->rotate(Ogre::Vector3(0,0,1), Ogre::Degree(-90));
	modelNode->rotate(Ogre::Vector3(0,1,0), Ogre::Degree(-90));

	this->setModel("Fighter.mesh", 1.0f);
	//mesh = Game::getSceneManager()->createEntity( "Mesh"+Ogre::StringConverter::toString(objectCount++), "Fighter.mesh" );	
	
	


	//node1->translate(0,0,-100);
	//node1->scale(1,1,1);
	
	//v = 0;

	this->addCollBox(Ogre::Vector3(3,3,3), Ogre::Vector3(0,0,0));
	
	

	
	
	this->name = "Object" + Ogre::StringConverter::toString(objectCount);
}

Object::~Object(){
	NewtonDestroyBody(Game::getWorld(), objectBody);

	Game::getSceneManager()->destroySceneNode(this->baseNode->getName());

}



void Object::setPosition(Ogre::Vector3 point){
	float p[16];
	
	NewtonBodyGetMatrix(this->objectBody, &p[0]);
	
	p[12] = point.x;
	p[13] = point.y;
	p[14] = point.z;
	
	
	NewtonBodySetMatrix(this->objectBody, &p[0]);
	
	this->update();
	
	
};

void Object::setRotation(Ogre::Quaternion rot){
	float p[16];
	
	NewtonBodyGetMatrix(this->objectBody, &p[0]);
	
			using namespace Ogre;
		Matrix3 rotneu;
		Vector3 xcol, ycol, zcol;
		
		rot.ToRotationMatrix( rotneu );	// creates a 3x3 rotation matrix from the Quaternion.

		xcol = rotneu.GetColumn(0);
		ycol = rotneu.GetColumn(1);
		zcol = rotneu.GetColumn(2);
	
		// now fill the final matrix with the appropriate data:
		p[0] = xcol.x;
		p[1] = xcol.y;
		p[2] = xcol.z;
		p[3] = 0.0f;
	
		p[4] = ycol.x;
		p[5] = ycol.y;
		p[6] = ycol.z;
		p[7] = 0.0f;
	
		p[8] = zcol.x;
		p[9] = zcol.y;
		p[10] = zcol.z;
		p[11] = 0.0f;

		p[15] = 1.0;
	
	p[12] = baseNode->getPosition().x;
	p[13] = baseNode->getPosition().y;
	p[14] = baseNode->getPosition().z;
	
	
	
	NewtonBodySetMatrix(this->objectBody, &p[0]);
	this->update();
	
	
};

void Object::resetPhysics(){
	float o[] = {0,0,0};
	NewtonBodySetVelocity(this->objectBody, &o[0]);
	NewtonBodySetOmega(this->objectBody, &o[0]);
	
	NewtonWorldUnfreezeBody(Game::getWorld(), this->objectBody);
}

void Object::setModel(std::string file, float scaling){

	if(this->mesh != NULL)	{
		modelNode->detachAllObjects();
		Game::getSceneManager()->destroyEntity(this->mesh);
	}
	
	if(file == "") return;
	
	this->mesh = Game::getSceneManager()->createEntity( "Mesh"+Ogre::StringConverter::toString(objectCount++), file );
	modelNode->attachObject ( this->mesh );
}



void Object::addCollBox(Ogre::Vector3 size, Ogre::Vector3 pos){

	if(this->objectBody != NULL) NewtonDestroyBody (Game::getWorld(), this->objectBody);

	NewtonCollision* collision = NewtonCreateBox (Game::getWorld(), size.x,size.y,size.z, NULL);

	Ogre::Entity *collbox = Game::getSceneManager()->createEntity( "Collbox"+Ogre::StringConverter::toString(objectCount++), "uCube.mesh" );	
	Ogre::SceneNode *collboxnode = collNode->createChildSceneNode();
	collboxnode->attachObject (collbox);
	
	collboxnode->setScale(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
	
	this->objectBody = NewtonCreateBody (Game::getWorld(), collision);
	
	
	NewtonReleaseCollision (Game::getWorld(), collision);
	
		NewtonBodySetMassMatrix (this->objectBody, 1.0f, 5.0f, 1.0f, 5.0f);
	NewtonBodySetLinearDamping(this->objectBody, 0.2f);


				// set the force and torque call back function
				NewtonBodySetForceAndTorqueCallback (this->objectBody, PhysicsApplyForceAndTorque);
	NewtonBodySetAutoFreeze (objectBody, 0);
	
	NewtonBodySetUserData(this->objectBody, this);


}


void Object::update(){
	float mat[16];
	NewtonBodyGetMatrix(this->objectBody, &mat[0]);
	
	Ogre::Vector3 pos;
	Ogre::Quaternion quart;
	
	MatrixToQuatPos(mat, quart, pos);
	
	baseNode->setOrientation(quart);
	baseNode->setPosition(pos);
	//Ogre::LogManager::getSingleton().logMessage(this->name + "!" );

}


void addGlobalForce (NewtonBody* body, Ogre::Vector3* force, Ogre::Vector3* point) {
	float p[16];
	
	NewtonBodyGetMatrix(body, &p[0]);


   //R = Point – BodyMatrix.Position;
   float r[3] = {	point->x - p[12],
					point->y - p[13],
					point->z - p[14] };


   //Torque = CrossProduct (R, Force);
   float torque[3] = {	r[1] * force->z - r[2] * force->y,
						r[2] * force->x - r[0] * force->z,
						r[0] * force->y - r[1] * force->x };
						
   //NewtonAddForce (Force)
   float f[] = { force->x, force->y, force->z };
   NewtonBodyAddForce(body,&f[0]);
   NewtonBodyAddTorque(body, torque);
}

void addLocalForce (NewtonBody* body, Ogre::Vector3* force, Ogre::Vector3* point) {
   //GlobalForce = BodyMatrixRotate (Force)
   	float p[16];
	
	NewtonBodyGetMatrix(body, &p[0]);
	
    Ogre::Matrix4 bodyMatrix(	p[0], p[4], p[8],  p[12], 
								p[1], p[5], p[9],  p[13], 
								p[2], p[6], p[10], p[14], 
								p[3], p[7], p[11], p[15] ); 
								
/*
   Ogre::Matrix4 bodyMatrix(	p[0], p[1], p[2],  p[3], 
								p[4], p[5], p[9],  p[13], 
								p[8], p[9], p[10], p[11], 
								p[12], p[13], p[14], p[15] ); 
*/



						
	Ogre::Vector3 gf = bodyMatrix.extractQuaternion()  * *force;
/*Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(gf.x) );	
Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(gf.y) );	
Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(gf.z) );	
Ogre::LogManager::getSingleton().logMessage("--" );	*/


	Ogre::Vector3 gp = bodyMatrix.getTrans() + ( bodyMatrix.extractQuaternion() * *point);
								
   //GlobalPoint = BodyMatrixTranform (localPoint)

	
	addGlobalForce (body, &gf, &gp);
}

void Object::addForces(){

	float p[16];
	
	NewtonBodyGetMatrix(this->objectBody, &p[0]);
	
	
	//dVector omega (1.0f, 1.0f, 1.0f);
	
	// engine

	Ogre::Vector3 o = Ogre::Vector3::ZERO;
	Ogre::Vector3 f = this->v * this->maxvelocity;
	addLocalForce(this->objectBody,	&f, &o );
	
	// rotation z achse
	f = Ogre::Vector3::ZERO;
	f.y = this->steer.z * this->maxrotation.z;
	o.x = -3.0f;

	addLocalForce(	this->objectBody, &f, &o );
	f.y = -(this->steer.z) * this->maxrotation.z;
	o.x = 3.0f;
	addLocalForce(	this->objectBody, &f, &o );
	
	// rotation x achse
	f = Ogre::Vector3::ZERO;
	o = Ogre::Vector3::ZERO;
	f.y = this->steer.x * this->maxrotation.x;
	o.z = -4.0f;
	addLocalForce(	this->objectBody, &f, &o );
	
	// rotation y achse
	f = Ogre::Vector3::ZERO;
	o = Ogre::Vector3::ZERO;
	f.x = -this->steer.y * this->maxrotation.y;
	o.z = 3.0f;

	addLocalForce(	this->objectBody, &f, &o );
/*	
	Ogre::Vector3 a = Ogre::Vector3(1, 0 , 0);
	Ogre::Vector3 b = Ogre::Vector3(0, 0 , 3);

	addLocalForce( this->objectBody, &a, &b );
*/
/*	
	// rotation y achse
	addLocalForce(	this->objectBody,	
					Ogre::Vector3( this->steer.y,	0,		0 ),
					Ogre::Vector3( 0.0f,			0.0f,	3.0f) );
	
	// rotation x achse
	addLocalForce(	this->objectBody,	
					Ogre::Vector3( 0,		this->steer.x,		0 ),
					Ogre::Vector3( 0.0f,	0.0f,	-4.0f) );
	*/
//	NewtonBodySetOmega (rigidBodyBox, &omega[0]);
	//NewtonBodySetVelocity(this->objectBody, &v[0]);
	//NewtonBodyAddForce(this->objectBody,&v[0]);
	

}

void Object::setName(std::string n){
	this->name = n;
}

std::string Object::getName(){
	return name;
}

Ogre::SceneNode* Object::getNode(){
	return this->baseNode;
}

void Object::live(float time){
}

void Object::die(){
	this->alive = false;
	
}

bool Object::is_alive(){
	return this->alive;
}	

void Object::contact(Object* c){
	Ogre::LogManager::getSingleton().logMessage("bla");

}
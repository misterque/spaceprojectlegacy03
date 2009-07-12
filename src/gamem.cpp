#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

// Xcode will search a framework for the include automatically
// given you follow the rule of <FRAMEWORK_NAME/HEADER.h>
#include <Ogre/Ogre.h>
#include <Ogre/OgreFrameListener.h>

#include <newton.h>

#include "gamem.h"
#include "scene.h"
#include "object.h"
#include "fighter.h"
#include "rocket.h"
#include "game.h"


OIS::InputManager* mInputManager;
OIS::Keyboard* mKeyboard;
OIS::Mouse* mMouse;
OIS::JoyStick* mGamepad;

Ogre::SceneNode* node1; 

// Die Camera Node
Ogre::SceneNode* camNode;
Ogre::Camera* camera;
Ogre::SceneNode* envNode;
Ogre::SceneNode* rocketNode;

	Ogre::Overlay* mouseOverlay;
	Ogre::OverlayContainer* mouseContainer;

Fighter* test1;
Fighter* test2;

//Rocket* rocket;
list<Rocket*> rockets;
list<Laser*> lasers;

Ogre::Root* root;

float mousex, mousey;



bool appGo;


struct contact {
	Object* o1;
	Object* o2;
};

int  GenericContactBegin (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1){
	Ogre::LogManager::getSingleton().logMessage("kontakt" );
	Ogre::LogManager::getSingleton().logMessage(((Object*)NewtonBodyGetUserData(body0))->getName());
	Ogre::LogManager::getSingleton().logMessage(((Object*)NewtonBodyGetUserData(body1))->getName());
	((Object*)NewtonBodyGetUserData(body1))->contact((Object*)NewtonBodyGetUserData(body0));
	((Object*)NewtonBodyGetUserData(body0))->contact((Object*)NewtonBodyGetUserData(body1));
	
	return true;
}

////////////////////////////////////////////////////////////////
// class SimpleFrameListener
//////////////////////////////////////////////////////////////// 
class SimpleFrameListener : public Ogre::FrameListener
{


	// This gets called before the next frame is beeing rendered.
	bool frameStarted(const Ogre::FrameEvent& evt) { 
		mKeyboard->capture();
		mMouse->capture();
		OIS::MouseState mouseState = mMouse->getMouseState();
		
		mousex += mouseState.X.rel * 0.005;
		mousey += mouseState.Y.rel * 0.005;
//		Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(mouseState.buttonDown(OIS::MB_Right) ) );
//		if(mouseState.buttonDown(OIS::MB_Left)) Ogre::LogManager::getSingleton().logMessage("bla" );



		
		if(mousex < 0) mousex = 0;
		if(mousex > 1) mousex = 1;
		if(mousey < 0) mousey = 0;
		if(mousey > 1) mousey = 1;
		
		mouseContainer->setPosition(mousex-0.025f,mousey-0.0335f);
		
		test1->steer.x = 0;
		test1->steer.y = 0;
		test1->steer.z = 0;
		
		if (mKeyboard->isKeyDown(OIS::KC_R)) {
			test2->v.z = -1;
		} else {
					test2->v.z = 0;
		}
		
		test1->steer.x = -2.0f * (mousey-0.0335f-0.5f);
		test1->steer.y =  2.0f * (mousex-0.025f-0.5f);
		test1->steer.z = test1->steer.y * 0.5f;
		

		test1->v = Ogre::Vector3::ZERO;
		
		if (mKeyboard->isKeyDown(OIS::KC_P)) {
			appGo = false;
		}
		if (mKeyboard->isKeyDown(OIS::KC_LEFT)) {
			test1->steer.z = -0.5f;	
			test1->steer.y = -1;	
		}
		if (mKeyboard->isKeyDown(OIS::KC_RIGHT)) {
			test1->steer.z =  0.5f;	
			test1->steer.y =  1.0f;	
			
		}
		if (mKeyboard->isKeyDown(OIS::KC_UP)) {
			test1->steer.x = -1;	
		}
		if (mKeyboard->isKeyDown(OIS::KC_DOWN)) {
			test1->steer.x = 1;	
		}
		if (mKeyboard->isKeyDown(OIS::KC_Q)) {
			test1->steer.z = -1;	
		}
		if (mKeyboard->isKeyDown(OIS::KC_E)) {
			test1->steer.z = 1;	
		}
		if (mKeyboard->isKeyDown(OIS::KC_W)) {
			test1->v.z = -1;
		}
		if (mKeyboard->isKeyDown(OIS::KC_S)) {
			test1->v.z = 1;
		}
		if (mKeyboard->isKeyDown(OIS::KC_A)) {
			test1->v.x = -1;
		}
		if (mKeyboard->isKeyDown(OIS::KC_D)) {
			test1->v.x = 1;
		}

		if (mKeyboard->isKeyDown(OIS::KC_SPACE)) {
		            Ray mouseRay = camera->getCameraToViewportRay(mousex,mousey);
				test1->fireLaser( mouseRay.getDirection() );
		}
		if (mKeyboard->isKeyDown(OIS::KC_LMENU)) {
				test1->fireRocket( );
		}
		
		
		
		/*	OIS::JoyStickState s = mGamepad->getJoyStickState();
		if (s.buttonDown(1)) {
			camNode->translate( Ogre::Vector3(0,0,1), Ogre::Node::TS_LOCAL );
		}*/
		//node1->rotate(Ogre::Vector3(1,0,0),Ogre::Degree(0.2f), Ogre::Node::TS_LOCAL );
		//	node1->yaw(Ogre::Degree(0.3),Ogre::Node::TS_LOCAL);
		NewtonUpdate (Game::getWorld(), evt.timeSinceLastFrame);
		
		test1->update();
		test2->update();
		
		test1->live(evt.timeSinceLastFrame);
		test2->live(evt.timeSinceLastFrame);
		/*	 vector<int>::iterator iter1 = v.begin();
		while( iter1 != v.end() && *iter1 % 2 != 0 ) {
			iter1++;
		}    */
		
		list<Rocket*>::iterator iter = rockets.begin();
		while( iter != rockets.end() ) {
			(*iter)->update();
			(*iter)->live(evt.timeSinceLastFrame);
			
			if((*iter)->is_alive() == false) {
				list<Rocket*>::iterator del = iter;
				iter++;
				rockets.erase(del);
				//Ogre::LogManager::getSingleton().logMessage("Die!" );
				
				delete (*del);
				
			} else { 
				iter++;
			}
			
		}		
		list<Laser*>::iterator iterl = lasers.begin();
		while( iterl != lasers.end() ) {
			(*iterl)->update();
			(*iterl)->live(evt.timeSinceLastFrame);
			
			if((*iterl)->is_alive() == false) {
				list<Laser*>::iterator del = iterl;
				iterl++;
				lasers.erase(del);
				//Ogre::LogManager::getSingleton().logMessage("Die!" );
				
				delete (*del);
				
			} else { 
				iterl++;
			}
			
		}
		//				rocket->update();
		//		rocket->live(evt.timeSinceLastFrame);
		
		
		
		Ogre::Vector3 dest = test1->getNode()->getWorldPosition() + test1->getNode()->getOrientation() * Ogre::Vector3 (0, 10, 40);
		Ogre::Vector3 dv = dest - camNode->getWorldPosition();
		
		float d = evt.timeSinceLastFrame * 5; //time * 0.01 *(dv.length() - 40) / (40 + dv.length());
		camNode->translate(dv*d);
		//this->pNode->setPosition(dest);
		//camNode->translate(test1->getNode()->getOrientation() * (mTarget->getVelocity() * time));
		
		envNode->setPosition(camNode->getPosition());
		
		Quaternion q = test1->getNode()->getWorldOrientation();
		Quaternion qq =  camNode->getOrientation();
		
		q.normalise();
		qq.normalise();
		Quaternion p = Quaternion::Slerp(evt.timeSinceLastFrame*10, qq, q, true);
		camNode->setOrientation(p);
		//this->mCameraNode->lookAt(this->mTarget->getNode()->getPosition() + this->mTarget->getNode()->getOrientation() * Vector3 (0, 40, 0), Node::TS_WORLD);
		//*/
		//camNode->lookAt(test1->getNode()->getPosition() + (test1->getNode()->getOrientation() * Vector3 (0, 10, 0)), Node::TS_WORLD);
		
		
		return true; 
		
	} 
	// This gets called at the end of a frame.
	bool frameEnded(const Ogre::FrameEvent& evt) { return appGo; } 
};



void GameManager::init(std::string resourcePath){
	
	appGo = true;
	// Initialize Ogre
	// In this case we will not use a plugins.cfg, or ogre.cfg
	root = new Ogre::Root("", "", resourcePath + "Ogre.log");
	
	// Load the plugins
	root->loadPlugin("RenderSystem_GL");
	root->loadPlugin("Plugin_OctreeSceneManager");
	
	//Additional Plugins
	root->loadPlugin("Plugin_ParticleFX");
	/*root->loadPlugin("Plugin_BSPSceneManager");
	root->loadPlugin("Plugin_CgProgramManager");
	*/
	
	// Setup the renderer
	root->setRenderSystem(root->getAvailableRenderers()->front());
	
	// Set up the render system
	root->getRenderSystem()->setConfigOption( "Colour Depth"       , "32"        );
	root->getRenderSystem()->setConfigOption( "FSAA"               , "0"         );
	root->getRenderSystem()->setConfigOption( "Full Screen"        , "No"        );
	root->getRenderSystem()->setConfigOption( "RTT Preferred Mode" , "PBuffer"   );
	root->getRenderSystem()->setConfigOption( "Video Mode"         , "800 x 600" );
	//root->getRenderSystem()->setConfigOption( "Video Mode"         , "1280 x 800" );
	
	// Create the render window
	Ogre::RenderWindow* window = root->initialise(true, "macspace");
	
	// Add resource locations -- looking at folders recursively
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, std::string("FileSystem"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
	// Create a SceneManager
	Game::setSceneManager( root->createSceneManager(Ogre::ST_GENERIC) );
	
	// Create a Camera
	camera = Game::getSceneManager()->createCamera("macspaceCamera");
	
	// Create the viewport
	Ogre::Viewport* viewPort = window->addViewport(camera);
	
	// Setup the Event Handler to call our doWindowClose function
	// when a close event is recieved
	
	
	// Install our FrameListener
	Ogre::FrameListener* frameListener = new SimpleFrameListener();
    root->addFrameListener(frameListener);
	
	
	// OIS anwerfen
	// http://www.ogre3d.org/wiki/index.php/Using_OIS
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	
	window->getCustomAttribute("WINDOW", &windowHndStr);
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	
	mInputManager = OIS::InputManager::createInputSystem( pl );
	
	mMouse = static_cast<OIS::Mouse*> (mInputManager->createInputObject ( OIS::OISMouse, true ));
	mKeyboard = static_cast<OIS::Keyboard*> (mInputManager->createInputObject ( OIS::OISKeyboard, false ));
	   const OIS::MouseState &mouseState = mMouse->getMouseState();
    mouseState.width  = 800;
    mouseState.height = 600;
	
	mousex = 0.5f; mousey = 0.5f;
	
	
	mouseContainer = (Ogre::OverlayContainer*) Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("Panel", "MouseCursor");
	mouseContainer->setMaterialName("Mouse/Crosshair");

	
	mouseContainer->setDimensions(0.05f,0.0677f);
	
	mouseOverlay = OverlayManager::getSingletonPtr()->create("MouseCursor");
	mouseOverlay->add2D(mouseContainer);
	mouseOverlay->show();
	
	mGamepad = static_cast<OIS::JoyStick*> (mInputManager->createInputObject ( OIS::OISJoyStick, true ));
	if(mInputManager->numJoySticks() == 0) 	Ogre::LogManager::getSingleton().logMessage("NULL" );
	
	// Dirks testcode:
	
	camera->setNearClipDistance(5.0f);
	camera->setFarClipDistance(50000.0f);
	
	Game::getSceneManager()->setAmbientLight( Ogre::ColourValue(0.0f,0.1f,0.2f) );
	
	
	
	Ogre::Light *sun = Game::getSceneManager()->createLight("Sun");
	//	sun->setAttenuation(<#Real range#>,<#Real constant#>,<#Real linear#>,<#Real quadratic#>)
	
	
	
	camNode = Game::getSceneManager()->getRootSceneNode()->createChildSceneNode( "mCameraNode" );
	envNode = Game::getSceneManager()->getRootSceneNode()->createChildSceneNode( "envNode" );
	Ogre::SceneNode *pNode = Game::getSceneManager()->getRootSceneNode()->createChildSceneNode( "pNode" );
	Ogre::ParticleSystem *p = Game::getSceneManager()->createParticleSystem("Sun", "Space/Sun");
	pNode->attachObject(p);
	pNode->setPosition(Ogre::Vector3(0,0,500));
	
	
	camNode->attachObject ( camera );

	
	
	Environment env(Game::getSceneManager(), camera, "");
	
	
	
	
	// Newton scheiß
	
	
	
	Game::setWorld( NewtonCreate ( NULL, NULL) );
	
	float min[3];
	float max[3];
	
	min[0] = -5000.0;
	min[1] = -5000.0;
	min[2] = -5000.0;
	
	max[0] = 5000.0;
	max[1] = 5000.0;
	max[2] = 5000.0;
	
	NewtonSetWorldSize( Game::getWorld(), min, max );
	
	int defaultID = NewtonMaterialGetDefaultGroupID (Game::getWorld());

// Set default material properties
/*
NewtonMaterialSetDefaultSoftness (nWorld, defaultID, defaultID, 0.05f);
NewtonMaterialSetDefaultElasticity (nWorld, defaultID, defaultID, 0.4f);
NewtonMaterialSetDefaultCollidable (nWorld, defaultID, defaultID, 1);
NewtonMaterialSetDefaultFriction (nWorld, defaultID, defaultID, 1.0f, 0.5f);*/

contact std;
NewtonMaterialSetCollisionCallback (Game::getWorld(), defaultID, defaultID, &std, GenericContactBegin, NULL, NULL);  
	
	test1 = new Fighter();
	test2 = new Fighter();
	
	//	Rocket* t = new Rocket();
	//	Rocket* s = new Rocket();
	
	
	test1->setName("test1");
	test2->setName("test2");
	test2->setModel("gunship.mesh", 1.0f);
	test2->addCollBox(Ogre::Vector3(5.0f, 10.0f, 20.0f), Ogre::Vector3::ZERO);
	//	rocket->setName("Rocket");
	//rocket->setModel("Sphere.001.mesh", 1.0f);
	
	//	Ogre::Vector3 v = OgreVector3
	test1->setPosition(Ogre::Vector3(0	,0	,100));
	test2->setPosition(Ogre::Vector3(4	,0	,0));
	
	
	//test1->update();
	//test2->update();
	//	test2->push();
	
	//dVector omega (1.0f, 1.0f, 1.0f);
	/*float omega[] = {0.0f, 0.0f, 0.0f};
	float v[] = {0.0f, 0.0f, -2.0f};
	
	
	NewtonBodySetOmega (rigidBodyBox, &omega[0]);
	NewtonBodySetVelocity(rigidBodyBox, &v[0]);
	*/
	
}

// Start Rendering
// Ogre will keep rendering until a frame listener returns false
// In our case we set up a custom Event UPP for the close events
void GameManager::run(){	
	root->startRendering();
	
	// Clean up
	
	delete test1, test2;
	
	list<Rocket*>::iterator iter = rockets.begin();
	while( iter != rockets.end() ) {
		
		delete (*iter);
		iter++;
		
	}
		list<Laser*>::iterator iterl = lasers.begin();
	while( iterl != lasers.end() ) {
		
		delete (*iterl);
		iterl++;
		
	}
	delete root;
	
	NewtonDestroy (Game::getWorld());
	
	return;
}

void GameManager::stop(){
	appGo = false;
}

Rocket* GameManager::createRocket(){
				Rocket* rocket = new Rocket();
	rockets.push_back(rocket);
	return rocket;
}

Laser* GameManager::createLaser(){
				Laser* rocket = new Laser();
	lasers.push_back(rocket);
	return rocket;
}
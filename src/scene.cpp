#include "scene.h"


Environment::Environment(SceneManager* mScene,Camera* mCam, string scenefile)  {
	mSceneManager = mScene;
	starsnumber = 0;
    initialise(mScene, mCam);
}

Environment::~Environment() {
/* for(unsigned int i = 0; i < this->Lights.size(); i++){
    delete this->Lights[i].second;
  }*/
}


void Environment::addStars(Vector3 pos, Vector3 size, float scale, string material, ColourValue color, int random, int number) {
	BillboardSet* billy = this->mSceneManager->createBillboardSet("Stars"+StringConverter::toString(this->starsnumber), number);
	this->starsnumber++;
	
    billy->setMaterialName(material);
    billy->setRenderQueueGroup(RENDER_QUEUE_SKIES_EARLY);

        billy->setDefaultDimensions(scale, scale);

        for(unsigned int i=0; i<number; i++){
              billy->createBillboard(Math::RangeRandom(-size.x, size.x),
                                     Math::RangeRandom(-size.y, size.y),
                                     Math::RangeRandom(-size.z, size.z));
        }
      Ogre::SceneNode *billynode = this->mSkyNode->createChildSceneNode("StarsNode"+StringConverter::toString(this->starsnumber), pos);
      billynode->attachObject(billy);
}

void setSkymodel(string file){

}


void Environment::initialise(SceneManager* mScene, Camera* mCam) {
    // Pointer zum GameManagerSingleton holen
   // GameManager *mGameManager = GameManager::GetInstancePointer();
//    SceneManager *mScene = mGameManager->mSceneManager;

   // mScene->setAmbientLight( AmbientLight );
   


    this->mCam = mCam;
    this->mSkyNode = mScene->getSceneNode("envNode"); //)->getParentSceneNode()->createChildSceneNode("SkyBoxNode", this->mCam->getDerivedPosition());
    Entity* mesh = mScene->createEntity( "SkyModel", "SkySphere.mesh" );
    mesh->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);

    this->mSkyNode->attachObject(mesh);



	this->mSkyNode->rotate(Vector3(1,0,0),Degree(90),Node::TS_LOCAL);
    this->mSkyNode->setScale(50,50,50);

	this->addStars(	Vector3(0,0,0),
					Vector3(1,1,1),
					0.01f,
					"Stars/Standard2",
					ColourValue::White,
					4,
					50);
	
	this->addStars(	Vector3(0,0,0),
					Vector3(1,1,1),
					0.008f,
					"Stars/Standard",
					ColourValue::White,
					0,
					100);
					
	this->addStars(	Vector3(0,0,0),
					Vector3(2,2,0.2f),
					0.01f,
					"Stars/Blue",
					ColourValue::White,
					0,
					100);	
	this->addStars(	Vector3(1,1,0),
					Vector3(0.5f,0.5f,0.5f),
					0.01f,
					"Stars/Red",
					ColourValue::White,
					0,
					10);

/*

    vector< pair<Vector3, cLight*> >::iterator iter = Lights.begin();
    while( iter != Lights.end() ) {
      iter->second->initialise(mScene,iter->first);
      iter++;
    }
	
	*/
}


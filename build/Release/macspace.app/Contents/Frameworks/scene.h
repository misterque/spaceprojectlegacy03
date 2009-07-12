#ifndef SCENE_H_
#define SCENE_H_

#include <Ogre/Ogre.h>
#include <string>
#include <vector>
#include <map>
/*
#include "../objects/object.h"
#include "../objects/light.h"
#include "../opt/xmlutil.h"
*/
using namespace std;
using namespace Ogre;


struct Stars {
    Vector3 pos;
    Vector3 size;
    float   scale;
    string material;
    ColourValue color;
    int random;
    unsigned int number;

};

class Environment {

    public:

        Environment(SceneManager* mScene,Camera* mCam, string scenefile);
		
		void addStars(Vector3 pos, Vector3 size, float scale, string material, ColourValue color, int random, int number);
		void setSkymodel(string file);

        // Funktion zum synchronisieren der Skybox mit der aktuellen Kamera
        void syncSkyBox();

        // Destruktor
        ~Environment();

    private:
        //Liste von Sternen
        vector<Stars> Starses;
		
		// Anzahl an Sternen
		unsigned int starsnumber;

        // Liste von LightObjekten und deren Positionen
   //     vector< pair<Vector3, cLight*> > Lights;

        // Liste von statischen Objekten in dieser Szene
      //  vector<cObject*> Statics;


        SceneNode* mSkyNode;
		SceneManager* mSceneManager;

     //   ColourValue AmbientLight;
       // string SkyBox;

        Camera* mCam;

        // Interne Funktion zum initialisieren der Objekte im SzenenManager
        void initialise(SceneManager* mScene, Camera* mCam);

        // Interne Funktion zum parsen der XMLDatei in die Objektvariablen
      //  void parseXML(string file) throw(string);

};

#endif /*SCENE_H_*/

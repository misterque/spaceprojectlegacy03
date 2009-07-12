#include "rocket.h"
#include "laser.h"
class GameManager {
	private:


	public:
	
	static Rocket* createRocket();
	static Laser* createLaser();
	
	void init(std::string resourcePath);
	void run();
	void stop();
};
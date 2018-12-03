#include<cstdlib>

class Body {
	public:
		int width;
		int height;
		int positionX;
		int positionY;
		float velocityX;
		float velocityY;
		Body()
		{
			width = 50;
			height = 100;
			positionY = 0;
			positionX = 100;
			velocityX = 0.0f;
		}
};

class Enem {
	public:
		int wid;
		int hgt;
		float posX;
		float posY;
		float velX;
		float velY;
		bool patrol;
		Enem()
		{
			wid = 35;
			hgt = 35;
			posX = 200;
			posY = 0;
			velY = 0.0f;
			velX = 0.0f;
			patrol = false;
		}
		Enem(float pos)
		{	

			wid = 50;
			hgt = 90;
			posY = 0;
			velY = 0.0f;
			velX = 0.0f;
			patrol = false;

			this->posX = pos;

		}
};

class Fall {
	public:
		int w;
		int h;
		float pX;
		float pY;
		float vX;
		float vY;
		float grav;
		float vel;
		Fall() {
			w = 40;
			h = 40;
			pX =  ((rand() % 1250) +1);
			pY = 600;
			vY = 0.0f;
			grav = 16.0f;
			vel = 0;
		}
};


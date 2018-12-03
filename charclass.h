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
                width = 40;
                height = 120;
                positionY = 100;
                positionX = 0;
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
                        wid = 50;
                        hgt = 90;
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
	int w;
	int h;
	float pX;
	float pY;
	float vX;
	float vY;
	Fall() {
		w = 20;
		h = 40;
		pX = 0;
		pY = 1605;
		pX = 0.0f;
		vY = 0.0f;
	}
};0


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
                height = 150;
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


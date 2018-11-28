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
                positionX = 0;
                velocityX = 0.0f;
        }
};

class Enem {
        public:
                int wid;
                int hgt;
                int posX;
                int posY;
                float velX;
                float velY;
                Enem()
                {
                        wid = 25;
                        hgt = 50;
                        posX = 0;
                        posY = 0;
                        velY = 0.0f;
                        velX = 0.0f;

                }
};


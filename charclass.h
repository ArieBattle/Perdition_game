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
                velocityY = 0.0f;
        }
};


#ifndef LED_H
#define LED_H

#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 2

///////////////////////////////

#include <DMD.h>

#define PIXELS_WIDE ((DISPLAYS_ACROSS * DMD_PIXELS_ACROSS))
#define PIXELS_TALL ((DISPLAYS_DOWN * DMD_PIXELS_DOWN))
#define PIXELS_TOTAL ((PIXELS_WIDE * PIXELS_TALL))

class ImgDMD : public DMD {
	public:
		ImgDMD(byte panelsWide, byte panelsHigh);
		void drawArray(int* array, byte bGraphicsMode);
		void clearAndDrawArray(int* array, byte bGraphicsMode);
		void drawFilledCircle(int xCenter, int yCenter, int radius, byte bGraphicsMode);
		void posSquareTo(int* array, int dlay);
		void negSquareTo(int* array, int dlay);
		void posCircleTo(int* array, int dlay);
                void bubbleTo(int* array, int dlay);
	private:
		void drawFilledCircleSub(int cx, int cy, int x, int y, byte bGraphicsMode);
		int pythag(int a, int b);
};

#endif

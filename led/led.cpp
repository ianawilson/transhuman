#include "led.h"
#include <avr/pgmspace.h>

// constructor for ImgDMD simply passes things on up to DMD
ImgDMD::ImgDMD(byte panelsWide, byte panelsHigh) : DMD::DMD(panelsWide, panelsHigh) {
  // do nothing
}


// our interesting methods

void ImgDMD::drawArray(int* array, byte bGraphicsMode) 
{
  for (int i=0; i< PIXELS_TOTAL; i++) {
    if(pgm_read_word_near(array+i)) {
      int x = i % PIXELS_WIDE;
      int y = i / PIXELS_WIDE;
      
      writePixel(x, y, bGraphicsMode, true);
    }
  }
}

void ImgDMD::clearAndDrawArray(int* array, byte bGraphicsMode)
{
  // clear it
  clearScreen(true);
  
  drawArray(array, bGraphicsMode);
}

void ImgDMD::drawFilledCircle(int xCenter, int yCenter, int radius, byte bGraphicsMode)
{
    int x = 0;
    int y = radius;
    int p = (5 - radius * 4) / 4;
	
    drawFilledCircleSub(xCenter, yCenter, x, y, bGraphicsMode);
    while (x < y) {
	    x++;
	    if (p < 0) {
	        p += 2 * x + 1;
	    } else {
	        y--;
	        p += 2 * (x - y) + 1;
	    }
	    drawFilledCircleSub(xCenter, yCenter, x, y, bGraphicsMode);
    }
}

void ImgDMD::drawFilledCircleSub(int cx, int cy, int x, int y, byte bGraphicsMode)
{
	
    if (x == 0) {
	    writePixel(cx + y, cy, bGraphicsMode, true);
	    writePixel(cx - y, cy, bGraphicsMode, true);
		drawLine(cx,cy+y,cx,cy-y,bGraphicsMode);
    } else if (x == y) {
		drawLine(cx+x, cy+y, cx+x, cy-y, bGraphicsMode);
		drawLine(cx-x, cy+y, cx-x, cy-y, bGraphicsMode);
    } else if (x < y) {
		drawLine(cx+x, cy+y, cx+x, cy-y, bGraphicsMode);
		drawLine(cx-x, cy+y, cx-x, cy-y, bGraphicsMode);
		drawLine(cx+y, cy+x, cx+y, cy-x, bGraphicsMode);
		drawLine(cx-y, cy+x, cx-y, cy-x, bGraphicsMode);
    }
}

void ImgDMD::posSquareTo(int* array, int dlay)
{
	for (int i=0; i<PIXELS_WIDE/2-1; i++) {
		drawBox(  0+i,  0+i, PIXELS_WIDE-1-i, PIXELS_TALL-1-i, GRAPHICS_OR );
		delay(dlay);
	}
	for (int i=PIXELS_WIDE/2-1; i>=0; i--) {
		drawBox(  0+i,  0+i, PIXELS_WIDE-1-i, PIXELS_TALL-1-i, GRAPHICS_NOR );
		drawArray(array, GRAPHICS_NORMAL);
		delay(dlay);
	}
}

void ImgDMD::negSquareTo(int* array, int dlay)
{
	for (int i=0; i<PIXELS_WIDE/2-1; i++) {
		drawBox(  0+i,  0+i, PIXELS_WIDE-1-i, PIXELS_TALL-1-i, GRAPHICS_NOR );
		delay(dlay);
	}
	for (int i=PIXELS_WIDE/2-1; i>=0; i--) {
		drawBox(  0+i,  0+i, PIXELS_WIDE-1-i, PIXELS_TALL-1-i, GRAPHICS_OR );
		//drawArraySubtractive();
		delay(dlay);
	}
}

void ImgDMD::posCircleTo(int* array, int dlay) {
	int centerX = PIXELS_WIDE/2;
	int centerY = PIXELS_TALL/2;
	int radius = pythag(PIXELS_WIDE-centerX, PIXELS_TALL-centerY);
	for (float i=0; i<radius; i++) {
		drawFilledBox( 0, 0, PIXELS_WIDE-1, PIXELS_TALL-1, GRAPHICS_OR );
		drawFilledCircle( centerX, centerY, radius-i, GRAPHICS_NOR);
		drawArray(array, GRAPHICS_NORMAL);
		delay(dlay);
	}
	for (int i=radius; i>=0; i--) {
		drawFilledCircle( centerX, centerY, radius-i, GRAPHICS_NOR);
		drawArray(array, GRAPHICS_NORMAL);
		delay(dlay);
	}
}

int ImgDMD::pythag(int a, int b) {
	return sqrt(a*a+b*b);
}
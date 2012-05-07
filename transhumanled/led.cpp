#include "led.h"
#include <avr/pgmspace.h>
#include<stdio.h>

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

void ImgDMD::bubbleTo(int* array, int bubblesWide, int dlay) { 

  #define bubblesWide bubblesWide
  #define r (PIXELS_WIDE/(2*bubblesWide)) //radius of each bubble
  #define bubblesTall (PIXELS_TALL/(2*r))
  #define offsetBubblesWide (bubblesWide+1) //bubbles that fill in the gaps between central bubbles
  #define offsetBubblesTall (bubblesTall+1)
  #define numBubbles ((bubblesTall*bubblesWide+offsetBubblesTall*offsetBubblesWide)
  
  Serial.print(r);
  
  typedef struct{
  int x; int y; int radius;
  } bubble;
  
  int bubbleIndex = -1;
  bubble barry[numBubbles];
  //init central bubbles
  for (int i=0; i<bubblesWide; i++) {
    for (int j=0; j<bubblesTall; j++) {
      bubbleIndex++;
      barry[bubbleIndex]=(bubble){r+i*2*r, r+j*2*r, r};
    }
  }
  //init offset bubbles
  for (int i=0; i<offsetBubblesWide; i++) {
	for (int j=0; j<offsetBubblesTall; j++) {
	  bubbleIndex++;
	  barry[bubbleIndex]=(bubble){i*2*r, j*2*r, r};
	}	
  }
  
  //randomizes the order in which the bubbles are placed
  int order[numBubbles];
  for (int i=0; i<numBubbles; i++)
    order[i]=i;
  int a; int b;
  for (int i=0; i<numBubbles*2; i++) {
	a = random(numBubbles);
	b = random(numBubbles);
	int temp = order[a];
	order[a] = order[b];
	order[b] = temp;
  }	  
  
  //place bubbles on LED
  for (int i=0; i<numBubbles; i++) {
    drawFilledCircle(barry[order[i]].x, barry[order[i]].y, barry[order[i]].radius, GRAPHICS_NORMAL);
    delay(dlay);
  }
  //in case there are gaps, finish the job
  drawFilledBox(0,0,PIXELS_WIDE,PIXELS_TALL, GRAPHICS_NORMAL);
}

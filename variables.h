#ifndef VARIABLES.H
#define VARIABLES.H

#define GROUND_LEVEL 0
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define NUMBER_OF_TRAPS 1

int backgroundY = 0;
int scroll = 0;
int charY = 40;
int charX = 40;
int index;
int distanceTraveled = 0;
int gamestate = 0;
int apressed = 0;
int bpressed = 0;
unsigned char jumpcoord[] = {40, 37, 32, 28, 25, 23, 21, 19, 18, 17, 16, 16, 15, 15, 14, 14, 14,14, 15, 15, 16, 16, 17, 17, 18,19, 20, 21, 23, 25, 28, 32, 37, 40};
unsigned char getImageWidth(const byte *image) {
  return pgm_read_byte(image); 
}

unsigned char getImageHeight(const byte *image) {
  return pgm_read_byte(image + 1);
}
#endif

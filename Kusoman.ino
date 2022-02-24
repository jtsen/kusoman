#include "sprites.h"
#include "variables.h"
#include <Arduboy2.h>
#include <Tinyfont.h>
Arduboy2 ab;
Tinyfont tinyfont = Tinyfont(ab.sBuffer, Arduboy2::width(), Arduboy2::height());


typedef struct Kuso{
  int x;
  int y;
  bool jumping;
  byte jumpIndex;
  const byte *image;
  const byte *mask;
};

typedef struct Trap{
  int x;
  int y;
  int type;
  bool active;
  const byte *image;
};

Trap trap [NUMBER_OF_TRAPS];

Kuso kuso = {charX, charY, false, 0, kusoman, kusomanmask};

enum bgtype {
  bg1,
  bg2,
  bg3,
  bg4,
  bg5,
};

const byte *backgroundimages [] = {background_1, background_2, background_3, background_4, background_5};
const byte *kusoman_images [] = {kusoman};
const byte *kusoman_masks [] = {kusomanmask};
const byte *trapimages [] = {spike, angryman};

bgtype back[5] = {
  bgtype::bg1,
  bgtype::bg2,
  bgtype::bg3,
  bgtype::bg4,
  bgtype::bg5,
};

void setup() {
  ab.begin();
  ab.setFrameRate(60);
  for (int i=0; i < NUMBER_OF_TRAPS; i++) {
    trap[i].active == false;
  }
  ab.clear();
}

void loop() {
  if(!ab.nextFrame()) return;
  ab.clear();
  
  if (gamestate == 0){
    beginningScreen();
  }
  
  if (gamestate == 1){
    playerControls();
    moveGround();
    drawKusoman();
    kusojump();
    
    for (int i = 0; i < NUMBER_OF_TRAPS; i++) {
      updateTrap(&trap[i]);
      drawTrap(trap[i]);
    }

    index = rand() % NUMBER_OF_TRAPS;
    
    if (!trap[index].active) {
      randomizeTrap(&trap[index]);  
    }

    if (collison() == true){
      gamestate = 2;
    }

    if (distanceTraveled==4000 or distanceTraveled==4001 or distanceTraveled==4002 or distanceTraveled==4003 or distanceTraveled == 4004 or distanceTraveled==4005 or distanceTraveled==4006 or distanceTraveled==4007 or distanceTraveled==4008 or distanceTraveled==4009 or distanceTraveled==4010){
      ab.clear();
      ab.display();
      gamestate = 3;
    }
  }

  if (gamestate == 2){
    youloseScreen();
    for(int i = 0; i < NUMBER_OF_TRAPS; i++){
      trap[i].active = false;
    }
    scroll = 0;
    distanceTraveled = 0;
  }

  if (gamestate == 3){
    youWinScreen();
    for(int i = 0; i < NUMBER_OF_TRAPS; i++){
      trap[i].active = false;
    }
    distanceTraveled = 0;
    scroll = 0;
  }
  
  ab.display();
}

void playerControls () {
   if (distanceTraveled <= 1000) {
    if (ab.pressed(RIGHT_BUTTON) and distanceTraveled <= 4000 and gamestate == 1) {
      scroll += 7;
      distanceTraveled += 7;
    }
  
    if (ab.pressed(LEFT_BUTTON) and distanceTraveled <= 4000 and gamestate == 1) {
      scroll += 2;
      distanceTraveled += 2;
    }
   }

   if (distanceTraveled > 1000 and distanceTraveled <= 2000){
    if (ab.pressed(LEFT_BUTTON) and distanceTraveled <= 4000 and gamestate == 1) {
      scroll += 7;
      distanceTraveled += 7;
    }
  
    if (ab.pressed(RIGHT_BUTTON) and distanceTraveled <= 4000 and gamestate == 1) {
      scroll += 2;
      distanceTraveled += 2;
    }
   }

   if (distanceTraveled > 2000 and distanceTraveled <= 3000) {
    if (ab.pressed(RIGHT_BUTTON) and distanceTraveled <= 4000 and gamestate == 1) {
      scroll += 7;
      distanceTraveled += 7;
    }
  
    if (ab.pressed(LEFT_BUTTON) and distanceTraveled <= 4000 and gamestate == 1) {
      scroll += 2;
      distanceTraveled += 2;
    }
   }

   if (distanceTraveled > 3000 and distanceTraveled <= 4000){
    if (ab.pressed(LEFT_BUTTON) and distanceTraveled <= 4000 and gamestate == 1) {
      scroll += 7;
      distanceTraveled += 7;
    }
  
    if (ab.pressed(RIGHT_BUTTON) and distanceTraveled <= 4000 and gamestate == 1) {
      scroll += 2;
      distanceTraveled += 2;
    }
   }

  if(ab.pressed(B_BUTTON) and kuso.jumping == false and gamestate == 1){
    kuso.jumping = true;
    kuso.jumpIndex = 0;
  }
}

void moveGround () {
  if (scroll>=32 and distanceTraveled <= 4000 and gamestate == 1) {
    scroll = 0;
    byte type = random(0,4);
    switch (type) { 
      case 0:
      type = bgtype::bg1;
      break;
      
      case 1:
      type = bgtype::bg2;
      break;
      
      case 2:
      type = bgtype::bg3;
      break;
      
      case 3:
      type = bgtype::bg4;
      break;

      case 4:
      type = bgtype::bg5;
      break;
    }
    back[0] = back[1];
    back[1] = back[2];
    back[2] = back[3];
    back[3] = back[4];
    back[4] = (bgtype)type;
  }

  for (byte i=0; i<5; i++) {
    ab.drawBitmap(i*32 - scroll,backgroundY,backgroundimages[back[i]], 32, 64, WHITE);
  };
}

void drawKusoman(){
  ab.drawBitmap(kuso.x, kuso.y, kuso.image, 16, 20, WHITE);
  ab.drawBitmap(kuso.x, kuso.y, kuso.mask, 16, 20, BLACK);
}

void kusojump(){
  if(gamestate == 1){
    if(kuso.jumping == true){
      kuso.y = jumpcoord[kuso.jumpIndex];
      kuso.jumpIndex++;
      if(kuso.jumpIndex == sizeof(jumpcoord)){
        kuso.jumping = false;
        kuso.jumpIndex = 0;
        kuso.y = charY;
      }
    }
  }
}

void beginningScreen(){
  ab.drawBitmap(0, 0, startscreen, 128, 64, WHITE);
  tinyfont.setCursor(27,55);
  tinyfont.print("PRESS A TO START!");

  if(ab.notPressed(A_BUTTON) and apressed == 1){
    apressed = 0;
  }

  if(ab.pressed(A_BUTTON) and apressed == 0){
    apressed = 1;
    if (apressed = 1){
      gamestate = 1;
      apressed = 0;
    }
  }
}

bool collison(){
  for (int i = 0; i < NUMBER_OF_TRAPS; i++){
    if (trap[i].active == true){
      Rect KUSO = {kuso.x, kuso.y, 16, 20};
      Rect TRAP = {trap[i].x, trap[i].y, getImageWidth(trap[i].image), getImageHeight(trap[i].image)};
      Rect TRAP2 = {trap[i].x, trap[i].y, getImageHeight(trap[i].image), getImageWidth(trap[i].image)};
      if(ab.collide(KUSO, TRAP)){
        return true;
      }
      if(ab.collide(KUSO, TRAP2)){
        return true;
      }
    }
  }
 return false;
}

void youloseScreen(){
  ab.drawBitmap(0, 0, losescreen, 128, 64, WHITE);
  if(ab.pressed(A_BUTTON) and apressed == 0){
    apressed = 1;
    gamestate = 0;
  }
  if(ab.notPressed(A_BUTTON) and apressed == 0){
    apressed = 0;
  }
}

void drawTrap(Trap t){
  if(!t.active) return;
  switch(t.type){
    case 0:
    ab.drawBitmap(t.x, t.y, t.image, 16, 20, WHITE);
    break;

    case 1:
    ab.drawBitmap(t.x, t.y, t.image, 25, 4, WHITE);
    break;
  }
}

void updateTrap(Trap *t){
  if (!(t->active)) return;
  if (distanceTraveled <= 1000){
    if (ab.pressed(RIGHT_BUTTON) and distanceTraveled <= 4000){
      t->x -= 7;
    }
    if (ab.pressed(LEFT_BUTTON) and distanceTraveled <= 4000){
      t->x -= 2;
    }
  }
  if (distanceTraveled > 1000 and distanceTraveled <= 2000){
    if (ab.pressed(LEFT_BUTTON) and distanceTraveled <= 4000){
      t->x -= 7;
    }
    if (ab.pressed(RIGHT_BUTTON) and distanceTraveled <= 4000){
      t->x -= 2;
    }
  }

  if (distanceTraveled > 2000 and distanceTraveled <= 3000){
    if (ab.pressed(RIGHT_BUTTON) and distanceTraveled <= 4000){
      t->x -= 7;
    }
    if (ab.pressed(LEFT_BUTTON) and distanceTraveled <= 4000){
      t->x -= 2;
    }
  }

  if (distanceTraveled > 3000 and distanceTraveled <= 4000){
    if (ab.pressed(LEFT_BUTTON) and distanceTraveled <= 4000){
      t->x -= 7;
    }
    if (ab.pressed(RIGHT_BUTTON) and distanceTraveled <= 4000){
      t->x -= 2;
    }
  }

  if (t->x <= -10){
    t->active = false;
  }
}

void randomizeTrap(Trap *t){
  int trapp = rand()  % 2;
  switch(trapp){
    case 0:
    t->active = true;
    t->x = SCREEN_WIDTH-rand() % 40;
    t->y = SCREEN_HEIGHT-22;
    t->image = angryman;
    t->type = 0;
    break;

    case 1:
    t->active = true;
    t->x = SCREEN_WIDTH-rand() % 40;
    t->y = SCREEN_HEIGHT-9;
    t->image = spike;
    t->type = 1;
  }
}

void youWinScreen(){
  ab.drawBitmap(0, 0, winscreen, 128, 64,WHITE);
  if (ab.pressed(A_BUTTON) and apressed == 0){
    apressed = 1;
    gamestate = 0;
  }
  if (ab.notPressed(A_BUTTON) and apressed == 1){
    apressed = 0;
  }
  if (ab.pressed(B_BUTTON) and bpressed == 0){
    bpressed = 1;
    gamestate = 0;
  }
  if (ab.notPressed(B_BUTTON) and bpressed == 1){
    bpressed = 0;
  }
}


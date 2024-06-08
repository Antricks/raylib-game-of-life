#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <sys/param.h>

#include "data-structure.h"

static int camX = -40;
static int camY = -20;
static int cellWidth = 10;

static int screenWidth = 1000;
static int screenHeight = 500;

void updateCam(struct QTree *tree) {
  int min_x = minX(tree);
  int min_y = minY(tree);
  int max_x = maxX(tree);
  int max_y = maxY(tree);

  camX = min_x;
  camY = min_y;

  cellWidth = MIN(screenWidth / (1 + max_x - min_x),
                  screenHeight / (1 + max_y - min_y));

  printf("Updated cam to: x=%i y=%i cellWidth=%i (min_x=%i, max_x=%i, min_y=%i, max_y=%i)\n",
         camX, camY, cellWidth, min_x, max_x, min_y, max_y);
}

void drawCell(int x, int y) {
  DrawRectangleRec((Rectangle){.x = x * cellWidth - camX * cellWidth,
                               .y = y * cellWidth - camY * cellWidth,
                               .width = cellWidth,
                               .height = cellWidth},
                   WHITE);
}

int main(void) {
  struct QTree *tree = newTree(0, 0, 128, NULL);


  //  x
  // xx
  //  xx
  insertCell(tree, 1, 0);
  insertCell(tree, 0, 1);
  insertCell(tree, 1, 1);
  insertCell(tree, 1, 2);
  insertCell(tree, 2, 2);

  /*
  //   x
  // x x
  //  xx
  insertCell(tree, 0, 1);
  insertCell(tree, 1, 2);
  insertCell(tree, 2, 0);
  insertCell(tree, 2, 1);
  insertCell(tree, 2, 2);

  // xxx
  insertCell(tree, 0, 0);
  insertCell(tree, 0, 1);
  insertCell(tree, 0, 2);
  */

  updateCam(tree);

  InitWindow(screenWidth, screenHeight, "Hello Raylib");
  SetTargetFPS(10);

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(DARKGRAY);
    traverseWithCallbackOnCell(tree, drawCell);

    EndDrawing();

    tree = nextGeneration(tree);
    updateCam(tree);
  }

  freeTree(tree);

  return 0;
}

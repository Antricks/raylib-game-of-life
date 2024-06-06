#include <math.h>
#include <raylib.h>
#include <stdio.h>

#include "data-structure.h"

static int camX = -10;
static int camY = -10;
static int cellWidth = 10;

/*
void updateCam(struct QTree *tree) {
    camX = tree->xMin;
    camY = tree->yMin;
}
*/

void drawCell(int x, int y) {
    DrawRectangleRec((Rectangle){.x = x * cellWidth - camX * cellWidth,
                                 .y = y * cellWidth - camY * cellWidth,
                                 .width = cellWidth,
                                 .height = cellWidth},
                     WHITE);
}

int main(void) {
    struct QTree *tree = newTree(0, 0, 32, NULL);

    insertCell(tree, 10, 10);
    insertCell(tree, 11, 10);
    insertCell(tree, 12, 10);

    InitWindow(1000, 500, "Hello Raylib");
    SetTargetFPS(1);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(DARKGRAY);
        traverseWithCallbackOnCell(tree, drawCell);
        
        EndDrawing();

        nextGeneration(tree);
    }

    freeTree(tree);

    return 0;
}

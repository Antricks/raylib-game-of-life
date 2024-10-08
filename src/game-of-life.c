#include <raylib.h>
#include <stdio.h>
#include <sys/param.h>

#include "data-structure.h"

static int camX = -40;
static int camY = -20;
static float cellWidth = 10;

static int screenWidth = 1024;
static int screenHeight = 1024;

void updateCam(struct QTree *tree) {
    int min_x = minX(tree);
    int min_y = minY(tree);
    int max_x = maxX(tree);
    int max_y = maxY(tree);

    camX = min_x;
    camY = min_y;

    cellWidth = MIN((float)screenWidth / (1 + max_x - min_x), (float)screenHeight / (1 + max_y - min_y));

    printf("Updated cam to: x=%i y=%i cellWidth=%f (min_x=%i, max_x=%i, "
           "min_y=%i, max_y=%i)\n",
           camX, camY, cellWidth, min_x, max_x, min_y, max_y);
}

void drawCell(int x, int y) {
    DrawRectangleRec((Rectangle){.x = (float)x * cellWidth - (float)camX * cellWidth, .y = (float)y * cellWidth - (float)camY * cellWidth, .width = cellWidth, .height = cellWidth}, WHITE);
}

int main(void) {
    struct QTree *tree = newTree(0, 0, 64, NULL);
    //  x
    // xx
    //  xx
    insertCell(tree, 1, 0);
    insertCell(tree, 0, 1);
    insertCell(tree, 1, 1);
    insertCell(tree, 1, 2);
    insertCell(tree, 2, 2);

    insertCell(tree, 20, 20);
    insertCell(tree, 20, 21);
    insertCell(tree, 21, 20);
    insertCell(tree, 21, 21);

    // xxx
    // x x
    // x x
    //
    // x x
    // x x
    // xxx
    insertCell(tree, 50, 50);
    insertCell(tree, 51, 50);
    insertCell(tree, 52, 50);
    insertCell(tree, 50, 51);
    insertCell(tree, 52, 51);
    insertCell(tree, 50, 52);
    insertCell(tree, 52, 52);
    insertCell(tree, 50, 54);
    insertCell(tree, 52, 54);
    insertCell(tree, 50, 55);
    insertCell(tree, 52, 55);
    insertCell(tree, 50, 56);
    insertCell(tree, 51, 56);
    insertCell(tree, 52, 56);

    updateCam(tree);

    InitWindow(screenWidth, screenHeight, "Game of Life");
    SetTargetFPS(15);

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

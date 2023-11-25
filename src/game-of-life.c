#include <math.h>
#include <raylib.h>
#include <stdio.h>

#include <data-structure.h>

static int camX = 0;
static int camY = 0;
static int cellWidth = 10;

void updateCam(struct Field *field) {
    camX = field->xMin;
    camY = field->yMin;
}

void drawCell(struct Cell *cell) {
    if (cell == NULL) {
        return;
    }

    DrawRectangleRec((Rectangle){.x = cell->x * cellWidth - camX * cellWidth,
                                 .y = cell->y * cellWidth - camY * cellWidth,
                                 .width = cellWidth,
                                 .height = cellWidth},
                     WHITE);
}

int main(void) {
    struct Field *field = malloc(sizeof(struct Field));
    *field = (struct Field){.cells = NULL, .xMin = 0, .xMax = 0, .yMin = 0, .yMax = 0};

    insertToField(field, newCell(1, 1));
    insertToField(field, newCell(0, 1));
    insertToField(field, newCell(1, 0));
    insertToField(field, newCell(3, 0));
    insertToField(field, newCell(0, -3));

    removeFromField(field, 3, 0);
    removeFromField(field, 0, -3);

    printf("Field: xMin=%i xMax=%i yMin=%i yMax=%i\n", field->xMin, field->xMax, field->yMin, field->yMax);

    InitWindow(1000, 500, "Hello Raylib");
    SetTargetFPS(2);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        updateCam(field);

        if (isAlive(field, 0, -3)) {
            removeFromField(field, 0, -3);
        } else {
            insertToField(field, newCell(0, -3));
        }

        traverseFieldWithCallback(field, drawCell);

        EndDrawing();
    }

    // TODO leaking like hell down here. Gotta do sum about that. Wish I had some kind of language
    // that did this (at least semi-)automatically :,)

    freeField(field);

    return 0;
}

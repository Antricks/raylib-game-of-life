#include <math.h>
#include <raylib.h>
#include <stdio.h>

int main(void)
{
    InitWindow(1000, 500, "Hello Raylib");

    SetTargetFPS(60);

    int x = 0;

    while (!WindowShouldClose())
    {
        // this will be here until I actually start implementing GUI I guess. Have fun watching the funni sin rect :3
        x++;
        x = x%1000;
        BeginDrawing();
            ClearBackground(DARKGRAY);
            DrawRectangleRec((Rectangle){x,250+200*sin((double)x/31.4),32,32}, (Color){255,127,64,255});
            DrawText("Look funni sin rectangle :3", 300, 250, 32, LIGHTGRAY);
        EndDrawing();
    }

    return 0;
}

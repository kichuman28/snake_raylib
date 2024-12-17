#include <iostream>
#include <raylib.h>

using namespace std;

int main () {
    cout << "Startig the game..." << endl;

    InitWindow(750, 750, "Retro Snake");
    SetTargetFPS(60);

    while (WindowShouldClose() == false){
        BeginDrawing();


        EndDrawing();
    }

    CloseWindow();
    return 0;
}
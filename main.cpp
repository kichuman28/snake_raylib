#include <iostream>
#include <raylib.h>

using namespace std;

Color green = {173, 204, 93, 255};      //"Color" is an in-built struct in which the first 3 items are RGB and last one is intensity
Color darkGreen = {43, 51, 24, 255};


int cellSize = 30;      //This is basically dividing the screen into cells for placement of objects. Nothing is happening here, 
int cellCount = 25;     //this just indicates that there is 25 blocks of 30 size each. So 25*30 = 750, which is the size of our screen.

class Food{     //For the class Food, you need to know the x and y coordinate of the food. 
    public:     //So we can set that as the attribute in a in-built struct called Vector2 which stores the coordinates.
        Vector2 position = {5, 6}; //Here this stands for 5th row and 6th column.

        void Draw(){        //Now we need a function to draw the food on the canvas
            DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, darkGreen);   //So this is a function to create a rectangle. Needs x & y cooridnates as positions, height and width of the rectangle as well along with the color         
                            //We are multiplying it with cellSize to get it interms of the window, which is 750 in size. 
        }

};



int main () {
    cout << "Startig the game..." << endl;

    InitWindow(cellSize * cellCount, cellSize * cellCount, "Retro Snake");    //Starts the window
    SetTargetFPS(60);                       //Sets the frame rate

    Food food = Food(); //Creating an object to call the Draw function in food class.


    while (WindowShouldClose() == false){   //Loop to start the game. If pressed escape the loop stops.
        BeginDrawing();

        //Drawing
        ClearBackground(green);
        
        food.Draw();    //Calling the draw function.

        EndDrawing();
    }

    CloseWindow();                          //Ends the window
    return 0;
}
#include <iostream>
#include <raylib.h>
#include <deque>

using namespace std;
// ----------------------------------------------------------GLOBAL VARIABLES------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Color green = {173, 204, 93, 255};                                          //"Color" is an in-built struct in which the first 3 items are RGB and last one is intensity
Color darkGreen = {43, 51, 24, 255};


int cellSize = 30;                                                          //This is basically dividing the screen into cells for placement of objects. Nothing is happening here, 
int cellCount = 25;                                                         //this just indicates that there is 25 blocks of 30 size each. So 25*30 = 750, which is the size of our screen.

// ----------------------------------------------------------GLOBAL VARIABLES ENDS HERE------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------






// ----------------------------------------------------------SNAKE CLASS------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Snake{
    public:
        deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9},                //So I need a snake class for drawing the body of the snake. Using a deque as the body. Currently hardcoding 3 positions for the snake
         Vector2{4, 9}};

        void draw(){                                                        //A function which uses a for loop to traverse through the deque and mark all the positions as dark green.
            for(unsigned int i = 0; i < body.size(); i++){
                int x = body[i].x;
                int y = body[i].y;
                DrawRectangle(x * cellSize, y * cellSize, cellSize,
                 cellSize, darkGreen);  
            }                                                               
        }
};

// ----------------------------------------------------------SNAKE CLASS ENDS------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------






// ----------------------------------------------------------FOOD CLASS------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Food{                                                                 //For the class Food, you need to know the x and y coordinate of the food. 
    public:                                                                 //So we can set that as the attribute in a in-built struct called Vector2 which stores the coordinates.
        Vector2 position;                                                   //Here this stands for 5th row and 6th column.
        Texture2D texture;                                                  //So Texture2D is a data structure that allows us to convert the image to a type that can be rendered on screen in the most optimal way.

        Food(){                                                             //So this is like the init function(constructor), we want this called whenever an object is made. Here we want the image to be loaded each time the object is made.

            Image image = LoadImage("Graphics/food.png");
            texture = LoadTextureFromImage(image);                          //Loads the texture from the image.
            UnloadImage(image);                                             //Unloads the image to free some memory.
            position = GenerateRandomPos();

        }


        ~Food(){
            UnloadTexture(texture);                                         //This is for unloading the texture after the object is destroyed. This is called a destructor.
        }


        Vector2 GenerateRandomPos(){                                        //A random position generator function so that each time the game loads the food is at a different position.
            float x = GetRandomValue(0, cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);

            return Vector2{x, y};
        }


        void Draw(){                                                        //Now we need a function to draw the food on the canvas
            // DrawRectangle(position.x * cellSize,
            //  position.y * cellSize, cellSize, cellSize, darkGreen);      //So this is a function to create a rectangle. Needs x & y cooridnates as positions, height and width of the rectangle as well along with the color         
            //                                                              //We are multiplying it with cellSize to get it interms of the window, which is 750 in size. 
        
            DrawTexture(texture, position.x * cellSize,                     //So now to render the image we can use DrawTexture function.
            position.y * cellSize, GREEN);                                  //WHITE is more like an overlay
        }

};

// ----------------------------------------------------------FOOD CLASS ENDS HERE------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------








int main () {
    cout << "Startig the game..." << endl;

    InitWindow(cellSize * cellCount, cellSize * cellCount, "Retro Snake");   //Starts the window
    SetTargetFPS(60);                                                        //Sets the frame rate

    Food food = Food();                                                      //Creating an object to call the Draw function in food class.
    Snake snake = Snake();                                                   //Creating an object to call the Draw function in snake class.

    while (WindowShouldClose() == false){                                    //Loop to start the game. If pressed escape the loop stops.
        BeginDrawing();

                                                                             
        ClearBackground(green);                                              //This is for giving the window a new color
        
        food.Draw();                                                         //Calling the draw function for rendering the food.
        snake.draw();                                                        //Calling the draw function for showing the snake.

        EndDrawing();
    }

    CloseWindow();                                                           //Ends the window
    return 0;
}
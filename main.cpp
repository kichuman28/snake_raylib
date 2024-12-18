#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;
// ----------------------------------------------------------GLOBAL VARIABLES------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Color green = {173, 204, 93, 255};                                          //"Color" is an in-built struct in which the first 3 items are RGB and last one is intensity
Color darkGreen = {43, 51, 24, 255};


int cellSize = 30;                                                          //This is basically dividing the screen into cells for placement of objects. Nothing is happening here, 
int cellCount = 25;                                                         //this just indicates that there is 25 blocks of 30 size each. So 25*30 = 750, which is the size of our screen.
int offset = 75;



double lastUpdateTime = 0;                                                  //So this is for updating the position of the snake every 200 milliseconds

bool eventTriggered(double interval){                                       //For that we check how much time has passed by subtracting the last update time from the current time, if it is greater than the given interval, then we return true to update the position else we return false.
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool ElementInDeque(Vector2 element, deque<Vector2> deque){                 //Checks if the new position is somewhere along the snake's body. If yes it returns true and a new position is genrated. Else returns false
    for(unsigned int i = 0; i < deque.size(); i++){
        if(Vector2Equals(element, deque[i])){
            return true;
        }
    }
    return false;
}





class Snake{
    public:
        deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9},                //So I need a snake class for drawing the body of the snake. Using a deque as the body. Currently hardcoding 3 positions for the snake
         Vector2{4, 9}};
        Vector2 direction = {1, 0};                                         //Storing the direction to move right
        bool addSegment = false;                                            //A new attribute to check whether the snake has eaten a food or not


        void Draw(){                                                        //A function which uses a for loop to traverse through the deque and mark all the positions as dark green.
            for(unsigned int i = 0; i < body.size(); i++){
                float x = body[i].x;
                float y = body[i].y;
                // DrawRectangle(x * cellSize, y * cellSize, cellSize,      //I am using a rounded rectangle now
                //  cellSize, darkGreen);  

                Rectangle segment  = Rectangle{offset + x * (float)cellSize, //Added offset here to the x & y cooridates to account for the border
                 offset + y * (float)cellSize,(float)cellSize,              //For rounded rectangles you need to pass a rectangle structure as the argument. 
                 (float)cellSize};
                DrawRectangleRounded(segment, 0.5, 6, darkGreen);           //This in-built function needs the rectangle, the radius, segments(affects the smoothness of the corners) and color
            }                                                               
        }


        void Update(){                                                      //To move the snake, all we have to do is remove the last element and add a new element in the front of the dequeue.
            body.push_front(Vector2Add(body[0], direction));
            if(addSegment){                                                 //Just made an attribute which checks if a collision happened or not. If yes, then only an element is added &  the variable is set back to false. If not an element is added and removed to move the snake
                addSegment = false; 
            }else{
                body.pop_back();
            }
        }

        void Reset(){                                                       //Just resets the snake to the original position
            body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
            direction = {1, 0};             
        }
};



class Food{                                                                 //For the class Food, you need to know the x and y coordinate of the food. 
    public:                                                                 //So we can set that as the attribute in a in-built struct called Vector2 which stores the coordinates.
        Vector2 position;                                                   //Here this stands for 5th row and 6th column.
        Texture2D texture;                                                  //So Texture2D is a data structure that allows us to convert the image to a type that can be rendered on screen in the most optimal way.

        Food(deque<Vector2> snakeBody){                                                             //So this is like the init function(constructor), we want this called whenever an object is made. Here we want the image to be loaded each time the object is made.

            Image image = LoadImage("Graphics/food.png");
            texture = LoadTextureFromImage(image);                          //Loads the texture from the image.
            UnloadImage(image);                                             //Unloads the image to free some memory.
            position = GenerateRandomPos(snakeBody);

        }


        ~Food(){
            UnloadTexture(texture);                                         //This is for unloading the texture after the object is destroyed. This is called a destructor.
        }

        Vector2 GenerateRandomCell(){                                       //A helper function for generating random cells.
            float x = GetRandomValue(0, cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);

            return Vector2{x, y};
        }


        Vector2 GenerateRandomPos(deque<Vector2> snakeBody){                //A random position generator function so that each time the game loads the food is at a different position.

            Vector2 position = GenerateRandomCell();                        //Takes in the snake body as argument, passes it to the global bool function, generates new position if true or else continues with the same position. 
            if(ElementInDeque(position, snakeBody)){
                position = GenerateRandomCell();
            }

            return position;
        }


        void Draw(){                                                        //Now we need a function to draw the food on the canvas
            // DrawRectangle(position.x * cellSize,
            //  position.y * cellSize, cellSize, cellSize, darkGreen);      //So this is a function to create a rectangle. Needs x & y cooridnates as positions, height and width of the rectangle as well along with the color         
            //                                                              //We are multiplying it with cellSize to get it interms of the window, which is 750 in size. 
        
            DrawTexture(texture, offset + position.x * cellSize,            //So now to render the image we can use DrawTexture function.
            offset + position.y * cellSize, GREEN);                         //WHITE is more like an overlay. Also added offset here to accoont for the border
        }

};



class Game{                                                                 //Created a new Game class for better code management.
    public:
        Snake snake = Snake();
        Food food = Food(snake.body);
        bool running = true;                                                //If this variable is true then only the update function is called, i.e the snake starts moving.
        int score = 0;                                                      //A variable for keeping track of the score
        Sound eatSound;                                                     //Make two attributes for storing the sounds
        Sound wallSound;


        Game(){                                                             //In a constructor, load the audio device, store the sounds from the folders
            InitAudioDevice();
            eatSound = LoadSound("Sounds/eat.mp3");                         
            wallSound = LoadSound("Sounds/wall.mp3");
        }


        ~Game(){                                                            //In the destructor, unload the sounds and close the audio devices. 
            UnloadSound(eatSound);
            UnloadSound(wallSound);
            CloseAudioDevice();
        }


        void Draw(){                                                        //Used a common method called draw that calls the draw method in both the snake and food class.
            snake.Draw();
            food.Draw();
        }

        void Update(){
            if(running){
                snake.Update();
                CheckCollisionWithFood();                                   //So each time the snake's head's position updates we check is the head's position is equal to the food's position.
                CheckCollisionWithEdges();
                CheckCollisionWithTail();
            }                                                      
        }


        void CheckCollisionWithFood(){                                      //So this function is used to check if the snake's head's position matches the food's position. This is done for the snake to eat the food.
            if(Vector2Equals(snake.body[0], food.position)){
                food.position = food.GenerateRandomPos(snake.body);
                snake.addSegment = true;                                    //Each time a collision happens the segment is made true, only a cell is added. If it is false then a cell is added AND a cell is removed.
                score++;                                                    //Each time the snake collides with the food increment the score by one
                PlaySound(eatSound);
            }
        }


        void CheckCollisionWithEdges(){                                     //Incase the snake goes outside the box, we call the game over method
            if(snake.body[0].x == cellCount || snake.body[0].x == -1){
                GameOver();
            }else if(snake.body[0].y == cellCount || snake.body[0].y == -1){
                GameOver();
            }
        }

        void GameOver(){                                                    //In the game over method, we reset the snake's position back to the original position and provide a new position for the food.
            snake.Reset();
            food.position = food.GenerateRandomPos(snake.body);
            running = false;
            score = 0;                                                      //THe score becomes 0 each time the game restarts
            PlaySound(wallSound);
        }

        void CheckCollisionWithTail(){                                      //A function for checking if the snake hit it's own tail
            deque<Vector2> headlessBody = snake.body;                       //First we create copy
            headlessBody.pop_front();                                       //Then we remove the head because in the function it'll always return true if the head is there in the copy.
            if(ElementInDeque(snake.body[0], headlessBody)){                //Now we check if the body's head is present in the headless body. This will only become true it collides with each other.
                GameOver();                                     
            }
        }
};





int main () {
    cout << "Startig the game..." << endl;

    InitWindow(2*offset + cellSize * cellCount,                              //Increased the border by 2*offset to account for both the sides for both x and y axis
     2*offset + cellSize * cellCount, "Retro Snake");                        //Starts the window
    SetTargetFPS(60);                                                        //Sets the frame rate

    // Food food = Food();                                                   //Creating an object to call the Draw function in food class.
    // Snake snake = Snake();                                                //Creating an object to call the Draw function in snake class.

    Game game = Game();                                                      //Created an object of game class to call the functions


    while (WindowShouldClose() == false){                                    //Loop to start the game. If pressed escape the loop stops.
        BeginDrawing();


        if(eventTriggered(0.2)){                                             //Here we are checking if the interval has exceeded 200 milliseconds.
            game.Update();
        }
        // snake.update();                                                   //We first update the position, then we draw the snake & food.

        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){             //Adding key board movements here for each move. The && condition is there so that I can't move in the opposite direction, like 180 degrees  
            game.snake.direction = {0, -1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction = {0, 1};
            game.running = true;                                            //Incase the game stops when the user presses any of the direction keys the snake should start moving again. So just update the running variable to true so that the update function keeps on getting called.
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction = {1, 0};
            game.running = true;
        }


        ClearBackground(green);                                              //This is for giving the window a new color
        Rectangle border = {(float)offset - 5, (float)offset - 5,            //Added & subtracted some values here and there to properly render the border.
         (float)cellSize * cellCount + 10,
         (float)cellSize * cellCount + 10};
        DrawRectangleLinesEx(border, 5, darkGreen);                          //This line of code helps to draw a border for the snake's play area


        // food.Draw();                                                      //Calling the draw function for rendering the food.
        // snake.Draw();                                                     //Calling the draw function for showing the snake.

        DrawText("Retro Snake", offset - 5, 20, 40, darkGreen);              //This line of code helps to draw a text 
        
        DrawText(TextFormat("%i", game.score), offset - 5,                   //This line of code shows the score of the game
        offset + cellSize * cellCount + 10,40, darkGreen);
        
        game.Draw();                                                         //Common function which calls the Draw methods in both the snake and food class.

        EndDrawing();
    }

    CloseWindow();                                                           //Ends the window
    return 0;
}
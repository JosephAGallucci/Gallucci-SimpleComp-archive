#include <stdio.h>
#include "raylib.h"
#include "functions.h"

/*These need to be global variables because when placed into their own function, the values never leave past the 
defaults due to the application running in a loop and constantly calling the function, which in turn resets the values*/
bool mouseOnText = false; //PC input box check for if mouse is active on that box
bool inputActive = false; //PC input active
int cursorPlace = 0;
char input[2];
Rectangle InBoxPC = {545, 335, 200, 50};

int InputPC()
{
    int out = 0;
    
    if (!mouseOnText)
    {
        inputActive = false;
        cursorPlace = 0; //Value that causes cursor to reset
    }
    
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
    {
        inputActive = false;
        cursorPlace = 0;
    }

    if (mouseOnText && inputActive)
    {
        // Set the window's cursor to the I-Beam
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            if ((key >= 48) && (key <= 57))
            {
                input[cursorPlace] = (char)key;
                input[cursorPlace+1] = '\0'; // Add null terminator at the end of the string.
                cursorPlace++;
            }
            
            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            cursorPlace--;
            if (cursorPlace < 0) cursorPlace = 0;
            input[cursorPlace] = '\0';
        }
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    
    if (cursorPlace >= 2)
        inputActive = false; //Deactive input when the full string is input

    
    if (inputActive == false)
    {
        //Convert the text character PC into a numerical one
        if (input[1] < '0' && input[0] > '0')
        {
            input[1] = input[0];
            input[0] = '0';
        }
        
        out = int(input[0]) - '0';
        out = (out * 10) + int(input[1]) - '0';
    }
    
    if (out < 0) out = 0;
    
    if (inputActive == true)
    {
        DrawRectangle(545, 335, 200, 50, LIGHTGRAY);
        DrawText(input, 555, 345, 30, BLACK);
        DrawText("_", 545 + 8 + MeasureText(input, 40), 335 + 12, 30, MAROON); //545 and 335 are the X and Y positions of the input box
    }
    
    return out;
}

void MemClear(int array[], int amount)
{
    for (int i=0;i<amount;i++)
        array[i] = 0;
}

void Cards(int cards1[], int cards2[], int amount, int cardIndex, int &PC)
{
    int yPos = 40; //Initial y position
    
    DrawText("Cards", 825, 15, 20, BLACK);
    
    if (IsKeyPressed(KEY_DELETE) || IsKeyPressed(KEY_BACKSPACE))
    {
        MemClear(cards1, 10);
        MemClear(cards2, 10);
    }
    
    //Draw card values
    for (int i=0;i<amount;i++)
    {
        if (i == cardIndex)
            DrawText(TextFormat("%03i", cards1[i]), 810, yPos, 20, RED);
        else
            DrawText(TextFormat("%03i", cards1[i]), 810, yPos, 20, BLACK);
        DrawText(TextFormat("%03i", cards2[i]), 865, yPos, 20, BLACK);
        yPos += 20;
    }
    yPos = 40; //Reset Y pos
    
    DrawText("In", 810, 250, 20, BLACK);
    //The input card box drawing is handled in the toggleCard function
    
    DrawText("Out", 865, 250, 20, BLACK);
    DrawRectangleLines(860, 35, 45, 210, BLACK);
    
    DrawText("Load Input Cards", 805, 285, 20, BLACK);
    DrawText("Save Output Cards", 805, 335, 20, BLACK);
    
    //Trigger PC input, needed to be in seperate place to avoid errors where the PC box would retain its contents after input
    if (CheckCollisionPointRec(GetMousePosition(), InBoxPC)) mouseOnText = true;
    else mouseOnText = false;
    
    if (mouseOnText)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouseOnText)
        {
            switch (inputActive)
            {
                case false:
                    inputActive = true;
                    cursorPlace = 0;
                    for (int i=0;i<2;i++) //Clear the input
                        input[i] = 0;
                break;
                    
                case true:
                    inputActive = false;
                break;
            }
        }
        
        if (inputActive == true)
            PC = InputPC();
        else
            DrawText("Click to enter", 555, 345, 20, BLACK);   
    }
    
    if (mouseOnText)
        DrawRectangleLines(545, 335, 200, 50, RED);
    else
        DrawRectangleLines(545, 335, 200, 50, BLACK);
}

int NavPC(int out)
{
    int newOut = out; //Maintain last PC value
    
    if (IsKeyPressed(KEY_UP))
        newOut--;
    
    if (IsKeyPressed(KEY_DOWN))
        newOut++;
    
    if (IsKeyPressed(KEY_RIGHT))
        newOut += 20;
    
    if (IsKeyPressed(KEY_LEFT))
        newOut -= 20;
    
    //Caps
    if (newOut > 99) newOut = 0;
    if (newOut < 0) newOut = 99;
    
    return newOut;
}

void autoButton(bool &active)
{
    Rectangle autoBox = {35, 455, 195, 40};
    DrawText("Autorun Program", 45, 465, 20, BLACK);
    
    if (CheckCollisionPointRec(GetMousePosition(), autoBox))
    {
        if (active == false)
            DrawRectangleLinesEx(autoBox, 1, RED);
        else
            DrawRectangleRec(autoBox, RED);
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && active == false)
            active = true; //Start autorun
    }
    else
        DrawRectangleLines(35, 455, 195, 40, BLACK);
}

void stepButton(bool &active)
{
    Rectangle autoBox = {235, 455, 140, 40};
    DrawText("Single Step", 245, 465, 20, BLACK);
    
    if (CheckCollisionPointRec(GetMousePosition(), autoBox))
    {
        DrawRectangleLinesEx(autoBox, 1, RED);
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            active = true;
    }
    else
        DrawRectangleLinesEx(autoBox, 1, BLACK);
}

int navInCards(int out)
{
    int newOut = out; //Maintain last value
    
    if (IsKeyPressed(KEY_UP))
        newOut--;
    
    if (IsKeyPressed(KEY_DOWN))
        newOut++;
    
    if (newOut > 9) newOut = 0;
    if (newOut < 0) newOut = 9;
    
    return newOut;
}
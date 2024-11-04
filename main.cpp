/*

SIMPLE COMPUTER PROJECT

*/

#include <stdio.h>
#include "raylib.h"
#include "functions.h"

int main()
{
    bool isProgramRunning = false;
    bool autoRun = false;
    bool inMainScreen = false;
    bool instrucSetActive = false;
    int readIndex = 0; //Used for reading from input cards
    int outputIndex = 0;
    
    //These positions are used for drawing the memory contents
    int textXPos = 65;
    int textYPos = 40;
    
    InitWindow(1024,512,"Simple Computer");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        if (inMainScreen == true)
        {      
            stepButton(isProgramRunning);
            
            if (autoRun == true) isProgramRunning = true;
            
            if (isProgramRunning == true)
                InterpretInstruction(isProgramRunning, readIndex, outputIndex, autoRun);
            
            if (IsKeyPressed(KEY_DELETE))
            {
                readIndex = 0; //Return input card pointer to top of list
                outputIndex = 0; //And do the same for the output card pointer
            }
            
            autoButton(autoRun);
            
            if (isProgramRunning == true && !autoRun == true)
                isProgramRunning = false;
        }
        else
        {
            if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            {
                inMainScreen = true;
                instrucSetActive = false;
            }
        }
        
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        if (inMainScreen == true)
        {
            DrawText("Accumulator", 545, 10, 20, BLACK);
            DrawText("IR (Current Instruction)", 545, 180, 20, BLACK);
            DrawText("PC (Active Memory Slot)", 545, 315, 20, BLACK);
            DrawText("Memory Slots", 35, 10, 20, BLACK);
            
            toggleCard(readIndex); //This needs to go up front for text layering purposes
            
            DrawRegs(isProgramRunning, readIndex);
            DrawMem(textXPos, textYPos, readIndex);
            
            instructionSetScreen(instrucSetActive, inMainScreen);
            helpScreen(inMainScreen);
            
            DrawRectangleLines(545, 35, 200, 50, BLACK);
            DrawRectangleLines(545, 200, 200, 50, BLACK);
        }
        else
        {
            if (instrucSetActive == false)
            {
                DrawText("SIMPLE COMPUTER", 315, 50, 40, BLACK);
                DrawText("KEYBOARD CONTROLS", 395, 105, 20, BLACK);
                DrawText("[ENTER] - Confirm input", 340, 128, 20, BLACK);
                DrawText("Arrow Keys - Navigate PC (PC value can also be typed in)/Navigate incards", 140, 175, 20, BLACK);
                DrawRectangleLines(185, 210+20, 700, 50, BLACK);
                DrawText("The large box contains the programs, and the tall box contains the data used for the program", 25, 205, 20, BLACK);
                DrawText("There is a central input box for both memory input and card input", 195, 225+20, 20, BLACK);
                DrawText("Click on a large box to enable memory input, and click on a tall box to enable card input", 55, 270+20, 20, BLACK);
                DrawText("Press Enter while inputting values to enter the current contents of the input box", 55, 290+20, 20, BLACK);
                DrawText("[DELETE] - Clear all memory", 340, 152, 20, BLACK);
                DrawText("Programs and incards can be loaded from files;", 260, 310+20, 20, BLACK);
                DrawText("programs save to program.txt, and incards save to incards.txt", 200, 330+20, 20, BLACK);
                DrawText("Press enter to go to main program", 340, 400+20, 20, BLACK);
            }
            else
            {
                //Drawing the instruction set screen
                ClearBackground(RAYWHITE);
                DrawText("INSTRUCTION SET", 392, 20, 30, BLACK);
                DrawText("0xx - Load the value from the current input card to slot xx. Move input card pointer", 20, 20+30, 20, BLACK);
                DrawText("1xx - Load the value from slot xx to an output card. Move output card pointer", 20, 40+30, 20, BLACK);
                DrawText("2xx - Add the value from slot xx to the accumulator", 20, 60+30, 20, BLACK);
                DrawText("3xx - Subtract the value from slot xx to the accumulator", 20, 80+30, 20, BLACK);
                DrawText("4xx - Load the value from slot xx into the accumulator", 20, 100+30, 20, BLACK);
                DrawText("5xx - Store the value from slot xx into the accumulator", 20, 120+30, 20, BLACK);
                DrawText("6xx - Move the current PC value to slot 99 and jump to slot xx", 20, 140+30, 20, BLACK);
                DrawText("7xx - Checks if the accumulator is negative. If it is, jump the PC to cell xx", 20, 160+30, 20, BLACK);
  
                DrawText("9xx - Returns PC to slot xx and ends the program, xx should be the slot the program starts on", 20, 180+30, 20, BLACK);
                DrawText("Press enter to return to main screen", 20, 280, 20, BLACK);
            }
        }
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}

bool CheckAutoRun(int IR)
{
    return (IR / 100 != 9); //End program on the 9 instruction
}

void instructionSetScreen(bool &active, bool &mainScreen)
{   
    //Process the button to enter this screen
    Rectangle inputBox = {545, 120, 230, 40};
    DrawRectangleLinesEx(inputBox, 1, BLACK);
    DrawText("View Instruction Set", 555, 130, 20, BLACK);
    
    if (CheckCollisionPointRec(GetMousePosition(), inputBox))
    {
        DrawRectangleLinesEx(inputBox, 1, RED);
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && active == false)
        {
            active = true;
            mainScreen = false;
        }
    }
    else
       DrawRectangleLinesEx(inputBox, 1, BLACK);
}

void helpScreen(bool &mainScreen)
{   
    //Process the button to enter this screen
    Rectangle inputBox = {935, 90, 80, 40};
    DrawRectangleLinesEx(inputBox, 1, BLACK);
    DrawText("Help", 945, 100, 20, BLACK);
    
    if (CheckCollisionPointRec(GetMousePosition(), inputBox))
    {
        DrawRectangleLinesEx(inputBox, 1, RED);
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            mainScreen = false;
        }
    }
    else
       DrawRectangleLinesEx(inputBox, 1, BLACK);
}
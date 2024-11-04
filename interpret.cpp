#include <stdio.h>
#include "raylib.h"
#include "functions.h"

int memorySlots[100] = {0};
int inputCards[10] = {0};
int outputCards[10];
int accum, PC, IR;

Rectangle InBox = {545, 335, 200, 50};
char memInput[3];
int cursor = 0;
//These are for inputting the memory values into the input box
bool isActive = false;
bool memActive = true; //Can the memory be manipulated?
bool cardActive = false; //Can the input cards be manipulated?

void InterpretInstruction(bool running, int &readIndex, int &outputIndex, bool &autoRun)
{
    IR = memorySlots[PC];
    int currentMemSlot = IR % 100;
    int command = IR / 100;
    
    if (autoRun == true)
        autoRun = CheckAutoRun(IR); //Determine end of autorun sequence
    
    switch (command)
    {
        case 0: //INP
            memorySlots[currentMemSlot] = inputCards[readIndex];
            
            if (readIndex < 9)
                readIndex ++;
            else
                readIndex = 0;
        break;
        
        case 1: //OUT
            outputCards[outputIndex] = memorySlots[currentMemSlot];
            WriteOutCard(outputCards, outputIndex);
            outputIndex ++;
        break;
        
        case 2: //ADD
            accum = accum + memorySlots[currentMemSlot];
        break;
        
        case 3: //SUB
            accum = accum - memorySlots[currentMemSlot];
        break;
        
        case 4: //LDA
            accum = memorySlots[currentMemSlot];
        break;
        
        case 5: //STA
           memorySlots[currentMemSlot] = accum; 
        break;
        
        case 6: //JMP
            memorySlots[99] = PC;
            PC = (currentMemSlot - 1); //Compensate for the ++ that happens at the end of the loop for the PC
        break;
        
        case 7: //TAC
            if (accum < 0)
                PC = (currentMemSlot - 1); //Compensate for the ++ that happens at the end of the loop for the PC
        break;
        
        case 9: //HLT
            running = false;
            PC = (currentMemSlot - 1); //See previous two instructions
        break;
    }   
    
    if (PC <= 99)
        PC ++;
    else
        PC = 0;
}

void DrawRegs(bool running, int readIndex)
{
    int memSlotContents = memorySlots[PC] % 100; //Use to gather the data from the specified memory slot
    if (!CheckCollisionPointRec(GetMousePosition(), InBox))
    {
        if (memActive == true)
            PC = NavPC(PC);
        DrawText(TextFormat("%02i", PC), 555, 345, 30, BLACK);
    }
    
    Cards(inputCards, outputCards, 10, readIndex, PC); //Draws card boxes and values
    
    DrawText(TextFormat("%03i", accum), 555, 45, 30, BLACK);
    //Draw current accumulator operation
    switch (memorySlots[PC] / 100) //Get instruction, IR is delayed when it comes to drawing on the screen
    {
        case 2: //Add
            DrawText(TextFormat("%03i + %03i", accum, memorySlots[memSlotContents]), 615, 45, 20, DARKGRAY);
        break;
        
        case 3: //Subtract
            DrawText(TextFormat("%03i - %03i", accum, memorySlots[memSlotContents]), 615, 45, 20, DARKGRAY);
        break;
    }
    
    DrawText(TextFormat("%03i", IR), 555, 215, 30, BLACK);
    
    //Input/output card file IO
    LoadCards(inputCards);
    SaveCards(outputCards);  
}

void DrawMem(int &x, int &y, int &readIndex)
{
    //Collision boxes
    Rectangle programLoad = {795, 375, 160, 40};
    Rectangle inputMem = {545, 430, 200, 50};
    Rectangle programSave = {795, 425, 160, 40};
    
    //Memory input section
    bool onBox = false;
    
    if (CheckCollisionPointRec(GetMousePosition(), inputMem)) onBox = true;
    else onBox = false;
    
    if (onBox)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && onBox)
        {
            switch (isActive)
            {
                case false:
                    isActive = true;
                    cursor = 0;
                    for (int i=0;i<3;i++) //Clear the input
                        memInput[i] = 0;
                break;
                    
                case true:
                    isActive = false;
                break;
            }
        }
        if (isActive == true)
        {
            if (memActive == true)
                memorySlots[PC] = InputMem(onBox);
            
            if (cardActive == true)
                inputCards[readIndex] = InputMem(onBox);
        }
        else
            DrawText("Click to input", 555, 440, 20, BLACK);
    }
    
    if (memActive == true)
        DrawText("Program Input:", 385, 450, 20, BLACK);
    
    if (cardActive == true)
        DrawText("    Card Input:", 385, 450, 20, BLACK);

    if (onBox)
        DrawRectangleLinesEx(inputMem, 1, RED);
    else
        DrawRectangleLinesEx(inputMem, 1, BLACK);
    
    //Draw memory values
    for (int i=0;i<100;i++)
    {
        if (i == PC)
           DrawText(TextFormat("%03i", memorySlots[i]), x, y, 20, RED); 
        else
           DrawText(TextFormat("%03i", memorySlots[i]), x, y, 20, BLACK);
        y += 20;
        
        if (y > 420)
        {
            x += 100;
            y = 40;
        }
    }
    
    x = 65; //Reset the x pos for the next time goes the draw sequence...
    y = 40; //...and the same goes for the y position
    
    if (IsKeyPressed(KEY_DELETE))
        MemClear(memorySlots, 100);
    
    //Program load box
    if (CheckCollisionPointRec(GetMousePosition(), programLoad))
        DrawRectangleLines(795, 375, 160, 40, RED);    
    else 
        DrawRectangleLines(795, 375, 160, 40, BLACK);
    
    //Program save box
    if (CheckCollisionPointRec(GetMousePosition(), programSave))
        DrawRectangleLines(795, 425, 160, 40, RED);    
    else 
        DrawRectangleLines(795, 425, 160, 40, BLACK);
    
    DrawText("Load Program", 805, 385, 20, BLACK);
    
    DrawText("Save Program", 805, 435, 20, BLACK);
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), programLoad))
        ReadProgram(memorySlots, PC); //Load program
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), programSave))
        WriteProgram(memorySlots); //Save program
}

int InputMem(bool onBox)
{ 
    int output = 0;
    
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
    {
        isActive = false;
        cursor = 0;
    }
    
    if (onBox && isActive)
    {
        int key = GetCharPressed();
        
        while (key > 0)
        {
            if ((key >= 48) && (key <= 57))
            {
                memInput[cursor] = (char)key;
                memInput[cursor+1] = '\0'; // Add null terminator at the end of the string.
                cursor++;
            }
            
            key = GetCharPressed();  // Check next character in the queue            
        }
        
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            cursor--;
            if (cursor < 0) cursor = 0;
            memInput[cursor] = '\0';
        }
    }
    
    if (cursor > 2)     
        isActive = false;
    
    if (isActive == false)
    {
        //Single digit input
        if (!(memInput[2] > '0') && !(memInput[1] > '0') && memInput[0] > '0')
        {
            memInput[2] = memInput[0];
            memInput[1] = '0';
            memInput[0] = '0';
        }
        
        //Double digit
        if (!(memInput[2] >= '0') && memInput[1] > '0' && memInput[0] > '0')
        {
            memInput[2] = memInput[1];
            memInput[1] = memInput[0];
            memInput[0] = '0';
        }
        //Converts the text character input to a numerical value
        output = int(memInput[0]) - '0';
        output = (output * 10) + int(memInput[1]) - '0';
        output = (output * 10) + int(memInput[2]) - '0';
    }
    
    if (isActive == true)
    {
        DrawRectangle(545, 430, 200, 50, LIGHTGRAY);
        DrawText(memInput, 555, 440, 30, BLACK);
        // Draw underscore char
        DrawText("_", 545 + 8 + MeasureText(memInput, 40), 430 + 12, 30, MAROON); //545 and 430 are the X and Y positions of the input box
    }
    
    if (output < 0)
        output = 0;   

    return output;
}

void toggleCard(int &readIndex)
{
    Rectangle memBox = {35, 35, 500, 410};
    Rectangle cardBox = {805, 35, 45, 210};
    
    //Switch between memory input and card input
    if (memActive == true)
    {
        if (CheckCollisionPointRec(GetMousePosition(), cardBox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            memActive = false;
            cardActive = true;
        }
        
        //Highlight box when mouse hovers over it
        if (CheckCollisionPointRec(GetMousePosition(), cardBox) && cardActive == false)
            DrawRectangleLinesEx(cardBox, 1, RED);
        else 
           DrawRectangleLinesEx(cardBox, 1, BLACK); 
        
        DrawRectangle(35, 35, 500, 410, LIGHTGRAY);
        DrawRectangleLines(35, 35, 500, 410, RED);   
    }
    
    if (cardActive == true)
    {
        if (CheckCollisionPointRec(GetMousePosition(), memBox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            memActive = true;
            cardActive = false;
            readIndex = 0; //Reset card index to the top
        }
        
        //Highlight box when mouse hovers over it
        if (CheckCollisionPointRec(GetMousePosition(), memBox) && memActive == false)
            DrawRectangleLines(35, 35, 500, 410, RED);
        else
            DrawRectangleLines(35, 35, 500, 410, BLACK);
        
        readIndex = navInCards(readIndex);
        DrawRectangle(805, 35, 45, 210, LIGHTGRAY);
        DrawRectangleLinesEx(cardBox, 1, RED); 
    }
}
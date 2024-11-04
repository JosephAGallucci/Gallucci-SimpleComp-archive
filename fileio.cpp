#include <stdio.h>
#include "raylib.h"
#include "functions.h"
#include <string>

FILE *infile;

void ReadInCard(int array[], int amount)
{
    //Call in entered filename for file
    infile = fopen("./incards.txt", "r");
    
    for (int i=0;i<amount;i++)
    {
        fscanf(infile, "%d", &array[i]);
    }
    
    fclose(infile);
}

void ReadProgram(int array[], int &value) //Amount is the current PC value
{
    int currentInstruct = 0; //The number is large so that it can store programs of any size
    int startingPC = 0;
    infile = fopen("./program.txt", "r");
    
    //Get the PC slot where program should start
    for (int i=0;i<99;i++) //Run through every slot in memory
    {
        fscanf(infile, "%d", &currentInstruct);
        
        if (currentInstruct / 100 == 9)
        {
            startingPC = currentInstruct % 100;
            break; //We've finished reading the program, so exit the loop
        }
    }
    
    fclose(infile); //We've read through the file once, so we have to close it...
    infile = fopen("./program.txt", "r"); //...only to open it again, in order to start back at the top of the file
    
    for (int i=startingPC;i<99;i++) //Run through every slot in memory
    {
        fscanf(infile, "%d", &array[i]);
    }
    
    fclose(infile);    
    
    value = startingPC;
}

void WriteOutCard(int array[], int value)
{
    int counter = 0;
    infile = fopen("./outcards.txt", "w");
    
    do
    {
        fprintf(infile, "%d", array[counter]);
        fprintf(infile, "\n");
        counter ++;
    }
    while (array[counter] != 0 || counter > 10);
    
    fclose(infile);
}

void WriteProgram(int array[])
{
    infile = fopen("./program.txt", "w");
    
    for (int i=0;i<99;i++)
    {
        if (array[i] != 0) //Read only valid program data and not zeroes
        {
            fprintf(infile, "%d", array[i]);
            fprintf(infile, "\n");
            
            if (array[i] / 100 == 9) break; //Exit loop once end of program is found
        }
    }
    
    fclose(infile);
}

void LoadCards(int cards[])
{
    Rectangle loadBox = {795, 275, 200, 40}; //Collision box
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), loadBox))
        ReadInCard(cards, 10);
    
    if (CheckCollisionPointRec(GetMousePosition(), loadBox) && !IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        DrawRectangleLines(795, 275, 200, 40, RED);
    else
        DrawRectangleLines(795, 275, 200, 40, BLACK); 
}

void SaveCards(int cards[])
{
    Rectangle loadBox = {795, 325, 215, 40}; //Collision box
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), loadBox))
        WriteOutCard(cards, 10);
    
    if (CheckCollisionPointRec(GetMousePosition(), loadBox) && !IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        DrawRectangleLines(795, 325, 215, 40, RED);
    else
        DrawRectangleLines(795, 325, 215, 40, BLACK); 
}
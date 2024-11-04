void InterpretInstruction(bool running, int &readIndex, int &outputIndex, bool &autoRun); //Outside draw
void DrawRegs(bool running, int readIndex); //I mean this one's a given where it falls
void DrawMem(int &x, int &y, int &readIndex); //Same here
int InputPC();
void MemClear(int array[], int amount);
void Cards(int cards1[], int cards2[], int amount, int cardIndex, int &PC); //Draws card values to the screen
int NavPC(int out);
void ReadInCard(int array[], int amount);
void ReadProgram(int array[], int &value);
void WriteOutCard(int array[], int value);
void LoadCards(int cards[]);
void SaveCards(int cards[]);
int InputMem(bool onBox); //Also used to input card values
bool CheckAutoRun(int IR); //This function checks if the autorun is still happening...
void autoButton(bool &active); //...while this function initiates the autorun and draws the box for the icon
void WriteProgram(int array[]);
void toggleCard(int &readIndex); //Used to switch between inputting the program and inputting card values
int navInCards(int out);
void stepButton(bool &active); //Steps through the program one instruction at a time
void instructionSetScreen(bool &active, bool &mainScreen);
void helpScreen(bool &mainScreen);
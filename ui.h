#include <iostream>
using namespace std;

extern const char* RESET;
extern const char* BOLD;
extern const char* RED;
extern const char* GREEN;
extern const char* YELLOW;
extern const char* BLUE;
extern const char* MAGENTA;
extern const char* CYAN;
extern const char* WHITE;

void clearScreen();
void printLine(int width = 70, char ch = '=');
void printHeader(const char* title);
void printSection(const char* title, int width = 55);
void printPrompt(const char* text);
void printSuccess(const char* text);
void printError(const char* text);
void printWarning(const char* text);
void printInfo(const char* text);
void printMenuOption(int num, const char* label, const char* color);
void pauseScreen();
#include "ui.h"

const char* RESET   = "\033[0m";
const char* BOLD    = "\033[1m";
const char* RED     = "\033[31m";
const char* GREEN   = "\033[32m";
const char* YELLOW  = "\033[33m";
const char* BLUE    = "\033[34m";
const char* MAGENTA = "\033[35m";
const char* CYAN    = "\033[36m";
const char* WHITE   = "\033[37m";

void clearScreen()
{
    cout << "\033[2J\033[1;1H";
}

void printLine(int width, char ch)
{
    for (int i = 0; i < width; i++)
    {
        cout << ch;
    }
    cout << endl;
}

void printHeader(const char* title)
{
    clearScreen();
    cout << CYAN;
    printLine(70, '=');
    cout << RESET;
    cout << BOLD << BLUE << "  " << title << RESET << endl;
    cout << CYAN;
    printLine(70, '=');
    cout << RESET << endl;
}

void printSection(const char* title, int width)
{
    cout << CYAN;
    printLine(width, '-');
    cout << RESET;
    cout << BOLD << MAGENTA << "  " << title << RESET << endl;
    cout << CYAN;
    printLine(width, '-');
    cout << RESET;
}

void printPrompt(const char* text)
{
    cout << BOLD << YELLOW << text << RESET;
}

void printSuccess(const char* text)
{
    cout << BOLD << GREEN << text << RESET << endl;
}

void printError(const char* text)
{
    cout << BOLD << RED << text << RESET << endl;
}

void printWarning(const char* text)
{
    cout << BOLD << YELLOW << text << RESET << endl;
}

void printInfo(const char* text)
{
    cout << BOLD << WHITE << text << RESET << endl;
}

void printMenuOption(int num, const char* label, const char* color)
{
    cout << color << "  [" << num << "] " << RESET << label << endl;
}

void pauseScreen()
{
    cout << endl;
    cout << BOLD << WHITE << "Press Enter to continue..." << RESET;
    cin.get();
}
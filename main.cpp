#include <iostream>
#include "customer.h"
#include "employee.h"
#include "admin.h"

using namespace std;

const char* RESET  = "\033[0m";
const char* BOLD   = "\033[1m";
const char* RED    = "\033[31m";
const char* GREEN  = "\033[32m";
const char* YELLOW = "\033[33m";
const char* BLUE   = "\033[34m";
const char* MAGENTA= "\033[35m";
const char* CYAN   = "\033[36m";
const char* WHITE  = "\033[37m";

void showWelcomeScreen()
{
    cout << endl;
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;

    cout << BOLD << BLUE;
    cout << "                    WELCOME TO SECURESHOP" << endl;
    cout << RESET;

    cout << YELLOW;
    cout << "             A Simulated Secure Shopping Platform" << endl;
    cout << RESET;

    cout << CYAN;
    displayLine(70, '=');
    cout << RESET << endl;
}

void showRoleMenu()
{
    cout << BOLD << WHITE << "Please select your login type:" << RESET << endl << endl;

    cout << GREEN   << "  [1] ADMIN"    << RESET << "    - Full system control" << endl;
    cout << BLUE    << "  [2] CUSTOMER" << RESET << " - Browse products and shop" << endl;
    cout << MAGENTA << "  [3] EMPLOYEE" << RESET << " - Manage inventory and support" << endl;

    cout << endl;
    cout << BOLD << YELLOW << "Enter your choice: " << RESET;
}

void showExitMessage()
{
    cout << endl;
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;

    cout << BOLD << GREEN;
    cout << "              THANK YOU FOR USING SECURESHOP!" << endl;
    cout << RESET;

    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;
}

int main()
{
    showWelcomeScreen();
    showRoleMenu();

    int a = getValidInt(1, 3);

    cout << endl;

    if (a == 1)
    {
        cout << BOLD << GREEN << "Opening Admin Module..." << RESET << endl;
        amenu();
    }
    else if (a == 2)
    {
        cout << BOLD << BLUE << "Opening Customer Module..." << RESET << endl;
        cmenu();
    }
    else if (a == 3)
    {
        cout << BOLD << MAGENTA << "Opening Employee Module..." << RESET << endl;
        emenu();
    }

    showExitMessage();

    return 0;
}
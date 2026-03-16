#include "customer.h"
#include "employee.h"
#include "admin.h"

int main()
{
    printHeader("WELCOME TO SECURESHOP");
    printInfo("A Simulated Secure Shopping Platform");
    cout << endl;

    printInfo("Please select your login type:");
    cout << endl;
    printMenuOption(1, "ADMIN    - Full system control", GREEN);
    printMenuOption(2, "CUSTOMER - Browse products and shop", BLUE);
    printMenuOption(3, "EMPLOYEE - Manage inventory and support", MAGENTA);
    cout << endl;
    printPrompt("Enter your choice: ");

    int a = getValidInt(1, 3);

    cout << endl;

    if (a == 1)
    {
        printSuccess("Opening Admin Module...");
        amenu();
    }
    else if (a == 2)
    {
        printSuccess("Opening Customer Module...");
        cmenu();
    }
    else
    {
        printSuccess("Opening Employee Module...");
        emenu();
    }

    cout << endl;
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;
    cout << BOLD << GREEN << "  THANK YOU FOR USING SECURESHOP!" << RESET << endl;
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;

    return 0;
}
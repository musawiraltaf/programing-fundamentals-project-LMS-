#include "employee.h"

static bool parseEmployeeRecord(ifstream &read, char *name, char *password)
{
    if (!read.getline(name, PASS_LEN, '|'))
    {
        return false;
    }
    if (!read.getline(password, PASS_LEN, '|'))
    {
        return false;
    }
    read.ignore(1000, '\n');
    return true;
}

void emenu()
{
    printHeader("SECURESHOP - EMPLOYEE PORTAL");

    while (true)
    {
        printInfo("Choose an option:");
        cout << endl;
        printMenuOption(1, "Register New Employee", GREEN);
        printMenuOption(2, "Login", BLUE);
        printMenuOption(3, "Back", RED);
        cout << endl;
        printPrompt("Enter your choice: ");

        int choice = getValidInt(1, 3);

        if (choice == 1)
        {
            eregistration();
            pauseScreen();
            printHeader("SECURESHOP - EMPLOYEE PORTAL");
        }
        else if (choice == 2)
        {
            if (everify())
            {
                cout << endl;
                cout << GREEN;
                displayLine(70, '=');
                cout << RESET;
                cout << BOLD << GREEN << "  LOGIN SUCCESSFUL - Welcome Employee!" << RESET << endl;
                cout << GREEN;
                displayLine(70, '=');
                cout << RESET;

                logActivity("Employee login successful");

                while (true)
                {
                    cout << endl;
                    printSection("EMPLOYEE MENU", 50);
                    printMenuOption(1, "Manage Feedback", GREEN);
                    printMenuOption(2, "Handle Support Requests", BLUE);
                    printMenuOption(3, "Catalogue Management", YELLOW);
                    printMenuOption(4, "View Sales Record", MAGENTA);
                    printMenuOption(5, "Low Stock Alert", CYAN);
                    printMenuOption(6, "Send Announcement", WHITE);
                    printMenuOption(7, "Exit Menu", RED);
                    cout << endl;
                    printPrompt("Enter your choice: ");

                    int mchoice = getValidInt(1, 7);

                    if (mchoice == 1)
                    {
                        mfeedback();
                        pauseScreen();
                    }
                    else if (mchoice == 2)
                    {
                        msupport();
                        pauseScreen();
                    }
                    else if (mchoice == 3)
                    {
                        catalogueManage();
                    }
                    else if (mchoice == 4)
                    {
                        sales();
                        pauseScreen();
                    }
                    else if (mchoice == 5)
                    {
                        stock();
                        pauseScreen();
                    }
                    else if (mchoice == 6)
                    {
                        sendAnnouncement();
                        logActivity("Employee sent announcement");
                        pauseScreen();
                    }
                    else
                    {
                        printWarning("Exiting employee menu.");
                        logActivity("Employee logged out");
                        currentEmployeeName[0] = '\0';
                        break;
                    }
                }

                printHeader("SECURESHOP - EMPLOYEE PORTAL");
            }
            else
            {
                pauseScreen();
                printHeader("SECURESHOP - EMPLOYEE PORTAL");
            }
        }
        else
        {
            break;
        }
    }
}

bool everify()
{
    bool authenticated = false;
    int attempts = 0;

    while (attempts < 3 && !authenticated)
    {
        if (attempts > 0)
        {
            printError("Wrong credentials! Please try again.");
            logActivity("Employee login failed attempt");
        }

        char *cname = new char[PASS_LEN];
        char *cpassword = new char[PASS_LEN];

        getNonEmptyLine("Enter Your Name: ", cname, PASS_LEN);
        getNonEmptyLine("Enter Your Password: ", cpassword, PASS_LEN);

        encryptPassword(cpassword);

        ifstream read("employee.txt");
        if (!read)
        {
            printError("Error: Could not open employee.txt!");
            delete[] cname;
            delete[] cpassword;
            return false;
        }

        char *fname = new char[PASS_LEN];
        char *fpassword = new char[PASS_LEN];

        while (parseEmployeeRecord(read, fname, fpassword))
        {
            if (compareStrings(cname, fname) && compareStrings(cpassword, fpassword))
            {
                authenticated = true;
                copyString(currentEmployeeName, cname);
                break;
            }
        }

        read.close();
        delete[] cname;
        delete[] cpassword;
        delete[] fname;
        delete[] fpassword;

        attempts++;
    }

    if (!authenticated)
    {
        printError("WARNING: 3 failed login attempts!");
        logActivity("Employee multiple failed login attempts - SECURITY ALERT");
    }

    return authenticated;
}

void eregistration()
{
    char *rname = new char[PASS_LEN];
    char *rpassword = new char[PASS_LEN];

    getNonEmptyLine("Enter your Name: ", rname, PASS_LEN);

    if (usernameExists("employee.txt", rname))
    {
        printError("Employee username already exists.");
        delete[] rname;
        delete[] rpassword;
        return;
    }

    getNonEmptyLine("Enter your Password: ", rpassword, PASS_LEN);
    encryptPassword(rpassword);

    ofstream write("employee.txt", ios::app);
    if (!write.is_open())
    {
        printError("Error: Could not open employee.txt!");
        delete[] rname;
        delete[] rpassword;
        return;
    }

    write << rname << "|" << rpassword << "|" << endl;
    write.close();

    printSuccess("Employee registered successfully!");
    logActivity("New employee registered");

    delete[] rname;
    delete[] rpassword;
}

void mfeedback()
{
    char *fid = new char[ID_LEN];
    char *fname = new char[NAME_LEN];
    char *ftext = new char[TEXT_LEN];

    fstream display("feedback.txt", ios::in);
    if (!display.is_open())
    {
        printWarning("No feedback found or file could not be opened.");
        delete[] fid;
        delete[] fname;
        delete[] ftext;
        return;
    }

    cout << endl;
    printSection("CUSTOMER FEEDBACK", 70);
    cout << left << setw(15) << "Feedback ID" << setw(25) << "Customer" << "Feedback" << endl;
    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    bool hasData = false;

    while (display.getline(fid, ID_LEN, '|'))
    {
        display.getline(fname, NAME_LEN, '|');
        display.getline(ftext, TEXT_LEN, '\n');

        cout << left << setw(15) << fid << setw(25) << fname << endl;
        cout << "  -> " << ftext << endl << endl;
        hasData = true;
    }

    display.close();

    if (!hasData)
    {
        printWarning("No feedback entries available.");
        delete[] fid;
        delete[] fname;
        delete[] ftext;
        return;
    }

    char *replyID = new char[ID_LEN];
    char *response = new char[TEXT_LEN];

    getNonEmptyLine("Enter Feedback ID to reply to: ", replyID, ID_LEN);
    getNonEmptyLine("Enter your response: ", response, TEXT_LEN);

    fstream display1("feedback.txt", ios::in);
    fstream tempwrite("temp.txt", ios::out);

    while (display1.getline(fid, ID_LEN, '|'))
    {
        display1.getline(fname, NAME_LEN, '|');
        display1.getline(ftext, TEXT_LEN, '\n');

        if (compareStrings(replyID, fid))
        {
            tempwrite << fid << "|" << fname << "|" << ftext << " [REPLY: " << response << "]" << endl;
        }
        else
        {
            tempwrite << fid << "|" << fname << "|" << ftext << endl;
        }
    }

    tempwrite.close();
    display1.close();
    remove("feedback.txt");
    rename("temp.txt", "feedback.txt");

    printSuccess("Reply saved successfully!");
    logActivity("Employee replied to customer feedback");

    delete[] fid;
    delete[] fname;
    delete[] ftext;
    delete[] replyID;
    delete[] response;
}

void msupport()
{
    char *Sid = new char[ID_LEN];
    char *Sname = new char[NAME_LEN];
    char *Stext = new char[TEXT_LEN];

    fstream display("Support.txt", ios::in);
    if (!display.is_open())
    {
        printWarning("No support requests found or file could not be opened.");
        delete[] Sid;
        delete[] Sname;
        delete[] Stext;
        return;
    }

    cout << endl;
    printSection("SUPPORT REQUESTS", 70);
    cout << left << setw(15) << "Support ID" << setw(25) << "Customer" << "Request" << endl;
    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    bool hasData = false;

    while (display.getline(Sid, ID_LEN, '|'))
    {
        display.getline(Sname, NAME_LEN, '|');
        display.getline(Stext, TEXT_LEN, '\n');

        cout << left << setw(15) << Sid << setw(25) << Sname << endl;
        cout << "  -> " << Stext << endl << endl;
        hasData = true;
    }

    display.close();

    if (!hasData)
    {
        printWarning("No support entries available.");
        delete[] Sid;
        delete[] Sname;
        delete[] Stext;
        return;
    }

    char *replyID = new char[ID_LEN];
    char *response = new char[TEXT_LEN];

    getNonEmptyLine("Enter Support ID to reply to: ", replyID, ID_LEN);
    getNonEmptyLine("Enter your response: ", response, TEXT_LEN);

    fstream display1("Support.txt", ios::in);
    fstream tempwrite("temp.txt", ios::out);

    while (display1.getline(Sid, ID_LEN, '|'))
    {
        display1.getline(Sname, NAME_LEN, '|');
        display1.getline(Stext, TEXT_LEN, '\n');

        if (compareStrings(replyID, Sid))
        {
            tempwrite << Sid << "|" << Sname << "|" << Stext << " [RESOLVED: " << response << "]" << endl;
        }
        else
        {
            tempwrite << Sid << "|" << Sname << "|" << Stext << endl;
        }
    }

    tempwrite.close();
    display1.close();
    remove("Support.txt");
    rename("temp.txt", "Support.txt");

    printSuccess("Response saved successfully!");
    logActivity("Employee responded to support request");

    delete[] Sid;
    delete[] Sname;
    delete[] Stext;
    delete[] replyID;
    delete[] response;
}

void sales()
{
    char *readname = new char[NAME_LEN];
    char *readproduct = new char[NAME_LEN];
    int readprice;
    char *readp_ID = new char[ID_LEN];
    int readquantity;
    int totalRevenue = 0;

    fstream read("orderhistory.txt", ios::in);

    cout << endl;
    printSection("SALES RECORD", 65);
    cout << left << setw(20) << "Customer" << setw(20) << "Product"
         << "$" << setw(10) << "Price" << setw(10) << "Qty"
         << setw(10) << "ID" << endl;
    cout << CYAN;
    displayLine(65, '-');
    cout << RESET;

    bool hasData = false;

    if (read.is_open())
    {
        while (read.getline(readname, NAME_LEN, '|'))
        {
            read.getline(readproduct, NAME_LEN, '|');
            read >> readprice;
            read.ignore(1, '|');
            read.getline(readp_ID, ID_LEN, '|');
            read >> readquantity;
            read.ignore(1000, '\n');

            cout << left << setw(20) << readname << setw(20) << readproduct
                 << "$" << setw(10) << readprice << setw(10) << readquantity
                 << setw(10) << readp_ID << endl;

            totalRevenue += (readprice * readquantity);
            hasData = true;
        }
        read.close();
    }

    if (!hasData)
    {
        printWarning("Could not find sales data.");
    }

    cout << CYAN;
    displayLine(65, '-');
    cout << RESET;
    cout << BOLD << GREEN << "Total Revenue: $" << totalRevenue << RESET << endl;
    cout << CYAN;
    displayLine(65, '=');
    cout << RESET;

    delete[] readname;
    delete[] readproduct;
    delete[] readp_ID;
}

void stock()
{
    char *price = new char[NAME_LEN];
    char *p_ID = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int quantity;

    cout << endl;
    cout << YELLOW;
    displayLine(70, '!');
    cout << RESET;
    cout << BOLD << RED << "  LOW STOCK ALERT - Restocking Needed!" << RESET << endl;
    cout << YELLOW;
    displayLine(70, '!');
    cout << RESET;

    cout << left << setw(25) << "Product" << "$" << setw(10) << "Price"
         << setw(20) << "Category" << setw(10) << "ID"
         << setw(10) << "Stock" << endl;
    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    fstream read("catalogue.txt", ios::in);
    bool found = false;

    if (read.is_open())
    {
        while (read.getline(price, NAME_LEN, '|'))
        {
            read.getline(p_ID, ID_LEN, '|');
            read.getline(name, NAME_LEN, '|');
            read.getline(category, NAME_LEN, '|');
            read >> quantity;
            read.ignore(1000, '\n');

            if (quantity <= 5)
            {
                cout << left << setw(25) << name << "$" << setw(10) << price
                     << setw(20) << category << setw(10) << p_ID
                     << setw(10) << quantity << BOLD << RED << " LOW" << RESET << endl;
                found = true;
            }
        }
        read.close();
    }

    if (!found)
    {
        printSuccess("All products have sufficient stock.");
    }

    cout << YELLOW;
    displayLine(70, '!');
    cout << RESET;

    delete[] price;
    delete[] p_ID;
    delete[] name;
    delete[] category;
}
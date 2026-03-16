#include <cstdlib>
#include "admin.h"

static bool parseUserRecord(ifstream &read, char *name, char *password)
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

void amenu()
{
    printHeader("SECURESHOP - ADMIN PORTAL");

    if (!averify())
    {
        printError("Admin authentication failed.");
        pauseScreen();
        return;
    }

    srand(time(0));
    int otp = 100000 + (rand() % 900000);

    fstream gen("otp.txt", ios::out);
    gen << otp;
    gen.close();

    cout << endl;
    cout << YELLOW;
    displayLine(40, '*');
    cout << RESET;
    cout << BOLD << YELLOW << "  Generated OTP: " << otp << RESET << endl;
    cout << WHITE << "  (This OTP is being shown only for simulation)" << RESET << endl;
    cout << YELLOW;
    displayLine(40, '*');
    cout << RESET << endl;

    printPrompt("Enter OTP: ");
    int userotp;
    cin >> userotp;
    cin.ignore(1000, '\n');

    bool otpValid = false;
    fstream readOtp("otp.txt", ios::in);
    int storedOtp;

    if (readOtp.is_open())
    {
        readOtp >> storedOtp;
        if (storedOtp == userotp)
        {
            otpValid = true;
        }
        readOtp.close();
    }

    if (!otpValid)
    {
        printError("Incorrect OTP! Access DENIED.");
        logActivity("Admin login failed - wrong OTP (2FA failed)");
        logAudit("SECURITY: Admin 2FA failed - unauthorized access attempt");
        pauseScreen();
        return;
    }

    cout << endl;
    cout << GREEN;
    displayLine(70, '=');
    cout << RESET;
    cout << BOLD << GREEN << "  LOGIN + 2FA SUCCESSFUL - Welcome Admin!" << RESET << endl;
    cout << GREEN;
    displayLine(70, '=');
    cout << RESET << endl;

    logActivity("Admin login successful (2FA verified)");
    logAudit("Admin login - 2FA authentication passed");

    while (true)
    {
        cout << endl;
        printSection("ADMIN CONTROL PANEL", 55);
        printMenuOption(1, "Manage Users (Add/Delete)", GREEN);
        printMenuOption(2, "Inventory and Bulk Operations", BLUE);
        printMenuOption(3, "Discounts and Promotions", YELLOW);
        printMenuOption(4, "Activity Logs and Security Monitoring", CYAN);
        printMenuOption(5, "Sales and Revenue Analysis", MAGENTA);
        printMenuOption(6, "View Audit Trail", WHITE);
        printMenuOption(7, "Send Announcement", GREEN);
        printMenuOption(8, "Exit Menu", RED);
        cout << endl;
        printPrompt("Enter your choice: ");

        int achoice = getValidInt(1, 8);

        if (achoice == 1)
        {
            mnguser();
        }
        else if (achoice == 2)
        {
            while (true)
            {
                cout << endl;
                printSection("INVENTORY & BULK OPERATIONS", 50);
                printMenuOption(1, "Manage Inventory (Add/Edit/Delete Products)", GREEN);
                printMenuOption(2, "Bulk Import & Export", MAGENTA);
                printMenuOption(3, "Back", YELLOW);
                cout << endl;
                printPrompt("Enter your choice: ");

                int adchoice = getValidInt(1, 3);

                if (adchoice == 1)
                {
                    catalogueManage();
                }
                else if (adchoice == 2)
                {
                    mngbulk();
                }
                else
                {
                    break;
                }
            }
        }
        else if (achoice == 3)
        {
            mngdiscs();
            pauseScreen();
        }
        else if (achoice == 4)
        {
            viewActivityLogs();
            pauseScreen();
        }
        else if (achoice == 5)
        {
            revenue();
            pauseScreen();
        }
        else if (achoice == 6)
        {
            viewAuditTrail();
            pauseScreen();
        }
        else if (achoice == 7)
        {
            sendAnnouncement();
            logAudit("Admin sent announcement");
            pauseScreen();
        }
        else
        {
            printWarning("Exiting admin panel.");
            logActivity("Admin logged out");
            logAudit("Admin logged out");
            currentAdminName[0] = '\0';
            break;
        }
    }
}

bool averify()
{
    bool authenticated = false;
    int attempts = 0;

    while (attempts < 3 && !authenticated)
    {
        if (attempts > 0)
        {
            printError("Invalid credentials! Please try again.");
            logActivity("Admin login failed attempt");
        }

        char *aname = new char[PASS_LEN];
        char *apassword = new char[PASS_LEN];

        getNonEmptyLine("Enter Admin Name: ", aname, PASS_LEN);
        getNonEmptyLine("Enter Admin Password: ", apassword, PASS_LEN);

        encryptPassword(apassword);

        ifstream read("admin.txt");
        if (!read)
        {
            printError("Error: Could not open admin.txt!");
            delete[] aname;
            delete[] apassword;
            return false;
        }

        char *fname = new char[PASS_LEN];
        char *fpassword = new char[PASS_LEN];

        while (parseUserRecord(read, fname, fpassword))
        {
            if (compareStrings(aname, fname) && compareStrings(apassword, fpassword))
            {
                authenticated = true;
                copyString(currentAdminName, aname);
                break;
            }
        }

        read.close();

        delete[] aname;
        delete[] apassword;
        delete[] fname;
        delete[] fpassword;

        attempts++;
    }

    if (!authenticated)
    {
        printError("SECURITY WARNING: 3 invalid attempts. Access blocked.");
        logActivity("Admin multiple failed login attempts - HIGH SECURITY ALERT");
    }

    return authenticated;
}

void addcustomer()
{
    char *fname = new char[PASS_LEN];
    char *fpassword = new char[PASS_LEN];

    getNonEmptyLine("Enter Customer Name: ", fname, PASS_LEN);

    if (usernameExists("customer.txt", fname))
    {
        printError("Customer username already exists.");
        delete[] fname;
        delete[] fpassword;
        return;
    }

    getNonEmptyLine("Enter Customer Password: ", fpassword, PASS_LEN);
    encryptPassword(fpassword);

    ofstream outfile("customer.txt", ios::app);
    if (!outfile.is_open())
    {
        printError("Error: Could not open customer.txt!");
        delete[] fname;
        delete[] fpassword;
        return;
    }

    outfile << fname << "|" << fpassword << "|" << endl;
    outfile.close();

    printSuccess("Customer added successfully!");
    logAudit("Admin added new customer");
    logActivity("Admin added new customer");

    delete[] fname;
    delete[] fpassword;
}

void addemployee()
{
    char *fname = new char[PASS_LEN];
    char *fpassword = new char[PASS_LEN];

    getNonEmptyLine("Enter Employee Name: ", fname, PASS_LEN);

    if (usernameExists("employee.txt", fname))
    {
        printError("Employee username already exists.");
        delete[] fname;
        delete[] fpassword;
        return;
    }

    getNonEmptyLine("Enter Employee Password: ", fpassword, PASS_LEN);
    encryptPassword(fpassword);

    ofstream outfile("employee.txt", ios::app);
    if (!outfile.is_open())
    {
        printError("Error: Could not open employee.txt!");
        delete[] fname;
        delete[] fpassword;
        return;
    }

    outfile << fname << "|" << fpassword << "|" << endl;
    outfile.close();

    printSuccess("Employee added successfully!");
    logAudit("Admin added new employee");
    logActivity("Admin added new employee");

    delete[] fname;
    delete[] fpassword;
}

void deletecustomer()
{
    char *target = new char[PASS_LEN];
    getNonEmptyLine("Enter the Customer Name to delete: ", target, PASS_LEN);

    ifstream infile("customer.txt");
    ofstream outfile("temp.txt");

    if (!infile.is_open() || !outfile.is_open())
    {
        printError("Error: Could not open files!");
        delete[] target;
        return;
    }

    char *name = new char[PASS_LEN];
    char *password = new char[PASS_LEN];
    bool found = false;

    while (parseUserRecord(infile, name, password))
    {
        if (compareStrings(name, target))
        {
            found = true;
            continue;
        }

        outfile << name << "|" << password << "|" << endl;
    }

    infile.close();
    outfile.close();

    if (found)
    {
        remove("customer.txt");
        rename("temp.txt", "customer.txt");
        printSuccess("Customer deleted successfully!");
        logAudit("Admin deleted customer");
        logActivity("Admin deleted customer");
    }
    else
    {
        remove("temp.txt");
        printError("Customer not found.");
    }

    delete[] target;
    delete[] name;
    delete[] password;
}

void deleteemployee()
{
    char *target = new char[PASS_LEN];
    getNonEmptyLine("Enter the Employee Name to delete: ", target, PASS_LEN);

    ifstream infile("employee.txt");
    ofstream outfile("temp.txt");

    if (!infile.is_open() || !outfile.is_open())
    {
        printError("Error: Could not open files!");
        delete[] target;
        return;
    }

    char *name = new char[PASS_LEN];
    char *password = new char[PASS_LEN];
    bool found = false;

    while (parseUserRecord(infile, name, password))
    {
        if (compareStrings(name, target))
        {
            found = true;
            continue;
        }

        outfile << name << "|" << password << "|" << endl;
    }

    infile.close();
    outfile.close();

    if (found)
    {
        remove("employee.txt");
        rename("temp.txt", "employee.txt");
        printSuccess("Employee deleted successfully!");
        logAudit("Admin deleted employee");
        logActivity("Admin deleted employee");
    }
    else
    {
        remove("temp.txt");
        printError("Employee not found.");
    }

    delete[] target;
    delete[] name;
    delete[] password;
}

void mnguser()
{
    while (true)
    {
        cout << endl;
        printSection("USER MANAGEMENT", 50);
        printMenuOption(1, "Add New Customer", GREEN);
        printMenuOption(2, "Add New Employee", BLUE);
        printMenuOption(3, "Delete Customer", YELLOW);
        printMenuOption(4, "Delete Employee", RED);
        printMenuOption(5, "Back", WHITE);
        cout << endl;
        printPrompt("Enter your choice: ");

        int mng = getValidInt(1, 5);

        if (mng == 1)
        {
            addcustomer();
            pauseScreen();
        }
        else if (mng == 2)
        {
            addemployee();
            pauseScreen();
        }
        else if (mng == 3)
        {
            deletecustomer();
            pauseScreen();
        }
        else if (mng == 4)
        {
            deleteemployee();
            pauseScreen();
        }
        else
        {
            break;
        }
    }
}

void exportextra()
{
    char *price = new char[NAME_LEN];
    char *p_ID = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int quantity;

    fstream read("catalogue.txt", ios::in);
    if (!read.is_open())
    {
        printError("Error: Could not open catalogue.txt!");
        delete[] price;
        delete[] p_ID;
        delete[] name;
        delete[] category;
        return;
    }

    cout << endl;
    printSection("PRODUCTS WITH EXTRA STOCK", 70);
    cout << left << setw(25) << "Product" << "$" << setw(10) << "Price"
         << setw(20) << "Category" << setw(10) << "ID"
         << setw(10) << "Stock" << endl;
    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    bool hasAny = false;

    while (read.getline(price, NAME_LEN, '|'))
    {
        read.getline(p_ID, ID_LEN, '|');
        read.getline(name, NAME_LEN, '|');
        read.getline(category, NAME_LEN, '|');
        read >> quantity;
        read.ignore(1, '\n');

        if (quantity >= 10)
        {
            cout << left << setw(25) << name << "$" << setw(10) << price
                 << setw(20) << category << setw(10) << p_ID
                 << setw(10) << quantity << endl;
            hasAny = true;
        }
    }
    read.close();

    if (!hasAny)
    {
        printWarning("No products with extra stock found.");
        cout << CYAN;
        displayLine(70, '-');
        cout << RESET;
        delete[] price;
        delete[] p_ID;
        delete[] name;
        delete[] category;
        return;
    }

    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    printInfo("Do you want to export extra items?");
    printMenuOption(1, "Yes", GREEN);
    printMenuOption(2, "No", RED);
    printPrompt("Enter your choice: ");

    int mng = getValidInt(1, 2);

    if (mng == 1)
    {
        fstream write("extra.txt", ios::out);
        if (!write.is_open())
        {
            printError("Error: Could not open extra.txt!");
            delete[] price;
            delete[] p_ID;
            delete[] name;
            delete[] category;
            return;
        }

        read.open("catalogue.txt", ios::in);
        if (!read.is_open())
        {
            printError("Error: Could not reopen catalogue.txt!");
            write.close();
            delete[] price;
            delete[] p_ID;
            delete[] name;
            delete[] category;
            return;
        }

        while (read.getline(price, NAME_LEN, '|'))
        {
            read.getline(p_ID, ID_LEN, '|');
            read.getline(name, NAME_LEN, '|');
            read.getline(category, NAME_LEN, '|');
            read >> quantity;
            read.ignore(1, '\n');

            if (quantity >= 10)
            {
                write << price << "|" << p_ID << "|" << name << "|" << category << "|" << quantity << endl;
            }
        }

        read.close();
        write.close();
        printSuccess("Extra items exported successfully!");
        logAudit("Admin exported extra stock items");
    }
    else
    {
        printWarning("Export cancelled.");
    }

    delete[] price;
    delete[] p_ID;
    delete[] name;
    delete[] category;
}

void importStock()
{
    char *price = new char[NAME_LEN];
    char *p_ID = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int quantity;

    fstream read("catalogue.txt", ios::in);
    if (!read.is_open())
    {
        printError("Error: Could not open catalogue.txt!");
        delete[] price;
        delete[] p_ID;
        delete[] name;
        delete[] category;
        return;
    }

    cout << endl;
    printSection("LOW STOCK PRODUCTS", 70);
    cout << left << setw(25) << "Product" << "$" << setw(10) << "Price"
         << setw(20) << "Category" << setw(10) << "ID"
         << setw(10) << "Stock" << endl;
    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    bool hasAny = false;

    while (read.getline(price, NAME_LEN, '|'))
    {
        read.getline(p_ID, ID_LEN, '|');
        read.getline(name, NAME_LEN, '|');
        read.getline(category, NAME_LEN, '|');
        read >> quantity;
        read.ignore(1, '\n');

        if (quantity <= 5)
        {
            cout << left << setw(25) << name << "$" << setw(10) << price
                 << setw(20) << category << setw(10) << p_ID
                 << setw(10) << quantity << BOLD << RED << " LOW" << RESET << endl;
            hasAny = true;
        }
    }
    read.close();

    if (!hasAny)
    {
        printSuccess("No low stock items found.");
        cout << CYAN;
        displayLine(70, '-');
        cout << RESET;
        delete[] price;
        delete[] p_ID;
        delete[] name;
        delete[] category;
        return;
    }

    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    printInfo("Do you want to add 10 pieces to these items?");
    printMenuOption(1, "Yes", GREEN);
    printMenuOption(2, "No", RED);
    printPrompt("Enter your choice: ");

    int mng = getValidInt(1, 2);

    if (mng == 1)
    {
        fstream readFile("catalogue.txt", ios::in);
        fstream writeFile("temp.txt", ios::out);

        if (!readFile.is_open() || !writeFile.is_open())
        {
            printError("Error: Could not open files!");
            delete[] price;
            delete[] p_ID;
            delete[] name;
            delete[] category;
            return;
        }

        while (readFile.getline(price, NAME_LEN, '|'))
        {
            readFile.getline(p_ID, ID_LEN, '|');
            readFile.getline(name, NAME_LEN, '|');
            readFile.getline(category, NAME_LEN, '|');
            readFile >> quantity;
            readFile.ignore(1, '\n');

            if (quantity <= 5)
            {
                writeFile << price << "|" << p_ID << "|" << name << "|" << category << "|" << (quantity + 10) << endl;
            }
            else
            {
                writeFile << price << "|" << p_ID << "|" << name << "|" << category << "|" << quantity << endl;
            }
        }

        readFile.close();
        writeFile.close();
        remove("catalogue.txt");
        rename("temp.txt", "catalogue.txt");

        printSuccess("10 pieces have been added to low stock items!");
        logAudit("Admin restocked low inventory items (+10 each)");
    }
    else
    {
        printWarning("Restocking cancelled.");
    }

    delete[] price;
    delete[] p_ID;
    delete[] name;
    delete[] category;
}

void mngbulk()
{
    while (true)
    {
        cout << endl;
        printSection("BULK OPERATIONS", 40);
        printMenuOption(1, "Export Extra Stock", GREEN);
        printMenuOption(2, "Import Low Stock (Restock)", BLUE);
        printMenuOption(3, "Back", YELLOW);
        cout << endl;
        printPrompt("Enter your choice: ");

        int mng = getValidInt(1, 3);

        if (mng == 1)
        {
            exportextra();
            pauseScreen();
        }
        else if (mng == 2)
        {
            importStock();
            pauseScreen();
        }
        else
        {
            break;
        }
    }
}

void mngdiscs()
{
    cout << endl;
    printSection("DISCOUNT MANAGEMENT", 40);

    char *disc = new char[NAME_LEN];
    getNonEmptyLine("Enter Discount Code: ", disc, NAME_LEN);

    printPrompt("Enter Discount Percentage (1-100): ");
    int percentage = getValidInt(1, 100);

    fstream write("discount.txt", ios::app);
    if (write.is_open())
    {
        write << disc << "|" << percentage << endl;
        write.close();
        printSuccess("Discount code added successfully!");
        logAudit("Admin created new discount code");
    }
    else
    {
        printError("Error: Could not open discount.txt!");
    }

    delete[] disc;
}

void revenue()
{
    char *readname = new char[NAME_LEN];
    char *readproduct = new char[NAME_LEN];
    int readprice;
    char *readp_ID = new char[ID_LEN];
    int readquantity;

    int ttl = 0;

    fstream read("orderhistory.txt", ios::in);

    cout << endl;
    printSection("SALES & REVENUE ANALYSIS", 70);
    cout << left << setw(20) << "Customer" << setw(20) << "Product"
         << "$" << setw(10) << "Price" << setw(10) << "Qty"
         << setw(10) << "ID" << endl;
    cout << CYAN;
    displayLine(70, '-');
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

            ttl += (readprice * readquantity);

            cout << left << setw(20) << readname << setw(20) << readproduct
                 << "$" << setw(10) << readprice << setw(10) << readquantity
                 << setw(10) << readp_ID << endl;
            hasData = true;
        }
        read.close();
    }

    if (!hasData)
    {
        printWarning("No sales record found.");
    }

    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;
    cout << BOLD << GREEN << "TOTAL REVENUE: $" << ttl << RESET << endl;
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;

    logAudit("Admin viewed revenue report");

    delete[] readname;
    delete[] readproduct;
    delete[] readp_ID;
}

void viewAuditTrail()
{
    char *line = new char[TEXT_LEN];

    fstream read("audit.txt", ios::in);

    cout << endl;
    printSection("AUDIT TRAIL", 70);

    if (read.is_open())
    {
        bool hasRecords = false;
        while (read.getline(line, TEXT_LEN))
        {
            cout << WHITE << "  " << line << RESET << endl;
            hasRecords = true;
        }

        if (!hasRecords)
        {
            printWarning("No audit record found.");
        }
        read.close();
    }
    else
    {
        printWarning("Audit file has not been created yet.");
    }

    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;
    logAudit("Admin viewed audit trail");

    delete[] line;
}

void viewActivityLogs()
{
    char *line = new char[TEXT_LEN];

    fstream read("activity.txt", ios::in);

    cout << endl;
    printSection("ACTIVITY LOGS - SECURITY MONITORING", 70);

    if (read.is_open())
    {
        bool hasLogs = false;
        while (read.getline(line, TEXT_LEN))
        {
            cout << WHITE << "  " << line << RESET << endl;
            hasLogs = true;
        }

        if (!hasLogs)
        {
            printWarning("No activity logs found.");
        }
        read.close();
    }
    else
    {
        printWarning("Activity log file not found.");
    }

    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;
    logAudit("Admin reviewed activity logs");

    delete[] line;
}
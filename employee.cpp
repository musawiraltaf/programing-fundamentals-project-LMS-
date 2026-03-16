#include "employee.h"

static const char* RESET   = "\033[0m";
static const char* BOLD    = "\033[1m";
static const char* RED     = "\033[31m";
static const char* GREEN   = "\033[32m";
static const char* YELLOW  = "\033[33m";
static const char* BLUE    = "\033[34m";
static const char* MAGENTA = "\033[35m";
static const char* CYAN    = "\033[36m";
static const char* WHITE   = "\033[37m";

void emenu()
{
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;
    cout << BOLD << MAGENTA << "                 SECURESHOP - EMPLOYEE PORTAL" << RESET << endl;
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET << endl;

    cout << BOLD << WHITE << "Choose an option:" << RESET << endl;
    cout << GREEN << "1. Register New Employee" << RESET << endl;
    cout << BLUE << "2. Login" << RESET << endl;
    cout << BOLD << YELLOW << "Enter your choice: " << RESET;

    int choice = getValidInt(1, 2);

    if (choice == 1)
    {
        eregistration();
    }
    if (choice == 2)
    {
        if (everify())
        {
            cout << endl;
            cout << GREEN;
            displayLine(70, '=');
            cout << RESET;
            cout << BOLD << GREEN << "             LOGIN SUCCESSFUL - Welcome Employee!" << RESET << endl;
            cout << GREEN;
            displayLine(70, '=');
            cout << RESET;

            logActivity("Employee login successful");

            while (true)
            {
                cout << endl;
                cout << CYAN;
                displayLine(50, '-');
                cout << RESET;
                cout << BOLD << BLUE << "                 EMPLOYEE MENU" << RESET << endl;
                cout << CYAN;
                displayLine(50, '-');
                cout << RESET;

                cout << GREEN   << "1. Manage Feedback" << RESET << endl;
                cout << BLUE    << "2. Handle Support Requests" << RESET << endl;
                cout << YELLOW  << "3. Catalogue Management" << RESET << endl;
                cout << MAGENTA << "4. View Sales Record" << RESET << endl;
                cout << CYAN    << "5. Low Stock Alert" << RESET << endl;
                cout << WHITE   << "6. Send Announcement" << RESET << endl;
                cout << RED     << "7. Exit Menu" << RESET << endl;
                cout << BOLD << YELLOW << "Enter your choice: " << RESET;

                int mchoice = getValidInt(1, 7);

                if (mchoice == 1)
                {
                    mfeedback();
                }
                else if (mchoice == 2)
                {
                    msupport();
                }
                else if (mchoice == 3)
                {
                    catalogueManage();
                }
                else if (mchoice == 4)
                {
                    sales();
                }
                else if (mchoice == 5)
                {
                    stock();
                }
                else if (mchoice == 6)
                {
                    sendAnnouncement();
                    logActivity("Employee sent announcement");
                }
                else if (mchoice == 7)
                {
                    cout << BOLD << RED << "Exiting employee menu." << RESET << endl;
                    logActivity("Employee logged out");
                    break;
                }
            }
        }
    }
}

bool everify()
{
    bool authenticated = false;
    int attempts = 0;

    do
    {
        if (attempts > 0)
        {
            cout << BOLD << RED << "Wrong credentials! Please try again." << RESET << endl;
            logActivity("Employee login failed attempt");
        }

        char *cname = new char[PASS_LEN];
        char *cpassword = new char[PASS_LEN];

        cout << BOLD << CYAN << "Enter Your Name: " << RESET;
        cin.getline(cname, PASS_LEN);
        cout << BOLD << CYAN << "Enter Your Password: " << RESET;
        cin.getline(cpassword, PASS_LEN);

        encryptPassword(cpassword);

        ifstream read("employee.txt");
        if (!read)
        {
            cout << BOLD << RED << "Error: Could not open employee.txt!" << RESET << endl;
            delete[] cname;
            delete[] cpassword;
            return false;
        }

        char *fname = new char[PASS_LEN];
        char *fpassword = new char[PASS_LEN];

        while (read.getline(fname, PASS_LEN, '|'))
        {
            read.getline(fpassword, PASS_LEN, '|');

            if (compareStrings(cname, fname) && compareStrings(cpassword, fpassword))
            {
                authenticated = true;
                break;
            }
        }
        read.close();

        delete[] cname;
        delete[] cpassword;
        delete[] fname;
        delete[] fpassword;

        attempts++;

        if (!authenticated && attempts >= 3)
        {
            cout << BOLD << RED << "WARNING: " << attempts << " failed login attempts!" << RESET << endl;
            logActivity("Employee multiple failed login attempts - SECURITY ALERT");
        }

    } while (!authenticated);

    return authenticated;
}

void eregistration()
{
    char *rname = new char[PASS_LEN];
    char *rpassword = new char[PASS_LEN];

    cout << BOLD << CYAN << "Enter your Name: " << RESET;
    cin.getline(rname, PASS_LEN, '\n');
    cout << BOLD << CYAN << "Enter your Password: " << RESET;
    cin.getline(rpassword, PASS_LEN, '\n');

    encryptPassword(rpassword);

    ofstream write("employee.txt", ios::app);
    if (!write.is_open())
    {
        cout << BOLD << RED << "Error: Could not open employee.txt!" << RESET << endl;
        delete[] rname;
        delete[] rpassword;
        return;
    }

    write << rname << "|" << rpassword << "|" << endl;
    cout << BOLD << GREEN << rname << " Registered Successfully!" << RESET << endl;
    write.close();

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
        cout << BOLD << RED << "No feedback found or file could not be opened." << RESET << endl;
        delete[] fid;
        delete[] fname;
        delete[] ftext;
        return;
    }

    cout << endl;
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;
    cout << BOLD << GREEN << "                 CUSTOMER FEEDBACK" << RESET << endl;
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;

    cout << left << setw(15) << "Feedback ID" << setw(25) << "Customer" << "Feedback" << endl;
    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    while (display.getline(fid, ID_LEN, '|'))
    {
        display.getline(fname, NAME_LEN, '|');
        display.getline(ftext, TEXT_LEN, '\n');
        cout << left << setw(15) << fid << setw(25) << fname << endl;
        cout << "  -> " << ftext << endl << endl;
    }
    display.close();

    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    char *replyID = new char[ID_LEN];
    cout << BOLD << CYAN << "Enter Feedback ID to reply to: " << RESET;
    cin.getline(replyID, ID_LEN, '\n');

    char *response = new char[TEXT_LEN];
    cout << BOLD << CYAN << "Enter your response: " << RESET << endl;
    cin.getline(response, TEXT_LEN, '\n');

    fstream display1("feedback.txt", ios::in);
    fstream tempwrite("temp.txt", ios::out);

    if (!display1.is_open())
    {
        cout << BOLD << RED << "Error: Could not open feedback.txt!" << RESET << endl;
        delete[] fid;
        delete[] fname;
        delete[] ftext;
        delete[] replyID;
        delete[] response;
        return;
    }

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

    cout << BOLD << GREEN << "Reply saved successfully!" << RESET << endl;
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
        cout << BOLD << RED << "No support requests found or file could not be opened." << RESET << endl;
        delete[] Sid;
        delete[] Sname;
        delete[] Stext;
        return;
    }

    cout << endl;
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;
    cout << BOLD << YELLOW << "                 SUPPORT REQUESTS" << RESET << endl;
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;

    cout << left << setw(15) << "Support ID" << setw(25) << "Customer" << "Request" << endl;
    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    while (display.getline(Sid, ID_LEN, '|'))
    {
        display.getline(Sname, NAME_LEN, '|');
        display.getline(Stext, TEXT_LEN, '\n');
        cout << left << setw(15) << Sid << setw(25) << Sname << endl;
        cout << "  -> " << Stext << endl << endl;
    }
    display.close();

    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    char *replyID = new char[ID_LEN];
    cout << BOLD << CYAN << "Enter Support ID to reply to: " << RESET;
    cin.getline(replyID, ID_LEN, '\n');

    char *response = new char[TEXT_LEN];
    cout << BOLD << CYAN << "Enter your response: " << RESET << endl;
    cin.getline(response, TEXT_LEN, '\n');

    fstream display1("Support.txt", ios::in);
    fstream tempwrite("temp.txt", ios::out);

    if (!display1.is_open())
    {
        cout << BOLD << RED << "Error: Could not open Support.txt!" << RESET << endl;
        delete[] Sid;
        delete[] Sname;
        delete[] Stext;
        delete[] replyID;
        delete[] response;
        return;
    }

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

    cout << BOLD << GREEN << "Response saved successfully!" << RESET << endl;
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
    cout << CYAN;
    displayLine(65, '=');
    cout << RESET;
    cout << BOLD << MAGENTA << "                   SALES RECORD" << RESET << endl;
    cout << CYAN;
    displayLine(65, '=');
    cout << RESET;

    cout << left << setw(20) << "Customer" << setw(20) << "Product"
         << "$" << setw(10) << "Price" << setw(10) << "Qty"
         << setw(10) << "ID" << endl;
    cout << CYAN;
    displayLine(65, '-');
    cout << RESET;

    if (read.is_open())
    {
        while (read.getline(readname, NAME_LEN, '|'))
        {
            read.getline(readproduct, NAME_LEN, '|');
            read >> readprice;
            read.ignore(1, '|');
            read.getline(readp_ID, ID_LEN, '|');
            read >> readquantity;
            read.ignore();

            cout << left << setw(20) << readname << setw(20) << readproduct
                 << "$" << setw(10) << readprice << setw(10) << readquantity
                 << setw(10) << readp_ID << endl;

            totalRevenue += (readprice * readquantity);
        }
    }
    else
    {
        cout << BOLD << RED << "Could not open order history file." << RESET << endl;
    }

    cout << CYAN;
    displayLine(65, '-');
    cout << RESET;
    cout << BOLD << GREEN << "Total Revenue: $" << totalRevenue << RESET << endl;
    cout << CYAN;
    displayLine(65, '=');
    cout << RESET;
    read.close();

    delete[] readname;
    delete[] readproduct;
    delete[] readp_ID;
}

void stock()
{
    char *price = new char[NAME_LEN];
    char *p_ID = new char[NAME_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int quantity;

    cout << endl;
    cout << YELLOW;
    displayLine(70, '!');
    cout << RESET;
    cout << BOLD << RED << "          LOW STOCK ALERT - Restocking Needed!" << RESET << endl;
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
            read.getline(p_ID, NAME_LEN, '|');
            read.getline(name, NAME_LEN, '|');
            read.getline(category, NAME_LEN, '|');
            read >> quantity;
            read.ignore(1, '\n');

            if (quantity <= 5)
            {
                cout << left << setw(25) << name << "$" << setw(10) << price
                     << setw(20) << category << setw(10) << p_ID
                     << setw(10) << quantity << BOLD << RED << " *** LOW ***" << RESET << endl;
                found = true;
            }
        }
    }

    if (!found)
    {
        cout << BOLD << GREEN << "All products have sufficient stock - no alerts." << RESET << endl;
    }

    cout << YELLOW;
    displayLine(70, '!');
    cout << RESET;
    cout << BOLD << YELLOW << "These products need RESTOCKING!" << RESET << endl;
    cout << YELLOW;
    displayLine(70, '!');
    cout << RESET;

    read.close();

    delete[] price;
    delete[] p_ID;
    delete[] name;
    delete[] category;
}
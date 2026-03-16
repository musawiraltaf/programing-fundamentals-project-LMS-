#include "utils.h"

static const char* RESET   = "\033[0m";
static const char* BOLD    = "\033[1m";
static const char* RED     = "\033[31m";
static const char* GREEN   = "\033[32m";
static const char* YELLOW  = "\033[33m";
static const char* BLUE    = "\033[34m";
static const char* MAGENTA = "\033[35m";
static const char* CYAN    = "\033[36m";
static const char* WHITE   = "\033[37m";

bool compareStrings(char *p1, char *p2)
{
    int a;
    for (a = 0; p2[a] != '\0'; a++)
    {
        if (p1[a] != p2[a])
        {
            return false;
        }
    }
    return (p1[a] == '\0');
}

void encryptPassword(char *enpass)
{
    for (char *ptr = enpass; *ptr != '\0'; ptr++)
    {
        *ptr = (*ptr + *ptr) % 126;

        if (*ptr == 0)
        {
            *ptr = 1;
        }
    }
}

int getValidInt(int min, int max)
{
    int value;
    while (true)
    {
        cin >> value;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << BOLD << RED << "Invalid input! Enter a number (" << min << " to " << max << "): " << RESET;
        }
        else if (value < min || value > max)
        {
            cout << BOLD << RED << "Invalid choice! Enter again (" << min << " to " << max << "): " << RESET;
        }
        else
        {
            cin.ignore();
            return value;
        }
    }
}

void logActivity(const char *message)
{
    fstream act("activity.txt", ios::app);
    if (act.is_open())
    {
        time_t now = time(0);
        char *timeStr = ctime(&now);
        timeStr[strlen(timeStr) - 1] = '\0';

        act << "[" << timeStr << "] " << message << endl;
        act.close();
    }
}

void logAudit(const char *action)
{
    fstream audit("audit.txt", ios::app);
    if (audit.is_open())
    {
        time_t now = time(0);
        char *timeStr = ctime(&now);
        timeStr[strlen(timeStr) - 1] = '\0';

        audit << "[AUDIT] [" << timeStr << "] " << action << endl;
        audit.close();
    }
}

void displayLine(int width, char ch)
{
    for (int i = 0; i < width; i++)
    {
        cout << ch;
    }
    cout << endl;
}

void displayCatalogue()
{
    char *price = new char[NAME_LEN];
    char *p_ID = new char[NAME_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    char *quantity = new char[NAME_LEN];

    cout << endl;
    cout << CYAN;
    displayLine(80, '-');
    cout << RESET;
    cout << BOLD << BLUE << "                           PRODUCT CATALOGUE" << RESET << endl;
    cout << CYAN;
    displayLine(80, '-');
    cout << RESET;

    cout << left << setw(25) << "Product"
         << "$" << setw(10) << "Price"
         << setw(20) << "Category"
         << setw(10) << "ID"
         << setw(10) << "Stock" << endl;
    cout << CYAN;
    displayLine(80, '-');
    cout << RESET;

    fstream read("catalogue.txt", ios::in);
    if (!read.is_open())
    {
        cout << BOLD << RED << "Error: Could not open catalogue.txt!" << RESET << endl;
        delete[] price;
        delete[] p_ID;
        delete[] name;
        delete[] category;
        delete[] quantity;
        return;
    }

    while (read.getline(price, NAME_LEN, '|'))
    {
        read.getline(p_ID, NAME_LEN, '|');
        read.getline(name, NAME_LEN, '|');
        read.getline(category, NAME_LEN, '|');
        read.getline(quantity, NAME_LEN, '\n');
        cout << left << setw(25) << name
             << "$" << setw(10) << price
             << setw(20) << category
             << setw(10) << p_ID
             << setw(10) << quantity << endl;
    }
    read.close();

    cout << CYAN;
    displayLine(80, '-');
    cout << RESET;

    delete[] price;
    delete[] p_ID;
    delete[] name;
    delete[] category;
    delete[] quantity;
}

void catalogueAdd()
{
    char *id = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    char *stock = new char[ID_LEN];
    int price;

    cout << BOLD << CYAN << "Enter Product ID: " << RESET;
    cin.getline(id, ID_LEN);
    cout << BOLD << CYAN << "Enter Product Name: " << RESET;
    cin.getline(name, NAME_LEN);
    cout << BOLD << CYAN << "Enter Product Category: " << RESET;
    cin.getline(category, NAME_LEN);
    cout << BOLD << CYAN << "Enter Product Price: " << RESET;
    cin >> price;
    cin.ignore();
    cout << BOLD << CYAN << "Enter Stock Quantity: " << RESET;
    cin.getline(stock, ID_LEN);

    ofstream outfile("catalogue.txt", ios::app);
    if (!outfile)
    {
        cout << BOLD << RED << "Error: Could not open catalogue.txt for writing." << RESET << endl;
        delete[] id;
        delete[] name;
        delete[] category;
        delete[] stock;
        return;
    }

    outfile << price << "|" << id << "|" << name << "|" << category << "|" << stock << endl;
    outfile.close();
    cout << BOLD << GREEN << "Product added successfully!" << RESET << endl;

    logAudit("Product added to catalogue");
    logActivity("Product added to catalogue");

    delete[] id;
    delete[] name;
    delete[] category;
    delete[] stock;
}

void catalogueDelete()
{
    char *id = new char[ID_LEN];
    cout << BOLD << CYAN << "Enter Product ID to Delete: " << RESET;
    cin.getline(id, ID_LEN);

    ifstream infile("catalogue.txt");
    ofstream temp("temp.txt");
    if (!infile || !temp)
    {
        cerr << BOLD << RED << "Error: Could not open files." << RESET << endl;
        delete[] id;
        return;
    }

    char *price = new char[ID_LEN];
    char *pid = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    char *stock = new char[ID_LEN];
    bool found = false;

    while (infile.getline(price, ID_LEN, '|'))
    {
        infile.getline(pid, ID_LEN, '|');
        infile.getline(name, NAME_LEN, '|');
        infile.getline(category, NAME_LEN, '|');
        infile.getline(stock, ID_LEN, '\n');

        if (compareStrings(pid, id))
        {
            found = true;
            continue;
        }
        temp << price << "|" << pid << "|" << name << "|" << category << "|" << stock << endl;
    }

    infile.close();
    temp.close();

    if (found)
    {
        remove("catalogue.txt");
        rename("temp.txt", "catalogue.txt");
        cout << BOLD << GREEN << "Product deleted successfully!" << RESET << endl;
        logAudit("Product deleted from catalogue");
    }
    else
    {
        remove("temp.txt");
        cout << BOLD << RED << "Product ID not found." << RESET << endl;
    }

    delete[] id;
    delete[] price;
    delete[] pid;
    delete[] name;
    delete[] category;
    delete[] stock;
}

void catalogueEdit()
{
    char *id = new char[ID_LEN];
    cout << BOLD << CYAN << "Enter Product ID to Edit: " << RESET;
    cin.getline(id, ID_LEN);

    ifstream infile("catalogue.txt");
    ofstream temp("temp.txt");

    if (!infile || !temp)
    {
        cerr << BOLD << RED << "Error: Could not open files." << RESET << endl;
        delete[] id;
        return;
    }

    char *price = new char[ID_LEN];
    char *pid = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    char *stock = new char[ID_LEN];
    bool found = false;

    while (infile.getline(price, ID_LEN, '|'))
    {
        infile.getline(pid, ID_LEN, '|');
        infile.getline(name, NAME_LEN, '|');
        infile.getline(category, NAME_LEN, '|');
        infile.getline(stock, ID_LEN, '\n');

        if (compareStrings(pid, id))
        {
            found = true;
            cout << BOLD << CYAN << "Enter New Product Name: " << RESET;
            cin.getline(name, NAME_LEN);
            cout << BOLD << CYAN << "Enter New Category: " << RESET;
            cin.getline(category, NAME_LEN);
            cout << BOLD << CYAN << "Enter New Price: " << RESET;
            cin.getline(price, ID_LEN);
            cout << BOLD << CYAN << "Enter New Stock Quantity: " << RESET;
            cin.getline(stock, ID_LEN);
        }

        temp << price << "|" << pid << "|" << name << "|" << category << "|" << stock << endl;
    }

    infile.close();
    temp.close();

    if (found)
    {
        remove("catalogue.txt");
        rename("temp.txt", "catalogue.txt");
        cout << BOLD << GREEN << "Product updated successfully!" << RESET << endl;
        logAudit("Product updated in catalogue");
    }
    else
    {
        remove("temp.txt");
        cout << BOLD << RED << "Product ID not found." << RESET << endl;
    }

    delete[] id;
    delete[] price;
    delete[] pid;
    delete[] name;
    delete[] category;
    delete[] stock;
}

void catalogueManage()
{
    cout << endl;
    cout << CYAN;
    displayLine(50, '=');
    cout << RESET;
    cout << BOLD << MAGENTA << "              CATALOGUE MANAGEMENT" << RESET << endl;
    cout << CYAN;
    displayLine(50, '=');
    cout << RESET;

    cout << GREEN << "1. Add new item to Catalogue" << RESET << endl;
    cout << RED << "2. Delete item from Catalogue" << RESET << endl;
    cout << BLUE << "3. Edit item in Catalogue" << RESET << endl;
    cout << BOLD << YELLOW << "Enter your choice: " << RESET;

    int choice = getValidInt(1, 3);

    if (choice == 1)
    {
        catalogueAdd();
    }
    else if (choice == 2)
    {
        catalogueDelete();
    }
    else if (choice == 3)
    {
        catalogueEdit();
    }
}

void sendAnnouncement()
{
    char *announcement = new char[TEXT_LEN];
    cout << BOLD << CYAN << "Write your announcement: " << RESET;
    cin.getline(announcement, TEXT_LEN);

    fstream write("announcements.txt", ios::app);
    if (write.is_open())
    {
        time_t now = time(0);
        char *timeStr = ctime(&now);
        timeStr[strlen(timeStr) - 1] = '\0';
        write << "[" << timeStr << "] " << announcement << endl;
        write.close();
        cout << BOLD << GREEN << "Announcement sent successfully!" << RESET << endl;
        logAudit("Announcement sent");
    }
    else
    {
        cout << BOLD << RED << "Error: Could not open announcements.txt!" << RESET << endl;
    }

    delete[] announcement;
}

void viewAnnouncements()
{
    fstream read("announcements.txt", ios::in);
    if (!read.is_open())
    {
        return;
    }

    char *line = new char[TEXT_LEN];
    bool hasAnnouncements = false;

    if (read.peek() != EOF)
    {
        hasAnnouncements = true;
        cout << endl;
        cout << CYAN;
        displayLine(60, '*');
        cout << RESET;
        cout << BOLD << YELLOW << "                NEW ANNOUNCEMENTS" << RESET << endl;
        cout << CYAN;
        displayLine(60, '*');
        cout << RESET;
    }

    while (read.getline(line, TEXT_LEN))
    {
        cout << WHITE << "  >> " << line << RESET << endl;
    }

    if (hasAnnouncements)
    {
        cout << CYAN;
        displayLine(60, '*');
        cout << RESET;
    }

    read.close();
    delete[] line;
}
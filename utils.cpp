#include "utils.h"

char currentCustomerName[NAME_LEN] = "";
char currentEmployeeName[NAME_LEN] = "";
char currentAdminName[NAME_LEN] = "";

bool compareStrings(const char *p1, const char *p2)
{
    int i = 0;
    while (p1[i] != '\0' && p2[i] != '\0')
    {
        if (p1[i] != p2[i])
        {
            return false;
        }
        i++;
    }
    return (p1[i] == '\0' && p2[i] == '\0');
}

void copyString(char *dest, const char *src)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

bool isEmptyString(const char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
        {
            return false;
        }
        i++;
    }
    return true;
}

bool getNonEmptyLine(const char *promptText, char *buffer, int len)
{
    while (true)
    {
        printPrompt(promptText);
        cin.getline(buffer, len);

        if (!isEmptyString(buffer))
        {
            return true;
        }

        printError("Input cannot be empty. Please try again.");
    }
}

bool usernameExists(const char *filename, const char *username)
{
    ifstream read(filename);
    if (!read.is_open())
    {
        return false;
    }

    char *storedName = new char[NAME_LEN];
    bool found = false;

    while (read.getline(storedName, NAME_LEN, '|'))
    {
        if (compareStrings(storedName, username))
        {
            found = true;
            break;
        }
        read.ignore(1000, '\n');
    }

    read.close();
    delete[] storedName;
    return found;
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
            cout << BOLD << RED
                 << "Invalid input! Enter a number (" << min << " to " << max << "): "
                 << RESET;
        }
        else if (value < min || value > max)
        {
            cin.ignore(1000, '\n');
            cout << BOLD << RED
                 << "Invalid choice! Enter again (" << min << " to " << max << "): "
                 << RESET;
        }
        else
        {
            cin.ignore(1000, '\n');
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

static bool productIdExists(const char *targetId)
{
    ifstream read("catalogue.txt");
    if (!read.is_open())
    {
        return false;
    }

    char *price = new char[NAME_LEN];
    char *id = new char[ID_LEN];
    bool found = false;

    while (read.getline(price, NAME_LEN, '|'))
    {
        read.getline(id, ID_LEN, '|');

        if (compareStrings(id, targetId))
        {
            found = true;
            break;
        }

        read.ignore(1000, '\n');
    }

    read.close();
    delete[] price;
    delete[] id;
    return found;
}

void displayCatalogue()
{
    char *price = new char[NAME_LEN];
    char *p_ID = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    char *quantity = new char[NAME_LEN];

    cout << endl;
    printSection("PRODUCT CATALOGUE", 80);
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
        printError("Error: Could not open catalogue.txt!");
        delete[] price;
        delete[] p_ID;
        delete[] name;
        delete[] category;
        delete[] quantity;
        return;
    }

    while (read.getline(price, NAME_LEN, '|'))
    {
        read.getline(p_ID, ID_LEN, '|');
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
    int price;
    int stockQty;

    getNonEmptyLine("Enter Product ID: ", id, ID_LEN);

    if (productIdExists(id))
    {
        printError("Product ID already exists.");
        delete[] id;
        delete[] name;
        delete[] category;
        return;
    }

    getNonEmptyLine("Enter Product Name: ", name, NAME_LEN);
    getNonEmptyLine("Enter Product Category: ", category, NAME_LEN);

    printPrompt("Enter Product Price: ");
    price = getValidInt(1, 100000000);

    printPrompt("Enter Stock Quantity: ");
    stockQty = getValidInt(0, 1000000);

    ofstream outfile("catalogue.txt", ios::app);
    if (!outfile)
    {
        printError("Error: Could not open catalogue.txt for writing.");
        delete[] id;
        delete[] name;
        delete[] category;
        return;
    }

    outfile << price << "|" << id << "|" << name << "|" << category << "|" << stockQty << endl;
    outfile.close();

    printSuccess("Product added successfully!");
    logAudit("Product added to catalogue");
    logActivity("Product added to catalogue");

    delete[] id;
    delete[] name;
    delete[] category;
}

void catalogueDelete()
{
    char *id = new char[ID_LEN];
    getNonEmptyLine("Enter Product ID to Delete: ", id, ID_LEN);

    ifstream infile("catalogue.txt");
    ofstream temp("temp.txt");

    if (!infile || !temp)
    {
        printError("Error: Could not open files.");
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
        printSuccess("Product deleted successfully!");
        logAudit("Product deleted from catalogue");
    }
    else
    {
        remove("temp.txt");
        printError("Product ID not found.");
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
    getNonEmptyLine("Enter Product ID to Edit: ", id, ID_LEN);

    ifstream infile("catalogue.txt");
    ofstream temp("temp.txt");

    if (!infile || !temp)
    {
        printError("Error: Could not open files.");
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
            getNonEmptyLine("Enter New Product Name: ", name, NAME_LEN);
            getNonEmptyLine("Enter New Category: ", category, NAME_LEN);
            getNonEmptyLine("Enter New Price: ", price, ID_LEN);
            getNonEmptyLine("Enter New Stock Quantity: ", stock, ID_LEN);
        }

        temp << price << "|" << pid << "|" << name << "|" << category << "|" << stock << endl;
    }

    infile.close();
    temp.close();

    if (found)
    {
        remove("catalogue.txt");
        rename("temp.txt", "catalogue.txt");
        printSuccess("Product updated successfully!");
        logAudit("Product updated in catalogue");
    }
    else
    {
        remove("temp.txt");
        printError("Product ID not found.");
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
    while (true)
    {
        cout << endl;
        printSection("CATALOGUE MANAGEMENT", 50);
        printMenuOption(1, "Add new item to Catalogue", GREEN);
        printMenuOption(2, "Delete item from Catalogue", RED);
        printMenuOption(3, "Edit item in Catalogue", BLUE);
        printMenuOption(4, "Back", YELLOW);
        cout << endl;
        printPrompt("Enter your choice: ");

        int choice = getValidInt(1, 4);

        if (choice == 1)
        {
            catalogueAdd();
            pauseScreen();
        }
        else if (choice == 2)
        {
            catalogueDelete();
            pauseScreen();
        }
        else if (choice == 3)
        {
            catalogueEdit();
            pauseScreen();
        }
        else
        {
            return;
        }
    }
}

void sendAnnouncement()
{
    char *announcement = new char[TEXT_LEN];
    getNonEmptyLine("Write your announcement: ", announcement, TEXT_LEN);

    fstream write("announcements.txt", ios::app);
    if (write.is_open())
    {
        time_t now = time(0);
        char *timeStr = ctime(&now);
        timeStr[strlen(timeStr) - 1] = '\0';

        write << "[" << timeStr << "] " << announcement << endl;
        write.close();

        printSuccess("Announcement sent successfully!");
        logAudit("Announcement sent");
    }
    else
    {
        printError("Error: Could not open announcements.txt!");
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
        printSection("NEW ANNOUNCEMENTS", 60);
    }

    while (read.getline(line, TEXT_LEN))
    {
        cout << WHITE << "  >> " << line << RESET << endl;
    }

    if (hasAnnouncements)
    {
        cout << CYAN;
        displayLine(60, '-');
        cout << RESET;
    }

    read.close();
    delete[] line;
}
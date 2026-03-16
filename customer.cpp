#include "customer.h"

static bool findProductById(const char *targetId, char *outName, char *outCategory, int &outPrice, int &outStock)
{
    char *price = new char[NAME_LEN];
    char *p_ID = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int quantity;

    ifstream read("catalogue.txt");
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

            if (compareStrings(p_ID, targetId))
            {
                copyString(outName, name);
                copyString(outCategory, category);
                outPrice = atoi(price);
                outStock = quantity;
                found = true;
                break;
            }
        }
        read.close();
    }

    delete[] price;
    delete[] p_ID;
    delete[] name;
    delete[] category;
    return found;
}

static int getCurrentCartItemQuantity(const char *productId)
{
    ifstream read("cart.txt");
    if (!read.is_open())
    {
        return 0;
    }

    char *username = new char[NAME_LEN];
    char *pid = new char[ID_LEN];
    char *pname = new char[NAME_LEN];
    int qty, price;
    int foundQty = 0;

    while (read.getline(username, NAME_LEN, '|'))
    {
        read.getline(pid, ID_LEN, '|');
        read.getline(pname, NAME_LEN, '|');
        read >> qty;
        read.ignore(1, '|');
        read >> price;
        read.ignore(1000, '\n');

        if (compareStrings(username, currentCustomerName) && compareStrings(pid, productId))
        {
            foundQty = qty;
            break;
        }
    }

    read.close();
    delete[] username;
    delete[] pid;
    delete[] pname;
    return foundQty;
}

static void saveCartQuantity(const char *productId, const char *productName, int newQty, int price)
{
    ifstream read("cart.txt");
    ofstream temp("temp_cart.txt");
    bool updated = false;

    char *username = new char[NAME_LEN];
    char *pid = new char[ID_LEN];
    char *pname = new char[NAME_LEN];
    int qty, oldPrice;

    if (read.is_open())
    {
        while (read.getline(username, NAME_LEN, '|'))
        {
            read.getline(pid, ID_LEN, '|');
            read.getline(pname, NAME_LEN, '|');
            read >> qty;
            read.ignore(1, '|');
            read >> oldPrice;
            read.ignore(1000, '\n');

            if (compareStrings(username, currentCustomerName) && compareStrings(pid, productId))
            {
                temp << currentCustomerName << "|" << productId << "|" << productName << "|" << newQty << "|" << price << endl;
                updated = true;
            }
            else
            {
                temp << username << "|" << pid << "|" << pname << "|" << qty << "|" << oldPrice << endl;
            }
        }
        read.close();
    }

    if (!updated)
    {
        temp << currentCustomerName << "|" << productId << "|" << productName << "|" << newQty << "|" << price << endl;
    }

    temp.close();
    remove("cart.txt");
    rename("temp_cart.txt", "cart.txt");

    delete[] username;
    delete[] pid;
    delete[] pname;
}

static bool currentCartIsEmpty()
{
    ifstream read("cart.txt");
    if (!read.is_open())
    {
        return true;
    }

    char *username = new char[NAME_LEN];
    bool found = false;

    while (read.getline(username, NAME_LEN, '|'))
    {
        if (compareStrings(username, currentCustomerName))
        {
            found = true;
            break;
        }
        read.ignore(1000, '\n');
    }

    read.close();
    delete[] username;
    return !found;
}

static int showCurrentCart(bool showHeader)
{
    ifstream read("cart.txt");
    if (!read.is_open())
    {
        if (showHeader)
        {
            printWarning("Your cart is empty.");
        }
        return 0;
    }

    char *username = new char[NAME_LEN];
    char *pid = new char[ID_LEN];
    char *pname = new char[NAME_LEN];
    int qty, price;
    int total = 0;
    bool found = false;

    if (showHeader)
    {
        cout << endl;
        printSection("YOUR CART", 70);
        cout << left << setw(25) << "PRODUCT" << "$" << setw(10) << "PRICE"
             << setw(12) << "QUANTITY" << setw(10) << "ID" << endl;
        cout << CYAN;
        displayLine(70, '-');
        cout << RESET;
    }

    while (read.getline(username, NAME_LEN, '|'))
    {
        read.getline(pid, ID_LEN, '|');
        read.getline(pname, NAME_LEN, '|');
        read >> qty;
        read.ignore(1, '|');
        read >> price;
        read.ignore(1000, '\n');

        if (compareStrings(username, currentCustomerName))
        {
            cout << left << setw(25) << pname << "$" << setw(10) << price
                 << setw(12) << qty << setw(10) << pid << endl;
            total += qty * price;
            found = true;
        }
    }

    read.close();

    if (showHeader)
    {
        cout << CYAN;
        displayLine(70, '-');
        cout << RESET;
        if (found)
        {
            cout << BOLD << GREEN << "Cart Total: $" << total << RESET << endl;
        }
        else
        {
            printWarning("Your cart is empty.");
        }
        cout << CYAN;
        displayLine(70, '=');
        cout << RESET;
    }

    delete[] username;
    delete[] pid;
    delete[] pname;

    return total;
}

static void clearCurrentCustomerCart()
{
    ifstream read("cart.txt");
    ofstream temp("temp_cart.txt");

    if (!read.is_open() || !temp.is_open())
    {
        return;
    }

    char *username = new char[NAME_LEN];
    char *pid = new char[ID_LEN];
    char *pname = new char[NAME_LEN];
    int qty, price;

    while (read.getline(username, NAME_LEN, '|'))
    {
        read.getline(pid, ID_LEN, '|');
        read.getline(pname, NAME_LEN, '|');
        read >> qty;
        read.ignore(1, '|');
        read >> price;
        read.ignore(1000, '\n');

        if (!compareStrings(username, currentCustomerName))
        {
            temp << username << "|" << pid << "|" << pname << "|" << qty << "|" << price << endl;
        }
    }

    read.close();
    temp.close();
    remove("cart.txt");
    rename("temp_cart.txt", "cart.txt");

    delete[] username;
    delete[] pid;
    delete[] pname;
}

static bool decreaseStockForProduct(const char *productId, int quantityToRemove)
{
    ifstream infile("catalogue.txt");
    ofstream temp("temp.txt");

    if (!infile.is_open() || !temp.is_open())
    {
        return false;
    }

    char *price = new char[NAME_LEN];
    char *pid = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int stock;
    bool updated = false;

    while (infile.getline(price, NAME_LEN, '|'))
    {
        infile.getline(pid, ID_LEN, '|');
        infile.getline(name, NAME_LEN, '|');
        infile.getline(category, NAME_LEN, '|');
        infile >> stock;
        infile.ignore(1000, '\n');

        if (compareStrings(pid, productId))
        {
            stock -= quantityToRemove;
            if (stock < 0)
            {
                stock = 0;
            }
            updated = true;
        }

        temp << price << "|" << pid << "|" << name << "|" << category << "|" << stock << endl;
    }

    infile.close();
    temp.close();

    if (updated)
    {
        remove("catalogue.txt");
        rename("temp.txt", "catalogue.txt");
    }
    else
    {
        remove("temp.txt");
    }

    delete[] price;
    delete[] pid;
    delete[] name;
    delete[] category;

    return updated;
}

static bool wishlistAlreadyHas(const char *productId)
{
    ifstream read("wishlist.txt");
    if (!read.is_open())
    {
        return false;
    }

    char *username = new char[NAME_LEN];
    char *pid = new char[ID_LEN];
    char *pname = new char[NAME_LEN];
    bool found = false;

    while (read.getline(username, NAME_LEN, '|'))
    {
        read.getline(pid, ID_LEN, '|');
        read.getline(pname, NAME_LEN, '\n');

        if (compareStrings(username, currentCustomerName) && compareStrings(pid, productId))
        {
            found = true;
            break;
        }
    }

    read.close();
    delete[] username;
    delete[] pid;
    delete[] pname;
    return found;
}

static void browseByCategory()
{
    char *price = new char[NAME_LEN];
    char *p_ID = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int quantity;
    char *usersearch = new char[NAME_LEN];

    getNonEmptyLine("Enter category to browse: ", usersearch, NAME_LEN);

    fstream read("catalogue.txt", ios::in);
    if (read.is_open())
    {
        printSection("CATEGORY RESULTS", 80);
        bool found = false;

        while (read.getline(price, NAME_LEN, '|'))
        {
            read.getline(p_ID, ID_LEN, '|');
            read.getline(name, NAME_LEN, '|');
            read.getline(category, NAME_LEN, '|');
            read >> quantity;
            read.ignore(1000, '\n');

            if (compareStrings(usersearch, category))
            {
                cout << left << setw(25) << name << "$" << setw(10) << price
                     << setw(20) << category << setw(10) << p_ID
                     << setw(10) << quantity << endl;
                found = true;
            }
        }

        if (!found)
        {
            printWarning("No products found in this category.");
        }

        cout << CYAN;
        displayLine(80, '-');
        cout << RESET;
    }
    else
    {
        printError("Error: Could not open file!");
    }

    read.close();
    delete[] price;
    delete[] p_ID;
    delete[] name;
    delete[] category;
    delete[] usersearch;

    customerAddToCart();
}

static void browseByPrice()
{
    int price;
    int usersearch;
    char *p_ID = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int quantity;

    printPrompt("Enter upper price range: ");
    usersearch = getValidInt(1, 100000000);

    fstream read("catalogue.txt", ios::in);

    if (read.is_open())
    {
        printSection("PRICE RANGE RESULTS", 80);
        bool found = false;

        while (read >> price)
        {
            read.ignore(1, '|');
            read.getline(p_ID, ID_LEN, '|');
            read.getline(name, NAME_LEN, '|');
            read.getline(category, NAME_LEN, '|');
            read >> quantity;
            read.ignore(1000, '\n');

            if (price <= usersearch)
            {
                cout << left << setw(25) << name << "$" << setw(10) << price
                     << setw(20) << category << setw(10) << p_ID
                     << setw(10) << quantity << endl;
                found = true;
            }
        }

        if (!found)
        {
            printWarning("No products found in this price range.");
        }

        cout << CYAN;
        displayLine(80, '-');
        cout << RESET;
    }
    else
    {
        printError("Error: Could not open file!");
    }

    read.close();
    delete[] p_ID;
    delete[] name;
    delete[] category;

    customerAddToCart();
}

static void browseByAvailability()
{
    int price;
    int usersearch;
    char *p_ID = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int quantity;

    printPrompt("Enter minimum required quantity: ");
    usersearch = getValidInt(0, 1000000);

    fstream read("catalogue.txt", ios::in);

    if (read.is_open())
    {
        printSection("AVAILABILITY RESULTS", 80);
        bool found = false;

        while (read >> price)
        {
            read.ignore(1, '|');
            read.getline(p_ID, ID_LEN, '|');
            read.getline(name, NAME_LEN, '|');
            read.getline(category, NAME_LEN, '|');
            read >> quantity;
            read.ignore(1000, '\n');

            if (quantity >= usersearch)
            {
                cout << left << setw(25) << name << "$" << setw(10) << price
                     << setw(20) << category << setw(10) << p_ID
                     << setw(10) << quantity << endl;
                found = true;
            }
        }

        if (!found)
        {
            printWarning("No products found with this quantity.");
        }

        cout << CYAN;
        displayLine(80, '-');
        cout << RESET;
    }
    else
    {
        printError("Error: Could not open file!");
    }

    read.close();
    delete[] p_ID;
    delete[] name;
    delete[] category;

    customerAddToCart();
}

void cmenu()
{
    printHeader("SECURESHOP - CUSTOMER PORTAL");

    while (true)
    {
        printInfo("Choose an option:");
        cout << endl;
        printMenuOption(1, "Register New Customer", GREEN);
        printMenuOption(2, "Login", BLUE);
        printMenuOption(3, "Back", RED);
        cout << endl;
        printPrompt("Enter your choice: ");

        int choice = getValidInt(1, 3);

        if (choice == 1)
        {
            cregistration();
            pauseScreen();
            printHeader("SECURESHOP - CUSTOMER PORTAL");
        }
        else if (choice == 2)
        {
            if (cverify())
            {
                cout << endl;
                cout << GREEN;
                displayLine(70, '=');
                cout << RESET;
                cout << BOLD << GREEN << "  LOGIN SUCCESSFUL - Welcome!" << RESET << endl;
                cout << GREEN;
                displayLine(70, '=');
                cout << RESET;

                viewAnnouncements();
                logActivity("Customer login successful");
                displayproducts();
                currentCustomerName[0] = '\0';
                printHeader("SECURESHOP - CUSTOMER PORTAL");
            }
            else
            {
                pauseScreen();
                printHeader("SECURESHOP - CUSTOMER PORTAL");
            }
        }
        else
        {
            break;
        }
    }
}

bool cverify()
{
    bool authenticated = false;
    int attempts = 0;

    while (attempts < 3 && !authenticated)
    {
        if (attempts > 0)
        {
            printError("Wrong credentials! Please try again.");
            logActivity("Customer login failed attempt");
        }

        char *cname = new char[PASS_LEN];
        char *cpassword = new char[PASS_LEN];

        getNonEmptyLine("Enter Your Name: ", cname, PASS_LEN);
        getNonEmptyLine("Enter Your Password: ", cpassword, PASS_LEN);

        encryptPassword(cpassword);

        ifstream read("customer.txt");
        if (!read)
        {
            printError("Error: Could not open customer.txt!");
            delete[] cname;
            delete[] cpassword;
            return false;
        }

        char *fname = new char[PASS_LEN];
        char *fpassword = new char[PASS_LEN];

        while (read.getline(fname, PASS_LEN, '|'))
        {
            read.getline(fpassword, PASS_LEN, '|');
            read.ignore(1000, '\n');

            if (compareStrings(cname, fname) && compareStrings(cpassword, fpassword))
            {
                authenticated = true;
                copyString(currentCustomerName, cname);
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
        logActivity("Customer multiple failed login attempts - SECURITY ALERT");
    }

    return authenticated;
}

void cregistration()
{
    char *rname = new char[PASS_LEN];
    char *rpassword = new char[PASS_LEN];

    getNonEmptyLine("Enter your Name: ", rname, PASS_LEN);

    if (usernameExists("customer.txt", rname))
    {
        printError("Customer username already exists.");
        delete[] rname;
        delete[] rpassword;
        return;
    }

    getNonEmptyLine("Enter your Password: ", rpassword, PASS_LEN);
    encryptPassword(rpassword);

    fstream write("customer.txt", ios::app);
    if (!write.is_open())
    {
        printError("Error: Could not open customer.txt!");
        delete[] rname;
        delete[] rpassword;
        return;
    }

    write << rname << "|" << rpassword << "|" << endl;
    write.close();

    printSuccess("Customer registered successfully!");
    logActivity("New customer registered");

    delete[] rname;
    delete[] rpassword;
}

void displayproducts()
{
    printHeader("SECURESHOP - CUSTOMER DASHBOARD");
    displayCatalogue();
    searchproducts();
}

void searchproducts()
{
    while (true)
    {
        cout << endl;
        printSection("CUSTOMER MENU", 50);
        printMenuOption(1, "Buy Products", GREEN);
        printMenuOption(2, "Wishlist", BLUE);
        printMenuOption(3, "View Order History", YELLOW);
        printMenuOption(4, "Support and Feedback", CYAN);
        printMenuOption(5, "Cart", MAGENTA);
        printMenuOption(6, "Exit", RED);
        cout << endl;
        printPrompt("Enter your choice: ");

        int choice = getValidInt(1, 6);

        if (choice == 1)
        {
            cout << endl;
            printSection("BROWSE PRODUCTS", 45);
            printMenuOption(1, "Category", GREEN);
            printMenuOption(2, "Price Range", BLUE);
            printMenuOption(3, "Availability", MAGENTA);
            printMenuOption(4, "Direct Product ID", CYAN);
            printMenuOption(5, "Back", YELLOW);
            cout << endl;
            printPrompt("Enter your choice: ");

            int choose = getValidInt(1, 5);

            if (choose == 1)
            {
                browseByCategory();
                pauseScreen();
            }
            else if (choose == 2)
            {
                browseByPrice();
                pauseScreen();
            }
            else if (choose == 3)
            {
                browseByAvailability();
                pauseScreen();
            }
            else if (choose == 4)
            {
                customerAddToCart();
                pauseScreen();
            }
        }
        else if (choice == 2)
        {
            addtowishlist();
            pauseScreen();
        }
        else if (choice == 3)
        {
            vieworder();
            pauseScreen();
        }
        else if (choice == 4)
        {
            cout << endl;
            printSection("SUPPORT & FEEDBACK", 40);
            printMenuOption(1, "Submit Feedback", GREEN);
            printMenuOption(2, "Submit Support Request", BLUE);
            printMenuOption(3, "Back", YELLOW);
            cout << endl;
            printPrompt("Enter your choice: ");

            int decision = getValidInt(1, 3);

            if (decision == 1)
            {
                Feedback();
                pauseScreen();
            }
            else if (decision == 2)
            {
                Support();
                pauseScreen();
            }
        }
        else if (choice == 5)
        {
            displaycart();
        }
        else
        {
            printWarning("Exiting customer menu.");
            break;
        }
    }
}

void customerAddToCart()
{
    if (isEmptyString(currentCustomerName))
    {
        printError("No customer session found.");
        return;
    }

    char *UPID = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int price = 0;
    int stock = 0;

    getNonEmptyLine("Enter Product ID: ", UPID, ID_LEN);

    if (!findProductById(UPID, name, category, price, stock))
    {
        printError("Product ID not found in catalogue.");
        delete[] UPID;
        delete[] name;
        delete[] category;
        return;
    }

    printPrompt("Enter Quantity: ");
    int reqquantity = getValidInt(1, 1000000);

    int alreadyInCart = getCurrentCartItemQuantity(UPID);

    if (alreadyInCart + reqquantity > stock)
    {
        printError("Not enough stock for this quantity.");
        delete[] UPID;
        delete[] name;
        delete[] category;
        return;
    }

    saveCartQuantity(UPID, name, alreadyInCart + reqquantity, price);
    printSuccess("Product added to cart successfully.");

    delete[] UPID;
    delete[] name;
    delete[] category;
}

void customerRemoveFromCart()
{
    if (currentCartIsEmpty())
    {
        printWarning("Your cart is empty.");
        return;
    }

    char *removeID = new char[ID_LEN];
    getNonEmptyLine("Enter Product ID to remove from cart: ", removeID, ID_LEN);

    ifstream readCart("cart.txt");
    ofstream tempFile("temp_cart.txt");

    char *username = new char[NAME_LEN];
    char *UPID = new char[ID_LEN];
    char *name = new char[NAME_LEN];
    int quantity, price;
    bool found = false;

    while (readCart.getline(username, NAME_LEN, '|'))
    {
        readCart.getline(UPID, ID_LEN, '|');
        readCart.getline(name, NAME_LEN, '|');
        readCart >> quantity;
        readCart.ignore(1, '|');
        readCart >> price;
        readCart.ignore(1000, '\n');

        if (compareStrings(username, currentCustomerName) && compareStrings(UPID, removeID))
        {
            found = true;
            continue;
        }

        tempFile << username << "|" << UPID << "|" << name << "|" << quantity << "|" << price << endl;
    }

    readCart.close();
    tempFile.close();

    if (found)
    {
        remove("cart.txt");
        rename("temp_cart.txt", "cart.txt");
        printSuccess("Item removed from cart.");
    }
    else
    {
        remove("temp_cart.txt");
        printError("Product ID not found in your cart.");
    }

    delete[] removeID;
    delete[] username;
    delete[] UPID;
    delete[] name;
}

void customerUpdateCart()
{
    if (currentCartIsEmpty())
    {
        printWarning("Your cart is empty.");
        return;
    }

    char *updateID = new char[ID_LEN];
    getNonEmptyLine("Enter Product ID to update quantity: ", updateID, ID_LEN);

    printPrompt("Enter new quantity: ");
    int newQty = getValidInt(1, 1000000);

    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int price = 0;
    int stock = 0;

    if (!findProductById(updateID, name, category, price, stock))
    {
        printError("Product ID not found in catalogue.");
        delete[] updateID;
        delete[] name;
        delete[] category;
        return;
    }

    if (newQty > stock)
    {
        printError("Requested quantity exceeds stock.");
        delete[] updateID;
        delete[] name;
        delete[] category;
        return;
    }

    ifstream readCart("cart.txt");
    ofstream tempFile("temp_cart.txt");

    char *username = new char[NAME_LEN];
    char *UPID = new char[ID_LEN];
    char *pname = new char[NAME_LEN];
    int quantity, oldPrice;
    bool found = false;

    while (readCart.getline(username, NAME_LEN, '|'))
    {
        readCart.getline(UPID, ID_LEN, '|');
        readCart.getline(pname, NAME_LEN, '|');
        readCart >> quantity;
        readCart.ignore(1, '|');
        readCart >> oldPrice;
        readCart.ignore(1000, '\n');

        if (compareStrings(username, currentCustomerName) && compareStrings(UPID, updateID))
        {
            tempFile << username << "|" << UPID << "|" << pname << "|" << newQty << "|" << oldPrice << endl;
            found = true;
        }
        else
        {
            tempFile << username << "|" << UPID << "|" << pname << "|" << quantity << "|" << oldPrice << endl;
        }
    }

    readCart.close();
    tempFile.close();

    if (found)
    {
        remove("cart.txt");
        rename("temp_cart.txt", "cart.txt");
        printSuccess("Cart quantity updated successfully.");
    }
    else
    {
        remove("temp_cart.txt");
        printError("Product ID not found in your cart.");
    }

    delete[] updateID;
    delete[] name;
    delete[] category;
    delete[] username;
    delete[] UPID;
    delete[] pname;
}

void displaycart()
{
    while (true)
    {
        cout << endl;
        printSection("CART MENU", 45);
        printMenuOption(1, "Add more products", GREEN);
        printMenuOption(2, "View Cart", BLUE);
        printMenuOption(3, "Remove item from cart", YELLOW);
        printMenuOption(4, "Update item quantity", CYAN);
        printMenuOption(5, "Proceed to Checkout", MAGENTA);
        printMenuOption(6, "Back", RED);
        cout << endl;
        printPrompt("Enter your choice: ");

        int a = getValidInt(1, 6);

        if (a == 1)
        {
            customerAddToCart();
        }
        else if (a == 2)
        {
            showCurrentCart(true);
        }
        else if (a == 3)
        {
            customerRemoveFromCart();
        }
        else if (a == 4)
        {
            customerUpdateCart();
        }
        else if (a == 5)
        {
            checkout();
        }
        else
        {
            break;
        }
    }
}

void addtowishlist()
{
    while (true)
    {
        cout << endl;
        printSection("WISHLIST", 40);
        printMenuOption(1, "Add Product to Wishlist", GREEN);
        printMenuOption(2, "View Wishlist", BLUE);
        printMenuOption(3, "Back", YELLOW);
        cout << endl;
        printPrompt("Enter your choice: ");

        int a = getValidInt(1, 3);

        if (a == 1)
        {
            char *UPID = new char[ID_LEN];
            char *name = new char[NAME_LEN];
            char *category = new char[NAME_LEN];
            int price = 0, stock = 0;

            getNonEmptyLine("Enter Product ID: ", UPID, ID_LEN);

            if (!findProductById(UPID, name, category, price, stock))
            {
                printError("Product ID not found.");
            }
            else if (wishlistAlreadyHas(UPID))
            {
                printWarning("This product is already in your wishlist.");
            }
            else
            {
                ofstream write("wishlist.txt", ios::app);
                write << currentCustomerName << "|" << UPID << "|" << name << endl;
                write.close();
                printSuccess("Product added to wishlist!");
            }

            delete[] UPID;
            delete[] name;
            delete[] category;
        }
        else if (a == 2)
        {
            char *username = new char[NAME_LEN];
            char *UPID = new char[ID_LEN];
            char *productName = new char[NAME_LEN];

            fstream read("wishlist.txt", ios::in);

            cout << endl;
            printSection("YOUR WISHLIST", 60);
            cout << left << setw(15) << "ID" << setw(25) << "Product" << setw(20) << "Customer" << endl;
            cout << CYAN;
            displayLine(60, '-');
            cout << RESET;

            bool found = false;

            if (read.is_open())
            {
                while (read.getline(username, NAME_LEN, '|'))
                {
                    read.getline(UPID, ID_LEN, '|');
                    read.getline(productName, NAME_LEN, '\n');

                    if (compareStrings(username, currentCustomerName))
                    {
                        cout << left << setw(15) << UPID << setw(25) << productName
                             << setw(20) << username << endl;
                        found = true;
                    }
                }
                read.close();
            }

            if (!found)
            {
                printWarning("Your wishlist is empty.");
            }

            cout << CYAN;
            displayLine(60, '=');
            cout << RESET;

            delete[] username;
            delete[] UPID;
            delete[] productName;
        }
        else
        {
            break;
        }
    }
}

void checkout()
{
    if (currentCartIsEmpty())
    {
        printWarning("Your cart is empty.");
        return;
    }

    cout << endl;
    printSection("CHECKOUT", 70);
    int ttl = showCurrentCart(false);

    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;
    cout << BOLD << GREEN << "Total Amount: $" << ttl << RESET << endl;
    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    printInfo("Do you want to apply a Discount Code?");
    printMenuOption(1, "Yes", GREEN);
    printMenuOption(2, "No", RED);
    printPrompt("Enter your choice: ");

    int a = getValidInt(1, 2);
    float finalTotal = ttl;

    if (a == 1)
    {
        char *code = new char[ID_LEN];
        int discount;
        fstream readDisc("discount.txt", ios::in);

        if (readDisc.is_open())
        {
            cout << endl;
            printSection("AVAILABLE DISCOUNTS", 30);

            while (readDisc.getline(code, ID_LEN, '|'))
            {
                readDisc >> discount;
                readDisc.ignore(1000, '\n');
                cout << "  " << code << " -> " << discount << "% OFF" << endl;
            }
            cout << CYAN;
            displayLine(30, '=');
            cout << RESET;
            readDisc.close();
        }

        char *ucode = new char[ID_LEN];
        getNonEmptyLine("Enter Discount Code: ", ucode, ID_LEN);

        fstream readDisc2("discount.txt", ios::in);
        bool discountApplied = false;

        if (readDisc2.is_open())
        {
            float apdiscount;
            while (readDisc2.getline(code, ID_LEN, '|'))
            {
                readDisc2 >> apdiscount;
                readDisc2.ignore(1000, '\n');

                if (compareStrings(ucode, code))
                {
                    float percentage = ((100.0f - apdiscount) / 100.0f);
                    finalTotal = ttl * percentage;
                    printSuccess("Discount applied successfully!");
                    discountApplied = true;
                    break;
                }
            }
            readDisc2.close();
        }

        if (!discountApplied)
        {
            printWarning("Invalid Discount Code. Original total will be used.");
            finalTotal = ttl;
        }

        delete[] code;
        delete[] ucode;
    }

    cout << BOLD << GREEN << "Final Total: $" << finalTotal << RESET << endl;

    printInfo("Confirm Checkout?");
    printMenuOption(1, "Yes, Confirm", GREEN);
    printMenuOption(2, "No, Cancel", RED);
    printPrompt("Enter your choice: ");

    int f = getValidInt(1, 2);

    if (f != 1)
    {
        printWarning("Checkout cancelled.");
        return;
    }

    char *username = new char[NAME_LEN];
    char *pid = new char[ID_LEN];
    char *pname = new char[NAME_LEN];
    int qty, price;

    ifstream validate("cart.txt");
    bool stockProblem = false;

    while (validate.getline(username, NAME_LEN, '|'))
    {
        validate.getline(pid, ID_LEN, '|');
        validate.getline(pname, NAME_LEN, '|');
        validate >> qty;
        validate.ignore(1, '|');
        validate >> price;
        validate.ignore(1000, '\n');

        if (compareStrings(username, currentCustomerName))
        {
            char *tmpName = new char[NAME_LEN];
            char *tmpCat = new char[NAME_LEN];
            int tmpPrice = 0, tmpStock = 0;

            if (!findProductById(pid, tmpName, tmpCat, tmpPrice, tmpStock) || qty > tmpStock)
            {
                stockProblem = true;
            }

            delete[] tmpName;
            delete[] tmpCat;

            if (stockProblem)
            {
                break;
            }
        }
    }
    validate.close();

    if (stockProblem)
    {
        printError("Checkout failed because stock changed. Review your cart again.");
        delete[] username;
        delete[] pid;
        delete[] pname;
        return;
    }

    fstream write("orderhistory.txt", ios::app);
    if (!write.is_open())
    {
        printError("Error: Could not open orderhistory.txt!");
        delete[] username;
        delete[] pid;
        delete[] pname;
        return;
    }

    ifstream read2("cart.txt");
    while (read2.getline(username, NAME_LEN, '|'))
    {
        read2.getline(pid, ID_LEN, '|');
        read2.getline(pname, NAME_LEN, '|');
        read2 >> qty;
        read2.ignore(1, '|');
        read2 >> price;
        read2.ignore(1000, '\n');

        if (compareStrings(username, currentCustomerName))
        {
            write << currentCustomerName << "|" << pname << "|" << price << "|" << pid << "|" << qty << endl;
            decreaseStockForProduct(pid, qty);
        }
    }
    read2.close();
    write.close();

    clearCurrentCustomerCart();

    cout << endl;
    cout << GREEN;
    displayLine(50, '*');
    cout << RESET;
    cout << BOLD << GREEN << "  Order Placed Successfully!" << RESET << endl;
    cout << BOLD << GREEN << "  Total Paid: $" << finalTotal << RESET << endl;
    cout << GREEN;
    displayLine(50, '*');
    cout << RESET;

    logActivity("Customer checkout completed");

    delete[] username;
    delete[] pid;
    delete[] pname;
}

void vieworder()
{
    char *readname = new char[NAME_LEN];
    char *readproduct = new char[NAME_LEN];
    int readprice;
    char *readp_ID = new char[ID_LEN];
    int readquantity;

    fstream read("orderhistory.txt", ios::in);

    cout << endl;
    printSection("ORDER HISTORY", 60);
    cout << left << setw(25) << "Product" << "$" << setw(10) << "Price"
         << setw(12) << "Quantity" << setw(10) << "ID" << endl;
    cout << CYAN;
    displayLine(60, '-');
    cout << RESET;

    bool found = false;

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

            if (compareStrings(currentCustomerName, readname))
            {
                cout << left << setw(25) << readproduct << "$" << setw(10) << readprice
                     << setw(12) << readquantity << setw(10) << readp_ID << endl;
                found = true;
            }
        }
        read.close();
    }

    if (!found)
    {
        printWarning("No order history found.");
    }

    cout << CYAN;
    displayLine(60, '=');
    cout << RESET;

    delete[] readname;
    delete[] readproduct;
    delete[] readp_ID;
}

void Feedback()
{
    char *reqID = new char[ID_LEN];
    char *feedback = new char[TEXT_LEN];

    getNonEmptyLine("Enter Request ID (unique): ", reqID, ID_LEN);

    bool idExists = true;
    while (idExists)
    {
        idExists = false;
        char *readreqID = new char[ID_LEN];
        char *tempName = new char[NAME_LEN];
        char *tempText = new char[TEXT_LEN];

        fstream read("feedback.txt", ios::in);
        if (read.is_open())
        {
            while (read.getline(readreqID, ID_LEN, '|'))
            {
                read.getline(tempName, NAME_LEN, '|');
                read.getline(tempText, TEXT_LEN, '\n');

                if (compareStrings(reqID, readreqID))
                {
                    idExists = true;
                    printError("This Request ID already exists.");
                    getNonEmptyLine("Enter a different Request ID: ", reqID, ID_LEN);
                    break;
                }
            }
            read.close();
        }

        delete[] readreqID;
        delete[] tempName;
        delete[] tempText;
    }

    getNonEmptyLine("Enter Your Feedback: ", feedback, TEXT_LEN);

    fstream write("feedback.txt", ios::app);
    if (write.is_open())
    {
        write << reqID << "|" << currentCustomerName << "|" << feedback << endl;
        write.close();
        printSuccess("Feedback submitted successfully!");
    }
    else
    {
        printError("Could not open feedback.txt!");
    }

    logActivity("Customer submitted feedback");

    delete[] reqID;
    delete[] feedback;
}

void Support()
{
    char *SID = new char[ID_LEN];
    char *Supreq = new char[TEXT_LEN];

    getNonEmptyLine("Enter Support ID (unique): ", SID, ID_LEN);

    bool idExists = true;
    while (idExists)
    {
        idExists = false;
        char *readSID = new char[ID_LEN];
        char *tempName = new char[NAME_LEN];
        char *tempText = new char[TEXT_LEN];

        fstream read("Support.txt", ios::in);
        if (read.is_open())
        {
            while (read.getline(readSID, ID_LEN, '|'))
            {
                read.getline(tempName, NAME_LEN, '|');
                read.getline(tempText, TEXT_LEN, '\n');

                if (compareStrings(SID, readSID))
                {
                    idExists = true;
                    printError("This Support ID already exists.");
                    getNonEmptyLine("Enter a different Support ID: ", SID, ID_LEN);
                    break;
                }
            }
            read.close();
        }

        delete[] readSID;
        delete[] tempName;
        delete[] tempText;
    }

    getNonEmptyLine("Enter Your Support Request: ", Supreq, TEXT_LEN);

    fstream write("Support.txt", ios::app);
    if (write.is_open())
    {
        write << SID << "|" << currentCustomerName << "|" << Supreq << endl;
        write.close();
        printSuccess("Support request submitted successfully!");
    }
    else
    {
        printError("Could not open Support.txt!");
    }

    logActivity("Customer submitted support request");

    delete[] SID;
    delete[] Supreq;
}
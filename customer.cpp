#include "customer.h"

static const char* RESET   = "\033[0m";
static const char* BOLD    = "\033[1m";
static const char* RED     = "\033[31m";
static const char* GREEN   = "\033[32m";
static const char* YELLOW  = "\033[33m";
static const char* BLUE    = "\033[34m";
static const char* MAGENTA = "\033[35m";
static const char* CYAN    = "\033[36m";
static const char* WHITE   = "\033[37m";

void cmenu()
{
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;
    cout << BOLD << BLUE << "                 SECURESHOP - CUSTOMER PORTAL" << RESET << endl;
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET << endl;

    cout << BOLD << WHITE << "Choose an option:" << RESET << endl;
    cout << GREEN << "1. Register New Customer" << RESET << endl;
    cout << BLUE << "2. Login" << RESET << endl;
    cout << BOLD << YELLOW << "Enter your choice: " << RESET;

    int choice = getValidInt(1, 2);

    if (choice == 1)
    {
        cregistration();
    }
    if (choice == 2)
    {
        if (cverify())
        {
            cout << endl;
            cout << GREEN;
            displayLine(70, '=');
            cout << RESET;
            cout << BOLD << GREEN << "                  LOGIN SUCCESSFUL - Welcome!" << RESET << endl;
            cout << GREEN;
            displayLine(70, '=');
            cout << RESET;

            viewAnnouncements();
            logActivity("Customer login successful");
            displayproducts();
        }
    }
}

bool cverify()
{
    bool authenticated = false;
    int attempts = 0;

    do
    {
        if (attempts > 0)
        {
            cout << BOLD << RED << "Wrong credentials! Please try again." << RESET << endl;
            logActivity("Customer login failed attempt");
        }

        char *cname = new char[PASS_LEN];
        char *cpassword = new char[PASS_LEN];

        cout << BOLD << CYAN << "Enter Your Name: " << RESET;
        cin.getline(cname, PASS_LEN);
        cout << BOLD << CYAN << "Enter Your Password: " << RESET;
        cin.getline(cpassword, PASS_LEN);

        encryptPassword(cpassword);

        ifstream read("customer.txt");
        if (!read)
        {
            cout << BOLD << RED << "Error: Could not open customer.txt!" << RESET << endl;
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
            logActivity("Customer multiple failed login attempts - SECURITY ALERT");
        }

    } while (!authenticated);

    return authenticated;
}

void cregistration()
{
    char *rname = new char[PASS_LEN];
    char *rpassword = new char[PASS_LEN];

    cout << BOLD << CYAN << "Enter your Name: " << RESET;
    cin.getline(rname, PASS_LEN, '\n');
    cout << BOLD << CYAN << "Enter your Password: " << RESET;
    cin.getline(rpassword, PASS_LEN, '\n');

    encryptPassword(rpassword);

    fstream write("customer.txt", ios::app);
    if (!write.is_open())
    {
        cout << BOLD << RED << "Error: Could not open customer.txt!" << RESET << endl;
        delete[] rname;
        delete[] rpassword;
        return;
    }

    write << rname << '|' << rpassword << '|' << endl;
    cout << BOLD << GREEN << rname << " Registered Successfully!" << RESET << endl;
    write.close();

    logActivity("New customer registered");

    delete[] rname;
    delete[] rpassword;
}

void displayproducts()
{
    displayCatalogue();
    searchproducts();
}

void searchproducts()
{
    cout << endl;
    cout << CYAN;
    displayLine(50, '=');
    cout << RESET;
    cout << BOLD << MAGENTA << "                CUSTOMER MENU" << RESET << endl;
    cout << CYAN;
    displayLine(50, '=');
    cout << RESET;

    cout << GREEN   << "1. Buy Products" << RESET << endl;
    cout << BLUE    << "2. Wishlist" << RESET << endl;
    cout << YELLOW  << "3. View Order History" << RESET << endl;
    cout << CYAN    << "4. Support and Feedback" << RESET << endl;
    cout << RED     << "5. Exit" << RESET << endl;
    cout << BOLD << YELLOW << "Enter your choice: " << RESET;

    int choice = getValidInt(1, 5);

    if (choice == 1)
    {
        cout << endl;
        cout << BOLD << WHITE << "Browse products by:" << RESET << endl;
        cout << GREEN << "1. Category" << RESET << endl;
        cout << BLUE << "2. Price Range" << RESET << endl;
        cout << MAGENTA << "3. Availability" << RESET << endl;
        cout << CYAN << "4. Direct Product ID" << RESET << endl;
        cout << BOLD << YELLOW << "Enter your choice: " << RESET;

        int choose = getValidInt(1, 4);

        if (choose == 1)
        {
            char *price = new char[ID_LEN];
            char *p_ID = new char[NAME_LEN];
            char *name = new char[NAME_LEN];
            char *category = new char[NAME_LEN];
            char *quantity = new char[NAME_LEN];
            char *usersearch = new char[NAME_LEN];

            fstream read("catalogue.txt", ios::in);
            cout << BOLD << CYAN << "Enter category to browse: " << RESET;
            cin.getline(usersearch, NAME_LEN, '\n');

            if (read.is_open())
            {
                cout << CYAN;
                displayLine(80, '-');
                cout << RESET;

                bool found = false;
                while (read.getline(price, NAME_LEN, '|'))
                {
                    read.getline(p_ID, NAME_LEN, '|');
                    read.getline(name, NAME_LEN, '|');
                    read.getline(category, NAME_LEN, '|');
                    read.getline(quantity, NAME_LEN, '\n');

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
                    cout << BOLD << RED << "No products found in this category." << RESET << endl;
                }

                cout << CYAN;
                displayLine(80, '-');
                cout << RESET;

                customerAddToCart();
            }
            else
            {
                cout << BOLD << RED << "Error: Could not open file!" << RESET << endl;
            }
            read.close();

            delete[] price;
            delete[] p_ID;
            delete[] name;
            delete[] category;
            delete[] quantity;
            delete[] usersearch;
        }

        if (choose == 2)
        {
            int price;
            int usersearch;
            char *p_ID = new char[NAME_LEN];
            char *name = new char[NAME_LEN];
            char *category = new char[NAME_LEN];
            char *quantity = new char[NAME_LEN];

            fstream read("catalogue.txt", ios::in);
            cout << BOLD << CYAN << "Enter upper price range: " << RESET;
            cin >> usersearch;
            cin.ignore();

            if (read.is_open())
            {
                cout << CYAN;
                displayLine(80, '-');
                cout << RESET;

                bool found = false;
                while (read >> price)
                {
                    read.ignore(1, '|');
                    read.getline(p_ID, NAME_LEN, '|');
                    read.getline(name, NAME_LEN, '|');
                    read.getline(category, NAME_LEN, '|');
                    read.getline(quantity, NAME_LEN, '\n');

                    if (price < usersearch)
                    {
                        cout << left << setw(25) << name << "$" << setw(10) << price
                             << setw(20) << category << setw(10) << p_ID
                             << setw(10) << quantity << endl;
                        found = true;
                    }
                }
                if (!found)
                {
                    cout << BOLD << RED << "No products found in this price range." << RESET << endl;
                }

                cout << CYAN;
                displayLine(80, '-');
                cout << RESET;

                customerAddToCart();
            }
            else
            {
                cout << BOLD << RED << "Error: Could not open file!" << RESET << endl;
            }
            read.close();

            delete[] p_ID;
            delete[] name;
            delete[] category;
            delete[] quantity;
        }

        if (choose == 3)
        {
            int price;
            int usersearch;
            char *p_ID = new char[NAME_LEN];
            char *name = new char[NAME_LEN];
            char *category = new char[NAME_LEN];
            int quantity;

            fstream read("catalogue.txt", ios::in);
            cout << BOLD << CYAN << "Enter minimum required quantity: " << RESET;
            cin >> usersearch;
            cin.ignore();

            if (read.is_open())
            {
                cout << CYAN;
                displayLine(80, '-');
                cout << RESET;

                bool found = false;
                while (read >> price)
                {
                    read.ignore(1, '|');
                    read.getline(p_ID, NAME_LEN, '|');
                    read.getline(name, NAME_LEN, '|');
                    read.getline(category, NAME_LEN, '|');
                    read >> quantity;
                    read.ignore();

                    if (quantity > usersearch)
                    {
                        cout << left << setw(25) << name << "$" << setw(10) << price
                             << setw(20) << category << setw(10) << p_ID
                             << setw(10) << quantity << endl;
                        found = true;
                    }
                }
                if (!found)
                {
                    cout << BOLD << RED << "No products found with this quantity." << RESET << endl;
                }

                cout << CYAN;
                displayLine(80, '-');
                cout << RESET;

                customerAddToCart();
            }
            else
            {
                cout << BOLD << RED << "Error: Could not open file!" << RESET << endl;
            }
            read.close();

            delete[] p_ID;
            delete[] name;
            delete[] category;
        }

        if (choose == 4)
        {
            customerAddToCart();
        }
    }
    else if (choice == 2)
    {
        addtowishlist();
    }
    else if (choice == 3)
    {
        vieworder();
    }
    else if (choice == 4)
    {
        cout << BOLD << WHITE << "1. Submit Feedback" << RESET << endl;
        cout << BOLD << WHITE << "2. Submit Support Request" << RESET << endl;
        cout << BOLD << YELLOW << "Enter your choice: " << RESET;
        int decision = getValidInt(1, 2);

        if (decision == 1)
        {
            Feedback();
        }
        else if (decision == 2)
        {
            Support();
        }
    }
    else if (choice == 5)
    {
        cout << BOLD << RED << "Exiting SecureShop. Thank you!" << RESET << endl;
        return;
    }
}

void customerAddToCart()
{
    int price;
    char *p_ID = new char[NAME_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int quantity;

    cout << BOLD << CYAN << "Enter Product ID: " << RESET;
    char *UPID = new char[NAME_LEN];
    cin.getline(UPID, NAME_LEN, '\n');

    cout << BOLD << CYAN << "Enter Quantity: " << RESET;
    int reqquantity;
    cin >> reqquantity;
    cin.ignore();

    fstream read("catalogue.txt", ios::in);
    fstream write("cart.txt", ios::app);
    bool found = false;

    if (read.is_open())
    {
        while (read >> price)
        {
            read.ignore(1, '|');
            read.getline(p_ID, NAME_LEN, '|');
            read.getline(name, NAME_LEN, '|');
            read.getline(category, NAME_LEN, '|');
            read >> quantity;
            read.ignore();

            if (compareStrings(UPID, p_ID))
            {
                found = true;
                if (reqquantity > quantity)
                {
                    cout << BOLD << RED << "Not enough stock! Only " << quantity << " available." << RESET << endl;
                }
                else
                {
                    write << p_ID << "|" << name << "|" << reqquantity << "|" << price << endl;
                    cout << BOLD << GREEN << name << " added to cart!" << RESET << endl;
                }
                break;
            }
        }
        if (!found)
        {
            cout << BOLD << RED << "Product ID not found in catalogue." << RESET << endl;
        }
    }
    else
    {
        cout << BOLD << RED << "Error: Could not open catalogue.txt!" << RESET << endl;
    }

    read.close();
    write.close();

    delete[] p_ID;
    delete[] name;
    delete[] category;
    delete[] UPID;

    displaycart();
}

void customerRemoveFromCart()
{
    char *removeID = new char[NAME_LEN];
    cout << BOLD << CYAN << "Enter Product ID to remove from cart: " << RESET;
    cin.getline(removeID, NAME_LEN);

    ifstream readCart("cart.txt");
    ofstream tempFile("temp_cart.txt");

    if (!readCart.is_open())
    {
        cout << BOLD << RED << "Cart is empty or file could not be opened!" << RESET << endl;
        delete[] removeID;
        return;
    }

    char *UPID = new char[NAME_LEN];
    char *name = new char[NAME_LEN];
    int quantity, price;
    bool found = false;

    while (readCart.getline(UPID, NAME_LEN, '|'))
    {
        readCart.getline(name, NAME_LEN, '|');
        readCart >> quantity;
        readCart.ignore(1, '|');
        readCart >> price;
        readCart.ignore();

        if (compareStrings(UPID, removeID))
        {
            found = true;
            cout << BOLD << GREEN << name << " removed from cart!" << RESET << endl;
            continue;
        }
        tempFile << UPID << "|" << name << "|" << quantity << "|" << price << endl;
    }

    readCart.close();
    tempFile.close();

    if (found)
    {
        remove("cart.txt");
        rename("temp_cart.txt", "cart.txt");
    }
    else
    {
        remove("temp_cart.txt");
        cout << BOLD << RED << "Product ID not found in cart." << RESET << endl;
    }

    delete[] removeID;
    delete[] UPID;
    delete[] name;
}

void customerUpdateCart()
{
    char *updateID = new char[NAME_LEN];
    cout << BOLD << CYAN << "Enter Product ID to update quantity: " << RESET;
    cin.getline(updateID, NAME_LEN);

    cout << BOLD << CYAN << "Enter new quantity: " << RESET;
    int newQty;
    cin >> newQty;
    cin.ignore();

    ifstream readCart("cart.txt");
    ofstream tempFile("temp_cart.txt");

    if (!readCart.is_open())
    {
        cout << BOLD << RED << "Cart is empty or file could not be opened!" << RESET << endl;
        delete[] updateID;
        return;
    }

    char *UPID = new char[NAME_LEN];
    char *name = new char[NAME_LEN];
    int quantity, price;
    bool found = false;

    while (readCart.getline(UPID, NAME_LEN, '|'))
    {
        readCart.getline(name, NAME_LEN, '|');
        readCart >> quantity;
        readCart.ignore(1, '|');
        readCart >> price;
        readCart.ignore();

        if (compareStrings(UPID, updateID))
        {
            found = true;
            tempFile << UPID << "|" << name << "|" << newQty << "|" << price << endl;
            cout << BOLD << GREEN << name << " quantity updated to " << newQty << "!" << RESET << endl;
        }
        else
        {
            tempFile << UPID << "|" << name << "|" << quantity << "|" << price << endl;
        }
    }

    readCart.close();
    tempFile.close();

    if (found)
    {
        remove("cart.txt");
        rename("temp_cart.txt", "cart.txt");
    }
    else
    {
        remove("temp_cart.txt");
        cout << BOLD << RED << "Product ID not found in cart." << RESET << endl;
    }

    delete[] updateID;
    delete[] UPID;
    delete[] name;
}

void displaycart()
{
    cout << endl;
    cout << CYAN;
    displayLine(45, '=');
    cout << RESET;
    cout << BOLD << MAGENTA << "                 CART MENU" << RESET << endl;
    cout << CYAN;
    displayLine(45, '=');
    cout << RESET;

    cout << GREEN   << "1. Add more products?" << RESET << endl;
    cout << BLUE    << "2. View Cart?" << RESET << endl;
    cout << YELLOW  << "3. Remove item from cart?" << RESET << endl;
    cout << CYAN    << "4. Update item quantity?" << RESET << endl;
    cout << MAGENTA << "5. Proceed to Checkout?" << RESET << endl;
    cout << BOLD << YELLOW << "Enter your choice: " << RESET;

    int a = getValidInt(1, 5);

    if (a == 1)
    {
        customerAddToCart();
        return;
    }
    else if (a == 3)
    {
        customerRemoveFromCart();
        displaycart();
        return;
    }
    else if (a == 4)
    {
        customerUpdateCart();
        displaycart();
        return;
    }

    if (a == 2 || a == 5)
    {
        int price;
        char *name = new char[NAME_LEN];
        int quantity;
        char *UPID = new char[NAME_LEN];

        fstream read("cart.txt", ios::in);
        cout << endl;
        cout << CYAN;
        displayLine(70, '=');
        cout << RESET;
        cout << BOLD << BLUE << "                     YOUR CART" << RESET << endl;
        cout << CYAN;
        displayLine(70, '=');
        cout << RESET;

        cout << left << setw(25) << "PRODUCT" << "$" << setw(10) << "PRICE"
             << setw(12) << "QUANTITY" << setw(10) << "ID" << endl;
        cout << CYAN;
        displayLine(70, '-');
        cout << RESET;

        int total = 0;
        if (read.is_open())
        {
            while (read.getline(UPID, NAME_LEN, '|'))
            {
                read.getline(name, NAME_LEN, '|');
                read >> quantity;
                read.ignore(1, '|');
                read >> price;
                read.ignore();
                cout << left << setw(25) << name << "$" << setw(10) << price
                     << setw(12) << quantity << setw(10) << UPID << endl;
                total += (price * quantity);
            }
        }
        read.close();

        cout << CYAN;
        displayLine(70, '-');
        cout << RESET;
        cout << BOLD << GREEN << "Cart Total: $" << total << RESET << endl;
        cout << CYAN;
        displayLine(70, '=');
        cout << RESET;

        delete[] name;
        delete[] UPID;

        if (a == 5)
        {
            checkout();
        }
    }
}

void addtowishlist()
{
    char *rname = new char[PASS_LEN];
    cout << BOLD << CYAN << "Enter Your Name: " << RESET;
    cin.getline(rname, PASS_LEN, '\n');

    cout << endl;
    cout << BOLD << WHITE << "1. Add Product to Wishlist" << RESET << endl;
    cout << BOLD << WHITE << "2. View Wishlist" << RESET << endl;
    cout << BOLD << YELLOW << "Enter your choice: " << RESET;

    int a = getValidInt(1, 2);

    if (a == 1)
    {
        bool addMore = true;
        while (addMore)
        {
            char *price = new char[ID_LEN];
            char *p_ID = new char[NAME_LEN];
            char *name = new char[NAME_LEN];
            char *category = new char[NAME_LEN];
            char *quantity = new char[NAME_LEN];
            char *UPID = new char[NAME_LEN];

            cout << BOLD << CYAN << "Enter Product ID: " << RESET;
            cin.getline(UPID, NAME_LEN, '\n');

            fstream read("catalogue.txt", ios::in);
            fstream write("wishlist.txt", ios::app);
            bool found = false;

            if (read.is_open())
            {
                while (read.getline(price, ID_LEN, '|'))
                {
                    read.getline(p_ID, NAME_LEN, '|');
                    read.getline(name, NAME_LEN, '|');
                    read.getline(category, NAME_LEN, '|');
                    read.getline(quantity, NAME_LEN, '\n');

                    if (compareStrings(UPID, p_ID))
                    {
                        write << UPID << "|" << name << "|" << rname << endl;
                        cout << BOLD << GREEN << name << " added to wishlist!" << RESET << endl;
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << BOLD << RED << "Product ID not found." << RESET << endl;
                }
            }
            write.close();
            read.close();

            delete[] price;
            delete[] p_ID;
            delete[] name;
            delete[] category;
            delete[] quantity;
            delete[] UPID;

            cout << BOLD << WHITE << "Add more products to wishlist?" << RESET << endl;
            cout << GREEN << "1. Yes" << RESET << endl;
            cout << RED << "2. No" << RESET << endl;
            cout << BOLD << YELLOW << "Enter your choice: " << RESET;
            int more = getValidInt(1, 2);
            if (more == 2)
            {
                addMore = false;
                cout << BOLD << GREEN << "Products saved to wishlist!" << RESET << endl;
            }
        }
    }
    else if (a == 2)
    {
        char *UPID = new char[NAME_LEN];
        char *productName = new char[NAME_LEN];
        char *ownerName = new char[NAME_LEN];

        fstream read("wishlist.txt", ios::in);
        cout << endl;
        cout << CYAN;
        displayLine(60, '=');
        cout << RESET;
        cout << BOLD << MAGENTA << "                  YOUR WISHLIST" << RESET << endl;
        cout << CYAN;
        displayLine(60, '=');
        cout << RESET;

        cout << left << setw(15) << "ID" << setw(25) << "Product" << setw(20) << "Customer" << endl;
        cout << CYAN;
        displayLine(60, '-');
        cout << RESET;

        bool found = false;
        if (read.is_open())
        {
            while (read.getline(UPID, NAME_LEN, '|'))
            {
                read.getline(productName, NAME_LEN, '|');
                read.getline(ownerName, NAME_LEN, '\n');

                if (compareStrings(rname, ownerName))
                {
                    cout << left << setw(15) << UPID << setw(25) << productName
                         << setw(20) << ownerName << endl;
                    found = true;
                }
            }
        }
        if (!found)
        {
            cout << BOLD << RED << "Your wishlist is empty." << RESET << endl;
        }

        cout << CYAN;
        displayLine(60, '=');
        cout << RESET;

        read.close();

        delete[] UPID;
        delete[] productName;
        delete[] ownerName;
    }

    delete[] rname;
}

void checkout()
{
    int price;
    char *name = new char[NAME_LEN];
    int quantity;
    char *UPID = new char[NAME_LEN];

    fstream read1("cart.txt", ios::in);

    cout << endl;
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;
    cout << BOLD << GREEN << "                     CHECKOUT" << RESET << endl;
    cout << CYAN;
    displayLine(70, '=');
    cout << RESET;

    cout << left << setw(25) << "PRODUCT" << "$" << setw(10) << "PRICE"
         << setw(12) << "QUANTITY" << setw(10) << "ID" << endl;
    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    int ttl = 0;

    if (!read1.is_open())
    {
        cout << BOLD << RED << "Cart is empty!" << RESET << endl;
        delete[] name;
        delete[] UPID;
        return;
    }

    while (read1.getline(UPID, NAME_LEN, '|'))
    {
        read1.getline(name, NAME_LEN, '|');
        read1 >> quantity;
        read1.ignore(1, '|');
        read1 >> price;
        read1.ignore();
        cout << left << setw(25) << name << "$" << setw(10) << price
             << setw(12) << quantity << setw(10) << UPID << endl;
        ttl += (price * quantity);
    }
    read1.close();

    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;
    cout << BOLD << GREEN << "Total Amount: $" << ttl << RESET << endl;
    cout << CYAN;
    displayLine(70, '-');
    cout << RESET;

    cout << BOLD << WHITE << "Do you want to apply a Discount Code?" << RESET << endl;
    cout << GREEN << "1. Yes" << RESET << endl;
    cout << RED << "2. No" << RESET << endl;
    cout << BOLD << YELLOW << "Enter your choice: " << RESET;

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
            cout << CYAN;
            displayLine(30, '=');
            cout << RESET;
            cout << BOLD << YELLOW << "      AVAILABLE DISCOUNTS" << RESET << endl;
            cout << CYAN;
            displayLine(30, '=');
            cout << RESET;

            while (readDisc.getline(code, ID_LEN, '|'))
            {
                readDisc >> discount;
                readDisc.ignore();
                cout << "  " << code << " -> " << discount << "% OFF" << endl;
            }

            cout << CYAN;
            displayLine(30, '=');
            cout << RESET;
            readDisc.close();
        }

        cout << BOLD << CYAN << "Enter Discount Code: " << RESET;
        char *ucode = new char[ID_LEN];
        cin.getline(ucode, ID_LEN, '\n');

        fstream readDisc2("discount.txt", ios::in);
        bool discountApplied = false;

        if (readDisc2.is_open())
        {
            float apdiscount;
            while (readDisc2.getline(code, ID_LEN, '|'))
            {
                readDisc2 >> apdiscount;
                readDisc2.ignore();

                if (compareStrings(ucode, code))
                {
                    float percentage = ((100.0 - apdiscount) / 100.0);
                    finalTotal = ttl * percentage;
                    cout << BOLD << GREEN << "Discount applied! New Total: $" << finalTotal << RESET << endl;
                    discountApplied = true;
                    break;
                }
            }
            readDisc2.close();
        }

        if (!discountApplied)
        {
            cout << BOLD << RED << "Invalid Discount Code! Total: $" << ttl << RESET << endl;
            finalTotal = ttl;
        }

        delete[] code;
        delete[] ucode;
    }
    else
    {
        cout << BOLD << GREEN << "Your Total: $" << ttl << RESET << endl;
    }

    cout << endl;
    cout << BOLD << WHITE << "Confirm Checkout?" << RESET << endl;
    cout << GREEN << "1. Yes, Confirm" << RESET << endl;
    cout << RED << "2. No, Cancel" << RESET << endl;
    cout << BOLD << YELLOW << "Enter your choice: " << RESET;

    int f = getValidInt(1, 2);

    if (f == 1)
    {
        fstream write("orderhistory.txt", ios::app);
        if (!write.is_open())
        {
            cout << BOLD << RED << "Error: Could not open orderhistory.txt!" << RESET << endl;
            delete[] name;
            delete[] UPID;
            return;
        }

        cout << BOLD << CYAN << "Enter Your Name (for order record): " << RESET;
        char *username = new char[NAME_LEN];
        cin.getline(username, NAME_LEN);

        fstream read2("cart.txt", ios::in);
        if (read2.is_open())
        {
            while (read2.getline(UPID, NAME_LEN, '|'))
            {
                read2.getline(name, NAME_LEN, '|');
                read2 >> quantity;
                read2.ignore(1, '|');
                read2 >> price;
                read2.ignore();
                write << username << "|" << name << "|" << price << "|" << UPID << "|" << quantity << endl;
            }
            read2.close();
        }

        write.close();

        remove("cart.txt");
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
    }
    else
    {
        cout << BOLD << RED << "Checkout cancelled." << RESET << endl;
    }

    delete[] name;
    delete[] UPID;
}

void vieworder()
{
    char *username = new char[NAME_LEN];
    cout << BOLD << CYAN << "Enter Your Name: " << RESET;
    cin.getline(username, NAME_LEN);

    char *readname = new char[NAME_LEN];
    char *readproduct = new char[NAME_LEN];
    int readprice;
    char *readp_ID = new char[ID_LEN];
    int readquantity;

    fstream read("orderhistory.txt", ios::in);

    cout << endl;
    cout << CYAN;
    displayLine(60, '=');
    cout << RESET;
    cout << BOLD << BLUE << "                  ORDER HISTORY" << RESET << endl;
    cout << CYAN;
    displayLine(60, '=');
    cout << RESET;

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
            read.ignore();

            if (compareStrings(username, readname))
            {
                cout << left << setw(25) << readproduct << "$" << setw(10) << readprice
                     << setw(12) << readquantity << setw(10) << readp_ID << endl;
                found = true;
            }
        }
    }

    if (!found)
    {
        cout << BOLD << RED << "No order history found." << RESET << endl;
    }

    cout << CYAN;
    displayLine(60, '=');
    cout << RESET;
    read.close();

    delete[] username;
    delete[] readname;
    delete[] readproduct;
    delete[] readp_ID;
}

void Feedback()
{
    char *feedname = new char[NAME_LEN];
    char *reqID = new char[ID_LEN];
    char *feedback = new char[TEXT_LEN];

    cout << BOLD << CYAN << "Enter Your Name: " << RESET;
    cin.getline(feedname, NAME_LEN, '\n');
    cout << BOLD << CYAN << "Enter Request ID (unique): " << RESET;
    cin.getline(reqID, ID_LEN, '\n');

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
                    cout << BOLD << RED << "This Request ID already exists! Enter a different one: " << RESET;
                    cin.getline(reqID, ID_LEN, '\n');
                    break;
                }
            }
            read.close();
        }

        delete[] readreqID;
        delete[] tempName;
        delete[] tempText;
    }

    cout << BOLD << CYAN << "Enter Your Feedback: " << RESET;
    cin.getline(feedback, TEXT_LEN, '\n');

    fstream write("feedback.txt", ios::app);
    if (write.is_open())
    {
        write << reqID << "|" << feedname << "|" << feedback << endl;
        cout << BOLD << GREEN << "Feedback submitted successfully!" << RESET << endl;
        write.close();
    }

    logActivity("Customer submitted feedback");

    delete[] feedname;
    delete[] reqID;
    delete[] feedback;
}

void Support()
{
    char *Sname = new char[NAME_LEN];
    char *SID = new char[ID_LEN];
    char *Supreq = new char[TEXT_LEN];

    cout << BOLD << CYAN << "Enter Your Name: " << RESET;
    cin.getline(Sname, NAME_LEN, '\n');
    cout << BOLD << CYAN << "Enter Support ID (unique): " << RESET;
    cin.getline(SID, ID_LEN, '\n');

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
                    cout << BOLD << RED << "This Support ID already exists! Enter a different one: " << RESET;
                    cin.getline(SID, ID_LEN, '\n');
                    break;
                }
            }
            read.close();
        }

        delete[] readSID;
        delete[] tempName;
        delete[] tempText;
    }

    cout << BOLD << CYAN << "Enter Your Support Request: " << RESET;
    cin.getline(Supreq, TEXT_LEN, '\n');

    fstream write("Support.txt", ios::app);
    if (write.is_open())
    {
        write << SID << "|" << Sname << "|" << Supreq << endl;
        cout << BOLD << GREEN << "Support request submitted successfully!" << RESET << endl;
        write.close();
    }

    logActivity("Customer submitted support request");

    delete[] Sname;
    delete[] SID;
    delete[] Supreq;
}
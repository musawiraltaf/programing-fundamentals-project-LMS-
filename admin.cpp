// ====================== ADMIN.CPP ======================
// This file implements all functionality of the admin module
// The admin has the highest level of access and control in the system
// Features: User management, inventory, discounts, 2FA (OTP),
// activity monitoring, sales analysis, audit trail
// Cybersecurity concepts: 2FA, access control, audit trail, monitoring
// Dynamic memory allocation and pointers are being used (PF requirement)
// ===========================================================

#include "admin.h"

// ===================== amenu =====================
// Main menu function for the admin
// The admin logs in (password + OTP 2FA) and then gets access to the admin panel
// Cybersecurity: Two-Factor Authentication (2FA) - an extra security layer
void amenu()
{
    // Display welcome banner
    displayLine(70, '=');
    cout << "          SECURESHOP - ADMIN PORTAL" << endl;
    displayLine(70, '=');
    cout << endl;

    // First verify the password
    if (averify())
    {
        // ===== TWO-FACTOR AUTHENTICATION (2FA) =====
        // Generate OTP - cybersecurity bonus feature
        // Use srand to set a random seed so a different OTP appears each time
        srand(time(0));
        int otp = 100000 + (rand() % 900000); // Generate a 6-digit OTP

        // Save OTP in a file (simulate sending to phone/email)
        fstream gen("otp.txt", ios::out); // Overwrite old OTP
        gen << otp;
        gen.close();

        // Show OTP to the user (simulation - in reality it would be sent via SMS/email)
        cout << endl;
        displayLine(40, '*');
        cout << "  Generated OTP: " << otp << endl;
        cout << "  (This OTP is being shown only for simulation)" << endl;
        displayLine(40, '*');

        // Take OTP input from user
        cout << "Enter OTP: ";
        int userotp;
        cin >> userotp;
        cin.ignore();

        // Verify OTP
        bool otpValid = false;
        fstream readOtp("otp.txt", ios::in);
        int storedOtp;

        if (readOtp.is_open())
        {
            readOtp >> storedOtp;
            // Match the user OTP with the stored OTP
            if (storedOtp == userotp)
            {
                otpValid = true;
            }
            readOtp.close();
        }

        if (otpValid)
        {
            // 2FA successful - give access to admin panel
            cout << endl;
            displayLine(70, '=');
            cout << "          LOGIN + 2FA SUCCESSFUL - Welcome Admin!" << endl;
            displayLine(70, '=');

            logActivity("Admin login successful (2FA verified)");
            logAudit("Admin login - 2FA authentication passed");

            // Main admin menu loop
            // Keep showing the menu until the admin exits
            while (true)
            {
                cout << endl;
                displayLine(55, '-');
                cout << "     ADMIN CONTROL PANEL" << endl;
                displayLine(55, '-');
                cout << "1. Manage Users (Add/Delete)" << endl;
                cout << "2. Inventory and Bulk Operations" << endl;
                cout << "3. Discounts and Promotions" << endl;
                cout << "4. Activity Logs and Security Monitoring" << endl;
                cout << "5. Sales and Revenue Analysis" << endl;
                cout << "6. View Audit Trail" << endl;
                cout << "7. Send Announcement" << endl;
                cout << "8. Exit Menu" << endl;
                cout << "Enter your choice: ";

                int achoice = getValidInt(1, 8);

                if (achoice == 1)
                {
                    // User management (add/delete customers/employees)
                    mnguser();
                }
                else if (achoice == 2)
                {
                    // Inventory and bulk operations sub-menu
                    cout << endl;
                    cout << "1. Manage Inventory (Add/Edit/Delete Products)" << endl;
                    cout << "2. Bulk Import & Export" << endl;
                    cout << "Enter your choice: ";

                    int adchoice = getValidInt(1, 2);

                    if (adchoice == 1)
                    {
                        // Shared catalogue management function (from utils)
                        catalogueManage();
                    }
                    else if (adchoice == 2)
                    {
                        // Bulk import/export operations
                        mngbulk();
                    }
                }
                else if (achoice == 3)
                {
                    // Create discount codes
                    mngdiscs();
                }
                else if (achoice == 4)
                {
                    // BUG FIX: Function to view activity logs
                    // Previously the file was opened with ios::out which erased all data
                    viewActivityLogs();
                }
                else if (achoice == 5)
                {
                    // Sales and revenue analysis
                    revenue();
                }
                else if (achoice == 6)
                {
                    // Function to view audit trail (new feature)
                    viewAuditTrail();
                }
                else if (achoice == 7)
                {
                    // Send announcement
                    sendAnnouncement();
                    logAudit("Admin sent announcement");
                }
                else if (achoice == 8)
                {
                    // Exit admin panel
                    cout << "Exiting admin panel." << endl;
                    logActivity("Admin logged out");
                    logAudit("Admin logged out");
                    break;
                }
            }
        }
        else
        {
            // OTP was incorrect - access denied
            cout << "Incorrect OTP! Access DENIED." << endl;
            logActivity("Admin login failed - wrong OTP (2FA failed)");
            logAudit("SECURITY: Admin 2FA failed - unauthorized access attempt");
        }
    }
}

// ===================== averify =====================
// Verifies the admin's login credentials
// Matches the name and encrypted password from the file
// Cybersecurity: authentication - first layer of 2FA
bool averify()
{
    bool authenticated = false;
    int attempts = 0;

    do
    {
        if (attempts > 0)
        {
            cout << "Invalid credentials! Please try again." << endl;
            logActivity("Admin login failed attempt");
        }

        // Dynamic memory allocation (PF requirement: new operator)
        char *aname = new char[PASS_LEN];
        char *apassword = new char[PASS_LEN];

        // Take credentials from admin
        cout << "Enter Admin Name: ";
        cin.getline(aname, PASS_LEN);
        cout << "Enter Admin Password: ";
        cin.getline(apassword, PASS_LEN);

        // Encrypt password (shared function - no duplication)
        encryptPassword(apassword);

        // Read credentials from admin file
        ifstream read("admin.txt");
        if (!read)
        {
            cout << "Error: Could not open admin.txt!" << endl;
            delete[] aname;
            delete[] apassword;
            return false;
        }

        char *fname = new char[PASS_LEN];
        char *fpassword = new char[PASS_LEN];

        // Check every record
        while (read.getline(fname, PASS_LEN, '|'))
        {
            read.getline(fpassword, PASS_LEN, '|');

            // Shared compareStrings function (no duplication)
            if (compareStrings(aname, fname) && compareStrings(apassword, fpassword))
            {
                authenticated = true;
                break;
            }
        }
        read.close();

        // Free dynamic memory (PF requirement: delete operator)
        delete[] aname;
        delete[] apassword;
        delete[] fname;
        delete[] fpassword;

        attempts++;

        if (!authenticated && attempts >= 3)
        {
            cout << "SECURITY WARNING: " << attempts << " invalid attempts!" << endl;
            logActivity("Admin multiple failed login attempts - HIGH SECURITY ALERT");
        }

    } while (!authenticated);

    return authenticated;
}

// ===================== addcustomer =====================
// Admin can add a new customer into the system
// The password is saved after encryption (cybersecurity)
void addcustomer()
{
    // Dynamic memory allocation
    char *fname = new char[PASS_LEN];
    char *fpassword = new char[PASS_LEN];

    cout << "Enter Customer Name: ";
    cin.getline(fname, PASS_LEN);
    cout << "Enter Customer Password: ";
    cin.getline(fpassword, PASS_LEN);

    // Encrypt password (shared function)
    encryptPassword(fpassword);

    // Save in customer file
    ofstream outfile("customer.txt", ios::app);
    if (!outfile.is_open())
    {
        cout << "Error: Could not open customer.txt!" << endl;
        delete[] fname;
        delete[] fpassword;
        return;
    }

    outfile << fname << "|" << fpassword << "|" << endl;
    outfile.close();
    cout << "Customer added successfully!" << endl;

    // Record in audit trail
    logAudit("Admin added new customer");
    logActivity("Admin added new customer");

    delete[] fname;
    delete[] fpassword;
}

// ===================== addemployee =====================
// Admin can add a new employee
// Same process as addcustomer but saved in employee file
void addemployee()
{
    // Dynamic memory allocation
    char *fname = new char[PASS_LEN];
    char *fpassword = new char[PASS_LEN];

    cout << "Enter Employee Name: ";
    cin.getline(fname, PASS_LEN);
    cout << "Enter Employee Password: ";
    cin.getline(fpassword, PASS_LEN);

    // Encrypt password
    encryptPassword(fpassword);

    // Save in employee file
    ofstream outfile("employee.txt", ios::app);
    if (!outfile.is_open())
    {
        cout << "Error: Could not open employee.txt!" << endl;
        delete[] fname;
        delete[] fpassword;
        return;
    }

    outfile << fname << "|" << fpassword << "|" << endl;
    outfile.close();
    cout << "Employee added successfully!" << endl;

    logAudit("Admin added new employee");
    logActivity("Admin added new employee");

    delete[] fname;
    delete[] fpassword;
}

// ===================== deletecustomer =====================
// Admin can delete a customer from the system
// Removes the matching customer entry by name
// Uses the temp file technique for safe deletion
void deletecustomer()
{
    // Dynamic memory allocation
    char *fname = new char[PASS_LEN];
    cout << "Enter the Customer Name to delete: ";
    cin.getline(fname, PASS_LEN);

    char *line = new char[LINE_LEN];
    ifstream infile("customer.txt");
    ofstream outfile("temp.txt");

    if (!infile.is_open() || !outfile.is_open())
    {
        cout << "Error: Could not open files!" << endl;
        delete[] fname;
        delete[] line;
        return;
    }

    bool found = false;
    // Read every line from the customer file
    while (infile.getline(line, LINE_LEN))
    {
        // If the line starts with the customer name, skip it (delete)
        if (compareStrings(line, fname))
        {
            found = true;
            continue; // Do not write this record into temp
        }
        // Preserve the remaining records in temp
        outfile << line << endl;
    }

    infile.close();
    outfile.close();

    if (found)
    {
        remove("customer.txt");
        rename("temp.txt", "customer.txt");
        cout << "Customer deleted successfully!" << endl;
        logAudit("Admin deleted customer");
    }
    else
    {
        remove("temp.txt");
        cout << "Customer not found." << endl;
    }

    delete[] fname;
    delete[] line;
}

// ===================== deleteemployee =====================
// Admin can delete an employee from the system
// BUG FIX: Previously the compareStrings logic inside the delete function
// was reversed - now it has been fixed properly
void deleteemployee()
{
    // Dynamic memory allocation
    char *fname = new char[PASS_LEN];
    cout << "Enter the Employee Name to delete: ";
    cin.getline(fname, PASS_LEN);

    char *line = new char[LINE_LEN];
    ifstream infile("employee.txt");
    ofstream outfile("temp.txt");

    if (!infile.is_open() || !outfile.is_open())
    {
        cout << "Error: Could not open files!" << endl;
        delete[] fname;
        delete[] line;
        return;
    }

    bool found = false;
    while (infile.getline(line, LINE_LEN))
    {
        // BUG FIX: Use compareStrings properly
        // If the name matches, skip it (delete), otherwise keep it
        // Previously there was a prefix-only bug in the nested check
        // Extract the name using dynamic memory for comparison
        char *tempName = new char[PASS_LEN];
        int i = 0;
        // Extract the name from the line until the pipe
        while (line[i] != '|' && line[i] != '\0')
        {
            tempName[i] = line[i];
            i++;
        }
        tempName[i] = '\0'; // Null terminate

        if (compareStrings(tempName, fname))
        {
            found = true;
            delete[] tempName;
            continue; // Skip this record (delete)
        }
        // Write the remaining records into temp
        outfile << line << endl;
        delete[] tempName;
    }

    infile.close();
    outfile.close();

    if (found)
    {
        remove("employee.txt");
        rename("temp.txt", "employee.txt");
        cout << "Employee deleted successfully!" << endl;
        logAudit("Admin deleted employee");
    }
    else
    {
        remove("temp.txt");
        cout << "Employee not found." << endl;
    }

    delete[] fname;
    delete[] line;
}

// ===================== mnguser =====================
// User management menu - add/delete customers and employees
// Admin-only feature (access control)
void mnguser()
{
    cout << endl;
    displayLine(50, '=');
    cout << "     USER MANAGEMENT" << endl;
    displayLine(50, '=');
    cout << "1. Add New Customer" << endl;
    cout << "2. Add New Employee" << endl;
    cout << "3. Delete Customer" << endl;
    cout << "4. Delete Employee" << endl;
    cout << "Enter your choice: ";

    int mng = getValidInt(1, 4);

    if (mng == 1)
    {
        addcustomer();
    }
    else if (mng == 2)
    {
        addemployee();
    }
    else if (mng == 3)
    {
        deletecustomer();
    }
    else if (mng == 4)
    {
        deleteemployee();
    }
}

// ===================== exportextra =====================
// Exports products with extra stock into extra.txt
// Products with quantity 10 or more are exported
// Part of the admin's bulk management feature
void exportextra()
{
    // Dynamic memory allocation
    char *price = new char[NAME_LEN];
    char *p_ID = new char[NAME_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int quantity;

    fstream read("catalogue.txt", ios::in);
    if (!read.is_open())
    {
        cout << "Error: Could not open catalogue.txt!" << endl;
        delete[] price;
        delete[] p_ID;
        delete[] name;
        delete[] category;
        return;
    }

    // Show products with extra stock
    cout << endl;
    displayLine(70, '=');
    cout << "  Products with 10 or more items in stock:" << endl;
    displayLine(70, '=');
    cout << left << setw(25) << "Product" << "$" << setw(10) << "Price"
         << setw(20) << "Category" << setw(10) << "ID"
         << setw(10) << "Stock" << endl;
    displayLine(70, '-');

    while (read.getline(price, NAME_LEN, '|'))
    {
        read.getline(p_ID, NAME_LEN, '|');
        read.getline(name, NAME_LEN, '|');
        read.getline(category, NAME_LEN, '|');
        read >> quantity;
        read.ignore(1, '\n');

        // Show only items with 10+ stock
        if (quantity >= 10)
        {
            cout << left << setw(25) << name << "$" << setw(10) << price
                 << setw(20) << category << setw(10) << p_ID
                 << setw(10) << quantity << endl;
        }
    }
    read.close();
    displayLine(70, '-');

    // Ask user for confirmation before exporting
    cout << "Do you want to export extra items?" << endl;
    cout << "1. Yes" << endl;
    cout << "2. No" << endl;
    cout << "Enter your choice: ";

    int mng = getValidInt(1, 2);

    if (mng == 1)
    {
        // Export into extra.txt
        fstream write("extra.txt", ios::out);
        if (!write.is_open())
        {
            cout << "Error: Could not open extra.txt!" << endl;
            delete[] price;
            delete[] p_ID;
            delete[] name;
            delete[] category;
            return;
        }

        // Read catalogue again and export extra items
        read.open("catalogue.txt", ios::in);
        if (!read.is_open())
        {
            cout << "Error: Could not reopen catalogue.txt!" << endl;
            write.close();
            delete[] price;
            delete[] p_ID;
            delete[] name;
            delete[] category;
            return;
        }

        while (read.getline(price, NAME_LEN, '|'))
        {
            read.getline(p_ID, NAME_LEN, '|');
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
        cout << "Extra items exported successfully!" << endl;
        logAudit("Admin exported extra stock items");
    }
    else
    {
        cout << "Export cancelled." << endl;
    }

    // Free dynamic memory
    delete[] price;
    delete[] p_ID;
    delete[] name;
    delete[] category;
}

// ===================== importStock =====================
// Restocks low stock items (adds 10 pieces)
// The name was previously 'import' which was confusing, now it is 'importStock'
// The catalogue is updated using the temp file technique
void importStock()
{
    // Dynamic memory allocation
    char *price = new char[NAME_LEN];
    char *p_ID = new char[NAME_LEN];
    char *name = new char[NAME_LEN];
    char *category = new char[NAME_LEN];
    int quantity;

    fstream read("catalogue.txt", ios::in);
    if (!read.is_open())
    {
        cout << "Error: Could not open catalogue.txt!" << endl;
        delete[] price;
        delete[] p_ID;
        delete[] name;
        delete[] category;
        return;
    }

    // Show low stock products
    cout << endl;
    displayLine(70, '=');
    cout << "  Products with 5 or fewer items in stock (LOW STOCK):" << endl;
    displayLine(70, '=');
    cout << left << setw(25) << "Product" << "$" << setw(10) << "Price"
         << setw(20) << "Category" << setw(10) << "ID"
         << setw(10) << "Stock" << endl;
    displayLine(70, '-');

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
                 << setw(10) << quantity << " *** LOW ***" << endl;
        }
    }
    read.close();
    displayLine(70, '-');

    // Ask user for confirmation
    cout << "Do you want to add 10 pieces to these items?" << endl;
    cout << "1. Yes" << endl;
    cout << "2. No" << endl;
    cout << "Enter your choice: ";

    int mng = getValidInt(1, 2);

    if (mng == 1)
    {
        // Update using the temp file technique
        fstream readFile("catalogue.txt", ios::in);
        fstream writeFile("temp.txt", ios::out);

        if (!readFile.is_open() || !writeFile.is_open())
        {
            cout << "Error: Could not open files!" << endl;
            delete[] price;
            delete[] p_ID;
            delete[] name;
            delete[] category;
            return;
        }

        while (readFile.getline(price, NAME_LEN, '|'))
        {
            readFile.getline(p_ID, NAME_LEN, '|');
            readFile.getline(name, NAME_LEN, '|');
            readFile.getline(category, NAME_LEN, '|');
            readFile >> quantity;
            readFile.ignore(1, '\n');

            // If stock is low, add 10
            if (quantity <= 5)
            {
                writeFile << price << "|" << p_ID << "|" << name << "|"
                          << category << "|" << (quantity + 10) << endl;
            }
            else
            {
                // Keep the remaining products unchanged
                writeFile << price << "|" << p_ID << "|" << name << "|"
                          << category << "|" << quantity << endl;
            }
        }

        readFile.close();
        writeFile.close();
        remove("catalogue.txt");
        rename("temp.txt", "catalogue.txt");

        cout << "10 pieces have been added to low stock items!" << endl;
        logAudit("Admin restocked low inventory items (+10 each)");
    }
    else
    {
        cout << "Restocking cancelled." << endl;
    }

    // Free dynamic memory
    delete[] price;
    delete[] p_ID;
    delete[] name;
    delete[] category;
}

// ===================== mngbulk =====================
// Displays the bulk import/export menu
// Export: export extra stock items
// Import: restock low stock items
void mngbulk()
{
    cout << endl;
    displayLine(40, '=');
    cout << "     BULK OPERATIONS" << endl;
    displayLine(40, '=');
    cout << "1. Export Extra Stock" << endl;
    cout << "2. Import Low Stock (Restock)" << endl;
    cout << "Enter your choice: ";

    int mng = getValidInt(1, 2);

    if (mng == 1)
    {
        exportextra();
    }
    else if (mng == 2)
    {
        importStock();
    }
}

// ===================== mngdiscs =====================
// Creates and manages discount codes
// BUG FIX: Previously it wrote to "dicounts.txt" (typo)
// Now it correctly writes to "discount.txt"
void mngdiscs()
{
    cout << endl;
    displayLine(40, '=');
    cout << "     DISCOUNT MANAGEMENT" << endl;
    displayLine(40, '=');

    // Dynamic memory allocation
    char *disc = new char[NAME_LEN];
    cout << "Enter Discount Code: ";
    cin.getline(disc, NAME_LEN, '\n');

    cout << "Enter Discount Percentage (1-100): ";
    int percentage = getValidInt(1, 100);

    // BUG FIX: Correct filename "discount.txt" (previously "dicounts.txt" - typo)
    fstream write("discount.txt", ios::app);
    if (write.is_open())
    {
        write << disc << "|" << percentage << endl;
        write.close();
        cout << "Discount Code '" << disc << "' (" << percentage << "% OFF) added successfully!" << endl;
        logAudit("Admin created new discount code");
    }
    else
    {
        cout << "Error: Could not open discount.txt!" << endl;
    }

    delete[] disc;
}

// ===================== revenue =====================
// Performs sales and revenue analysis
// Reads data from order history and calculates total revenue
// BUG FIX: ttl is now properly initialized to 0
// Previously it was uninitialized - garbage values caused incorrect results
void revenue()
{
    // Dynamic memory allocation
    char *readname = new char[NAME_LEN];
    char *readproduct = new char[NAME_LEN];
    int readprice;
    char *readp_ID = new char[ID_LEN];
    int readquantity;

    // BUG FIX: Initialize ttl with 0
    int ttl = 0;

    fstream read("orderhistory.txt", ios::in);

    cout << endl;
    displayLine(70, '=');
    cout << "          SALES & REVENUE ANALYSIS" << endl;
    displayLine(70, '=');
    cout << left << setw(20) << "Customer" << setw(20) << "Product"
         << "$" << setw(10) << "Price" << setw(10) << "Qty"
         << setw(10) << "ID" << endl;
    displayLine(70, '-');

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

            // Calculate revenue
            ttl += (readprice * readquantity);

            cout << left << setw(20) << readname << setw(20) << readproduct
                 << "$" << setw(10) << readprice << setw(10) << readquantity
                 << setw(10) << readp_ID << endl;
        }
    }
    else
    {
        cout << "Could not open order history file." << endl;
    }

    displayLine(70, '-');
    cout << "TOTAL REVENUE: $" << ttl << endl;
    displayLine(70, '=');
    read.close();

    logAudit("Admin viewed revenue report");

    // Free dynamic memory
    delete[] readname;
    delete[] readproduct;
    delete[] readp_ID;
}

// ===================== viewAuditTrail =====================
// Displays the audit trail
// Shows the record of all admin actions - for accountability
// Cybersecurity: Audit trail feature - everything is recorded
// Previously this function was EMPTY - now it is properly implemented
void viewAuditTrail()
{
    // Dynamic memory allocation
    char *line = new char[TEXT_LEN];

    fstream read("audit.txt", ios::in);

    cout << endl;
    displayLine(70, '=');
    cout << "          AUDIT TRAIL" << endl;
    displayLine(70, '=');

    if (read.is_open())
    {
        bool hasRecords = false;
        while (read.getline(line, TEXT_LEN))
        {
            cout << "  " << line << endl;
            hasRecords = true;
        }

        if (!hasRecords)
        {
            cout << "  No audit record found." << endl;
        }
        read.close();
    }
    else
    {
        cout << "  Audit file has not been created yet (no actions recorded)." << endl;
    }

    displayLine(70, '=');
    logAudit("Admin viewed audit trail");

    delete[] line;
}

// ===================== viewActivityLogs =====================
// Displays activity logs - for security monitoring
// BUG FIX: Previously the file was opened with ios::out which ERASED all data
// Now it is correctly opened with ios::in (read-only)
void viewActivityLogs()
{
    // Dynamic memory allocation
    char *line = new char[TEXT_LEN];

    // BUG FIX: Use ios::in (previously ios::out which truncated the file)
    fstream read("activity.txt", ios::in);

    cout << endl;
    displayLine(70, '=');
    cout << "          ACTIVITY LOGS - Security Monitoring" << endl;
    displayLine(70, '=');

    if (read.is_open())
    {
        bool hasLogs = false;
        while (read.getline(line, TEXT_LEN))
        {
            cout << "  " << line << endl;
            hasLogs = true;
        }

        if (!hasLogs)
        {
            cout << "  No activity logs found." << endl;
        }
        read.close();
    }
    else
    {
        cout << "  Activity log file not found." << endl;
    }

    displayLine(70, '=');
    logAudit("Admin reviewed activity logs");

    delete[] line;
}
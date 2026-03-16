// ====================== UTILS.H ======================
// This header file contains all shared/common functions
// that are used by customer, employee, and admin modules.
// This prevents code duplication (DRY - Don't Repeat Yourself).
// Include guards are used to prevent multiple inclusions.
// ======================================================

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <cstring>
using namespace std;

// --------------- Constants ---------------
// These constants define array sizes to avoid magic numbers
const int NAME_LEN = 50;
const int PASS_LEN = 30;
const int ID_LEN = 20;
const int TEXT_LEN = 500;
const int LINE_LEN = 200;

// --------------- String Compare Function ---------------
// Compares two strings character by character using pointers
bool compareStrings(char *p1, char *p2);

// --------------- Password Encryption ---------------
// Encrypts password using a basic cipher with pointer arithmetic
void encryptPassword(char *enpass);

// --------------- Input Validation ---------------
// Takes valid integer input from user within a given range
// Handles invalid input (text or out-of-range) automatically
int getValidInt(int min, int max);

// --------------- Activity Logging ---------------
// Logs any activity to activity.txt with timestamp
// Cybersecurity requirement: security monitoring
void logActivity(const char *message);

// --------------- Audit Trail ---------------
// Records admin actions to audit.txt with timestamp
// Cybersecurity requirement: accountability
void logAudit(const char *action);

// --------------- Display Line ---------------
// Prints a decorative line for better output formatting
void displayLine(int width, char ch);

// --------------- Catalogue Management (Shared) ---------------
// These functions are used by both admin and employee
// Kept shared to avoid code duplication
void catalogueAdd();
void catalogueDelete();
void catalogueEdit();
void catalogueManage();

// --------------- Catalogue Display ---------------
// Displays all products from the catalogue
void displayCatalogue();

// --------------- Announcements ---------------
// Handles notifications and announcements system
void sendAnnouncement();
void viewAnnouncements();

#endif

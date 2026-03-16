#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include "ui.h"

using namespace std;

const int PASS_LEN = 100;
const int NAME_LEN = 100;
const int ID_LEN   = 50;
const int LINE_LEN = 400;
const int TEXT_LEN = 1000;

extern char currentCustomerName[NAME_LEN];
extern char currentEmployeeName[NAME_LEN];
extern char currentAdminName[NAME_LEN];

bool compareStrings(const char *p1, const char *p2);
void copyString(char *dest, const char *src);
bool isEmptyString(const char *str);
bool getNonEmptyLine(const char *promptText, char *buffer, int len);
bool usernameExists(const char *filename, const char *username);

void encryptPassword(char *enpass);
int getValidInt(int min, int max);

void logActivity(const char *message);
void logAudit(const char *action);

void displayLine(int width, char ch);

void displayCatalogue();
void catalogueAdd();
void catalogueDelete();
void catalogueEdit();
void catalogueManage();

void sendAnnouncement();
void viewAnnouncements();
// ====================== EMPLOYEE.H ======================
// This header file holds declarations for the employee module
// Employees handle inventory, feedback, support, and sales
// Common/shared functions are in utils.h (no duplication)
// Include guard prevents multiple inclusion errors
// =========================================================

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "utils.h"

// --------------- Employee Menu ---------------
// Main menu for employee - register or login
void emenu();

// --------------- Employee Login Verification ---------------
// Verifies employee credentials against the file
bool everify();

// --------------- Employee Registration ---------------
// Registers a new employee
void eregistration();

// --------------- Feedback Management ---------------
// Employee views and replies to customer feedback
void mfeedback();

// --------------- Support Management ---------------
// Employee views and replies to support requests
void msupport();

// --------------- Sales Record ---------------
// Displays sales history
void sales();

// --------------- Stock Alert ---------------
// Shows low stock items that need restocking
void stock();

#endif
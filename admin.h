// ====================== ADMIN.H ======================
// This header file holds declarations for the admin module
// Admin has the highest level of access and control
// Features: user management, inventory, discounts, 2FA,
// security monitoring, sales analysis, audit trail
// Common functions are in utils.h (no duplication)
// Include guard prevents multiple inclusion errors
// =====================================================

#ifndef ADMIN_H
#define ADMIN_H

#include "utils.h"

// --------------- Admin Menu ---------------
// Main menu for admin - login with 2FA, then access admin panel
void amenu();

// --------------- Admin Login Verification ---------------
// Verifies admin credentials (password + OTP for 2FA)
bool averify();

// --------------- User Management ---------------
// Admin can add and delete customers and employees
void mnguser();
void addcustomer();
void addemployee();
void deletecustomer();
void deleteemployee();

// --------------- Bulk Import/Export ---------------
// Admin can manage inventory in bulk
void mngbulk();
void exportextra();
void importStock();

// --------------- Discounts Management ---------------
// Admin creates and manages discount codes
void mngdiscs();

// --------------- Revenue Analysis ---------------
// Admin can analyze sales and revenue data
void revenue();

// --------------- Audit Trail View ---------------
// Admin can view the audit trail (accountability feature)
void viewAuditTrail();

// --------------- Activity Logs View ---------------
// Admin can view activity logs (security monitoring)
void viewActivityLogs();

#endif
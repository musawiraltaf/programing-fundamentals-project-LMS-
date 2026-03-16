// ====================== CUSTOMER.H ======================
// This header file holds declarations for the customer module
// Only customer-specific functions are declared here
// Common functions are in utils.h (no duplication)
// Include guard prevents multiple inclusion errors
// =========================================================

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "utils.h"

// --------------- Customer Menu ---------------
// Main menu for customer - register or login
void cmenu();

// --------------- Customer Login Verification ---------------
// Checks customer credentials against the file
bool cverify();

// --------------- Customer Registration ---------------
// Registers a new customer
void cregistration();

// --------------- Product Display ---------------
// Displays all products in a formatted table
void displayproducts();

// --------------- Product Search/Browse Menu ---------------
// Menu for browsing products - by category, price, availability
void searchproducts();

// --------------- Cart Management ---------------
// Customer can add, remove, and update items in their cart
void customerAddToCart();
void customerRemoveFromCart();
void customerUpdateCart();
void displaycart();

// --------------- Wishlist ---------------
// Customer can add and view items in their wishlist
void addtowishlist();

// --------------- Checkout ---------------
// Finalizes cart items and saves to order history
void checkout();

// --------------- Order History ---------------
// Customer can view their past orders
void vieworder();

// --------------- Feedback and Support ---------------
// Customer can submit feedback and support requests
void Feedback();
void Support();

#endif
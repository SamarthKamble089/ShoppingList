# BudgetBrake (ShoppingList)

**BudgetBrake - Shop till the Budget Stops**

A Windows Desktop GUI Application built in C using the Win32 API. BudgetBrake helps users manage their shopping list, track their expenses against a set monthly budget, and maintain a transaction history across multiple categories of items.

## Features

- **User Authentication**: Secure login system for multiple users (e.g., *samarth*, *krithika*).
- **Categorized Shopping**: Browse items through categories such as *Groceries*, *Clothes*, *Snacks*, and *Outside Food*.
- **Dynamic Inventory**: Shop and item details are loaded dynamically from `.txt` files in the `data/` directory. New items added by the user during runtime are also saved back to the text files.
- **Shopping Cart System**: 
  - Add items to your cart with specific quantities.
  - View real-time Cart Total and Remaining Budget.
  - Remove individual items or clear the entire cart with a single click.
  - Budget constraints prevent you from overspending your monthly limit.
- **Checkout & Logging**: Complete your purchase to update your remaining budget. Every successful transaction is logged with a timestamp in the user's history file.
- **User Profile Management**: Users can update their login password and adjust their monthly budget allowance dynamically.
- **Dark Mode UI**: Custom-drawn Win32 interface featuring a sleek dark blue aesthetic, complete with owner-drawn buttons and custom brushes.

## Project Structure

- `BudgetBrake.c`: The main C source code file containing the application logic, Win32 event loop, and UI creation.
- `data/`: A directory created on first launch to store all persistent data.
  - `history_<username>.txt`: Stores past transactions and checkout logs for specific users.
  - `<Category>/<Shop>.txt`: Stores the available items and their prices in a `ItemName,Price` comma-separated format.

## How to Run

1. Compile the source code using a C compiler (like GCC via MinGW) with Windows subsystem flags:
   ```bash
   gcc BudgetBrake.c -o BudgetBrake.exe -mwindows
   ```
2. Run the generated `BudgetBrake.exe` file.
3. Login using the default credentials. 
   - *Example User 1*: `samarth` / Password: `1rz25cs152`
   - *Example User 2*: `krithika` / Password: `1rz25cs145`

## Managing Items

You can manually edit the inventory by modifying the `.txt` files located in the `data/` folder. 
Ensure the format follows `ItemName,Price` (e.g., `Apple,120`). The application will automatically parse these files and populate the dropdown menus inside the GUI.
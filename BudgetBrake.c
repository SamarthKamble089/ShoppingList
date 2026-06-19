#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <direct.h> // For directory creation

// ==========================================
// DATA STRUCTURES & GLOBALS
// ==========================================
struct Shop {
    char name[50];
    char items[100][50];
    int prices[100];
    int item_count;
};

struct Category {
    char name[50];
    struct Shop shops[10];
    int shop_count;
};

struct Log {
    char date_time[60];
    int amount_spent;
};

struct User {
    char uname[50];
    char password[50];
    int budget;
    struct Log history[50]; 
    int history_count;      
    int shop_spent[4][10];
};

struct Category categories[4];
struct User users[10];

int active_user = -1;  
int cart_total = 0;
int current_cat_idx = -1;
int current_shop_idx = -1;

// --- Custom Color Brushes ---
HBRUSH hBgBrush;     // Main dark blue background
HBRUSH hInputBrush;  // Slightly lighter blue for text boxes & cart

// ==========================================
// UI CONTROL IDs
// ==========================================
#define ID_BTN_LOGIN       1
#define ID_TXT_USER        2
#define ID_TXT_PASS        3
#define ID_BTN_QUIT        4  

#define ID_BTN_CAT_0       10
#define ID_BTN_CAT_1       11
#define ID_BTN_CAT_2       12
#define ID_BTN_CAT_3       13
#define ID_BTN_CHECKOUT    14
#define ID_BTN_PROFILE     15
#define ID_BTN_LOGOUT      16
#define ID_BTN_HISTORY     17 
#define ID_BTN_CLEAR_CART  18 

#define ID_COMBO_SHOPS     19
#define ID_COMBO_ITEMS     20
#define ID_TXT_ITEM_NAME   21
#define ID_TXT_ITEM_PRICE  22
#define ID_BTN_ADD_CART    23
#define ID_TXT_ITEM_QTY    24
#define ID_BTN_REMOVE_CART_ITEM 25

#define ID_BTN_SAVE_PROF   30
#define ID_BTN_CLEAR_HISTORY 31
#define ID_BTN_GRAPHS      32
#define ID_COMBO_GRAPH_CAT 33
#define ID_LIST_HISTORY    34

// ==========================================
// UI HANDLES
// ==========================================
HWND hAppTitle; 
HFONT hTitleFont; 

HWND hLblUser, hUsername, hLblPass, hPass, hBtnLogin, hBtnQuit, hLoginTitle; 
HWND hBtnCat[4], hBtnCheckout, hBtnProfile, hBtnLogout, hBtnHistory, hBtnGraphs;
HWND hCartList, hLblBudget, hLblCartTotal, hCartTitle, hBtnClearCart, hBtnRemoveItem;
HWND hCatTitle, hLblShop, hComboShops, hLblSelect, hComboItems, hLblItemName, hTxtItemName, hLblItemPrice, hTxtItemPrice, hLblItemQty, hTxtItemQty, hBtnAddToCart;
int cart_item_prices[100];
int cart_item_cat_idx[100];
int cart_item_shop_idx[100];
char cart_item_names[100][100];
int cart_item_qtys[100];
int cart_item_unit_prices[100];
int cart_items_count = 0;
HWND hProfTitle, hProfUser, hLblNewPass, hTxtNewPass, hLblNewBudg, hTxtNewBudg, hBtnSaveProf;
HWND hHistoryTitle, hHistoryList, hBtnClearHistory, hTxnDetailsTitle, hTxnDetailsList;
HFONT hMonoFont;
HWND hGraphTitle, hComboGraphCat, hGraphArea;
int current_graph_cat_idx = 0;

// ==========================================
// DATA INITIALIZATION
// ==========================================
void load_categories_from_files() {
    _mkdir("data"); 
    
    const char* cat_names[4] = {"Groceries", "Clothes", "Snacks", "Outside Food"};
    const char* default_shops[4][5] = {
        {"DMart", "Reliance Smart", "BigBasket", "More", "Star Bazaar"},
        {"Puma", "Zudio", "H&M", "Levi's", "Zara"},
        {"Haldiram's", "Bikanerwala", "Sweet Shop", "Local Bakery", "A2B"},
        {"Dominos", "McDonald's", "KFC", "Local Dhaba", "Taco Bell"}
    };
    
    for(int i=0; i<4; i++) {
        strcpy(categories[i].name, cat_names[i]);
        categories[i].shop_count = 5;
        
        char cat_path[100];
        sprintf(cat_path, "data/%s", cat_names[i]);
        _mkdir(cat_path);
        
        for (int j=0; j<5; j++) {
            strcpy(categories[i].shops[j].name, default_shops[i][j]);
            categories[i].shops[j].item_count = 0;
            
            char filepath[150];
            sprintf(filepath, "data/%s/%s.txt", cat_names[i], default_shops[i][j]);
            
            FILE *file = fopen(filepath, "r");
            if (file == NULL) {
                file = fopen(filepath, "w");
                if (file != NULL) {
                    if (i == 0 && j == 0) fprintf(file, "Rice,60\nWheat,40\nSugar,45\nDal,110\nOil,150\n");
                    else if (i == 0 && j == 1) fprintf(file, "Apple,120\nBanana,60\nMilk,30\nBread,40\nEggs,60\n");
                    else if (i == 0 && j == 2) fprintf(file, "Onion,40\nPotato,30\nTomato,50\nCarrot,60\nCapsicum,80\n");
                    else if (i == 0 && j == 3) fprintf(file, "Soap,40\nShampoo,150\nToothpaste,80\nDetergent,120\nCleaner,90\n");
                    else if (i == 0 && j == 4) fprintf(file, "Butter,50\nCheese,120\nPaneer,90\nYogurt,30\nIce Cream,150\n");
                    else if (i == 1 && j == 0) fprintf(file, "Running Shoes,2999\nT-Shirt,999\nTrack Pants,1499\nSocks,399\nGym Bag,1299\n");
                    else if (i == 1 && j == 1) fprintf(file, "Casual Shirt,499\nJeans,799\nT-Shirt,299\nJacket,999\nSneakers,699\n");
                    else if (i == 1 && j == 2) fprintf(file, "Hoodie,1999\nChinos,1499\nDress,2499\nSweater,1799\nScarf,499\n");
                    else if (i == 1 && j == 3) fprintf(file, "Denim Jacket,3499\n501 Jeans,2999\nLogo T-Shirt,1299\nBelt,899\nWallet,999\n");
                    else if (i == 1 && j == 4) fprintf(file, "Blazer,4999\nTrousers,2499\nPerfume,1299\nFormal Shirt,1999\nBoots,3999\n");
                    else if (i == 2 && j == 0) fprintf(file, "Aloo Bhujia,50\nMoong Dal,40\nSoan Papdi,120\nRasgulla,150\nSamosa,20\n");
                    else if (i == 2 && j == 1) fprintf(file, "Kaju Katli,300\nNamkeen Mix,60\nDhokla,80\nKachori,25\nJalebi,100\n");
                    else if (i == 2 && j == 2) fprintf(file, "Gulab Jamun,100\nBarfi,150\nLadoo,120\nPeda,140\nRasmalai,180\n");
                    else if (i == 2 && j == 3) fprintf(file, "Veg Puff,20\nChocolate Cake,350\nCream Roll,15\nBreadsticks,40\nMuffins,50\n");
                    else if (i == 2 && j == 4) fprintf(file, "Murukku,60\nMixture,70\nMysore Pak,200\nThattai,50\nChettinad Snacks,80\n");
                    else if (i == 3 && j == 0) fprintf(file, "Margherita Pizza,199\nPepperoni Pizza,399\nGarlic Bread,99\nChoco Lava Cake,109\nPasta,129\n");
                    else if (i == 3 && j == 1) fprintf(file, "McAloo Tikki,50\nMcVeggie,100\nFrench Fries,90\nChicken McGrill,80\nCoke,60\n");
                    else if (i == 3 && j == 2) fprintf(file, "Zinger Burger,180\nFried Chicken,250\nPopcorn Chicken,120\nChicken Wings,150\nKrushers,90\n");
                    else if (i == 3 && j == 3) fprintf(file, "Paneer Butter Masala,180\nDal Makhani,150\nButter Naan,40\nChicken Tikka,220\nLassi,60\n");
                    else if (i == 3 && j == 4) fprintf(file, "Crunchwrap,150\nTacos,100\nBurrito,180\nNachos,120\nQuesadilla,140\n");
                    fclose(file);
                    
                    file = fopen(filepath, "r");
                }
            }
            
            if (file != NULL) {
                char line[150];
                while(fgets(line, sizeof(line), file)) {
                    char *comma = strchr(line, ',');
                    if (comma != NULL) {
                        *comma = '\0';
                        char *name = line;
                        char *price_str = comma + 1;
                        
                        char *newline = strchr(price_str, '\n');
                        if(newline) *newline = '\0';
                        
                        if (categories[i].shops[j].item_count < 100) {
                            strcpy(categories[i].shops[j].items[categories[i].shops[j].item_count], name);
                            categories[i].shops[j].prices[categories[i].shops[j].item_count] = atoi(price_str);
                            categories[i].shops[j].item_count++;
                        }
                    }
                }
                fclose(file);
            }
        }
    }
}

void load_user_history() {
    for (int i = 0; i < 2; i++) {
        char filepath[100];
        sprintf(filepath, "data/history_%s.txt", users[i].uname);
        FILE *file = fopen(filepath, "r");
        if (file) {
            char line[150];
            while (fgets(line, sizeof(line), file)) {
                char *comma = strchr(line, ',');
                if (comma) {
                    *comma = '\0';
                    strcpy(users[i].history[users[i].history_count].date_time, line);
                    users[i].history[users[i].history_count].amount_spent = atoi(comma + 1);
                    users[i].history_count++;
                }
            }
            fclose(file);
        }
    }
}

void save_user_stats(int user_idx) {
    char filepath[100];
    sprintf(filepath, "data/shop_stats_%s.txt", users[user_idx].uname);
    FILE *file = fopen(filepath, "w");
    if (file) {
        for(int c=0; c<4; c++) {
            for(int s=0; s<10; s++) {
                if (users[user_idx].shop_spent[c][s] > 0) {
                    fprintf(file, "%d,%d,%d\n", c, s, users[user_idx].shop_spent[c][s]);
                }
            }
        }
        fclose(file);
    }
}

void load_user_stats() {
    for (int i = 0; i < 2; i++) {
        for (int c=0; c<4; c++) {
            for (int s=0; s<10; s++) {
                users[i].shop_spent[c][s] = 0;
            }
        }
        char filepath[100];
        sprintf(filepath, "data/shop_stats_%s.txt", users[i].uname);
        FILE *file = fopen(filepath, "r");
        if (file) {
            char line[150];
            while (fgets(line, sizeof(line), file)) {
                int cat, shop, amount;
                if (sscanf(line, "%d,%d,%d", &cat, &shop, &amount) == 3) {
                    if (cat >= 0 && cat < 4 && shop >= 0 && shop < 10) {
                        users[i].shop_spent[cat][shop] = amount;
                    }
                }
            }
            fclose(file);
        }
    }
}

void save_user_profile(int user_idx) {
    char filepath[100];
    sprintf(filepath, "data/user_profile_%s.txt", users[user_idx].uname);
    FILE *file = fopen(filepath, "w");
    if (file) {
        fprintf(file, "%s\n%d\n", users[user_idx].password, users[user_idx].budget);
        fclose(file);
    }
}

void load_user_profiles() {
    for (int i = 0; i < 2; i++) {
        char filepath[100];
        sprintf(filepath, "data/user_profile_%s.txt", users[i].uname);
        FILE *file = fopen(filepath, "r");
        if (file) {
            char pass[50];
            int budg;
            if (fscanf(file, "%49s", pass) == 1) {
                if (fscanf(file, "%d", &budg) == 1) {
                    strcpy(users[i].password, pass);
                    users[i].budget = budg;
                }
            }
            fclose(file);
        }
    }
}

void initialize_data() {
    strcpy(users[0].uname, "samarth");
    strcpy(users[0].password, "1rz25cs152");
    users[0].budget = 5000;
    users[0].history_count = 0;

    strcpy(users[1].uname, "krithika");
    strcpy(users[1].password, "1rz25cs145");
    users[1].budget = 5000;
    users[1].history_count = 0;

    load_categories_from_files();
    load_user_history();
    load_user_stats();
    load_user_profiles();
}

// ==========================================
// VIEW CLEARING FUNCTIONS
// ==========================================
void ClearMiddleArea() {
    if (hCatTitle) DestroyWindow(hCatTitle);
    if (hLblShop) DestroyWindow(hLblShop);
    if (hComboShops) DestroyWindow(hComboShops);
    if (hLblSelect) DestroyWindow(hLblSelect);
    if (hComboItems) DestroyWindow(hComboItems);
    if (hLblItemName) DestroyWindow(hLblItemName);
    if (hTxtItemName) DestroyWindow(hTxtItemName);
    if (hLblItemPrice) DestroyWindow(hLblItemPrice);
    if (hTxtItemPrice) DestroyWindow(hTxtItemPrice);
    if (hLblItemQty) DestroyWindow(hLblItemQty);
    if (hTxtItemQty) DestroyWindow(hTxtItemQty);
    if (hBtnAddToCart) DestroyWindow(hBtnAddToCart);

    if (hProfTitle) DestroyWindow(hProfTitle);
    if (hProfUser) DestroyWindow(hProfUser);
    if (hLblNewPass) DestroyWindow(hLblNewPass);
    if (hTxtNewPass) DestroyWindow(hTxtNewPass);
    if (hLblNewBudg) DestroyWindow(hLblNewBudg);
    if (hTxtNewBudg) DestroyWindow(hTxtNewBudg);
    if (hBtnSaveProf) DestroyWindow(hBtnSaveProf);
    
    if (hHistoryTitle) DestroyWindow(hHistoryTitle);
    if (hHistoryList) DestroyWindow(hHistoryList);
    if (hBtnClearHistory) DestroyWindow(hBtnClearHistory);
    if (hTxnDetailsTitle) DestroyWindow(hTxnDetailsTitle);
    if (hTxnDetailsList) DestroyWindow(hTxnDetailsList);
    
    if (hGraphTitle) DestroyWindow(hGraphTitle);
    if (hComboGraphCat) DestroyWindow(hComboGraphCat);
    if (hGraphArea) DestroyWindow(hGraphArea);
}

void ClearDashboard() {
    ClearMiddleArea();
    for (int i=0; i<4; i++) {
        if (hBtnCat[i]) DestroyWindow(hBtnCat[i]);
    }
    DestroyWindow(hBtnCheckout); DestroyWindow(hBtnProfile); DestroyWindow(hBtnHistory); DestroyWindow(hBtnGraphs);
    DestroyWindow(hCartList); DestroyWindow(hLblBudget); DestroyWindow(hLblCartTotal);
    DestroyWindow(hCartTitle); DestroyWindow(hBtnClearCart); DestroyWindow(hBtnRemoveItem); DestroyWindow(hBtnLogout);
}

void ClearLoginView() {
    DestroyWindow(hLoginTitle); DestroyWindow(hLblUser); DestroyWindow(hUsername);
    DestroyWindow(hLblPass); DestroyWindow(hPass); DestroyWindow(hBtnLogin); DestroyWindow(hBtnQuit);
}

// ==========================================
// VIEW CREATION FUNCTIONS (Buttons changed to BS_OWNERDRAW)
// ==========================================
void ShowLoginView(HWND hwnd) {
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    
    hLoginTitle = CreateWindow("STATIC", "Please login to your account", WS_VISIBLE | WS_CHILD | SS_CENTER, (screenW/2)-150, 250, 300, 25, hwnd, NULL, NULL, NULL);
    hLblUser = CreateWindow("STATIC", "Username:", WS_VISIBLE | WS_CHILD, (screenW/2)-150, 300, 80, 25, hwnd, NULL, NULL, NULL);
    hUsername = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, (screenW/2)-50, 300, 200, 25, hwnd, (HMENU)ID_TXT_USER, NULL, NULL);
    hLblPass = CreateWindow("STATIC", "Password:", WS_VISIBLE | WS_CHILD, (screenW/2)-150, 350, 80, 25, hwnd, NULL, NULL, NULL);
    hPass = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_PASSWORD, (screenW/2)-50, 350, 200, 25, hwnd, (HMENU)ID_TXT_PASS, NULL, NULL);
    
    hBtnLogin = CreateWindow("BUTTON", "Login", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, (screenW/2)-160, 410, 150, 35, hwnd, (HMENU)ID_BTN_LOGIN, NULL, NULL);
    hBtnQuit = CreateWindow("BUTTON", "Quit", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, (screenW/2)+10, 410, 150, 35, hwnd, (HMENU)ID_BTN_QUIT, NULL, NULL);
}

void UpdateFinancialLabels() {
    if (active_user == -1) return;
    char budgetText[100];
    char cartText[100];
    sprintf(budgetText, "Budget Remaining: Rs. %d", users[active_user].budget - cart_total);
    sprintf(cartText, "Cart Total: Rs. %d", cart_total);
    SetWindowText(hLblBudget, budgetText);
    SetWindowText(hLblCartTotal, cartText);
}

void BuildDashboardLayout(HWND hwnd) {
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    
    int btnWidth = 120;
    int spacing = 10;
    int startX = (screenW - (8 * btnWidth + 7 * spacing)) / 2; 
    int topY = 70; 
    
    hBtnCat[0] = CreateWindow("BUTTON", categories[0].name, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, startX, topY, btnWidth, 40, hwnd, (HMENU)ID_BTN_CAT_0, NULL, NULL);
    hBtnCat[1] = CreateWindow("BUTTON", categories[1].name, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, startX + 1*(btnWidth + spacing), topY, btnWidth, 40, hwnd, (HMENU)ID_BTN_CAT_1, NULL, NULL);
    hBtnCat[2] = CreateWindow("BUTTON", categories[2].name, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, startX + 2*(btnWidth + spacing), topY, btnWidth, 40, hwnd, (HMENU)ID_BTN_CAT_2, NULL, NULL);
    hBtnCat[3] = CreateWindow("BUTTON", categories[3].name, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, startX + 3*(btnWidth + spacing), topY, btnWidth, 40, hwnd, (HMENU)ID_BTN_CAT_3, NULL, NULL);
    hBtnCheckout = CreateWindow("BUTTON", "Checkout", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, startX + 4*(btnWidth + spacing), topY, btnWidth, 40, hwnd, (HMENU)ID_BTN_CHECKOUT, NULL, NULL);
    hBtnProfile = CreateWindow("BUTTON", "Profile", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, startX + 5*(btnWidth + spacing), topY, btnWidth, 40, hwnd, (HMENU)ID_BTN_PROFILE, NULL, NULL);
    hBtnHistory = CreateWindow("BUTTON", "Logs", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, startX + 6*(btnWidth + spacing), topY, btnWidth, 40, hwnd, (HMENU)ID_BTN_HISTORY, NULL, NULL);
    hBtnGraphs = CreateWindow("BUTTON", "Graphs", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, startX + 7*(btnWidth + spacing), topY, btnWidth, 40, hwnd, (HMENU)ID_BTN_GRAPHS, NULL, NULL);

    hBtnLogout = CreateWindow("BUTTON", "Logout", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 30, screenH - 120, 100, 40, hwnd, (HMENU)ID_BTN_LOGOUT, NULL, NULL);

    int cartY = 550; 
    hCartTitle = CreateWindow("STATIC", "Shopping Cart:", WS_VISIBLE | WS_CHILD, 100, cartY - 25, 200, 20, hwnd, NULL, NULL, NULL);
    
    hBtnRemoveItem = CreateWindow("BUTTON", "Remove Selected", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, screenW - 350, cartY - 30, 120, 25, hwnd, (HMENU)ID_BTN_REMOVE_CART_ITEM, NULL, NULL);
    hBtnClearCart = CreateWindow("BUTTON", "Clear Cart", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, screenW - 220, cartY - 30, 120, 25, hwnd, (HMENU)ID_BTN_CLEAR_CART, NULL, NULL);

    hCartList = CreateWindow("LISTBOX", "", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY, 100, cartY, screenW - 200, 150, hwnd, NULL, NULL, NULL);
    hLblBudget = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD, 100, cartY + 160, 250, 25, hwnd, NULL, NULL, NULL);
    hLblCartTotal = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD, 400, cartY + 160, 250, 25, hwnd, NULL, NULL, NULL);
    
    UpdateFinancialLabels();

    hCatTitle = CreateWindow("STATIC", "Select a category to begin.", WS_VISIBLE | WS_CHILD | SS_CENTER, (screenW/2)-200, 200, 400, 25, hwnd, NULL, NULL, NULL);
}

void ShowCategoryView(HWND hwnd, int cat_idx) {
    ClearMiddleArea();
    current_cat_idx = cat_idx;
    int screenW = GetSystemMetrics(SM_CXSCREEN);

    char title[100];
    sprintf(title, "--- Browsing %s ---", categories[cat_idx].name);
    hCatTitle = CreateWindow("STATIC", title, WS_VISIBLE | WS_CHILD | SS_CENTER, (screenW/2)-150, 150, 300, 25, hwnd, NULL, NULL, NULL);

    int startX = (screenW / 2) - 150; 
    int y_pos = 200;
    
    current_shop_idx = -1; // Reset shop selection

    hLblShop = CreateWindow("STATIC", "Select Shop:", WS_VISIBLE | WS_CHILD, startX, y_pos, 100, 25, hwnd, NULL, NULL, NULL);
    hComboShops = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, startX + 110, y_pos, 200, 150, hwnd, (HMENU)ID_COMBO_SHOPS, NULL, NULL);

    for (int i = 0; i < categories[cat_idx].shop_count; i++) {
        SendMessage(hComboShops, CB_ADDSTRING, 0, (LPARAM)categories[cat_idx].shops[i].name);
    }

    y_pos += 50;

    hLblSelect = CreateWindow("STATIC", "Select Item:", WS_VISIBLE | WS_CHILD, startX, y_pos, 100, 25, hwnd, NULL, NULL, NULL);
    hComboItems = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, startX + 110, y_pos, 200, 150, hwnd, (HMENU)ID_COMBO_ITEMS, NULL, NULL);
    
    y_pos += 50;

    hLblItemName = CreateWindow("STATIC", "Item Name:", WS_VISIBLE | WS_CHILD, startX, y_pos, 100, 25, hwnd, NULL, NULL, NULL);
    hTxtItemName = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX + 110, y_pos, 200, 25, hwnd, (HMENU)ID_TXT_ITEM_NAME, NULL, NULL);

    y_pos += 50;

    hLblItemPrice = CreateWindow("STATIC", "Price (Rs):", WS_VISIBLE | WS_CHILD, startX, y_pos, 100, 25, hwnd, NULL, NULL, NULL);
    hTxtItemPrice = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_AUTOHSCROLL, startX + 110, y_pos, 100, 25, hwnd, (HMENU)ID_TXT_ITEM_PRICE, NULL, NULL);

    y_pos += 50;
    
    hLblItemQty = CreateWindow("STATIC", "Quantity:", WS_VISIBLE | WS_CHILD, startX, y_pos, 100, 25, hwnd, NULL, NULL, NULL);
    hTxtItemQty = CreateWindow("EDIT", "1", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_AUTOHSCROLL, startX + 110, y_pos, 100, 25, hwnd, (HMENU)ID_TXT_ITEM_QTY, NULL, NULL);

    y_pos += 50;

    hBtnAddToCart = CreateWindow("BUTTON", "Add to Cart", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, startX + 110, y_pos, 150, 40, hwnd, (HMENU)ID_BTN_ADD_CART, NULL, NULL);
}

void ShowProfileView(HWND hwnd) {
    ClearMiddleArea();
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int startX = (screenW / 2) - 200;
    int y_pos = 150;

    hProfTitle = CreateWindow("STATIC", "--- User Settings ---", WS_VISIBLE | WS_CHILD | SS_CENTER, startX, y_pos, 400, 25, hwnd, NULL, NULL, NULL); y_pos += 50;

    char uText[100];
    sprintf(uText, "Logged in as: %s", users[active_user].uname);
    hProfUser = CreateWindow("STATIC", uText, WS_VISIBLE | WS_CHILD, startX, y_pos, 300, 25, hwnd, NULL, NULL, NULL); y_pos += 50;

    hLblNewPass = CreateWindow("STATIC", "Reset Password:", WS_VISIBLE | WS_CHILD, startX, y_pos, 150, 25, hwnd, NULL, NULL, NULL);
    hTxtNewPass = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD | ES_AUTOHSCROLL, startX + 160, y_pos, 200, 25, hwnd, NULL, NULL, NULL); y_pos += 50;

    char bText[50];
    sprintf(bText, "%d", users[active_user].budget);
    hLblNewBudg = CreateWindow("STATIC", "Set Monthly Budget (Rs):", WS_VISIBLE | WS_CHILD, startX, y_pos, 200, 25, hwnd, NULL, NULL, NULL);
    hTxtNewBudg = CreateWindow("EDIT", bText, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_AUTOHSCROLL, startX + 210, y_pos, 150, 25, hwnd, NULL, NULL, NULL); y_pos += 60;

    hBtnSaveProf = CreateWindow("BUTTON", "Save Changes", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, startX + 100, y_pos, 200, 40, hwnd, (HMENU)ID_BTN_SAVE_PROF, NULL, NULL);
}

void ShowHistoryView(HWND hwnd) {
    ClearMiddleArea();
    int screenW = GetSystemMetrics(SM_CXSCREEN);

    hHistoryTitle = CreateWindow("STATIC", "--- Transaction Logs ---", WS_VISIBLE | WS_CHILD | SS_CENTER, (screenW/2)-400, 150, 350, 25, hwnd, NULL, NULL, NULL);
    hHistoryList = CreateWindow("LISTBOX", "", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY, (screenW/2)-400, 200, 350, 250, hwnd, (HMENU)ID_LIST_HISTORY, NULL, NULL);
    hBtnClearHistory = CreateWindow("BUTTON", "Clear History", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, (screenW/2)-300, 470, 150, 35, hwnd, (HMENU)ID_BTN_CLEAR_HISTORY, NULL, NULL);

    hTxnDetailsTitle = CreateWindow("STATIC", "Select a transaction to see details", WS_VISIBLE | WS_CHILD | SS_CENTER, (screenW/2), 150, 450, 25, hwnd, NULL, NULL, NULL);
    hTxnDetailsList = CreateWindow("LISTBOX", "", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL, (screenW/2), 200, 450, 250, hwnd, NULL, NULL, NULL);

    if (!hMonoFont) {
        hMonoFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, "Consolas");
    }
    SendMessage(hTxnDetailsList, WM_SETFONT, (WPARAM)hMonoFont, TRUE);

    if (users[active_user].history_count == 0) {
        SendMessage(hHistoryList, LB_ADDSTRING, 0, (LPARAM)"No past transactions found.");
    } else {
        for (int i = 0; i < users[active_user].history_count; i++) {
            char logStr[150];
            sprintf(logStr, "[ %s ] Rs. %d", users[active_user].history[i].date_time, users[active_user].history[i].amount_spent);
            SendMessage(hHistoryList, LB_ADDSTRING, 0, (LPARAM)logStr);
        }
    }
}

LRESULT CALLBACK GraphWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        
        RECT rc;
        GetClientRect(hwnd, &rc);
        
        // Fill background
        HBRUSH hBg = CreateSolidBrush(RGB(20, 30, 60));
        FillRect(hdc, &rc, hBg);
        DeleteObject(hBg);
        
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        TextOut(hdc, 10, 10, "Transactions vs Time (Line Graph)", 33);
        
        if (active_user != -1) {
            // Draw Axes
            HPEN hAxesPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hAxesPen);
            int w = rc.right;
            int h = rc.bottom - 40;
            
            // Y-axis
            MoveToEx(hdc, 20, 40, NULL);
            LineTo(hdc, 20, rc.bottom - 20);
            
            // X-axis
            MoveToEx(hdc, 20, rc.bottom - 20, NULL);
            LineTo(hdc, w - 20, rc.bottom - 20);
            
            // Labels
            TextOut(hdc, 25, 40, "Price", 5);
            TextOut(hdc, w - 50, rc.bottom - 20, "Time", 4);
            SelectObject(hdc, hOldPen);
            DeleteObject(hAxesPen);

            // Line Graph
            int h_count = users[active_user].history_count;
            if (h_count > 0) {
                int max_amt = 1;
                for (int i=0; i<h_count; i++) {
                    if (users[active_user].history[i].amount_spent > max_amt) 
                        max_amt = users[active_user].history[i].amount_spent;
                }
                
                int stepX = h_count > 1 ? (w - 60) / (h_count - 1) : w/2; // reduced width slightly to not overlap text
                
                HPEN hGraphPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 128));
                hOldPen = (HPEN)SelectObject(hdc, hGraphPen);
                
                for (int i=0; i<h_count; i++) {
                    int x = 20 + i * stepX;
                    int y = (rc.bottom - 20) - (users[active_user].history[i].amount_spent * h / max_amt);
                    if (i == 0) MoveToEx(hdc, x, y, NULL);
                    else LineTo(hdc, x, y);
                    
                    Ellipse(hdc, x-3, y-3, x+3, y+3);
                }
                SelectObject(hdc, hOldPen);
                DeleteObject(hGraphPen);
            } else {
                TextOut(hdc, rc.right/2 - 50, rc.bottom/2, "No Data", 7);
            }
        }
        
        EndPaint(hwnd, &ps);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void ShowGraphView(HWND hwnd) {
    ClearMiddleArea();
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    
    hGraphTitle = CreateWindow("STATIC", "--- Analytics Dashboard ---", WS_VISIBLE | WS_CHILD | SS_CENTER, (screenW/2)-200, 130, 400, 25, hwnd, NULL, NULL, NULL);

    hGraphArea = CreateWindow("GraphControl", "", WS_VISIBLE | WS_CHILD, (screenW/2)-400, 170, 800, 500, hwnd, NULL, NULL, NULL);
}

// ==========================================
// MAIN EVENT HANDLER
// ==========================================
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            {
                int screenW = GetSystemMetrics(SM_CXSCREEN);
                hAppTitle = CreateWindow("STATIC", "BudgetBrake - Shop till the Budget Stops", 
                                         WS_VISIBLE | WS_CHILD | SS_CENTER, 
                                         0, 15, screenW, 40, hwnd, NULL, NULL, NULL);

                hTitleFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
                                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
                                        DEFAULT_PITCH | FF_SWISS, "Segoe UI");
                SendMessage(hAppTitle, WM_SETFONT, (WPARAM)hTitleFont, TRUE);
            }
            
            ShowLoginView(hwnd);
            return 0;

        // ---------------------------------------------------------
        // CUSTOM DRAWING SECTION (Dark Mode Logic)
        // ---------------------------------------------------------
        case WM_CTLCOLORSTATIC:
            {
                HDC hdcStatic = (HDC)wParam;
                SetTextColor(hdcStatic, RGB(255, 255, 255)); // White text
                SetBkMode(hdcStatic, TRANSPARENT);
                return (LRESULT)hBgBrush; // Match the dark blue background
            }
            
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
            {
                HDC hdcBox = (HDC)wParam;
                SetTextColor(hdcBox, RGB(255, 255, 255)); // White text
                SetBkColor(hdcBox, RGB(30, 40, 70));      // Slate blue inner background
                return (LRESULT)hInputBrush; 
            }

        case WM_DRAWITEM:
            {
                // This triggers for every button because we used BS_OWNERDRAW
                LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
                if (pdis->CtlType == ODT_BUTTON) {
                    HDC hdc = pdis->hDC;
                    RECT rc = pdis->rcItem;

                    // Light blue for standard button, slightly darker when clicked
                    COLORREF btnColor = (pdis->itemState & ODS_SELECTED) ? RGB(40, 90, 160) : RGB(60, 120, 200);

                    // Paint the button background
                    HBRUSH hBrush = CreateSolidBrush(btnColor);
                    FillRect(hdc, &rc, hBrush);
                    DeleteObject(hBrush);

                    // Grab the text from the button and draw it in white
                    char btnText[100];
                    GetWindowText(pdis->hwndItem, btnText, 100);
                    SetTextColor(hdc, RGB(255, 255, 255)); 
                    SetBkMode(hdc, TRANSPARENT);
                    DrawText(hdc, btnText, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                    return TRUE;
                }
                break;
            }

        // ---------------------------------------------------------
        // BUTTON CLICK LOGIC
        // ---------------------------------------------------------
        case WM_COMMAND:
            if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == ID_COMBO_SHOPS) {
                int sel = SendMessage(hComboShops, CB_GETCURSEL, 0, 0);
                if (sel != CB_ERR && current_cat_idx != -1) {
                    current_shop_idx = sel;
                    SendMessage(hComboItems, CB_RESETCONTENT, 0, 0);
                    for (int i = 0; i < categories[current_cat_idx].shops[sel].item_count; i++) {
                        SendMessage(hComboItems, CB_ADDSTRING, 0, (LPARAM)categories[current_cat_idx].shops[sel].items[i]);
                    }
                    SetWindowText(hTxtItemName, "");
                    SetWindowText(hTxtItemPrice, "");
                }
            }

            if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == ID_COMBO_ITEMS) {
                int sel = SendMessage(hComboItems, CB_GETCURSEL, 0, 0);
                if (sel != CB_ERR && current_cat_idx != -1 && current_shop_idx != -1) {
                    char priceStr[20];
                    SetWindowText(hTxtItemName, categories[current_cat_idx].shops[current_shop_idx].items[sel]);
                    sprintf(priceStr, "%d", categories[current_cat_idx].shops[current_shop_idx].prices[sel]);
                    SetWindowText(hTxtItemPrice, priceStr);
                }
            }

            if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == ID_COMBO_GRAPH_CAT) {
                int sel = SendMessage(hComboGraphCat, CB_GETCURSEL, 0, 0);
                if (sel != CB_ERR) {
                    current_graph_cat_idx = sel;
                    if (hGraphArea) InvalidateRect(hGraphArea, NULL, TRUE);
                }
            }

            if (HIWORD(wParam) == LBN_SELCHANGE && LOWORD(wParam) == ID_LIST_HISTORY) {
                int sel = SendMessage(hHistoryList, LB_GETCURSEL, 0, 0);
                if (sel != LB_ERR && sel < users[active_user].history_count) {
                    SendMessage(hTxnDetailsList, LB_RESETCONTENT, 0, 0);
                    char header[150];
                    sprintf(header, "%-20s | %-5s | %-12s | %-12s", "Item Name", "Qty", "Price/Item", "Total Price");
                    SendMessage(hTxnDetailsList, LB_ADDSTRING, 0, (LPARAM)header);
                    SendMessage(hTxnDetailsList, LB_ADDSTRING, 0, (LPARAM)"-------------------------------------------------------------");

                    char filepath[100];
                    sprintf(filepath, "data/txn_details_%s_%d.txt", users[active_user].uname, sel);
                    FILE *file = fopen(filepath, "r");
                    if (file) {
                        char line[200];
                        while (fgets(line, sizeof(line), file)) {
                            char iName[100];
                            int iQty, iUnit, iTotal;
                            if (sscanf(line, "%[^,],%d,%d,%d", iName, &iQty, &iUnit, &iTotal) == 4) {
                                char formatted[150];
                                sprintf(formatted, "%-20.20s | %-5d | Rs. %-8d | Rs. %-8d", iName, iQty, iUnit, iTotal);
                                SendMessage(hTxnDetailsList, LB_ADDSTRING, 0, (LPARAM)formatted);
                            }
                        }
                        fclose(file);
                    }
                    SendMessage(hTxnDetailsList, LB_ADDSTRING, 0, (LPARAM)"-------------------------------------------------------------");
                    char footer[150];
                    sprintf(footer, "%-20s   %-5s   %-12s | Rs. %-8d", "", "", "GRAND TOTAL", users[active_user].history[sel].amount_spent);
                    SendMessage(hTxnDetailsList, LB_ADDSTRING, 0, (LPARAM)footer);
                }
            }

            if (LOWORD(wParam) == ID_BTN_QUIT) {
                PostQuitMessage(0);
            }
            else if (LOWORD(wParam) == ID_BTN_LOGIN) {
                char name[100], pass[100];
                GetWindowText(hUsername, name, 100);
                GetWindowText(hPass, pass, 100);

                int found = 0;
                for (int i = 0; i < 2; i++) { 
                    if (strcmp(users[i].uname, name) == 0 && strcmp(users[i].password, pass) == 0) {
                        active_user = i;
                        found = 1;
                        break;
                    }
                }

                if (found) {
                    ClearLoginView();
                    BuildDashboardLayout(hwnd);
                } else {
                    MessageBox(hwnd, "Invalid Username or Password.", "Login Failed", MB_OK | MB_ICONERROR);
                }
            }

            else if (LOWORD(wParam) == ID_BTN_LOGOUT) {
                if (MessageBox(hwnd, "Are you sure you want to log out?", "Logout", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                    ClearDashboard();
                    active_user = -1;
                    cart_total = 0;
                    cart_items_count = 0;
                    ShowLoginView(hwnd);
                }
            }

            else if (LOWORD(wParam) == ID_BTN_REMOVE_CART_ITEM) {
                int sel = SendMessage(hCartList, LB_GETCURSEL, 0, 0);
                if (sel != LB_ERR) {
                    cart_total -= cart_item_prices[sel];
                    SendMessage(hCartList, LB_DELETESTRING, sel, 0);
                    for(int i = sel; i < cart_items_count - 1; i++) {
                        cart_item_prices[i] = cart_item_prices[i+1];
                        cart_item_cat_idx[i] = cart_item_cat_idx[i+1];
                        cart_item_shop_idx[i] = cart_item_shop_idx[i+1];
                    }
                    cart_items_count--;
                    UpdateFinancialLabels();
                } else {
                    MessageBox(hwnd, "Please select an item to remove.", "Remove Item", MB_OK);
                }
            }

            else if (LOWORD(wParam) == ID_BTN_CLEAR_CART) {
                if (cart_total > 0) {
                    if (MessageBox(hwnd, "Are you sure you want to empty your cart?", "Clear Cart", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        cart_total = 0; 
                        cart_items_count = 0;
                        SendMessage(hCartList, LB_RESETCONTENT, 0, 0); 
                        UpdateFinancialLabels(); 
                    }
                } else {
                    MessageBox(hwnd, "Your cart is already empty.", "Clear Cart", MB_OK);
                }
            }

            else if (LOWORD(wParam) == ID_BTN_PROFILE) { ShowProfileView(hwnd); }
            else if (LOWORD(wParam) == ID_BTN_HISTORY) { ShowHistoryView(hwnd); }
            else if (LOWORD(wParam) == ID_BTN_GRAPHS) { ShowGraphView(hwnd); }
            else if (LOWORD(wParam) == ID_BTN_CLEAR_HISTORY) {
                if (users[active_user].history_count > 0) {
                    if (MessageBox(hwnd, "Are you sure you want to clear all transaction history?", "Clear History", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        users[active_user].history_count = 0;
                        char filepath[100];
                        sprintf(filepath, "data/history_%s.txt", users[active_user].uname);
                        FILE *file = fopen(filepath, "w");
                        if (file) fclose(file);
                        
                        SendMessage(hHistoryList, LB_RESETCONTENT, 0, 0);
                        SendMessage(hHistoryList, LB_ADDSTRING, 0, (LPARAM)"No past transactions found.");
                        MessageBox(hwnd, "History cleared successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                    }
                } else {
                    MessageBox(hwnd, "Your history is already empty.", "Clear History", MB_OK);
                }
            }
            
            else if (LOWORD(wParam) == ID_BTN_SAVE_PROF) {
                char newPass[100];
                char newBudgStr[50];
                GetWindowText(hTxtNewPass, newPass, 100);
                GetWindowText(hTxtNewBudg, newBudgStr, 50);

                if (strlen(newPass) > 0) { strcpy(users[active_user].password, newPass); }
                if (strlen(newBudgStr) > 0) { users[active_user].budget = atoi(newBudgStr); }

                save_user_profile(active_user);
                UpdateFinancialLabels();
                SetWindowText(hTxtNewPass, ""); 
                MessageBox(hwnd, "Profile Settings Updated Successfully!", "Success", MB_OK | MB_ICONINFORMATION);
            }
            
            else if (LOWORD(wParam) >= ID_BTN_CAT_0 && LOWORD(wParam) <= ID_BTN_CAT_3) {
                int selected_cat = LOWORD(wParam) - ID_BTN_CAT_0; 
                ShowCategoryView(hwnd, selected_cat);
            }
            
            else if (LOWORD(wParam) == ID_BTN_CHECKOUT) {
                if (cart_total == 0) {
                    MessageBox(hwnd, "Your cart is empty!", "Checkout", MB_OK);
                } else {
                    char msg[150];
                    sprintf(msg, "Total to Pay: Rs. %d\nProceed?", cart_total);
                    if (MessageBox(hwnd, msg, "Checkout", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        
                        users[active_user].budget -= cart_total; 
                        save_user_profile(active_user);
                        
                        int h_idx = users[active_user].history_count;
                        if (h_idx < 50) { 
                            time_t t = time(NULL);
                            struct tm tm = *localtime(&t);
                            
                            sprintf(users[active_user].history[h_idx].date_time, "%02d-%02d-%04d %02d:%02d:%02d", 
                                    tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
                            
                            users[active_user].history[h_idx].amount_spent = cart_total;
                            users[active_user].history_count++;

                            // Save to history file
                            char filepath[100];
                            sprintf(filepath, "data/history_%s.txt", users[active_user].uname);
                            FILE *f = fopen(filepath, "a");
                            if (f) {
                                fprintf(f, "%s,%d\n", users[active_user].history[h_idx].date_time, cart_total);
                                fclose(f);
                            }

                            // Save transaction details
                            char txnpath[100];
                            sprintf(txnpath, "data/txn_details_%s_%d.txt", users[active_user].uname, h_idx);
                            FILE *txnfile = fopen(txnpath, "w");
                            if (txnfile) {
                                for(int i=0; i<cart_items_count; i++) {
                                    fprintf(txnfile, "%s,%d,%d,%d\n", cart_item_names[i], cart_item_qtys[i], cart_item_unit_prices[i], cart_item_prices[i]);
                                }
                                fclose(txnfile);
                            }
                        }

                        for(int i=0; i<cart_items_count; i++) {
                            users[active_user].shop_spent[cart_item_cat_idx[i]][cart_item_shop_idx[i]] += cart_item_prices[i];
                        }
                        save_user_stats(active_user);

                        cart_total = 0;
                        cart_items_count = 0;
                        SendMessage(hCartList, LB_RESETCONTENT, 0, 0); 
                        UpdateFinancialLabels();
                        MessageBox(hwnd, "Payment Successful! Transaction logged.", "Success", MB_OK | MB_ICONINFORMATION);
                    }
                }
            }

            else if (LOWORD(wParam) == ID_BTN_ADD_CART) {
                if (current_cat_idx == -1 || current_shop_idx == -1) {
                    MessageBox(hwnd, "Please select a shop first.", "Error", MB_OK | MB_ICONERROR);
                } else {
                    char name[100];
                    char priceStr[50];
                    char qtyStr[20];
                    GetWindowText(hTxtItemName, name, 100);
                    GetWindowText(hTxtItemPrice, priceStr, 50);
                    GetWindowText(hTxtItemQty, qtyStr, 20);
                    
                    int unit_cost = atoi(priceStr);
                    int qty = atoi(qtyStr);
                    
                    if (qty <= 0) qty = 1;
                    int total_cost = unit_cost * qty;

                    if (strlen(name) == 0 || unit_cost <= 0) {
                        MessageBox(hwnd, "Please enter a valid item name and price.", "Error", MB_OK | MB_ICONERROR);
                    } else if (cart_total + total_cost > users[active_user].budget) {
                        MessageBox(hwnd, "BUDGET EXCEEDED!", "Warning", MB_OK | MB_ICONWARNING);
                    } else {
                        struct Shop *shop = &categories[current_cat_idx].shops[current_shop_idx];
                        
                        // Check if item exists in shop
                        int found = 0;
                        for(int i=0; i < shop->item_count; i++) {
                            if(stricmp(shop->items[i], name) == 0) {
                                found = 1;
                                break;
                            }
                        }
                        
                        if(!found && shop->item_count < 100) {
                            // Add to array
                            strcpy(shop->items[shop->item_count], name);
                            shop->prices[shop->item_count] = unit_cost;
                            shop->item_count++;
                            
                            // Append to file
                            char filepath[150];
                            sprintf(filepath, "data/%s/%s.txt", categories[current_cat_idx].name, shop->name);
                            FILE *file = fopen(filepath, "a");
                            if(file) {
                                fprintf(file, "%s,%d\n", name, unit_cost);
                                fclose(file);
                            }

                            // Add to the ComboBox visually
                            SendMessage(hComboItems, CB_ADDSTRING, 0, (LPARAM)name);
                        }

                        if (cart_items_count < 100) {
                            strcpy(cart_item_names[cart_items_count], name);
                            cart_item_qtys[cart_items_count] = qty;
                            cart_item_unit_prices[cart_items_count] = unit_cost;
                            cart_item_prices[cart_items_count] = total_cost;
                            cart_item_cat_idx[cart_items_count] = current_cat_idx;
                            cart_item_shop_idx[cart_items_count] = current_shop_idx;
                            cart_items_count++;
                        }

                        cart_total += total_cost;
                        char listItem[200];
                        sprintf(listItem, "%s (x%d from %s - %s) - Rs. %d", name, qty, categories[current_cat_idx].name, shop->name, total_cost);
                        SendMessage(hCartList, LB_ADDSTRING, 0, (LPARAM)listItem);
                        UpdateFinancialLabels();
                    }
                }
            }
            return 0;

        case WM_DESTROY:
            // Prevent memory leaks by deleting our custom brushes and fonts when the app closes
            DeleteObject(hBgBrush);
            DeleteObject(hInputBrush);
            DeleteObject(hTitleFont);
            if (hMonoFont) DeleteObject(hMonoFont);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ==========================================
// ENTRY POINT
// ==========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    initialize_data(); 

    // Create our Dark Mode Brushes
    hBgBrush = CreateSolidBrush(RGB(10, 15, 40));    // Very Dark Blue Background
    hInputBrush = CreateSolidBrush(RGB(30, 40, 70)); // Slate Blue for text inputs and cart

    const char CLASS_NAME[] = "BudgetBrakeClass";
    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WindowProc;       
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = hBgBrush; // Apply the dark background to the main window

    RegisterClass(&wc);

    WNDCLASS wcGraph = {0};
    wcGraph.lpfnWndProc = GraphWndProc;
    wcGraph.hInstance = hInstance;
    wcGraph.lpszClassName = "GraphControl";
    wcGraph.hbrBackground = NULL;
    RegisterClass(&wcGraph);

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "BudgetBrake - Shop till the Budget Stops", 
        WS_OVERLAPPEDWINDOW, 
        0, 0, screenW, screenH, 
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;
    
    ShowWindow(hwnd, SW_SHOWMAXIMIZED);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
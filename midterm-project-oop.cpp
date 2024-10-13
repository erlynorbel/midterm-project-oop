#include <iostream>
#include <string>
#include <cctype> //isdigit
#include <iomanip>
#include <regex>

using namespace std;

// System Clear, Pause, Numeric Checker
void SystemClear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
};

void SystemPause() {
#ifdef _WIN32
    system("pause");
#else
    cout << "Press enter to continue..." << endl;
    string temp;
    getline(cin, temp);
#endif
}

bool isValidNumericString(const string &input) {
    bool decimal = false;

    for (const char ch: input) {
        if (ch == '.') {
            if (decimal) // Only allow one decimal point
            {
                return false;
            }
            decimal = true;
        } else if (!isdigit(ch)) // Check if each character is a digit
        {
            return false;
        }
    }
    return !input.empty(); // Return false if the string is empty
}

bool isValidString(const std::string &str) {
    return !str.empty();
}

bool hasNoWhitespace(const std::string &str) {
    for (const char c: str) {
        if (c == ' ') {
            return false;
        }
    }
    return true;
}


bool isNumeric(const std::string &str) {
    for (char const &c: str) {
        if (std::isdigit(c) == 0) {
            return false;
        }
    }
    return true;
}

string toLowerCase(const string &str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

class Item {
private:
    string id;
    string name;
    int quantity;
    double price;
    string category;

public:
    // Constructor
    Item(string id, string name, int quantity, double price, string category)
        : id(id), name(name), quantity(quantity), price(price), category(category) {
    }

    // Getter and Setter functions to encapsulate data
    string getId() const { return id; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    string getCategory() const { return category; }

    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setPrice(double newPrice) { price = newPrice; }

    // Display the item's details
    void displayItem() const {
        cout <<
                setw(8) << left << id <<
                setw(12) << left << name <<
                setw(8) << right << quantity <<
                setw(6) << right << price <<
                setw(12) << right << category
                << endl;
    }

    // Validation for low stock
    bool isLowStock() const { return quantity <= 5; }
};


class BaseInventory {
protected:
    Item *items[100] = {};
    int itemCount = 0;

public:
    BaseInventory() = default;

    virtual ~BaseInventory() = default;

    static bool isValidCategory(const string &category) {
        string lowerCaseCategory = toLowerCase(category);
        return (lowerCaseCategory == "clothing" || lowerCaseCategory == "electronics" || lowerCaseCategory ==
                "entertainment");
    }

    int findItemIndexById(const string &id) const {
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->getId() == id) {
                return i;
            }
        }
        return -1;
    }

    bool isEmpty() {
        for (const Item *item: items) {
            if (item != nullptr) {
                return false;
            }
        }

        return true;
    }

    // virtual methods
    virtual void addItem() = 0;

    virtual void removeItem() = 0;

    virtual void updateItem() = 0;

    virtual void displayItemsByCategory() = 0;

    virtual void searchItem() = 0;

    virtual void sortItems() = 0;

    virtual void displayAllItems() = 0;

    virtual void displayLowStockItems() = 0;
};

// Inventory Management System class
class Inventory : public BaseInventory {
public:
    void addTestItems() {
        items[itemCount++] = new Item("C1", "Shorts", 20, 100, "clothing");
        items[itemCount++] = new Item("C2", "Shirt", 5, 120, "clothing");
    }


    // Add an item
    void addItem() override {
        SystemClear();

        cout << "Add Item Menu" << endl;

        string id, name, category;
        string quantity;
        string price;

        cout << "Enter category (Clothing, Electronics, Entertainment): ";
        cin >> category;


        if (!isValidCategory(category)) {
            SystemClear();
            cout << "Category " << category << " does not exist!" << endl;
            cout << "Please enter a valid category." << endl << endl;
            cin.ignore();
            SystemPause();

            addItem();
            return;
        }


        bool isIdNotValid = true;
        cin.ignore();
        while (isIdNotValid) {
            SystemClear();

            cout << "Enter ID: ";
            getline(cin, id);


            if (isValidString(id) && hasNoWhitespace(id)) {
                isIdNotValid = false;
            } else {
                SystemClear();
                cout << "Invalid id. Please try again.\n";
                SystemPause();
            }
        }


        cout << "Enter name: ";
        cin >> name;

        cin.ignore();

        bool isQuantityNotValid = true;
        while (isQuantityNotValid) {
            SystemClear();

            cout << "Enter quantity: ";
            getline(cin, quantity);

            if (isNumeric(quantity)) {
                isQuantityNotValid = false;
            } else {
                SystemClear();
                cout << "Invalid Quantity. Please try again." << endl;
                SystemPause();
            }
        }

        bool isPriceNotValid = true;
        while (isPriceNotValid) {
            SystemClear();

            cout << "Enter price: ";
            getline(cin, price);

            if (isValidNumericString(price)) {
                isPriceNotValid = false;
            } else {
                SystemClear();
                cout << "Invalid Price. Please try again." << endl;
                SystemPause();
            }
        }

        Item *item = new Item(id, name, stoi(quantity), stod(price), category);
        items[itemCount++] = item;

        SystemClear();
        cout << "Item added successfully!" << endl;
        SystemPause();
    }

    // Update an item
    void updateItem() override {
        if (displayIfEmpty()) {
            return;
        }


        string id;
        cout << "Enter item ID: ";
        cin >> id;

        const int index = findItemIndexById(id);
        if (index == -1) {
            SystemClear();
            cout << "Item not found!" << endl;
            cin.ignore();
            SystemPause();
            return;
        }

        string choice;
        while (choice != "quantity" && choice != "price") {
            cout << "Do you want to update quantity or price? ";
            cin >> choice;
            toLowerCase(choice);

            if (choice != "quantity" && choice != "price") {
                SystemClear();

                cout << "Invalid choice. Try again (quantity, price)" << endl;
            }
        }


        if (choice == "quantity") {
            int newQuantity;
            cout << "Enter new quantity: ";
            cin >> newQuantity;
            int oldQuantity = items[index]->getQuantity();
            items[index]->setQuantity(newQuantity);
            cout << "Quantity of Item " << items[index]->getName() << " updated from " << oldQuantity << " to " <<
                    newQuantity << endl;
        } else if (choice == "price") {
            double newPrice;
            cout << "Enter new price: ";
            cin >> newPrice;
            double oldPrice = items[index]->getPrice();
            items[index]->setPrice(newPrice);
            cout << "Price of Item " << items[index]->getName() << " updated from " << oldPrice << " to " << newPrice <<
                    endl;
        } else {
            cout << "Invalid choice!" << endl;
        }

        cin.ignore();
        SystemPause();
    }

    bool displayIfEmpty() {
        if (isEmpty()) {
            SystemClear();

            cout << "No items in the inventory.\n";

            SystemPause();
            return true;
        }

        return false;
    }

    // Remove an item
    void removeItem() override {
        if (displayIfEmpty()) {
            return;
        }
        string id;
        cout << "Enter item ID: ";
        cin >> id;

        const int index = findItemIndexById(id);
        if (index == -1) {
            cout << "Item not found!" << endl;
            return;
        }

        cout << "Item " << items[index]->getName() << " has been removed from the inventory" << endl;

        // Shift items to fill the gap
        for (int i = index; i < itemCount - 1; i++) {
            items[i] = items[i + 1];
        }
        itemCount--;
    }

    // Display items by category
    void displayItemsByCategory() override {
        if (displayIfEmpty()) return;
        SystemClear();

        string category;
        cout << "Enter category (Clothing, Electronics, Entertainment): ";
        cin >> category;

        toLowerCase(category);

        if (!isValidCategory(category)) {
            cout << "Category " << category << " does not exist!" << endl;
            SystemPause();
            return;
        }

        displayTableHeader();
        bool hasDisplayedAtleastOnce = false;
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->getCategory() == category) {
                hasDisplayedAtleastOnce = true;
                items[i]->displayItem();
            }
        }

        if (!hasDisplayedAtleastOnce) {
            SystemClear();
            cout << "Items in category " << category << " is empty.\n";
        }

        cin.ignore();
        SystemPause();
    }

    // Display all items
    void displayAllItems() override {
        if (displayIfEmpty()) return;

        displayTableHeader();
        for (int i = 0; i < itemCount; i++) {
            items[i]->displayItem();
        }

        SystemPause();
    }

    void displayTableHeader() {
        cout <<
                setw(8) << left << "Id" <<
                setw(12) << left << "Name" <<
                setw(8) << right << "Quantity" <<
                setw(6) << right << "Price" <<
                setw(12) << right << "Category" << endl;
    }

    // Search item by ID
    void searchItem() override {
        if (displayIfEmpty()) return;

        string id;
        cout << "Enter item ID: ";
        cin >> id;

        int index = findItemIndexById(id);
        if (index == -1) {
            cout << "Item not found!" << endl;
        } else {
            displayTableHeader();
            items[index]->displayItem();
        }
    }

    // Display low stock items
    void displayLowStockItems() override {
        if (displayIfEmpty()) return;

        displayTableHeader();
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->isLowStock()) {
                items[i]->displayItem();
            }
        }
    }

    // Sort items (can be by quantity or price, and ascending or descending)
    void sortItems() override {
        SystemClear();
        string sortBy, order;

        bool isValidCategory = false;
        while (!isValidCategory) {
            cout << "Sort by (quantity/price): ";
            getline(cin, sortBy);

            toLowerCase(sortBy);

            if (sortBy == "quantity" || sortBy == "price") {
                isValidCategory = true;
            } else {
                SystemClear();
                cout << "Invalid category. Please try again.\n";
                SystemPause();
            }
        }

        bool isValidOrder = false;
        while (!isValidOrder) {
            cout << "Order (asc/desc): ";
            getline(cin, order);
            toLowerCase(order);

            if (order == "asc" || order == "desc") {
                isValidOrder = true;
            } else {
                SystemClear();
                cout << "Invalid category. Please try again.\n";
                SystemPause();
            }
        }


        // Bubble sort based on quantity or price
        for (int i = 0; i < itemCount - 1; i++) {
            for (int j = 0; j < itemCount - i - 1; j++) {
                bool condition = false;

                if (sortBy == "quantity") {
                    if (order == "asc") {
                        condition = items[j]->getQuantity() > items[j + 1]->getQuantity();
                    } else if (order == "desc") {
                        condition = items[j]->getQuantity() < items[j + 1]->getQuantity();
                    }
                } else if (sortBy == "price") {
                    if (order == "asc") {
                        condition = items[j]->getPrice() > items[j + 1]->getPrice();
                    } else if (order == "desc") {
                        condition = items[j]->getPrice() < items[j + 1]->getPrice();
                    }
                }

                if (condition) {
                    // Swap
                    Item *temp = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = temp;
                }
            }
        }

        displayAllItems();
    }
};


int main() {
    Inventory inventory;
    int choice;

    inventory.addTestItems();

    bool userContinue = true;
    while (userContinue) {
        SystemClear();

        cout << "\nMenu\n";
        cout << "1 - Add Item\n";
        cout << "2 - Update Item\n";
        cout << "3 - Remove Item\n";
        cout << "4 - Display Items by Category\n";
        cout << "5 - Display All Items\n";
        cout << "6 - Search Item\n";
        cout << "7 - Sort Items\n";
        cout << "8 - Display Low Stock Items\n";
        cout << "9 - Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        isValidNumericString(to_string(choice));


        //make a validation for the input choice to be between 1 and 9
        if (choice < 1 || choice > 9) {
            cout << "Invalid choice! Please try again." << endl;
            continue;
        }

        cin.ignore();

        switch (choice) {
            case 1:
                inventory.addItem();
                break;
            case 2:
                inventory.updateItem();
                break;
            case 3:
                inventory.removeItem();
                break;
            case 4:
                inventory.displayItemsByCategory();
                break;
            case 5:
                inventory.displayAllItems();
                break;
            case 6:
                inventory.searchItem();
                break;
            case 7:
                inventory.sortItems();
                break;
            case 8:
                inventory.displayLowStockItems();
                break;
            case 9:
                cout << "Exiting program..." << endl;
                return 0; // Exits the program
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}

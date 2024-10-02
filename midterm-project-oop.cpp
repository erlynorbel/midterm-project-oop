#include <iostream>
#include <string>

using namespace std;

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
        : id(id), name(name), quantity(quantity), price(price), category(category) {}

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
        cout << id << "\t" << name << "\t" << quantity << "\t" << price << "\t" << category << endl;
    }

    // Validation for low stock
    bool isLowStock() const { return quantity <= 5; }
};

// Inventory Management System class
class Inventory {
private:
    Item* items[100];
    int itemCount;

    bool isValidCategory(const string& category) const {
        return (category == "Clothing" || category == "Electronics" || category == "Entertainment");
    }

    int findItemIndexById(const string& id) const {
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->getId() == id) {
                return i;
            }
        }
        return -1;
    }

public:
    // Constructor
    Inventory() : itemCount(0) {}

    // Add an item
    void addItem() {
        string id, name, category;
        int quantity;
        double price;

        cout << "Enter category: ";
        cin >> category;

        if (!isValidCategory(category)) {
            cout << "Category " << category << " does not exist!" << endl;
            return;
        }

        cout << "Enter ID: ";
        cin >> id;
        cout << "Enter name: ";
        cin >> name;
        cout << "Enter quantity: ";
        cin >> quantity;
        cout << "Enter price: ";
        cin >> price;

        items[itemCount++] = new Item(id, name, quantity, price, category);
        cout << "Item added successfully!" << endl;
    }

    // Update an item
    void updateItem() {
        string id;
        cout << "Enter item ID: ";
        cin >> id;

        int index = findItemIndexById(id);
        if (index == -1) {
            cout << "Item not found!" << endl;
            return;
        }

        string choice;
        cout << "Do you want to update quantity or price? ";
        cin >> choice;

        if (choice == "quantity") {
            int newQuantity;
            cout << "Enter new quantity: ";
            cin >> newQuantity;
            int oldQuantity = items[index]->getQuantity();
            items[index]->setQuantity(newQuantity);
            cout << "Quantity of Item " << items[index]->getName() << " updated from " << oldQuantity << " to " << newQuantity << endl;
        } else if (choice == "price") {
            double newPrice;
            cout << "Enter new price: ";
            cin >> newPrice;
            double oldPrice = items[index]->getPrice();
            items[index]->setPrice(newPrice);
            cout << "Price of Item " << items[index]->getName() << " updated from " << oldPrice << " to " << newPrice << endl;
        } else {
            cout << "Invalid choice!" << endl;
        }
    }

    // Remove an item
    void removeItem() {
        string id;
        cout << "Enter item ID: ";
        cin >> id;

        int index = findItemIndexById(id);
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
    void displayItemsByCategory() {
        string category;
        cout << "Enter category: ";
        cin >> category;

        if (!isValidCategory(category)) {
            cout << "Category " << category << " does not exist!" << endl;
            return;
        }

        cout << "ID\tName\tQuantity\tPrice\tCategory" << endl;
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->getCategory() == category) {
                items[i]->displayItem();
            }
        }
    }

    // Display all items
    void displayAllItems() {
        cout << "ID\tName\tQuantity\tPrice\tCategory" << endl;
        for (int i = 0; i < itemCount; i++) {
            items[i]->displayItem();
        }
    }

    // Search item by ID
    void searchItem() {
        string id;
        cout << "Enter item ID: ";
        cin >> id;

        int index = findItemIndexById(id);
        if (index == -1) {
            cout << "Item not found!" << endl;
        } else {
            items[index]->displayItem();
        }
    }

    // Display low stock items
    void displayLowStockItems() {
        cout << "ID\tName\tQuantity\tPrice\tCategory" << endl;
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->isLowStock()) {
                items[i]->displayItem();
            }
        }
    }

    // Sort items (can be by quantity or price, and ascending or descending)
    void sortItems() {
        string sortBy, order;
        cout << "Sort by (quantity/price): ";
        cin >> sortBy;
        cout << "Order (asc/desc): ";
        cin >> order;

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
                    Item* temp = items[j];
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

    do {
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
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }

    } while (choice != 9);

    return 0;
}

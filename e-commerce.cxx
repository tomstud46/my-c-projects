{#include <iostream>
using namespace std;
#define MAX_SIZE 100
// Stack implementation
struct Stack {
    string data[MAX_SIZE];
    int top = -1;
    void push(string item) {
        if (top >= MAX_SIZE - 1) {
            cout << "Stack Overflow.\n";
            return;
        }
        data[++top] = item;
    }
    string pop() {
        if (top < 0) {
            cout << "Stack Underflow.\n";
            return "";
        }
        return data[top--];
    }
    bool isEmpty() {
        return top == -1;
    }
};
// Queue implementation
struct Queue {
    string data[MAX_SIZE];
    int front = 0, rear = -1, size = 0;
    void enqueue(string item) {
        if (size >= MAX_SIZE) {
            cout << "Queue Overflow.\n";
            return;
        }
        rear = (rear + 1) % MAX_SIZE;
        data[rear] = item;
        size++;
    }
    string dequeue() {
        if (size == 0) {
            cout << "Queue Underflow.\n";
            return "";
        }
        string item = data[front];
        front = (front + 1) % MAX_SIZE;
        size--;
        return item;
    }
    bool isEmpty() {
        return size == 0;
    }
};
// Main program
int main() {
    Stack undoStack, redoStack;
    Queue checkoutQueue;
    string cart[MAX_SIZE];
    int cartSize = 0;
    int choice;
    string item;
    do {
        cout << "\nE-Commerce Cart Management\n";
        cout << "1. Add Item to Cart\n";
        cout << "2. Remove Last Item from Cart\n";
        cout << "3. Undo Last Action\n";
        cout << "4. Redo Last Action\n";
        cout << "5. View Cart\n";
        cout << "6. Add Cart to Checkout \n";
        cout << "7. Process Checkout\n";
        cout << "8. View Checkout \n";
        cout << "9. Enter any other key to exit except 1-8\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: // Add item
                cout << "Enter item name: ";
                cin >> item;
                if (cartSize < MAX_SIZE) {
                    cart[cartSize++] = item;
                    undoStack.push("add " + item);
                    while (!redoStack.isEmpty()) redoStack.pop(); // Clear redo stack
                    cout << "Item added to cart.\n";
                } else {
                    cout << "Cart is full.\n";
                }
                break;
            case 2: // Remove last item
                if (cartSize > 0) {
                    item = cart[--cartSize];
                    undoStack.push("remove " + item);
                    while (!redoStack.isEmpty()) redoStack.pop(); // Clear redo stack
                    cout << "Last item removed from cart.\n";
                } else {
                    cout << "Cart is empty.\n";
                }
                break;
            case 3: // Undo last action
                if (!undoStack.isEmpty()) {
                    string action = undoStack.pop();
                    string type = action.substr(0, action.find(" "));
                    string item = action.substr(action.find(" ") + 1);
                    if (type == "add") {
                        cartSize--;
                        redoStack.push(action);
                        cout << "Undo: Removed " << item << " from cart.\n";
                    } else if (type == "remove") {
                        cart[cartSize++] = item;
                        redoStack.push(action);
                        cout << "Undo: Added " << item << " back to cart.\n";
                    }
                } else {
                    cout << "Nothing to undo.\n";
                }
                break;
            case 4: // Redo last undone action
                if (!redoStack.isEmpty()) {
                    string action = redoStack.pop();
                    string type = action.substr(0, action.find(" "));
                    string item = action.substr(action.find(" ") + 1);
                    if (type == "add") {
                        cart[cartSize++] = item;
                        undoStack.push(action);
                        cout << "Redo: Added " << item << " to cart.\n";
                    } else if (type == "remove") {
                        cartSize--;
                        undoStack.push(action);
                        cout << "Redo: Removed " << item << " from cart.\n";
                    }
                } else {
                    cout << "Nothing to redo.\n";
                }
                break;
            case 5: // View cart
                if (cartSize > 0) {
                    cout << "Current Cart Contents:\n";
                    for (int i = 0; i < cartSize; i++) {
                        cout << "- " << cart[i] << endl;
                    }
                } else {
                    cout << "Cart is empty.\n";
                }
                break;
            case 6: // Add cart to checkout queue
                if (cartSize > 0) {
                    string order = "Order: ";
                    for (int i = 0; i < cartSize; i++) {
                        order += cart[i] + " ";
                    }
                    checkoutQueue.enqueue(order);
                    cartSize = 0; // Clear the cart
                    cout << "Cart added to checkout queue.\n";
                } else {
                    cout << "Cart is empty. Nothing to add to checkout queue.\n";
                }
                break;
            case 7: // Process checkout
                if (!checkoutQueue.isEmpty()) {
                    cout << "Processing " << checkoutQueue.dequeue() << endl;
                } else {
                    cout << "Checkout queue is empty.\n";
                }
                break;
            case 8: // View checkout queue
                if (!checkoutQueue.isEmpty()) {
                    cout << "Checkout Queue:\n";
                    int tempSize = checkoutQueue.size;
                    int tempFront = checkoutQueue.front;
                    for (int i = 0; i < tempSize; i++) {
                        cout << "- " << checkoutQueue.data[(tempFront + i) % MAX_SIZE] << endl;
                    }
                } else {
                    cout << "Checkout queue is empty.\n";
                }
                break;
            case 9: // Exit
                cout << "Exiting program sucessful.\n";

                break;
            default:
                cout << "Exiting program sucessful.\n";
                choice = 9;
                break;

        }
    } while (choice != 9   );
    return 0;
}

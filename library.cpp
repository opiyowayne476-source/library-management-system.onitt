#include <iostream>
#include <string>

using namespace std;

//  Book CLASS 
class Book {
private:
    string title;
    string author;
    bool available;
    string borrowDate;
    string returnDate;

public:
    // Constructor
    Book() {
        available = true;
    }

    void setBook(string t, string a) {
        title = t;
        author = a;
        available = true;
    }

    string getTitle() {
        return title;
    }

    string getAuthor() {
        return author;
    }

    bool isAvailable() {
        return available;
    }

    void borrow(string date) {
        available = false;
        borrowDate = date;
        returnDate = "Not returned";
    }

    void giveBack(string date) {
        available = true;
        returnDate = date;
    }
};

// User CLASS 
class User {
private:
    string username;
    string password;

public:
    void setUser(string u, string p) {
        username = u;
        password = p;
    }

    bool login(string u, string p) {
        if (username == u && password == p)
            return true;
        else
            return false;
    }
};

//  Library CLASS 
class Library {
private:
    Book books[3];
    User user;
    bool loggedIn;

public:
    Library() {
        loggedIn = false;

        books[0].setBook("Anatomy", "Jonny");
        books[1].setBook("Data Structures", "Mark");
        books[2].setBook("Algorithms", "CLRS");
    }

    void registerUser(string u, string p) {
        user.setUser(u, p);
        cout << "User registered successfully\n";
    }

    void loginUser(string u, string p) {
        if (user.login(u, p)) {
            loggedIn = true;
            cout << "Login successful\n";
        } else {
            cout << "Login failed\n";
        }
    }

    void showBooks() {
        cout << "\nBOOK LIST\n";
        for (int i = 0; i < 3; i++) {
            cout << (i + 1) << ". "
                 << books[i].getTitle() << " - "
                 << books[i].getAuthor() << " - ";
            if (books[i].isAvailable())
                cout << "Available\n";
            else
                cout << "Borrowed\n";
        }
    }

    void borrowBook(int number, string date) {
        if (!loggedIn) {
            cout << "Please login first\n";
            return;
        }

        if (books[number - 1].isAvailable()) {
            books[number - 1].borrow(date);
            cout << "Book borrowed successfully\n";
        } else {
            cout << "Book already borrowed\n";
        }
    }

    void returnBook(int number, string date) {
        books[number - 1].giveBack(date);
        cout << "Book returned successfully\n";
    }
};

// MAIN FUNCTION 
int main() {
    Library lib;
    int choice, bookNumber;
    string username, password, date;

    while (true) {
        cout << "\n1 Register\n2 Login\n3 View Books\n4 Borrow Book\n5 Return Book\n0 Exit\nChoice: ";
        cin >> choice;

        if (choice == 0)
            break;

        if (choice == 1) {
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            lib.registerUser(username, password);
        }

        else if (choice == 2) {
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            lib.loginUser(username, password);
        }

        else if (choice == 3) {
            lib.showBooks();
        }

        else if (choice == 4) {
            cout << "Enter book number: ";
            cin >> bookNumber;
            cout << "Borrow date: ";
            cin >> date;
            lib.borrowBook(bookNumber, date);
        }

        else if (choice == 5) {
            cout << "Enter book number: ";
            cin >> bookNumber;
            cout << "Return date: ";
            cin >> date;
            lib.returnBook(bookNumber, date);
        }
    }

    return 0;
}
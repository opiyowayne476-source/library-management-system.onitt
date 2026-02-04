#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype>

using namespace std;

// ===================== Book =====================
struct Book {
    string title;
    string author;
    string isbn;
    bool available;
    string borrowedBy;
};

// ===================== User =====================
struct User {
    string username;
    string password;
    vector<string> borrowedISBNs;
};

// ===================== Library =====================
class Library {
private:
    vector<Book> books;
    vector<User> users;
    User* currentUser;

public:
    Library() {
        currentUser = NULL;

        addBook("Anatomy and Physiology", "Fundamentals of Nursing", "978-0321563842");
        addBook("Clean Code", "Robert C. Martin", "978-0132350884");
        addBook("Design Patterns", "Gamma et al.", "978-0201633610");

        registerUser("student1", "pass123");
    }

    void addBook(const string& title, const string& author, const string& isbn) {
        Book b;
        b.title = title;
        b.author = author;
        b.isbn = isbn;
        b.available = true;
        b.borrowedBy = "";

        books.push_back(b);
    }

    Book* findBook(const string& isbn) {
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i].isbn == isbn)
                return &books[i];
        }
        return NULL;
    }

    void displayAllBooks() const {
        cout << "\nLibrary Catalog:\n";
        cout << left << setw(30) << "Title"
             << setw(25) << "Author"
             << setw(18) << "ISBN"
             << "Status\n";
        cout << string(80, '-') << "\n";

        for (size_t i = 0; i < books.size(); i++) {
            cout << left << setw(30) << books[i].title
                 << setw(25) << books[i].author
                 << setw(18) << books[i].isbn
                 << (books[i].available ? "Available" : "Borrowed")
                 << "\n";
        }
    }

    bool registerUser(const string& username, const string& password) {
        for (size_t i = 0; i < users.size(); i++) {
            if (users[i].username == username)
                return false;
        }

        User u;
        u.username = username;
        u.password = password;
        users.push_back(u);
        return true;
    }

    bool login(const string& username, const string& password) {
        for (size_t i = 0; i < users.size(); i++) {
            if (users[i].username == username &&
                users[i].password == password) {
                currentUser = &users[i];
                return true;
            }
        }
        return false;
    }

    void logout() {
        currentUser = NULL;
    }

    bool borrowBook(const string& isbn) {
        if (currentUser == NULL)
            return false;

        Book* b = findBook(isbn);
        if (b == NULL || !b->available)
            return false;

        b->available = false;
        b->borrowedBy = currentUser->username;
        currentUser->borrowedISBNs.push_back(isbn);
        return true;
    }

    bool returnBook(const string& isbn) {
        if (currentUser == NULL)
            return false;

        Book* b = findBook(isbn);
        if (b == NULL)
            return false;

        b->available = true;
        b->borrowedBy = "";

        for (size_t i = 0; i < currentUser->borrowedISBNs.size(); i++) {
            if (currentUser->borrowedISBNs[i] == isbn) {
                currentUser->borrowedISBNs.erase(
                    currentUser->borrowedISBNs.begin() + i);
                break;
            }
        }
        return true;
    }

    bool isLoggedIn() const {
        return currentUser != NULL;
    }

    string getCurrentUsername() const {
        if (currentUser)
            return currentUser->username;
        return "";
    }
};

// ===================== MAIN =====================
int main() {
    Library lib;
    string choice, u, p;

    while (true) {
        cout << "\n1. Login\n2. Register\n3. View Books\n0. Exit\nChoice: ";
        getline(cin, choice);

        if (choice == "0")
            break;

        if (choice == "1") {
            cout << "Username: ";
            getline(cin, u);
            cout << "Password: ";
            getline(cin, p);
            if (lib.login(u, p))
                cout << "Login successful\n";
            else
                cout << "Login failed\n";
        }
        else if (choice == "2") {
            cout << "Username: ";
            getline(cin, u);
            cout << "Password: ";
            getline(cin, p);
            lib.registerUser(u, p);
        }
        else if (choice == "3") {
            lib.displayAllBooks();
        }
    }

    return 0;
}
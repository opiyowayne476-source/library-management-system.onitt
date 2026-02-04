#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// ===================== Book =====================
struct Book {
    string title;
    string author;
    string isbn;
    bool available;
    string borrowedBy;
    string borrowDate;
    string returnDate;
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

        addBook("Clean Code", "Robert C. Martin", "978-0132350884");
        addBook("Design Patterns", "Gamma et al.", "978-0201633610");
        addBook("The Pragmatic Programmer", "Andrew Hunt", "978-0201616224");
    }

    void addBook(const string& title, const string& author, const string& isbn) {
        Book b;
        b.title = title;
        b.author = author;
        b.isbn = isbn;
        b.available = true;
        b.borrowedBy = "";
        b.borrowDate = "";
        b.returnDate = "";
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
        cout << "\nLibrary Books:\n";
        cout << left << setw(30) << "Title"
             << setw(20) << "Author"
             << setw(18) << "ISBN"
             << setw(12) << "Status"
             << "Dates\n";
        cout << string(95, '-') << "\n";

        for (size_t i = 0; i < books.size(); i++) {
            cout << left << setw(30) << books[i].title
                 << setw(20) << books[i].author
                 << setw(18) << books[i].isbn
                 << setw(12) << (books[i].available ? "Available" : "Borrowed");

            if (!books[i].available) {
                cout << "Borrowed: " << books[i].borrowDate
                     << " | Returned: " << books[i].returnDate;
            }
            cout << "\n";
        }
    }

    bool registerUser(const string& u, const string& p) {
        for (size_t i = 0; i < users.size(); i++) {
            if (users[i].username == u)
                return false;
        }
        User user;
        user.username = u;
        user.password = p;
        users.push_back(user);
        return true;
    }

    bool login(const string& u, const string& p) {
        for (size_t i = 0; i < users.size(); i++) {
            if (users[i].username == u && users[i].password == p) {
                currentUser = &users[i];
                return true;
            }
        }
        return false;
    }

    void logout() {
        currentUser = NULL;
    }

    bool borrowBook(const string& isbn, const string& date) {
        if (currentUser == NULL) return false;

        Book* b = findBook(isbn);
        if (b == NULL || !b->available) return false;

        b->available = false;
        b->borrowedBy = currentUser->username;
        b->borrowDate = date;
        b->returnDate = "Not returned";

        currentUser->borrowedISBNs.push_back(isbn);
        return true;
    }

    bool returnBook(const string& isbn, const string& date) {
        if (currentUser == NULL) return false;

        Book* b = findBook(isbn);
        if (b == NULL) return false;

        if (b->borrowedBy != currentUser->username) return false;

        b->available = true;
        b->borrowedBy = "";
        b->returnDate = date;

        for (size_t i = 0; i < currentUser->borrowedISBNs.size(); i++) {
            if (currentUser->borrowedISBNs[i] == isbn) {
                currentUser->borrowedISBNs.erase(
                    currentUser->borrowedISBNs.begin() + i
                );
                break;
            }
        }
        return true;
    }

    void showMyBooks() const {
        if (currentUser == NULL) {
            cout << "Not logged in.\n";
            return;
        }

        if (currentUser->borrowedISBNs.empty()) {
            cout << "You have no borrowed books.\n";
            return;
        }

        cout << "\nMy Borrowed Books:\n";
        for (size_t i = 0; i < currentUser->borrowedISBNs.size(); i++) {
            Book* b = NULL;
            for (size_t j = 0; j < books.size(); j++) {
                if (books[j].isbn == currentUser->borrowedISBNs[i]) {
                    b = (Book*)&books[j];
                    break;
                }
            }
            if (b) {
                cout << "- " << b->title
                     << " | Borrowed: " << b->borrowDate
                     << " | Return: " << b->returnDate << "\n";
            }
        }
    }

    bool isLoggedIn() const {
        return currentUser != NULL;
    }

    string getCurrentUsername() const {
        if (currentUser) return currentUser->username;
        return "";
    }
};

// ===================== MAIN =====================
int main() {
    Library lib;
    string choice, user, pass, isbn, date;

    while (true) {
        if (!lib.isLoggedIn()) {
            cout << "\n--- Library System ---\n";
            cout << "1. Login\n2. Register\n3. View Books\n0. Exit\nChoice: ";
            getline(cin, choice);

            if (choice == "0") break;

            if (choice == "1") {
                cout << "Username: ";
                getline(cin, user);
                cout << "Password: ";
                getline(cin, pass);
                if (lib.login(user, pass))
                    cout << "Login successful\n";
                else
                    cout << "Login failed\n";
            }
            else if (choice == "2") {
                cout << "Username: ";
                getline(cin, user);
                cout << "Password: ";
                getline(cin, pass);
                if (lib.registerUser(user, pass))
                    cout << "Registered successfully\n";
                else
                    cout << "Username already exists\n";
            }
            else if (choice == "3") {
                lib.displayAllBooks();
            }
        } else {
            cout << "\n--- Logged in as " << lib.getCurrentUsername() << " ---\n";
            cout << "1. View Books\n";
            cout << "2. Borrow Book\n";
            cout << "3. Return Book\n";
            cout << "4. My Borrowed Books\n";
            cout << "5. Logout\n";
            cout << "0. Exit\nChoice: ";
            getline(cin, choice);

            if (choice == "0") break;

            if (choice == "1") {
                lib.displayAllBooks();
            }
            else if (choice == "2") {
                cout << "Enter ISBN: ";
                getline(cin, isbn);
                cout << "Enter borrow date (YYYY-MM-DD): ";
                getline(cin, date);
                if (lib.borrowBook(isbn, date))
                    cout << "Book borrowed successfully\n";
                else
                    cout << "Borrow failed\n";
            }
            else if (choice == "3") {
                cout << "Enter ISBN: ";
                getline(cin, isbn);
                cout << "Enter return date (YYYY-MM-DD): ";
                getline(cin, date);
                if (lib.returnBook(isbn, date))
                    cout << "Book returned successfully\n";
                else
                    cout << "Return failed\n";
            }
            else if (choice == "4") {
                lib.showMyBooks();
            }
            else if (choice == "5") {
                lib.logout();
                cout << "Logged out\n";
            }
        }
    }

    cout << "Goodbye!\n";
    return 0;
}
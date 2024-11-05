#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;
class Book 
{
public:
    string title;
    string author;
    string ISBN;
    bool available;

    Book(string t, string a, string isbn)
        : title(t), author(a), ISBN(isbn), available(true) {}

    void displayInfo() 
    {
        cout << "Title: " << title << "\n"
             << "Author: " << author << "\n"
             << "ISBN: " << ISBN << "\n"
             << "Available: " << (available ? "Yes" : "No") << "\n";
    }

    string getISBN() const 
    {
        return ISBN;
    }

    string getTitle() const 
    {
        return title;
    }

    string getAuthor() const 
    {
        return author;
    }

    bool isAvailable() const
     {
        return available;
    }

    void markAsBorrowed()
     {
        available = false;
    }

    void markAsReturned()
     {
        available = true;
    }
};

class User
 {
public:
    string userID;
    string name;
    string contactInfo;

    User(string id, string n, string contact)
        : userID(id), name(n), contactInfo(contact) {}

    void displayInfo()
     {
        cout << "User ID: " << userID << "\n"
             << "Name: " << name << "\n"
             << "Contact Info: " << contactInfo << "\n";
    }

    string getUserID() const
     {
        return userID;
    }
};

class Loan : public Book, public User 
{
public:
    time_t loanDate;
    time_t dueDate;
    time_t returnDate;

    Loan(string t, string a, string isbn, string id, string n, string contact)
        : Book(t, a, isbn), User(id, n, contact) 
        {
        loanDate = time(nullptr);
        dueDate = loanDate + (14 * 24 * 60 * 60); // 2 weeks due date
        returnDate = 0;
    }

    void markAsReturned() 
    {
        returnDate = time(nullptr);
        Book::markAsReturned();
    }

    void displayLoanInfo() 
    {
        User::displayInfo();
        Book::displayInfo();
        cout << "Loan Date: " << ctime(&loanDate)
             << "Due Date: " << ctime(&dueDate)
             << "Return Date: " << (returnDate ? ctime(&returnDate) : "Not Returned Yet") << "\n";
    }

    bool isLoanForUser(string userID)
     {
        return this->userID == userID;
    }

    string getBookISBN() 
    {
        return this->ISBN;
    }
};

class Library
 {
public:
    vector<Book*> books;
    vector<User*> users;
    vector<Loan*> loans;

    void addBook(Book* book)
     {
        books.push_back(book);
    }

    void addUser(User* user) 
    {
        users.push_back(user);
    }

    void loanBook(string isbn, string userID)
     {
        Book* book = findBook(isbn);
        User* user = findUser(userID);
        if (book && user && book->isAvailable())
         {
            Loan* loan = new Loan(book->getTitle(), book->getAuthor(), isbn, userID, user->getUserID(), user->getUserID());
            loans.push_back(loan);
            book->markAsBorrowed();
            cout << "Book loaned successfully.\n";
        } 
        else 
        {
            cout << "Loan failed: Book is either not available or user not found.\n";
        }
    }

    void returnBook(string isbn, string userID) 
    {
        for (Loan* loan : loans) 
        {
            if (loan->getBookISBN() == isbn && loan->isLoanForUser(userID)) 
            {
                loan->markAsReturned();
                cout << "Book returned successfully.\n";
                return;
            }
        }
        cout << "Return failed: No matching loan record found.\n";
    }

    void displayBooks() 
    {
        for (auto book : books) 
        {
            book->displayInfo();
            cout << "-------------------------\n";
        }
    }

    void displayUsers() 
    {
        for (auto user : users) 
        {
            user->displayInfo();
            cout << "-------------------------\n";
        }
    }

    void displayLoans()
     {
        for (auto loan : loans)
         {
            loan->displayLoanInfo();
            cout << "-------------------------\n";
        }
    }

private:
    Book* findBook(const string& isbn) 
    {
        for (auto book : books) 
        {
            if (book->getISBN() == isbn)
             {
                return book;
            }
        }
        return nullptr;
    }

    User* findUser(const string& userID) 
    {
        for (auto user : users) 
        {
            if (user->getUserID() == userID) 
            {
                return user;
            }
        }
        return nullptr;
    }
};

int main() 
{
    Library library;
    int choice;

    do {
        cout << "Library Management System Menu:\n";
        cout << "1. Add Book\n";
        cout << "2. Add User\n";
        cout << "3. Loan Book\n";
        cout << "4. Return Book\n";
        cout << "5. Display Books\n";
        cout << "6. Display Users\n";
        cout << "7. Display Loans\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
         {
            case 1:
             {
                string title, author, isbn;
                cout << "Enter title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter author: ";
                getline(cin, author);
                cout << "Enter ISBN: ";
                getline(cin, isbn);
                library.addBook(new Book(title, author, isbn));
                cout << "Book added successfully!\n";
                break;
            }
            case 2: 
            {
                string id, name, contact;
                cout << "Enter User ID: ";
                cin >> id;
                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Contact Info: ";
                getline(cin, contact);
                library.addUser(new User(id, name, contact));
                cout << "User added successfully!\n";
                break;
            }
            case 3: 
            {
                string isbn, userID;
                cout << "Enter ISBN of the book: ";
                cin >> isbn;
                cout << "Enter User ID: ";
                cin >> userID;
                library.loanBook(isbn, userID);
                break;
            }
            case 4: {
                string isbn, userID;
                cout << "Enter ISBN of the book: ";
                cin >> isbn;
                cout << "Enter User ID: ";
                cin >> userID;
                library.returnBook(isbn, userID);
                break;
            }
            case 5: 
            {
                cout << "Books in Library:\n";
                library.displayBooks();
                break;
            }
            case 6: 
            {
                cout << "Registered Users:\n";
                library.displayUsers();
                break;
            }
            case 7: 
            {
                cout << "Current Loans:\n";
                library.displayLoans();
                break;
            }
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }

        cout << endl;

    } while (choice != 0);

    return 0;
}

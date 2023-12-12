#include "mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>
#include <QLineEdit>
#include <QTextEdit>

#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

const int MAX_BOOKS = 100;

class Book {
public:
    string title;
    string author;
    string ISBN;
    bool isCheckedOut;
    time_t dueDate;
    string borrowerName;
    time_t checkOutDate;
    double fineAmount;

    Book() : isCheckedOut(false), dueDate(0), fineAmount(0.0) {}

    Book(string title, string author, string ISBN, time_t dueDate) {
        this->title = title;
        this->author = author;
        this->ISBN = ISBN;
        this->isCheckedOut = false;
        this->dueDate = dueDate;
        this->fineAmount = 0.0;
    }

    void checkOutBook(string borrower, time_t checkOutDate, time_t dueDate) {
        isCheckedOut = true;
        borrowerName = borrower;
        this->checkOutDate = checkOutDate;
        this->dueDate = dueDate;
    }

    void returnBook() {
        isCheckedOut = false;
        borrowerName.clear();
        checkOutDate = 0;
        fineAmount = 0.0;
    }

    double calculateFine() {
        if (isCheckedOut && time(0) > dueDate) {
            double overdueDays = difftime(time(0), dueDate) / (24 * 60 * 60);
            fineAmount = overdueDays * 0.50;
        } else {
            fineAmount = 0.0;
        }
        return fineAmount;
    }

    double getFineAmount() const {
        return fineAmount;
    }

    void setFineAmount(double amount) {
        fineAmount = amount;
    }
};

class Library {
private:
    Book bookList[MAX_BOOKS];
    int bookCount;

public:
    Library() : bookCount(0) {}

    void addBook(Book book) {
        if (bookCount < MAX_BOOKS) {
            bookList[bookCount] = book;
            bookCount++;
        } else {
            cout << "Library is full, cannot add more books." << endl;
        }
    }

    void searchBook(string searchQuery, QTextEdit* displayArea) {
        bool found = false;
        QString result;

        string searchQueryLower = searchQuery;
        transform(searchQueryLower.begin(), searchQueryLower.end(), searchQueryLower.begin(), ::tolower);

        for (int i = 0; i < bookCount; ++i) {
            Book book = bookList[i];
            string titleLower = book.title;
            transform(titleLower.begin(), titleLower.end(), titleLower.begin(), ::tolower);

            cout << "Debug - Title (Original): " << book.title << endl;
            cout << "Debug - Title (Lowercase): " << titleLower << endl;
            cout << "Debug - Search Query (Lowercase): " << searchQueryLower << endl;

            if (titleLower.find(searchQueryLower) != string::npos) {
                found = true;
                result += "Title: " + QString::fromStdString(book.title) + "\n";
                result += "Author: " + QString::fromStdString(book.author) + "\n";
                result += "ISBN: " + QString::fromStdString(book.ISBN) + "\n";
                result += "Availability: " + (book.isCheckedOut ? QString("Not Available") : QString("Available")) + "\n\n";
            }
        }

        if (found) {
            displayArea->clear();
            displayArea->append("Search Results:\n" + result);
        } else {
            displayArea->clear();
            displayArea->append("No books found with the search query: " + QString::fromStdString(searchQuery) + "\n");
        }
    }

    void checkOutBook(string ISBN, string borrower, QTextEdit* displayArea) {
        for (int i = 0; i < bookCount; ++i) {
            if (bookList[i].ISBN == ISBN && !bookList[i].isCheckedOut) {
                bookList[i].checkOutBook(borrower, time(0), bookList[i].dueDate);
                displayArea->append("Book checked out successfully:\n" + QString::fromStdString(bookList[i].title));
                return;
            }
        }
        displayArea->append("Book not available or already checked out.");
    }

    void returnBook(string ISBN, QTextEdit* displayArea) {
        for (int i = 0; i < bookCount; ++i) {
            if (bookList[i].ISBN == ISBN && bookList[i].isCheckedOut) {
                double fine = bookList[i].calculateFine();
                bookList[i].returnBook();
                displayArea->append("Book returned successfully:\n" + QString::fromStdString(bookList[i].title));
                if (fine > 0.0) {
                    displayArea->append("Fine Amount: $" + QString::number(fine));
                }
                return;
            }
        }
        displayArea->append("Book not found or not checked out.");
    }
};

class LibraryApp : public QWidget {
    Q_OBJECT
private:
    Library library;
    QTextEdit* displayArea;
    QLineEdit* searchInput;
    QPushButton* searchButton;

public:
    LibraryApp(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *titleLabel = new QLabel("Library Management System");
        displayArea = new QTextEdit();
        displayArea->setReadOnly(true);

        QLabel *searchLabel = new QLabel("Search by Title/Author/ISBN:");
        searchInput = new QLineEdit();
        searchButton = new QPushButton("Search");

        QPushButton* checkOutButton = new QPushButton("Check Out");
        QPushButton* returnButton = new QPushButton("Return");

        layout->addWidget(titleLabel);
        layout->addWidget(searchLabel);
        layout->addWidget(searchInput);
        layout->addWidget(searchButton);
        layout->addWidget(checkOutButton);
        layout->addWidget(returnButton);
        layout->addWidget(displayArea);

        connect(searchButton, SIGNAL(clicked()), this, SLOT(searchClicked()));
        connect(checkOutButton, SIGNAL(clicked()), this, SLOT(checkOutClicked()));
        connect(returnButton, SIGNAL(clicked()), this, SLOT(returnClicked()));
    }

public slots:
    void searchClicked() {
        QString query = searchInput->text();
        library.searchBook(query.toStdString(), displayArea);
    }

    void checkOutClicked() {
        QString ISBN = searchInput->text();
        QString borrower = QInputDialog::getText(this, "Check Out", "Enter Borrower's Name:");
        library.checkOutBook(ISBN.toStdString(), borrower.toStdString(), displayArea);
    }

    void returnClicked() {
        QString ISBN = searchInput->text();
        library.returnBook(ISBN.toStdString(), displayArea);
    }
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    LibraryApp window;
    window.setWindowTitle("Library Management System");
    window.resize(400, 300);
    window.show();

    Library library;

    library.addBook(Book("C++ Programming Language", "Bjarne Stroustrup", "1234567890", time(0) + (30 * 24 * 60 * 60)));
    library.addBook(Book("Effective Java", "Joshua Bloch", "0987654321", time(0) + (30 * 24 * 60 * 60)));
    library.addBook(Book("The Catcher in the Rye", "J.D. Salinger", "2345678901", time(0) + (20 * 24 * 60 * 60)));
    library.addBook(Book("To Kill a Mockingbird", "Harper Lee", "3456789012", time(0) + (25 * 24 * 60 * 60)));
    library.addBook(Book("1984", "George Orwell", "4567890123", time(0) + (22 * 24 * 60 * 60)));
    library.addBook(Book("The Great Gatsby", "F. Scott Fitzgerald", "5678901234", time(0) + (18 * 24 * 60 * 60)));

    return app.exec();
}

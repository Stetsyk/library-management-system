#include <mysql.h>
#include <mysqld_error.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
using namespace std;

class Student {
 private:
  string Id;
  string Name;

 public:
  Student() {
    Id   = "";
    Name = "";
  }
  Student(string id, string name) {
    Id   = id;
    Name = name;
  }
  void addStudentByValue(string id, string name) {
    Id   = id;
    Name = name;
  }

  string getId() {
    return Id;
  }
  string getName() {
    return Name;
  }
  void setId(string id) {
    Id = id;
  }
  void setName(string name) {
    Name = name;
  }
  void print() {
    cout << "Name: " << Name << endl;
    cout << "Id: " << Id << endl;
  }
  void addStudent() {
    system("cls");
    cout << "Enter student's name: ";
    cin >> Name;
    cout << "Enter unique Id: ";
    cin >> Id;
  }
  inline const char *addStudentQuery() {
    string query = "INSERT INTO students (Id,Name) VALUES(" + Id + "," + Name + ")";
    return query.c_str();
  };
  void clear() {
    Id   = "";
    Name = "";
  }
};

class Book {
 private:
  string Id;
  string Name;
  int Quantity;

 public:
  Book() {
    Id       = "";
    Name     = "";
    Quantity = 0;
  }

  Book(string id, string name, int quantity) {
    Id       = id;
    Name     = name;
    Quantity = quantity;
  }
  void addBookByValue(string id, string name, int quantity) {
    Id       = id;
    Name     = name;
    Quantity = quantity;
  }
  void setId(string id) {
    Id = id;
  }

  void setName(string name) {
    Name = name;
  }

  void setQuantity(int quantity) {
    Quantity = quantity;
  }

  string getName() {
    return Name;
  }

  string getId() {
    return Id;
  }

  int getQuantity() {
    return Quantity;
  }
  void print() {
    cout << "Name: " << Name << endl;
    cout << "Id: " << Id << endl;
    cout << "Quantity: " << Quantity << endl;
  }
  string getQuantityString() {
    int q = Quantity;
    stringstream ss;
    ss << q;
    string sq = ss.str();
    return sq;
  }

  void addBook() {
    system("cls");
    cout << "Enter book name: ";
    cin >> Name;
    cout << "Enter book's quantity: ";
    cin >> Quantity;
    cout << "Enter unique Id: ";
    cin >> Id;
  }

  inline const char *addBookQuery() {
    string query = "INSERT INTO lib (Id,Name,Quantity) VALUES(" + Id + "," + Name + "," + getQuantityString() + ")";
    return query.c_str();
  }
  void clear() {
    Id       = "";
    Name     = "";
    Quantity = 0;
  }
};

class LibraryManagement {
 private:
  const char *HOST                  = "localhost";
  const char *USER                  = "root";
  const char *PW                    = "abc123#";
  const char *DB                    = "mydb";
  const string DISPLAYBOOKSQUERY    = "SELECT * FROM books";
  const string DISPLAYSTUDENTSQUERY = "SELECT * FROM students";
  Student s;
  Book b;
  MYSQL *conn;

  inline const char *GETBOOKBYIDQUERY(string id) {
    string query = "SELECT * FROM books WHERE Id =" + id;
    return query.c_str();
  }
  inline const char *FINDBOOKBYIDQUERY(string id) {
    string query = "SELECT * FROM books WHERE Id =" + id;
    return query.c_str();
  }

  inline const char *FINDSTUDENTBYIDQUERY(string id) {
    string query = "SELECT * FROM students WHERE Id =" + id;
    return query.c_str();
  }

  void printError() {
    cout << "Error: " << mysql_error(conn) << endl;
  }
  void printTable() {
    MYSQL_RES *res;
    res = mysql_store_result(conn);
    if (res) {
      int col       = mysql_num_fields(res);
      MYSQL_ROW row = mysql_fetch_row(res);
      while (row) {
        for (int i = 0; i < col; i++) {
          cout << " " << row[i] << "/t";
        }
        cout << endl;
        row = mysql_fetch_row(res);
      }
      mysql_free_result(res);
    }
  }

 public:
  void init() {
    conn = mysql_init(NULL);
    if (mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0)) {
      cout << "Logged In!" << endl;
      createTable();
    } else {
      printError();
    }
  }

  void displayAllBooks() {
    if (mysql_query(conn, DISPLAYBOOKSQUERY.c_str())) {
      printError();
    } else {
      system("cls");
      cout << "Available Books" << endl;
      cout << "***************" << endl;
      printTable();
    }
  }

  void displayAllStudents() {
    if (mysql_query(conn, DISPLAYSTUDENTSQUERY.c_str())) {
      printError();
    } else {
      system("cls");
      cout << "Students Registered in Library" << endl;
      cout << "******************************" << endl;
      printTable();
    }
  }

  void printBookById(string id) {
    if (mysql_query(conn, FINDBOOKBYIDQUERY(id))) {
      printError();
    } else {
      printTable();
    }
  }

  void printStudentById(string id) {
    if (mysql_query(conn, FINDSTUDENTBYIDQUERY(id))) {
      printError();
    } else {
      printTable();
    }
  }

  int getBookQuantity(string id) {
    if (mysql_query(conn, GETBOOKBYIDQUERY(id))) {
      printError();
    } else {
      MYSQL_RES *res = mysql_store_result(conn);
      if (res) {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (id == row[0]) {
          int quantity = atoi(row[2]);
          return quantity;
        } else {
          cout << "Book not found" << endl;
        }
      }
      mysql_free_result(res);
    }
    return 0;
  }

  void admin() {
    bool open = true;
    while (open) {
      int option;
      cout << "1. Add Book." << endl;
      cout << "2. Add Student." << endl;
      cout << "3. Print all books." << endl;
      cout << "4. Print all students." << endl;
      cout << "0. Exit." << endl;
      cout << "Enter Choice: ";
      cin >> option;

      switch (option) {
        case 1:
          b.addBook();
          if (mysql_query(conn, b.addBookQuery())) {
            printError();
          } else {
            cout << "Book Inserted Successfully" << endl;
          }
          b.clear();
          break;
        case 2:
          s.addStudent();
          if (mysql_query(conn, s.addStudentQuery())) {
            printError();
          } else {
            cout << "Student Inserted Successfully" << endl;
          }
          s.clear();
          break;
        case 3:
          displayAllBooks();
          break;
        case 4:
          displayAllStudents();
        case 0:
          open = false;
          cout << "Adminstration service is closing" << endl;
        default:
          cout << "Invalid Input, Please input correct input" << endl;
          break;
      }
    }
  }
  void user() {
    string sid;
    cout << "Enter your ID: ";
    cin >> sid;
    if (mysql_query(conn, s.addStudentQuery())) {
      printError();
    } else {
      MYSQL_RES *res = mysql_store_result(conn);
      if (res) {
        int num = mysql_num_rows(res);
        if (num == 0) {
          cout << "This student is not registered" << endl;
        } else {
          cout << "Student Found" << endl;
          MYSQL_ROW row = mysql_fetch_row(res);
          s.addStudentByValue(row[0], row[1]);
          s.print();
          system("cls");
          displayAllBooks();
          string Bid;
          cout << "Enter Book Name to issue" << endl;
          cin >> Bid;
          if (getBookQuantity(Bid) > 0) {
            int bookQuantity = getBookQuantity(Bid) - 1;
            stringstream ss;
            ss << bookQuantity;
            string Sq = ss.str();

            string upd = "UPDATE books SET Quantity ='" + Sq + "' WHERE Id = '" + Bid + "' ";
            if (mysql_query(conn, upd.c_str())) {
              printError();
            } else {
              cout << "Book is available. Borrowing Book...." << endl;
            }
          }
        }
      }
      s.clear();
      b.clear();
      mysql_free_result(res);
    }
  }
  void createTable() {
    string CREATESTUDENTS = "CREATE TABLE students (Id INT PRIMARY KEY AUTO_INCREMENT, Name VARCHAR(100) NOT NULL)";
    string CREATEBOOKS    = "CREATE TABLE books(Id INT PRIMARY KEY AUTO_INCREMENT,Name VARCHAR(150) NOT NULL,Quantity INT NOT NULL)";
    if (mysql_query(conn, CREATEBOOKS.c_str()))
      printError();
    if (mysql_query(conn, CREATESTUDENTS.c_str()))
      printError();
  }
  void startLibrary() {
    bool working = true;
    while (working) {
      int option;
      cout << "Welcome To Library Management System" << endl;
      cout << "1. Admin." << endl;
      cout << "2. User." << endl;
      cout << "0. Exit." << endl;
      cout << "Enter Choice: ";
      cin >> option;

      switch (option) {
        case 1:
          cout << "Accessing adminstration services" << endl;
          admin();
          break;
        case 2:
          cout << "Accessing students services" << endl;
          user();
        case 3:
          working = false;
          cout << "Exiting the program!" << endl;
          cout << "Thanks" << endl;
        default:
          cout << "Invalid Input, Please input correct input" << endl;
          break;
      }
    }
    mysql_close(conn);
  }
};

int main() {
  LibraryManagement library;
  library.init();
  library.createTable();
  library.startLibrary();
}
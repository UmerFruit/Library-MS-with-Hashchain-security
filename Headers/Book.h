#ifndef BOOK_H_
#define BOOK_H_
#include "Headers.h"
using namespace std;
class Book
{
private:
    string book_num;    // bookno.
    string book_name;   // bookname
    string author_name; // authorname

public:
    void createbook()
    {
        system("clear");
        cout << "NEW BOOK ENTRY\n\n";
        cout << "Enter Book Number:" << endl;
        cin >> book_num;
        cout << "Enter Book Name:" << endl;
        cin.ignore();
        getline(cin, book_name);
        cout << "Enter Author Name:" << endl;
        getline(cin, author_name);
        cout << "Book Created!" << endl;
    }
    void showbook()
    {
        cout << setw(20) << left << "Book Number: " << book_num << endl;
        cout << setw(20) << left << "Enter Book Name: " << book_name << endl;
        cout << setw(20) << left << "Enter Author Name: " << author_name << endl
             << endl;
    }
    void modifybook()
    {
        cout << "Book Number: " << book_num << endl;
        cout << "Modify Book Name:" << endl;
        getline(cin, book_name);
        cout << "Modify Author's Name :" << endl;
        getline(cin, author_name);
    }

    string getbook_num() // string getter
    {
        return book_num;
    }
};
bool writebook()
{
    bool created = false;
    ofstream fout;
    char ch;
    fout.open("Book.dat", ios::binary | ios::app); // write and append data
    do
    {
        Book bk;
        system("clear");
        bk.createbook();
        fout.write(reinterpret_cast<char *>(&bk), sizeof(Book)); // size of class
        created = true;
        cout << "\nWould you like to add more records? (y/n):" << endl;
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
    fout.close();
    return created;
}
bool displayspb() // display specific book
{
    bool found = false;
    system("clear");
    string n;
    cout << "\nPlease Enter Book no." << endl;
    cin >> n;
    Book bk;
    ifstream fin;
    cout << "\nBOOK DETAILS" << endl;
    int flag = 0;                                // if book not found
    fin.open("Book.dat", ios::in | ios::binary); // read data
    while (fin.read(reinterpret_cast<char *>(&bk), sizeof(Book)) && flag == 0)
    {
        if (bk.getbook_num() == n)
        {
            bk.showbook();
            found = true;
            flag = 1;
        }
    }
    fin.close();
    if (flag == 0) // book not found
    {
        cout << "\nBook does not exist." << endl;
    }
    cin.ignore();
    getch();
    return found;
}
bool modifybook()
{
    Book bk;
    bool modified = false;
    fstream fp;
    string n;
    int found = 0; // seach book of given data
    system("clear");
    cout << "\nMODIFY BOOK RECORD" << endl;
    cout << "\nEnter The Book No. ";
    cin >> n;
    fp.open("Book.dat", ios::in | ios::out | ios::binary);
    int pos = fp.tellg();
    while (fp.read(reinterpret_cast<char *>(&bk), sizeof(Book)) && found == 0)
    {
        if (bk.getbook_num() == n)
        {
            bk.showbook();
            cout << "\nEnter the new details for book." << endl;
            cin.ignore();
            bk.modifybook();
            fp.seekp(pos); // back from current position
            fp.write(reinterpret_cast<char *>(&bk), sizeof(Book));
            cout << "\nRecord Updated" << endl;
            found = 1;
            modified = true;
        }
        pos = fp.tellg();
    }
    fp.close();
    if (found == 0)
    {
        cin.ignore();
        cout << "\nRecord Not Found" << endl;
    }
    getch(); // press key to get out
    return modified;
}
bool deletebook()
{
    bool deleted = false;
    Book bk;
    fstream fp;
    string n; // book no.
    int flag = 0;
    system("clear");
    cout << "\nDELETE BOOK." << endl;
    cout << "\nEnter the Book num:" << endl;
    cin >> n;
    fp.open("Book.dat", ios::in | ios::out | ios::binary);
    fstream fp2;                                  // New object
    fp2.open("Temp.dat", ios::out | ios::binary); // temp having data else than that to be deleted
    fp.seekg(0, ios::beg);
    while (fp.read(reinterpret_cast<char *>(&bk), sizeof(Book)))
    {
        if (bk.getbook_num() != n)
        {
            fp2.write(reinterpret_cast<char *>(&bk), sizeof(Book));
        }
        else
        {
            flag = 1; // student found
        }
    }
    fp2.close();
    fp.close();
    remove("Book.dat");
    rename("Temp.dat", "Book.dat"); // data after deletion moved to temp
    if (flag == 1)
    {
        cout << "\nRecord Deleted." << endl;
        deleted = true;
    }
    else
    {
        cout << "\nRecord not Found." << endl;
    }
    cin.ignore();
    getch();
    return deleted;
}
void displayallb()
{
    Book bk;
    ifstream fin;
    system("clear");
    fin.open("Book.dat", ios::in | ios::binary); // read mode
    if (!fin)
    {
        cout << "File Could Not Be Open";
        cin.ignore();
        getch();
        return; // press any key and return
    }
    cout << "\n\t\t\tBook List\n";
    cout << "==================================================================" << endl;
    cout << "Book Number" << setw(20) << right << "Book Name" << setw(25) << "Book Author" << endl;
    cout << "==================================================================" << endl;
    while (fin.read(reinterpret_cast<char *>(&bk), sizeof(Book)))
    {
        bk.showbook();
    }
    fin.close();
    cin.ignore();
    getch();
}

#endif
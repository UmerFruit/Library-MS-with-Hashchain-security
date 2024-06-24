#ifndef LIBRARIAN_H_
#define LIBRARIAN_H_
#include "Headers.h"
using namespace std;
class Librarian
{
    string name;
    string libID;
    string username;
    string password;

public:
    string getusername() { return username; }
    void createlibrarian()
    {
        system("clear");
        cout << "NEW Librarian ENTRY\n\n";
        cout << "Enter Your First Name only:" << endl;
        cin >> name;
        cout << "Enter The Libraian ID:" << endl;
        cin >> libID;
        cin.ignore();
        Register();
        cout << "Librarian Record Created!" << endl;
    }
    void libMenu()
    {
        cout << "\nWELCOME " << name << endl;
        cout << "LIBRARIAN MENU" << endl;
        cout << "\n1. CREATE STUDENT RECORD" << endl;
        cout << "2. DISPLAY ALL STUDENT RECORD" << endl;
        cout << "3. MODIFY STUDENT RECORD" << endl;
        cout << "4. DELETE STUDENT RECORD" << endl;
        cout << "5. CREATE BOOK" << endl;
        cout << "6. DISPLAY ALL BOOKS" << endl;
        cout << "7. DISPLAY SPECIFIC BOOK" << endl;
        cout << "8. MODIFY BOOK RECORD" << endl;
        cout << "9. DELETE BOOK RECORD" << endl;
        cout << "10.BACK TO MAIN MENU" << endl;
        cout << "\nPLEASE ENTER YOUR CHOICE(1-10)" << endl;
    }

    bool Check()
    {
        Librarian lb;
        string un, pw;
        fstream fp("Librarians.dat", ios::binary | ios::in);
        cout << "----------------------" << endl;
        cout << "Enter the Username: ";
        cin >> un;
        cin.ignore();
        cout << un << endl;
        bool exists = false;
        if (fp)
            while (fp.read(reinterpret_cast<char *>(&lb), sizeof(Librarian)) && !exists)
            {
                if (un == lb.username)
                {
                    exists = true;
                    *this = lb;
                    break;
                }
            }

        if (!exists)
        {
            cout << "----------------------" << endl;
            cout << "User doesn't exist" << endl;
            cout << "----------------------" << endl;
        }
        else
        {
            cout << "----------------------" << endl;
            cout << "Enter the Password:" << endl;
            cout << "If you have forgotten the password then type \"forgot\". :)" << endl;
            pw = inputPass();
            if (pw == lb.password)
            {
                cout << endl;
                cout << "----------------------" << endl;
                cout << "You are now logged in." << endl;
                cout << "----------------------" << endl;
                fp.close();

                return true;
            }
            else if (pw == "forgot")
            {
                cout << "----------------------" << endl;
                cout << "Enter the same Username again." << endl;
                getline(cin, un);
                if (un == lb.username)
                {
                    cout << endl;
                    cout << "----------------------" << endl;
                    cout << "You are now logged in" << endl;
                    cout << "----------------------" << endl;
                    fp.close();

                    return true;
                }
                else
                {
                    cout << "\n----------------------" << endl;
                    cout << "Incorrect entry sorry" << endl;
                    cout << "----------------------" << endl;
                    fp.close();

                    return false;
                }
            }
            else
            {
                cout << "\n----------------------" << endl;
                cout << "Incorrect password" << endl;
                cout << "----------------------" << endl;
                fp.close();

                return false;
            }
            fp.close();
        }
        return false;
    }
    string inputPass()
    {
        string password;
        password = "";
        char c;
        while ((c = getch()) != 10) // until newline char \n is entered
        {
            if (c != 127) // if backspace is not pressed
            {
                password += c;
                cout << '*'; // display asterisks instead of actual password characters
            }
            else
            {
                cout << "\r" << setw(100) << setfill(' ') << ""
                     << "\r";
                password.pop_back();
                int size = 0;
                for (int i = 0; password[i] != '\0'; i++)
                {
                    size++;
                }
                for (int i = 0; i < size; i++)
                {
                    cout << "*";
                }
            }
        }
        return password;
    }
    void Register()
    {
        string pss;
        char choice;
        cout << "----------------------" << endl;
        cout << "Your Name is your username." << endl;
        cout << "Type your Password: " << endl;
        pss = inputPass();
        cout << "----------------------" << endl;

        username = name;
        password = pss;
    }
    string getid() { return libID; }

    void showLibrarian()
    {
        cout << setw(10) << left << "Name:" << name << endl;
        cout << setw(10) << left << "LibID:" << libID << endl;
    }
};
bool writesLib()
{
    bool created = false;
    ofstream fout;
    char ch;
    fout.open("Librarians.dat", ios::binary | ios::app); // write and append data
    do
    {
        Librarian lb;
        system("clear");
        lb.createlibrarian();
        fout.write(reinterpret_cast<char *>(&lb), sizeof(Librarian)); // size of class
        created = true;
        cout << "\nWould you like to add more records? (y/n):" << endl;
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
    fout.close();
    return created;
}
void displayallL() // display all Librarians
{
    ifstream fin;
    Librarian lb;
    system("clear");
    fin.open("Librarians.dat", ios::binary); // read mode
    if (!fin)
    {
        cout << "File Could Not Be Opened" << endl;
        cin.ignore();
        getch();
        return; // press any key and return
    }
    cout << "\n\t\t\tLibrarians List\n";
    cout << "==================================================================" << endl;
    cout << "Name" << setw(20) << right << "LibID" << endl;
    cout << "==================================================================" << endl;
    while (fin.read(reinterpret_cast<char *>(&lb), sizeof(Librarian)))
    {
        lb.showLibrarian();
        cout << endl;
    }
    fin.close();
    cin.ignore();
    getch();
}
bool deletelib()
{
    bool deleted = 0;
    fstream fp;
    Librarian lb;
    string n;
    int flag = 0;
    system("clear");
    cout << "\nDELETE LIBRARIAN" << endl;
    cout << "\nEnter the LibID :" << endl;
    cin >> n;
    fp.open("Librarians.dat", ios::out | ios::in | ios::binary);
    fstream fp2;
    fp2.open("temp.dat", ios::out | ios::binary);
    fp.seekg(0);
    while (fp.read(reinterpret_cast<char *>(&lb), sizeof(Librarian)))
    {
        if (lb.getid() != n)
        {
            fp2.write(reinterpret_cast<char *>(&lb), sizeof(Librarian));
        }
        else
        {
            flag = 1; // librarian found
        }
    }
    fp2.close();
    fp.close();
    remove("Librarians.dat");
    rename("temp.dat", "Librarians.dat"); // data after deletion moved to temp
    if (flag == 1)
    {
        cout << "\nRecord Deleted." << endl;
        deleted = 1;
    }
    else
    {
        cout << "\nRecord not Found." << endl;
    }
    cin.ignore();
    getch();
    return deleted;
}
#endif
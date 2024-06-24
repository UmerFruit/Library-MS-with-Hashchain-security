#ifndef ADMIN_H_
#define ADMIN_H_
#include "Headers.h"
using namespace std;

class Admin
{
    string name;
    string adminID;
    string username;
    string password;

public:
    string getusername() { return username; }
    void createadmin()
    {
        system("clear");
        cout << "NEW ADMIN ENTRY\n\n";
        cout << "Enter Your Name:" << endl;
        cin >> name;
        cout << "Enter The Admin ID:" << endl;
        cin.ignore();
        getline(cin, adminID);
        Register();
        cout << "Admin Record Created!" << endl;
    }
    bool Check()
    {
        Admin a;
        string un, pw;
        fstream fp("Admins.dat", ios::binary | ios::in);
        cout << "----------------------" << endl;
        cout << "Enter the Username: ";
        cin >> un;
        cin.ignore();
        cout << un << endl;
        bool exists = false;
        if (fp)
            while (fp.read(reinterpret_cast<char *>(&a), sizeof(Admin)) && !exists)
            {
                if (un == a.username)
                {
                    exists = true;
                    *this = a;
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
            if (pw == a.password)
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
                if (un == a.username)
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
    string getid() { return adminID; }
    void showadmin()
    {
        cout << setw(10) << left << "Name:" << name << endl;
        cout << setw(10) << left << "AdminID:" << adminID << endl;
    }
    void adminMenu()
    {
        cout << "\nWELCOME " << name << endl;
        cout << "ADMINISTRATOR MENU" << endl;
        cout << "\n1. CREATE STUDENT RECORD" << endl;
        cout << "2. DISPLAY ALL STUDENT RECORD" << endl;
        cout << "3. DISPLAY SPECIFIC STUDENT RECORD" << endl;
        cout << "4. MODIFY STUDENT RECORD" << endl;
        cout << "5. DELETE STUDENT RECORD" << endl;
        cout << "6. CREATE BOOK" << endl;
        cout << "7. DISPLAY ALL BOOKS" << endl;
        cout << "8. DISPLAY SPECIFIC BOOK" << endl;
        cout << "9. MODIFY BOOK RECORD" << endl;
        cout << "10.DELETE BOOK RECORD" << endl;
        cout << "11.CREATE LIBRARIAN" << endl;
        cout << "12.DISPLAY ALL LIBRARIAN" << endl;
        cout << "13.DELETE LIBRARIAN" << endl;
        cout << "14.CREATE ADMIN" << endl;
        cout << "15.DISPLAY ALL ADMIN" << endl;
        cout << "16.DELETE ADMIN" << endl;
        cout << "17.BACK TO MAIN MENU" << endl;
        cout << "\nPLEASE ENTER YOUR CHOICE(1-17)" << endl;
    }
};
bool writesadm()
{
    ofstream fout;
    char ch;
    fout.open("Admins.dat", ios::binary | ios::app); // write and append data
    do
    {
        Admin am;
        system("clear");
        am.createadmin();
        fout.write(reinterpret_cast<char *>(&am), sizeof(Admin)); // size of class
        cout << "\nWould you like to add more records? (y/n):" << endl;
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
    fout.close();
    return 1;
}
void displayallA() // display all admins
{
    ifstream fin;
    Admin am;
    system("clear");
    fin.open("Admins.dat", ios::binary); // read mode
    if (!fin)
    {
        cout << "File Could Not Be Opened" << endl;
        cin.ignore();
        getch();
        return; // press any key and return
    }
    cout << "\n\t\t\tAdmins List\n";
    cout << "==================================================================" << endl;
    cout << "Name" << setw(20) << right << "AdminID" << endl;
    cout << "==================================================================" << endl;
    while (fin.read(reinterpret_cast<char *>(&am), sizeof(Admin)))
    {
        am.showadmin();
        cout << endl;
    }
    fin.close();
    cin.ignore();
    getch();
}
bool deleteadm()
{
    bool deleted = false;
    fstream fp;
    Admin am;
    string n;
    int flag = 0;
    system("clear");
    cout << "\nDELETE ADMIN" << endl;
    cout << "\nEnter the AdminID :" << endl;
    cin >> n;
    fp.open("Admins.dat", ios::in | ios::out | ios::binary);
    fstream fp2;
    fp2.open("temp.dat", ios::out | ios::binary);
    fp.seekg(0);
    while (fp.read(reinterpret_cast<char *>(&am), sizeof(Admin)))
    {
        if (am.getid() != n)
        {
            fp2.write(reinterpret_cast<char *>(&am), sizeof(Admin));
        }
        else
        {
            flag = 1; // Admin found
        }
    }
    fp2.close();
    fp.close();
    remove("Admins.dat");
    rename("temp.dat", "Admins.dat"); // data after deletion moved to temp
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

#endif
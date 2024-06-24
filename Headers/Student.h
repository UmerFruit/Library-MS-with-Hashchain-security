#ifndef STUDENT_H_
#define STUDENT_H_
#include "Headers.h"
using namespace std;
class Student
{
    string adm_num; // admission no.
    string name;
    string stbook_num; // student book no
    int token;         // total book of student
    string username;
    string password; // for access control
public:
    string getusername() { return username; }
    void createstudent()
    {
        system("clear");
        cout << "NEW STUDENT ENTRY\n\n";
        cout << "Enter The Admission Num:" << endl;
        cin >> adm_num;
        cout << "Enter The Student Name:" << endl;
        cin.ignore();
        getline(cin, name);
        token = 0;
        stbook_num = "";
        Register();
        cout << "Student Record Created!" << endl;
    }
    void studentMenu()
    {
        cout << "\nWELCOME " << name << endl;
        cout << "STUDENT MENU" << endl;
        cout << "1. BOOK ISSUE" << endl;
        cout << "2. BOOK DEPOSIT" << endl;
        cout << "3. DISPLAY ALL BOOKS" << endl;
        cout << "4. DISPLAY SPECIFIC BOOK" << endl;
        cout << "5. BACK TO MAIN MENU" << endl;
        cout << "\nPLEASE ENTER YOUR CHOICE(1-5)" << endl;
    }

    bool Check()
    {
        Student st;
        string un, pw;
        fstream fp("Students.dat", ios::binary | ios::in);
        cout << "----------------------" << endl;
        cout << "Enter the Username: ";
        cin >> un;
        cin.ignore();
        cout << un << endl;
        bool exists = false;
        if (fp)
            while (fp.read(reinterpret_cast<char *>(&st), sizeof(Student)) && !exists)
            {
                if (un == st.username)
                {
                    exists = true;
                    *this = st;
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
            if (pw == st.password)
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
                if (un == st.username)
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
    void showstudent()
    {
        cout << setw(30) << left << "Admission Number: " << adm_num << endl;
        cout << setw(30) << left << "Student Name : " << name << endl;
        cout << setw(30) << left << "No of Books Issued : " << token << endl;
        if (token == 1)
        {
            cout << setw(30) << left << "Book Number " << stbook_num << endl;
        }
    }
    void modifystudent()
    {
        cout << "\nAdmission No. " << adm_num;
        cout << "\nModify Student Name : ";
        cin.ignore();
        getline(cin, name);
    }
    string getadm_num()
    {
        return adm_num;
    }
    string getstbook_num()
    {
        return stbook_num;
    }
    int gettoken()
    {
        return token;
    }
    void addtoken()
    {
        token = 1;
    }
    void resettoken()
    {
        token = 0;
    }
    void setstbook_num(string t)
    {
        stbook_num = t;
        addtoken();
    }
};
bool findstudent(string admnum)
{
    Student st;
    bool alreadyE = false;
    ifstream fp("Students.dat", ios::in | ios::binary); // read data

    while (fp.read(reinterpret_cast<char *>(&st), sizeof(Student)))
    {
        if (st.getadm_num() == admnum)
        {
            alreadyE = true;
            break;
        }
    }
    return alreadyE;
}
bool writestudent()
{
    bool created = false;
    ofstream fout;
    char ch;
    fout.open("Students.dat", ios::binary | ios::app); // write and append data
    do
    {
        Student st;
        system("clear");
        st.createstudent();
        if (findstudent(st.getadm_num()) == false)
        {
            fout.write(reinterpret_cast<char *>(&st), sizeof(Student));// size of class
            created = true;
        } 
        else
            cout << "Student Already exists.Try again." << endl;
        cout << "\nWould you like to add more records? (y/n):" << endl;
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
    fout.close();
    return created;
}
void displaysps() // display specific student
{
    system("clear");
    string n;
    cout << "\nPlease Enter Admission no. ";
    cin >> n;
    Student st;
    ifstream fin;
    cout << "\nSTUDENT DETAILS" << endl;
    int flag = 0;                                    // if student not found
    fin.open("Students.dat", ios::in | ios::binary); // read data
    while (fin.read(reinterpret_cast<char *>(&st), sizeof(Student)) && flag == 0)
    {
        if (st.getadm_num() == n)
        {
            st.showstudent();
            flag = 1;
        }
    }
    fin.close();
    if (flag == 0) // student not found
    {
        cout << "\nStudent does not exist." << endl;
    }
    cin.ignore();
    getch();
}
bool modifystudent()
{
    bool modified = false;
    Student st;
    fstream fp;
    string n;
    int found = 0; // seach book of given data
    system("clear");
    cout << "\nMODIFY STUDENT RECORD" << endl;
    cout << "\nEnter the Admission no. ";
    cin >> n;
    fp.open("Students.dat", ios::in | ios::out | ios::binary);
    int pos = fp.tellg();
    while (fp.read(reinterpret_cast<char *>(&st), sizeof(Student)) && found == 0)
    {

        if (st.getadm_num() == n)
        {
            st.showstudent();
            cout << "\nEnter the new details of student";
            st.modifystudent();
            fp.seekp(pos); // back from current position
            fp.write(reinterpret_cast<char *>(&st), sizeof(Student));
            cout << "\nRecord Updated." << endl;
            found = 1;
            modified = true;
        }
        pos = fp.tellg();
    }
    fp.close();
    if (found == 0)
    {
        cin.ignore();
        cout << "\nRecord Not Found." << endl;
    }
    getch(); // press key to get out
    return modified;
}
bool deletestudent()
{
    Student st;
    fstream fp;
    bool deleted = false;
    string n;
    int flag = 0;
    system("clear");
    cout << "\nDELETE STUDENT" << endl;
    cout << "\nEnter the Admission num :" << endl;
    cin >> n;
    fp.open("Students.dat", ios::in | ios::out | ios::binary);
    fstream fp2;
    fp2.open("temp.dat", ios::out | ios::binary);
    fp.seekg(0);
    /*
        while(!fp.eof())
        {
            fp>>inputstr;
            if(id == inputstr)
            {

            }
            else
            {
                fout<<
            }
        }
    */
    while (fp.read(reinterpret_cast<char *>(&st), sizeof(Student)))
    {
        if (st.getadm_num() != n)
        {
            fp2.write(reinterpret_cast<char *>(&st), sizeof(Student));
        }
        else
        {
            flag = 1; // student found
        }
    }
    fp2.close();
    fp.close();
    remove("Students.dat");
    rename("temp.dat", "Students.dat"); // data after deletion moved to temp
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
void displayalls() // display all students
{
    ifstream fin;
    Student st;
    system("clear");
    fin.open("Students.dat", ios::binary); // read mode
    if (!fin)
    {
        cout << "File Could Not Be Opened" << endl;
        cin.ignore();
        getch();
        return; // press any key and return
    }
    cout << "\n\t\t\tStudent List\n";
    cout << "==================================================================" << endl;
    cout << "Admission No." << setw(15) << right << "Name" << setw(20) << "Book Issued" << endl;
    cout << "==================================================================" << endl;
    while (fin.read(reinterpret_cast<char *>(&st), sizeof(Student)))
    {
        st.showstudent();
        cout << endl;
    }
    fin.close();
    cin.ignore();
    getch();
}

#endif
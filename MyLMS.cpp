#include "Headers/Headers.h"
void stlogin();
void liblogin();
void adminlogin();
void stmenu(Student &);
void libmenu(Librarian &);
void adminmenu(Admin &);
void start();
bool bookdeposit(string);
bool bookissue(string);
void RegMenu();
void SecMenu();
string lastFileInDirectory();

int main()
{
    char ch;
    start();
    do
    {
        system("clear");
        cout << "\nMAIN MENU" << endl;
        cout << "1. REGULAR MENU" << endl;
        cout << "2. SECURITY MENU" << endl;
        cout << "3. EXIT" << endl;
        cout << "\nPLEASE SELECT YOUR OPTION(1-3)" << endl;
        ch = getch();
        switch (ch)
        {
        case '1':
            RegMenu();
            break;
        case '2':
            SecMenu();
            break;
        case '3':
            return 0;
        default:
            cout << "INVALID CHOICE";
        }
    } while (1);
}
void SecMenu()
{
    int c;
    int idx;
    char ch;
    string data;
    system("clear");
    cout << "\nSECURITY MENU" << endl;
    cout << "1. SHOW LOGS OF CURRENT SESSION" << endl;
    cout << "2. REMOVE A LOG" << endl;
    cout << "3. CHECK LOGS FOR CONSISTENCY" << endl;
    cout << "4. EDIT LOG" << endl;
    cout << "5. READ LOGS FROM LAST FILE" << endl;
    cout << "6. BACK TO MAIN MENU" << endl;
    cout << "\nPLEASE SELECT YOUR OPTION(1-6)" << endl;
    ch = getch();
    switch (ch)
    {
    case '1':
        system("clear");
        LM.chain.printList();
        getch();
        break;
    case '2':
        system("clear");
        cout << "Enter the Log# to delete:" << endl;
        cin >> c;
        cin.ignore();
        LM.chain.RemoveBlock(c) ? cout << "Deleted Successfully" << endl : cout << "Failed to Delete" << endl;
        getch();
        break;
    case '3':
        system("clear");
        idx = LM.chain.checkChainConsistency();
        if (idx == -1)
            cout << "Healthy Chain" << endl;
        else
        {
            cout << "BREACH!" << endl;
            LM.chain.autoRepairchain(idx);
        }
        cout << "Chain Has Been AutoRepaired. You can view the correct logs now." << endl;
        getch();
        break;
    case '4':
        system("clear");
        cout << "Enter the Log# to Modify:" << endl;
        cin >> c;
        cin.ignore();
        cout << "Enter Data to Inject:" << endl;
        getline(cin, data);
        LM.chain.injectDataInChain(c, data) ? cout << "Modification Successful" << endl : cout << "Failed to modify data" << endl;
        getch();
        break;
    case '5':
        system("clear");
        LM.chain.readChain(lastFileInDirectory());
        cout << "Read Done" << endl;
        getch();
        break;
    case '6':
        return;
    default:
        cout << "INVALID CHOICE";
    }
    SecMenu();
}
void RegMenu()
{
    char ch;
    system("clear");
    cout << "\nREGULAR MENU" << endl;
    cout << "1. ADMINISTRATOR LOGIN" << endl;
    cout << "2. LIBRARIAN LOGIN" << endl;
    cout << "3. STUDENT LOGIN" << endl;
    cout << "4. BACK TO MAIN MENU" << endl;
    cout << "\nPLEASE SELECT YOUR OPTION(1-4)" << endl;
    ch = getch();
    switch (ch)
    {
    case '1':
        adminlogin();
        break;
    case '2':
        liblogin();
        break;
    case '3':
        stlogin();
        break;
    case '4':
        return;
        break;
    default:
        cout << "INVALID CHOICE";
    }
    RegMenu();
}
bool bookissue(string sn)
{
    Book bk;
    Student st;
    fstream fp1, fp;
    string bn;
    int found = 0, flag = 0;
    bool issued = false;
    system("clear");
    cout << "\nBOOK ISSUE" << endl;
    fp.open("Students.dat", ios::in | ios::out | ios::binary);
    fp1.open("Book.dat", ios::in | ios::out | ios::binary);
    int pos = fp.tellg();
    while (fp.read(reinterpret_cast<char *>(&st), sizeof(Student)) && found == 0)
    {
        if (st.getadm_num() == sn) // compare admsn no.
        {
            found = 1;
            if (st.gettoken() == 0) // if book not issued
            {
                cout << "\nEnter The Book No. ";
                cin >> bn;

                while (fp1.read(reinterpret_cast<char *>(&bk), sizeof(Book)) && flag == 0)
                {
                    if (bk.getbook_num() == bn) // compare book no.
                    {
                        flag = 1;
                        bk.showbook();
                        st.addtoken();
                        st.setstbook_num(bk.getbook_num()); // pass book no.
                        fp.seekg(pos);
                        fp.write(reinterpret_cast<char *>(&st), sizeof(Student));
                        cout << "\n\nBook Issued Successfully\n\nPlease Note The Book Issue Date Of Your Book And Return Within 15 Days\nOtherwise Fine = 15 Rs Per Day" << endl;
                        issued = true;
                    }
                }
                if (flag == 0)
                {
                    cout << "Book No. Does Not Exist" << endl;
                }
            }
            else
            {
                cout << "You Have Not Returned The Last Book" << endl;
            }
        }
        pos = fp.tellg();
    }
    if (found == 0)
    {
        cout << "Student Record Not Exist" << endl;
    }
    fp.close();
    fp1.close();
    cin.ignore();
    getch();
    return issued;
}
bool bookdeposit(string sn)
{
    bool deposited = false;
    fstream fp, fp1;
    string bn;
    Student st;
    Book bk;
    int found = 0, flag = 0, day, fine;
    system("clear");
    cout << "\nBOOK DEPOSIT" << endl;
    fp.open("Students.dat", ios::in | ios::out | ios::binary);
    fp1.open("Book.dat", ios::in | ios::out | ios::binary);
    int pos = fp.tellg();
    while (fp.read(reinterpret_cast<char *>(&st), sizeof(Student)) && found == 0)
    {
        if (st.getadm_num() == sn) // compare admsn no.
        {
            found = 1;
            if (st.gettoken() == 1) // if book issued
            {
                while (fp1.read(reinterpret_cast<char *>(&bk), sizeof(Book)) && flag == 0)
                {
                    if (bk.getbook_num() == st.getstbook_num())
                    {
                        flag = 1;
                        bk.showbook();
                        cout << "\nBook Deposited In No. Of Days?" << endl;
                        cin >> day;
                        if (day > 15)
                        {
                            fine = (day - 15) * 15;
                            cout << "\nFine = " << fine << " Rs" << endl;
                        }
                        st.resettoken();

                        fp.seekg(pos);
                        fp.write(reinterpret_cast<char *>(&st), sizeof(Student));
                        cout << "\nBook Deposited Successfully" << endl;
                        deposited = true;
                    }
                }
                if (flag == 0)
                {
                    cout << "Book No. Does Not Exists";
                }
            }

            else
            {

                cout << "No Book Issued";
            }
        }
        pos = fp.tellg();
    }
    if (found == 0)
    {
        cout << "Student Record Not Exist" << endl;
    }
    cin.ignore();
    getch();
    fp.close();
    fp1.close();
    return deposited;
}
void start()
{
    system("clear");
    cout << "LIBRARY MANAGEMENT SYSTEM" << endl;
    cout << "By:\nUmer Farooq and Bilal Ikram" << endl;
    cout << "I22-0518 & I22-1636" << endl;
    getch();
}
void adminmenu(Admin &a)
{
    system("clear");
    string ad = "Admin";
    int ch2;
    string num;
    a.adminMenu();
    cin >> ch2;
    switch (ch2)
    {
    case 1:
        LM.addLog(a.getusername(), "Add New Student", ad, writestudent());
        break;
    case 2:
        LM.addLog(a.getusername(), "Displayed All Students", ad, 1);
        displayalls();
        break;
    case 3:
        LM.addLog(a.getusername(), "Displayed Specific Student", ad, 1);
        displaysps();
        break;
    case 4:
        LM.addLog(a.getusername(), "Modified Student", ad, modifystudent());
        break;
    case 5:
        LM.addLog(a.getusername(), "Removed Student", ad, deletestudent());
        break;
    case 6:
        LM.addLog(a.getusername(), "Add New Book", ad, writebook());
        break;
    case 7:
        LM.addLog(a.getusername(), "Displayed All Books", ad, 1);
        displayallb();
        break;
    case 8:
        LM.addLog(a.getusername(), "Displayed Specific Book", ad, 1);
        displayspb();
        break;
    case 9:
        LM.addLog(a.getusername(), "Modified Book", ad, modifybook());
        break;
    case 10:
        LM.addLog(a.getusername(), "Removed Book", ad, deletebook());
        break;
    case 11:
        LM.addLog(a.getusername(), "Add Librarian", ad, writesLib());
        break;
    case 12:
        LM.addLog(a.getusername(), "Displayed All Librarians", ad, 1);
        displayallL();
        break;
    case 13:
        LM.addLog(a.getusername(), "Removed Librarian", ad, deletelib());
        break;
    case 14:
        LM.addLog(a.getusername(), "Add Admin", ad, writesadm());
        break;
    case 15:
        LM.addLog(a.getusername(), "Displayed All Admins", ad, 1);
        displayallA();
        break;
    case 16:
        LM.addLog(a.getusername(), "Removed Admin", ad, deleteadm());
        break;
    case 17:
        return;
    default:
        cout << "Invalid choice";
    }
    adminmenu(a);
}
void libmenu(Librarian &lb)
{
    system("clear");
    string l = "Librarian";
    int ch2;
    string num;
    lb.libMenu();
    cin >> ch2;
    switch (ch2)
    {
    case 1:
        LM.addLog(lb.getusername(), "Add New Student", l, writestudent());
        break;
    case 2:
        LM.addLog(lb.getusername(), "Displayed All Students", l, 1);
        displayalls();
        break;
    case 3:
        LM.addLog(lb.getusername(), "Modified Student", l, modifystudent());
        break;
    case 4:
        LM.addLog(lb.getusername(), "Removed Student", l, deletestudent());
        break;
    case 5:
        LM.addLog(lb.getusername(), "Add New Book", l, writebook());
        break;
    case 6:
        LM.addLog(lb.getusername(), "Displayed All Books", l, 1);
        displayallb();
        break;
    case 7:
        LM.addLog(lb.getusername(), "Displayed Specific Book", l, displayspb());

        break;
    case 8:
        LM.addLog(lb.getusername(), "Modified Book", l, modifybook());
        break;
    case 9:
        LM.addLog(lb.getusername(), "Removed Book", l, deletebook());
        break;
    case 10:
        return;
        cout << "Invalid choice";
    }
    libmenu(lb);
}
void stmenu(Student &st)
{
    system("clear");
    char ch2;
    string s = "Student";
    string num;
    st.studentMenu();
    ch2 = getch();
    switch (ch2)
    {
    case '1':
        LM.addLog(st.getusername(), "Book Issue", s, bookissue(st.getadm_num()));
        break;
    case '2':
        LM.addLog(st.getusername(), "Book Deposit", s, bookdeposit(st.getadm_num()));
        break;
    case '3':
        LM.addLog(st.getusername(), "Displayed All Books", s, 1);
        displayallb();
        break;
    case '4':
        LM.addLog(st.getusername(), "Displayed Specific Book", s, 1);
        displayspb();
        break;
    case '5':
        return;
    default:
        cout << "Invalid choice";
    }
    stmenu(st);
}
void adminlogin()
{
    system("clear");
    Admin a;

    if (a.Check())
    {
        LM.addLog(a.getusername(), "Login", "Admin", 1);
        adminmenu(a);
    }
    else
        LM.addLog(a.getusername(), "Login", "Admin", 0);
    getch();
}
void liblogin()
{
    system("clear");
    Librarian lb;
    if (lb.Check())
    {
        LM.addLog(lb.getusername(), "Login", "Librarian", 1);
        libmenu(lb);
    }
    else
        LM.addLog(lb.getusername(), "Login", "Librarian", 0);
    getch();
}
void stlogin()
{
    system("clear");
    Student st;
    if (st.Check())
    {
        LM.addLog(st.getusername(), "Login", "Student", 1);
        stmenu(st);
    }
    else
        LM.addLog(st.getusername(), "Login", "Student", 0);
    getch();
}
string lastFileInDirectory()
{
    string directory_path = "HashLogs/";
    string file = "";
    DIR *dir = opendir(directory_path.c_str());
    if (dir == nullptr)
    {
        cout << "Failed to open directory." << endl;
        return file;
    }

    string max = file;
    struct dirent *entry;
    while ((entry = readdir(dir)))
    {
        if (entry->d_type == DT_REG)
        { 
            file = (entry->d_name);
            if (file > max)
                max = file;
        }
    }
    return max;
}
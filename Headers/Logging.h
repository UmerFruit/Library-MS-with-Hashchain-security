#ifndef LOGGING_H_
#define LOGGING_H_

#include "Headers.h"
string getTimestamp()
{
    time_t now = time(0);
    string timestamp = ctime(&now);
    return timestamp;
}
void replaceCharInString(string &str, char oldChar, char newChar)
{
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (str[i] == oldChar)
        {
            str[i] = newChar;
        }
    }
}
class st
{
public:
    string log;
    string timestamp;
    string prev_hash;

    st()
    {
        prev_hash = "";
        log = "";
        timestamp = "";
    }
};

class Hash_node
{
public:
    st data;
    Hash_node *next;
    Hash_node *prev;

    Hash_node(string log)
    {
        replaceCharInString(log, ' ', '_');
        replaceCharInString(log, ':', '.');
        data.log = log;
        string t = getTimestamp();
        t.pop_back();
        replaceCharInString(t, ' ', '_');
        replaceCharInString(t, ':', '.');
        data.timestamp = t;
        this->next = NULL;
        this->prev = NULL;
    }
    Hash_node(st &da)
    {

        data.prev_hash = da.prev_hash;
        data.log = da.log;
        data.timestamp = da.timestamp;
        this->next = NULL;
        this->prev = NULL;
    }
    string inputHash()
    {
        string hashinput = data.log + data.prev_hash;
        return hashinput;
    }
    void print()
    {
        string log = data.log;
        replaceCharInString(log, '_', ' ');
        replaceCharInString(log, '.', ':');
        cout << "Log: " << log << endl
             << endl;
    }
    bool operator==(Hash_node &h)
    {
        return (!(data.log.compare(h.data.log)) && next == h.next && prev == h.prev);
    }
};
class Hash_chain
{
public:
    Hash_node *head;
    Hash_node *tail;

public:
    Hash_chain()
    {
        head = NULL;
        tail = NULL;
    }
    void AddBlock(string timestamp, string action, string user, string position, bool s)
    {
        string log = concat(timestamp, action, user, position, s);
        Hash_node *temp = new Hash_node(log);
        if (head == NULL)
        {
            head = temp;
            tail = temp;
        }
        else
        {
            Hash_node *curr = head;

            while (curr->next != NULL)
            {
                curr = curr->next;
            }

            curr->next = temp;

            curr->next->prev = curr;

            tail = temp;
        }
        if (tail->prev != NULL)
            tail->data.prev_hash = Generatehash(tail->prev->inputHash());
        else
            temp->data.prev_hash = ".";
    }

    string concat(string timestamp, string action, string user, string position, bool s)
    {
        timestamp.pop_back();
        string success;
        s ? success = "Done Successfully" : success = "Unsuccessful";
        string cat = "Timestamp: " + timestamp + " " + "User: " + user + " " + position + " " + "Action: " + action + " " + success;
        return cat;
    }
    void printList()
    {
        if (head == NULL)
        {
            cout << "\nThere are no Logs" << endl;
            return;
        }
        int i = 1;
        Hash_node *curr = head;
        while (curr != NULL)
        {
            cout << "LOG# " << i++ << endl;
            curr->print();
            curr = curr->next;
        }
    }

    string Generatehash(string str)
    {
        SHA1 checksum;
        checksum.update(str);
        string hash = checksum.final();
        return hash;
    }
    void autoRepairchain(int idx, int s = 2)
    {
        if (s == 0)
        {
            return;
        }

        Hash_node *ogcurr = head;
        int i = 1;
        while (i < idx)
        {
            ogcurr = ogcurr->next;
            i++;
        }
        string t = ogcurr->data.timestamp;
        string name = "../HashLogs/HASHCHAIN_<" + t + ">.txt";
        ifstream fin;
        fin.open(name);
        Hash_chain temp;
        temp.readChain(name);
        Hash_node *curr2 = temp.head;

        while (curr2->next != NULL)
        {
            curr2 = curr2->next;
        }
        if (s == 1) // remove case
        {
            insertAtIndex(idx - 2, curr2->prev->prev->data);
            if (checkChainConsistency() == -1)
            {
                autoRepairchain(idx, 0);
            }
            else
            {
                autoRepairchain(idx, 2);
            }
        }
        if (s == 2)
        {
            insertAtIndex(idx - 1, curr2->prev->data); // modify case
            RemoveBlock(idx - 1);
            if (checkChainConsistency() == -1)
            {
                autoRepairchain(idx, 0);
            }
            else
            {
                autoRepairchain(idx, 1);
            }
        }
    }
    void insertAtIndex(int index, st data)
    {
        Hash_node *newNode = new Hash_node(data);
        if (!head && index != 0)
        {
            cout << "List is empty. Can't insert at index other than 0." << endl;
            return;
        }

        if (index == 0)
        {
            newNode->next = head;
            if (head)
                head->prev = newNode;
            head = newNode;
            return;
        }

        Hash_node *current = head;
        int currentIndex = 0;

        while (current && currentIndex < index - 1)
        {
            current = current->next;
            currentIndex++;
        }

        if (!current)
        {
            cout << "Index out of range." << endl;
            delete newNode;
            return;
        }

        newNode->next = current->next;
        newNode->prev = current;

        if (current->next)
            current->next->prev = newNode;

        current->next = newNode;
    }
    bool injectDataInChain(int InjectIndex, string InjectData)
    {
        if (head == NULL)
            return false;

        Hash_node *curr = head;
        int i = 1;
        while (i < InjectIndex && curr)
        {
            curr = curr->next;
            i++;
        }
        if (curr == NULL)
        {
            cout << "Index not found" << endl;
            return false;
        }
        curr->data.log = InjectData;
        return true;
    }

    int checkChainConsistency()
    {
        if (head == NULL)
        {
            cout << "Empty Chain" << endl;
            return -1;
        }
        head->next->prev = head;
        Hash_node *curr = head->next;
        while (curr != NULL)
        {
            if (curr->data.prev_hash != Generatehash(curr->prev->inputHash()))
            {
                cout << "Current Node:\n\n";
                curr->print();
                cout << "Next 2 Nodes:\n\n";
                if (curr->next != NULL)
                {
                    curr->next->print();

                    if (curr->next->next != NULL)
                    {
                        curr->next->next->print();
                    }
                }
                cout << "Previous 2 Nodes:\n\n";
                if (curr->prev != NULL)
                {
                    curr->prev->print();

                    if (curr->prev->prev != NULL)
                    {
                        curr->prev->prev->print();
                    }
                }

                return FindBlock(curr);
            }
            curr = curr->next;
        }

        return -1;
    }

    int Size()
    {
        int size = 0;
        Hash_node *curr = head;
        while (curr != NULL)
        {
            curr = curr->next;
            size++;
        }
        return size;
    }
    bool RemoveBlock(int position = -1)
    {
        bool deleted = false;
        if (head == NULL)
        {
            cout << "EMPTY LIST!" << endl;
            return deleted;
        }
        else if (position == -1)
        {
            Hash_node *curr = head;
            while (curr->next != NULL)
            {
                curr = curr->next;
            }
            tail = curr->prev;
            curr->prev->next = curr->next;
            curr->next = NULL;
            delete curr;
            deleted = true;
            return deleted;
        }
        else if (position < 0 || position > Size())
        {
            cout << "Index not found" << endl;
            return deleted;
        }
        else if (position == 1)
        {
            Hash_node *prev = head;
            head = head->next;
            prev->next = NULL;
            delete prev;
            deleted = true;
            return deleted;
        }

        Hash_node *curr = head;
        int i = 1;
        while (i < position)
        {
            curr = curr->next;
            i++;
        }
        if (curr->next == NULL)
        {
            tail = curr->prev;
            curr->prev->next = curr->next;
            curr->next = NULL;
            curr->prev = NULL;
            delete curr;
            deleted = true;
            return deleted;
        }
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        curr->next = NULL;
        curr->prev = NULL;
        delete curr;
        deleted = true;
        return deleted;
    }
    void writeChain()
    {
        ofstream fout;
        string time = getTimestamp();
        time.pop_back();
        replaceCharInString(time, ' ', '_');
        replaceCharInString(time, ':', '.');
        Hash_node *curr = head;
        fout.open("Hashlogs/HASHCHAIN_<" + time + ">.txt");
        if (!fout)
        {
            cout << "Not opened" << endl;
            return;
        }
        int i = 0;
        while (curr)
        {
            size_t s1 = curr->data.log.size();
            size_t s2 = curr->data.prev_hash.size();
            size_t s3 = curr->data.timestamp.size();
            fout << curr->data.log << endl;
            fout << curr->data.prev_hash << endl;
            fout << curr->data.timestamp << endl;
            curr = curr->next;
        }
        fout.close();
    }
    void readChain(const string name)
    {
        ifstream fin;
        fin.open("HashLogs/" + name);
        if (!fin)
        {
            cout << "File doesnt exist" << endl;
            return;
        }
        st test;
        fin >> test.log;
        fin >> test.prev_hash;
        fin >> test.timestamp;
        this->head = new Hash_node(test);
        this->tail = head;
        Hash_node *curr = this->head;
        curr->prev = NULL;
        while (!fin.eof())
        {
            fin >> test.log;
            fin >> test.prev_hash;
            fin >> test.timestamp;
            curr->next = new Hash_node(test);
            curr->next->prev = curr;
            curr = curr->next;
            tail = curr;
        }
        fin.close();
        RemoveBlock();
    }
    void reverse()
    {
        Hash_node *curr = tail;
        while (curr)
        {
            curr->print();
            curr = curr->prev;
        }
    }
    int FindBlock(Hash_node *data)
    {
        if (head == NULL)
        {
            cout << "EMPTY LIST!" << endl;
            return -1;
        }
        int idx, i = 1;

        Hash_node *curr = head;
        while (curr != NULL)
        {
            if (curr == data)
            {
                idx = i;
                break;
            }
            curr = curr->next;
            i++;
        }

        if (curr == data)
        {
            return idx;
        }
        else
        {
            cout << "Data not found :(" << endl;
            return -1;
        }
    }
};
class LogManager
{
public:
    Hash_chain chain;
    // Add a data.log entry to the data.logs
    void addLog(string user, string action, string position, bool s)
    {
        string success;
        s ? success = "Done Successfully" : success = "Unsuccessful";
        ofstream fout;
        fout.open("Logs.txt", ios::app);
        fout << "Timestamp: " << getTimestamp()
             << "User: " << user << " " << position << " \t"
             << "Action: " << action << " " << success << "\n\n";
        fout.close();
        chain.AddBlock(getTimestamp(), action, user, position, s);
        chain.writeChain();
    }
}LM;

#endif
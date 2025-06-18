#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <string>
#include <cctype>
#include <fstream>
#include <iomanip>

typedef double db;
#define vs vector<string>
#define vi vector<int>

using namespace std;

const string clientfilename = "clients.txt";

int random(int from, int to) {
    return rand() % (to - from + 1) + from;
}

struct clients {
    string AC;
    string Pin;
    string name;
    string phone;
    db AB = 0;
    bool markfordel = false;
};

string readclientaccnum() {
    string AC;
    cout << "\nEnter client account number: ";
    cin >> AC;
    return AC;
}

clients readnewclient(string AC) {
    clients cl;
    cl.AC = AC;
    cout << "Enter pin code: ";
    getline(cin >> ws, cl.Pin);
    cout << "Enter name: ";
    getline(cin, cl.name);
    cout << "Enter phone number: ";
    getline(cin, cl.phone);
    cout << "Enter account balance: ";
    cin >> cl.AB;
    return cl;
}

vector<string> splitstring(string s, string sep = "#//#") {
    vs vst;
    string word;
    int pos;
    while ((pos = s.find(sep)) != string::npos) {
        word = s.substr(0, pos);
        if (!word.empty()) vst.push_back(word);
        s.erase(0, pos + sep.length());
    }
    if (!s.empty()) vst.push_back(s);
    return vst;
}

clients linetorecord(string line, string sep = "#//#") {
    clients cl;
    vs vst = splitstring(line);
    if (vst.size() >= 5) {
        cl.AC = vst[0];
        cl.Pin = vst[1];
        cl.name = vst[2];
        cl.phone = vst[3];
        cl.AB = stod(vst[4]);
    }
    return cl;
}

string recordtoline(clients cl, string sep = "#//#") {
    return cl.AC + sep + cl.Pin + sep + cl.name + sep + cl.phone + sep + to_string(cl.AB) + sep;
}

vector<clients> clientsfromfile(string filename) {
    vector<clients> vcl;
    fstream file(filename, ios::in);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            vcl.push_back(linetorecord(line));
        }
        file.close();
    }
    return vcl;
}

bool findclientbyaccnum(string AC, vector<clients> vcl, clients& cl) {
    for (clients& c : vcl) {
        if (c.AC == AC) {
            cl = c;
            return true;
        }
    }
    return false;
}

void adddatalinetofile(string filename, string dataline) {
    fstream file(filename, ios::out | ios::app);
    if (file.is_open()) {
        file << dataline << endl;
    }
}

clients changeclientdata(string AC) {
    clients c;
    c.AC = AC;
    cout << "\nEnter new pin code: ";
    getline(cin >> ws, c.Pin);
    cout << "Enter new name: ";
    getline(cin, c.name);
    cout << "Enter new phone number: ";
    getline(cin, c.phone);
    cout << "Enter new account balance: ";
    cin >> c.AB;
    return c;
}

void addnewclient(string AC) {
    clients cl = readnewclient(AC);
    adddatalinetofile(clientfilename, recordtoline(cl));
}

void printaddclients() {
    cout << "---------------------------------\n";
    cout << "\tAdd New Clients Screen\n";
    cout << "---------------------------------\n\n";
}

void addclients(vector<clients> vcl) {
    char ans = 'Y';
    do {
        clients cl;
        string AC = readclientaccnum();
        while (findclientbyaccnum(AC, vcl, cl)) {
            cout << "Client with [" << AC << "] already exists. Enter another account number: ";
            AC = readclientaccnum();
        }
        addnewclient(AC);
        cout << "Client added successfully. Do you want to add more clients? (Y/N): ";
        cin >> ans;
        ans = toupper(ans);
        vcl = clientsfromfile(clientfilename);
    } while (ans == 'Y');
}

bool markfordelete(string AC, vector<clients>& vcl) {
    for (clients& c : vcl) {
        if (c.AC == AC) {
            c.markfordel = true;
            return true;
        }
    }
    return false;
}

vector<clients> saveclientdatatofile(string filename, vector<clients> vcl) {
    fstream file(filename, ios::out);
    if (file.is_open()) {
        for (clients& c : vcl) {
            if (!c.markfordel) {
                file << recordtoline(c) << endl;
            }
        }
        file.close();
    }
    return vcl;
}

void showClientlist(clients Client) {
    cout << "\nThe following are the client details:\n";
    cout << "Account Number: " << Client.AC << endl;
    cout << "Pin Code: " << Client.Pin << endl;
    cout << "Name: " << Client.name << endl;
    cout << "Phone: " << Client.phone << endl;
    cout << "Account Balance: " << Client.AB << endl;
}

void printdeletescreen() {
    cout << "---------------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "---------------------------------\n\n";
}

bool deleteclientbyaccnum(vector<clients> vcl) {
    clients cl;
    string AC = readclientaccnum();
    if (findclientbyaccnum(AC, vcl, cl)) {
        system("cls");
        showClientlist(cl);
        char ans;
        cout << "\nAre you sure you want to delete this client? (Y/N): ";
        cin >> ans;
        if (tolower(ans) == 'y') {
            markfordelete(AC, vcl);
            saveclientdatatofile(clientfilename, vcl);
            cout << "\nClient deleted successfully.\n";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AC << ") is not found!\n";
    }
    return false;
}

void printupdatescreen() {
    cout << "---------------------------------\n";
    cout << "\tUpdate Client Info Screen\n";
    cout << "---------------------------------\n\n";
}

bool updateclientdata(vector<clients> vcl) {
    clients cl;
    string AC = readclientaccnum();
    if (findclientbyaccnum(AC, vcl, cl)) {
        system("cls");
        showClientlist(cl);
        char ans;
        cout << "\nAre you sure you want to update this client? (Y/N): ";
        cin >> ans;
        if (tolower(ans) == 'y') {
            for (clients& c : vcl) {
                if (c.AC == AC) {
                    c = changeclientdata(AC);
                    break;
                }
            }
            saveclientdatatofile(clientfilename, vcl);
            cout << "\nClient updated successfully.\n";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AC << ") is not found!\n";
    }
    return false;
}

void printclientrec(clients cl) {
    cout << "|" << setw(15) << left << cl.AC;
    cout << "|" << setw(10) << left << cl.Pin;
    cout << "|" << setw(40) << left << cl.name;
    cout << "|" << setw(12) << left << cl.phone;
    cout << "|" << setw(12) << left << cl.AB;
}

void showallclients(vector<clients> vcl) {
    cout << "------------------------------------------------------\n";
    cout << "\t\tClient List (" << vcl.size() << ") Client(s)\n";
    cout << "------------------------------------------------------\n";
    cout << "|" << left << setw(15) << "Account Number";
    cout << "|" << left << setw(10) << "Pin Code";
    cout << "|" << left << setw(40) << "Name";
    cout << "|" << left << setw(12) << "Phone";
    cout << "|" << left << setw(12) << "Account Balance\n";
    cout << "----------------------------------------------------------------------------------------------------\n";
    for (clients c : vcl) {
        printclientrec(c);
        cout << endl;
    }
    cout << "----------------------------------------------------------------------------------------------------\n";
}

void showmainmenu() {
    system("cls");
    cout << "==========================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "==========================================\n";
    cout << "\t[1] Show Client List\n";
    cout << "\t[2] Add New Client\n";
    cout << "\t[3] Delete Client\n";
    cout << "\t[4] Update Client Info\n";
    cout << "\t[5] Find Client\n";
    cout << "\t[6] Exit\n";
    cout << "==========================================\n";
}

void allproject() {
    int n;
    clients cl;
    vector<clients> vcl = clientsfromfile(clientfilename);

    do {
        showmainmenu();
        cout << "Choose an option [1 to 6]: ";
        cin >> n;
        string AC;

        switch (n) {
        case 1:
            system("cls");
            showallclients(vcl);
            break;
        case 2:
            system("cls");
            printaddclients();
            addclients(vcl);
            break;
        case 3:
            system("cls");
            printdeletescreen();
            deleteclientbyaccnum(vcl);
            break;
        case 4:
            system("cls");
            printupdatescreen();
            updateclientdata(vcl);
            break;
        case 5:
            system("cls");
            AC = readclientaccnum();
            if (findclientbyaccnum(AC, vcl, cl)) {
                showClientlist(cl);
            }
            else {
                cout << "\nClient with Account Number (" << AC << ") is not found!\n";
            }
            break;
        case 6:
            cout << "Exiting the program. Goodbye!\n";
            return;
        default:
            cout << "Invalid input. Try again.\n";
            break;
        }
        cout << "\nPress any key to return to the main menu...\n";
        system("pause");
        vcl = clientsfromfile(clientfilename);
    } while (true);
}
int main() {
    srand((unsigned)time(NULL));
    allproject();
    return 0;
}

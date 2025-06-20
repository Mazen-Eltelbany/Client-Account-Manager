#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <string>
#include <cctype>
#include <fstream>
#include <iomanip>
using namespace std;
typedef double db;
#define vs vector<string>
#define vi vector<int>
enum enmainmenuop{
    enshowallclients=1,
    enaddclients=2,
    endeleteclient=3,
    enupdateclient=4,
    enfindclient=5,
    enshowtranscmenu=6,
    enshowmanageusers=7,
    enexit=8
};
enum entransactionmenuop{
    endeposit=1,
    enwithdraw=2,
    entotalbalances=3,
    enmainmenutransc=4
};
enum enmanageusersmenu {
    enlistusers=1,
    enaddnewuser=2,
    endeleteuser=3,
    enupdateuser=4,
    enfinduser=5,
    enmainmenumanageusers=6
};
enum enperimissions {
    eall = -1,
    enshowallclientper = 1,//0001
    enaddclientper = 2,
    endeleteclientper = 4,//1000
    enupdateuclientper = 8,
    enfindclientper = 16,
    entransactionper = 32, 
    enmanageuserper=64,
};

void showtransactionmenu();
void showmanageuserscreen();
void showmainmenu();
void gobacktomainmenu();
void mainmenu();
bool checkperimisions(enperimissions permision);
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
struct suser {
    string username;
    string password;
    int permissions;
    bool markfordel = false;
};
const string clientfilename = "cli.txt";
const string usersfilename = "users.txt";
suser currentuser;


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
suser linetorecord_user(string line, string sep = "#//#") {
    suser user;
    vector<string> vst = splitstring(line, sep);
    if (vst.size() >= 2) {
        user.username = vst[0];
        user.password = vst[1];
        user.permissions =stoi(vst[2]);
    }
    return user;
}

string recordtoline(suser user, string sep = "#//#") {
    string s = user.username + sep + user.password + 
        sep + to_string(user.permissions);
    return s;
}
vector<suser> getuserfromfile(string filename) {
    vector<suser> vuser;
    fstream file(filename, ios::in);
    if (!file.is_open()) {
        cout << " ERROR: Cannot open file" << filename << "'\n";
        return vuser;
    }
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            vuser.push_back(linetorecord_user(line));
        }
        file.close();
    }
    return vuser;
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
void showaccessdeniedmessage() {
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
}
bool finduserbyusername(string username, vector<suser>vus, suser &us) {
    for (suser u : vus) {
        if (u.username == username) {
            us = u;
            return true;
        }
    }
    return false;
}

bool findusersbyusernameandpincode(string username, string password, suser& user) {
    vector<suser> vuser = getuserfromfile(usersfilename);
    for (suser u : vuser) {
        if (u.username == username && u.password == password) {
            user = u;
            return true;
        }
    }
    return false;
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
bool userexistnyusername(string username,string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open()) {
        string line;
        suser user;
        while (getline(file, line)) {
            user = linetorecord_user(line);
            if (user.username == username) {
                return true;
            }
        }
        file.close();
    }
    return false;

}
int readperimissiontoset() {
    int permission = 0;
    char ans = 'n';
    cout << "\nDo you want to give Full Access?y/n?";
    cin >> ans;
    ans = tolower(ans);
    if (ans ==  'y') {
        return enperimissions::eall;
    }
    cout << "\nDo you want to give access to : \n";
	cout << "\nShow client list?y/n?\n";
    cin >> ans;
    ans = tolower(ans);
    if (ans == 'y') {
        permission+=enperimissions::enshowallclientper;
    }
    cout << "\nadd new client?y/n?\n";
    cin >> ans;
    ans = tolower(ans);
    if (ans == 'y') {
        permission+= enperimissions::enaddclientper;
    }
    cout << "\ndelete client?y/n?\n";
    cin >> ans;
    ans = tolower(ans);
    if (ans == 'y') {
        permission+= enperimissions::endeleteclientper;
    }
    cout << "\nUpdate client?y/n\n";
    cin >> ans;
    ans = tolower(ans);
    if (ans == 'y') {
        permission+= enperimissions::enupdateuclientper;
    }
    cout << "\nTransaction?y/n\n";
    cin >> ans;
    ans = tolower(ans);
    if (ans == 'y') {
        permission+= enperimissions::entransactionper;
    }
    cout << "\nmanage users?y/n\n";
    cin >> ans;
    ans = tolower(ans);
    if (ans == 'y') {
        permission+= enperimissions::enmanageuserper;
    }
    return permission;
}
suser readnewuser() {
    suser user;
    cout << "Enter User Name?";
    getline(cin >> ws, user.username);
    while (userexistnyusername(user.username, usersfilename)) {
        cout << "\nUser with [" << user.username 
            << "] already exist, Enter another Username? ";
        getline(cin >> ws, user.username);
    }
	cout << "Enter Password?";
    getline(cin, user.password);
    user.permissions = readperimissiontoset();
    return user;
    
}
void addnewuser() {
    suser user = readnewuser();
    adddatalinetofile(usersfilename,recordtoline(user));
}
void addnewclient(string AC) {
    clients cl = readnewclient(AC);
    adddatalinetofile(clientfilename, recordtoline(cl));
}
string readusername() {
	cout << "Enter User Name? ";
	string username;
	getline(cin >> ws, username);
	return username;
}
void addusers(vector<suser>vuser) {
    char ans = 'y';
    do {
		addnewuser();
		cout << "User added successfully. Do you want to add more users? (Y/N): ";
		cin >> ans;
		ans = toupper(ans);
		vuser = getuserfromfile(usersfilename);
    } while (ans == 'y');
}

void printaddnewuser() {
    cout << "---------------------------------\n";
    cout << "\tAdd New Users Screen\n";
    cout << "---------------------------------\n";
	addusers(getuserfromfile(usersfilename));
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
void printaddclients() {
    if (!checkperimisions(enperimissions::enaddclientper)) {
        showaccessdeniedmessage();
        return;
    }
    cout << "---------------------------------\n";
    cout << "\tAdd New Clients Screen\n";
    cout << "---------------------------------\n\n";
    addclients(clientsfromfile(clientfilename));
}


bool markfordelete_user(string username,vector<suser>&vus) {
    for (suser& us : vus) {
        if (us.username == username) {
            us.markfordel = true;
            return true;
        }
    }
    return false;
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
vector<suser>saveuserdatetofile(string filename, vector<suser>vus) {
    fstream file;
    file.open(filename, ios::out);
    if (file.is_open()) {
        for (suser& us : vus) {
            if (!us.markfordel) {
				file << recordtoline(us) << endl;
            }
        }
    }
    return vus;

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
void showusercard(suser user) {
    cout << "===================================\n";
    cout << "\nThe Following are the User Details:\n";
	cout << "User Name: " << user.username << endl;
	cout << "Password: " << user.password << endl;
	cout << "Permissions: " << user.permissions << endl;
	cout << "===================================\n";
}
bool deleteuserbyusername(vector<suser>vus) {
    suser user;
    string username = readusername();
    if (finduserbyusername(username, vus, user)) {
        while (username == "admin") {
            cout << "you can't delete this user!!!\n";
            cout << "enter another username?\n";
            username = readusername();
        }
        showusercard(user);
        char ans = 'y';
        cout << "\nAre you sure you want to delete this user? (y/n): ";
        cin >> ans;
        ans = tolower(ans);
        if (ans == 'y') {
            markfordelete_user(username, vus);
            vus = saveuserdatetofile(usersfilename, vus);
            cout << "\nUser deleted successfully.\n";
            return true;
        }
    }
    else {
        cout << "\nUser with Username (" << username << ") is not found!\n";
    }
    return false;
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
void printdeletescreen() {
    if (!checkperimisions(endeleteclientper)) {
        showaccessdeniedmessage();
        return;
    }
    cout << "---------------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "---------------------------------\n\n";
	deleteclientbyaccnum(clientsfromfile(clientfilename));
}

void printuserdeletescree() {
    cout << "---------------------------------\n";
    cout << "\tDelete User Screen\n";
    cout << "---------------------------------\n";
    deleteuserbyusername(getuserfromfile(usersfilename));
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
void printupdatescreen() {
    if (!checkperimisions(enperimissions::enupdateuclientper)) {
        showaccessdeniedmessage();
        return;
    }
    cout << "---------------------------------\n";
    cout << "\tUpdate Client Info Screen\n";
    cout << "---------------------------------\n\n";
    updateclientdata(clientsfromfile(clientfilename));
}



void printclientrec(clients cl) {
    cout << "|" << setw(15) << left << cl.AC;
    cout << "|" << setw(10) << left << cl.Pin;
    cout << "|" << setw(40) << left << cl.name;
    cout << "|" << setw(12) << left << cl.phone;
    cout << "|" << setw(12) << left << cl.AB;
}

void showallclients(vector<clients> vcl) {
    if (!checkperimisions(enperimissions::enshowallclientper)) {
        showaccessdeniedmessage();
        return;
    }
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
bool depooperation(string AC, double amount, vector<clients>& vcl) {
    char ans = 'Y';
    cout << "\n\nAre you sure you want perfrom this transaction?y / n ? ";
    cin >> ans;
    if (ans == 'Y' || ans == 'y') {
        for (clients& c : vcl) {
            if (c.AC == AC) {
                c.AB += amount;
                saveclientdatatofile(clientfilename, vcl);
                return true;
            }
        }
        return false;
    }


}
void showdeposcreen() {
    cout << "--------------------------------\n";
    cout << "\tDeposit menu screen\n";
    cout << "--------------------------------\n";
    clients cl;
    vector<clients>vcl = clientsfromfile(clientfilename);
    string AC = readclientaccnum();
    while (!findclientbyaccnum(AC, vcl, cl)) {
        cout << "\nClient with [" << AC << "] does not exist.\n";
        AC = readclientaccnum();
    }
    showClientlist(cl);
    double amount = 0;
    cout << "\nPlease enter deposit amount?";
    cin >> amount;
    depooperation(AC, amount, vcl);
}
bool withdrawoperation(string AC, double amount, vector<clients>& vcl) {
    char ans = 'y';
    cout << "\n\nAre you sure you want perfrom this transaction?y / n ? ";
    cin >> ans;
    if (ans == 'Y' || ans == 'y') {
        for (clients& c : vcl) {
            if (c.AC == AC) {
                c.AB -= amount;
                saveclientdatatofile(clientfilename, vcl);
                return true;
            }
        }
        return false;
    }
}
suser changeuserdata(string username) {
    suser user;
    user.username = username;
    cout << "Enter the new password?";
    getline(cin >> ws, user.password);
    user.permissions=readperimissiontoset();
    return user;
}
bool updateuserdata(vector<suser>vus) {
    string username = readusername();
    suser user;
    if (finduserbyusername(username, vus, user)) {
        showusercard(user);
        char ans = 'y';
        cout << "Are you sure you want to update this user?y/n?";
        cin >> ans;
        if (tolower(ans) == 'y') {
            for (suser &us : vus) {
                if (us.username == username) {
                    us=changeuserdata(username);
                    break;
                }
            }
            vus=saveuserdatetofile(usersfilename,vus);
			cout << "\nUser updated successfully.\n";
            return true;
        }
    }
    else {
        cout << "\nUser with Username (" <<username<< ") is not found!\n";
    }
	return false;
}
void printupdateuserscreen() {
    cout << "==============================\n";
    cout << "\tUpdat User Screen\n";
    cout << "==============================\n";
    updateuserdata(getuserfromfile(usersfilename));
}
void showwithdrawscreen() {
    cout << "--------------------------------\n";
    cout << "\tWithDraw menu screen\n";
    cout << "--------------------------------\n";
    string AC = readclientaccnum();
    clients cl;
    vector<clients>vcl = clientsfromfile(clientfilename);
    while (!findclientbyaccnum(AC, vcl, cl)) {
        cout << "\nClient with [" << AC << "] does not exist.\n";
        AC = readclientaccnum();
    }
    double amount = 0;
    cout << "\nPlease enter With Draw amount?";
    cin >> amount;
    while (amount > cl.AB) {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << cl.AB << endl;
        cout << "Please Enter another amount?";
        cin >> amount;
    }
    withdrawoperation(AC, amount, vcl);
}
void showtotalbalancesscreen() {
    vector<clients>vcl = clientsfromfile(clientfilename);
    cout << "\n\t\t\t\t\tBalances List(" << vcl.size() << ")Client(s)";
    cout << "\n_____________________________________________";
    cout << "______________________________________\n" << endl;
    cout << '|' << left << setw(15) << "Account Number";
    cout << "|" << left << setw(40) << "Client Name";
    cout << "|" << left << setw(12) << "Balance";
    cout << "\n_____________________________________________";
    cout << "______________________________________\n" << endl;
    double tot = 0;
    if (vcl.size() == 0) {
        cout << "\t\t\t\tNo Clients Available In the System!";
    }
    else {
        for (clients& c : vcl) {
            printclientrec(c);
            tot += c.AB;
            cout << endl;
        }
        cout <<
            "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;
        cout << "\t\t\t\t\t Total Balances = " << tot;
    }



}
void gobacktotransactionmenu() {
    cout << "\n\nPress any key to go back to Transactions Menu...";
    system("pause>0");
    showtransactionmenu();
}
void gobacktomanageusermenu() {
    cout << "\n\nPress any key to go back to Manage User Menu...";
    system("pause>0");
    showmanageuserscreen();
}
void gobacktomainmenu() {
    cout << "\n\nPress any key to go back to main menu...";
    system("pause>0");
    showmainmenu();
}
entransactionmenuop choiceontransactionmenu() {
    short n;
    do {
        cout << "\nChoose what do you want to do?[1 to 4]?\n";
        cin >> n;
    } while (n < 1 || n > 4);
    return (entransactionmenuop)n;
}
void preformoperationontransmenu(entransactionmenuop enchoice) {
    switch (enchoice)
    {
    case entransactionmenuop::endeposit:
        system("cls");
        showdeposcreen();
        gobacktotransactionmenu();
        break;
    case entransactionmenuop::enwithdraw:
        system("cls");
        showwithdrawscreen();
        gobacktotransactionmenu();
        break;
    case entransactionmenuop::entotalbalances:
        system("cls");
        showtotalbalancesscreen();
        gobacktotransactionmenu();
        break;
    case entransactionmenuop::enmainmenutransc:
        system("cls");
        gobacktomainmenu();
    default:
        break;
    }
}

void printuserrecordline(suser user) {
    cout << "|" << setw(15) << left << user.username;
    cout << "|" << setw(10) << left << user.password;
    cout << "|" << setw(40) << left << user.permissions;
}
void listusers() {
    vector<suser>vus=getuserfromfile(usersfilename);
    cout << "\n\t\t\t\t\t Users List (" << vus.size() << ") User(s)";
    cout << "\n------------------------------------------------------";
    cout << "------------------------------------\n" << endl;
    cout << "|" << left << setw(15)<<"User Name";
    cout << "|" << left << setw(10) << "Password";
    cout << "|" << left << setw(40) << "Permissions";
    cout << "\n------------------------------------------------------";
    cout << "------------------------------------\n" << endl;
    if (vus.size() ==0) {
        cout << "\t\t\t\tNo Users Available In the Sytem!";
    }
    else {
        for (suser us : vus) {
            printuserrecordline(us);
            cout << endl;
        }
    }
}
void finduserbyusernamefunc() {
    string username = readusername();
    suser user;
    vector<suser>vus = getuserfromfile(usersfilename);
    if (finduserbyusername(username, vus, user)) {
        showusercard(user);
    }
}
void printfinduserscreen() {
    cout << "===================================\n";
    cout << "\tFind User Screen\n";
    cout << "===================================\n";
    finduserbyusernamefunc();
}

enmanageusersmenu readmanageusermenu() {
    short n;
    do {
        cout << "Choose What do you want to do?[1 to 6]:";
        cin >> n;
    } while (1 > n || n > 6);
    return (enmanageusersmenu)n;
}
bool checkperimisions(enperimissions permision) {
    if (currentuser.permissions == enperimissions::eall) {
		return true; 
    }
	if (permision & currentuser.permissions) {
		return true; 
	}
    else {
        return false;
    }
}
void processonmanageusermenu(enmanageusersmenu enchoice) {
    switch (enchoice) {
    case enmanageusersmenu::enlistusers:
		system("cls");
        listusers();
        gobacktomanageusermenu();
        break;
	case enmanageusersmenu::enaddnewuser:
		system("cls");
        printaddnewuser();
        gobacktomanageusermenu();
		break;
    case enmanageusersmenu::endeleteuser:
		system("cls");
		printuserdeletescree();
		gobacktomanageusermenu();
		break;
    case enmanageusersmenu::enupdateuser:
        system("cls");
        printupdateuserscreen();
        gobacktomanageusermenu();
    case enmanageusersmenu::enfinduser:
        system("cls");
        printfinduserscreen();
        gobacktomanageusermenu();
    case enmanageusersmenu::enmainmenumanageusers:
        system("cls");
        showmainmenu();
        break;
    }
}
void printloginscreen() {
    cout << "==============================\n";
    cout << "\tLogin Screen\n";
    cout << "==============================\n";
}
void showtransactionmenu() {
    system("cls");
    if (!checkperimisions(enperimissions::entransactionper)) {
        showaccessdeniedmessage();
		return;
    }
    cout << "==============================================\n";
    cout << "\t\Transaction menu screen\n";
    cout << "==============================================\n";
    cout << "\t[1] Deposit\n";
    cout << "\t[2] WithDraw\n";
    cout << "\t[3] Total Balances\n";
    cout << "\t[4] Main Menu\n";
    cout << "==============================================";
    preformoperationontransmenu(choiceontransactionmenu());
}
void showmanageuserscreen() {
    system("cls");
    if (!checkperimisions(enperimissions::enmanageuserper)) {
        showaccessdeniedmessage();
        return;
    }
    cout << "================================================\n";
    cout << "\t\tManage User Menu Screen\n";
    cout << "================================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menu\n";
    cout << "================================================\n";
    processonmanageusermenu(readmanageusermenu());
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
    cout << "\t[6] Transactions\n";
    cout << "\t[7] Manage Users\n";
    cout << "\t[8] Logout\n";
    cout << "==========================================\n";
}
void login() {
    string username, password;
    bool loginfailed = false;
    do {
        system("cls");
        printloginscreen();
        if (loginfailed) {
            cout << "Invlaid Username/Password!\n";
        }
        cout << "Enter Username?";
        getline(cin >> ws, username);
        cout << "Enter Password?";
        getline(cin, password);
        loginfailed = !findusersbyusernameandpincode(username, password, currentuser);
    } while (loginfailed);
    mainmenu();
}
void findclientbyaccnumfunc() {
    string ac = readclientaccnum();
    clients cl;
    vector<clients>vcl = clientsfromfile(clientfilename);
    if (findclientbyaccnum(ac, vcl, cl)) {
        showClientlist(cl);
    }
    else {
        cout << "\nClient with Account Number (" << ac << ") is not found!\n";
    }
}
void printfindclientscreen() {
    if (!checkperimisions(enperimissions::enfindclientper)) {
        showaccessdeniedmessage();
        return;
    }
    cout << "---------------------------------\n";
    cout << "\tFind Client Screen\n";
    cout << "---------------------------------\n";
    findclientbyaccnumfunc();
}
void mainmenu() {
    int n;
    clients cl;
    vector<clients> vcl = clientsfromfile(clientfilename);
    do {
        showmainmenu();
        cout << "Choose an option [1 to 8]: ";
        cin >> n;
        string AC;
        switch ((enmainmenuop)(n)) {
        case enmainmenuop::enshowallclients :
            system("cls");
            showallclients(vcl);
            gobacktomainmenu();
            break;
        case enmainmenuop::enaddclients:
            system("cls");
            printaddclients();
            gobacktomainmenu();
            break;
        case enmainmenuop::endeleteclient:
            system("cls");
            printdeletescreen();
            gobacktomainmenu();
            break;
        case enmainmenuop::enupdateclient:
            system("cls");
            printupdatescreen();
            gobacktomainmenu();
            break;
        case enmainmenuop::enfindclient:
            system("cls");
            printfindclientscreen();
            gobacktomainmenu();
            break;
        case enmainmenuop::enshowtranscmenu:
            showtransactionmenu();
            break;
        case enmainmenuop::enshowmanageusers:
            showmanageuserscreen();
            break;
        case enmainmenuop::enexit:
            login();
            break;
        default:
            cout << "Invalid input. Try again.\n";
            break;
        }
        vcl = clientsfromfile(clientfilename);
    } while (true);
}
int main() {
    srand((unsigned)time(NULL));
    login();
    return 0;
}


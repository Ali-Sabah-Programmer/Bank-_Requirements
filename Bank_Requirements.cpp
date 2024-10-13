// Project - Bank (Requirements)
//_______________________________________________________________________________

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

void ShowMainMenu();
void ShowTranscationsMenu();
void Login();
void ShowManageUsersMenu();

enum enMainMenuOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eShowTransactionsMenu = 6,
    eManageUsers = 7, Logout = 8
};

enum enTransactionsMenuOptions
{
    eDeposit = 1, eWithdraw = 2,
    eShowTotalBalance = 3, eShowMainMenu = 4,
};

enum enManageUsersOptions
{
    eListUsers = 1, eAddNewUsers = 2,
    eDeleteUsers = 3, eUpdateUsers = 4,
    eFindUsers = 5, eMainMenu
};

enum enMainMenuPremissions
{
    eAll = -1, pListClients = 1, pAddNewClient = 2,
    pDeleteClient = 4, pUpdateClient = 8,
    pFindClient = 16, pTransactionsMenu = 32,
    pManageUsers = 64
};

struct stUsers {

    string Username;
    string Password;
    int Permissions;
    bool MarkForDelete = false;

};

stUsers CurrentUser;

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double
    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
        }

        MyFile.close();

    }
    return false;
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

bool CheckAccessPermission(enMainMenuPremissions Premission) {

    if (CurrentUser.Permissions == enMainMenuPremissions::eAll)
        return true;
    else
        return ((Premission & CurrentUser.Permissions) == Premission) ? true : false;
}

void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenu();

}

void ShowAccessDeniedMessage()
{
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuPremissions::pListClients))
    {
        ShowAccessDeniedMessage();
        return;
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }

        }

        MyFile.close();
    }

    return vClients;
}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enMainMenuPremissions::pDeleteClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enMainMenuPremissions::pUpdateClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuPremissions::pAddNewClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();
}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enMainMenuPremissions::pFindClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

void GoBackToTransactionsMenu()
{
    cout << "\n\nPress any key to go back to Transactions Menu...";
    system("pause>0");
    ShowTranscationsMenu();

}

short ReadMainMenuOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

short ReadTransactionsMenuOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients) {


    char Answer = 'n';
    cout << "\n\nAre you sure you want Perform this Transactions? y/n ? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {

        for (sClient& C : vClients)
        {

            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully, New Balance is : " << C.AccountBalance << endl;;
                return true;
            }

        }

    }
    return false;

}

void ShowDepositScreen() {

    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] is not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}

void ShowWithdrawScreen() {

    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] is not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    while (Client.AccountBalance < Amount)
    {
        cout << "\n\nAmount Exceeds the balance, you can Withdraw up to : " << Client.AccountBalance;
        cout << "\nPlease enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}

void PrintClientRecordBalanceLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void ShowTotalBalances() {

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
        for (sClient& Client : vClients)
        {
            TotalBalances += Client.AccountBalance;
            PrintClientRecordBalanceLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\n\t\t\t\t\tTotal tBalances = " << TotalBalances;

}

void ShowTotalBalancesScreen() {

    ShowTotalBalances();

}

void PerfromTransactionsMenuOption(enTransactionsMenuOptions TransactionsMenuOptions)
{
    switch (TransactionsMenuOptions)
    {
    case enTransactionsMenuOptions::eDeposit:

        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenu();
        break;

    case enTransactionsMenuOptions::eWithdraw:
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionsMenu();
        break;

    case enTransactionsMenuOptions::eShowTotalBalance:
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenu();
        break;

    case enTransactionsMenuOptions::eShowMainMenu:
        system("cls");
        ShowMainMenu();
        break;
    }
}

void ShowTranscationsMenu() {

    if (!CheckAccessPermission(enMainMenuPremissions::pTransactionsMenu))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenu();
    }
    system("cls");
    cout << "===========================================\n";
    cout << "\t\Transactions Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balance.\n";
    cout << "\t[4] Main Menu.\n";
    cout << "===========================================\n";
    PerfromTransactionsMenuOption((enTransactionsMenuOptions)ReadTransactionsMenuOption());
}

stUsers ConvertUserLinetoRecord(string Line, string Seperator = "#//#")
{
    stUsers User;
    vector<string> vUserDate;
    vUserDate = SplitString(Line, Seperator);

    User.Username = vUserDate[0];
    User.Password = vUserDate[1];
    User.Permissions = stoi(vUserDate[2]);

    return User;
}

vector <stUsers> LoadUsersDataFromFile(string FileName)
{
    vector <stUsers> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        stUsers User;

        while (getline(MyFile, Line))
        {
            User = ConvertUserLinetoRecord(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}

bool FindUserByUsernameAndPasword(string Username, string Password,stUsers& User) {

    vector<stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (stUsers& U : vUsers)
    {
        if (U.Username == Username && U.Password == Password)
        {
            User = U;
            return true;
        }
    }
    return false;

}

bool LoadUserInfo(string Username, string Password) {

    if (FindUserByUsernameAndPasword(Username, Password, CurrentUser))
        return true;
    else
        return false;
}

void GoBackToManageUsersMenu()
{
    cout << "\n\nPress any key to go back to Manage Users Menue...";
    system("pause>0");
    ShowManageUsersMenu();

}

void PrintUserRecordLine(stUsers User)
{
    cout << "| " << setw(15) << left << User.Username;
    cout << "| " << setw(10) << left << User.Password;
    cout << "| " << setw(40) << left << User.Permissions;

}

void ShowAllUsersScreen()
{
    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tClient List (" << vUsers.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Username";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vUsers.size() == 0)
        cout << "\t\t\t\tNo Users Available In the System!";
    else

        for (stUsers &User : vUsers)
        {

            PrintUserRecordLine(User);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void ShowListUsersScreen()
{
    ShowAllUsersScreen();
}

bool UserExistsByUsername(string Username, string FileName)
{

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        stUsers User;

        while (getline(MyFile, Line))
        {
            User = ConvertUserLinetoRecord(Line);
            if (User.Username == Username)
            {
                MyFile.close();
                return true;
            }
        }

        MyFile.close();

    }
    return false;
}

int ReadPremissionsToSet() {

    int Premissions = 0;

    char Answer = 'n';
    cout << "\nDo you want give full access? y/n? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        return -1;
    }

    cout << "\nDo you want give full access to :\n ";

    cout << "\nShow Clients List? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Premissions += enMainMenuPremissions::pListClients;

    cout << "\nAdd New Client y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Premissions += enMainMenuPremissions::pAddNewClient;

    cout << "\nDelete Client y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Premissions += enMainMenuPremissions::pDeleteClient;


    cout << "\nUpdate Client Info y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Premissions += enMainMenuPremissions::pUpdateClient;


    cout << "\nFind Client y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Premissions += enMainMenuPremissions::pFindClient;


    cout << "\nTransactions y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Premissions += enMainMenuPremissions::pTransactionsMenu;


    cout << "\nManage Users y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Premissions += enMainMenuPremissions::pManageUsers;

    return Premissions;
}

stUsers ReadNewUser()
{
    stUsers User;

    cout << "Enter Username? ";
    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, User.Username);

    while (UserExistsByUsername(User.Username, UsersFileName))
    {
        cout << "\nUSer with [" << User.Username << "] already exists, Enter another Username? ";
        getline(cin >> ws, User.Username);
    }

    cout << "Enter Password? ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPremissionsToSet();

    return User;
}

string ConvertRecordUserToLine(stUsers User, string Seperator = "#//#")
{

    string stUserRecord = "";
    stUserRecord +=User.Username + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.Permissions);

    return stUserRecord;
}

void AddNewUser()
{
    stUsers User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertRecordUserToLine(User));
}

void AddNewUsers()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Users:\n\n";

        AddNewUser();
        cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

void ShowAddNewUsersScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Users Screen";
    cout << "\n-----------------------------------\n";

    AddNewUsers();
}

bool FindUserByUsername(string Username, vector <stUsers>& vUsers, stUsers& User)
{
    for (stUsers & U : vUsers)
    {

        if (U.Username == Username)
        {
            User = U;
            return true;
        }

    }
    return false;
}

void PrintUserCard(stUsers User)
{
    cout << "\nThe following are the user details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername    : " << User.Username;
    cout << "\nPassword    : " << User.Password;
    cout << "\nPermissions : " << User.Permissions;
    cout << "\n-----------------------------------\n";

}

bool MarkUserForDeleteByUsername(string Username, vector <stUsers>& vUsers)
{
    for (stUsers& U : vUsers)
    {
        if (U.Username == Username)
        {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector <stUsers> SaveUsersDataToFile(string FileName, vector <stUsers> vUsers)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (stUsers U : vUsers)
        {

            if (U.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordUserToLine(U);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vUsers;

}

bool DeleteUserByUsername(string Username, vector <stUsers>& vUsers)
{
    if (Username == "Admin")
    {
        cout << "\nYou cannont delete this user.\n";
        return false;
    }

    stUsers User;
    char Answer = 'n';

    if (FindUserByUsername(Username, vUsers, User))
    {
 
        PrintUserCard(User);

        cout << "\n\nAre you sure you want delete this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteByUsername(Username, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);

            //Refresh Clients 
            vUsers = LoadUsersDataFromFile(UsersFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nUser with Username (" << Username << ") is Not Found!";
        return false;
    }
}

string ReadUserName()
{
    string Username = "";

    cout << "\nPlease enter Username? ";
    cin >> Username;
    return Username;

}

void ShowDeleteUserScreen()
    {
        cout << "\n-----------------------------------\n";
        cout << "\tDelete Users Screen";
        cout << "\n-----------------------------------\n";

        vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
        string Username = ReadUserName();
        DeleteUserByUsername(Username, vUsers);
    }

short ReadManageUsersOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

stUsers ChangeUserRecord(string Username)
{
    stUsers User;

    User.Username = Username;

    cout << "Enter Password? ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPremissionsToSet();

    return User;
}

bool UpdateUserByUsername(string Username, vector <stUsers>& vUsers)
{

    stUsers User;
    char Answer = 'n';

    if (FindUserByUsername(Username, vUsers, User))
    {

        PrintUserCard(User);
        cout << "\n\nAre you sure you want update this User? y/n ? ";
        cin >> Answer;
        if (toupper(Answer)== 'Y')
        {

            for (stUsers& U : vUsers)
            {
                if (U.Username == Username)
                {
                    U = ChangeUserRecord(Username);
                    break;
                }

            }

            SaveUsersDataToFile(UsersFileName, vUsers);

            cout << "\n\nUser Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nUser with Account Number (" << Username << ") is Not Found!";
        return false;
    }

}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";

    vector <stUsers> vUser = LoadUsersDataFromFile(UsersFileName);
    string Username = ReadUserName();
    UpdateUserByUsername(Username, vUser);

}

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    stUsers User;
    string Username = ReadUserName();
    if (FindUserByUsername(Username, vUsers, User))
        PrintUserCard(User);
    else
        cout << "\nClient with Account Number[" << Username << "] is not found!";
}

void PerfromenManageUsersOptionsOption(enManageUsersOptions ManageUsersOptions)
{
    switch (ManageUsersOptions)
    {
    case enManageUsersOptions::eListUsers:
    {
        system("cls");
        ShowListUsersScreen();
        GoBackToManageUsersMenu();
        break;
    }
    case enManageUsersOptions::eAddNewUsers:
        system("cls");
        ShowAddNewUsersScreen();
        GoBackToManageUsersMenu();
        break;

    case enManageUsersOptions::eDeleteUsers:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenu();
        break;

    case enManageUsersOptions::eUpdateUsers:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenu();
        break;

    case enManageUsersOptions::eFindUsers:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenu();
        break;

    case enManageUsersOptions::eMainMenu:
        system("cls");
        ShowMainMenu();
        break;
    }
}

void ShowManageUsersMenu() {

    if (!CheckAccessPermission(enMainMenuPremissions::pManageUsers))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenu();
    }
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New Users.\n";
    cout << "\t[3] Delete Users.\n";
    cout << "\t[4] Update Users.\n";
    cout << "\t[5] Find Users.\n";
    cout << "\t[6] Main Menu.\n";
    cout << "===========================================\n";
    PerfromenManageUsersOptionsOption((enManageUsersOptions)ReadManageUsersOption());

}

void PerfromMainMenuOption(enMainMenuOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenuOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::eShowTransactionsMenu:
        system("cls");
        ShowTranscationsMenu();
        break;
    case enMainMenuOptions::eManageUsers:
        system("cls");
        ShowManageUsersMenu();
        break;
    case enMainMenuOptions::Logout:
        system("cls");
        Login();
        break;
    }
}

void ShowMainMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";
    PerfromMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}

void Login() {

    bool LoginFaild = false;
    string Username, Password;

    do
    {
        system("cls");

        cout << "\n-----------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n-----------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invalid Username/Password\n";
        }

        cout << "Enter Username ? ";
        cin >> Username;
        cout << "Enter Password ? ";
        cin >> Password;

        LoginFaild = !LoadUserInfo(Username, Password);

    } while (LoginFaild);

    ShowMainMenu();

}

int main()
{
    Login();
    system("pause>0");
    return 0;
}
#pragma once
#include <iostream>
using namespace std;
#include "clsPerson.h";
#include "clsString.h";
#include <string>
#include <vector>
#include <fstream>

class clsBankClient : public clsPerson {
private:
	enum enMode {EmptyMode =0, UpdateMode =1, AddNewMode=3};
	enMode _Mode;

	string _AccountNumber;
	string _PinCode;
	float _AccountBalance;
	
	bool _MarkForDeleted = false;
	struct stTrnsferLogRecord;

	static clsBankClient _ConvertLineToClientObject(string Line, string Separator = "#//#")
	{
		vector<string> vClientData;
		vClientData = clsString::Split(Line, Separator);

		return clsBankClient(enMode::UpdateMode, vClientData[0], vClientData[1], vClientData[2], vClientData[3], vClientData[4], vClientData[5], static_cast<float>(stod(vClientData[6])));
	}
	static clsBankClient _GetEmptyClientObject() {
		return clsBankClient(enMode::EmptyMode, "", "", "", "", "", "",0);
	}

	static  vector <clsBankClient> _LoadClientsDataFromFile() {
		vector <clsBankClient> vClients;

		fstream MyFile;
		MyFile.open("Clients.txt", ios::in);//read Mode

		if (MyFile.is_open()) {
			string Line;
			while (getline(MyFile, Line)) {
				clsBankClient Client = _ConvertLineToClientObject(Line);
				vClients.push_back(Client);
			}
			MyFile.close();
		}
		return vClients;
	}
	static string _ConvertClientObjectToLine(clsBankClient Client, string Seperator = "#//#") {
		string stClientRecord = "";
		stClientRecord += Client.FirstName + Seperator;
		stClientRecord += Client.LastName + Seperator;
		stClientRecord += Client.Email + Seperator;
		stClientRecord += Client.PhoneNumber + Seperator;
		stClientRecord += Client.getAccountNumber() + Seperator;
		stClientRecord += Client.PinCode + Seperator;
		stClientRecord += to_string(Client.AccountBalance);

		return stClientRecord;
	}
	static void _SaveClientsDataToFile(vector <clsBankClient> vClients) {
		fstream MyFile;
		MyFile.open("Clients.txt", ios::out);//overwrite

		string DataLine;
		if (MyFile.is_open()) {
			for (clsBankClient C : vClients) {
				DataLine = _ConvertClientObjectToLine(C);
				MyFile << DataLine << endl;
			}
			MyFile.close();
		}
	}

	void _AddDataLineToFile(string  stDataLine){
		fstream MyFile;
		MyFile.open("Clients.txt", ios::out | ios::app);

		if (MyFile.is_open())	{
			MyFile << stDataLine << endl;
			MyFile.close();
		}
	}
	void _Update() {
		vector <clsBankClient> _vClients;
		_vClients = _LoadClientsDataFromFile();

		for (clsBankClient& C : _vClients) {
			if (C.getAccountNumber() == getAccountNumber()) {
				C = *this;
				break;
			}
		}
		_SaveClientsDataToFile(_vClients);

	}
	void _AddNew() {
		_AddDataLineToFile(_ConvertClientObjectToLine(*this));
	}
	string _PrepareTransferLogRecord(float Amount, clsBankClient DestinationClient,	string UserName, string Seperator = "#//#"){
		string TransferLogRecord = "";
		TransferLogRecord += clsDate::GetSystemDateTimeString() + Seperator;
		TransferLogRecord += getAccountNumber() + Seperator;
		TransferLogRecord += DestinationClient.getAccountNumber() + Seperator;
		TransferLogRecord += to_string(Amount) + Seperator;
		TransferLogRecord += to_string(AccountBalance) + Seperator;
		TransferLogRecord += to_string(DestinationClient.AccountBalance) + Seperator;
		TransferLogRecord += UserName;
		return TransferLogRecord;
	}
	void _RegisterTransferLog(float Amount, clsBankClient DestinationClient, string UserName){
		string stDataLine = _PrepareTransferLogRecord(Amount, DestinationClient, UserName);

		fstream MyFile;
		MyFile.open("TransferLog.txt", ios::out | ios::app);

		if (MyFile.is_open()){
			MyFile << stDataLine << endl;
			MyFile.close();
		}
	}
	static stTrnsferLogRecord _ConvertTransferLogLineToRecord(string Line, string Seperator = "#//#"){
		stTrnsferLogRecord TrnsferLogRecord;

		vector <string> vTrnsferLogRecordLine = clsString::Split(Line, Seperator);
		TrnsferLogRecord.DateTime = vTrnsferLogRecordLine[0];
		TrnsferLogRecord.SourceAccountNumber = vTrnsferLogRecordLine[1];
		TrnsferLogRecord.DestinationAccountNumber = vTrnsferLogRecordLine[2];
		TrnsferLogRecord.Amount = stod(vTrnsferLogRecordLine[3]);
		TrnsferLogRecord.srcBalanceAfter = stod(vTrnsferLogRecordLine[4]);
		TrnsferLogRecord.destBalanceAfter = stod(vTrnsferLogRecordLine[5]);
		TrnsferLogRecord.UserName = vTrnsferLogRecordLine[6];

		return TrnsferLogRecord;
	}

public:
	struct stTrnsferLogRecord
	{
		string DateTime;
		string SourceAccountNumber;
		string DestinationAccountNumber;
		float Amount;
		float srcBalanceAfter;
		float destBalanceAfter;
		string UserName;

	};
	clsBankClient(enMode Mode, string FirstName, string LastName, string Email, string PhoneNumber,string AccountNumber, string PinCode,float AccountBalance)
		: clsPerson (FirstName, LastName, Email, PhoneNumber)
	{
		_Mode = Mode;
		_AccountNumber = AccountNumber;
		_PinCode = PinCode;
		_AccountBalance = AccountBalance;
	}

	string getAccountNumber() {
		return _AccountNumber;
	}
	//++++++++
	string getPinCode() {
		return _PinCode;
	}
	void setPinCode(string PinCode) {
		_PinCode = PinCode;
	}
	__declspec(property(get = getPinCode, put = setPinCode)) string PinCode;

	//++++++++
	float getAccountBalance() {
		return _AccountBalance;
	}
	void setAccountBalance(float AccountBalance) {
		_AccountBalance = AccountBalance;
	}
	__declspec(property(get = getAccountBalance, put = setAccountBalance)) float AccountBalance;
	
	//++++++++
	bool IsEmpty() {
		return (_Mode == enMode::EmptyMode);
	}

	//++++++++
	string FullName() {
		return FirstName + " " + LastName;
	}

	//++++++++
	void Print()
	{
		cout << "\nClient Card:";
		cout << "\n___________________";
		cout << "\nFirstName   : " << FirstName;
		cout << "\nLastName    : " << LastName;
		cout << "\nFull Name   : " << FullName();
		cout << "\nEmail       : " << Email;
		cout << "\nPhone       : " << PhoneNumber;
		cout << "\nAcc. Number : " << _AccountNumber;
		cout << "\nPassword    : " << _PinCode;
		cout << "\nBalance     : " << _AccountBalance;
		cout << "\n___________________\n";

	}

	//++++++++
	static clsBankClient Find(string AccountNumber)
	{
		fstream MyFile;
		MyFile.open("Clients.txt", ios::in);//read Mode

		if (MyFile.is_open())
		{
			string Line;
			while (getline(MyFile, Line))
			{
				clsBankClient Client = _ConvertLineToClientObject(Line);
				if (Client.getAccountNumber() == AccountNumber)
				{
					MyFile.close();
					return Client;
				}

			}

			MyFile.close();

		}

		return _GetEmptyClientObject();
	}
	static clsBankClient Find(string AccountNumber, string PinCode)
	{



		fstream MyFile;
		MyFile.open("Clients.txt", ios::in);//read Mode

		if (MyFile.is_open())
		{
			string Line;
			while (getline(MyFile, Line))
			{
				clsBankClient Client = _ConvertLineToClientObject(Line);
				if (Client.getAccountNumber() == AccountNumber && Client.PinCode == PinCode)
				{
					MyFile.close();
					return Client;
				}

			}

			MyFile.close();

		}
		return _GetEmptyClientObject();
	}

	//++++++++
	static bool IsClientExist(string AccountNumber) {
		clsBankClient Client1 = clsBankClient::Find(AccountNumber);
		return (!Client1.IsEmpty());
		}

	//++++++++
	enum enSaveResults { svFaildEmptyObject = 0, svSucceeded = 1, svFaildAccountNumberExists =2 };
    enSaveResults Save(){
		switch (_Mode) {
		case enMode::EmptyMode: {
			return enSaveResults::svFaildEmptyObject;
		}
        case enMode::UpdateMode:{
			_Update();
			return enSaveResults::svSucceeded;
			break;
		}
		case enMode::AddNewMode: {
			if (clsBankClient::IsClientExist(_AccountNumber)) {
				return enSaveResults::svFaildAccountNumberExists;
			}
			else {
				_AddNew();
				_Mode = enMode::UpdateMode;
				return enSaveResults::svSucceeded;
			}
		}
	}
}

	//++++++++
	static clsBankClient GetAddNewClientObject(string AccountNumber) {
		return clsBankClient(enMode::AddNewMode, "", "", "", "", AccountNumber, "", 0.0f);
	}

	//++++++++
	bool Delete() {
		vector <clsBankClient> _vClients;
		_vClients = _LoadClientsDataFromFile();

		for (clsBankClient& C : _vClients) {
			if (C.getAccountNumber() == _AccountNumber) {
				C._MarkForDeleted = true;
				break;
			}
		}
		_SaveClientsDataToFile(_vClients);
		*this = _GetEmptyClientObject();
		return true;
	}

	//++++++++
	static vector <clsBankClient> GetClientsList() {
		return _LoadClientsDataFromFile();
	}

	//++++++++
	static float GetTotalBalances() {
		vector <clsBankClient> vClients = clsBankClient::GetClientsList();
		double TotalBalances = 0;

		for (clsBankClient Client : vClients) {
			TotalBalances += Client.AccountBalance;
		}
		return TotalBalances;
	}

	//++++++++
	void Deposit(double Amount)
	{
		_AccountBalance += Amount;
		Save();
	}

	//++++++++
	bool Withdraw(double Amount)
	{
		if (Amount > _AccountBalance)
		{
			return false;
		}
		else
		{
			_AccountBalance -= Amount;
			Save();
			return true;
		}

	}

	//++++++++
	bool Transfer(float Amount, clsBankClient& DestinationClient,string UserName) {
		if (Amount > AccountBalance){
			return false;
		}

		Withdraw(Amount);
		DestinationClient.Deposit(Amount);
		_RegisterTransferLog(Amount, DestinationClient, UserName);
		return true;
	}

	//++++++++
	static  vector <stTrnsferLogRecord> GetTransfersLogList() {
		vector <stTrnsferLogRecord> vTransferLogRecord;

		fstream MyFile;
		MyFile.open("TransfersLog.txt", ios::in);//read Mode
		if (MyFile.is_open()){
			string Line;
			stTrnsferLogRecord TransferRecord;

			while (getline(MyFile, Line)){
				TransferRecord = _ConvertTransferLogLineToRecord(Line);
				vTransferLogRecord.push_back(TransferRecord);
			}
			MyFile.close();
		}
		return vTransferLogRecord;
	}

	//++++++++
};


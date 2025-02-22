#pragma once
#include <iostream>
#include "InterfaceCommunication.h"
using namespace std;

class clsPerson : public InterfaceCommunication {
	string _FirstName;
	string _LastName;
	string _Email;
	string _PhoneNumber;

public:
	clsPerson(string FirstName, string LastName, string Email, string PhoneNumber) {
		_FirstName = FirstName;
		_LastName = LastName;
		_Email = Email;
		_PhoneNumber = PhoneNumber;
	}

	string getFirstName() {
		return _FirstName;
	}

	void setFirstName(string FirstName) {
		_FirstName = FirstName;
	}

	string getLastName() {
		return _LastName;
	}

	void setLastName(string LastName) {
		_LastName = LastName;
	}

	string getEmail() {
		return _Email;
	}

	void setEmail(string Email) {
		_Email = Email;
	}

	string getPhoneNumber() {
		return _PhoneNumber;
	}

	void setPhoneNumber(string PhoneNumber) {
		_PhoneNumber = PhoneNumber;
	}

	//=========
	void SendEmail(string Title, string Body)
	{

	}
	void SendFax(string Title, string Body)
	{

	}
	void SendSMS(string Title, string Body)
	{


	}

	//=========
	__declspec(property(get = getFirstName, put = setFirstName)) string FirstName;
	__declspec(property(get = getLastName, put = setLastName)) string LastName;
	__declspec(property(get = getEmail, put = setEmail)) string Email;
	__declspec(property(get = getPhoneNumber, put = setPhoneNumber)) string PhoneNumber;

};


#pragma once
#include <iostream>

using namespace std;
struct Garbage
{
private:
	streampos address;
	Garbage* Next;
public:

	Garbage() {
		this->address = -1;
		this->Next = NULL;
	}

	Garbage(streampos pos) {
		this->address = pos;
		this->Next = NULL;
	}

	Garbage(streampos pos, Garbage* next) {
		this->address = pos;
		this->Next = next;
	}

	void setAddress(streampos pos) {
		this->address = pos;
	}

	streampos getAddress() {
		return this->address;
	}

	void setNext(Garbage* temp) {
		this->Next = temp;
	}

	Garbage* getNext() {
		return this->Next;
	}
};

bool checkEmpty(Garbage* head) {
	if (head->getAddress() != -1)
		return true;
	else
		return false;
}

void AddNewGarbageAddress(Garbage** head, streampos address) {
	if (address < 0) {
		cerr << "Invalid address. Error adding to garbage" << endl;
		return;
	}

	Garbage* temp = new Garbage(address);
	if ((*head)->getAddress() == -1) {
		*head = temp;
		return;
	}

	temp->setNext(*head);
	*head = temp;
}

streampos takeAddress(Garbage** head) {
	
	Garbage* temp = *head;
	if (!temp or (temp->getAddress() == -1)) {
		return -1;
	}
		
	streampos address = (temp)->getAddress();
	*head = temp->getNext();

	delete temp;
	return address;	
}

void writeGarbage(Garbage** head, string filename) {
	ofstream outFile(filename);

	if (!outFile) {
		cout << "Error opening index file!" << endl;
		return;
	}

	Garbage* temp = *head;
	while (temp) {
		//cout << temp->getAudienceNumber() << " " << temp->getAudienceLink() << endl;
		outFile << temp->getAddress() << " " << endl;
		temp = temp->getNext();
	}

	outFile.close();
}

void printGarbage(Garbage* head) {
	cout << "head: ";
	while (head) {
		cout << head->getAddress() << endl;
		head = head->getNext();
	}
}
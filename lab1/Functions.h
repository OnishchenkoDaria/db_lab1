#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Audience.h"
#include "StudentNode.h"
#include "Utilities.h"
#include "indexNode.h"
#include "Garbage.h"

indexNode* indexHead = new indexNode;
indexNode* studentsHead = new indexNode;

Garbage* audienceGarbage = new Garbage(-1);
Garbage* studentsGarbage = new Garbage(-1);

void readAllIndexTable() {
	ifstream inFile("index.txt");

	if (!inFile) {
		cout << "Error opening index file!" << endl;
		return;
	}

	while (!inFile.eof()) {
		indexNode indx = readIndexFromFile(inFile);

		if ((indx.getAudienceLink() != -1) or (indx.getAudienceNumber() != 0)) {
			addNewIndex(indx.getAudienceNumber(), indx.getAudienceLink(), &indexHead);
		}
	}

	sortIndexTable(&indexHead);
}

void readStudentGarbage() {
	ifstream file("studentGarbage.txt");

	if (!file.is_open()) {
		cerr << "error opening file: " << "studentGarbage.txt" << endl;
		return;
	}

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		int position;
		ss >> position;
		streampos adr = position;
		AddNewGarbageAddress(&studentsGarbage, adr);
	}

	file.close();
	//printGarbage(studentsGarbage);
}

void readAudienceGarbage() {
	ifstream file("audienceGarbage.txt");

	if (!file.is_open()) {
		cerr << "error opening file: " << "audienceGarbage.txt" << endl;
		return;
	}

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		int position;
		ss >> position;
		streampos adr = position;
		AddNewGarbageAddress(&audienceGarbage, adr);
	}

	file.close();
	//printGarbage(audienceGarbage);
}

void readAllStudentTable() {
	ifstream inFile("studentTable.txt");

	if (!inFile) {
		cout << "Error opening index file!" << endl;
		return;
	}

	while (!inFile.eof()) {
		indexNode indx = readIndexFromFile(inFile);

		if ((indx.getAudienceLink() != -1) or (indx.getAudienceNumber() != 0)) {
			addNewIndex(indx.getAudienceNumber(), indx.getAudienceLink(), &studentsHead);
		}
	}

	sortIndexTable(&studentsHead);
}

void showMasterFile(){
	ifstream inFile("audience.txt");

	if (!inFile) {
		cout << "Error opening audience file!" << endl;
		return;
	}
	cout << endl;
	while (!inFile.eof()) {
		readAudFromFile(inFile);
	}
	cout << endl;
}

void showSlave() {
	ifstream inFile("students.txt");

	if (!inFile) {
		cout << "Error opening audience file!" << endl;
		return;
	}

	cout << endl;
	while (!inFile.eof()) {
		readStudentFromFile(inFile);
	}
	cout << endl;
}

void AddNewAudience() {
	Audience aud;
	aud.createObj(indexHead);
	if (aud.getNumber() == -1) {
		cerr << "The item with such Id already exist!" << endl;
		return;
	}
	
	streampos freeAddress;
	
	if (checkEmpty(audienceGarbage) == false) {
		freeAddress = takeAddress(&audienceGarbage);
		replaceTheLineiInFile(freeAddress, aud.TransformObjDataToLine(), "audience.txt");
		writeGarbage(&audienceGarbage, "audienceGarbage.txt");
	} 
	else {
		freeAddress = writeAudienceToFile(aud);
	}

	if (freeAddress != -1) {
		indexNode obj(aud.getNumber(), freeAddress);
		addNewIndex(obj.getAudienceNumber(), obj.getAudienceLink(), &indexHead);
		writeNewIndexRecord(obj, "index.txt");
	}
	//added else block w/o testing - can cause problems
	else {
		return;
	}
	sortIndexTable(&indexHead);
	writeSortedIndexTable(&indexHead, "index.txt");
}

int AskForId() {
	int id;
	cout << "Insert id: ";
	cin >> id;
	return id;
}

Audience findAudience(int id) {
	streampos pos = findById(id, indexHead);
	if (pos != -1) {
		string line = readLineFromPosition(pos, "audience.txt");
		return createAudfromLine(line);
	}
	else {
		cerr << "No Audience with such Number recorded earlier" << endl;
		Audience Empty;
		Empty.setNumber(-1);
		return Empty;
	}
}

void AddNewStudent() {
	StudentNode stud;
	stud.createObj(studentsHead);
	if (stud.getId() == -1) {
		cerr << "The item with such Id already exist!" << endl;
		return;
	}
	// id check (modernise the function for the master)

	streampos found = findKey(stud.getAudience(), indexHead);
	Audience foundAud = findAudience(stud.getAudience());
	if (foundAud.getNumber() == -1) {
		return;
	}

	//StudentNode head = createNode(stud, foundAud);
	if (foundAud.getStudentSubList() != -1) {
		stud.setNext(foundAud.getStudentSubList());
		//update the student attribute in audience master file
	}

	streampos freeAddress;

	if (checkEmpty(studentsGarbage) == false) {
		freeAddress = takeAddress(&studentsGarbage);
		replaceTheLineiInFile(freeAddress, stud.TransformObjDataToLine(), "students.txt");
		writeGarbage(&studentsGarbage, "studentGarbage.txt");
	}
	else {
		freeAddress = writeStudentToFile(stud);
	}
	//streampos NewStudentsHeadPos = writeStudentToFile(stud);
	
	if (freeAddress != -1) {
		indexNode obj(stud.getId(), freeAddress);
		//cout << "new student index object: "; obj.printNode();
		addNewIndex(obj.getAudienceNumber(), obj.getAudienceLink(), &studentsHead);
		writeNewIndexRecord(obj, "studentTable.txt");
	}
	//added else block w/o testing - can cause problems
	else {
		return;
	}

	sortIndexTable(&studentsHead);
	writeSortedIndexTable(&studentsHead, "studentTable.txt");

	foundAud.setStudentCount(foundAud.getStudentCount() + 1);
	foundAud.setStudentSubList(freeAddress);

	string replacement = foundAud.TransformObjDataToLine();
	replaceTheLineiInFile(found, replacement, "audience.txt");
}

StudentNode findStudent(int id) {
	streampos pos = findById(id, studentsHead);
	if (pos != -1) {
		string line = readLineFromPosition(pos, "students.txt");
		return createStudfromLine(line);
	}
	else {
		cerr << "No Student with such Number recorded earlier" << endl;
		return StudentNode();
	}
}

Audience getAudience() {
	return findAudience(AskForId());
}

StudentNode getStudent() {
	return findStudent(AskForId());
}

void EditAudience(Audience& aud) {
	char answer;
	cout << "Press 'F' to change the floor data" << endl
		<< "Press 'T' to change the type data" << endl
		<< "Press 'U' to change the university data" << endl
		<< "Press 'A' to change the faculty data" << endl;

	int newFloor{};
	string newString;
	streampos found;

	cin >> answer;
	switch (answer)
	{
	case 'F':
		cout << "Input new value: "; cin >> newFloor;
		aud.setFloor(newFloor);
		break;
	case 'T':
		cout << "Input new value: "; cin >> newString;
		aud.setType(newString);
		break;
	case 'U':
		cout << "Input new value: "; cin >> newString;
		aud.setUniversity(newString);
		break;
	case 'A':
		cout << "Input new value: "; cin >> newString;
		aud.setFaculty(newString);
		break;
	}
	newString = aud.TransformObjDataToLine();
	found = findKey(aud.getNumber(), indexHead);
	replaceTheLineiInFile(found, newString, "audience.txt");
}

void EditStudent(StudentNode& stud) {
	char answer;
	cout << "Press 'N' to change the name data" << endl
		<< "Press 'B' to change the birthday data" << endl
		<< "Press 'G' to change the gender data" << endl
		<< "Press 'J' to change the group data" << endl;

	int newAudience{};
	string newString;
	char newGender{};
	streampos found;

	cin >> answer;
	switch (answer)
	{
	case 'N':
		cout << "Input new value: "; cin >> newString;
		stud.setName(newString);
		break;
	case 'B':
		cout << "Input new value: "; cin >> newString;
		stud.setDate(newString);
		break;
	case 'G':
		cout << "Input new value: "; cin >> newGender;
		stud.setGender(newGender);
		break;
	case 'J':
		cout << "Input new value: "; cin >> newString;
		stud.setGroup(newString);
		break;
	}
	newString = stud.TransformObjDataToLine();
	found = findKey(stud.getId(), studentsHead);
	replaceTheLineiInFile(found, newString, "students.txt");
}

void printIndexList() {
	showAllList(indexHead);
}

int MasterCount() {
	return Count(indexHead);
}

int SlaveCount() {
	return Count(studentsHead);
}

int CountStudentsInAudience() {
	streampos pos = findById(AskForId(), indexHead);
	string line = readLineFromPosition(pos, "audience.txt");
	streampos StudentPos = createAudfromLine(line).getStudentSubList();

	int count = 0;
	
	while (StudentPos != -1) {
		string studentLine = readLineFromPosition(StudentPos, "students.txt");
		StudentNode Student = createStudfromLine(studentLine);
		count++;
		StudentPos = Student.getNextStudent();
	}
	
	return count;
}

void removeStudent(int id) {
	StudentNode stud = findStudent(id);
	streampos studAddress;
	if (stud.getId() == -1) {
		return;
	}

	Audience aud = findAudience(stud.getAudience());
	streampos audPos = findById(aud.getNumber(), indexHead);
	streampos address = aud.getStudentSubList();
	string line = readLineFromPosition(address, "students.txt");
	StudentNode currStud = createStudfromLine(line);

	if (stud.getId() == currStud.getId()) {
		studAddress = aud.getStudentSubList();
		aud.setStudentSubList(currStud.getNextStudent());
	}
	else {
		StudentNode prevStud;
		while (currStud.getId() != stud.getId()) {
			prevStud = currStud;
			string nextStudLine = readLineFromPosition(currStud.getNextStudent(), "students.txt");
			currStud = createStudfromLine(nextStudLine);
		}

		studAddress = prevStud.getNextStudent();
		
		if (currStud.getNextStudent()) {
			prevStud.setNextStudent(currStud.getNextStudent());
		}
		else {
			prevStud.setNextStudent(-1);
		}
		streampos prevAdrs = findById(prevStud.getId(), studentsHead);
		replaceTheLineiInFile(prevAdrs, prevStud.TransformObjDataToLine(), "students.txt");

	}
	
	deleteNode(stud.getId(), &studentsHead);
	
	aud.setStudentCount(aud.getStudentCount() - 1);
	stud.changeVisibility();
	stud.setId(-1);
	
	AddNewGarbageAddress(&studentsGarbage, studAddress);

	replaceTheLineiInFile(audPos, aud.TransformObjDataToLine(), "audience.txt");
	replaceTheLineiInFile(studAddress, stud.TransformObjDataToLine(), "students.txt");
	writeSortedIndexTable(&studentsHead, "studentTable.txt");
	writeGarbage(&studentsGarbage, "studentGarbage.txt");
}

void removeAudience() {
	Audience aud = getAudience();
	if (aud.getNumber() == -1) {
		return;
	}

	streampos address = findById(aud.getNumber(), indexHead);
	streampos firststudent = aud.getStudentSubList();

	while (firststudent != -1) {
		string studentLine = readLineFromPosition(firststudent, "students.txt");
		StudentNode Student = createStudfromLine(studentLine);
		removeStudent(Student.getId());
		firststudent = Student.getNextStudent();
	}	
	
	deleteNode(aud.getNumber(), &indexHead);
	AddNewGarbageAddress(&audienceGarbage, address);
	writeSortedIndexTable(&indexHead, "index.txt");
	writeGarbage(&audienceGarbage, "audienceGarbage.txt");

	aud.changeVisibility();
	aud.setNumber(-1);
	aud.setStudentCount(0);
	replaceTheLineiInFile(address, aud.TransformObjDataToLine(), "audience.txt");
}

void isEmty() {
	cout << "is empty audience: " << checkEmpty(audienceGarbage) << endl;
	cout << "is empty students: " << checkEmpty(studentsGarbage) << endl;
}

void writeGarbage() {
	writeGarbage(&audienceGarbage, "audienceGarbage.txt");
	writeGarbage(&audienceGarbage, "audienceGarbage.txt");
}
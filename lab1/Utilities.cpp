#include "Utilities.h"

void readAudFromFile(ifstream& file) {
	string line;
	if (getline(file, line)) {
		istringstream iss(line);
		int num, floor, position, count;
		string type, uni, faculty;
		bool visib;

		if (iss >> num >> floor >> type >> uni >> faculty >> visib >> position >> count) {
			streampos pos = position;
			cout << " Audience Number: " << num << " | "
				<< " Floor: " << floor << " | "
				<< " Type: " << type << " | "
				<< " University: " << uni << " | "
				<< " Faculty: " << faculty << " | "
				<< " Visibility: " << visib << " | "
				<< " Students List head: " << pos << " | "
				<< " Overall Students: " << count << endl;
			return;
		}
		else {
			cout << "Invalid text found in audiences file" << endl;
			return;
		}
	}
}

string readLineFromPosition(streampos startPos, string fileName) {
	ifstream inFile(fileName);

	if (!inFile) {
		cerr << "Error opening audience file!" << endl;
		return "";
	}

	string line;
	inFile.seekg(startPos);
	// get to the position in file
	getline(inFile, line); // read the line from the position
	//cout << line << endl;
	return line;
}

StudentNode createStudfromLine(string line) {
	//cout << "LINE: " << line << endl;
	istringstream iss(line);
	int id, audience, position;
	string name, date, group;
	char gender;
	bool visib;
	streampos pos;

	if (iss >> id >> name >> date >> gender >> group >> audience >> visib >> position) {
		/*cout << " Student Id: " << id << " | "
			<< " Name: " << name << " | "
			<< " BirthDate: " << date << " | "
			<< " Gender: " << gender << " | "
			<< " Group: " << group << " | "
			<< " Audience: " << audience << " | " << endl;*/
		pos = position;
		return StudentNode(id, name, date, gender, group, audience, visib);
	}
	else {
		return StudentNode();
	}
}

void readStudentFromFile(ifstream& file) {
	string line;
	if (getline(file, line)) {
		istringstream iss(line);
		int id, audience, position;
		string name, date, group;
		char gender;
		bool visib;

		if (iss >> id >> name >> date >> gender >> group >> audience >> visib >> position) {
			cout << " Student Id: " << id << " | "
				<< " Name: " << name << " | "
				<< " Birth date: " << date << " | "
				<< " Gender: " << gender << " | "
				<< " Group: " << group << " | "
				<< " Audience: " << audience << " | "
				<< " Visibility: " << visib << " | "
				<< " Next Student: " << position << endl;
			return;
		}
		else {
			cout << "Invalid text found in audiences file" << endl;
			return;
		}
	}
}

indexNode readIndexFromFile(ifstream& file) {
	string line;
	if (getline(file, line)) {
		istringstream iss(line);

		int num;
		int address;
		if (iss >> num >> address) {
			cout << "num: " << num << " address: " << address << endl;
			streampos Adrs = address;
			return indexNode(num, Adrs);
		}
		else {
			cout << "Invalid text found in index file" << endl;
			return indexNode();
		}
	}
	else {
		//cout << "Failed to read the line in index file" << endl;
		return indexNode();
	}
}

streampos writeAudienceToFile(Audience obj) {
	ofstream outFile("audience.txt", ios::app);

	if (!outFile) {
		cout << "Error opening audience file!" << endl;
		return -1;
	}
	outFile << " ";
	streampos startPos = outFile.tellp();
	//cout << startPos << endl;
	outFile << obj.getNumber() << " "
		<< obj.getFloor() << " "
		<< obj.getType() << " "
		<< obj.getUniversity() << " "
		<< obj.getFaculty() << " "
		<< obj.getVisibility() << " "
		<< obj.getStudentSubList() << " "
		<< obj.getStudentCount() << endl;

	//outFile.close();
	return startPos;
}

void writeNewIndexRecord(indexNode obj) {
	ofstream outFile("index.txt", ios::app);

	if (!outFile) {
		cout << "Error opening index file!" << endl;
		return;
	}

	outFile << obj.getAudienceNumber() << " " << obj.getAudienceLink() << endl;
	outFile.close();
}

streampos writeStudentToFile(StudentNode obj) {
	ofstream outFile("students.txt", ios::app);

	if (!outFile) {
		cout << "Error opening students file!" << endl;
		return -1;
	}
	outFile << " ";
	streampos startPos = outFile.tellp();
	cout << "POSITION IN STUDENT FILE: " << startPos << endl;
	outFile << obj.getId() << " "
		<< obj.getName() << " "
		<< obj.getDate() << " "
		<< obj.getGender() << " "
		<< obj.getGroup() << " "
		<< obj.getAudience() << " "
		<< obj.getVisibility() << " "
		<< obj.getNextStudent() << endl;

	return startPos;
	//outFile.close();
}

Audience createAudfromLine(string line) {
	istringstream iss(line);
	int num, floor, position, count;
	string type, uni, facult;
	bool visib;
	streampos studLink;
	if (iss >> num >> floor >> type >> uni >> facult >> visib >> position >> count) {
		/*cout << "AUDIENCE READ:  num " << num << " floor " << floor
			<< " type: " << type << " uni " << uni
			<< " facult " << facult << " vis " << visib
			<< " stud link: " << position << " stud count: " << count << endl;*/
		cout << endl << " Audience Number: " << num << " | "
			<< " Floor: " << floor << " | "
			<< " Type: " << type << " | "
			<< " University: " << uni << " | "
			<< " Faculty: " << facult << " | "
			<< " Overall Students: " << count << endl << endl;
		studLink = position;
		return Audience(num, floor, type, uni, facult, visib, studLink, count);
	}
	else {
		cout << "Invalid text found" << endl;
		return Audience();
	}
}

int streamposToInt(std::streampos pos) {
	return static_cast<int>(pos);
}

void replaceTheLineiInFile(streampos position, const string& newString) {
	//ifstream inFile("audience.txt");
	//ofstream outFile("temp.txt", ios::app);
	fstream file("audience.txt", ios::in | ios::out);
	if (!file.is_open()) {
		cerr << "Error: Failed to open audience file." << std::endl;
		return;
	}
	file.seekg(position);
	string line;
	getline(file, line);
	const char* charrArr = line.c_str();

	int pos = streamposToInt(position);
	for (int i = 0; i < line.size(); i++) {
		file.seekg(pos + i);
		file.put(' ');
	}

	file.seekg(position);
	file << newString;

	//check
	cout << "check after replacement: ";
	file.seekg(position);
	string line1;
	getline(file, line1);
	cout << line1 << endl;

	file.close();
}
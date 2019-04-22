#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

//          <! Initializing 4 structures for writing data from CSV files !>

//     1. Structure for auditories
struct lectureHalls {
	lectureHalls() : busy(false){}
	string specialization;
	string auditory;
	string seats;
	bool busy;
};

//     2. Structure for groups
struct groups {
	groups() : busy(false){}
	string groupName;
	string specialization;
	string seats;
	string period;
	bool busy = false;
};

//     3. Structure for schedule
struct schedule {
	string namedate;
	string period;
};

//     4. Structure for distributed groups
struct ready {
	string auditory;
	string groupName;
	string period;
	string specialization;
};

int main() {
	//     <! Initializing 2 variables for searching amount of lines !>
	int amountOfLines = 0;
	int writeIndex = 0;

	//     <! Buffer for lines !>
	char buffer[1024];

	//     <! Dynamic arrays with int type for auditory numbers and amount of seat in groups !>
	int *auditoryNumber;
	int *seatGroups;

	//    <! Amount of lines in CSV files !>
	int amountOfHalls;
	int amountOfGroups;
	int amountOfSchedule;

	//    <! Variables for sort !>
	int minDifferenceOfClasses = INT_MAX;
	int IndexForReadyGroup;
	int IndexForReadyAuditory;
	int CountOfReadyClasses = 0;

	//    <! Amount of of text for JSON !>
	int textIndex = 0;

	//    <! Dynamic structure arrays !>
	lectureHalls *lectureList;
	groups *groupList;
	schedule *scheduleList;
	ready *readyList;

	//    <! Opening files !>
	ifstream lectureCSV("LectureHall.csv");
	ifstream groupCSV("Groups.csv");
	ifstream scheduleCSV("Schedule.csv");

	//    <! Checking for correct opened files !>
	if (!lectureCSV || !groupCSV || !scheduleCSV) {
		cout << "An error occured while opening file(s)." << endl;
		system("pause");
		return 1;
	}
	//    <! Searching amount of lines !>
	while (lectureCSV.good()) {
		lectureCSV.getline(buffer, 1024, '\n');
		amountOfLines++;
	}
	amountOfLines--;
	amountOfHalls = amountOfLines;

	//    <! Initialize size of dynamic arrays !>
	lectureList = new lectureHalls[amountOfHalls];
	auditoryNumber = new int[amountOfHalls];
	lectureCSV.clear();
	lectureCSV.seekg(0);

	//    <! Write in dynamic arrays !>
	while (writeIndex != amountOfLines) {
		getline(lectureCSV, lectureList[writeIndex].specialization, ';');
		getline(lectureCSV, lectureList[writeIndex].auditory, ';');
		auditoryNumber[writeIndex] = stoi(lectureList[writeIndex].auditory);
		getline(lectureCSV, lectureList[writeIndex].seats, '\n');
		writeIndex++;
	}

	writeIndex = 0;
	amountOfLines = 0;

	//    <! Searching amount of lines !>
	while (groupCSV.good()) {
		groupCSV.getline(buffer, 1024, '\n');
		amountOfLines++;
	}
	amountOfLines--;
	amountOfGroups = amountOfLines;

	//    <! Initialize size of dynamic arrays !>
	groupList = new groups[amountOfGroups];
	readyList = new ready[amountOfGroups];
	seatGroups = new int[amountOfGroups];
	groupCSV.clear();
	groupCSV.seekg(0);

	//    <! Write in dynamic arrays !>
	while (writeIndex != amountOfLines) {
		getline(groupCSV, groupList[writeIndex].groupName, ';');
		getline(groupCSV, groupList[writeIndex].specialization, ';');
		getline(groupCSV, groupList[writeIndex].seats, ';');
		seatGroups[writeIndex] = stoi(groupList[writeIndex].seats);
		getline(groupCSV, groupList[writeIndex].period, '\n');
		writeIndex++;
	}

	writeIndex = 0;
	amountOfLines = 0;

	//    <! Searching amount of lines !>
	while (scheduleCSV.good()) {
		scheduleCSV.getline(buffer, 1024, '\n');
		amountOfLines++;
	}
	amountOfLines--;
	amountOfSchedule = amountOfLines;

	//    <! Initialize size of dynamic arrays !>
	scheduleList = new schedule[amountOfSchedule];
	scheduleCSV.clear();
	scheduleCSV.seekg(0);

	//    <! Write in dynamic arrays !>
	while (writeIndex != amountOfLines) {
		getline(scheduleCSV, scheduleList[writeIndex].namedate, ';');
		getline(scheduleCSV, scheduleList[writeIndex].period, '\n');
		writeIndex++;
	}

	writeIndex = 0;
	amountOfLines = 0;

	//    <! Sort auditories by ascending !>
	for (int i = 0; i < amountOfHalls; i++) {
		for (int j = 0; j < amountOfHalls; j++) {
			if (auditoryNumber[j] > auditoryNumber[i]) {
				swap(auditoryNumber[j], auditoryNumber[i]);
				swap(lectureList[j].auditory, lectureList[i].auditory);
				swap(lectureList[j].specialization, lectureList[i].specialization);
				swap(lectureList[j].seats, lectureList[i].seats);
			}
		}
	}

	//    <! Sort groups by seats by ascending !>
	for (int i = 0; i < amountOfGroups; i++) {
		for (int j = 0; j < amountOfGroups; j++) {
			if (seatGroups[j] < seatGroups[i]) {
				swap(seatGroups[j], seatGroups[i]);
				swap(groupList[j].groupName, groupList[i].groupName);
				swap(groupList[j].specialization, groupList[i].specialization);
				swap(groupList[j].seats, groupList[i].seats);
				swap(groupList[j].period, groupList[i].period);
			}
		}
	}

	//    <! Finding groups and writing them with a specialized auditories !>
	for (int i = 0; i < amountOfGroups; i++) {
		bool flagForReadyClass = false;
		minDifferenceOfClasses = INT_MAX;
		for (int j = 0; j < amountOfHalls; j++) {
			for (int z = 0; z < amountOfGroups; z++) {
				if (lectureList[j].busy == false && lectureList[j].specialization == groupList[z].specialization && groupList[z].busy == false) {
					int differenceOfClasses = stoi(lectureList[j].seats) - stoi(groupList[z].seats);
					if (differenceOfClasses < minDifferenceOfClasses && differenceOfClasses >= 0) {
						minDifferenceOfClasses = differenceOfClasses;
						IndexForReadyGroup = z;
						IndexForReadyAuditory = j;
						flagForReadyClass = true;
					}
				}
			}
		}
		if (flagForReadyClass == true) {
			lectureList[IndexForReadyAuditory].busy = true;
			groupList[IndexForReadyGroup].busy = true;
			readyList[CountOfReadyClasses].groupName = groupList[IndexForReadyGroup].groupName;
			readyList[CountOfReadyClasses].specialization = groupList[IndexForReadyGroup].specialization;
			readyList[CountOfReadyClasses].auditory = lectureList[IndexForReadyAuditory].auditory;
			for (int s = 0; s < amountOfSchedule; s++) {
				if (groupList[IndexForReadyGroup].period == scheduleList[s].period) {
					readyList[CountOfReadyClasses].period = scheduleList[s].namedate;
					break;
				}
			}
			CountOfReadyClasses++;
		}
	}

	//    <! Finding groups and writing them with a common auditories !>
	for (int i = 0; i < amountOfGroups; i++) {
		bool flagForReadyClass = false;
		minDifferenceOfClasses = INT_MAX;
		for (int j = 0; j < amountOfHalls; j++) {
			for (int z = 0; z < amountOfGroups; z++) {
				if (lectureList[j].specialization == "Common" && lectureList[j].busy == false && groupList[z].busy == false) {
					int differenceOfClasses = stoi(lectureList[j].seats) - stoi(groupList[z].seats);
					if (differenceOfClasses < minDifferenceOfClasses && differenceOfClasses >= 0) {
						minDifferenceOfClasses = differenceOfClasses;
						IndexForReadyGroup = z;
						IndexForReadyAuditory = j;
						flagForReadyClass = true;
					}
				}
			}
		}
		if (flagForReadyClass == true) {
			lectureList[IndexForReadyAuditory].busy = true;
			groupList[IndexForReadyGroup].busy = true;
			readyList[CountOfReadyClasses].groupName = groupList[IndexForReadyGroup].groupName;
			readyList[CountOfReadyClasses].specialization = groupList[IndexForReadyGroup].specialization;
			readyList[CountOfReadyClasses].auditory = lectureList[IndexForReadyAuditory].auditory;
			for (int s = 0; s < amountOfSchedule; s++) {
				if (groupList[IndexForReadyGroup].period == scheduleList[s].period) {
					readyList[CountOfReadyClasses].period = scheduleList[s].namedate;
					break;
				}
			}
			CountOfReadyClasses++;
		}
	}

	//    <! Output in console !>
	cout << setw(25) << "Group name" << setw(25) << "Specialization" << setw(25) << "Auditory" << setw(25) << "Name of period" << endl;
	for (int i = 0; i < amountOfGroups; i++) {
		if (readyList[i].groupName != "") {
			cout << "______________________________________________________________________________________________________" << endl;
			cout << setw(25) << readyList[i].groupName << setw(25) << readyList[i].specialization << setw(25) << readyList[i].auditory << setw(25) << readyList[i].period << endl;
		}
	}
	cout << "______________________________________________________________________________________________________" << endl;

	//    <! Creating file for JSON output !>
	ofstream outputJSON("Output.json");

	//    <! JSON text !>
	string *textJSON;

	//     <! Static write(Possible only with already known amount of text) !>
	/*for (int i = 0; i < amountOfSchedule; i++) {
		textJSON[textIndex] = "\"";
		textIndex++;
		textJSON[textIndex] = schlist[i].namedate;
		textIndex++;
		textJSON[textIndex] = "\"";
		textIndex++;
		textJSON[textIndex] = ": [";
		textIndex++;
		for (int j = 0; j < amountOfGroups; j++) {
			if (readyList[j].period == schlist[i].namedate) {
				textJSON[textIndex] = "\"";
				textIndex++;
				textJSON[textIndex] = readyList[j].auditory;
				textIndex++;
				textJSON[textIndex] = "\"";
				textIndex++;
				textJSON[textIndex] = ": ";
				textIndex++;
				textJSON[textIndex] = "\"";
				textIndex++;
				textJSON[textIndex] = readyList[j].groupName;
				textIndex++;
				textJSON[textIndex] = "\"";
				textIndex++;
				textJSON[textIndex] = ",";
				textIndex++;
			}
		}
		textJSON[textIndex - 1] = "]";
		textJSON[textIndex] = ",";
		textIndex++;
	}
	textJSON[textIndex - 1] = "}";*/

	//    <! Searching for amount of text !>
	for (int i = 0; i < amountOfSchedule; i++) {
		textIndex++;
		textIndex++;
		textIndex++;
		textIndex++;
		for (int j = 0; j < amountOfGroups; j++) {
			if (readyList[j].period == scheduleList[i].namedate) {
				textIndex++;
				textIndex++;
				textIndex++;
				textIndex++;
				textIndex++;
				textIndex++;
				textIndex++;
				textIndex++;
			}
		}
		textIndex++;
	}

	//    <! Initializing amount of text array !>
	textJSON = new string[textIndex + 1];
	textIndex = 1;
	textJSON[0] = "{";

	//    <! Writing data in text !>
	for (int i = 0; i < amountOfSchedule; i++) {
		textJSON[textIndex] = "\"";
		textIndex++;
		textJSON[textIndex] = scheduleList[i].namedate;
		textIndex++;
		textJSON[textIndex] = "\"";
		textIndex++;
		textJSON[textIndex] = ": [";
		textIndex++;
		for (int j = 0; j < amountOfGroups; j++) {
			if (readyList[j].period == scheduleList[i].namedate) {
				textJSON[textIndex] = "\"";
				textIndex++;
				textJSON[textIndex] = readyList[j].auditory;
				textIndex++;
				textJSON[textIndex] = "\"";
				textIndex++;
				textJSON[textIndex] = ": ";
				textIndex++;
				textJSON[textIndex] = "\"";
				textIndex++;
				textJSON[textIndex] = readyList[j].groupName;
				textIndex++;
				textJSON[textIndex] = "\"";
				textIndex++;
				textJSON[textIndex] = ",";
				textIndex++;
			}
		}
		textJSON[textIndex - 1] = "]";
		textJSON[textIndex] = ",";
		textIndex++;
	}
	textJSON[textIndex - 1] = "}";

	//    <! Writing text in file !>
	for (int i = 0; i < textIndex; i++) {
		outputJSON << textJSON[i];
	}

	system("pause");
	//    <! Memory clearing !>
	delete[] textJSON;
	delete[] lectureList;
	delete[] groupList;
	delete[] scheduleList;
	delete[] readyList;
}

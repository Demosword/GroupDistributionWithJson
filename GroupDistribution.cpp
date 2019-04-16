#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

//          <! ���������� 4-�� �������� ��� ������ ������ � CSV ����� !>

//     1. ��������� ��� ������ ���������
struct lectureHalls {
	string specialization;
	string auditory;
	string seats;
	bool busy = false;
};

//     2. ��������� ��� ������ �����
struct groups {
	string groupName;
	string specialization;
	string seats;
	string period;
	bool busy = false;
};

//     3. ��������� ��� ������ ����������
struct schedule {
	string namedate;
	string period;
};

//     4. ��������� ��� �������������� �������
struct ready {
	string auditory;
	string groupName;
	string period;
	string specialization;
};

int main() {
	//     <! ���������� 2-�� ���������� ��� ������ ���������� ����� � ������ � ��������� !>
	int amountOfLines = 0;
	int writeIndex = 0;

	//     <! ������ ��� ������ ���������� ����� !>
	char buffer[1024];

	//     <! ������������ ������� ���� int ��� ������ ������ ��������� � ���������� ���� � ������ !>
	int *auditoryNumber;
	int *seatGroups;

	//    <! ���������� ����� � ������ ����� !>
	int amountOfHalls;
	int amountOfGroups;
	int amountOfSchedule;

	//    <! ���������� ��� ���������� ���������� !>
	int minDifferenceOfClasses = INT_MAX;
	int IndexForReadyGroup;
	int IndexForReadyAuditory;
	int CountOfReadyClasses = 0;

	//    <! ���������� ������ ��� JSON !>
	int textIndex = 0;

	//    <! ������������ ������� �������� !>
	lectureHalls *lectureList;
	groups *groupList;
	schedule *scheduleList;
	ready *readyList;

	//    <! �������� ������ !>
	ifstream lectureCSV("LectureHall.csv");
	ifstream groupCSV("Groups.csv");
	ifstream scheduleCSV("Schedule.csv");

	//    <! �������� �� ���������� �������� ������ !>
	if (!lectureCSV || !groupCSV || !scheduleCSV) {
		cout << "An error occured while opening file(s)." << endl;
		system("pause");
		return 1;
	}
	//    <! ����� ���������� ����� !>
	while (lectureCSV.good()) {
		lectureCSV.getline(buffer, 1024, '\n');
		amountOfLines++;
	}
	amountOfLines--;
	amountOfHalls = amountOfLines;

	//    <! ���������� ������� ������������ �������� !>
	lectureList = new lectureHalls[amountOfHalls];
	auditoryNumber = new int[amountOfHalls];
	lectureCSV.clear();
	lectureCSV.seekg(0);

	//    <! ������ � ������������ ������� !>
	while (writeIndex != amountOfLines) {
		getline(lectureCSV, lectureList[writeIndex].specialization, ';');
		getline(lectureCSV, lectureList[writeIndex].auditory, ';');
		auditoryNumber[writeIndex] = stoi(lectureList[writeIndex].auditory);
		getline(lectureCSV, lectureList[writeIndex].seats, '\n');
		writeIndex++;
	}

	writeIndex = 0;
	amountOfLines = 0;

	//    <! ����� ���������� ����� !>
	while (groupCSV.good()) {
		groupCSV.getline(buffer, 1024, '\n');
		amountOfLines++;
	}
	amountOfLines--;
	amountOfGroups = amountOfLines;

	//    <! ���������� ������� ������������ �������� !>
	groupList = new groups[amountOfGroups];
	readyList = new ready[amountOfGroups];
	seatGroups = new int[amountOfGroups];
	groupCSV.clear();
	groupCSV.seekg(0);

	//    <! ������ � ������������ ������� !>
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

	//    <! ����� ���������� ����� !>
	while (scheduleCSV.good()) {
		scheduleCSV.getline(buffer, 1024, '\n');
		amountOfLines++;
	}
	amountOfLines--;
	amountOfSchedule = amountOfLines;

	//    <! ���������� ������� ������������ �������� !>
	scheduleList = new schedule[amountOfSchedule];
	scheduleCSV.clear();
	scheduleCSV.seekg(0);

	//    <! ������ � ������������ ������� !>
	while (writeIndex != amountOfLines) {
		getline(scheduleCSV, scheduleList[writeIndex].namedate, ';');
		getline(scheduleCSV, scheduleList[writeIndex].period, '\n');
		writeIndex++;
	}

	writeIndex = 0;
	amountOfLines = 0;

	//    <! ���������� ��������� �� ����������� !>
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

	//    <! ���������� ����� �� ������ �� ����������� !>
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

	//    <! ���������� � ������ ����� �� ������������������ ���������� !>
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

	//    <! ���������� � ������ ����� �� ������� ���������� !>
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

	//    <! ����� � ������� !>
	cout << setw(25) << "Group name" << setw(25) << "Specialization" << setw(25) << "Auditory" << setw(25) << "Name of period" << endl;
	for (int i = 0; i < amountOfGroups; i++) {
		if (readyList[i].groupName != "") {
			cout << "______________________________________________________________________________________________________" << endl;
			cout << setw(25) << readyList[i].groupName << setw(25) << readyList[i].specialization << setw(25) << readyList[i].auditory << setw(25) << readyList[i].period << endl;
		}
	}
	cout << "______________________________________________________________________________________________________" << endl;

	//    <! �������� ����� ��� ������ JSON ������ !>
	ofstream outputJSON("Output.json");

	//    <! JSON ����� !>
	string *textJSON;

	//     <! ����������� ������(�������� ���� ��� ��� ��������� ���������� �������� ������) !>
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

	//    <! ���������� ���������� ������ !>
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

	//    <! ���������� ������� ������ !>
	textJSON = new string[textIndex + 1];
	textIndex = 1;
	textJSON[0] = "{";

	//    <! ������ ������ � ����� !>
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

	//    <! ������ ������ � ���� !>
	for (int i = 0; i < textIndex; i++) {
		outputJSON << textJSON[i];
	}

	system("pause");
	//    <! �������� ������ !>
	delete[] textJSON;
	delete[] lectureList;
	delete[] groupList;
	delete[] scheduleList;
	delete[] readyList;
}
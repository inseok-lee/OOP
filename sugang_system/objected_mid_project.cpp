#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Utils
int findStudentById(vector<string> studentIds, string id);
int findLectureByCode(vector<string> lectureCodes, string code);
void deleteElement(vector<string>& v, int index);

// File read
void readStudentFile(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes);
void readLectureFile(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits);

// File write
void writeStudentFile(const vector<string>& studentIds, const vector<string>& passwords, const vector<string>& names, const vector<int>& credits, const vector<vector<string>>& appliedLectureCodes);
void writeLectureFile(const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, const vector<int>& limits);

// Get user input
string getInputId();
string getInputPassword();

// Login
int studentLogin(const vector<string>& studentIds, const vector<string>& passwords);
bool adminLogin();
int login(const vector<string>& studentIds, const vector<string>& passwords);

// Common
void printLectures(const vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, const vector<int>& limits, const int& user = -100);

// Admin
void addStudent(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes);
void addLecture(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits);
void deleteLecture(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits, vector<int>& credits, vector<vector<string>>& appliedLectureCodes);
int runAdmin(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits);

// User
void printStudent(const vector<string>& studentIds, const vector<string>& names, const vector<int>& credits, const int& user);
void applyLecture(const vector<string>& studentIds, const vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, vector<int>& limits, const int& user);
void disapplyLecture(const vector<string>& studentIds, const vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, vector<int>& limits, const int& user);
void changePassword(vector<string>& passwords, const int& user);
int runStudent(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits, int user);

int main() {
	int user = -1; //user index
	int status = -1;

	// Student Info
	vector<string> studentIds;
	vector<string> passwords;
	vector<string> names;
	vector<int> credits;
	vector<vector<string>> appliedLectureCodes;

	// Lecture Info
	vector<string> lectureCodes;
	vector<string> lectureNames;
	vector<int> lectureCredits;
	vector<int> limits;

	// Read from files
	readStudentFile(studentIds, passwords, names, credits, appliedLectureCodes);
	readLectureFile(lectureCodes, lectureNames, lectureCredits, limits);

	// Login phase
	while (status == -1) {
		user = login(studentIds, passwords);

		if (user == -999) { // Login fail
			break;
		}
		else if (user == -1) { // Exit command
			break;
		}
		else if (user == -100) { // Admin login success
			status = runAdmin(studentIds, passwords, names, credits, appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits);
		}
		else { // Student login success
			status = runStudent(studentIds, passwords, names, credits, appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits, user);
		}
	}

	// Write to files
	writeStudentFile(studentIds, passwords, names, credits, appliedLectureCodes);
	writeLectureFile(lectureCodes, lectureNames, lectureCredits, limits);

	return 0;
}

int findStudentById(vector<string> studentIds, string id) {
	for (unsigned i = 0; i < studentIds.size(); i++) {
		if (studentIds[i] == id)
			return i;
	}
	return -1;
}

int findLectureByCode(vector<string> lectureCodes, string code) {
	for (unsigned i = 0; i < lectureCodes.size(); i++) {
		if (lectureCodes[i] == code)
			return i;
	}
	return -1;
}

void deleteElement(vector<string>& v, int index) {
	v.erase(v.begin() + index);
}

void readStudentFile(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes) {
	/* ���� */
	ifstream fin;
	string s;
	fin.open("students.txt");
	if (!fin) {
		cout << "File open error" << endl;
		exit(100);
	}
	fin >> s;
	studentIds.push_back(s);
	fin >> s;
	passwords.push_back(s);
	fin >> s;
	names.push_back(s);
	fin >> s;
	credits.push_back(stoi(s));
	fin >> s;
	appliedLectureCodes[0].push_back(s);
	fin >> s;
	appliedLectureCodes[1].push_back(s);

	while (true) {
		fin >> s;
		if (!fin) {
			break;
		}
		studentIds.push_back(s);
		fin >> s;
		passwords.push_back(s);
		fin >> s;
		names.push_back(s);
		fin >> s;
		credits.push_back(stoi(s));
		fin >> s;
		appliedLectureCodes[0].push_back(s);
	}
}

void readLectureFile(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits) {
	/* ���� */
	ifstream fin;
	string s;
	fin.open("lectures.txt");
	if (!fin) {
		cout << "File open error" << endl;
		exit(100);
	}
	while (true) {
		fin >> s;
		if (!fin) {
			break;
		}
		lectureCodes.push_back(s);
		fin >> s;
		lectureNames.push_back(s);
		fin >> s;
		lectureCredits.push_back(stoi(s));
		fin >> s;
		limits.push_back(stoi(s));
	}
}

void writeStudentFile(const vector<string>& studentIds, const vector<string>& passwords, const vector<string>& names, const vector<int>& credits, const vector<vector<string>>& appliedLectureCodes) {
	/* ���� */

}

void writeLectureFile(const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, const vector<int>& limits) {
	/* ���� */
}

string getInputId() {
	/* ���� */
	string studentId;
	cout << "���̵�: ";
	cin >> studentId;
	return studentId;
}

string getInputPassword() {
	/* ���� */
	int password;
	cout << "��й�ȣ: ";
	cin >> password;
	return to_string(password);
}

int studentLogin(const vector<string>& studentIds, const vector<string>& passwords) {
	/* ���� */
	/* ���� ȣ�� �Լ�: getInputId(), getInputPassword()*/
	for (int n = 0; n < 3; n++) {
		string id = getInputId();
		if (findStudentById(studentIds, id) != -1) {
			string pw = getInputPassword();
			if (pw == passwords[findStudentById(studentIds, id)]) {
				return findStudentById(studentIds, id);
			}
			else {
				cout << "�α��� " << n + 1 << "ȸ ���� (3ȸ ���н� ����)" << endl;
			}
		}
		else {
			n--;
		}
	}
	cout << "3ȸ �����Ͽ� �����մϴ�." << endl;
	return -1;
}

bool adminLogin() {
	/* ���� */
	/* ���� ȣ�� �Լ�: getInputId(), getInputPassword()*/
	for (int n = 0; n < 3; n++) {
		string id = getInputId();
		if (id == "admin") {
			string pw = getInputPassword();
			if (pw == "admin") {
				return true;
			}
			else {
				cout << "�α��� " << n + 1 << "ȸ ���� (3ȸ ���н� ����)" << endl;
			}
		}
		else {
			n--;
		}
	}
	cout << "3ȸ �����Ͽ� �����մϴ�." << endl;
	return false;

}

int login(const vector<string>& studentIds, const vector<string>& passwords) {
	/* ���� */
	/* ���� ȣ�� �Լ�: studentLogin(), adminLogin()*/
	int num;
	cout << '_' * 100 << endl;
	cout << "1. �л� �α���" << endl;
	cout << "2. ������ �α���" << endl;
	cout << "0. ����" << endl;
	cout << '_' * 100 << endl;
	cin >> num;
	if (num == 1) {
		studentLogin(studentIds, passwords);
	}
	else if (num == 2) {
		bool adLogin = adminLogin();
		if (adLogin == true) {
			return -100;
		}
		else {
			return -999;
		}
	}
	else if (num == 0) {
		cout << "���α׷��� �����մϴ�." << endl;
		exit(100);
	}

}

void printLectures(const vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, const vector<int>& limits, const int& user) {
	/* ���� */
	cout << '_' * 100 << endl;
	cout << "�����ڵ�\t���Ǹ�\t����\t���������ο�" << endl;
	cout << '_' * 100 << endl;
	if (user == -100) {// ������ ���� ��� ���
		for (int i = 0; i < 4; i++) {
			cout << lectureCodes[i] << ' ' << lectureNames[i] << '\t' << lectureCredits[i] << limits[i] << endl;
		}
		cout << '_' * 100 << endl;
	}
	else {
		for(string elem : appliedLectureCodes[1])//����ڰ� ��û�� �������� ��� ���???
			cout << 
	}
}

void addStudent(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes) {
	/* ���� */
}

void addLecture(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits) {
	/* ���� */
}

void deleteLecture(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits, vector<int>& credits, vector<vector<string>>& appliedLectureCodes) {
	/* ���� */
	/* ���� ȣ�� �Լ�: printLectures*/
}

int runAdmin(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits) {
	/* ���� */
	/* ���� ȣ�� �Լ�: addStudent, addLecture, deleteLecture*/
}

void printStudent(const vector<string>& studentIds, const vector<string>& names, const vector<int>& credits, const int& user) {
	/* ���� */
	cout << '_' * 100 << endl;
	cout << "�й�: " << studentIds[user] << "\t�̸�: " << names[user] << "\t������������: " << credits[user] << endl;
	cout << '_' * 100 << endl;
}

void applyLecture(const vector<string>& studentIds, const vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, vector<int>& limits, const int& user) {
	/* ���� */
	/* ���� ȣ�� �Լ�: printStudent, printLectures*/
	printStudent(studentIds, names, credits, user);
	printLectures(appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits, user);
	string code;
	cout << "��û�� ���� �ڵ�(0: �ڷΰ���)";
	cin >> code;
	int i = findLectureByCode(lectureCodes, code);
	if()


	}
	
}

void disapplyLecture(const vector<string>& studentIds, const vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, vector<int>& limits, const int& user) {
	/* ���� */
	/* ���� ȣ�� �Լ�: printStudent, printLectures*/
}

void changePassword(vector<string>& passwords, const int& user) {
	/* ���� */
}

int runStudent(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits, int user) {
	/* ���� */
	/* ���� ȣ�� �Լ�: applyLecture, printStudent, printLectures, disapplyLecture, changePassword*/
}
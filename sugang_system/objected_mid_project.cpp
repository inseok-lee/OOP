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
	/* 구현 */
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
	/* 구현 */
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
	/* 구현 */

}

void writeLectureFile(const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, const vector<int>& limits) {
	/* 구현 */
}

string getInputId() {
	/* 구현 */
	string studentId;
	cout << "아이디: ";
	cin >> studentId;
	return studentId;
}

string getInputPassword() {
	/* 구현 */
	int password;
	cout << "비밀번호: ";
	cin >> password;
	return to_string(password);
}

int studentLogin(const vector<string>& studentIds, const vector<string>& passwords) {
	/* 구현 */
	/* 내부 호출 함수: getInputId(), getInputPassword()*/
	for (int n = 0; n < 3; n++) {
		string id = getInputId();
		if (findStudentById(studentIds, id) != -1) {
			string pw = getInputPassword();
			if (pw == passwords[findStudentById(studentIds, id)]) {
				return findStudentById(studentIds, id);
			}
			else {
				cout << "로그인 " << n + 1 << "회 실패 (3회 실패시 종료)" << endl;
			}
		}
		else {
			n--;
		}
	}
	cout << "3회 실패하여 종료합니다." << endl;
	return -1;
}

bool adminLogin() {
	/* 구현 */
	/* 내부 호출 함수: getInputId(), getInputPassword()*/
	for (int n = 0; n < 3; n++) {
		string id = getInputId();
		if (id == "admin") {
			string pw = getInputPassword();
			if (pw == "admin") {
				return true;
			}
			else {
				cout << "로그인 " << n + 1 << "회 실패 (3회 실패시 종료)" << endl;
			}
		}
		else {
			n--;
		}
	}
	cout << "3회 실패하여 종료합니다." << endl;
	return false;

}

int login(const vector<string>& studentIds, const vector<string>& passwords) {
	/* 구현 */
	/* 내부 호출 함수: studentLogin(), adminLogin()*/
	int num;
	cout << '_' * 100 << endl;
	cout << "1. 학생 로그인" << endl;
	cout << "2. 관리자 로그인" << endl;
	cout << "0. 종료" << endl;
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
		cout << "프로그램을 종료합니다." << endl;
		exit(100);
	}

}

void printLectures(const vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, const vector<int>& limits, const int& user) {
	/* 구현 */
	cout << '_' * 100 << endl;
	cout << "과목코드\t강의명\t학점\t수강가능인원" << endl;
	cout << '_' * 100 << endl;
	if (user == -100) {// 개설된 강의 목록 출력
		for (int i = 0; i < 4; i++) {
			cout << lectureCodes[i] << ' ' << lectureNames[i] << '\t' << lectureCredits[i] << limits[i] << endl;
		}
		cout << '_' * 100 << endl;
	}
	else {
		for(string elem : appliedLectureCodes[1])//사용자가 신청한 수강과목 목록 출력???
			cout << 
	}
}

void addStudent(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes) {
	/* 구현 */
}

void addLecture(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits) {
	/* 구현 */
}

void deleteLecture(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits, vector<int>& credits, vector<vector<string>>& appliedLectureCodes) {
	/* 구현 */
	/* 내부 호출 함수: printLectures*/
}

int runAdmin(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits) {
	/* 구현 */
	/* 내부 호출 함수: addStudent, addLecture, deleteLecture*/
}

void printStudent(const vector<string>& studentIds, const vector<string>& names, const vector<int>& credits, const int& user) {
	/* 구현 */
	cout << '_' * 100 << endl;
	cout << "학번: " << studentIds[user] << "\t이름: " << names[user] << "\t수강가능학점: " << credits[user] << endl;
	cout << '_' * 100 << endl;
}

void applyLecture(const vector<string>& studentIds, const vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, vector<int>& limits, const int& user) {
	/* 구현 */
	/* 내부 호출 함수: printStudent, printLectures*/
	printStudent(studentIds, names, credits, user);
	printLectures(appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits, user);
	string code;
	cout << "신청할 과목 코드(0: 뒤로가기)";
	cin >> code;
	int i = findLectureByCode(lectureCodes, code);
	if()


	}
	
}

void disapplyLecture(const vector<string>& studentIds, const vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, vector<int>& limits, const int& user) {
	/* 구현 */
	/* 내부 호출 함수: printStudent, printLectures*/
}

void changePassword(vector<string>& passwords, const int& user) {
	/* 구현 */
}

int runStudent(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits, int user) {
	/* 구현 */
	/* 내부 호출 함수: applyLecture, printStudent, printLectures, disapplyLecture, changePassword*/
}
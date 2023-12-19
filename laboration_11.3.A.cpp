#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <iomanip>
using namespace std;

enum Specialnist { Computer_Science, Philology, Information_Technology };
string specStr[] = { "Computer_Science", "Philology", "Information_Technology" };

struct Student
{
	char prizv[60];
	int course;
	Specialnist specialnist;
	double physics_grade;
	double math_grade;
	double informatics_grade;
};

void Create(string filename);
void Print(string filename);
void BubbleSort(string filename);
int BinSearch(string filename, const string prizv, const Specialnist specialnist, const double grade);
int* Index_Sort(string filename);
void PrintIndexSorted(string filename);

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);


	string filename;
	cout << "Input name of file: "; cin >> filename;
	char prizv[60];
	int ispecialnist;
	double grade;
	Specialnist specialnist;
	int found;

	int menuItem;
	do {
		cout << endl << endl << endl;
		cout << "Choose an extion: " << endl << endl;
		cout << " [1] - entering data from the keyboard" << endl;
		cout << " [2] - print data in display" << endl;
		cout << " [3] - sort" << endl;
		cout << " [4] - index sort" << endl;
		cout << " [5] - bin search" << endl;
		cout << " [0] - Exit" << endl;
		cout << "Input value: "; cin >> menuItem;
		cout << endl << endl << endl;

		switch (menuItem)
		{
		case 1:
			Create(filename);
			break;
		case 2:
			Print(filename);
			break;
		case 3:
			BubbleSort(filename);
			Print(filename);
			break;
		case 4:
			PrintIndexSorted(filename);
			break;
		case 5:
			cout << "Input lastname of student: "; cin >> prizv;
			cout << "Input keys of search: " << endl;
			cout << endl;
			cout << "Specialnost (0 - Computer_Science, 1 - Philology, 2 -  Information_Technology): ";
			cin >> ispecialnist;

			specialnist = (Specialnist)ispecialnist;

			cout << "Input third grade: " << endl;
			cin >> grade;
			cout << endl;
			if ((found = BinSearch(filename, prizv, specialnist, grade)) != -1)
				cout << "Student was founded " << found + 1 << endl;
			else
				cout << "Student wasn't founded " << endl;
			break;
		case 0:
			break;
		default:
			cout << "You input mistake value!" << endl;
		}
	} while (menuItem != 0);

	return 0;
}

void Create(string filename) {
	ofstream f(filename, ios::binary);
	Student s;
	char ch;
	int spec, i = 0;

	do {
		i++;
		cin.get();
		cout << "#" << i << ":" << endl;
		cout << "LastName: "; cin.getline(s.prizv, 60);
		cout << "Course: "; cin >> s.course;
		cout << "Specialnost: (0 - Computer_Science, 1 - Philology, 2 - Information_Technology): "; cin >> spec;
		s.specialnist = (Specialnist)spec;
		cout << "Math_Grade: "; cin >> s.math_grade;
		cout << "Phisics_Grade: "; cin >> s.physics_grade;
		cout << "Informatics_Grade: "; cin >> s.informatics_grade;
		cin.get();

		f.write((char*)&s, sizeof(Student));
		cout << "Continue? (Y/N): "; cin >> ch; cout << endl;
	} while (ch == 'Y' || ch == 'y');
	f.close();
	cout << endl;
}

void Print(string filename)
{
	ifstream f(filename, ios::binary);
	Student s;
	int i = 0;
	cout << "======================================================================="
		<< endl;
	cout << "| # | Last Name | Course | Specialnist | Phisics | Math | Informatics |"
		<< endl;
	cout << "-----------------------------------------------------------------------"
		<< endl;
	while (f.read((char*)&s, sizeof(Student))) {
		i++;
		cout << "| " << i << " | " << setw(10) << left << s.prizv << " | " << setw(4) << left << s.course << " | " << setw(13) << left << specStr[s.specialnist];
		cout << " | " << setw(10) << left << s.math_grade << " | " << setw(6) << left << s.physics_grade << " | " << setw(13) << left << s.informatics_grade << " | ";
		cout << endl;
		cout << "-----------------------------------------------------------------------" << endl;
	}
	cout << "======================================================================="
		<< endl;
	f.close();
}

void BubbleSort(string filename) {
	ifstream inFile(filename, ios::binary);
	if (!inFile.is_open()) {
		cout << "Error opening the file for reading." << endl;
		return;
	}

	const int maxStudents = 100; 
	Student students[maxStudents];
	int N = 0;

	while (inFile.read(reinterpret_cast<char*>(&students[N]), sizeof(Student))) {
		N++;
		if (N >= maxStudents) {
			cout << "Maximum number of students reached. Cannot read more data." << endl;
			break;
		}
	}

	inFile.close();

	for (int i0 = 0; i0 < N - 1; i0++) {
		for (int i1 = 0; i1 < N - i0 - 1; i1++) {
			double grade1, grade2;

			grade1 = students[i1].math_grade;
			grade2 = students[i1 + 1].math_grade;

			if (grade1 < grade2) {
				swap(students[i1], students[i1 + 1]);
			}
			else if (grade1 == grade2) {
				if (students[i1].specialnist < students[i1 + 1].specialnist) {
					swap(students[i1], students[i1 + 1]);
				}
				else if (students[i1].specialnist == students[i1 + 1].specialnist && students[i1].prizv < students[i1+1].prizv) {
					swap(students[i1], students[i1 + 1]);
				}
			}
		}
	}

	ofstream outFile(filename, ios::binary);
	if (!outFile.is_open()) {
		cout << "Error opening the file for writing." << endl;
		return;
	}

	outFile.write(reinterpret_cast<const char*>(students), N * sizeof(Student));

	outFile.close();
}



int BinSearch(string filename, const string prizv, const Specialnist specialnist, const double grade) {
	ifstream inFile(filename, ios::binary);
	if (!inFile.is_open()) {
		cout << "Error opening the file for reading." << endl;
		return -1;
	}

	inFile.seekg(0, ios::end);
	int fileSize = inFile.tellg();
	inFile.seekg(0, ios::beg);

	int maxStudents = fileSize / sizeof(Student);
	Student student;

	int left = 0, right = maxStudents - 1;

	do {
		int mid = (right + left) / 2;
		inFile.seekg(mid * sizeof(Student), ios::beg);
		inFile.read(reinterpret_cast<char*>(&student), sizeof(Student));

		if (student.prizv == prizv && student.specialnist == specialnist && student.informatics_grade == grade) {
			inFile.close();
			return mid;
		}

		if (student.informatics_grade < grade) {
			left = mid + 1;
		}
		else if (student.informatics_grade == grade) {
			if (student.specialnist < specialnist) {
				left = mid + 1;
			}
			else if (student.specialnist == specialnist && student.prizv < prizv) {
				left = mid + 1;
			}
		}
		else
			right = mid - 1;
	} while (left <= right);

	inFile.close();
	return -1;
}

int* Index_Sort(string filename) {
	ifstream inFile(filename, ios::binary | ios::in);
	if (!inFile.is_open()) {
		cout << "Error opening the file for reading." << endl;
		return nullptr;
	}

	inFile.seekg(0, ios::end);
	int fileSize = inFile.tellg();
	int N = fileSize / sizeof(Student);
	inFile.seekg(0, ios::beg);

	Student* students = new Student[N];
	inFile.read(reinterpret_cast<char*>(students), fileSize);
	inFile.close();

	int* indexArray = new int[N];
	for (int i = 0; i < N; ++i) {
		indexArray[i] = i;
	}

	for (int i0 = 0; i0 < N - 1; i0++) {
		for (int i1 = 0; i1 < N - i0 - 1; i1++) {
			double grade1 = students[indexArray[i1]].informatics_grade;
			double grade2 = students[indexArray[i1 + 1]].informatics_grade;

			if (grade1 < grade2) {
				swap(indexArray[i1], indexArray[i1 + 1]);
			}
			else if (grade1 == grade2) {
				if (students[indexArray[i1]].specialnist > students[indexArray[i1 + 1]].specialnist) {
					swap(indexArray[i1], indexArray[i1 + 1]);
				}
				else if (students[indexArray[i1]].specialnist == students[indexArray[i1 + 1]].specialnist &&
					students[indexArray[i1]].prizv < students[indexArray[i1 + 1]].prizv) {
					swap(indexArray[i1], indexArray[i1 + 1]);
				}
			}
		}
	}

	delete[] students;
	return indexArray;
}

void PrintIndexSorted(string filename) {
	ifstream inFile(filename, ios::binary | ios::in);
	if (!inFile.is_open()) {
		cout << "Error opening the file for reading." << endl;
		return;
	}

	inFile.seekg(0, ios::end);
	int fileSize = inFile.tellg();
	inFile.seekg(0, ios::beg);

	int N = fileSize / sizeof(Student);
	Student* students = new Student[N];
	inFile.read(reinterpret_cast<char*>(students), fileSize);
	inFile.close();

	int* indexArray = Index_Sort(filename);

	cout << "==================================================================================================" << endl;
	cout << "| # | Last Name             | Course | Specialnist                | Phisics | Math | Informatics |" << endl;
	cout << "--------------------------------------------------------------------------------------------------" << endl;

	for (int i = 0; i < N; i++) {
		cout << "| " << setw(3) << right << i + 1 << " ";
		cout << "| " << setw(24) << left << students[indexArray[i]].prizv << " | " << left << students[indexArray[i]].course
			<< " | ";

		switch (students[indexArray[i]].specialnist) {
		case Computer_Science:
			cout << setw(27) << "Computer_Science";
			break;
		case Philology:
			cout << setw(27) << "Philology";
			break;
		case Information_Technology:
			cout << setw(27) << "Information_Technology";
			break;
		}

		cout << " | " << setw(6) << students[indexArray[i]].physics_grade << " | " << setw(10) << students[indexArray[i]].math_grade
			<< " | " << setw(11) << students[indexArray[i]].informatics_grade << " |" << endl;
	}

	cout << "==================================================================================================" << endl;

	delete[] students;
	delete[] indexArray;
}
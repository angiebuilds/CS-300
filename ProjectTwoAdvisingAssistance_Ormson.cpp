// ProjectTwoAdvisingAssistance_Ormson.cpp
// Angela Ormson
// CS 300 -- Professor Rubey
// 8/14/2026

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

//Store all information connected to one course from a single line of the input file
struct Course {
	string courseNumber;
	string courseTitle;
	vector<string> prerequisites;
};

//Each node keeps one course and links it into the binary search tree
struct TreeNode {
	Course course;
	TreeNode* left;
	TreeNode* right;

	TreeNode(Course newCourse) {
		course = newCourse;
		left = nullptr;
		right = nullptr;
	}
};

class BinarySearchTree {
private: TreeNode* root;
	   TreeNode* insertNode(TreeNode* node, Course course) {

		   // Reaching an empty branch means we found the course's correct location instead of needing to rearrange anything already stored
		   if (node == nullptr) {
			   return new TreeNode(course);
		   }
		   if (course.courseNumber < node->course.courseNumber) {
			   node->left = insertNode(node->left, course);
		   }
		   else {
			   node->right = insertNode(node->right, course);
		   }
		   return node;
	   }

	   TreeNode* searchNode(TreeNode* node, const string& courseNumber) const {

		   //Following only one branch at each comparison keeps us from searching parts of the tree that cannot contain the requested course
		   while (node != nullptr) {
			   if (courseNumber == node->course.courseNumber) {
				   return node;
			   }
			   else if (courseNumber < node->course.courseNumber) {
				   node = node->left;
			   }
			   else {
				   node = node->right;
			   }
		   }
		   return nullptr;
	   }

	   void inOrder(TreeNode* node) const {
		   if (node == nullptr) {
			   return;
		   }

		   //Visiting the left branch, then the current node, then the right branch ensures that the courses are printed in ascending order by course number
		   inOrder(node->left);
		   cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
		   inOrder(node->right);
	   }

	   void destroyTree(TreeNode* node) {
		   
		   if (node == nullptr) {
			   return;
		   }

		   //The children are deleted before the parent so every dynamically allocated node is deleted without leaving dangling pointers
		   
		   destroyTree(node->left);
		   destroyTree(node->right);
		   
		   delete node;
		   
	   }

public:
	BinarySearchTree() {
		root = nullptr;
	}

	~BinarySearchTree() {
		destroyTree(root);
	}

	void insert(Course course) {
		root = insertNode(root, course);
	}

	Course* search(const string& courseNumber) const {
		TreeNode* result = searchNode(root, courseNumber);
		if (result == nullptr) {
			return nullptr;
		}
		return &result->course;
	}

	void printCourseList() const {
		inOrder(root);
	}

	void clear() {
		destroyTree(root);
		root = nullptr;
	}
};

//Removes extra spaces that could otherwise make a valid course number fail comparison just because of the formatting in the input file
string trim(const string& text) {
	size_t start = text.find_first_not_of(" \t\n\r");

	if (start == string::npos) {
		return "";
	}

	size_t end = text.find_last_not_of(" \t\n\r");

	return text.substr(start, end - start + 1);
}

//Course numbers are stored consistently so later searches can accept input such as "csci400" without requiring the adviser to match case
string toUpperCase(string text) {
	transform(text.begin(), text.end(), text.begin(), [](unsigned char character) {
		return toupper(character);
	});

	return text;
}

//Breaks one CSV line into the individual values needed to build a Course
vector<string> splitLine(const string& line) {
	vector<string> fields;
	string field;
	stringstream stream(line);

	while (getline(stream, field, ',')) {
		fields.push_back(trim(field));
	}
	return fields;
}

bool loadCourseFile(const string& filename, BinarySearchTree& courses) {
	ifstream file(filename);

	if (!file.is_open()) {
		cout << "Error: File could not be opened." << endl;
		return false;
	}

	vector<Course> pendingCourses;
	vector<string> courseNumbers;

	string line;

	while (getline(file, line)) {
		vector<string> fields = splitLine(line);
		if (fields.size() < 2) {
			cout << "Error: Each line must contain a course number and title." << endl;

			file.close();
			return false;
		}

		Course course;
		course.courseNumber = toUpperCase(fields[0]);
		course.courseTitle = fields[1];

		for (size_t i = 2; i < fields.size(); ++i) {
			if (!fields[i].empty()) {
				course.prerequisites.push_back(toUpperCase(fields[i]));
			}
		}

		courseNumbers.push_back(course.courseNumber);
		pendingCourses.push_back(course);
	}

	file.close();

	//Prerequisites are checked after all courses have been read to ensure that the order of the input file does not affect the ability to validate prerequisites
	for (const Course& course : pendingCourses) {
		for (const string& prerequisite : course.prerequisites) {
			bool prerequisiteExists = false;
			for (const string& courseNumber : courseNumbers) {
				if (prerequisite == courseNumber) {
					prerequisiteExists = true;
					break;
				}
			}

			if (!prerequisiteExists) {
				cout << "Error: Prerequisite " << prerequisite << " does not exist as a course in the file." << endl;
				return false;
			}
		}
	}

	courses.clear();

	//Nothing enters the actual tree until the complete file passes validation, which avoids leaving partially loaded course data behind
	for (const Course& course : pendingCourses) {
		courses.insert(course);
	}

	cout << "Course data loaded successfully." << endl;

	return true;
}

int main() {
	BinarySearchTree courses;
	bool dataLoaded = false;
	int choice = 0;

	cout << "Welcome to the course planner." << endl;

	while (choice != 9) {

		cout << "\nMenu Options:" << endl;
		cout << "1. Load Data Structure" << endl;
		cout << "2. Print Course List" << endl;
		cout << "3. Print Course" << endl;
		cout << "9. Exit" << endl;
		cout << "What would you like to do?";

		cin >> choice;

		// Clearing bad input here keeps the menu usable if someone accidentally enters a letter or symbol instead of a number
		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');

			cout << "That is not a valid option." << endl;
			continue;
		}

		if (choice == 1) {
			string fileName;

			cout << "Enter the course data file name: ";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			getline(cin, fileName);

			if (loadCourseFile(fileName, courses)) {
				dataLoaded = true;
			}
		}
		else if (choice == 2) {

			//Printing before loading would produce an empty schedule that looks valid, so the program makes the required order of operations clear to the user
			if (!dataLoaded) {
				cout << "Please load the course data first." << endl;
			}
			else {
				cout << "Here is a sample schedule:" << endl;
				courses.printCourseList();
			}
		}
		else if (choice == 3) {

			if (!dataLoaded) {

				cout << "Please load the course data first." << endl;
			}
			else {
				string courseNumber;

				cout << "What course do you want to know about?";
				cin >> courseNumber;

				//Normalizing the input lets an advisor enter a course number in any case and still find the correct course
				Course* course = courses.search(toUpperCase(courseNumber));
				if (course == nullptr) {
					cout << "Course " << courseNumber << " not found." << endl;
				}
				else {
					cout << "Course Number: " << course->courseNumber << endl;
					cout << "Course Title: " << course->courseTitle << endl;

					if (course->prerequisites.empty()) {
						cout << "Prerequisites: None" << endl;
					}
					else {
						cout << "Prerequisites:" << endl;

						for (const string& prerequisite : course->prerequisites) {

							Course* prerequisiteCourse = courses.search(prerequisite);

							if (prerequisiteCourse != nullptr) {
								cout << prerequisiteCourse->courseNumber << ", " << prerequisiteCourse->courseTitle << endl;
							}
						}
					}
				}
			}
		}

		else if (choice == 9) {

			cout << "Thank you for using the course planner!" << endl;
		}
		else {
			cout << choice << " is not a valid option." << endl;
		}
	}

	return 0;
}
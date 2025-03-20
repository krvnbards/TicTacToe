#include <iostream>
#include <limits>

using namespace std;

int main() {
	
	int action;
	
	cout << "Enter number from 1-4: ";
	cin >> action;
	
	if(cin.fail() || action < 0 || action > 4) {
		cout << "You have entered an invalid input (character)!";
		return 1;
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	string name;
	cout << "Enter name: ";
	cin >> name;
	
	cout << "Action: " << action << endl << "Name: " << name;
	
	return 1;
}
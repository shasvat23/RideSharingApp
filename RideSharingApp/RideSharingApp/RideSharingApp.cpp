// RideSharingApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "RideSharing_Functionality.h"
#include <RideSharing_Functionality.cc>


using namespace std;

int main(int argc, char** argv)
{

	std::cout << "Welcome to not so uber app" << std::endl;
	int x, y;


	cout << "Enter the max streets on the grid in x and y direction :  ";
	cin >> x >> y;

	Route R(x, y);
	R.Start();
	while (1)
	{
		cout << "Type 'n' for next move, type 'q' to quit" << endl;
		char buf[100];
		char* ptr = fgets(buf, ARRAYSIZE(buf), stdin);
		if (!ptr)
			return 0;
		char cmd = *ptr;

		switch (cmd)
		{
		default:

			break;
		case 'n':
			R.Start();
			break;
		case 'q':
			return 0;
			break;
		}
	}


	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

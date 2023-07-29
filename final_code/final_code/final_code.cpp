#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#define INPUT_FILE "Using_time_input.txt"
#define OUTPUT_FILE "Energy_consumption.txt"
#define POWER_FILE "Power.txt"
#define NUM_FILE "num.txt"

using namespace std;

class appliance
{
public:
	double power;
	double timeInUse[31];
	double energy[31];
	double energySum;
	string name;

	appliance()
	{
		name = "Nothing";
		power = 0;
		for (int i = 0; i < 31; i++)
		{
			timeInUse[i] = 0;
			energy[i] = 0;
		}
		energySum = 0;
	}

	appliance(double p, string n)
	{
		power = p;
		name = n;
	}

	void calculateEnergy()
	{
		for (int i = 0; i < 31; i++)
		{
			energy[i] = power * timeInUse[i];
			energySum += energy[i];
		}
	}
};

void inputData(appliance& a)
{
	cout << "Name: ";
	getline(cin, a.name);
	cout << "Power(W): ";
	cin >> a.power;
	cin.ignore();
}


void writeDataToInputFileAndReadData(vector<appliance>& appliances, int& number)
{
	char choice;
	cout << "Press \"1\" to create a new system, press \"2\" to add an appliance, press another key to start calculating" << endl;
	cin >> choice;
	cin.ignore();

	if (choice == '1')//Create new system and input new appliances in the system
	{
		while (choice == '1')
		{
			appliance a;
			inputData(a);
			appliances.push_back(a);
			number++;
			cout << "Press \"1\" to create a new appliance, press another key if you are done" << endl;
			cin >> choice;
			cin.ignore();
		}
		//Write all the appliance names and powers, and number to files
		fstream f;
		f.open(POWER_FILE, ios::out);
		for (int i = 0; i < number; i++)
		{
			f << appliances[i].power << "\t";
		}
		f.close();

		//Write the number of appliances in the number file
		f.open(NUM_FILE, ios::out);
		f << number;
		f.close();

		//Write the appliances' names to the input file 
		f.open(INPUT_FILE, ios::out);
		f << "\t";
		for (int i = 0; i < number; i++)
		{
			f << appliances[i].name << "\t";
		}
		f.close();

		//Write the number in the input file and set all the default data to 0
		f.open(INPUT_FILE, ios::app);
		f << endl;
		for (int i = 1; i <= 31; i++)
		{
			f << i<<"\t";
			for (int j = 0; j < number; j++)
			{
				f << "0\t";
			}
			f << endl;
		}
		f.close();

		cout << "You need to input some data" << endl;
	}
	else if (choice == '2')//add appliance
	{
		string powerList;
		string line[32];
		int previousNum;

		//get the number from number file to variable 'number'
		fstream f;
		f.open(NUM_FILE, ios::in);
		if (!f.is_open())
		{
			cout << "File not found" << endl;
			return;
		}
		f >> number;
		f.close();

        //get the number from number file to variable 'previousNum'(to use later)
		f.open(NUM_FILE, ios::in);
		if (!f.is_open())
		{
			cout << "File not found" << endl;
			return;
		}
		f >> previousNum;
		f.close();

		//add new appliance
		while (choice == '2')
		{
			appliance a;
			inputData(a);
			appliances.push_back(a);
			number++;
			cout << "Press \"2\" to add another appliance, press another key if you are done" << endl;
			cin >> choice;
			cin.ignore();
		}

		//get the power from the power filee
		f.open(NUM_FILE, ios::out);
		f << number;
		f.close();
		appliances.resize(number);
		f.open(POWER_FILE, ios::in);
		if (!f.is_open())
		{
			cout << "File not found" << endl;
			return;
		}
		getline(f, powerList);
		f.close();

		//Write the old power list to the power file and add new powers
		f.open(POWER_FILE, ios::out);
		f << powerList;
		for (int i = 0; i < number - previousNum; i++)
		{
			f << appliances[i].power << "\t";
		}
		f.close();

		f.open(INPUT_FILE, ios::in);
		if (!f.is_open())
		{
			cout << "File not found" << endl;
			return;
		}

		//take the data from the input file before modifying it
		getline(f, line[0]);
		for (int i = 1; i <= 31; i++)
		{
			getline(f, line[i]);
		}
		f.close();

		//put new data on the input file
		f.open(INPUT_FILE, ios::out);
		f << line[0];
		for (int i = 0; i < number - previousNum; i++)
		{
			f << appliances[i].name << "\t";
		}
		f << endl;
		for (int i = 1; i <= 31; i++)
		{
			f << line[i];
			for (int j = previousNum; j < number; j++)
			{
				f << "0\t";
			}
			f << endl;
		}
		f.close();
	}
	else
	{
		//Read data from files
		int k = 0;
		string line;
		string powerLine;
		string nameList;
		string numStr;
		fstream f;

		//get number from number file
		f.open(NUM_FILE, ios::in);
		f >> number;
		f.close();

		//get the using time from input file
		f.open(INPUT_FILE, ios::in);
		getline(f, line);
		for (int day = 0; day < 31; day++)
		{
			getline(f, line);
			stringstream ss(line);//Use to extract the string and assign to respective variables
			string using_time;
			getline(ss, using_time, '\t');
			for (int i = 0; i < number; i++)
			{
				getline(ss, using_time, '\t');
				if (appliances.size() <= i) {
					appliance a;
					a.timeInUse[day] = stod(using_time);//Convert string to double
					appliances.push_back(a);
				}
				else {
					appliances[i].timeInUse[day] = stod(using_time);
				}
			}
		}//Get the string from the file, extract it and assign values to each appliance
		f.close();

		f.open(POWER_FILE, ios::in);
		getline(f, powerLine);
		stringstream s(powerLine);//extract the string and assign to respective variables
		string power;
		for (int i = 0; i < number; i++)
		{
			getline(s, power, '\t');
			appliances[i].power = stod(power);//Convert string to double
		}//Get the saved power from the file and assign them to respective appliances
		f.close();

		for (int i = 0; i < number; i++)
		{
			appliances[i].calculateEnergy();
		}

		f.open(INPUT_FILE, ios::in);
		getline(f, nameList);//get the name list from input file

		//extract the names in nameList and assign to the name of each appliance
		stringstream sss(nameList);
		string names;
		getline(sss, names, '\t');
		for (int i = 0; i < number; i++)
		{
			getline(sss, names, '\t');
			appliances[i].name = names;
		}
		f.close();
	}
}

string findMostConsumedAppliance(vector<appliance>& appliances, int number)
{
	double maxEnergy = appliances[0].energySum;
	string mostConsumedAppliance = appliances[0].name;

	for (int i = 1; i < number; i++)
	{
		if (appliances[i].energySum > maxEnergy)
		{
			maxEnergy = appliances[i].energySum;
			mostConsumedAppliance = appliances[i].name;
		}
	}

	return mostConsumedAppliance;
}

void findMostConsumedDays(vector<appliance>& appliances, int number, vector<vector<int>>& mostConsumedDays)
{
	//2 dimensions: 1st dimension: Indicate the appliance; 2nd dimension: Indicate most consumed days of the appliance
	mostConsumedDays.resize(number);
	for (int i = 0; i < number; i++)
	{
		double maxEnergy = appliances[i].energy[0];
		for (int day = 1; day < 31; day++)
		{
			if (appliances[i].energy[day] > maxEnergy)
			{
				maxEnergy = appliances[i].energy[day];
			}
		}
		for (int day = 0; day < 31; day++)
		{
			if (appliances[i].energy[day] == maxEnergy)
			{
				mostConsumedDays[i].push_back(day + 1);//add "day + 1" to the vector mostConsumedDays of the ith appliance
			}
		}
	}
}

void writeResultsToFile(vector<appliance>& appliances, int number, vector<vector<int>>& mostConsumedDays)
{
	fstream f;
	//Get the name list from the input file
	f.open(INPUT_FILE, ios::in);
	string nameList;
	getline(f, nameList);
	f.close();
	
	//Put the name list on the output file
	f.open(OUTPUT_FILE, ios::out);
	f << nameList;
	f.close();

	//Put the next lines on the output file
	f.open(OUTPUT_FILE, ios::app);
	f << endl;
	for (int day = 0; day < 31; day++)
	{
		f << day + 1 << "\t";
		for (int i = 0; i < number; ++i)
		{
			f << appliances[i].energy[day] << "\t";
		}
		f << endl;
	}

	f << "Sum\t";
	for (int i = 0; i < number; i++)
	{
		f << appliances[i].energySum << "\t";
	}
	f << endl;
	string mostConsumedAppliance = findMostConsumedAppliance(appliances, number);
	f << endl << "The most consumed appliance: " << mostConsumedAppliance << endl;
	for (int i = 0; i < number; i++)//i: order of the device
	{
		f << endl << "The day " << appliances[i].name << " consumed most energy is: ";
		for (int  j = 0; j < mostConsumedDays[i].size(); j++)
		{
			f << "Day " << mostConsumedDays[i][j];
			if (j < mostConsumedDays[i].size() - 1)
				f << ", ";
		}
		f << "\t";
	}
	f << endl;

	f.close();
}

int main()
{
	vector<appliance> appliances;
	int num = 0;

	writeDataToInputFileAndReadData(appliances, num);

	vector<vector<int>> mostConsumedDays;
	findMostConsumedDays(appliances, num, mostConsumedDays);

	writeResultsToFile(appliances, num, mostConsumedDays);

	return 0;
}

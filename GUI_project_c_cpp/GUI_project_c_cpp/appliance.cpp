#include "stdafx.h"
#include "appliance.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#define INPUT_FILE "Using_time_input.txt"
#define OUTPUT_FILE "Energy_consumption.txt"
#define POWER_FILE "Power.txt"
#define NUM_FILE "num.txt"

using namespace std;

appliance app_set[100];
int num_app;
//int previousNum;


appliance::appliance(appliance& _a)
{
	power = _a.power;
	for (int i = 0; i < 31; i++)
	{
		timeInUse[i] = _a.timeInUse[i];
	}
}

appliance::appliance()
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
void appliance::calculateEnergy()
{
	for (int i = 0; i < 31; i++)
	{
		energy[i] = power * timeInUse[i];
	}
	for (int i = 0; i < 31; i++)
	{
		energySum += energy[i];
	}
}


//int writeDatatoFile(CString str, string file_name)
//{
//	fstream file;
//	//tring middle_string;
//	file.open(file_name, ios::out);
//	file << str << "\t";
//	file.close();
//	return 0;
//}

int getNumberFromFile()
{
	int number;
	fstream f;
	f.open(NUM_FILE, ios::in);
	if (!f.is_open())
	{
		return 1;
	}
	f >> number;
	f.close();
	return number;
}
//num_app = getNumberFromFile();

int getPowerFromFile()
{
	fstream f;
	string powerLine;
	num_app = getNumberFromFile();
	int i;
	f.open(POWER_FILE, ios::in);
	if (!f.is_open())
	{
		//MessageBox(L"File not found", L"Error", MB_OK + MB_ICONERROR);
		return 1;
	}
	getline(f, powerLine);
	stringstream s(powerLine);
	string power;
	for (i = 0; i < num_app; i++)
	{
		getline(s, power, '\t');
		app_set[i].power = stoi(power);
	}//Get the saved power from the file and assign them to respective appliances
	f.close();
	return 0;
}

int getUsingTimeFromFile()
{
	fstream f;
	string line;
	int day;
	//num_app = getNumberFromFile(); //Already read
	int i;
	f.open(INPUT_FILE, ios::in);
	if (!f.is_open())
	{
		//MessageBox(L"File not found", L"Error", MB_OK + MB_ICONERROR);
		return 1;
	}
	getline(f, line);
	for (day = 0; day < 31; day++)
	{
		getline(f, line);
		stringstream ss(line);
		string using_time;
		getline(ss, using_time, '\t');
		for (i = 0; i < num_app; i++)
		{
			getline(ss, using_time, '\t');
			app_set[i].timeInUse[day] = stod(using_time);
		}
	}//Get the string from the file, extract it and assign values to each appliance
	f.close();
	return 0;
}

int getNamesFromFile()
{
	fstream f;
	string nameList;
	int i;
	f.open(INPUT_FILE, ios::in);
	if (!f.is_open())
	{
		//MessageBox(L"File not found", L"Error", MB_OK + MB_ICONERROR);
		return 1;
	}
	getline(f, nameList);//get the name list from input file
	stringstream sss(nameList);
	string names;
	getline(sss, names, '\t');
	for (i = 0; i < num_app; i++)
	{
		getline(sss, names, '\t');
		//CString cNameStr = CA2CT(names.c)
		app_set[i].name = CA2CT(names.c_str());
	}
	f.close();
	return 0;
}

int writeDataToFiles()
{
	fstream file;
	string numStr;
	//string nameStr;
	file.open(POWER_FILE, ios::out);
	if (!file.is_open())
	{
		//MessageBox(L"File not found", L"Error", MB_OK + MB_ICONERROR);
		return 1;
	}
	for (int i = 0; i < num_app; i++)
	{
		file << app_set[i].power << "\t";
	}
	file.close();
	//Write the number of appliance to file
	file.open(NUM_FILE, ios::out);
	if (!file.is_open())
	{
		//MessageBox(L"File not found", L"Error", MB_OK + MB_ICONERROR);
		return 1;
	}
	file << num_app;
	file.close();

	//Write the appliance names to the input files
	file.open(INPUT_FILE, ios::out);
	if (!file.is_open())
	{
		//MessageBox(L"File not found", L"Error", MB_OK + MB_ICONERROR);
		return 1;
	}
	file << "\t";
	for (int i = 0; i < num_app; i++)
	{
		string nameStr;
		nameStr = CT2CA(app_set[i].name);
		file << nameStr << "\t";
	}
	file.close();
	//Write the numbers from 1 to 31 to input file
	file.open(INPUT_FILE, ios::app);
	if (!file.is_open())
	{
		//MessageBox(L"File not found", L"Error", MB_OK + MB_ICONERROR);
		return 1;
	}
	file << endl;
	for (int i = 1; i <= 31; i++)
	{
		file << i << "\t";
		for (int j = 0; j < num_app; j++)
		{
			file << "0\t";
		}
		file << endl;
	}
	file.close();
	return 0;
}
int outputData()
{
	double sumByDay[31];
	double max;
	fstream f;
	int i, day;
	/*for (i = 0; i < num_app; i++)
	{
	app_set[i].calculateEnergy();
	}
	for (day = 0; day < 30; day++)
	{
	for (i = 0; i < num_app; i++)
	{
	sumByDay[day] = calculateTotalEnergyByDay(i, day);
	}
	}*/
	f.open(OUTPUT_FILE, ios::out);
	if (!f.is_open())
	{
		//MessageBox(L"File not found", L"Error", MB_OK + MB_ICONERROR);
		return 1;
	}
	f << "\t";
	for (i = 0; i < num_app; i++)
	{
		f << CT2CA(app_set[i].name) << "\t";
	}
	//f << nameList;//put the name list on the output file
	f.close();
	f.open(OUTPUT_FILE, ios::app);
	if (!f.is_open())
	{
		//MessageBox(L"File not found", L"Error", MB_OK + MB_ICONERROR);
		return 1;
	}
	f << endl;
	for (day = 0; day < 31; day++)
	{
		f << day + 1 << "\t";
		for (int j = 0; j < num_app; ++j)
		{
			f << app_set[j].energy[day] << "\t";
		}
		//f << sumByDay[day];
		f << endl;
	}
	f << "Sum\t";
	max = app_set[0].energySum;
	for (i = 0; i < num_app; i++)
	{
		f << app_set[i].energySum << "\t";
		if (app_set[i].energySum > max)
		{
			max = app_set[i].energySum;
		}
	}
	f << endl << "Most consumed appliance: ";
	for (i = 0; i < num_app; i++)
	{
		if (app_set[i].energySum == max)
		{
			f << CT2CA(app_set[i].name);
		}
	}
	f.close();
	/*for (i = 0; i < num_app; i++)
	{
	int maxDay = findMostConsumedDayForAppliance(app_set[i]);
	f.open(OUTPUT_FILE, ios::app);
	if (!f.is_open())
	{
	return 1;
	}
	f << endl << "The day ";
	f << CT2CA(app_set[i].name) << " consumed most energy: Day " << maxDay + 1;
	f.close();
	}*/
	return 0;
}
int findMostConsumedDayForAppliance(appliance app)
{
	int day, maxDay;
	fstream file;
	double maxEnergy;
	maxEnergy = app.energy[0];
	for (day = 0; day < 31; day++)
	{
		if (app.energy[day] > maxEnergy)
		{
			maxEnergy = app.energy[day];
		}
	}
	for (day = 0; day < 31; day++)
	{
		if (app.energy[day] == maxEnergy)
		{
			maxDay = day;
			/*file.open(OUTPUT_FILE, ios::app);
			if (!file.is_open())
			{
			return 1;
			}
			file << endl << "The day ";
			file << CT2CA(app.name) << " consumed most energy: Day " << day + 1;
			file.close();*/
		}
	}
	return maxDay;
}
int writeNewDataToFiles(int previousNum)
{
	string line[32];
	fstream f;
	int i;
	f.open(NUM_FILE, ios::out);
	if (!f.is_open())
	{
		return 1;
	}
	f << num_app;
	f.close();
	f.open(POWER_FILE, ios::out);
	if (!f.is_open())
	{
		return 1;
	}
	for (i = 0; i < num_app; i++)
	{
		f << app_set[i].power << "\t";
	}
	f.close();
	f.open(INPUT_FILE, ios::in);
	if (!f.is_open())
	{
		return 1;
	}
	for (i = 0; i <= 31; i++)
	{
		getline(f, line[i]);
	}
	f.close();
	f.open(INPUT_FILE, ios::out);
	if (!f.is_open())
	{
		return 1;
	}
	f << "\t";
	for (i = 0; i < num_app; i++)
	{
		f << CT2CA(app_set[i].name) << "\t";
	}
	f << endl;
	for (i = 1; i <= 31; i++)
	{
		//f << line[i] << endl;
		f << line[i];
		//f << i << "\t";
		for (int j = previousNum - 1; j < num_app; j++)
		{
			f << "0\t";
		}
		f << endl;
	}
	f.close();
	return 0;
}
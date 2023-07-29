#pragma once
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class appliance
{
public:
	int power;
	double timeInUse[31];
	double energy[31];
	//double sumByDay[30];
	double energySum;
	CString name;
public:
	appliance();

	appliance(int p, CString n)
	{
		power = p;
		name = n;
	}
	appliance(appliance& _a);

	~appliance() {}
	void calculateEnergy();
	//void calculateTotalEnergyByDay(int num, int day);
	void inputData(int p, CString n)
	{
		power = p;
		name = n;
	}
};

int writeDatatoFile(CString str, string file_name);

int getNumberFromFile();
int getPowerFromFile();
int getUsingTimeFromFile();
int getNamesFromFile();
int writeDataToFiles();
int writeNewDataToFiles(int previousNum);
int outputData();
double calculateTotalEnergyByDay(int num, int day);
int findMostConsumedDayForAppliance(appliance app);

//public:
//	appliance();
//	~appliance();


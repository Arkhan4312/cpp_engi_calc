#pragma once
#include <iostream>
#include <list>
class UI
{
	std::list<double> ResTable;
	void ClearConsole();
	void NewExpression();
	bool ViewTable();
	void CreateTXT();
	void PrintResults(const std::string& fileName);
	bool Settings();
	int m = 5;
public:
	bool Menu();
	
};


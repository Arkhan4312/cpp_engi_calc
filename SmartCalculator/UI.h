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
	bool Settings();
	int m = 5;
public:
	bool Menu();
	
};


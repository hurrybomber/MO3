#pragma once
#include <tgmath.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

class simplex {
public:
	double **matrix;
	int *variables;
	int aspiration = 1;
	int rows = 0;
	int columns = 0;
	simplex();
	simplex(const std::string &path);
	simplex(simplex &, double, int);
	void branch(int x, int index, double* &variables);
	void print();
	bool step();
	void dual();
	void enumeration();
	~simplex();
};

#include "Simplex.hpp"
#include <algorithm>
#include <limits>
#include <vector>

simplex::simplex() {
	int aspiration = 1;
	int rows = 0;
	int columns = 0;
}

simplex::simplex(simplex& solution, double t, int index) {
	char c = ' ';
	int temp = 0;
	aspiration = 1;
	std::string buffer;
	std::vector<double> coefficients;
	std::ifstream input("data");
	rows = solution.rows + 1;
	columns = solution.columns;
	aspiration = solution.aspiration;
	matrix = new double*[rows];
	for (size_t i = 0; i < rows; i++) matrix[i] = new double[columns] {0};
	if (input.is_open()) {
		getline(input, buffer);
		while (c != '(') input >> c;
		while (input) {
			input >> temp;
			coefficients.push_back(temp);
		}
		for (size_t i = 0; i < columns - 1; i++)
			matrix[rows - 1][i + 1] = -coefficients[i] * aspiration;
		input.clear();
		input.get();
		input >> c;
		while (c != '(') input >> c;
		for (size_t i = 0; i < columns - 1; i++) {
			for (size_t j = 1; j < columns; j++) {
				input >> matrix[i][j];
			}
		}

		c = ' ';
		while (c != '(') input >> c;
		for (size_t i = 0; i < rows - 1; i++) input >> matrix[i][0];
		input.close();
	}
	else {
		std::cout << "File doesn't exist" << std::endl;
	}

	for (size_t i = columns - 1; i < rows - 2; i++)
		for (size_t j = 0; j < columns; j++) matrix[i][j] = solution.matrix[i][j];
	matrix[rows - 2][0] = t;
	matrix[rows - 2][index] = 1;
	variables = new int[rows + columns - 2];
	for (size_t i = 0; i < rows + columns - 2; i++)
		variables[i] = i + 1;
}

simplex::simplex(const std::string& path) {
	char c = ' ';
	int temp = 0;
	std::string buffer;
	std::vector<double> coefficients;
	std::ifstream input(path);
	;
	if (input.is_open()) {
		getline(input, buffer);
		if (buffer == "max") aspiration = -1;
		while (c != '(') input >> c;
		while (input) {
			input >> temp;
			coefficients.push_back(temp);
			columns++;
		}
		rows = columns;

		matrix = new double*[rows];
		for (int i = 0; i < rows; i++) matrix[i] = new double[columns] {0};

		variables = new int[rows + columns - 2];
		for (size_t i = 0; i < rows + columns - 2; i++) variables[i] = i + 1;

		for (size_t i = 0; i < rows - 1; i++)
			matrix[rows - 1][i + 1] = -coefficients[i] * aspiration;

		input.clear();
		input.get();
		input >> c;
		while (c != '(') input >> c;
		for (size_t i = 0; i < rows - 1; i++) {
			for (size_t j = 1; j < columns; j++) {
				input >> matrix[i][j];
			}
		}

		c = ' ';
		while (c != '(') input >> c;
		for (size_t i = 0; i < rows - 1; i++) input >> matrix[i][0];
		input.close();
	}
	else {
		std::cout << "File doesn't exist " << std::endl;
	}
}

simplex::~simplex() {
	for (int i = 0; i < columns; i++) delete[] matrix[i];
	delete[] matrix;

	delete[] variables;
	int aspiration = 1;
	int rows = 0;
	int columns = 0;
}

void simplex::print() {
	std::cout << std::setw(14) << "si";
	for (size_t i = 0; i < columns - 1; i++)
		std::cout << std::setw(11) << "x" << variables[i];
	std::cout << std::endl;
	for (size_t i = 0; i < rows - 1; ++i) {
		std::cout << "x" << variables[columns - 1 + i] << std::setw(8);
		for (size_t j = 0; j < columns; ++j) {
			std::cout << std::setw(12) << std::setprecision(4) << matrix[i][j];
		}
		std::cout << std::endl;
	}
	std::cout << "F" << std::setw(13);
	for (size_t i = 0; i < columns; i++)
		std::cout << matrix[rows - 1][i] << std::setw(12);
	std::cout << std::endl << std::endl;
}

bool simplex::step() {
	size_t decision_row = 0;
	size_t decision_column = 0;
	for (;;) {
		decision_row = 0;
		decision_column = 0;
		for (size_t i = 0; i < rows - 1; i++) {
			if (matrix[i][0] < 0) {
				for (size_t j = 1; j < columns; j++)
					if (matrix[i][j] < 0) decision_column = j;
				if (decision_column == 0) {
					std::cout << "No decision" << std::endl;
					return false;
				}
			}
		}
		if (decision_column != 0) {
			int relation = std::numeric_limits<int>::max();;
			for (size_t i = 0; i < rows - 1; i++) {
				if ((matrix[i][0] / matrix[i][decision_column] < relation) &&
					(matrix[i][0] / matrix[i][decision_column] >= 0)) {
					relation = matrix[i][0] / matrix[i][decision_column];
					decision_row = i;
				}
			}
			matrix[decision_row][decision_column] =
				1 / matrix[decision_row][decision_column];
			for (size_t i = 0; i < columns; i++)
				if (i != decision_column)
					matrix[decision_row][i] =
					matrix[decision_row][i] * matrix[decision_row][decision_column];
			for (size_t i = 0; i < rows; i++)
				if (i != decision_row)
					matrix[i][decision_column] = matrix[i][decision_column] *
					matrix[decision_row][decision_column] *
					-1;
			for (size_t i = 0; i < rows; i++)
				for (size_t j = 0; j < columns; j++)
					if ((i != decision_row) && (j != decision_column))
						matrix[i][j] = matrix[i][j] +
						matrix[i][decision_column] *
						matrix[decision_row][j] /
						matrix[decision_row][decision_column];
			std::swap(variables[decision_column - 1],
				variables[columns + decision_row - 1]);
			print();
		}
		else
			break;
	}

	size_t number = 0;

	std::cout << std::endl;
	std::cout << "step" << std::endl;

	for (;;) {
		number++;

		decision_column = 0;
		for (size_t j = 1; j < columns; j++) {
			if (matrix[rows - 1][j] > 0) {
				decision_column = j;
				break;
			}
		}

		if (decision_column == 0) {
			std::cout << "optimal solution" << std::endl;
			break;
		}

		double relation = std::numeric_limits<int>::max();;
		decision_row = 0;

		for (size_t i = 0; i < rows - 1; i++) {
			if ((matrix[i][0] / matrix[i][decision_column] >= 0) &&
				(matrix[i][0] / matrix[i][decision_column] < relation)) {
				relation = matrix[i][0] / matrix[i][decision_column];
				decision_row = i;
			}
		}

		matrix[decision_row][decision_column] =
			1 / matrix[decision_row][decision_column];
		for (size_t i = 0; i < columns; i++)
			if (i != decision_column)
				matrix[decision_row][i] =
				matrix[decision_row][i] * matrix[decision_row][decision_column];
		for (size_t i = 0; i < rows; i++)
			if (i != decision_row)
				matrix[i][decision_column] = matrix[i][decision_column] *
				matrix[decision_row][decision_column] * -1;
		for (size_t i = 0; i < rows; i++)
			for (size_t j = 0; j < columns; j++)
				if ((i != decision_row) && (j != decision_column))
					matrix[i][j] = matrix[i][j] +
					matrix[i][decision_column] * matrix[decision_row][j] /
					matrix[decision_row][decision_column];

		std::cout << "iteration " << number << " :" << std::endl;
		std::swap(variables[decision_column - 1],
			variables[columns + decision_row - 1]);
		print();
	}
	matrix[rows - 1][0] *= aspiration;
	std::cout << "F = " << matrix[rows - 1][0] << std::endl;
	return true;
}

void simplex::branch(int x, int index, double*& mxvariables) {
	simplex solution1(*this, x, index);
	solution1.print();

	simplex solution2(*this, x + 1, index);
	solution2.matrix[solution2.rows - 2][0] *= -1;
	solution2.matrix[solution2.rows - 2][index] = -1;
	solution2.print();
	if (solution1.step()) {
		double* table1;
		table1 = new double[solution1.columns];
		for (size_t i = 0; i < solution1.columns; i++) table1[i] = 0;
		table1[0] = solution1.matrix[solution1.columns][0];
		for (size_t i = 1; i < solution1.columns; i++) {
			for (size_t j = 0; j < solution1.rows + solution1.columns - 2; j++) {
				if (solution1.variables[j] == i)
					if (j < solution1.columns - 1)
						table1[i] = 0;
					else
						table1[i] = solution1.matrix[j - solution1.columns + 1][0];
			}
		}
		bool flag = true;
		for (size_t i = index + 1; i < solution1.columns; i++) {
			double mod = table1[i] - int(table1[i]);
			if (mod != 0) {
				simplex solutionx(*this, x, index);
				solutionx.branch(int(table1[i]), i, mxvariables);
				flag = false;
				break;
			}
		}
		if ((table1[0] > mxvariables[0]) && (flag))
			for (size_t i = 0; i < solution1.columns; i++)
				mxvariables[i] = table1[i];
		delete[] table1;
	}

	solution2.print();
	if (solution2.step()) {
		double* table2;
		table2 = new double[solution2.columns];
		for (size_t i = 0; i < solution2.columns; i++) table2[i] = 0;
		table2[0] = solution2.matrix[solution2.columns][0];
		for (size_t i = 1; i < solution2.columns; i++) {
			for (size_t j = 0; j < solution2.rows + solution2.columns - 2; j++) {
				if (solution2.variables[j] == i)
					if (j < solution2.columns - 1)
						table2[i] = 0;
					else
						table2[i] = solution2.matrix[j - solution2.columns + 1][0];
			}
		}

		bool flag = true;
		for (size_t i = index + 1; i < solution1.columns; i++) {
			double mod = table2[i] - int(table2[i]);
			if (mod != 0) {
				simplex solutionx(*this, x + 1, index);
				solutionx.branch(int(table2[i]), i, mxvariables);
				flag = false;
				break;
			}
		}
		if ((table2[0] > mxvariables[0]) && (flag))
			for (size_t i = 0; i < solution2.columns; i++)
				mxvariables[i] = table2[i];
		delete[] table2;
	}
}

void simplex::enumeration() {
	simplex sol("data");
	double* variables;
	std::vector<int> _vec_ind;
	variables = new double[sol.columns];
	for (size_t i = 0; i < sol.columns; i++) {
		variables[i] = 0;
		_vec_ind.push_back(i);
	}
	std::vector<int> _vec_var;
	for (size_t i = 0; i < sol.columns - 1; i++) {
		_vec_var.push_back(std::numeric_limits<int>::max());
	}
	std::cout << "bust:" << std::endl;
	std::cout << "F x1 x2 x3" << std::endl;
	for (size_t i = 0; i < 3; i++) {
		if (sol.matrix[i][1] != 0)
			if (((trunc(sol.matrix[i][0] / sol.matrix[i][1])) < _vec_var[0]) &&
				(sol.matrix[i][1] > 0))
				_vec_var[0] = trunc(sol.matrix[i][0] / sol.matrix[i][1]);
		if (sol.matrix[i][2] != 0)
			if (((trunc(sol.matrix[i][0] / sol.matrix[i][2])) < _vec_var[1]) &&
				(sol.matrix[i][2] > 0))
				_vec_var[1] = trunc(sol.matrix[i][0] / sol.matrix[i][2]);
		if (sol.matrix[i][3] != 0)
			if (((trunc(sol.matrix[i][0] / sol.matrix[i][3])) < _vec_var[2]) &&
				(sol.matrix[i][3] > 0))
				_vec_var[2] = trunc(sol.matrix[i][0] / sol.matrix[i][3]);
	}
	for (size_t i = 0; i <= _vec_var[0]; i++) {
		for (size_t j = 0; j <= _vec_var[1]; j++) {
			for (size_t k = 0; k <= _vec_var[2]; k++)
				if ((sol.matrix[0][1] * i + sol.matrix[0][2] * j +
					sol.matrix[0][3] * k <=
					sol.matrix[0][0]) &&
					(sol.matrix[1][1] * i + sol.matrix[1][2] * j +
						sol.matrix[1][3] * k <=
						sol.matrix[1][0]) &&
						(sol.matrix[2][1] * i + sol.matrix[2][2] * j +
							sol.matrix[2][3] * k <=
							sol.matrix[2][0])) {
					if (sol.matrix[3][1] * i + sol.matrix[3][2] * j +
						sol.matrix[3][3] * k >
						variables[0]) {
						variables[0] = sol.matrix[3][1] * i + sol.matrix[3][2] * j +
							sol.matrix[3][3] * k;
						variables[1] = i;
						variables[2] = j;
						variables[3] = k;
					}
					std::cout << sol.matrix[3][1] * i + sol.matrix[3][2] * j +
						sol.matrix[3][3] * k
						<< "  " << i << "  " << j << "  " << k << std::endl;
				}
		}
	}
	std::cout << std::endl;
	for (size_t i = 0; i < sol.columns; i++) {
		if (i == 0) {
			std::cout << "F=" << variables[i];
		}
		else {
			std::cout << " x" << i << "=" << variables[i] << " ";
		}
	}
	std::cout << std::endl;
	delete[] variables;
	system("pause");
}
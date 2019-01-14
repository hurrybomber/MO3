#include "Simplex.hpp"

int main() {
	double mod = 0;
	int F = 0;
	simplex solution("data");
	std::cout << "simplex table:" << std::endl;
	solution.print();
	solution.step();
	double* table;
	table = new double[solution.columns];
	double* variables;
	variables = new double[solution.columns];
	for (size_t i = 0; i < solution.columns; i++) {
		variables[i] = 0;
		table[i] = 0;
	}
	variables[0] = solution.matrix[solution.rows - 1][0];
	for (size_t i = 1; i < solution.columns; i++) {
		for (size_t j = 0; j < solution.rows + solution.columns - 2; j++) {
			if (solution.variables[j] == i)
				if (j < solution.columns - 1)
					variables[i] = 0;
				else
					variables[i] = solution.matrix[j - solution.columns + 1][0];
		}
	}

	std::cout << std::endl;
	std::cout << std::endl;
	for (size_t i = 1; i < solution.columns; i++) {
		mod = variables[i] - int(variables[i]);
		if (mod != 0) {
			solution.branch(int(variables[i]), i, table);
			break;
		}
	}
	std::cout << std::endl;
	std::cout << std::endl;
	for (size_t i = 0; i < solution.columns; i++) {
		if (i == 0) {
			std::cout << "F=" << table[i];
		}
		else {
			std::cout << " x" << i << "=" << table[i] << " ";
		}
	}
	std::cout << std::endl;
	std::cout << std::endl;
	solution.enumeration();
	delete[] variables;
	delete[] table;
	return 0;
}


#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <chrono>
#include <thread>


using namespace std;

// ������� ���������� �������(����㳿)
int calculateCost(const vector<int>& queens) {
	int n = queens.size();
	int cost = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			if (queens[i] == queens[j] || abs(i - j) == abs(queens[i] - queens[j])) {
				++cost;
			}
		}
	}
	return cost;
}

// ������� ���������� ��������� ��������� ������ ��������
double acceptanceProbability(int delta, double temperature) {
	// ���� ����� ���� ������ �� ���� ���������� ������
	if (delta < 0) {
		return 1.0;
	}
	// ������ ������ ��������� �� ������� ��������
	return exp(-delta / temperature);
}

// ������� ���������� ������
vector<int> solve(int n, double initialTemperature, double coolingRate, int iterations) {
	// ��������� ���������� �����
	random_device rd;
	mt19937 rng(rd());
	// ���������� ���� �����
	vector<int> queens(n);
	for (int i = 0; i < n; ++i) {
		queens[i] = i;
	}
	shuffle(queens.begin(), queens.end(), rng);
	// ������� ����� �� ����� ������
	int currentCost = calculateCost(queens);
	int newCost;
	// ��������� � ��������� �����
	vector<int> bestQueens = queens;
	int bestCost = currentCost;
	// �����������
	double temperature = initialTemperature;
	// �������� ���� ���������
	for (int i = 0; i < iterations; ++i) {
		// ��������� ������ �����
		vector<int> newQueens = queens;
		shuffle(newQueens.begin(), newQueens.end(), rng);
		// ���������� ���� �������
		newCost = calculateCost(newQueens);
		// ��������� ������ �����
		if (acceptanceProbability(newCost - currentCost, temperature) > uniform_real_distribution<double>(0, 1)(rng)) {
			queens = newQueens;
			currentCost = newCost;
		}
		// ��������� ���������� �����
		if (currentCost < bestCost) {
			bestQueens = queens;
			bestCost = currentCost;
		}
		// �������� �����������
		temperature *= 1 - coolingRate;
	}
	return bestQueens;
}

int main() {
	clock_t start = clock();
	int n = 15; // ����� �����
	double initialTemperature = 100; // ��������� �����������
	double coolingRate = 0.0001; // ���������� �����������
	int iterations = 10000; // ������� ��������
	// ���������� ������
	vector<int> queens = solve(n, initialTemperature, coolingRate, iterations);
	// ���� ����������
	cout << "Solution found with cost " << calculateCost(queens) << ":" << endl;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (queens[i] == j) {
				cout << "Q ";
			}
			else {
				cout << ". ";
			}
		}
		cout << endl;
	}
	clock_t end = clock();

	cout << "\n\n\n";
	for (int i = 0; i < n; i++) {
		cout << queens[i] << "\t";
	}
	
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	printf("The time: %f seconds\n", seconds);

	//cout << "Loading.";
	//while (1) {
	//	this_thread::sleep_for(chrono::milliseconds(500));
	//	cout << ".";
	//	this_thread::sleep_for(chrono::milliseconds(500));
	//	cout << ".";
	//	this_thread::sleep_for(chrono::milliseconds(500));
	//	cout << "\b\b  \b\b";
	//}


	return 0;
}

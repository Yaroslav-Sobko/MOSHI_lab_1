#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <chrono>
#include <thread>


using namespace std;

// Функція обчислення вартості(енергії)
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

// Функція обчислення ймовірності прийняття нового розвязку
double acceptanceProbability(int delta, double temperature) {
	// якщо новий стан кращий то його приймається завжди
	if (delta < 0) {
		return 1.0;
	}
	// інакше рахуємо ймовірність за поданою формулою
	return exp(-delta / temperature);
}

// Функція розвязання задачі
vector<int> solve(int n, double initialTemperature, double coolingRate, int iterations) {
	// генерація випадкових чисел
	random_device rd;
	mt19937 rng(rd());
	// початковий стан ферзів
	vector<int> queens(n);
	for (int i = 0; i < n; ++i) {
		queens[i] = i;
	}
	shuffle(queens.begin(), queens.end(), rng);
	// вартість станів на даний момент
	int currentCost = calculateCost(queens);
	int newCost;
	// найкращий зі знайдених станів
	vector<int> bestQueens = queens;
	int bestCost = currentCost;
	// Температура
	double temperature = initialTemperature;
	// Основний цикл алгоритму
	for (int i = 0; i < iterations; ++i) {
		// Генерація нового стану
		vector<int> newQueens = queens;
		shuffle(newQueens.begin(), newQueens.end(), rng);
		// обчислення нової вартості
		newCost = calculateCost(newQueens);
		// Прийняття нового стану
		if (acceptanceProbability(newCost - currentCost, temperature) > uniform_real_distribution<double>(0, 1)(rng)) {
			queens = newQueens;
			currentCost = newCost;
		}
		// Оновлення найкращого стану
		if (currentCost < bestCost) {
			bestQueens = queens;
			bestCost = currentCost;
		}
		// Зниження температури
		temperature *= 1 - coolingRate;
	}
	return bestQueens;
}

int main() {
	clock_t start = clock();
	int n = 15; // Розмір дошки
	double initialTemperature = 100; // Початкова температура
	double coolingRate = 0.0001; // Коефіцієнт охоледження
	int iterations = 10000; // кількість ітерацій
	// розвязання задачі
	vector<int> queens = solve(n, initialTemperature, coolingRate, iterations);
	// Вивід результату
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

#include <iostream>
#include <cstdarg>
#include "Matrix.h"
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

template<typename T = double>
class Matrix {
public:
	Matrix(const int n) {
		N_ = n;
		M_ = 1;
		mas = new T* [n];
	}
	Matrix(const int n, const int m) {
		N_ = n;
		M_ = m;
		mas = new T * [N_ * M_];
	}

	Matrix(std::initializer_list<T>& lst) {
		N_ = 1;
		M_ = lst.size();
		mas = new T * [M_];
		for (size_t i = 0; i < M_; i++) {
			mas[i] = *lst(lst.begin() + i);
		}
	}

	Matrix(std::initializer_list<std::initializer_list<T>>& lst) {
		N_ = lst.size();
		M_ = *(lst.begin()).size();
		mas = new T * [N_ * M_];
		for (size_t i = 0; i < N_; i++) {
			for (size_t j = 0; j < M_; j++) {
				mas[i * M_ + j] = *((lst.begin() + i)->begin())[j];
			}
		}
	}

	int getN() const{
		return N_;
	}
	int getM() const{
		return M_;
	}
	T** getMas() const{
		return mas;
	}
	//copy constructor
	Matrix(Matrix& other) {
		N_ = other.N_;
		M_ = other.M_;
		mas = new T * [N_ * M_];
		for (size_t i = 0; i < N_; i++) {
			for (size_t j = 0; j < M_; j++) {
				mas[i * M_ + j] = other.mas[i * M_ + j];
			}
		}
	}

	// assignment operator
	Matrix& operator=(Matrix& other) {
		if (this != &other) {
			delete[] mas_;
			mas_ = new T[other.N_ * other.M_];
			for (size_t i = 0; i < other.N_; ++i) {
				for (size_t j = 0; j < other.M_; ++j) {
					mas[i * M_ + j] = other.mas[i * M_ + j];
				}
			}
			M_ = other.M_;
			N_ = other.N_;
		}
		return *this;
	}

	//move constructor
	Matrix(Matrix&& other) noexcept{
		delete[] mas_;
		mas_ = new T[other.N_ * other.M_];
		for (size_t i = 0; i < other.N_; ++i) {
			for (size_t j = 0; j < other.M_; ++j) {
				mas[i * M_ + j] = other.mas[i * M_ + j];
			}
		}
		M_ = other.M_;
		N_ = other.N_;
		delete[] other.mas_;
		other.M_ = 0;
		other.N_ = 0;
	}

	//move operator
	Matrix& operator=(Matrix&& other)  noexcept {
		if (this != &other) {
			delete[] mas_;
			mas_ = new T[other.N_ * other.M_];
			for (size_t i = 0; i < other.N_; ++i) {
				for (size_t j = 0; j < other.M_; ++j) {
					mas[i * M_ + j] = other.mas[i * M_ + j];
				}
			}
			M_ = other.M_;
			N_ = other.N_;
			delete[] other.mas_;
			other.M_ = 0;
			other.N_ = 0;
		}
		return *this;
	}

	// output operator
	friend ostream& operator<<(ostream& out, Matrix& matrix) {
		for (size_t i = 0; i < matrix.N_; i++) {
			for (size_t j = 0; j < matrix.M_; j++) {
				out << *(matrix.mas_.begin() + i * M_ + j) << " ";
			}
			out << "\n";
		}
		return out;
	}

private:
	int N_;
	int M_;
	T* mas;
};



int main() {
	/*Matrix<int>vec(2, 4);
	int* a = new int[2] { 2, 1 };
	int* b = new int[2] { 3, 4 };*/
	
}
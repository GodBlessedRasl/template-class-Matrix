﻿#include <iostream>
#include <cstdarg>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <type_traits>

typedef std::common_type<unsigned char, short, int, const int>::type NumericType;
typedef std::common_type<float, double>::type FloatType;

template<typename T = double>
class Matrix {
public:

	template<typename T2>
	void check() {
		try {
			if (std::is_same<T2, NumericType>::value == false && std::is_same<T2, FloatType>::value == false) {
				//std::cout << typeid(T2).name() << "\n\n";
				std::exception error("Inappropriate type");
				throw error;
			}
		}
		catch (std::exception& err) {
			std::cout << err.what() << "\n";
		}
	}

	Matrix(const int n) {
		N_ = n;
		M_ = 1;
		mas_ = new T[n];
	}
	Matrix(const int n, const int m) {
		N_ = n;
		M_ = m;
		mas_ = new T[N_ * M_];
	}
	
	template<typename T2>
	Matrix(std::initializer_list<T2>& lst) {
		//check<T2>();
		N_ = 1;
		M_ = lst.size();
		mas_ = new T[M_];
		for (size_t i = 0; i < M_; i++) {
			mas_[i] = static_cast<T>(*lst(lst.begin() + i));
		}
	}

	template<typename T2>
	Matrix(std::initializer_list<std::initializer_list<T2>>& lst) {
		//check<T2>();
		N_ = lst.size();
		M_ = (*lst.begin()).size();
		mas_ = new T[N_ * M_];
		for (size_t i = 0; i < N_; i++) {
			for (size_t j = 0; j < M_; j++) {
				mas_[i * M_ + j] = static_cast<T>(*((*(lst.begin() + i)).begin() + j));
			}
		}
	}

	size_t getN() const{
		return N_;
	}
	size_t getM() const{
		return M_;
	}
	T* getMas() const{
		return mas_;
	}
	//copy constructor
	template<typename T2>
	Matrix(const Matrix<T2>& other) {
		//check<T2>();
		N_ = other.N_;
		M_ = other.M_;
		mas_ = new T[N_ * M_];
		for (size_t i = 0; i < N_; i++) {
			for (size_t j = 0; j < M_; j++) {
				mas_[i * M_ + j] = static_cast<T>(other.mas_[i * M_ + j]);
			}
		}
	}

	// assignment operator
	template<typename T2>
	Matrix& operator=(const Matrix<T2>& other) {
		//check<T2>();
		if (this != reinterpret_cast<Matrix<T>*>(& other)) {
			delete[] mas_;
			mas_ = new T[other.getN() * other.getM()];
			M_ = other.getM();
			N_ = other.getN();
			for (size_t i = 0; i < other.getN(); ++i) {
				for (size_t j = 0; j < other.getM(); ++j) {
					mas_[i * M_ + j] = other.getMas()[i * M_ + j];
				}
			}
		}
		return *this;
	}

	//move constructor
	Matrix(Matrix&& other) noexcept{
		delete[] mas_;
		mas_ = new T[other.getN() * other.getM()];
		M_ = other.getM();
		N_ = other.getN();
		for (size_t i = 0; i < other.getN(); ++i) {
			for (size_t j = 0; j < other.getM(); ++j) {
				mas_[i * M_ + j] = other.getMas()[i * M_ + j];
			}
		}
		delete[] other.mas_;
		other.M_ = 0;
		other.N_ = 0;
	}

	//move operator
	template<typename T2>
	Matrix<T>& operator=(Matrix<T2>&& other)  noexcept {
		if (this != &other) {
			delete[] mas_;
			mas_ = new T[other.N_ * other.M_];
			M_ = other.M_;
			N_ = other.N_;
			for (size_t i = 0; i < other.N_; ++i) {
				for (size_t j = 0; j < other.M_; ++j) {
					mas_[i * M_ + j] = static_cast<T>(other.mas_[i * M_ + j]);
				}
			}
			delete[] other.mas_;
			other.M_ = 0;
			other.N_ = 0;
		}
		return *this;
	}

	// output operator
	friend std::ostream& operator<<(std::ostream& out, Matrix<T>& matrix) {
		for (size_t i = 0; i < matrix.N_; i++) {
			for (size_t j = 0; j < matrix.M_; j++) {
				out << matrix.mas_[i * matrix.M_ + j] << " ";
			}
			out << "\n";
		}
		return out;
	}

	// operator () const
	T& operator () (const int i, const int j) {
		try {
			if (i >= N_ || j >= M_) {
				std::exception error("Too big size value");
				throw error;
			}
			return mas_[i * M_ + j];
		}
		catch (std::exception& err) {
			std::cout << err.what() << "\n";
		}
	}

	// operator () 
	T operator () (const int i, const int j) const{
		try {
			if (i >= N_ || j >= M_) {
				std::exception error("Too big size value");
				throw error;
			}
			return mas_[i * M_ + j];
		}
		catch (std::exception& err) {
			std::cout << err.what() << "\n";
		}
	}

	// operator +
	template<typename T2>
	friend Matrix operator + (const Matrix& matrix, const Matrix<T2>& other) {
		//matrix.check<T2>();
		std::cout << typeid(T2).name() << "\n\n";
		try {
			if (!(matrix.N_ == other.getN() && matrix.M_ == other.getM())) {
				std::exception error("mismatched sizes");
				throw error;
			}
			Matrix result(matrix.N_, matrix.M_);
			for (size_t i = 0; i < matrix.N_; i++) {
				for (size_t j = 0; j < matrix.M_; j++) {
					result.mas_[i * result.M_ + j] = matrix.mas_[i * matrix.M_ + j] +
						static_cast<T>(other.getMas()[i * other.getM() + j]);
				}
			}
			return Matrix(result);
		}
		catch (std::exception& err) {
			std::cout << err.what() << "\n";
		}
	}

	// operator +=
	template<typename T2>
	Matrix<T>& operator += (Matrix<T2> other) {
		//check<T2>();
		try {
			if (!(N_ == other.getN() && M_ == other.getM())) {
				std::exception error("mismatched sizes");
				throw error;
			}
			for (size_t i = 0; i < N_; i++) {
				for (size_t j = 0; j < M_; j++) {
					mas_[i * M_ + j] += static_cast<T>(other.getMas()[i * M_ + j]);
				}
			}
			return *this;
		}
		catch (std::exception& err) {
			std::cout << err.what() << "\n";
		}
	}

	// operator -
	template<typename T2>
	friend Matrix operator - (const Matrix& matrix, const Matrix<T2>& other) {
		//matrix.check<T2>();
		std::cout << typeid(T2).name() << "\n\n";
		try {
			if (!(matrix.N_ == other.getN() && matrix.M_ == other.getM())) {
				std::exception error("mismatched sizes");
				throw error;
			}
			Matrix result(matrix.N_, matrix.M_);
			for (size_t i = 0; i < matrix.N_; i++) {
				for (size_t j = 0; j < matrix.M_; j++) {
					result.mas_[i * result.M_ + j] = matrix.mas_[i * matrix.M_ + j] -
						static_cast<T>(other.getMas()[i * other.getM() + j]);
				}
			}
			return Matrix(result);
		}
		catch (std::exception& err) {
			std::cout << err.what() << "\n";
		}
	}

	// operator -=
	template<typename T2>
	Matrix<T>& operator -= (Matrix<T2> other) {
		//check<T2>();
		try {
			if (!(N_ == other.getN() && M_ == other.getM())) {
				std::exception error("mismatched sizes");
				throw error;
			}
			for (size_t i = 0; i < N_; i++) {
				for (size_t j = 0; j < M_; j++) {
					mas_[i * M_ + j] -= static_cast<T>(other.getMas()[i * M_ + j]);
				}
			}
			return *this;
		}
		catch (std::exception& err) {
			std::cout << err.what() << "\n";
		}
	}

	// multiplication of matrices operator
	template<typename T2>
	friend Matrix operator * (Matrix& matrix, Matrix<T2>& other) {
		//matrix.check<T2>();
		try {
			if (!(matrix.N_ == other.getM())) {
				std::exception error("mismatched sizes");
				throw error;
			}
			Matrix result(matrix.N_, other.getM());
			for (size_t i = 0; i < matrix.N_; i++) {
				for (size_t j = 0; j < other.getM(); j++) {
					result.mas_[i * result.M_ + j] = 0;
					for (size_t k = 0; k < other.getN(); k++) {
						result.mas_[i * result.M_ + j] += matrix.mas_[i * matrix.M_ + k]
							* static_cast<T>(other.getMas()[k * other.getM() + j]);
					}
				}
			}
			return Matrix(result);
		}
		catch (std::exception& err) {
			std::cout << err.what() << "\n";
		}
	}

	// multiplication of matrix and number operator
	template<typename T2>
	Matrix<T> operator * (T2 num) {
		//check<T2>();
		Matrix result(N_, M_);
		for (size_t i = 0; i < N_; i++) {
			for (size_t j = 0; j < M_; j++) {
				result.mas_[i * M_ + j] = mas_[i * M_ + j] * static_cast<T>(num);
			}
		}
		return Matrix(result);
	}

	// *=
	template<typename T2>
	Matrix<T>& operator *= (T2 num) {
		check<T2>();
		for (size_t i = 0; i < N_; i++) {
			for (size_t j = 0; j < M_; j++) {
				mas_[i * M_ + j] *= static_cast<T>(num);
			}
		}
		return *this;
	}

private:
	size_t N_;
	size_t M_;
	T* mas_;
};


int main() {
	std::initializer_list<std::initializer_list<int>> a_lst = { {1,2,3}, {1,2,3} };
	Matrix<double>a(a_lst);
	std::initializer_list<std::initializer_list<int>> b_lst = { {1,1}, {2,2}, {3,3} };
	Matrix<int>b(b_lst);
	int k = 4;
	Matrix<short>c = b;
	std::cout << c;

	std::initializer_list<std::initializer_list<int>> c_lst = { {1, 2, 3}, {4, 5, 6} };
	Matrix<int>m(c_lst);
	int val = m(0, 2); // 3 т.к. это 1-ая строка, 3-ий элемент
	m(0, 2) = 7;
	std::cout << m(0, 2) << "\n";

	// int список инициализации в double матрицу
	std::initializer_list<std::initializer_list<int>> d_lst = { {1, 2}, {3, 4}, {5, 6} };
	Matrix<double> m_d(d_lst);
	Matrix<double> ans = m * m_d;
	std::cout << ans;
	// double список инициализации в int матрицу
	std::initializer_list<std::initializer_list<double>> e_lst = { {1.1, 2.2, 3.3}, {4.4, 5.5, 6.6} };
	Matrix<int>m_i(e_lst);
	std::cout << m_i << "\n";
	// инициализация short матрицы с помощью double матрицы
	Matrix<short>m_s = m_d;
	// присваивание int матрицы в short матрицу
	m_s = m_i;
	std::cout << m_s << "\n";
	// Арифметические операции между матрицами с разными типами:
	m_s += m_i;
	std::cout << m_s << "\n";
	m_s -= m_i;
	std::cout << m_s << "\n";
	m_s *= 3.14;
	std::cout << m_s << "\n";
	m_s = m_s + m_i - m_s;
	std::cout << m_s << "\n";
	m_d* m_i;
	m_d * 0;
	m_i * 3.14;

}

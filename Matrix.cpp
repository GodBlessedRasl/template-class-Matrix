#include <iostream>
#include <cstdarg>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <type_traits>

typedef std::common_type<unsigned char, short, int, short int>::type NumericType;
typedef std::common_type<float, double>::type FloatType;

template<typename T = double>
class Matrix {
public:

	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
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

	size_t getN() const {
		return N_;
	}
	size_t getM() const {
		return M_;
	}
	void setN(const size_t N) {
		N_ = N;
	}
	void setM(const size_t M) {
		M_ = M;
	}
	void setMas(T& mas) {
		mas_ = mas;
	}
	T* getMas() const {
		return mas_;
	}

	Matrix<T>(const int n) {
		setN(n);
		setM(1);
		mas_ = new T[n];
	}
	Matrix<T>(const int n, const int m) {
		setN(n);
		setM(m);
		mas_ = new T[N_ * M_];
	}
	
	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
	Matrix<T>(std::initializer_list<T2>& lst) {
		check<T2>();
		N_ = 1;
		M_ = lst.size();
		mas_ = new T[M_];
		for (size_t i = 0; i < M_; i++) {
			mas_[i] = static_cast<T>(*lst(lst.begin() + i));
		}
	}

	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
	Matrix<T>(std::initializer_list<std::initializer_list<T2>>& lst) {
		check<T2>();
		N_ = lst.size();
		M_ = (*lst.begin()).size();
		mas_ = new T[N_ * M_];
		for (size_t i = 0; i < N_; i++) {
			for (size_t j = 0; j < M_; j++) {
				mas_[i * M_ + j] = static_cast<T>(*((*(lst.begin() + i)).begin() + j));
			}
		}
	}

	//copy constructor
	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
	Matrix<T>(Matrix<T2>& other) {
		//check<T2>();
		//std::cout << typeid(T2).name() << "\n\n";
		N_ = other.getN();
		M_ = other.getM();
		mas_ = new T[N_ * M_];
		for (size_t i = 0; i < N_; i++) {
			for (size_t j = 0; j < M_; j++) {
				mas_[i * M_ + j] = static_cast<T>(other.getMas()[i * M_ + j]);
			}
		}
	}

	// assignment operator
	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
	Matrix<T>& operator=(const Matrix<T2>& other) {
		//check<T2>();
			delete[] mas_;
			mas_ = new T[other.getN() * other.getM()];
			M_ = other.getM();
			N_ = other.getN();
			for (size_t i = 0; i < other.getN(); ++i) {
				for (size_t j = 0; j < other.getM(); ++j) {
					mas_[i * M_ + j] = other.getMas()[i * M_ + j];
				}
			}
		return *this;
	}

	//move constructor
	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
	Matrix<T>(Matrix<T2>&& other) noexcept{
		//check<T2>();
		delete[] mas_;
		mas_ = new T[other.getN() * other.getM()];
		M_ = other.getM();
		N_ = other.getN();
		for (size_t i = 0; i < other.getN(); ++i) {
			for (size_t j = 0; j < other.getM(); ++j) {
				mas_[i * M_ + j] = static_cast<T>(other.getMas()[i * M_ + j]);
			}
		}
		other.setMas(nullptr);
		other.setM(0);
		other.setN(0);
	}

	//move operator
	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
	Matrix<T>& operator=(Matrix<T2>&& other)  noexcept {
		//check<T2>();
		if (this != reinterpret_cast<Matrix<T>*>(&other)) {
			delete[] mas_;
			mas_ = new T[other.N_ * other.M_];
			M_ = other.getM();
			N_ = other.getN();
			for (size_t i = 0; i < other.N_; ++i) {
				for (size_t j = 0; j < other.M_; ++j) {
					mas_[i * M_ + j] = static_cast<T>(other.mas_[i * M_ + j]);
				}
			}
			other.setMas(nullptr);
			other.setM(0);
			other.setN(0);
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
	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
	friend Matrix<T> operator + (const Matrix& matrix, const Matrix<T2>& other) {
		//matrix.check<T2>();
		//std::cout << typeid(T2).name() << "\n\n";
		try {
			if (!(matrix.N_ == other.getN() && matrix.M_ == other.getM())) {
				std::exception error("mismatched sizes");
				throw error;
			}
			Matrix<T> result(matrix.N_, matrix.M_);
			for (size_t i = 0; i < matrix.N_; i++) {
				for (size_t j = 0; j < matrix.M_; j++) {
					result.mas_[i * result.M_ + j] = matrix.mas_[i * matrix.M_ + j] +
						static_cast<T>(other.getMas()[i * other.getM() + j]);
				}
			}
			return Matrix<T>(result);
		}
		catch (std::exception& err) {
			std::cout << err.what() << "\n";
		}
	}

	// operator +=
	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
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
	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
	friend Matrix<T> operator - (const Matrix& matrix, const Matrix<T2>& other) {
		//matrix.check<T2>();
		//std::cout << typeid(T2).name() << "\n\n";
		try {
			if (!(matrix.N_ == other.getN() && matrix.M_ == other.getM())) {
				std::exception error("mismatched sizes");
				throw error;
			}
			Matrix<T> result(matrix.N_, matrix.M_);
			for (size_t i = 0; i < matrix.N_; i++) {
				for (size_t j = 0; j < matrix.M_; j++) {
					result.mas_[i * result.M_ + j] = matrix.mas_[i * matrix.M_ + j] -
						static_cast<T>(other.getMas()[i * other.getM() + j]);
				}
			}
			return Matrix<T>(result);
		}
		catch (std::exception& err) {
			std::cout << err.what() << "\n";
		}
	}

	// operator -=
	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
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
	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
	Matrix<T> operator * (const Matrix<T2>& other) {
		//matrix.check<T2>();
		try {
			if (!(N_ == other.getM())) {
				std::exception error("mismatched sizes");
				throw error;
			}
			Matrix<T> result(N_, other.getM());
			for (size_t i = 0; i < N_; i++) {
				for (size_t j = 0; j < other.getM(); j++) {
					result.mas_[i * result.M_ + j] = 0;
					for (size_t k = 0; k < other.getN(); k++) {
						result.mas_[i * result.M_ + j] += mas_[i * M_ + k]
							* static_cast<NumericType>(other.getMas()[k * other.getM() + j]);
					}
				}
			}
			return Matrix<T>(result);
		}
		catch (std::exception& err) {
			std::cout << err.what() << "\n";
		}
	}


	// multiplication of matrix and number operator
	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
	Matrix<T> operator * (T2 num) {
		//check<T2>();
		Matrix<T> result(N_, M_);
		for (size_t i = 0; i < N_; i++) {
			for (size_t j = 0; j < M_; j++) {
				result.mas_[i * M_ + j] = mas_[i * M_ + j] * static_cast<T>(num);
			}
		}
		return Matrix<T>(result);
	}

	// *=
	template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
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
	b *= 4;
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
	// произведение векторов
	Matrix<int>ans = m * m_d;
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

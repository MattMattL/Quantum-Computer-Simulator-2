#ifndef QUMULATOR_MATRIX_HPP
#define QUMULATOR_MATRIX_HPP

#define Entry Complex

#include "complex.hpp"

template<class T>
class Matrix
{
private:
	Entry<T> **matrix;
	int ROWS, COLS;

	bool isInBoundary(int row, int col)
	{
		return (0 <= row && row < ROWS) && (0 <= col && col < COLS);
	}

	void barf(string function, string message)
	{
		cout << "[error] " << "<" << function << ">";
		cout << " " << message << endl;
		exit(1);
	}

public:
	/* Initialisation */
	Matrix(int, int);
	~Matrix();
	void initialise(int, int);
	void remove();

	/* Setters and Getters */
	void setRe(int, int, T);
	void setIm(int, int, T);
	void set(int, int, T, T);
	void set(int, int, Complex<T>);

	void setAllRe(T);
	void setAllIm(T);
	void setAll(T, T);
	void setAll(Complex<T>);

	Entry<T> get(int, int);

	/* Matrix Manipulation */
	void setToI();
	Matrix<T> transpose();

	/* Arithmetic Operations */
	void operator = (Matrix);
	bool operator == (Matrix);
	bool operator != (Matrix);

	Matrix<T> operator + (Matrix);
	Matrix<T> operator - (Matrix);
	void operator += (Matrix);
	void operator -= (Matrix);

	Matrix<T> operator * (Complex<T>);
	Matrix<T> operator * (Matrix);
	void operator *= (Complex<T>);
	void operator *= (Matrix);

	Matrix<T> operator / (Complex<T>);
	void operator /= (Complex<T>);

	/* Utilities */
	int rows();
	int cols();
	void copy(Matrix);
	Complex<T>** ptr();

	/* Debugging */
	void printRe();
	void print();

};

/* Initialisation */

template<class T>
Matrix<T>::Matrix(int rows, int cols)
{
	initialise(rows, cols);
}

template<class T>
Matrix<T>::~Matrix()
{

}

template<class T>
void Matrix<T>::initialise(int rows, int cols)
{
	ROWS = rows;
	COLS = cols;

	matrix = new Complex<T>*[ROWS];

	for(int i=0; i<ROWS; ++i)
		matrix[i] = new Complex<T>[COLS];
}

template<class T>
void Matrix<T>::remove()
{
	for(int i=0; i<ROWS; ++i)
		delete[] matrix[i];

	delete[] matrix;
}

/* Setters and Getters */

template<class T>
void Matrix<T>::setRe(int row, int col, T value)
{
	if(!isInBoundary(row, col))
		barf("setRe", "entry out of boundary");

	matrix[row][col].setRe(value);
}

template<class T>
void Matrix<T>::setIm(int row, int col, T value)
{
	if(!isInBoundary(row, col))
		barf("setIm", "entry out of boundary");

	matrix[row][col].setIm(value);
}

template<class T>
void Matrix<T>::set(int row, int col, T re, T im)
{
	if(!isInBoundary(row, col))
		barf("set(T, T)", "entry out of boundary");

	matrix[row][col].set(re, im);
}

template<class T>
void Matrix<T>::set(int row, int col, Complex<T> c)
{
	if(!isInBoundary(row, col))
		barf("set(Complex<T>)", "entry out of boundary");

	matrix[row][col] = c;
}

template<class T>
void Matrix<T>::setAllRe(T value)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix[i][j].setRe(value);
}

template<class T>
void Matrix<T>::setAllIm(T value)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix[i][j].setIm(value);
}

template<class T>
void Matrix<T>::setAll(T re, T im)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix[i][j].set(re, im);
}

template<class T>
void Matrix<T>::setAll(Complex<T> c)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix[i][j].set(c.getRe(), c.getIm());
}

template<class T>
Entry<T> Matrix<T>::get(int row, int col)
{
	if(!isInBoundary(row, col))
		barf("get", "entry out of boundary");

	return matrix[row][col];
}

/* Matrix Manipulation */

template<class T>
void Matrix<T>::setToI()
{
	if(rows() != cols())
		barf("setToI", "invalid matrix dimensions");

	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			set(i, j, (i == j)? 1 : 0, 0);
}

template<class T>
Matrix<T> Matrix<T>::transpose()
{
	Matrix<T> result(cols(), rows());

	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			result.set(j, i, matrix[i][j]);

	return result;
}

/* Arithmetic Operations */

template<class T>
void Matrix<T>::operator = (Matrix m)
{
	if(rows() != m.rows() || cols() != cols())
	{
		remove();
		initialise(m.rows(), m.cols());
	}

	copy(m);
	m.remove();
}

template<class T>
bool Matrix<T>::operator == (Matrix m)
{
	if(rows() != m.rows() || cols() != m.cols())
		return false;

	for(int i=0; i<rows(); ++i)
	{
		for(int j=0; j<cols(); ++j)
		{
			if(get(i, j) != m.get(i, j))
				return false;
		}
	}

	return true;
}

template<class T>
bool Matrix<T>::operator != (Matrix m)
{
	if(rows() != m.rows() || cols() != m.cols())
		return true;

	for(int i=0; i<rows(); ++i)
	{
		for(int j=0; j<cols(); ++j)
		{
			if(get(i, j) != m.get(i, j))
				return true;
		}
	}

	return false;
}


template<class T>
Matrix<T> Matrix<T>::operator + (Matrix m)
{
	Matrix<T> result(rows(), cols());

	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			result.set(i, j, get(i, j) + m.get(i, j));

	return result;
}

template<class T>
Matrix<T> Matrix<T>::operator - (Matrix m)
{
	Matrix<T> result(rows(), cols());

	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			result.set(i, j, get(i, j) - m.get(i, j));

	return result;
}

template<class T>
void Matrix<T>::operator += (Matrix m)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix[i][j] += m.get(i, j);
}

template<class T>
void Matrix<T>::operator -= (Matrix m)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix[i][j] -= m.get(i, j);
}


template<class T>
Matrix<T> Matrix<T>::operator * (Complex<T> c)
{
	Matrix<T> result(rows(), cols());

	for(int i=0; i<result.rows(); ++i)
		for(int j=0; j<result.cols(); ++j)
			result.set(i, j, matrix[i][j] * c);

	return result;
}

template<class T>
Matrix<T> Matrix<T>::operator * (Matrix m)
{
	if(cols() != m.rows())
		barf("operator *", "matrix dimensions do not match");

	Matrix<T> result(rows(), m.cols());

	for(int i=0; i<result.rows(); ++i)
	{
		for(int j=0; j<result.cols(); ++j)
		{
			Complex<T> sum(0, 0);

			for(int k=0; k<cols(); ++k)
				sum += get(i, k) * m.get(k, j);

			result.set(i, j, sum);
		}
	}

	return result;
}

template<class T>
void Matrix<T>::operator *= (Complex<T> c)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix[i][j] *= c;
}

template<class T>
void Matrix<T>::operator *= (Matrix m)
{
	if(cols() != m.rows())
		barf("operator *", "matrix dimensions do not match");

	Matrix<T> result(rows(), m.cols());

	for(int i=0; i<result.rows(); ++i)
	{
		for(int j=0; j<result.cols(); ++j)
		{
			Complex<T> sum(0, 0);

			for(int k=0; k<cols(); ++k)
				sum += get(i, k) * m.get(k, j);

			result.set(i, j, sum);
		}
	}

	copy(result);
	result.remove();
}


template<class T>
Matrix<T> Matrix<T>::operator / (Complex<T> c)
{
	Matrix<T> result(rows(), cols());

	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			result.set(i, j, matrix[i][j] / c);

	return result;
}

template<class T>
void Matrix<T>::operator /= (Complex<T> c)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix[i][j] /= c;
}

/* Utilities */

template<class T>
int Matrix<T>::rows()
{
	return ROWS;
}

template<class T>
int Matrix<T>::cols()
{
	return COLS;
}

template<class T>
void Matrix<T>::copy(Matrix m)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix[i][j] = m.get(i, j);
}

template<class T>
Complex<T>** Matrix<T>::ptr()
{
	return matrix;
}

/* Debugging */

template<class T>
void Matrix<T>::printRe()
{
	for(int i=0; i<rows(); ++i)
	{
		cout << ((i != 0)? " " : ":");

		for(int j=0; j<cols(); ++j)
			printf("%3.0f ", get(i, j).getRe());

		cout << endl;
	}

	cout << endl;
}

template<class T>
void Matrix<T>::print()
{
	for(int i=0; i<rows(); ++i)
	{
		for(int j=0; j<cols(); ++j)
		{
			matrix[i][j].print();
			cout << "\t";
		}

		cout << endl;
	}

	cout << endl;
}

#endif

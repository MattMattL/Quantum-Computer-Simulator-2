#ifndef QUMULATOR_MATRIX_HPP
#define QUMULATOR_MATRIX_HPP

#include <vector>
#include <memory.h>
#include "complex.hpp"

template<class T>
class Matrix
{
private:
	vector<vector<Complex<T> > > matrix;

	bool isInBoundary(int row, int col)
	{
		return (0 <= row && row < rows()) && (0 <= col && col < cols());
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

	/* Setters and Getters */
	void setRe(int, int, T);
	void setIm(int, int, T);
	void set(int, int, T, T);
	void set(int, int, Complex<T>);

	void setAllRe(T);
	void setAllIm(T);
	void setAll(T, T);
	void setAll(Complex<T>);

	Complex<T> get(int, int);

	/* Matrix Manipulation */
	void setToI();
	void transpose();
	void conjugate();
	void dagger();

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

	Matrix<T> tensor(Matrix<T>);

	// /* Utilities */
	int rows();
	int cols();
	void copy(Matrix);
	Complex<T>* ptr();

	// /* Debugging */
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
	matrix.clear();

	vector<Complex<T> > singleColumn;
	Complex<T> c(0, 0);

	for(int j=0; j<cols; ++j)
		singleColumn.push_back(c);

	for(int i=0; i<rows; ++i)
		matrix.push_back(singleColumn);
}

/* Setters and Getters */

template<class T>
void Matrix<T>::setRe(int row, int col, T value)
{
	if(!isInBoundary(row, col))
		barf("setRe", "entry out of boundary");

	matrix.at(row).at(col).setRe(value);
}

template<class T>
void Matrix<T>::setIm(int row, int col, T value)
{
	if(!isInBoundary(row, col))
		barf("setIm", "entry out of boundary");

	matrix.at(row).at(col).setIm(value);
}

template<class T>
void Matrix<T>::set(int row, int col, T re, T im)
{
	if(!isInBoundary(row, col))
		barf("set(T, T)", "entry out of boundary");

	matrix.at(row).at(col).set(re, im);
}

template<class T>
void Matrix<T>::set(int row, int col, Complex<T> c)
{
	if(!isInBoundary(row, col))
		barf("set(Complex<T>)", "entry out of boundary");

	matrix.at(row).at(col) = c;
}

template<class T>
void Matrix<T>::setAllRe(T value)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix.at(i).at(j).setRe(value);
}

template<class T>
void Matrix<T>::setAllIm(T value)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix.at(i).at(j).setIm(value);
}

template<class T>
void Matrix<T>::setAll(T re, T im)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix.at(i).at(j).set(re, im);
}

template<class T>
void Matrix<T>::setAll(Complex<T> c)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix.at(i).at(j).set(c.getRe(), c.getIm());
}

template<class T>
Complex<T> Matrix<T>::get(int row, int col)
{
	if(!isInBoundary(row, col))
		barf("get", "entry out of boundary");

	return matrix.at(row).at(col);
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
void Matrix<T>::transpose()
{
	for(int i=0; i<rows(); ++i)
	{
		for(int j=i+1; j<cols(); ++j)
		{
			Complex<T> c;

			c = get(i, j);
			set(i, j, get(j, i));
			set(j, i, c);
		}
	}
}

template<class T>
void Matrix<T>::conjugate()
{
	for(int i=0; i<rows(); ++i)
	{
		for(int j=0; j<cols(); ++j)
		{
			Complex<T> c;

			c = get(i, j);
			set(i, j, c.getRe(), -c.getIm());
		}
	}
}

template<class T>
void Matrix<T>::dagger()
{
	transpose();
	conjugate();
}

/* Arithmetic Operations */

template<class T>
void Matrix<T>::operator = (Matrix m)
{
	if(rows() != m.rows() || cols() != m.cols())
		initialise(m.rows(), m.cols());

	copy(m);
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
			matrix.at(i).at(j) += m.get(i, j);
}

template<class T>
void Matrix<T>::operator -= (Matrix m)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix.at(i).at(j) -= m.get(i, j);
}


template<class T>
Matrix<T> Matrix<T>::operator * (Complex<T> c)
{
	Matrix<T> result(rows(), cols());

	for(int i=0; i<result.rows(); ++i)
		for(int j=0; j<result.cols(); ++j)
			result.set(i, j, matrix.at(i).at(j) * c);

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
			matrix.at(i).at(j) *= c;
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
}


template<class T>
Matrix<T> Matrix<T>::operator / (Complex<T> c)
{
	Matrix<T> result(rows(), cols());

	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			result.set(i, j, matrix.at(i).at(j) / c);

	return result;
}

template<class T>
void Matrix<T>::operator /= (Complex<T> c)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix.at(i).at(j) /= c;
}

template<class T>
Matrix<T> Matrix<T>::tensor(Matrix<T> m)
{
	Matrix<T> result(rows() * m.rows(), cols() * m.cols());

	for(int i=0; i<result.rows(); ++i)
	{
		for(int j=0; j<result.cols(); ++j)
		{
			result.set(i, j, 
				get(i / m.rows(), j / m.cols()) * m.get(i % m.rows(), j % m.cols()));
		}
	}

	return result;
}

/* Utilities */

template<class T>
int Matrix<T>::rows()
{
	return matrix.size();
}

template<class T>
int Matrix<T>::cols()
{
	return (matrix.size() > 0)? matrix.at(0).size() : 0;
}

template<class T>
void Matrix<T>::copy(Matrix m)
{
	for(int i=0; i<rows(); ++i)
		for(int j=0; j<cols(); ++j)
			matrix.at(i).at(j) = m.get(i, j);
}

template<class T>
Complex<T>* Matrix<T>::ptr()
{
	return &matrix.at(0).at(0);
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
		cout << ((i != 0)? "  " : ": ");

		for(int j=0; j<cols(); ++j)
		{
			if(matrix.at(i).at(j).getIm() >= 0)
				printf("%6.3f +%6.3fi  ", matrix.at(i).at(j).getRe(), matrix.at(i).at(j).getIm());
			else
				printf("%6.3f %6.3fi  ", matrix.at(i).at(j).getRe(), matrix.at(i).at(j).getIm());
		}

		cout << endl;
	}

	cout << endl;
}

#endif

#ifndef HPP_MATRIX_DEFINED
#define HPP_MATRIX_DEFINED

#define Entry Complex

#include "complex.hpp"

template<class T>
class Matrix
{
private:
	Entry<T> **matrix;
	int ROWS, COLS;

	void barf(string function, string message)
	{
		cout << "[error] " << "<" << function << ">";
		cout << " " << message << endl;
		exit(1);
	}

public:
	/* Constructor and Deconstructor */
	Matrix(int, int);
	~Matrix();
	void remove();

	/* Setters and Getters */
	void set(int, int, Entry<T>);
	void set(int, int, T, T);
	Entry<T> get(int, int);

	void setToI();
	void setTo(T);
	void setTo(T, T);

	/* Arithmetic Operations */
	Matrix<T> operator + (Matrix);
	Matrix<T> operator - (Matrix);
	Matrix<T> operator * (Complex<T>);
	Matrix<T> operator * (Matrix);

	/* Utilities */
	int rows();
	int cols();
	void print();
};

/* Constructor and Deconstructor */

template <class T>
Matrix<T>::Matrix(int rows, int cols) : ROWS(rows), COLS(cols)
{
	matrix = new Entry<T>*[ROWS];

	for(int i=0; i<ROWS; i++)
		matrix[i] = new Entry<T>[COLS];
}

template <class T>
Matrix<T>::~Matrix()
{
	// This deleting method causes memory issues,
	// use seperate ::remove function
	
	// for(int i=0; i<ROWS; i++)
		// delete[] matrix[i];

	// delete[] matrix;
}

template <class T>
void Matrix<T>::remove()
{
	for(int i=0; i<ROWS; i++)
		delete[] matrix[i];

	delete[] matrix;
}

/* Setters and Getters */

template <class T>
void Matrix<T>::set(int row, int col, Entry<T> value)
{
	matrix[row][col] = value;
}

template <class T>
void Matrix<T>::set(int row, int col, T re, T im)
{
	matrix[row][col].set(re, im);
}

template <class T>
Entry<T> Matrix<T>::get(int row, int col)
{
	return matrix[row][col];
}

template <class T>
void Matrix<T>::setToI()
{
	if(rows() != cols())
		barf("setToI", "invalid matrix dimensions");

	for(int i=0; i<rows(); i++)
		for(int j=0; j<cols(); j++)
			set(i, j, (i == j)? 1 : 0, 0);
}

template <class T>
void Matrix<T>::setTo(T value)
{
	setTo(value, 0);
}

template <class T>
void Matrix<T>::setTo(T re, T im)
{
	for(int i=0; i<rows(); i++)
		for(int j=0; j<cols(); j++)
			matrix[i][j].set(re, im);
}

/* Arithmetic Operations */

template <class T>
Matrix<T> Matrix<T>::operator + (Matrix m)
{
	Matrix<T> result(rows(), cols());

	for(int i=0; i<rows(); i++)
		for(int j=0; j<cols(); j++)
			result.set(i, j, get(i, j) + m.get(i, j));

	return result;
}

template <class T>
Matrix<T> Matrix<T>::operator - (Matrix m)
{
	Matrix<T> result(rows(), cols());

	for(int i=0; i<rows(); i++)
		for(int j=0; j<cols(); j++)
			result.set(i, j, get(i, j) - m.get(i, j));

	return result;
}

// template <class T>
// Matrix<T> Matrix<T>::operator * (Complex<T> c)
// {

// }

template <class T>
Matrix<T> Matrix<T>::operator * (Matrix m)
{
	if(cols() != m.rows())
		barf("operator *", "matrix dimensions do not match");

	Matrix<T> result(rows(), m.cols());

	for(int i=0; i<result.rows(); i++)
	{
		for(int j=0; j<result.cols(); j++)
		{
			Entry<T> sum(0, 0);

			for(int k=0; k<cols(); k++)
				sum += get(i, k) * m.get(k, j);

			result.set(i, j, sum);
		}
	}

	return result;
}

/* Utilities */

template <class T>
int Matrix<T>::rows()
{
	return ROWS;
}

template <class T>
int Matrix<T>::cols()
{
	return COLS;
}

template <class T>
void Matrix<T>::print()
{
	for(int i=0; i<rows(); i++)
	{
		for(int j=0; j<cols(); j++)
			printf("%2.0f ", get(i, j).getRe());

		cout << endl;
	}
}

#endif
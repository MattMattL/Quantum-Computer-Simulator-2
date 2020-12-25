#ifndef QUMULATOR_COMPLEX_HPP
#define QUMULATOR_COMPLEX_HPP

#include <string>
#include <cmath>

using namespace std;

template<class T>
class Complex
{
private:
	T real;
	T imaginary;

public:
	/* Constructor and Deconstructor */
	Complex();
	Complex(T, T);
	~Complex();

	/* Setters and Getters */
	void setRe(T);
	void setIm(T);
	void set(T, T);
	T getRe();
	T getIm();

	/* Arithmetic Operators */
	bool operator == (Complex);
	bool operator != (Complex);

	Complex<T> operator + (Complex);
	Complex<T> operator - (Complex);
	void operator += (Complex);
	void operator -= (Complex);

	Complex<T> operator * (T);
	Complex<T> operator * (Complex);
	void operator *= (T);
	void operator *= (Complex);

	Complex<T> operator / (T);
	Complex<T> operator / (Complex);
	void operator /= (T);
	void operator /= (Complex);	

	/* Mathematical Functions */
	T norm();
	T normSq();

	/*Utilities*/
	void print();
	string toString();
};

/* Constructor and Deconstructor */

template<class T>
Complex<T>::Complex()
{
	setRe(0);
	setIm(0);
}

template<class T>
Complex<T>::Complex(T realIn, T imaginaryIn)
{
	setRe(realIn);
	setIm(imaginaryIn);
}

template<class T>
Complex<T>::~Complex()
{

}

/* Setters and Getters */

template<class T>
void Complex<T>::setRe(T realIn)
{
	real = realIn;
}

template<class T>
void Complex<T>::setIm(T imaginaryIn)
{
	imaginary = imaginaryIn;
}

template<class T>
void Complex<T>::set(T realIn, T imaginaryIn)
{
	real = realIn;
	imaginary = imaginaryIn;
}

template<class T>
T Complex<T>::getRe()
{
	return real;
}

template<class T>
T Complex<T>::getIm()
{
	return imaginary;
}

/* Arithmetic Operations */

template<class T>
bool Complex<T>::operator == (Complex c)
{
	return (getRe() == c.getRe()) && (getIm() == c.getIm());
}

template<class T>
bool Complex<T>::operator != (Complex c)
{
	return (getRe() != c.getRe()) || (getIm() != c.getIm());
}

template<class T>
Complex<T> Complex<T>::operator + (Complex c)
{
	Complex result(getRe() + c.getRe(), getIm() + c.getIm());
	
	return result;
}

template<class T>
Complex<T> Complex<T>::operator - (Complex c)
{
	Complex result(getRe() - c.getRe(), getIm() - c.getIm());
	
	return result;
}

template<class T>
void Complex<T>::operator += (Complex c)
{
	setRe(getRe() + c.getRe());
	setIm(getIm() + c.getIm());
}

template<class T>
void Complex<T>::operator -= (Complex c)
{
	setRe(getRe() - c.getRe());
	setIm(getIm() - c.getIm());
}


template<class T>
Complex<T> Complex<T>::operator * (T factor)
{
	Complex result(factor * getRe(), factor * getIm());

	return result;
}

template<class T>
Complex<T> Complex<T>::operator * (Complex c)
{
	Complex result;

	result.setRe(getRe() * c.getRe() - getIm() * c.getIm());
	result.setIm(getRe() * c.getIm() + getIm() * c.getRe());

	return result;
}

template<class T>
void Complex<T>::operator *= (T factor)
{
	setRe(factor * getRe());
	setIm(factor * getIm());
}

template<class T>
void Complex<T>::operator *= (Complex c)
{
	T newRe = getRe() * c.getRe() - getIm() * c.getIm();
	T newIm = getRe() * c.getIm() + getIm() * c.getRe();

	setRe(newRe);
	setIm(newIm);
}


template<class T>
Complex<T> Complex<T>::operator / (T factor)
{
	Complex result(factor / getRe(), factor / getIm());

	return result;
}

template<class T>
Complex<T> Complex<T>::operator / (Complex c)
{
	Complex result;

	T factor = c.getRe() * c.getRe() + c.getIm() * c.getIm();
	result.setRe((getRe() * c.getRe() + getIm() * c.getIm()) / factor);
	result.setIm((getIm() * c.getRe() - getRe() * c.getIm()) / factor);

	return result;
}

template<class T>
void Complex<T>::operator /= (T factor)
{
	setRe(getRe() / factor);
	setIm(getIm() / factor);
}

template<class T>
void Complex<T>::operator /= (Complex c)
{
	Complex result;

	T factor = c.getRe() * c.getRe() + c.getIm() * c.getIm();
	T newRe = (getRe() * c.getRe() + getIm() * c.getIm()) / factor;
	T newIm = (getIm() * c.getRe() - getRe() * c.getIm()) / factor;

	setRe(newRe);
	setIm(newIm);
}

/* Mathematical Functions */

template<class T>
T Complex<T>::norm()
{
	return sqrt(getRe() * getRe() + getIm() * getIm());
}

template<class T>
T Complex<T>::normSq()
{
	return getRe() * getRe() + getIm() * getIm();
}

/* Utilities */

template<class T>
void Complex<T>::print()
{
	if(real < 0)
		cout << "-";

	cout << abs(real);

	if(imaginary < 0)
		cout << "-";
	else
		cout << "+";

	cout << abs(imaginary) << "i";
}

#endif
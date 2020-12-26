#ifndef QUMULATOR_QUBITS_HPP
#define QUMULATOR_QUBITS_HPP

#include "complex.hpp"
#include "matrix.hpp"

template<class T>
class Qubits
{
private:
	Complex<T> *states;

	unsigned int numQubits;
	unsigned int numOfCoeffs;

public:
	Qubits(int);
	~Qubits();
};

template<class T>
Qubits<T>::Qubits(int qubits)
{
	numQubits = qubits;
	numOfCoeffs = 1 << numQubits;

	states = new Complex<T>[numOfCoeffs];
}

template<class T>
Qubits<T>::~Qubits()
{
	delete[] states;
}

#endif

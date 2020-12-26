#ifndef QUMULATOR_QUBITS_HPP
#define QUMULATOR_QUBITS_HPP

#include "complex.hpp"
#include "matrix.hpp"
#include "quantum_gates.hpp"

template<class T>
class Qubits
{
private:
	Complex<T> *states;
	unsigned int numQubits;
	unsigned int numOfCoeffs;

	QuantumGates<double> gate;

public:
	/* Constructor and Deconstructor */
	Qubits(int);
	~Qubits();
};

/* Constructor and Deconstructor */

template<class T>
Qubits<T>::Qubits(int qubits)
{
	numQubits = qubits;
	numOfCoeffs = 1 << numQubits;

	states = new Complex<T>[numOfCoeffs];

	for(int i=0; i<numOfCoeffs; i++)
		states[i].set(1, 0);
}

template<class T>
Qubits<T>::~Qubits()
{
	delete[] states;
}

#endif

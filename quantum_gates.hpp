#ifndef QUMULATOR_QUAMTUM_GATES_HPP
#define QUMULATOR_QUAMTUM_GATES_HPP

#include "complex.hpp"
#include "matrix.hpp"

template<class T>
class QuantumGates
{
private:
	void initialise();

	Matrix<T> *hadamard;
	Matrix<T> *cnot;

public:
	QuantumGates();
	~QuantumGates();

	Matrix<T> Hadamard() { return *hadamard; }
	Matrix<T> CNOT() { return *cnot; }

};

template<class T>
QuantumGates<T>::QuantumGates()
{
	initialise();
}

template<class T>
QuantumGates<T>::~QuantumGates()
{
	delete hadamard;
	delete cnot;
}

template<class T>
void QuantumGates<T>::initialise()
{
	Complex<T> c0(0, 0), c1(1, 0), cn1(-1, 0), ci(0, 1), cni(0, -1);
	Complex<T> cSqrt2(sqrt(2), 0);

	// Hadamard
	hadamard = new Matrix<T>(2, 2);

	hadamard->set(0, 0, c1);
	hadamard->set(0, 1, c1);
	hadamard->set(1, 0, c1);
	hadamard->set(1, 1, c1 * (-1));
	*hadamard /= cSqrt2;

	// CNOT
	cnot = new Matrix<T>(4, 4);

	cnot->set(0, 0, c1);
	cnot->set(1, 1, c1);
	cnot->set(2, 3, c1);
	cnot->set(3, 2, c1);
}

#endif
#ifndef QUMULATOR_QUAMTUM_GATES_HPP
#define QUMULATOR_QUAMTUM_GATES_HPP

#include <cmath>
#include "complex.hpp"
#include "matrix.hpp"

template<class T>
class QuantumGates
{
private:
	void initialise();

	Matrix<T> *identity;
	Matrix<T> *hadamard;
	Matrix<T> *pauli_x;
	Matrix<T> *pauli_y;
	Matrix<T> *pauli_z;
	Matrix<T> *phase;
	Matrix<T> *cnot;

public:
	QuantumGates();
	~QuantumGates();

	/* Single Qubit Operations */
	Matrix<T> Identity() { return *identity; }
	Matrix<T> Identity(int);

	Matrix<T> Hadamard() { return *hadamard; }
	Matrix<T> Hadamard(int);

	Matrix<T> Pauli_X() { return *pauli_x; }
	Matrix<T> Pauli_Y() { return *pauli_y; }
	Matrix<T> Pauli_Z() { return *pauli_z; }

	Matrix<T> Phase() { return *phase; }

	/* Multi-Qubit Operations */
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
	delete pauli_x;
	delete pauli_y;
	delete pauli_z;
}

template<class T>
void QuantumGates<T>::initialise()
{
	Complex<T> c0(0, 0), c1(1, 0), cn1(-1, 0), ci(0, 1), cni(0, -1);
	Complex<T> cSqrt2(sqrt(2), 0);

	// Identity
	identity = new Matrix<T>(2, 2);
	identity->setToI();

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

	// Pauli-X, Y, Z
	pauli_x = new Matrix<T>(2, 2);
	pauli_x->set(0, 1, c1);
	pauli_x->set(1, 0, c1);

	pauli_y = new Matrix<T>(2, 2);
	pauli_y->set(0, 1, cni);
	pauli_y->set(1, 0, ci);

	pauli_z = new Matrix<T>(2, 2);
	pauli_z->set(0, 0, c1);
	pauli_z->set(1, 1, cn1);

	// Phase
	phase = new Matrix<T>(2, 2);
	phase->set(0, 0, c1);
	phase->set(1, 1, ci);
}

template<class T>
Matrix<T> QuantumGates<T>::Identity(int dimension)
{
	Matrix<T> m(dimension, dimension);
	m.setToI();

	return m;
}

template<class T>
Matrix<T> QuantumGates<T>::Hadamard(int dimension)
{
	Complex<T> cSqrt2(sqrt(2), 0);

	if(dimension > 2)
		return Hadamard(dimension / 2).tensor(Hadamard()) / cSqrt2;
	else
		return Hadamard();
}

#endif
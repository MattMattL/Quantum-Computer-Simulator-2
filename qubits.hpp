#ifndef QUMULATOR_QUBITS_HPP
#define QUMULATOR_QUBITS_HPP

#include "complex.hpp"
#include "matrix.hpp"
#include "quantum_gates.hpp"

template<class T>
class Qubits
{
private:
	unsigned int numQubits;
	unsigned int numOfCoeffs;

	QuantumGates<T> gate;

public:
	Matrix<T> *states;

	/* Constructor and Deconstructor */
	Qubits(int);
	~Qubits();

	/* Quantum Logic Gates */
	void H(int);
	void X(int);
	void Y(int);
	void Z(int);
	void CNOT(int, int);

	/* Utilities */
	unsigned int size();
	unsigned int length();
	void print();
};

/* Constructor and Deconstructor */

template<class T>
Qubits<T>::Qubits(int qubits)
{
	numQubits = qubits;
	numOfCoeffs = 1 << numQubits;

	states = new Matrix<T>(numOfCoeffs, 1);
	states->set(0, 0, 1, 0);
}

template<class T>
Qubits<T>::~Qubits()
{
	delete states;
}

/* Quantum Logic Gates */

template<class T>
void Qubits<T>::H(int qubit)
{
	Matrix<T> m(1, 1);
	m.setToI();

	for(int i=0; i<numQubits; i++)
		m = m.tensor((i == qubit)? gate.Hadamard() : gate.Identity());

	(*states) = m * (*states);
}

template<class T>
void Qubits<T>::X(int qubit)
{
	Matrix<T> m(1, 1);
	m.setToI();

	for(int i=0; i<numQubits; i++)
		m = m.tensor((i == qubit)? gate.Pauli_X() : gate.Identity());

	(*states) = m * (*states);
}

template<class T>
void Qubits<T>::Y(int qubit)
{
	Matrix<T> m(1, 1);
	m.setToI();

	for(int i=0; i<numQubits; i++)
		m = m.tensor((i == qubit)? gate.Pauli_Y() : gate.Identity());

	(*states) = m * (*states);
}

template<class T>
void Qubits<T>::Z(int qubit)
{
	Matrix<T> m(1, 1);
	m.setToI();

	for(int i=0; i<numQubits; i++)
		m = m.tensor((i == qubit)? gate.Pauli_Z() : gate.Identity());

	(*states) = m * (*states);
}

template<class T>
void Qubits<T>::CNOT(int control, int target)
{
	Matrix<T> m1(1, 1), m2(1, 1), m00(2, 2), m11(2, 2);

	m1.setToI();
	m2.setToI();
	m00.set(0, 0, 1, 0);
	m11.set(1, 1, 1, 0);

	for(int i=0; i<numQubits; i++)
		m1 = m1.tensor((i == control)? m00 : gate.Identity());

	for(int i=0; i<numQubits; i++)
	{
		if(i == control)
			m2 = m2.tensor(m11);
		else if(i == target)
			m2 = m2.tensor(gate.Pauli_X());
		else
			m2 = m2.tensor(gate.Identity());
	}

	(*states) = (m1 + m2) * (*states);
}

/* Utilities */

template<class T>
unsigned int Qubits<T>::size()
{
	// Reutrns the number of qubits the system contains.
	return numQubits;
}

template<class T>
unsigned int Qubits<T>::length()
{
	// Returns the number of coefficients representing the states.
	return numOfCoeffs;
}

template<class T>
void Qubits<T>::print()
{
	for(int i=0; i<numOfCoeffs; i++)
	{
		string decToBin;

		for(int j=0; j<numQubits; j++)
			decToBin.insert(decToBin.begin(), (i >> j & 1) + '0');

		printf("|%s⟩", decToBin.c_str());
		printf(" = %6.3f +%6.3fi", states->get(i, 0).getRe(), states->get(i, 0).getIm());
		printf("  (%.3f)\n", states->get(i, 0).normSq());	
	}

	cout << endl;
}

#endif

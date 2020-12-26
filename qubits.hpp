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
	Complex<T> *states;

	/* Constructor and Deconstructor */
	Qubits(int);
	~Qubits();

	/* Quantum Logic Gates */
	void H(int);

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

	states = new Complex<T>[numOfCoeffs];
	states[0].set(1, 0);
}

template<class T>
Qubits<T>::~Qubits()
{
	delete[] states;
}

/* Quantum Logic Gates */

template<class T>
void Qubits<T>::H(int qubit)
{
	if(qubit < 0 || qubit >= numQubits)
		return;

	Matrix<T> m(2, 1);
	int j;

	for(int i=0; i<numOfCoeffs; i++)
	{
		j = i + (1 << qubit);

		if(!(i >> qubit & 1) && j < numOfCoeffs)
		{
			m.set(0, 0, states[i]);
			m.set(1, 0, states[j]);

			m = gate.Hadamard() * m;

			states[i] = m.get(0, 0);
			states[j] = m.get(1, 0);
		}
	}

	m.remove();
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
	// Returns the number of coefficients representing the qubits.
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

		cout << "|" << decToBin << ">";

		printf("  %.3f", states[i].normSq());
		printf("  (%5.2f +%5.2fi)\n", states[i].getRe(), states[i].getIm());
	}

	cout << endl;
}

#endif

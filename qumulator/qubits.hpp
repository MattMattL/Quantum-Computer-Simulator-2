#ifndef QUMULATOR_QUBITS_HPP
#define QUMULATOR_QUBITS_HPP

#include <stdio.h>
#include <queue>
#include <ctime>
#include "complex.hpp"
#include "matrix.hpp"
#include "quantum_gates.hpp"
#include "qumulator_graphics.hpp"

template<class T>
class Qubits
{
private:
	unsigned int numQubits;
	unsigned int numOfCoeffs;

	int measurement;
	priority_queue<int, vector<int>, greater<int> > measured;

	QuantumGates<T> gate;

public:
	Matrix<T> *states;

	/* Constructor and Deconstructor */
	Qubits(int);
	Qubits(int, unsigned int);
	~Qubits();

	/* Quantum Logic Gates */
	void H(int);
	void X(int);
	void Y(int);
	void Z(int);
	void Measure(int);

	Matrix<T> controlledU(int, int, Matrix<T>);
	void CNOT(int, int);
	void CY(int, int);
	void CZ(int, int);
	void Toffoli(int, int, int);

	void Swap(int, int);

	/* Utilities */
	unsigned int size();
	unsigned int length();

	void print();
	void save(string);

	/* Circuit Diagram */
	QumulatorGraphics graphics;
	bool enableGraphics;
};

/* Constructor and Deconstructor */

template<class T>
Qubits<T>::Qubits(int qubits)
{
	numQubits = qubits;
	numOfCoeffs = 1 << numQubits;

	graphics.setSize(numQubits);

	states = new Matrix<T>(numOfCoeffs, 1);
	states->set(0, 0, 1, 0);

	measurement = -1;
	enableGraphics = true;
}

template<class T>
Qubits<T>::~Qubits()
{
	delete states;
}

/* Single-Qubit Gates */

template<class T>
void Qubits<T>::H(int qubit)
{
	if(enableGraphics)
		graphics.add(qubit, 'H', graphics.MARK);

	Matrix<T> m(1, 1);
	m.setToI();

	for(int i=numQubits - 1; i>=0; --i)
		m = m.tensor((i == qubit)? gate.Hadamard() : gate.Identity());

	(*states) = m * (*states);
}

template<class T>
void Qubits<T>::X(int qubit)
{
	if(enableGraphics)
		graphics.add(qubit, 'X', graphics.MARK);

	Matrix<T> m(1, 1);
	m.setToI();

	for(int i=numQubits - 1; i>=0; --i)
		m = m.tensor((i == qubit)? gate.Pauli_X() : gate.Identity());

	(*states) = m * (*states);
}

template<class T>
void Qubits<T>::Y(int qubit)
{
	if(enableGraphics)
		graphics.add(qubit, 'Y', graphics.MARK);

	Matrix<T> m(1, 1);
	m.setToI();

	for(int i=numQubits - 1; i>=0; --i)
		m = m.tensor((i == qubit)? gate.Pauli_Y() : gate.Identity());

	(*states) = m * (*states);
}

template<class T>
void Qubits<T>::Z(int qubit)
{
	if(enableGraphics)
		graphics.add(qubit, 'Z', graphics.MARK);

	Matrix<T> m(1, 1);
	m.setToI();

	for(int i=numQubits - 1; i>=0; --i)
		m = m.tensor((i == qubit)? gate.Pauli_Z() : gate.Identity());

	(*states) = m * (*states);
}

template<class T>
void Qubits<T>::Measure(int qubit)
{
	/*
		Add a matrix-based measurement method later.
	*/

	if(enableGraphics)
		graphics.add(qubit, numQubits + qubit, 'M', 'V', '|', graphics.MARK_AND_FILL);

	if(measurement < 0)
	{
		srand(time(NULL));

		T probability = (T)(rand() % 10000) / 10000;
		T sum = 0;

		for(int n=0; n<numOfCoeffs; n++)
		{
			sum += states->get(n, 0).normSq();

			if(sum >= probability)
			{
				measurement = n;
				break;
			}
		}
	}

	measured.push(qubit);
}

/* Contol Gates */

template<class T>
Matrix<T> Qubits<T>::controlledU(int control, int target, Matrix<T> u)
{
	Matrix<T> m1(1, 1), m2(1, 1), m00(2, 2), m11(2, 2);

	m1.setToI();
	m2.setToI();
	m00.set(0, 0, 1, 0);
	m11.set(1, 1, 1, 0);

	for(int i=numQubits - 1; i>=0; --i)
		m1 = m1.tensor((i == control)? m00 : gate.Identity());

	for(int i=numQubits - 1; i>=0; --i)
	{
		if(i == control)
			m2 = m2.tensor(m11);
		else if(i == target)
			m2 = m2.tensor(u);
		else
			m2 = m2.tensor(gate.Identity());
	}

	return m1 + m2;
}

template<class T>
void Qubits<T>::CNOT(int control, int target)
{
	if(enableGraphics)
		graphics.add(control, target, '*', '@', '|', graphics.MARK_AND_FILL);

	(*states) = controlledU(control, target, gate.Pauli_X()) * (*states);
}

template<class T>
void Qubits<T>::CY(int control, int target)
{
	if(enableGraphics)
		graphics.add(control, target, '*', 'Y', '|', graphics.MARK_AND_FILL);

	(*states) = controlledU(control, target, gate.Pauli_Y()) * (*states);
}

template<class T>
void Qubits<T>::CZ(int control, int target)
{
	if(enableGraphics)
		graphics.add(control, target, '*', 'Z', '|', graphics.MARK_AND_FILL);

	(*states) = controlledU(control, target, gate.Pauli_Z()) * (*states);
}

template<class T>
void Qubits<T>::Toffoli(int control1, int control2, int target)
{
	if(enableGraphics)
	{
		vector<int> vecPos;
		vecPos.push_back(control1);
		vecPos.push_back(control2);
		vecPos.push_back(target);

		vector<char> vecGate;
		vecGate.push_back('*');
		vecGate.push_back('*');
		vecGate.push_back('@');
		vecGate.push_back('|');

		graphics.add(vecPos, vecGate, graphics.MARK_AND_FILL);
	}

	Matrix<T> m1(1, 1), m2(1, 1), m3(1, 1), m4(1, 1), m00(2, 2), m11(2, 2);

	m1.setToI();
	m2.setToI();
	m3.setToI();
	m4.setToI();
	m00.set(0, 0, 1, 0);
	m11.set(1, 1, 1, 0);

	for(int i=numQubits - 1; i>=0; i--)
	{
		if(i == control1)
		{
			m1 = m1.tensor(m00);
			m2 = m2.tensor(m00);
			m3 = m3.tensor(m11);
			m4 = m4.tensor(m11);
		}
		else if(i == control2)
		{
			m1 = m1.tensor(m00);
			m2 = m2.tensor(m11);
			m3 = m3.tensor(m00);
			m4 = m4.tensor(m11);
		}
		else if(i == target)
		{
			m1 = m1.tensor(gate.Identity());
			m2 = m2.tensor(gate.Identity());
			m3 = m3.tensor(gate.Identity());
			m4 = m4.tensor(gate.Pauli_X());
		}
		else
		{
			m1 = m1.tensor(gate.Identity());
			m2 = m2.tensor(gate.Identity());
			m3 = m3.tensor(gate.Identity());
			m4 = m4.tensor(gate.Identity());
		}
	}

	(*states) = (m1 + m2 + m3 + m4) * (*states);
}

/* Other Multi-Qubit Gates */

template<class T>
void Qubits<T>::Swap(int q1, int q2)
{
	if(enableGraphics)
		graphics.add(q1, q2, 'X', 'X', '|', graphics.MARK_AND_FILL);

	(*states) = controlledU(q1, q2, gate.Pauli_X()) *
				controlledU(q2, q1, gate.Pauli_X()) *
				controlledU(q1, q2, gate.Pauli_X()) *
				(*states);
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

	priority_queue<int, vector<int>, greater<int> > temp = measured;
	printf("\n");

	while(!temp.empty())
	{
		printf("Qubit%2d: %d\n", temp.top(), (measurement >> temp.top()) & 1);
		temp.pop();
	}

	printf("\n");
}

template<class T>
void Qubits<T>::save(string location)
{
	FILE *file;
	file = fopen(location.c_str(), "wt");

	for(int i=0; i<numOfCoeffs; i++)
	{
		string decToBin;

		for(int j=0; j<numQubits; j++)
			decToBin.insert(decToBin.begin(), (i >> j & 1) + '0');

		fprintf(file, "|%s⟩", decToBin.c_str());
		fprintf(file, " = %6.3f +%6.3fi", states->get(i, 0).getRe(), states->get(i, 0).getIm());
		fprintf(file, "  (%.3f)\n", states->get(i, 0).normSq());	
	}

	priority_queue<int, vector<int>, greater<int> > temp = measured;
	fprintf(file, "\n");

	while(!temp.empty())
	{
		fprintf(file, "Qubit%2d: %d\n", temp.top(), (measurement >> temp.top()) & 1);
		temp.pop();
	}

	fprintf(file, "\n");

	fclose(file);
}

#endif

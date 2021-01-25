#ifndef QUMULATOR_QUBITS_HPP
#define QUMULATOR_QUBITS_HPP

#include <stdio.h>
#include <queue>
#include <ctime>
#include "complex.hpp"
#include "matrix.hpp"
#include "quantum_gates.hpp"
#include "qumulator_graphics.hpp"

template<class Type>
class Qubits
{
private:
	unsigned int numQubits;
	unsigned int numCoeffs;

	int measurement;
	priority_queue<int, vector<int>, greater<int> > measured;

	QuantumGates<Type> gate;

public:
	Matrix<Type> *states;

	/* Constructor and Deconstructor */
	Qubits(int);
	Qubits(int, unsigned int);
	~Qubits();

	/* Quantum Logic Gates */
	void H(int);
	void X(int);
	void Y(int);
	void Z(int);
	void T(int);
	void S(int);
	void U(Matrix<Type>, int);
	unsigned int Measure(int);

	Matrix<Type> controlledU(int, int, Matrix<Type>);
	void CNOT(int, int);
	void CY(int, int);
	void CZ(int, int);
	void Toffoli(int, int, int);

	void Swap(int, int);

	/* Utilities */
	unsigned int size();
	unsigned int length();

	void setRandomSeed(int);

	void print();
	void save(string);

	/* Circuit Diagram */
	QumulatorGraphics graphics;
	bool enableGraphics;
};

/* Constructor and Deconstructor */

template<class Type>
Qubits<Type>::Qubits(int qubits)
{
	numQubits = qubits;
	numCoeffs = 1 << numQubits;

	graphics.initialise(numQubits);

	states = new Matrix<Type>(numCoeffs, 1);
	states->set(0, 0, 1, 0);

	measurement = -1;
	enableGraphics = true;

	srand(time(NULL));
}

template<class Type>
Qubits<Type>::~Qubits()
{
	delete states;
}

/* Single-Qubit Gates */

template<class Type>
void Qubits<Type>::H(int qubit)
{
	if(enableGraphics)
		graphics.add(qubit, "H", graphics.SINGLE_QUBIT);

	Matrix<Type> m(1, 1);
	m.setToI();

	for(int i=numQubits - 1; i>=0; --i)
		m = m.tensor((i == qubit)? gate.Hadamard() : gate.Identity());

	(*states) = m * (*states);
}

template<class Type>
void Qubits<Type>::X(int qubit)
{
	if(enableGraphics)
		graphics.add(qubit, "X", graphics.SINGLE_QUBIT);

	Matrix<Type> m(1, 1);
	m.setToI();

	for(int i=numQubits - 1; i>=0; --i)
		m = m.tensor((i == qubit)? gate.Pauli_X() : gate.Identity());

	(*states) = m * (*states);
}

template<class Type>
void Qubits<Type>::Y(int qubit)
{
	if(enableGraphics)
		graphics.add(qubit, "Y", graphics.SINGLE_QUBIT);

	Matrix<Type> m(1, 1);
	m.setToI();

	for(int i=numQubits - 1; i>=0; --i)
		m = m.tensor((i == qubit)? gate.Pauli_Y() : gate.Identity());

	(*states) = m * (*states);
}

template<class Type>
void Qubits<Type>::Z(int qubit)
{
	if(enableGraphics)
		graphics.add(qubit, "Z", graphics.SINGLE_QUBIT);

	Matrix<Type> m(1, 1);
	m.setToI();

	for(int i=numQubits - 1; i>=0; --i)
		m = m.tensor((i == qubit)? gate.Pauli_Z() : gate.Identity());

	(*states) = m * (*states);
}

template<class Type>
void Qubits<Type>::T(int qubit)
{
	if(enableGraphics)
		graphics.add(qubit, "T", graphics.SINGLE_QUBIT);

	Matrix<Type> m(1, 1);
	m.setToI();

	for(int i=numQubits - 1; i>=0; --i)
		m = m.tensor((i == qubit)? gate.PhaseShift(M_PI / 4) : gate.Identity());

	(*states) = m * (*states);
}

template<class Type>
void Qubits<Type>::S(int qubit)
{
	if(enableGraphics)
		graphics.add(qubit, "S", graphics.SINGLE_QUBIT);

	Matrix<Type> m(1, 1);
	m.setToI();

	for(int i=numQubits - 1; i>=0; --i)
		m = m.tensor((i == qubit)? gate.PhaseShift(M_PI / 2) : gate.Identity());

	(*states) = m * (*states);
}

template<class Type>
void Qubits<Type>::U(Matrix<Type> u, int qubit)
{
	if(enableGraphics)
		graphics.add(qubit, "U", graphics.SINGLE_QUBIT);

	Matrix<Type> m(1, 1);
	m.setToI();

	for(int i=numQubits - 1; i>=0; --i)
		m = m.tensor((i == qubit)? u : gate.Identity());

	(*states) = m * (*states);
}

template<class Type>
unsigned int Qubits<Type>::Measure(int qubit)
{
	if(enableGraphics)
		graphics.add(qubit, "M", graphics.MEASURE);

	// determine classical output
	Type probOfZero = 0;
	Type probability = (Type)(rand() % 10000) / 10000;
	unsigned int result;

	for(int i=0; i<numCoeffs; ++i)
	{
		if(((i >> qubit) & 1) == 0)
			probOfZero += states->get(i, 0).normSq();
	}

	result = (probability <= probOfZero)? 0 : 1;

	// remove unused states
	for(int i=0; i<numCoeffs; ++i)
	{
		if(((i >> qubit) & 1) != result)
			states->set(i, 0, 0, 0);
	}

	// normalise the coefficients
	Complex<Type> factor;

	if(result)
		factor.setRe(sqrt(1 - probOfZero));
	else
		factor.setRe(sqrt(probOfZero));

	for(int i=0; i<numCoeffs; ++i)
	{
		Complex<Type> coeff(states->get(i, 0).getRe(), states->get(i, 0).getIm());

		coeff /= factor;

		states->set(i, 0, coeff);
	}

	return result;
}

/* Contol Gates */

template<class Type>
Matrix<Type> Qubits<Type>::controlledU(int control, int target, Matrix<Type> u)
{
	Matrix<Type> m1(1, 1), m2(1, 1), m00(2, 2), m11(2, 2);

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

template<class Type>
void Qubits<Type>::CNOT(int control, int target)
{
	if(enableGraphics)
		graphics.add(control, target, "*", "@", graphics.TWO_QUBITS);

	(*states) = controlledU(control, target, gate.Pauli_X()) * (*states);
}

template<class Type>
void Qubits<Type>::CY(int control, int target)
{
	if(enableGraphics)
		graphics.add(control, target, "*", "Y", graphics.TWO_QUBITS);

	(*states) = controlledU(control, target, gate.Pauli_Y()) * (*states);
}

template<class Type>
void Qubits<Type>::CZ(int control, int target)
{
	if(enableGraphics)
		graphics.add(control, target, "*", "Z", graphics.TWO_QUBITS);

	(*states) = controlledU(control, target, gate.Pauli_Z()) * (*states);
}

template<class Type>
void Qubits<Type>::Toffoli(int control1, int control2, int target)
{
	if(enableGraphics)
	{
		vector<int> vecPos;
		vecPos.push_back(control1);
		vecPos.push_back(control2);
		vecPos.push_back(target);

		vector<string> vecGate;
		vecGate.push_back("*");
		vecGate.push_back("*");
		vecGate.push_back("@");
		vecGate.push_back("|");

		graphics.add(vecPos, vecGate, graphics.THREE_QUBITS);
	}

	Matrix<Type> m1(1, 1), m2(1, 1), m3(1, 1), m4(1, 1), m00(2, 2), m11(2, 2);

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

template<class Type>
void Qubits<Type>::Swap(int qubit1, int qubit2)
{
	if(enableGraphics)
		graphics.add(qubit1, qubit2, "x", "x", graphics.TWO_QUBITS);

	(*states) = controlledU(qubit1, qubit2, gate.Pauli_X()) *
				controlledU(qubit2, qubit1, gate.Pauli_X()) *
				controlledU(qubit1, qubit2, gate.Pauli_X()) *
				(*states);
}

/* Utilities */

template<class Type>
unsigned int Qubits<Type>::size()
{
	// Reutrns the number of qubits the system contains.
	return numQubits;
}

template<class Type>
unsigned int Qubits<Type>::length()
{
	// Returns the number of coefficients representing the states.
	return numCoeffs;
}

template<class Type>
void Qubits<Type>::setRandomSeed(int seed)
{
	srand(seed);
}

template<class Type>
void Qubits<Type>::print()
{
	for(int i=0; i<numCoeffs; ++i)
	{
		string decToBin;

		for(int j=0; j<numQubits; ++j)
			decToBin.insert(decToBin.begin(), (i >> j & 1) + '0');

		printf("|%s⟩", decToBin.c_str());

		if(states->get(i, 0).getRe() || states->get(i, 0).getIm())
			printf(" = %6.3f +%6.3fi", states->get(i, 0).getRe(), states->get(i, 0).getIm());
		else
			printf(" =  0             ");

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

template<class Type>
void Qubits<Type>::save(string location)
{
	FILE *file;
	file = fopen(location.c_str(), "wt");

	for(int i=0; i<numCoeffs; ++i)
	{
		string decToBin;

		for(int j=0; j<numQubits; ++j)
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

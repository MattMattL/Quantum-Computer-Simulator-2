#include <iostream>
#include "Qmulator.hpp"
#include "../timer.hpp"

#include <queue>


int main()
{
	Qubits<double> qubits(2);
	Matrix<double> m_I(2, 2), m_X(2, 2), m_Z(2, 2), m_iY(2, 2);
	QuantumGates<double> gate;

	m_I = gate.Identity();
	m_X = gate.Pauli_X();
	m_Z = gate.Pauli_Z();
	m_iY = m_X * m_Z;

	qubits.H(0);
	qubits.CNOT(0, 1);

	qubits.U(m_X, 0); // any of I, X, Z, iY

	qubits.CNOT(0, 1);
	qubits.H(0);

	qubits.Measure(0);
	qubits.Measure(1);

	qubits.print();
	qubits.graphics.draw();
	qubits.graphics.print();

	return 0;
}
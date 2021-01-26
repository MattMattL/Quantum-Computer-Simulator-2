/*
	Testing single and multi-qubit gates by implementing Toffoli gate
	using no more than double-qubit gates.
*/

#include <iostream>
#include <time.h>
#include "../../qumulator/Qumulator.hpp"

void toffoli(int q0, int q1, int q2, int seed)
{
	// initialisation
	Qubits<double> toffoli(3);
	toffoli.setRandomSeed(seed);

	if(q0) toffoli.X(0);
	if(q1) toffoli.X(1);
	if(q2) toffoli.X(2);

	toffoli.barrier();

	// Toffoli gate
	toffoli.Toffoli(0, 1, 2);

	// save result
	toffoli.save("./toffoli_result.txt");
	toffoli.graphics.draw();
	toffoli.graphics.save("./toffoli_gate.txt");
}

void simulatedToffoli(int q0, int q1, int q2, int seed)
{
	// initialisation
	Qubits<double> qubits(3);
	Matrix<double> t_dagger(2, 2);
	QuantumGates<double> gate;

	qubits.setRandomSeed(seed);

	t_dagger = gate.PhaseShift(M_PI / 4);
	t_dagger.dagger();

	if(q0) qubits.X(0);
	if(q1) qubits.X(1);
	if(q2) qubits.X(2);

	qubits.barrier();
	
	// simulated Toffoli gate
	qubits.H(2);
	qubits.CNOT(1, 2);
	qubits.U(t_dagger, 2);
	qubits.CNOT(0, 2);
	qubits.T(2);
	qubits.CNOT(1, 2);
	qubits.U(t_dagger, 2);
	qubits.CNOT(0, 2);
	qubits.T(1);
	qubits.T(2);
	qubits.H(2);
	qubits.CNOT(0, 1);
	qubits.T(0);
	qubits.U(t_dagger, 1);
	qubits.CNOT(0, 1);

	// save result
	qubits.save("./simulated_result.txt");
	qubits.graphics.draw();
	qubits.graphics.save("./simulated_gate.txt");
}

int main()
{
	// srand(time(NULL));

	int random_seed = 1234;
	int initial_setting = 4321 % 3;

	int q0 = initial_setting & 1;
	int q1 = (initial_setting >> 1) & 1;
	int q2 = (initial_setting >> 2) & 1;

	toffoli(q0, q1, q2, random_seed);
	simulatedToffoli(q0, q1, q2, random_seed);

	return 0;
}

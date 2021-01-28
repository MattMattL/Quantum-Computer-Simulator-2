/*
	Testing multi-qubit gates and the visualisation library by constructing
	a simple circuit - initialising and entangling five qubits.
*/

#include <iostream>
#include "../../qumulator/Qumulator.hpp"

int main()
{
	Qubits<double> qubits(5);

	qubits.H(0);
	qubits.CNOT(0, 1);
	qubits.CNOT(1, 2);
	qubits.CNOT(2, 3);
	qubits.CNOT(3, 4);

	qubits.Measure(0);
	qubits.Measure(1);
	qubits.Measure(2);
	qubits.Measure(3);
	qubits.Measure(4);

	qubits.save("result.txt");

	qubits.graphics.draw();
	qubits.graphics.save("circuit.txt");

	return 0;
}

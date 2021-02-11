# Qumulator V2

## About This Repository
This is the second version of my original quantum computer simulator, Qumulator (2019).

### Current Plan
I'm currently making various libraries to implement a working quantum computer simulator:

_Complex numbers -> Matrices -> Qubit -> Set of Qubits -> Quantum Logic Gates -> Quantum Algorithm_

### Progress
* Necessary single and multi-qubit gates implemented
* Able to measure individual qubits
* Graphics library automatically generates circuit diagrams


## Code

### Initialisation

'''C++
#include"Qumulator.hpp"

Qubits<double> qubits(3); // 3 qubits of type double
Qubits<float> qubit(1); // 1 qubit of type float
'''

### Quantum Logic Gates

'''C++
Qubits<double> qubits(3);

qubits.H(0); // Hadamard (on N-th qubit)

qubits.X(0); // Pauli-X,Y,Z
qubits.Y(0);
qubits.Z(0);

qubits.T(0); // Phase
qubits.S(0);

qubits.U(0); // arbitrary unitary matrix

qubits.CNOT(0, 1);
qubits.CY(0, 1);
qubits.CZ(0, 1);
qubits.Toffoli(0, 1, 2);

qubits.Swap(0, 1);
'''

### Level 3 Text
text text text
text text text

#### Level 4 Text
text text text
text text text

##### Level 5 Text
text text text
text text text

###### Level 6 Text
text text text
text text text

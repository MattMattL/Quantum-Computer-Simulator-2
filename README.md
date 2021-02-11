## Qumulator V2

### About This Repository
This is the second version of my original quantum computer simulator, Qumulator (2019).

### Current Plan
I'm currently writing more header files to implement a working quantum computer simulator:

Complex numbers -> Matrices -> Qubit -> Set of Qubits -> Quantum Logic Gates -> Quantum Algorithm

## Code

### Initialisation
```C++
#include "Qumulator.hpp"

Qubits<double> qubits(3); // 3 qubits of type double
Qubits<float> qubit(1); // 1 qubit of type float	
```

### Quantum Logic Gates
```C++
Qubits<double> qubits(3);

qubits.H(0); // Hadamard on first qubit

qubits.X(0);
qubits.Y(0);
qubits.Z(0);

qubits.T(0); // phase
qubits.S(0);

qubits.U(0); // arbitrary unitary matrix

qubits.CNOT(0, 1); // (control, target)
qubits.CY(0, 1);
qubits.CZ(0, 1);
qubits.Toffoli(0, 1, 2); // (control, control, target)

qubits.Swap(0, 1);

qubits.Measure(0);
qubits.Measure(1);
qubits.Measure(2);
```

### Visualisation Library
```C++
Qubits<double> qubits(2);

qubits.enableGraphics = true;

// ADD CODE HERE

qubits.margin(); // handling spacing
qubits.barrier();

qubits.graphics.draw(); // create diagram

qubits.graphics.print();
qubits.graphics.save("./name.txt");
```

### Matrices and Complex Numbers
```C++
Matrix<double> m1(2, 2); // initialise a 2 x 2 matrix and set the first entry to 1 - i
m1.set(0, 0, 1, -1);

Complex<double> c1(1, 2); // initialise c1 = 1 + 2i and copy to the last entry of m1
m1.set(1, 1, c);

Matrix<double> m2(2, 2); // arithmetic operations (+, -, *, /)
m2 = m1 * m2; 
m2 += m2;
c = c / 2;
c -= c;

m2 = m1.tensor(m2); // tensor product

m.transpose(); // matrix manipulations
m.conjugate();
m.dagger();
```

### Example Code
```C++
/*
    Implementing Super Dense Coding using Qumulator Library

    This code demonstrates how one qubit can "carry" two classical bit worth of
    information by entangling two qubits and changing the state of the first qubit.

    Measuring both qubits leaves the qubits in one of the four possible states
    depending on the gate applied to the first qubit.
 */

#include "Qumulator.hpp"

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
```

Result:
```
|00⟩ =  0               (0.000)
|01⟩ =  0               (0.000)
|10⟩ =  1.000 + 0.000i  (1.000)
|11⟩ =  0               (0.000)
```

Circuit Diagram:
```
──H──*──U──*──H──M═══
     │     │         
─────@─────@─────M═══
```

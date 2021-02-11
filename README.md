# Qumulator V2

## About This Repository
This is the second version of my original quantum computer simulator, Qumulator (2019).


I'm currently adding more libraries to implement a working quantum computer simulator:

_Complex numbers -> Matrices -> Qubit -> Set of Qubits -> Quantum Logic Gates -> Quantum Algorithm_


## Code

### Initialisation

```C++
#include "Qumulator.hpp"

Qubits<double> qubits(3); // 3 qubits of type double
Qubits<float> qubit(1); // 1 qubit of type float	
```

### Quantum Logic Gates

```C++
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

qubits.Measure(0);
qubits.Measure(1);
qubits.Measure(2);
```

### Visualisation Library

```C++
Qubits<double> qubits(2);

qubits.enableGraphics = true // enabled by default

// add gates here

qubits.margin(); // put extra space
qubits.barrier(); // draw a barrier

qubits.graphics.draw(); // create circuit diagram
qubits.graphics.print(); // print on screen
qubits.graphics.save("./name.txt"); // save as file
```

### Matrices and Complex Numbers

```C++
Matrix<double> m1(2, 2), m2(2, 2); // 2 x 2 matrices of type double
m1.set(0, 0, 1, -1); // set entry (0, 0) to 1 - i

Complex<double> c1(1, 2); // declaring 1 + 2i
m1.set(1, 1, c); // set entry (1, 1) to 1 + 2i

m2 = m1 * m2; // arithmetic operations (+, -, *, /)
m2 *= m2
c = 2 * c;
c *= c

m2 = m1.tensor(m2) // tensor product

m.transpose(); // matrix manipulations
m.conjugate();
m.dagger();
```

### Example Code
```C++
/*
 *	Implementing super dense coding using qumulator library:
 *	transfering two classical bit worth of information using one qubit.
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

──H──*──U──*──H──M═══
     │     │         
─────@─────@─────M═══
```

Circuit Diagram:
```
──H──*──U──*──H──M═══
     │     │         
─────@─────@─────M═══
```

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

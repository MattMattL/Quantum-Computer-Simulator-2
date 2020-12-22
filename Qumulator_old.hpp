#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>

using namespace std;

// A class representing a single qubit
class QCoefficient
{
private:

	double coeffRe;
	double coeffIm;

public:

	int isMeasured;

	QCoefficient()
	{
		coeffRe = 0;
		coeffIm = 0;

		isMeasured = 0;
	}

	double getRe()
	{
		return coeffRe;
	}

	double getIm()
	{
		return coeffIm;
	}

	double getReSq()
	{
		return coeffRe * coeffRe;
	}

	double getImSq()
	{
		return coeffIm * coeffIm;
	}

	double getProb()
	{
		return coeffRe * coeffRe + coeffIm * coeffIm;
	}

	void setCoeff(double realIn, double imgIn)
	{
		if(isMeasured)
		{
			printf("[QCoeff] Qubit state already measured.\n");
			return;
		}

		coeffRe = realIn;
		coeffIm = imgIn;
	}

	void setProb(double realIn, double imgIn)
	{
		if(isMeasured)
		{
			printf("[QCoeff] Qubit state already measured.\n");
			return;
		}

		coeffRe = sqrt(realIn);
		coeffIm = sqrt(imgIn);
	}

};

// A quantum system consisting of N Qubits
class Qubit
{
private:

	int numQubit;
	int numCoeff;

	int getNthBit(int num, int n);

public:

	QCoefficient *state;
	int isAllMeasured;

	// Constructor and Deconstructor
	Qubit(int numQubitIn);
	~Qubit();
	void reset();

	// Quantum Logic Gates
	void Hadamard(int qubit);
	void CNOT(int control, int target);
	void NOT(int qubit);
	int Measure();
	int Measure(int qubit);

	// Debuggers
	void printState();
	void printState(string msg);

};

Qubit::Qubit(int numQubitIn)
{
	srand(time(NULL));

	numQubit = numQubitIn;
	numCoeff = 1 << numQubitIn;

	state = new QCoefficient[numCoeff];
	reset();
}

Qubit::~Qubit()
{
	delete state;
}

void Qubit::reset()
{
	state[0].setProb(1, 0);

	for(int i=1; i<numCoeff; i++)
		state[i].setProb(0, 0);

	isAllMeasured = 0;
}

void Qubit::Hadamard(int qubit)
{
	for(int i=0; i<numCoeff; i++)
	{
		int j = i + (1 << qubit);

		if(!getNthBit(i, qubit) && j < numCoeff)
		{
			double newReA, newImA, newReB, newImB;

			newReA = state[i].getRe() + state[j].getRe();
			newImA = state[i].getIm() + state[j].getIm();

			newReB = state[i].getRe() - state[j].getRe();
			newImB = state[i].getIm() - state[j].getIm();

			state[i].setCoeff(newReA/sqrt(2), newImA/sqrt(2));
			state[j].setCoeff(newReB/sqrt(2), newImB/sqrt(2));
		}
	}
}

void Qubit::CNOT(int control, int target)
{
	for(int i=0; i<numCoeff; i++)
	{
		if(getNthBit(i, control) && !getNthBit(i, target))
		{
			int j = i + (1 << target);

			double coeffRe = state[i].getRe();
			double coeffIm = state[i].getIm();

			state[i].setCoeff(state[j].getRe(), state[j].getIm());
			state[j].setCoeff(coeffRe, coeffIm);
		}
	}
}

void Qubit::NOT(int qubit)
{
	for(int i=0; i<numCoeff; i++)
	{
		if(!getNthBit(i, qubit))
		{
			int j = i + (1 << qubit);

			double coeffRe = state[i].getRe();
			double coeffIm = state[i].getIm();

			state[i].setCoeff(state[j].getRe(), state[j].getIm());
			state[j].setCoeff(coeffRe, coeffIm);
		}
	}
}

int Qubit::Measure()
{
	// choose one random possible state
	int outcome;
	double randProb, sum = 0;

	randProb = rand() % 10000;
	randProb /= 10000;

	for(int i=0; i<numCoeff; i++)
	{
		sum += state[i].getProb();

		if(sum >= randProb)
		{
			outcome = i;
			break;
		}
	}

	// reset coefficients
	for(int i=0; i<numCoeff; i++)
	{
		state[i].setProb((i == outcome? 1 : 0), 0);
	}

	return outcome;
}

int Qubit::Measure(int qubit)
{
	int pureState = Measure();

	return getNthBit(pureState, qubit);
}

void Qubit::printState()
{
	for(int i=0; i<numCoeff; i++)
	{
		// print quantum state in Ket notation
		string strBin;
		int decToBin = i;

		for(int j=0; j<numQubit; j++)
		{
			strBin += decToBin % 2 + '0';
			decToBin /= 2;
		}

		for(int j=0; j<strBin.length() / 2; j++)
		{
			swap(strBin[j], strBin[numQubit-j-1]);
		}

		cout << "|" << strBin << ">";

		// print numeric info
		printf("\t%.3f", state[i].getProb());
		printf("\t(%5.2f +%5.2fi)\n", state[i].getRe(), state[i].getIm());
	}

	cout << endl;
}

void Qubit::printState(string msg)
{
	cout << " [" << msg << "]" << endl;
	printState();
}

int Qubit::getNthBit(int num, int n)
{
	return (num >> n) & 1;
}















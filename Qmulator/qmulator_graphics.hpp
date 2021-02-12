#ifndef QMULATOR_GRAPHICS_HPP
#define QMULATOR_GRAPHICS_HPP

#include <fstream>
#include <vector>
#include <string>

class QmulatorGraphics
{
public:
	enum gateType: int
	{
		NULL_TYPE = 0,
		MARGIN = 10,
		BARRIER = 11,
		SINGLE_QUBIT = 100,
		TWO_QUBITS = 200,
		THREE_QUBITS = 300,
		MEASURE = 400,
	};

	QmulatorGraphics();
	QmulatorGraphics(int);
	~QmulatorGraphics();
	void initialise(int);

	void add(gateType);
	void add(int, string, gateType);
	void add(int, int, string, gateType);
	void add(int, int, string, string, gateType);
	void add(int, int, string, string, string, gateType);
	void add(vector<int>, vector<string>, gateType);

	void newLine(int);

	void draw();
	void print();
	void save(string);

private:
	struct logger
	{
		vector<vector<int> > pos;
		vector<vector<string> > gate;
		vector<gateType> options;
	} logger;

	vector<vector<string> > map;
	bool *isClassical;

	int numOfLines;

	string QUANTUM_LINE;
	string CLASSICAL_LINE;
	string EMPTY_LINE;
	string VERTICAL_LINE;

	void link(int, int, int, string);
	void fill(int, int, int, string);

	void swap(int *num1, int *num2);
	int max(int num1, int num2);
	int min(int num1, int num2);
	int max(vector<int> vec);
	int min(vector<int> vec);
};

QmulatorGraphics::QmulatorGraphics()
{

}

QmulatorGraphics::QmulatorGraphics(int qubits)
{
	initialise(qubits);
}

QmulatorGraphics::~QmulatorGraphics()
{
	delete[] isClassical;
}

void QmulatorGraphics::initialise(int qubits)
{
	numOfLines = qubits;

	isClassical = new bool[qubits];

	for(int i=0; i<qubits; i++)
		isClassical[i] = false;

	CLASSICAL_LINE = "\u2550";
	QUANTUM_LINE = "\u2500";
	EMPTY_LINE = " ";
	VERTICAL_LINE = "\u2502";
}

void QmulatorGraphics::add(gateType type)
{
	vector<int> vecPos;
	vecPos.push_back(0);

	vector<string> vecGate;
	vecGate.push_back("NULL");

	add(vecPos, vecGate, type);
}

void QmulatorGraphics::add(int pos, string gate, gateType type)
{
	vector<int> vecPos;
	vecPos.push_back(pos);

	vector<string> vecGate;
	vecGate.push_back(gate);

	add(vecPos, vecGate, type);
}

void QmulatorGraphics::add(int pos1, int pos2, string gate, gateType type)
{
	vector<int> vecPos;
	vecPos.push_back(pos1);
	vecPos.push_back(pos2);

	vector<string> vecGate;
	vecGate.push_back(gate);

	add(vecPos, vecGate, type);
}

void QmulatorGraphics::add(int pos1, int pos2, string gate1, string gate2, gateType type)
{
	vector<int> vecPos;
	vecPos.push_back(pos1);
	vecPos.push_back(pos2);

	vector<string> vecGate;
	vecGate.push_back(gate1);
	vecGate.push_back(gate2);

	add(vecPos, vecGate, type);
}

void QmulatorGraphics::add(int pos1, int pos2, string gate1, string gate2, string gate3, gateType type)
{
	vector<int> vecPos;
	vecPos.push_back(pos1);
	vecPos.push_back(pos2);

	vector<string> vecGate;
	vecGate.push_back(gate1);
	vecGate.push_back(gate2);
	vecGate.push_back(gate3);

	add(vecPos, vecGate, type);
}

void QmulatorGraphics::add(vector<int> pos, vector<string> gates, gateType type)
{
	for(int i=0; i<pos.size(); i++)
		pos.at(i) *= 2;

	logger.pos.push_back(pos);
	logger.gate.push_back(gates);
	logger.options.push_back(type);
}

void QmulatorGraphics::link(int ptr, int start, int end, string ch)
{
	for(int i=start; i<end; i++)
	{
		if(map.at(i).at(ptr) != QUANTUM_LINE && map.at(i).at(ptr) != CLASSICAL_LINE)
			map.at(i).at(ptr) = ch;
	}
}

void QmulatorGraphics::fill(int ptr, int start, int end, string ch)
{
	for(int i=start; i<end; i++)
		map.at(i).at(ptr) = ch;
}

// Deprecated
void QmulatorGraphics::newLine(int n)
{
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<numOfLines; j++)
		{
			string circuitLine = (isClassical[j])? CLASSICAL_LINE : QUANTUM_LINE;

			map.at(j * 2).push_back(circuitLine);
			map.at(j * 2 + 1).push_back(" ");
		}
	}
}

void QmulatorGraphics::draw()
{
	// initialise map
	for(int i=0; i<numOfLines; i++)
	{
		vector<string> vecLine, vecSpace;

		vecLine.push_back(QUANTUM_LINE);
		vecSpace.push_back(EMPTY_LINE);

		map.push_back(vecLine);
		map.push_back(vecSpace);
	}

	newLine(2);
	int ptr = 2;

	// draw circuit diagram
	add(0, " ", NULL_TYPE);

	for(int i=0; i<logger.gate.size() - 1; i++)
	{
		vector<gateType> option = logger.options;
		vector<int> currPos = logger.pos.at(i);
		vector<int> nextPos = logger.pos.at(i + 1);
		vector<string> currGates = logger.gate.at(i);
		vector<string> nextGates = logger.gate.at(i + 1);

		// draw gates on current line
		switch(option.at(i))
		{
			case SINGLE_QUBIT:
				if(currGates.at(0) == "H")
					isClassical[currPos.at(0) / 2] = false;

				map.at(currPos.at(0)).at(ptr) = currGates.at(0);
				break;

			case TWO_QUBITS:
				fill(ptr, min(currPos), max(currPos), VERTICAL_LINE);
				map.at(currPos.at(0)).at(ptr) = currGates.at(0);
				map.at(currPos.at(1)).at(ptr) = currGates.at(1);
				break;

			case THREE_QUBITS:
				fill(ptr, min(currPos), max(currPos), VERTICAL_LINE);
				map.at(currPos.at(0)).at(ptr) = currGates.at(0);
				map.at(currPos.at(1)).at(ptr) = currGates.at(1);
				map.at(currPos.at(2)).at(ptr) = currGates.at(2);
				break;

			case MEASURE:
				isClassical[currPos.at(0) / 2] = true;
				map.at(currPos.at(0)).at(ptr) = currGates.at(0);
				break;

			case BARRIER:
				fill(ptr, 0, numOfLines * 2 - 1, "\u250A");
				break;

			default:
				break;
		}

		// draw new lines if required
		bool isMultiQubitGate = currPos.size() > 1 || nextPos.size() > 1;
		bool gateOverlaps = map.at(nextPos.at(0)).at(ptr) != QUANTUM_LINE && map.at(nextPos.at(0)).at(ptr) != CLASSICAL_LINE;
		bool requiresSpace = option.at(i) == MARGIN;
		bool isMeasuring = option.at(i) != MEASURE && option.at(i + 1) == MEASURE;
		bool isLastGate = i >= logger.gate.size() - 2;

		if(isMultiQubitGate || gateOverlaps || requiresSpace || isMeasuring)
		{
			if(requiresSpace)
			{
				newLine(1);
				ptr += 1;
			}
			else if(!isLastGate)
			{
				newLine(3);
				ptr += 3;
			}
		}
	}

	newLine(3);
}

void QmulatorGraphics::print()
{
	for(int i=0; i<map.size(); i++)
	{
		for(int j=0; j<map.at(0).size(); j++)
			cout << map.at(i).at(j);
		cout << endl;
	}
}

void QmulatorGraphics::save(string location)
{
	ofstream file;
	file.open(location, ofstream::trunc);

	for(int i=0; i<map.size(); i++)
	{
		for(int j=0; j<map.at(0).size(); j++)
			file << map.at(i).at(j);
		file << endl;
	}
	
	file.close();
}

void QmulatorGraphics::swap(int *num1, int *num2)
{
	int temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}

int QmulatorGraphics::max(int num1, int num2)
{
	return (num1 > num2)? num1 : num2;
}

int QmulatorGraphics::min(int num1, int num2)
{
	return (num1 < num2)? num1 : num2;
}

int QmulatorGraphics::max(vector<int> vec)
{
	int num = vec.at(0);

	for(int i=1; i<vec.size(); i++)
		num = max(num, vec.at(i));

	return num;
}

int QmulatorGraphics::min(vector<int> vec)
{
	int num = vec.at(0);

	for(int i=1; i<vec.size(); i++)
		num = min(num, vec.at(i));

	return num;
}

#endif
#ifndef QUMULATOR_GRAPHICS_HPP
#define QUMULATOR_GRAPHICS_HPP

#include <fstream>
#include <vector>
#include <string>

class QumulatorGraphics
{
public:
	enum gateType: int;

	enum gateType: int
	{
		NULL_TYPE = 0,
		SPACE = 10,
		SINGLE_QUBIT = 100,
		TWO_QUBITS = 200,
		THREE_QUBITS = 300,
		MEASURE = 400,
	};

	QumulatorGraphics();
	QumulatorGraphics(int);
	~QumulatorGraphics();
	void initialise(int);

	void add(int, char, gateType);
	void add(int, int, char, gateType);
	void add(int, int, char, char, gateType);
	void add(int, int, char, char, char, gateType);
	void add(vector<int>, vector<char>, gateType);

	// Deprecated
	void newLine();

	void draw();
	void print();
	void save(string);

private:
	struct logger
	{
		vector<vector<int> > pos;
		vector<vector<char> > gate;
		vector<gateType> options;
	} logger;

	vector<string> map;
	bool *isClassical;

	int numOfLines;

	void swap(int *num1, int *num2)
	{
		int temp = *num1;
		*num1 = *num2;
		*num2 = temp;
	}

	int max(int num1, int num2)
	{
		return (num1 > num2)? num1 : num2;
	}
	int min(int num1, int num2)
	{
		return (num1 < num2)? num1 : num2;
	}

	int max(vector<int> vec)
	{
		int num = vec.at(0);

		for(int i=1; i<vec.size(); i++)
			num = max(num, vec.at(i));

		return num;
	}

	int min(vector<int> vec)
	{
		int num = vec.at(0);

		for(int i=1; i<vec.size(); i++)
			num = min(num, vec.at(i));

		return num;
	}

	void link(int, int, int, char);
	void fill(int, int, int, char);
};

QumulatorGraphics::QumulatorGraphics()
{

}

QumulatorGraphics::QumulatorGraphics(int qubits)
{
	initialise(qubits);
}

QumulatorGraphics::~QumulatorGraphics()
{
	delete[] isClassical;
}

void QumulatorGraphics::initialise(int qubits)
{
	numOfLines = qubits;

	isClassical = new bool[qubits];

	for(int i=0; i<qubits; i++)
		isClassical[i] = false;
}

void QumulatorGraphics::add(int pos, char gate, gateType type)
{
	vector<int> vecPos;
	vecPos.push_back(pos);

	vector<char> vecGate;
	vecGate.push_back(gate);

	add(vecPos, vecGate, type);
}

void QumulatorGraphics::add(int pos1, int pos2, char gate, gateType type)
{
	vector<int> vecPos;
	vecPos.push_back(pos1);
	vecPos.push_back(pos2);

	vector<char> vecGate;
	vecGate.push_back(gate);

	add(vecPos, vecGate, type);
}

void QumulatorGraphics::add(int pos1, int pos2, char gate1, char gate2, gateType type)
{
	vector<int> vecPos;
	vecPos.push_back(pos1);
	vecPos.push_back(pos2);

	vector<char> vecGate;
	vecGate.push_back(gate1);
	vecGate.push_back(gate2);

	add(vecPos, vecGate, type);
}

void QumulatorGraphics::add(int pos1, int pos2, char gate1, char gate2, char gate3, gateType type)
{
	vector<int> vecPos;
	vecPos.push_back(pos1);
	vecPos.push_back(pos2);

	vector<char> vecGate;
	vecGate.push_back(gate1);
	vecGate.push_back(gate2);
	vecGate.push_back(gate3);

	add(vecPos, vecGate, type);
}

void QumulatorGraphics::add(vector<int> pos, vector<char> gates, gateType type)
{
	for(int i=0; i<pos.size(); i++)
		pos.at(i) *= 2;

	logger.pos.push_back(pos);
	logger.gate.push_back(gates);
	logger.options.push_back(type);
}

void QumulatorGraphics::link(int ptr, int start, int end, char ch)
{
	for(int i=start; i<=end; i++)
	{
		if(map.at(i).at(ptr) != '-')
			map.at(i).at(ptr) = ch;
	}
}

void QumulatorGraphics::fill(int ptr, int start, int end, char ch)
{
	for(int i=start; i<=end; i++)
		map.at(i).at(ptr) = ch;
}

// Deprecated
void QumulatorGraphics::newLine()
{
	for(int i=0; i<numOfLines; i++)
	{
		map.at(i * 2) += (isClassical[i])? "=" : "-";
		map.at(i * 2 + 1) += " ";
	}
}

void QumulatorGraphics::draw()
{
	// initialise map
	for(int i=0; i<numOfLines; i++)
	{
		map.push_back("-");
		map.push_back(" ");
	}

	newLine();
	newLine();

	// draw circuit diagram
	int ptr = 2;

	add(0, ' ', NULL_TYPE);

	for(int i=0; i<logger.gate.size() - 1; i++)
	{
		vector<gateType> gateType = logger.options;
		vector<int> currPos = logger.pos.at(i);
		vector<int> nextPos = logger.pos.at(i + 1);
		vector<char> currGates = logger.gate.at(i);
		vector<char> nextGates = logger.gate.at(i + 1);

		// draw circuits on current line
		switch(gateType.at(i))
		{
			case SINGLE_QUBIT:
				isClassical[currPos.at(0) / 2] = false;
				map.at(currPos.at(0)).at(ptr) = currGates.at(0);
				break;

			case TWO_QUBITS:
				// isClassical[currPos.at(0) / 2] = false;
				fill(ptr, min(currPos), max(currPos), '|');
				map.at(currPos.at(0)).at(ptr) = currGates.at(0);
				map.at(currPos.at(1)).at(ptr) = currGates.at(1);
				break;

			case THREE_QUBITS:
				// isClassical[currPos.at(0) / 2] = false;
				fill(ptr, min(currPos), max(currPos), '|');
				map.at(currPos.at(0)).at(ptr) = currGates.at(0);
				map.at(currPos.at(1)).at(ptr) = currGates.at(1);
				map.at(currPos.at(2)).at(ptr) = currGates.at(2);
				break;

			case MEASURE:
				isClassical[currPos.at(0) / 2] = true;
				map.at(currPos.at(0)).at(ptr) = currGates.at(0);
				break;

			default:
				break;
		}

		// draw new lines for next gate
		bool isMultiQubitGate = currPos.size() > 1 || nextPos.size() > 1;
		bool gateOverlaps = map.at(nextPos.at(0)).at(ptr) != '-';
		bool requriesSpace = gateType.at(i) == SPACE;
		bool isMeasurement = gateType.at(i) != MEASURE && gateType.at(i + 1) == MEASURE;

		if(isMultiQubitGate || gateOverlaps || requriesSpace || isMeasurement)
		{
			newLine();
			newLine();
			newLine();

			ptr += 3;
		}
	}
}

void QumulatorGraphics::print()
{
	for(int i=0; i<map.size(); i++)
	{
		for(int j=0; j<map.at(0).size(); j++)
			cout << map.at(i).at(j);
		cout << endl;
	}
}

void QumulatorGraphics::save(string location)
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

#endif
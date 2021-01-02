#ifndef QUMULATOR_GRAPHICS_HPP
#define QUMULATOR_GRAPHICS_HPP

#include <fstream>
#include <vector>
#include <string>

class QumulatorGraphics
{
public:
	enum gateType: int;
	enum markType: int;

private:
	struct logger
	{
		vector<vector<int> > pos;
		vector<vector<char> > gate;
		vector<markType> options;
	} logger;

	vector<string> map;

	int numOfLines;

	void swap(int *num1, int *num2)
	{
		int temp = *num1;
		*num1 = *num2;
		*num2 = temp;
	}

	int max(int num1, int num2) { return (num1 > num2)? num1 : num2; }

	int min(int num1, int num2) { return (num1 < num2)? num1 : num2; }

	void draw();
	void link(string*, int, int, char);
	void fill(string*, int, int, char);

public:
	enum gateType: int
	{
		LINE = (0 << 8) + 0,

		HADAMARD = (1 << 8) + 10, // required_qubits << 8 + serial
		PAULI_X = (1 << 8) + 11,
		PAULI_Y = (1 << 8) + 12,
		PAULI_Z = (1 << 8) + 13,
		CNOT = (2 << 8) + 14,
	};

	enum markType: int
	{
		MARK = 10,
		MARK_AND_FILL = 11,
		MARK_AND_LINK = 12,

		FILL_BETWEEN = 20,
		LINK_BETWEEN = 21,
		FILL_ALL = 30,
		LINK_ALL = 31,
	};

	QumulatorGraphics();
	QumulatorGraphics(int);
	~QumulatorGraphics();

	void setSize(int);

	void add(int, char, markType);
	void add(int, int, char, markType);
	void add(int, int, char, char, markType);
	void add(int, int, char, char, char, markType);
	void add(vector<int>, vector<char>, markType);

	void margin();
	void print();
	void save(string);
};

QumulatorGraphics::QumulatorGraphics()
{

}

QumulatorGraphics::QumulatorGraphics(int qubits)
{
	setSize(qubits);
}

QumulatorGraphics::~QumulatorGraphics()
{

}

void QumulatorGraphics::setSize(int qubits)
{
	numOfLines = qubits;
}

void QumulatorGraphics::add(int pos, char gate, markType type)
{
	vector<int> vecPos;
	vecPos.push_back(pos);

	vector<char> vecGate;
	vecGate.push_back(gate);

	add(vecPos, vecGate, type);
}

void QumulatorGraphics::add(int pos1, int pos2, char gate, markType type)
{
	vector<int> vecPos;
	vecPos.push_back(pos1);
	vecPos.push_back(pos2);

	vector<char> vecGate;
	vecGate.push_back(gate);

	add(vecPos, vecGate, type);
}

void QumulatorGraphics::add(int pos1, int pos2, char gate1, char gate2, markType type)
{
	vector<int> vecPos;
	vecPos.push_back(pos1);
	vecPos.push_back(pos2);

	vector<char> vecGate;
	vecGate.push_back(gate1);
	vecGate.push_back(gate2);

	add(vecPos, vecGate, type);
}

void QumulatorGraphics::add(int pos1, int pos2, char gate1, char gate2, char gate3, markType type)
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

void QumulatorGraphics::add(vector<int> pos, vector<char> gates, markType type)
{
	for(int i=0; i<pos.size(); i++)
		pos.at(i) *= 2;

	logger.pos.push_back(pos);
	logger.gate.push_back(gates);
	logger.options.push_back(type);
}

void QumulatorGraphics::link(string *str, int start, int end, char ch)
{
	if(start > end)
		swap(&start, &end);

	for(int i=start; i<=end; i++)
		(*str)[i] = ((*str)[i] != '-')? ch : (*str)[i];
}

void QumulatorGraphics::fill(string *str, int start, int end, char ch)
{
	if(start > end)
		swap(&start, &end);

	for(int i=start; i<=end; i++)
		(*str)[i] = ch;
}

void QumulatorGraphics::margin()
{
	// add(0, LINE);
}

void QumulatorGraphics::draw()
{
	add(0, ' ', MARK); // dummy element

	string emptyLines, currLine;

	for(int j=0; j<numOfLines; j++)
		emptyLines += "- ";

	currLine = emptyLines;

	map.push_back(emptyLines);
	map.push_back(emptyLines);

	// add one virtical line each time
	for(int i=0; i<logger.gate.size() - 1; i++)
	{
		vector<int> currPos = logger.pos.at(i);
		vector<int> nextPos = logger.pos.at(i + 1);
		vector<char> currGates = logger.gate.at(i);
		vector<char> nextGates = logger.gate.at(i + 1);

		switch(logger.options.at(i))
		{
			case MARK:
				for(int j=0; j<currGates.size(); j++)
					currLine[currPos.at(j)] = currGates.at(j);
				break;

			case MARK_AND_FILL:
				fill(&currLine, currPos.at(0), currPos.at(1), currGates.back());

				for(int j=0; j<currPos.size(); j++)
					currLine[currPos.at(j)] = currGates.at(j);
				break;

			case MARK_AND_LINK:
				link(&currLine, currPos.at(0), currPos.at(1), currGates.back());

				for(int j=0; j<currPos.size(); j++)
					currLine[currPos.at(j)] = currGates.at(j);
				break;

			case FILL_BETWEEN:
				fill(&currLine, currPos.at(0) + 1, currPos.at(1) - 1, currGates.at(0));
				break;
			
			case LINK_BETWEEN:
				link(&currLine, currPos.at(0) + 1, currPos.at(1) - 1, currGates.at(0));
				break;
			
			case FILL_ALL:
				fill(&currLine, currPos.at(0), currPos.at(1), currGates.at(0));
				break;
			
			case LINK_ALL:
				link(&currLine, currPos.at(0), currPos.at(1), currGates.at(0));
				break;

			default:
				break;
		}

		bool gateOverlaps = currLine[nextPos.at(0)] != '-';


		if(currPos.size() > 1 || nextPos.size() > 1 || gateOverlaps)
		{
			if(i >= logger.gate.size() - 2)
				break;

			map.push_back(currLine);
			map.push_back(emptyLines);
			map.push_back(emptyLines);

			currLine = emptyLines;
		}
	}

	map.push_back(currLine);

	map.push_back(emptyLines);
	map.push_back(emptyLines);
}

void QumulatorGraphics::print()
{
	draw();

	for(int j=0; j<2 * numOfLines - 1; j++)
	{
		for(int i=0; i<map.size(); i++)
			cout << map.at(i).at(j);

		cout << endl;
	}
}

void QumulatorGraphics::save(string location)
{
	ofstream file;
	file.open(location, ofstream::trunc);

	draw();

	for(int j=0; j<2 * numOfLines - 1; j++)
	{
		for(int i=0; i<map.size(); i++)
			file << map.at(i).at(j);

		file << endl;
	}
	
	file.close();
}

#endif
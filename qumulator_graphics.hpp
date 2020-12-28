#ifndef QUMULATOR_GRAPHICS_HPP
#define QUMULATOR_GRAPHICS_HPP

#include <vector>
#include <string>

class QumulatorGraphics
{
public:
	enum gateType: int;

private:
	struct logger
	{
		vector<gateType> gate;
		vector<vector<int> > pos;
	} logger;

	vector<string> map;

	int lines;

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

	QumulatorGraphics(int);
	~QumulatorGraphics();

	void add(int, gateType);
	void add(int, int, gateType);
	void add(vector<int>, gateType);

	void margin();
	void draw();
	void print();

private:

};

QumulatorGraphics::QumulatorGraphics(int qubits)
{
	lines = qubits;
}

QumulatorGraphics::~QumulatorGraphics()
{

}

void QumulatorGraphics::add(int pos0, gateType gateIn)
{
	vector<int> temp;
	temp.push_back(pos0);

	logger.pos.push_back(temp);
	logger.gate.push_back(gateIn);
}

void QumulatorGraphics::add(int pos0, int pos1, gateType gateIn)
{
	vector<int> temp;
	temp.push_back(pos0);
	temp.push_back(pos1);

	logger.pos.push_back(temp);
	logger.gate.push_back(gateIn);
}

void QumulatorGraphics::add(vector<int> pos, gateType gateIn)
{
	logger.pos.push_back(pos);
	logger.gate.push_back(gateIn);
}

void QumulatorGraphics::link(string *str, int start, int end, char ch)
{
	if(start > end)
	{
		int temp = end;
		end = start;
		start = temp;
	}

	for(int i=start+1; i<end; i++)
		(*str)[i] = ch;
}

void QumulatorGraphics::fill(string *str, int start, int end, char ch)
{
	if(start > end)
	{
		int temp = end;
		end = start;
		start = temp;
	}

	for(int i=start; i<=end; i++)
		(*str)[i] = ch;
}

void QumulatorGraphics::margin()
{
	add(0, LINE);
}

void QumulatorGraphics::draw()
{
	add(0, LINE);

	string emptyLines, currLine;
	bool isNewline, gateOverlaps, isMultiQubitGate;

	for(int j=0; j<lines; j++)
		emptyLines += "- ";

	currLine = emptyLines;
	map.push_back(currLine);

	for(int i=0; i<logger.gate.size() - 1; i++)
	{
		vector<int> currPos = logger.pos.at(i);
		gateType currGate = logger.gate.at(i);
		gateType nextGate = logger.gate.at(i);
		
		gateOverlaps = currLine[2 * currPos.at(0)] != '-';
		isMultiQubitGate = (currGate >> 8) > 1;
		isNewline = currGate == LINE;

		if(isMultiQubitGate || gateOverlaps)
		{
			map.push_back(currLine);
			map.push_back(emptyLines);

			currLine = emptyLines;
		}

		switch(currGate)
		{
			case HADAMARD:
				currLine[2 * currPos.at(0)] = 'H';
				break;
			case PAULI_X:
				currLine[2 * currPos.at(0)] = 'X';
				break;
			case PAULI_Y:
				currLine[2 * currPos.at(0)] = 'Y';
				break;
			case PAULI_Z:
				currLine[2 * currPos.at(0)] = 'Z';
				break;
			case CNOT:
				currLine[2 * currPos.at(0)] = '*';
				currLine[2 * currPos.at(1)] = '@';
				link(&currLine, 2 * currPos.at(0), 2 * currPos.at(1), '|');
				break;
			default:
				break;
		}

		if(isMultiQubitGate)
		{
			map.push_back(currLine);
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
	for(int j=0; j<2 * lines; j++)
	{
		for(int i=0; i<map.size(); i++)
			cout << map.at(i).at(j);

		cout << endl;
	}
}

#endif
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

	void addLine()
	{
		string str = "";

		for(int j=0; j<lines; j++)
			str += "- ";

		map.push_back(str);
	}

	void link(string*, int, int, char);
	void fill(string*, int, int, char);

public:
	enum gateType: int
	{
		LINE = (1 << 8) + 0,

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
	vector<int> dummy;
	dummy.push_back(0);

	add(0, LINE);
}

void QumulatorGraphics::draw()
{
	string str, strInitial;

	for(int j=0; j<lines; j++)
		strInitial += "- ";

	str = strInitial;

	addLine();

	for(int i=0; i<logger.gate.size(); i++)
	{
		int qubitsUsed = logger.gate.at(i) >> 8;
		int qubitSerial = 2 * logger.pos.at(i).at(0);

		if(qubitsUsed > 1 || str[qubitSerial] != '-')
		{
			// append previous result
			map.push_back(str);

			// shift line for multi-qubit gates
			str = strInitial;
			map.push_back(str);
		}

		vector<int> position = logger.pos.at(i);

		switch(logger.gate.at(i))
		{
			case LINE:
				addLine();
				break;
			case HADAMARD:
				str[2 * position.at(0)] = 'H';
				break;
			case CNOT:
				str[2 * position.at(0)] = '*';
				str[2 * position.at(1)] = '@';
				link(&str, 2 * position.at(0),2 * position.at(1), '|');
				break;
			case PAULI_X:
				break;
			case PAULI_Y:
				break;
			case PAULI_Z:
				break;
			default:
				break;
		}

		if(qubitsUsed > 1)
		{
			// append previous result
			map.push_back(str);

			// shift line for multi-qubit gates
			str = strInitial;
			map.push_back(str);
		}
	}

	// append the last result
	map.push_back(str);

	// append dummy lines
	addLine();
	addLine();
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
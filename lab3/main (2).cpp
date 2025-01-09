#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
struct LN
{

	static const uint32_t BASE = 1000 * 1000 * 1000;
	static const uint32_t SIZE = 16;
	bool positive = true;
	uint32_t digits[SIZE];
	LN()
	{	 // базовый конструктор
		for (int i = 0; i < SIZE; i++)
		{
			digits[i] = 0;
		}
	}

	LN(uint32_t x)
	{	 // assignmen constructor
		for (int i = 0; i < SIZE; i++)
		{
			digits[i] = 0;
		}
		int num = 0;
		while (x)
		{
			digits[num++] = x % BASE;
			x /= BASE;
		}
	}
	LN(const LN &newNum)
	{	 // copy constructor
		for (int i = 0; i < SIZE; i++)
		{
			digits[i] = newNum.digits[i];
		}
	}

	LN(LN &first, LN &sec)
	{	 // move constructor

		for (int i = 0; i < SIZE; i++)
		{
			first.digits[i] = sec.digits[i];
		}
	}

	LN &operator=(const LN &newNum)
	{	 // copy assignment operator
		for (int i = 0; i < SIZE; i++)
		{
			digits[i] = newNum.digits[i];
		}

		return *this;
	}

	void operator+=(const LN &other)
	{
		for (int i = 0; i < SIZE; i++)
		{
			digits[i] += other.digits[i];
		}

		for (int i = 0; i < SIZE - 1; i++)
		{
			if (digits[i] >= BASE)
			{
				digits[i] -= BASE;
				digits[i + 1]++;
			}
		}
	}

	LN operator+(const LN &other)
	{
		LN n(*this);
		n += other;
		return n;
	}

	void operator-=(const LN &other)
	{

		for (int i = 0; i < SIZE; i++)
		{
			digits[i] -= other.digits[i];
		}

		for (int i = 0; i < SIZE - 1; i++)
		{
			if (digits[i] < 0)
			{
				digits[i] += BASE;
				digits[i + 1]--;
			}
		}
	}

	LN operator-(const LN &other)
	{
		LN n(*this);
		n -= other;
		return n;
	}

	LN operator*(const LN &other)
	{
		LN n(*this);
		n *= other;
		return n;
	}

	LN operatorSqrt()
	{
		LN ans;
		uint32_t pos = (SIZE + 1) / 2;

		pos--;
		while (pos >= 0)
		{
			uint32_t l = 0;
			uint32_t r = SIZE;
			uint32_t curDig = 0;
			while (l <= r)
			{
				int m = (r + l) >> 1;
				ans.digits[pos] = m;
				operator*=(ans);
				if (operator<=(ans))
				{
					curDig = m;
					l = m + 1;
				}
				else
				{
					r = m - 1;
				}
				ans.digits[pos] = curDig;
				pos--;
			}
		}
		return ans;
	}

	void operator*=(const LN &other)
	{
		if (other.positive != positive)
		{
			positive = false;
		}

		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE - i; j++)
			{
				digits[i + j] += digits[i] * other.digits[j];
			}
		}

		for (int i = 0; i < SIZE - 1; i++)
		{
			digits[i + 1] += digits[i] / BASE;
			digits[i] %= BASE;
		}
	}

	void operator/=(uint32_t x)
	{
		for (int i = SIZE - 1; i >= 0; i--)
		{
			if (i)
			{
				digits[i - 1] += (digits[i] % x) * BASE;
			}

			digits[i] /= x;
		}
	}

	LN operator/(uint32_t x)
	{
		LN n(*this);
		n /= x;
		return n;
	}

	void operator~() { positive = false; }
	bool operator==(const LN &other)
	{
		bool equality = true;
		if (positive != other.positive)
		{
			equality = false;
		}
		else
		{
			for (size_t i = 0; i < SIZE; i++)
			{
				if (digits[i] != other.digits[i])
				{
					equality = false;
				}
			}
		}

		return equality;
	}

	bool operator!=(const LN &other)
	{
		bool inequality = true;
		if (operator==(other))
		{
			inequality = false;
		}

		return inequality;
	}
	bool operator<=(const LN &other)
	{
		if (operator==(other))
		{
			return true;
		}
		uint32_t ans = 0;
		if ((positive == false) && (other.positive == true))
		{
			return true;
		}
		if ((other.positive == false) && (positive == true))
		{
			return false;
		}

		else
		{
			for (int i = SIZE - 1; i >= 0; i--)
			{
				if (digits[i] < other.digits[i])
				{
					return true;
				}
				else if (digits[i] > other.digits[i])
				{
					ans = 0;
				}
			}

			ans = 1;
		}
		if ((other.positive == true) && (positive = true))
		{
			return ans > 0;
		}
		else
		{
			return ans = 0;
		}
	}
	bool operator>=(const LN &other) { return (!(operator<=(other))) || (operator==(other)); }
	bool operator<(const LN &other) { return !(operator>=(other)); }

	bool operator>(const LN &other) { return !(operator<=(other)); }
};
// Function to check if a string is a valid number
bool isNumber(const std::string &str)
{
	std::stringstream ss(str);
	double num;
	return ss >> num && ss.eof();
}

// Function to perform arithmetic operations based on the operator
LN performOperation(char op, uint32_t operand1, uint32_t operand2)
{
	switch (op)
	{
	case '+':
		LN(operand1).operator+(LN(operand2));
	case '-':
		return LN(operand1).operator-(LN(operand2));
	case '*':
		return LN(operand1).operator*(LN(operand2));
	case '/':
		return LN(operand1).operator/((operand2));

	case '<':
		return LN(operand1).operator<(LN(operand2));
	case '>':
		return LN(operand1).operator>(LN(operand2));
	default:
		return 0;	 // Unsupported operator
	}
}

// Function to parse and evaluate the result
double evaluateRPN(const std::vector<std::string> &tokens)
{
	std::stack<double> operandStack;
	uint32_t result = 0;

	for (const auto &token : tokens)
	{
		if (isNumber(token))
		{
			operandStack.push(std::stod(token));
		}
		else
		{
			if (operandStack.size() < 2)
			{
				// Invalid expression
				return 0.0;
			}
			double operand2 = operandStack.top();
			operandStack.pop();
			double operand1 = operandStack.top();
			operandStack.pop();
			// result = performOperation(token[0], operand1, operand2);
			operandStack.push(result);
		}
	}

	if (operandStack.size() != 1)
	{
		// Invalid RPN expression
		return 0;
	}

	return operandStack.top();
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cout << "Usage: " << argv[0] << " input_file output_file\n";
		return 1;
	}

	std::ifstream inputFile(argv[1]);
	if (!inputFile)
	{
		std::cerr << "Error opening input file.\n";
		return 1;
	}

	std::ofstream outputFile(argv[2]);
	if (!outputFile)
	{
		std::cerr << "Error opening output file.\n";
		return 1;
	}

	std::string line;
	while (std::getline(inputFile, line))
	{
		std::vector<std::string> tokens;
		std::stringstream ss(line);
		std::string token;
		while (ss >> token)
		{
			tokens.push_back(token);
		}
		double result = evaluateRPN(tokens);
		outputFile << result << "\n";
	}

	inputFile.close();
	outputFile.close();

	return 0;
}

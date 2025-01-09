#include <iostream>
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

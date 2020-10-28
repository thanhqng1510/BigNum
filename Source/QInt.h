#pragma once


#include <string>
#include <iostream>


enum class TYPE {
	BINARY = 2,
	DECIMAL = 10,
	HEXADECIMAL = 16,
};

class QInt {
private:
	__int64* m_bits;
protected:
	// Divide a positive decimal string by 2
	static void div2String(std::string& value);
	// Euclid division return X_prime and R in function X = base * X_prime + R
	static std::pair<std::string, int> euclidDivisionBinaryString(const std::string& X, int base);
protected:
	// Scan function for type BINARY
	void scanBinary(const std::string& value);
	// Scan function for type DECIMAL
	void scanDecimal(const std::string& value);
	// Scan function for type HEXADECIMAL
	void scanHexadecimal(const std::string& value);
	// Print function for type BINARY
	void printStreamBinary(std::ostream& stream) const;
	// Print function for type DECIMAL
	void printStreamDecimal(std::ostream& stream) const;
	// Print function for type HEXADECIMAL
	void printStreamHexadecimal(std::ostream& stream) const;
public:
	QInt();
	QInt(TYPE type, const std::string& value);	
	QInt(const QInt& q);
	QInt& operator=(const QInt& q);
	~QInt();
public:
	// Convert a string of type BINARY | DECIMAL | HEXADECIMAL to an __int64[2] array of bits
	void scan(TYPE type, const std::string& value);
	// Print out the number by type BINARY | DECIMAL | HEXADECIMAL to a stream (console or file)
	void printStream(TYPE type, std::ostream& stream) const;
	// Return a string of binary
	std::string toStringBinary() const;
	// Return a string of binary
	std::string toStringDecimal() const;
	// Return a string of binary
	std::string toStringHexadecimal() const;
	// Change QInt from positive to negative and vice versa
	void switchSign2Complement();
	// Flip bit from 0 to 1 and vice versa
	void flipBit(int bin_idx);
	// Flip all bit from 0 to 1 and vice versa
	void flipBitAll();
	// Get value of a bit (right-most is 0, left most is 127)
	bool getBit(int bin_idx) const;
	// Reset all bits to 0
	void reset();
public:
	bool operator==(const QInt& q) const;
	bool operator<(const QInt& q) const;
public:
	QInt& operator++();
	QInt& operator--();
public:
	QInt& operator+=(const QInt& q);
	QInt& operator-=(const QInt& q);
	QInt& operator*=(const QInt& q);
	QInt& operator/=(const QInt& q);
public:
	QInt& operator&=(const QInt& q);
	QInt& operator|=(const QInt& q);
	QInt& operator^=(const QInt& q);
	QInt& operator~();
public:
	QInt& operator<<=(int bits);
	QInt& operator>>=(int bits);
public:
	QInt& rol();
	QInt& ror();
};
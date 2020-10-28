#include <vector>
#include <iostream>
#include "QInt.h"


void QInt::div2String(std::string& value) {
	if (value == "")
		return;

	int additive = 0, next_additive = 0;
	for (unsigned int i = 0; i < value.length(); ++i) {
		additive = next_additive;
		next_additive = ((value[i] - 48) % 2) * 5;
		value[i] = (value[i] - 48) / 2 + additive + 48;
	}

	unsigned int i = 0;
	while (i < value.length() && value[i] == '0')
		++i;
	value = value.substr(i < value.length() ? i : value.length() - 1);
}

std::pair<std::string, int> QInt::euclidDivisionBinaryString(const std::string& X, int base) {
	if (X == "")
		return { "", 0 };

	int R = 0;	// Remainder from 0 to less than base
	std::string X_prime = "";

	for (char c : X) {
		R = R * 2 + (c - 48);
		if (R >= base) {
			X_prime += '1';
			R -= base;
		}
		else
			X_prime += '0';
	}

	unsigned int i = 0;
	while (i < X_prime.length() && X_prime[i] == '0')
		++i;
	X_prime = X_prime.substr(i < X_prime.length() ? i : X_prime.length() - 1);

	return { X_prime, R };
}

void QInt::scanBinary(const std::string& value) {
	reset();

	unsigned int bin_idx = 0;
	while (bin_idx < value.length()) {
		if (value[value.length() - 1 - bin_idx] == '1')
			flipBit(bin_idx);
		++bin_idx;
	}
}

void QInt::scanDecimal(const std::string& value) {
	if (value == "")
		return;

	reset();

	std::string new_val = value;
	bool negative = (value[0] == '-');
	if (negative)
		new_val = new_val.substr(1);

	unsigned int bin_idx = 0;
	while (new_val != "0") {
		if ((new_val.back() - 48) % 2)
			flipBit(bin_idx);
		div2String(new_val);
		++bin_idx;
	}

	if (negative)
		switchSign2Complement();
}

void QInt::scanHexadecimal(const std::string& value) {
	std::string bin_form = "";

	for (char c : value) {
		std::string s(1, c);
		if (c >= 'A' && c <= 'Z')
			s = std::to_string(int(c - 'A' + 10));

		std::string s_bin(4, '0');

		unsigned int i = s_bin.length() - 1;
		while (s != "0" && i >= 0) {
			if ((s.back() - 48) % 2)
				s_bin[i] = '1';
			div2String(s);
			--i;
		}

		bin_form += s_bin;
	}

	scanBinary(bin_form);
}

void QInt::printStreamBinary(std::ostream& stream) const {
	stream << toStringBinary() << "\n";
}

void QInt::printStreamDecimal(std::ostream& stream) const {
	stream << toStringDecimal() << "\n";
}

void QInt::printStreamHexadecimal(std::ostream& stream) const {
	stream << toStringHexadecimal() << "\n";
}

QInt::QInt() : m_bits(new __int64[2]) {
	m_bits[0] = m_bits[1] = 0;
}

QInt::QInt(TYPE type, const std::string& value) : m_bits(new __int64[2]) {
	m_bits[0] = m_bits[1] = 0;
	scan(type, value);
}

QInt::QInt(const QInt& q) : m_bits(new __int64[2]) {
	m_bits[0] = q.m_bits[0];
	m_bits[1] = q.m_bits[1];
}

QInt& QInt::operator=(const QInt& q) {
	m_bits[0] = q.m_bits[0];
	m_bits[1] = q.m_bits[1];
	return *this;
}

QInt::~QInt() {
	delete[] m_bits;
}

void QInt::scan(TYPE type, const std::string& value) {
	switch (type) {
	case TYPE::BINARY:
		scanBinary(value);
		break;
	case TYPE::DECIMAL:
		scanDecimal(value);
		break;
	case TYPE::HEXADECIMAL:
		scanHexadecimal(value);
		break;
	default:
		break;
	}
}

void QInt::printStream(TYPE type, std::ostream& stream) const {
	switch (type) {
	case TYPE::BINARY:
		printStreamBinary(stream);
		break;
	case TYPE::DECIMAL:
		printStreamDecimal(stream);
		break;
	case TYPE::HEXADECIMAL:
		printStreamHexadecimal(stream);
		break;
	default:
		break;
	}
}

std::string QInt::toStringBinary() const {
	std::string res = "";
	bool start = false;
	for (int bin_idx = 127; bin_idx >= 0; --bin_idx) {
		bool b = getBit(bin_idx);
		if (b)
			start = true;
		if (start)
			res += (b + 48);
	}
	if (res == "")
		res = "0";
	return res;
}

std::string QInt::toStringDecimal() const {
	QInt temp = *this;

	bool negative = temp.getBit(127);
	if (negative)
		temp.switchSign2Complement();

	std::vector<char> stack;
	std::string X = temp.toStringBinary();

	while (X != "0") {	// X = base * X_prime + R (Euclid devision) (base == 10)
		auto [X_prime, R] = euclidDivisionBinaryString(X, 10);
		stack.emplace_back(R + 48);
		X = X_prime;
	}

	if (negative)
		stack.emplace_back('-');

	std::string res = "";
	while (!stack.empty()) {
		res += stack.back();
		stack.pop_back();
	}

	if (res == "")
		res = "0";

	return res;
}

std::string QInt::toStringHexadecimal() const {
	std::vector<char> stack;
	std::string X = toStringBinary();

	while (X != "0") {	// X = base * X_prime + R (Euclid devision) (base == 16)
		auto [X_prime, R] = euclidDivisionBinaryString(X, 16);
		char c_R = (R >= 0 && R <= 9) ? (R + 48) : (R - 10 + 'A');
		stack.emplace_back(c_R);
		X = X_prime;
	}

	std::string res = "";
	while (!stack.empty()) {
		res += stack.back();
		stack.pop_back();
	}

	if (res == "")
		res = "0";

	return res;
}

void QInt::switchSign2Complement() {
	bool negative = getBit(127);

	if (!negative) {
		flipBitAll();
		++(*this);
	}
	else {
		--(*this);
		flipBitAll();
	}
}

void QInt::flipBit(int bin_idx) {
	if (bin_idx < 0 || bin_idx > 127)
		return;

	if (bin_idx < 64)
		m_bits[1] ^= (1LL << bin_idx);
	else
		m_bits[0] ^= (1LL << (bin_idx - 64));
}

void QInt::flipBitAll() {
	m_bits[1] ^= -1LL;
	m_bits[0] ^= -1LL;
}

bool QInt::getBit(int bin_idx) const {
	if (bin_idx < 0 || bin_idx > 127)
		return 0;

	if (bin_idx < 64)
		return (m_bits[1] >> bin_idx) & 1;
	return (m_bits[0] >> (bin_idx - 64)) & 1;
}

void QInt::reset() {
	m_bits[0] = m_bits[1] = 0;
}

bool QInt::operator==(const QInt& q) const {
	return m_bits[0] == q.m_bits[0] && m_bits[1] == q.m_bits[1];
}

bool QInt::operator<(const QInt& q) const {
	std::string q1 = toStringDecimal(), q2 = q.toStringDecimal();
	return q1.length() < q2.length() || (q1.length() == q2.length() && q1 < q2);
}

QInt& QInt::operator++() {
	int bin_idx = 0;
	while (getBit(bin_idx) && bin_idx <= 127) {
		flipBit(bin_idx);
		++bin_idx;
	}

	if (bin_idx <= 127)
		flipBit(bin_idx);

	return *this;
}

QInt& QInt::operator--() {
	int bin_idx = 0;
	while (!getBit(bin_idx) && bin_idx <= 127) {
		flipBit(bin_idx);
		++bin_idx;
	}

	if (bin_idx <= 127)
		flipBit(bin_idx);

	return *this;
}

QInt& QInt::operator+=(const QInt& q) {
	__int64 res[2] = { 0, 0 };
	bool temp = 0;

	// 128 bits from 0 to 127
	for (int bin_idx = 0; bin_idx < 128; ++bin_idx) {
		bool a = getBit(bin_idx);
		bool b = q.getBit(bin_idx);
		
		bool c;
		if (a && b) {
			c = temp;
			temp = 1;
		}
		else {
			c = a + b;
			if (c && temp) {
				c = 0;
				temp = 1;
			}
			else {
				c |= temp;
				temp = 0;
			}
		}

		if (c) {
			if (bin_idx < 64)
				res[1] |= (1LL << bin_idx);
			else
				res[0] |= (1LL << (bin_idx - 64));
		}
	}

	m_bits[0] = res[0];
	m_bits[1] = res[1];

	return *this;
}

QInt& QInt::operator-=(const QInt& q) {
	QInt temp = q;
	temp.switchSign2Complement();
	(*this) += temp;
	return *this;
}

QInt& QInt::operator*=(const QInt& q) {
	QInt A;
	QInt Q = q;
	bool Q1 = 0;
	QInt* M = this;

	for (int k = 0; k < 128; ++k) {
		bool Q0 = Q.getBit(0);

		if (Q0 != Q1) {
			if (Q0)
				A -= *M;
			else
				A += *M;
		}

		Q1 = Q0;
		Q >>= 1;
		if (A.getBit(0) != Q.getBit(127))
			Q.flipBit(127);
		A >>= 1;
	}

	(*this) = Q;
	return *this;
}

QInt& QInt::operator/=(const QInt& q) {
	if (*this < q) {
		this->reset();
		return *this;
	}

	QInt A;
	QInt Q = *this;
	QInt M = q;

	int count_negative = 0;
	if (Q.getBit(127)) {
		++count_negative;
		Q.switchSign2Complement();
	}
	if (M.getBit(127)) {
		++count_negative;
		M.switchSign2Complement();
	}

	if(Q.getBit(127))
		A.scanDecimal("-1");
	
	for (int k = 0; k < 128; ++k) {
		A <<= 1;
		if (A.getBit(0) != Q.getBit(127))
			A.flipBit(0);
		Q <<= 1;
		
		A -= M;

		if (A.getBit(127)) {
			if (Q.getBit(0))
				Q.flipBit(0);
			A += M;
		}
		else
			if (!Q.getBit(0))
				Q.flipBit(0);
	}

	(*this) = Q;

	if (count_negative == 1)
		this->switchSign2Complement();

	return *this;
}

QInt& QInt::operator&=(const QInt& q) {
	for (int bin_index = 0; bin_index < 128; ++bin_index) {
		bool b = getBit(bin_index) & q.getBit(bin_index);
		if (b != getBit(bin_index))
			flipBit(bin_index);
	}
	return *this;
}

QInt& QInt::operator|=(const QInt& q) {
	for (int bin_index = 0; bin_index < 128; ++bin_index) {
		bool b = getBit(bin_index) | q.getBit(bin_index);
		if (b != getBit(bin_index))
			flipBit(bin_index);
	}
	return *this;
}

QInt& QInt::operator^=(const QInt& q) {
	for (int bin_index = 0; bin_index < 128; ++bin_index) {
		bool b = getBit(bin_index) ^ q.getBit(bin_index);
		if (b != getBit(bin_index))
			flipBit(bin_index);
	}
	return *this;
}

QInt& QInt::operator~() {
	flipBitAll();
	return *this;
}

QInt& QInt::operator<<=(int bits) {
	m_bits[0] <<= bits;
	for (int i = 63 + bits; i > 63; --i)
		if (i - bits >= 0 && i - bits <= 127 && i >= 0 && i <= 127 && getBit(i) != getBit(i - bits))
			flipBit(i);
	m_bits[1] <<= bits;
	return *this;
}

QInt& QInt::operator>>=(int bits) {
	m_bits[1] >>= bits;
	for (int i = 63; i > 63 - bits; --i) 
		if (i + bits >= 0 && i + bits <= 127 && i >= 0 && i <= 127 && getBit(i + bits) != getBit(i))
			flipBit(i);
	m_bits[0] >>= bits;
	return *this;
}

QInt& QInt::rol() {
	bool temp = getBit(127);
	(*this) <<= 1;
	if (getBit(0) != temp)
		flipBit(0);
	return *this;
}

QInt& QInt::ror() {
	bool temp = getBit(0);
	(*this) >>= 1;
	if (getBit(127) != temp)
		flipBit(127);
	return *this;
}
#include <iostream>
#include <math.h>
#include <regex>
#include <string>

static const long symsTotal = 32;
static const long alphaTotal = 26;
static const long numericTotal = 10;
static const long symRangesSize = 4;
static const long alphaRangeLowered[] = { 'a', 'z' };
static const long alphaRangeUppered[] = { 'A', 'Z' };
static const long numericRange[] = { '0', '9' };
static const long symRanges[symRangesSize][2] = {
	{ '!', '/' },
	{ ':', '@' },
	{ '[', '`' },
	{ '{', '~' }
};

bool isSymbol(char c) {
	for (long i = 0; i < symRangesSize; i++) {
		if (c >= symRanges[i][0] && c <= symRanges[i][1]) {
			return true;
		}
	}

	return false;
}

bool isAlphaLowered(char c) {
	return (c >= alphaRangeLowered[0] && c <= alphaRangeLowered[1]);
}

bool isAlphaUppered(char c) {
	return (c >= alphaRangeUppered[0] && c <= alphaRangeUppered[1]);
}

bool isNumeric(char c) {
	return (c >= numericRange[0] && c <= numericRange[1]);
}

long combinations(std::string& str) {
	long count = 0;
	long baseSize = 0;
	bool hasSymbols = false;
	bool hasAlphaLowered = false;
	bool hasAlphaUppered = false;
	bool hasNumeric = false;

	for (char &c : str) {
		if (isSymbol(c)) {
			hasSymbols = true;
		} else if (isAlphaLowered(c)) {
			hasAlphaLowered = true;
		} else if (isAlphaUppered(c)) {
			hasAlphaUppered = true;
		} else if (isNumeric(c)) {
			hasNumeric = true;
		}

		// Tracking this independently - we want the count of chars, not the
		// length of the std::string (they can be different).
		count++;
	}

	if (hasSymbols) {
		baseSize += symsTotal;
	}
	if (hasAlphaLowered) {
		baseSize += alphaTotal;
	}
	if (hasAlphaUppered) {
		baseSize += alphaTotal;
	}
	if (hasNumeric) {
		baseSize += numericTotal;
	}

	std::cout << "base size: " << baseSize << std::endl;
	std::cout << "count: " << count << std::endl;

	return pow(baseSize, count);
}

long bitSize(std::string& str) {
	long combs = combinations(str);
	return (long)log2((double)combs);
}

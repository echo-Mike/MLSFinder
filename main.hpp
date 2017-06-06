//General I/O
#include <iostream>
//File stream I/O
#include <fstream>
//String stream I/O
#include <sstream>
//Standard string lib
#include <string>
//Standard vector container lib
#include <vector>
//Unsupported in VS 2013
#define noexcept
/*
* External code usage: Taywee/args :: https://github.com/Taywee/args
* Copyright (c) 2016 Taylor C. Richberger <taywee@gmx.com>
* https://github.com/Taywee/args/blob/master/LICENSE
*/
#include "args.hxx"

////Defines

//#define DEBUG
#ifdef DEBUG
	#define DEBUG_OUT std::cerr
	#define DEBUG_END std::endl
#endif

////Structures and types

//Sructure type of input parameters
struct Parameters {
	int power;
	int length;
	std::string inPath;
	std::string outPath;
	bool consoleOut;
};

enum EXIT_CODES : int {
	NORMAL,
	PARSE_ERROR,
	VALIDATION_ERROR,
	NO_POWER,
	NO_INPUT,
	READ_ERROR,
	INPUT_FILE_DOES_NOT_EXIST,
	NO_SEQUENCES,
	WRITE_ERROR,
	CANT_OPEN_OUTPUT_FILE
};

////INLINE functions

#ifdef DEBUG

	template < class T >
	inline void consoleCheck(std::string desc, T _value) { std::cout << desc << _value << std::endl; }

	template < class T >
	void consoleCheckArray(std::string desc, T& _array, int _count) {
		for (int _index = 0; _index < _count; _index++)
			std::cout << desc << std::to_string(_index) << " " << _array[_index] << std::endl;
	}

#endif

inline int toBit(char _bitS) { return _bitS == '1'; };

std::string getZerosString(int _power) {
	std::string _result;
	for (int _index = 1; _index < _power; _index++)
		_result += "0";
	return _result;
}

#ifdef OLD
	std::string createPolyString(std::vector<char>& _vec) {
		std::string _result;
		for (auto v : _vec)
			_result.push_back((bool)v ? '1' : '0');
		return _result;
	}
#endif

std::string createPolyString(const char* _vec, int _length) {
	std::string _result;
	for (int _index = 0; _index < _length; _index++)
		_result.push_back(_vec[_index] ? '1' : '0');
	return _result;
}

//https://stackoverflow.com/questions/4034750/find-all-a-substrings-occurrences-and-locations
void findPositions(std::string& _str, std::string& _sub, std::vector<size_t>& _result) {
	size_t pos = _str.find(_sub, 0);
	while (pos != std::string::npos)
	{
		_result.push_back(pos);
		pos = _str.find(_sub, pos + 1);
	}
}

////Functions

//Read input file
std::string readFile(std::string fileName);

//Write to output file
void writeFile(std::string fileName, std::stringstream& _content);

//Parse input args
void parse_args(int argc, char* argv[], Parameters& params);

/* Implementation of Berlekamp–Massey algorithm
* https://en.wikipedia.org/wiki/Berlekamp%E2%80%93Massey_algorithm
*/
std::string BMalgorithm(std::string& _seq, int _length);
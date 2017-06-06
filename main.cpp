#include "main.hpp"

int main(int argc, char* argv[])
{
	//Читаем параметры
	Parameters params;
	parse_args(argc, argv, params);

	//Получаем длинну искомой последовательности
	params.length = (1 << params.power) - 1;

	#ifdef DEBUG
		consoleCheck<int>("params.length: ", params.length);
	#endif

	//Создаём подпоследовательность нулей
	std::string zeros = getZerosString(params.power);

	#ifdef DEBUG
		consoleCheck<std::string>("zeros: ", zeros);
	#endif

	//Читаем содержимое входного файла
	std::string content = readFile(params.inPath);

	#ifdef DEBUG
		consoleCheck<std::string>("content: ", content);
	#endif

	//Ищем позиции всех подпоследовательностей нулей указаной длинны
	std::vector<size_t> positions;
	findPositions(content, zeros, positions);

	if (!positions.size()) {
		std::cout << "ERROR::MAIN::No suitable sequences found" << std::endl;
		exit(EXIT_CODES::NO_SEQUENCES);
	}

	#ifdef DEBUG
		consoleCheckArray<std::vector<size_t>>("Positions: ", positions, positions.size());	
	#endif

	std::cout << "Possible sequences found: " << positions.size() << std::endl;
	
	//Создаём поток строкового вывода для вывода в файл или консоль
	std::stringstream outString;
	outString << "Position\tLFSR\tSequence" << std::endl;

	//Для каждой найденной последовательности производим анализ алгоритмом БМ
	int _counter = 0;
	for (auto v : positions) {
		//Копируем последовательность
		std::string _buff = content.substr(v + params.power - 1, params.length - params.power + 1);
		//Проверяем длинну на соответсвие длинне последовательности
		if ((int)_buff.length() < params.length - params.power + 1)
			continue;
		//Алгоритм БМ над скопированной последовательностью
		std::string stepResult = std::move(BMalgorithm(_buff + zeros, params.length));
		//Вывод результата
		size_t pos = stepResult.rfind("1");
		if (pos == 0) {
			#ifdef DEBUG
				std::cout << "Sequence number: " << _counter << " Status: Zerro code" << std::endl;
			#endif
		} else {
			if (pos == params.power) {
				std::cout << "Sequence number: " << _counter << " Status: Correct code" << std::endl;
				#ifdef DEBUG
					consoleCheck<std::string>("Step result string: ", stepResult.substr(0, pos+1));
				#endif
				outString << v << "\t";
				outString << stepResult.substr(0, pos + 1) << "\t";
				outString << zeros + _buff << std::endl;
			} else {
				#ifdef DEBUG
					std::cout << "Sequence number: " << _counter << " Status: Incorrect code" << std::endl;
					consoleCheck<std::string>("Step result string: ", stepResult);
				#endif
			}
		}
		_counter++;
	}

	//Выводим содержимое строквого потока
	if (!params.consoleOut) {
		std::cout << "Writing results to file: " << params.outPath << std::endl;
		writeFile(params.outPath, outString);
	} else {
		std::cout << outString.rdbuf();
	}

	#ifdef DEBUG
		//Ждём выхода из программы
		char endWait;
		std::cout << "Press ENTER to exit" << std::endl;
		endWait = getchar();
	#endif
	return EXIT_CODES::NORMAL;
}

std::string BMalgorithm(std::string& _seq, int _length) {
	//Memory management
	char *b, *t, *c;
	b = new char[_length];
	t = new char[_length];
	c = new char[_length];
	b[0] = 1;
	for (int _index = 1; _index < _length; _index++)
		b[_index] = 0;
	std::memcpy(c, b, _length);

	//Variables
	int N, L, m;
	N = L = 0;
	m = -1;
	char d = 0;

	//Algorithm
	while (N < _length) {
		d = 0;
		for (int _index = N - L; _index <= N; _index++)
			d ^= toBit(_seq[_index]) & c[N - _index];

		if (d) {
			std::memcpy(t, c, _length);

			for (int _index = N - m; _index <= _length - 1; _index++)
				c[_index] = c[_index] ^ b[_index - (N - m)];

			if (2 * L <= N) {
				L = N + 1 - L;
				m = N;
				std::memcpy(b, t, _length);
			}
		}
		N++;
	}

	//Memory management
	delete b;
	delete t;
	std::string _result = std::move(createPolyString(c, _length));
	delete c;
	return _result;
}

std::string readFile(std::string fileName) {
	std::ifstream inpFile;
	std::string content;
	inpFile.exceptions(std::ifstream::badbit || std::ifstream::failbit);
	try {
		inpFile.open(fileName);
		if (inpFile.is_open()) {
			std::stringstream contentStream;
			contentStream << inpFile.rdbuf();
			inpFile.close();
			content = contentStream.str();
		} else {
			throw std::exception();
		}
	}
	catch (std::ifstream::failure e) {
		#ifdef DEBUG
			DEBUG_OUT << "ERROR::readFile::Read file failure" << DEBUG_END;
		#endif
		exit(EXIT_CODES::READ_ERROR);
	}
	catch (...) {
		#ifdef DEBUG
			DEBUG_OUT << "ERROR::readFile::Can't open file" << DEBUG_END;
		#endif
		exit(EXIT_CODES::INPUT_FILE_DOES_NOT_EXIST);
	}
	return content;
}

void writeFile(std::string fileName, std::stringstream& _content) {
	std::ofstream outFile;
	outFile.exceptions(std::ofstream::badbit || std::ofstream::failbit);
	try {
		outFile.open(fileName, std::ofstream::out);
		if (outFile.is_open()) {
			outFile << _content.rdbuf();
			outFile.close();
		} else {
			throw std::exception();
		}
	}
	catch (std::ofstream::failure e) {
		#ifdef DEBUG
			DEBUG_OUT << "ERROR::writeFile::Write file failure" << DEBUG_END;
		#endif
		exit(EXIT_CODES::WRITE_ERROR);
	}
	catch (...) {
		#ifdef DEBUG
			DEBUG_OUT << "ERROR::writeFile::Can't open file" << DEBUG_END;
		#endif
		exit(EXIT_CODES::CANT_OPEN_OUTPUT_FILE);
	}
}

void parse_args(int argc, char* argv[], Parameters& params) {

	args::ArgumentParser parser("This program can find all M-sequence in provided file.", "Author: https://github.com/echo-Mike");
	args::HelpFlag help(parser, "help", "Provide help information", { 'h', "help" });
	args::ValueFlag<int> power(parser, "power", "Expected power of creational polynom", { 'p', "power" });
	args::ValueFlag<std::string> output(parser, "path", "Output file path", { 'o', "output" });
	args::ValueFlag<std::string> input(parser, "path", "Input file path", { 'i', "input" });

	try	{
		parser.ParseCLI(argc, argv);
	}
	catch (args::Help) {
		std::cout << parser;
		#ifdef DEBUG
			char endWait;
			std::cout << "Press ENTER to exit" << std::endl;
			endWait = getchar();
		#endif
		exit(EXIT_CODES::NORMAL);
	}
	catch (args::ParseError e) {
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		exit(EXIT_CODES::PARSE_ERROR);
	}
	catch (args::ValidationError e)	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		exit(EXIT_CODES::VALIDATION_ERROR);
	}

	if (power) { 
		params.power = args::get(power); 
	} else {
		std::cout << "ERROR::Parsing::No estimated polynom power provided." << std::endl;
		std::cout << "Use -h for help." << std::endl;
		exit(EXIT_CODES::NO_POWER);
	}

	if (output) {
		params.outPath = args::get(output);
		params.consoleOut = false;
	} else {
		std::cout << "No output file path provided. Output directed to console." << std::endl;
		params.consoleOut = true;
		params.outPath = "";
	}

	if (input) {
		params.inPath = args::get(input);
	} else {
		std::cout << "ERROR::Parsing::No input file path provided." << std::endl;
		exit(EXIT_CODES::NO_INPUT);
	}
}
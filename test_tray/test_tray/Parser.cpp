#include "Parser.h"



Parser::Parser(std::fstream * file, const int buf_size)
{
	inf_file_ = file;
	key = "open";
	buf_lenght_ = buf_size;
	buf_ = new char[buf_lenght_];
}

std::cmatch Parser::parse_loop(std::string key)
{
	std::cmatch result;
	std::string reg_str = "(";
	reg_str += key;
	reg_str += R"([\s]?={1}[\s]?)([\S\s]*))";
	std::string reg_coment_str = ";";
	const std::regex regular(reg_str.c_str());
	const std::regex rregex(reg_coment_str.c_str());
	while (!inf_file_->eof()) {

		inf_file_->getline(buf_, buf_lenght_);
		std::regex_search(buf_, result, rregex);
		if (!result.empty()) { continue; }
		std::regex_search(buf_, result, regular);

		if (!result.empty()) { break; }
		if (inf_file_->eof()) { return result; }
	}
	return result;
}


Parser::~Parser()
{
	delete[] buf_;
}

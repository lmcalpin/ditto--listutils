#pragma once

#include <vector>
#include <string>
#include <sstream>

enum ConversionType
{
	CONVERT_LIST_TO_CSV,
	CONVERT_CSV_TO_LIST,
	TRIM,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
};

typedef void (*conversionFunction)(std::stringstream& outputbuffer, std::stringstream& tempbuffer);

class TextProcessor
{
public:
	TextProcessor(ConversionType conversionType);
	~TextProcessor(void);

	std::string Process(std::string input);
private:
	static void TextProcessor::TrimLine(std::string& str);

	static void TextProcessor::NoConversion(std::stringstream& outputbuffer, std::stringstream& tempbuffer);
	static void TextProcessor::Trim(std::stringstream& outputbuffer, std::stringstream& tempbuffer);
	static void TextProcessor::SingleQuote(std::stringstream& outputbuffer, std::stringstream& tempbuffer);
	static void TextProcessor::DoubleQuote(std::stringstream& outputbuffer, std::stringstream& tempbuffer);
	
	void AppendOutputDelimiter(std::stringstream& ss);

	char inputDelimiter;
	char outputDelimiter;
	conversionFunction converter;
};

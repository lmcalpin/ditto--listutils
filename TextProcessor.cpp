#include "StdAfx.h"
#include "TextProcessor.h"
#include "SelectPasteFormat.h"

const std::string WHITESPACE = " \n\r\t";

TextProcessor::TextProcessor(ConversionType conversionType)
{
	inputDelimiter = '\n';
	outputDelimiter = '\n';
	switch(conversionType)
	{
	case CONVERT_LIST_TO_CSV:
		outputDelimiter = ',';
		converter = TextProcessor::NoConversion;
		break;
	case CONVERT_CSV_TO_LIST:
		inputDelimiter = ',';
		converter = TextProcessor::NoConversion;
		break;
	case TRIM:
		converter = TextProcessor::Trim;
		break;
	case SINGLE_QUOTE:
		converter = TextProcessor::SingleQuote;
		break;
	case DOUBLE_QUOTE:
		converter = TextProcessor::DoubleQuote;
		break;
	default:
		converter = TextProcessor::NoConversion;
	}
}

TextProcessor::~TextProcessor(void)
{
}

bool TextProcessor::Convert(const CDittoInfo &DittoInfo, IClip *pClip, ConversionType conversionType)
{
	bool ret = false;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	IClipFormats *pFormats = pClip->Clips();

	CWnd* pWnd = CWnd::FromHandle(DittoInfo.m_hWndDitto);

	TextProcessor textProcessor(conversionType);
	IClipFormat *pText = pFormats->FindFormatEx(CF_TEXT);
	if(pText != NULL)
	{
		//We own the data, when we call DeleteAll tell it to not free the data
		pText->AutoDeleteData(false);
		HGLOBAL data = pText->Data();

		char *stringData = (char *)GlobalLock(data);

		int size = (int)GlobalSize(data);
		if(stringData != NULL)
		{
			std::string str(stringData);
			textProcessor.Process(str);
			size_t size = GlobalSize(pText->Data());
			if (str.length() > size) {
				GlobalFree(data);
				size = str.length() + 1;
				data = GlobalAlloc(GHND, size);
				stringData = (char *)GlobalLock(data);
			}
			strcpy_s(stringData, size, str.c_str());
		}
		GlobalUnlock(data);

		//Remove all over formats and add the selected date back as CF_TEXT
		pFormats->DeleteAll();
		pFormats->AddNew(CF_TEXT, data);
		IClipFormat *pText = pFormats->FindFormatEx(CF_TEXT);
		if(pText != NULL)
		{
			pText->AutoDeleteData(true);
		}
		ret = true;
	}
	return ret;
}

void TextProcessor::NoConversion(std::stringstream& outputbuffer, std::stringstream& tempbuffer)
{
	std::string temp = tempbuffer.str();
	outputbuffer << temp;
}

void TextProcessor::DoubleQuote(std::stringstream& outputbuffer, std::stringstream& tempbuffer)
{
	std::string temp = tempbuffer.str();
	outputbuffer << "\"" << temp << "\"";
}

void TextProcessor::SingleQuote(std::stringstream& outputbuffer, std::stringstream& tempbuffer)
{
	std::string temp = tempbuffer.str();
	outputbuffer << "'" << temp << "'";
}

void TextProcessor::Trim(std::stringstream& outputbuffer, std::stringstream& tempbuffer)
{
	std::string temp = tempbuffer.str();
	TrimLine(temp);
	outputbuffer << temp;
}

void TextProcessor::Process(std::string& str)
{
	std::stringstream outputbuffer;
	std::stringstream tempbuffer;

	for (unsigned int i = 0; i < str.length(); i++)
	{
		char c = str[i];
		if (inputDelimiter == '\n' && c == '\r')
			continue;
		if (c == inputDelimiter)
		{
			converter(outputbuffer, tempbuffer);
			outputbuffer << outputDelimiter;
			tempbuffer.str("");
			continue;
		}
		tempbuffer << c;
	}
	if (tempbuffer.str().length())
	{
		converter(outputbuffer, tempbuffer);
		outputbuffer << outputDelimiter;
	}
	std::string row = outputbuffer.str();
	TrimLine(row);
	str = row;
}

void TextProcessor::TrimLine(std::string& str)
{
	std::string::size_type pos1 = str.find_first_not_of(WHITESPACE);
	std::string::size_type pos2 = str.find_last_not_of(WHITESPACE);
	str = str.substr(pos1 == std::string::npos ? 0 : pos1, 
		pos2 == std::string::npos ? str.length() - 1 : pos2 - pos1 + 1);
}


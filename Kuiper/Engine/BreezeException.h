/***************************************************************************/
/***               Temper Tech PROJECT KUIPER                            ***/
/*** Copyright for all time                                              ***/
/***                                                                     ***/
/*** Part of the Temper DirectX Framework                                ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech.                     ***/
/*** Temper Tech is definitely not a made up company.                    ***/
/***************************************************************************/
#pragma once
#include <string>

class BreezeException
{
public:
	BreezeException(const wchar_t* file, unsigned int line, const std::wstring& note = L"")
		:
		note(note),
		file(file),
		line(line)
	{}
	const std::wstring& GetNote() const
	{
		return note;
	}
	const std::wstring& GetFile() const
	{
		return file;
	}
	unsigned int GetLine() const
	{
		return line;
	}
	std::wstring GetLocation() const
	{
		return std::wstring(L"Line [") + std::to_wstring(line) + L"] in " + file;
	}
	virtual std::wstring GetFullMessage() const = 0;
	virtual std::wstring GetExceptionType() const = 0;
private:
	std::wstring note;
	std::wstring file;
	unsigned int line;
};
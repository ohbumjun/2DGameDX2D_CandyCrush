#pragma once

#include "GameInfo.h"
#include <filesystem>

class CEngineUtil
{
public :
	static const size_t CalculatePadding(const size_t baseAddress,
		const size_t alignment);
	
	static const size_t CalculatePaddingWithHeader(const size_t baseAddress,
		const size_t alignment, const size_t HeaderSize);
	
	static const void ExtractFilePathAfterPathName(const std::string& PathName, std::string_view FullPath, 
		std::string_view Extracted);

	static const void ExtractFilePathAfterPathName(const std::wstring& PathName, std::wstring_view FullPath,
		std::wstring_view Extracted);

	static const void ExtractFullPathFromFileName(std::wstring_view PathName, std::wstring_view FileName, std::wstring& ExtractedPath);
};
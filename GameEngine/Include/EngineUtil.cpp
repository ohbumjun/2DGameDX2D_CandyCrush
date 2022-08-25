#include "EngineUtil.h"

namespace fs = std::filesystem;

const size_t CEngineUtil::CalculatePadding(const size_t baseAddress, const size_t alignment)
{
	const size_t multiplier = (baseAddress / alignment) + 1;
	const size_t alignedAddress = (multiplier * alignment);
	const size_t padding = alignedAddress - baseAddress;
	return padding;
}

const size_t CEngineUtil::CalculatePaddingWithHeader(const size_t baseAddress, const size_t alignment, const size_t HeaderSize)
{
	size_t padding = CalculatePadding(baseAddress, alignment);

	size_t neededSpace = HeaderSize;

	if (neededSpace > padding)
	{
		neededSpace = neededSpace - padding;

		if (neededSpace % alignment > 0)
			padding += alignment * (1 + (neededSpace / alignment));
		else
			padding += alignment * (neededSpace / alignment);
	}

	return padding;
}

const void CEngineUtil::ExtractFilePathAfterPathName(const std::string& PathName, std::string_view FullPath, 
	std::string_view Extracted)
{
	auto Iter = std::search(FullPath.begin(), FullPath.end(),
			std::boyer_moore_searcher(PathName.begin(), PathName.end()));

	if (Iter == FullPath.end())
		assert(false);

	int Distance = std::distance(FullPath.begin(), Iter);

	Extracted = FullPath.substr((size_t)Distance + 1, FullPath.length() - Distance);
}

const void CEngineUtil::ExtractFilePathAfterPathName(const std::wstring& PathName, 
	std::wstring_view FullPath, std::wstring_view Extracted)
{
	auto Iter = std::search(FullPath.begin(), FullPath.end(),
		std::boyer_moore_searcher(PathName.begin(), PathName.end()));

	if (Iter == FullPath.end())
		assert(false);

	int Distance = std::distance(FullPath.begin(), Iter);

	Extracted = FullPath.substr((size_t)Distance + 1, FullPath.length() - Distance);
}

const void CEngineUtil::ExtractFullPathFromFileName(
	std::wstring_view PathName, std::wstring_view FileName, 
	std::wstring& ExtractedPath)
{
	fs::recursive_directory_iterator itr(PathName);

	while (itr != fs::end(itr)) {

		const fs::directory_entry& entry = *itr;

		if (entry.path().filename() == FileName)
		{
			ExtractedPath = entry.path();
			return;
		}

		itr++;
	}
}

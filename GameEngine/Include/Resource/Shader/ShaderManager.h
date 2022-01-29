#pragma once

#include "../../GameInfo.h"

class CShaderManager
{
	friend class CResourceManager;
private :
	CShaderManager();
	~CShaderManager();
private :
	std::unordered_map<std::string, CSharedPtr<class CShader>> m_mapShader;
	std::unordered_map<std::string, CSharedPtr<class CConstantBufferBase>> m_mapConstantBuffer;
public :
	bool Init();
	class CShader* FindShader(const std::string& Name);
	class CConstantBufferBase* FindConstantBuffer(const std::string& Name);

};


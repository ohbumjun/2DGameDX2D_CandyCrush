#pragma once

#include "../../GameInfo.h"
#include "Shader.h"

class CShaderManager
{
	friend class CResourceManager;
private :
	CShaderManager();
	~CShaderManager();
private :
	std::unordered_map<std::string, CSharedPtr<class CShader>> m_mapShader;
	std::unordered_map<std::string, CSharedPtr<class CConstantBuffer>> m_mapConstantBuffer;
public :
	bool Init();
	class CShader* FindShader(const std::string& Name);
	class CConstantBuffer* FindConstantBuffer(const std::string& Name);
public :
	bool CreateConstantBuffer(const std::string& Name, int Register, int Size, int ShaderType);
	template<typename T>
	bool CreateShader(const std::string& Name)
	{
		CShader* Shader = FindShader(Name);

		if (Shader)
			return true;

		Shader = new T;
		Shader->SetName(Name);

		if (!Shader->Init())
		{
			SAFE_DELETE(Shader);
			return false;
		}

		m_mapShader.insert(std::make_pair(Name, Shader));

		return true;
	}

};


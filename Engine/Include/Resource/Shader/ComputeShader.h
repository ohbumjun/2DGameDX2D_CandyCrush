#pragma once

#include "Shader.h"

class CComputeShader : public CShader
{
	friend class CShaderManager;

protected :
	CComputeShader();
	CComputeShader(const CComputeShader& Shader);
	virtual ~CComputeShader();
private :
	ID3D11ComputeShader* m_CSShader;
	ID3DBlob* m_CSBlob;
	char m_EntryName[64];
	TCHAR m_FileName[MAX_PATH];
	std::string m_PathName;
public :
	virtual bool Init() override;
	bool LoadComputeShader(const char* EntryName, const TCHAR* FileNAme,
		const std::string& PathName);
	void Execute(unsigned int x, unsigned int y, unsigned int z);
	virtual void SetShader() override;
};


#include "ComputeShader.h"
#include "../../PathManager.h"
#include "../../Device.h"

CComputeShader::CComputeShader() :
m_CSBlob(nullptr),
m_CSShader(nullptr)
{
	m_ShaderType = Shader_Type::Compute;
}

CComputeShader::CComputeShader(const CComputeShader& Shader)
{
	LoadComputeShader(Shader.m_EntryName, Shader.m_FileName, Shader.m_PathName);
}

CComputeShader::~CComputeShader()
{
	SAFE_RELEASE(m_CSBlob);
	SAFE_RELEASE(m_CSShader);
}

bool CComputeShader::Init()
{
	if (!CShader::Init())
		return false;

	return true;
}
bool CComputeShader::LoadComputeShader(const char* EntryName, const TCHAR* FileName, const std::string& PathName)
{
	SAFE_RELEASE(m_CSShader);
	SAFE_RELEASE(m_CSBlob);

	memset(m_EntryName, 0, sizeof(64));
	strcpy_s(m_EntryName, EntryName);

	memset(m_FileName, 0, sizeof(MAX_PATH));
	lstrcpy(m_FileName, FileName);

	m_PathName = PathName;

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag |= D3DCOMPILE_DEBUG;
#endif

	TCHAR FullPath[MAX_PATH] = {};

	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);
	if (Path)
		lstrcpy(FullPath, Path->Path);
	lstrcat(FullPath, FileName);

	ID3DBlob* Error = nullptr;

	if (FAILED(D3DCompileFromFile(FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		EntryName, "cs_5_0", Flag, 0, &m_CSBlob, &Error)))
	{
		OutputDebugStringA((char*)Error->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// 해당 Blob으로 Shader 코드를 만든다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateComputeShader(
		m_CSBlob->GetBufferPointer(),
		m_CSBlob->GetBufferSize(),
		nullptr,
		&m_CSShader)))
		return false;

	return true;
}

void CComputeShader::Execute(unsigned x, unsigned y, unsigned z)
{
	SetShader();

	CDevice::GetInst()->GetDeviceContext()->Dispatch(x, y, z);

	// 다시 원상 복구하는 코드
	CDevice::GetInst()->GetDeviceContext()->CSSetShader(nullptr, nullptr, 0);
}

void CComputeShader::SetShader()
{
	CDevice::GetInst()->GetDeviceContext()->CSSetShader(m_CSShader, nullptr, 0);
}



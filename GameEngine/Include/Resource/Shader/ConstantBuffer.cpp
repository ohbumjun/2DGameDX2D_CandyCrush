#include "ConstantBuffer.h"
#include "../../Device.h"

CConstantBuffer::CConstantBuffer() :
	m_Buffer(nullptr),
	m_Register(0),
	m_Size(0),
	m_ConstantBufferShaderType(0)
{}

CConstantBuffer::CConstantBuffer(const CConstantBuffer& Buffer)
{
	*this = Buffer;
}

CConstantBuffer::~CConstantBuffer()
{
	SAFE_RELEASE(m_Buffer);
}

bool CConstantBuffer::Init(int Size, int Register, int ConstantBufferShaderType)
{
	m_Size = Size;
	m_Register = Register;
	m_ConstantBufferShaderType = ConstantBufferShaderType;

	D3D11_BUFFER_DESC Desc = {};
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.ByteWidth = m_Size;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Desc.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, nullptr, &m_Buffer)))
		return false;

	return true;
}

void CConstantBuffer::UpdateBuffer(void* Data)
{
	D3D11_MAPPED_SUBRESOURCE Map = {};

	CDevice::GetInst()->GetDeviceContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD,
		0, &Map);

	memcpy(Map.pData, Data, m_Size);

	CDevice::GetInst()->GetDeviceContext()->Unmap(m_Buffer, 0);

	// 셰이더 프로그램에 상수 버퍼 넘겨주기

	if (m_ConstantBufferShaderType & static_cast<int>(Buffer_Shader_Type::Vertex))
		CDevice::GetInst()->GetDeviceContext()->VSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & static_cast<int>(Buffer_Shader_Type::Pixel))
		CDevice::GetInst()->GetDeviceContext()->PSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & static_cast<int>(Buffer_Shader_Type::Domain))
		CDevice::GetInst()->GetDeviceContext()->DSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & static_cast<int>(Buffer_Shader_Type::Hull))
		CDevice::GetInst()->GetDeviceContext()->HSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & static_cast<int>(Buffer_Shader_Type::Geometry))
		CDevice::GetInst()->GetDeviceContext()->GSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & static_cast<int>(Buffer_Shader_Type::Compute))
		CDevice::GetInst()->GetDeviceContext()->CSSetConstantBuffers(m_Register, 1, &m_Buffer);


}

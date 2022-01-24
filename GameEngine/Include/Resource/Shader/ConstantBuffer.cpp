#include "ConstantBuffer.h"
#include "../../Device.h"

CConstantBuffer::CConstantBuffer()
{}

CConstantBuffer::CConstantBuffer(const CConstantBuffer& Buffer)
{}

CConstantBuffer::~CConstantBuffer()
{}

bool CConstantBuffer::Init(int Register, int Size, int ConstantShaderType)
{
	m_Size = Size;
	m_Register = Register;
	m_ConstantShaderType = ConstantShaderType;

	// 실제 버퍼 생성
	D3D11_BUFFER_DESC Desc = {};
	Desc.ByteWidth = m_Size;
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, nullptr, &m_Buffer)))
		return false;

	return true;
}

void CConstantBuffer::UpdateCBuffer(void* Data)
{
	D3D11_MAPPED_SUBRESOURCE Maps = {};

	// Mapping ( cpu로 cpu 자원 조작 )
	CDevice::GetInst()->GetDeviceContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE,
		0, &Maps);
	memcpy(&Maps.pData, Data, m_Size);
	CDevice::GetInst()->GetDeviceContext()->Unmap(m_Buffer, 0);

	// 상수 버퍼 셰이더 단계에 넘겨주기
	if (m_ConstantShaderType & (int)ConstantBuffer_Shader_Type::Vertex)
		CDevice::GetInst()->GetDeviceContext()->VSSetConstantBuffers(m_Register, 1, &m_Buffer);
	if (m_ConstantShaderType & (int)ConstantBuffer_Shader_Type::Pixel)
		CDevice::GetInst()->GetDeviceContext()->PSSetConstantBuffers(m_Register, 1, &m_Buffer);
	if (m_ConstantShaderType & (int)ConstantBuffer_Shader_Type::Hull)
		CDevice::GetInst()->GetDeviceContext()->HSSetConstantBuffers(m_Register, 1, &m_Buffer);
	if (m_ConstantShaderType & (int)ConstantBuffer_Shader_Type::Domain)
		CDevice::GetInst()->GetDeviceContext()->DSSetConstantBuffers(m_Register, 1, &m_Buffer);
	if (m_ConstantShaderType & (int)ConstantBuffer_Shader_Type::Geometry)
		CDevice::GetInst()->GetDeviceContext()->GSSetConstantBuffers(m_Register, 1, &m_Buffer);
	if (m_ConstantShaderType & (int)ConstantBuffer_Shader_Type::Compute)
		CDevice::GetInst()->GetDeviceContext()->CSSetConstantBuffers(m_Register, 1, &m_Buffer);

}

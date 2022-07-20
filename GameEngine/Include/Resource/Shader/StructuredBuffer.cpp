#include "StructuredBuffer.h"
#include "../../Device.h"

 CStructuredBuffer::CStructuredBuffer() :
	 m_Desc{},
	 m_Buffer(nullptr),
	 m_SRV(nullptr),
	 m_UAV(nullptr)
{
 }

 CStructuredBuffer::CStructuredBuffer(const CStructuredBuffer& Buffer)
{
	 *this = Buffer;

	 CDevice::GetInst()->GetDevice()->CreateBuffer(&m_Desc, nullptr, &m_Buffer);

	 D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	 SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	 SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	 SRVDesc.BufferEx.FirstElement = 0;
	 SRVDesc.BufferEx.Flags = 0;
	 SRVDesc.BufferEx.NumElements = m_Count;

	 CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_Buffer, &SRVDesc, &m_SRV);

	 if (!m_Dynamic)
	 {
		 D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		 UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		 UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		 UAVDesc.Buffer.FirstElement = 0;
		 UAVDesc.Buffer.Flags = 0;
		 UAVDesc.Buffer.NumElements = m_Count;

		 CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_Buffer, &UAVDesc, &m_UAV);
	 }
 }

 CStructuredBuffer::~CStructuredBuffer()
{
	 SAFE_RELEASE(m_SRV);
	 SAFE_RELEASE(m_UAV);
	 SAFE_RELEASE(m_Buffer);
 }

 bool CStructuredBuffer::Init(const std::string& Name, unsigned Size, unsigned Count, int Register, bool Dynamic,
	int StructuredBufferShaderType)
{
	 SAFE_RELEASE(m_SRV);
	 SAFE_RELEASE(m_UAV);
	 SAFE_RELEASE(m_Buffer);

	 m_Dynamic = Dynamic;
	 m_Name = Name;
	 m_Size = Size;
	 m_Count = Count;
	 m_Register = Register;
	 m_StructuredBufferShaderType = StructuredBufferShaderType;

	 m_Desc.ByteWidth = m_Size * m_Count;
	 m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	 m_Desc.StructureByteStride = m_Size;

	 if (m_Dynamic)
	 {
		 m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		 m_Desc.Usage = D3D11_USAGE_DYNAMIC;
		 m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	 }
	 else
	 {
		 m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		 m_Desc.Usage = D3D11_USAGE_DEFAULT;
	 }

	 if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_Desc, nullptr, &m_Buffer)))
		 return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.Flags = 0;
	SRVDesc.BufferEx.NumElements = m_Count;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_Buffer, &SRVDesc, &m_SRV)))
		return false;

	if (!m_Dynamic)
	{
		// 순서없는 접근 뷰
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};

		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.Flags = 0;
		UAVDesc.Buffer.NumElements = m_Count;

		if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_Buffer, &UAVDesc, &m_UAV)))
			return false;
	}

	 return true;
 }

 void CStructuredBuffer::UpdateBuffer(void* Data, int Count)
{
	 if (!m_Dynamic)
		 return;

	 D3D11_MAPPED_SUBRESOURCE Map = {};

	 CDevice::GetInst()->GetDeviceContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
		 &Map);

	 memcpy(Map.pData, Data, m_Size * Count);

	 CDevice::GetInst()->GetDeviceContext()->Unmap(m_Buffer, 0);
 }

 CStructuredBuffer* CStructuredBuffer::Clone()
{
	 return new CStructuredBuffer(*this);
 }

 void CStructuredBuffer::SetShader()
{
	 if (m_Dynamic)
	 {
		 if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Vertex)
			 CDevice::GetInst()->GetDeviceContext()->VSSetShaderResources(m_Register, 1, &m_SRV);
		 if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Pixel)
			 CDevice::GetInst()->GetDeviceContext()->PSSetShaderResources(m_Register, 1, &m_SRV);
		 if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Domain)
			 CDevice::GetInst()->GetDeviceContext()->DSSetShaderResources(m_Register, 1, &m_SRV);
		 if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Hull)
			 CDevice::GetInst()->GetDeviceContext()->HSSetShaderResources(m_Register, 1, &m_SRV);
		 if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Geometry)
			 CDevice::GetInst()->GetDeviceContext()->GSSetShaderResources(m_Register, 1, &m_SRV);
		 if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Compute)
			 CDevice::GetInst()->GetDeviceContext()->CSSetShaderResources(m_Register, 1, &m_SRV);
	}
	 else
	 {
		 unsigned int Count = 1;
		 CDevice::GetInst()->GetDeviceContext()->CSSetUnorderedAccessViews(m_Register, 1, &m_UAV,
			 &Count);
	 }
 }

 void CStructuredBuffer::ResetShader()
{
	 if (m_Dynamic)
	 {
		 ID3D11ShaderResourceView* SRV = nullptr;

		 if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Vertex)
			 CDevice::GetInst()->GetDeviceContext()->VSSetShaderResources(m_Register, 1, &m_SRV);
		 if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Pixel)
			 CDevice::GetInst()->GetDeviceContext()->PSSetShaderResources(m_Register, 1, &m_SRV);
		 if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Domain)
			 CDevice::GetInst()->GetDeviceContext()->DSSetShaderResources(m_Register, 1, &m_SRV);
		 if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Hull)
			 CDevice::GetInst()->GetDeviceContext()->HSSetShaderResources(m_Register, 1, &m_SRV);
		 if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Geometry)
			 CDevice::GetInst()->GetDeviceContext()->GSSetShaderResources(m_Register, 1, &m_SRV);
		 if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Compute)
			 CDevice::GetInst()->GetDeviceContext()->CSSetShaderResources(m_Register, 1, &m_SRV);
	}
	 else
	 {
		 ID3D11UnorderedAccessView* UAV = nullptr;
		 unsigned int Count = -1;
		 CDevice::GetInst()->GetDeviceContext()->CSSetUnorderedAccessViews(m_Register, 1,
			 &UAV, &Count);
	 }
}

 void CStructuredBuffer::SetShader(int Register, int StructuredBufferShaderType)
{
	 if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Vertex)
		 CDevice::GetInst()->GetDeviceContext()->VSSetShaderResources(Register, 1, &m_SRV);

	 if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Pixel)
		 CDevice::GetInst()->GetDeviceContext()->PSSetShaderResources(Register, 1, &m_SRV);

	 if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Domain)
		 CDevice::GetInst()->GetDeviceContext()->DSSetShaderResources(Register, 1, &m_SRV);

	 if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Hull)
		 CDevice::GetInst()->GetDeviceContext()->HSSetShaderResources(Register, 1, &m_SRV);

	 if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Geometry)
		 CDevice::GetInst()->GetDeviceContext()->GSSetShaderResources(Register, 1, &m_SRV);

	 if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Compute)
		 CDevice::GetInst()->GetDeviceContext()->CSSetShaderResources(Register, 1, &m_SRV);
 }

 void CStructuredBuffer::ResetShader(int Register, int StructuredBufferShaderType)
{
	 ID3D11ShaderResourceView* SRV = nullptr;

	 if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Vertex)
		 CDevice::GetInst()->GetDeviceContext()->VSSetShaderResources(Register, 1, &SRV);

	 if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Pixel)
		 CDevice::GetInst()->GetDeviceContext()->PSSetShaderResources(Register, 1, &SRV);

	 if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Domain)
		 CDevice::GetInst()->GetDeviceContext()->DSSetShaderResources(Register, 1, &SRV);

	 if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Hull)
		 CDevice::GetInst()->GetDeviceContext()->HSSetShaderResources(Register, 1, &SRV);

	 if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Geometry)
		 CDevice::GetInst()->GetDeviceContext()->GSSetShaderResources(Register, 1, &SRV);

	 if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Compute)
		 CDevice::GetInst()->GetDeviceContext()->CSSetShaderResources(Register, 1, &SRV);
 }

 void CStructuredBuffer::CopyData(void* Data)
{
	 D3D11_MAPPED_SUBRESOURCE Map = {};

	 CDevice::GetInst()->GetDeviceContext()->Map(m_Buffer, 0, D3D11_MAP_READ, 0, &Map);

	 memcpy(Data, Map.pData, m_Size * m_Count);

	 CDevice::GetInst()->GetDeviceContext()->Unmap(m_Buffer, 0);
 }

 void CStructuredBuffer::CopyResource(CStructuredBuffer* Buffer)
{
	 CDevice::GetInst()->GetDeviceContext()->CopyResource(Buffer->m_Buffer, m_Buffer);
 }
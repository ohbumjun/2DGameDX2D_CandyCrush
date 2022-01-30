#pragma once

#include "../../GameInfo.h"
#include "Material.h"

class CMaterialManager
{
	friend class CResourceManager;

private :
	std::unordered_map<std::string, CSharedPtr<CMaterial>> m_mapMaterial;
	// 모든 재질이 동일한 상수 버퍼
	class CMaterialConstantBuffer* m_CBuffer;

public :
	bool Init();
	CMaterial* FindMaterial(const std::string& Name);
	void ReleaseMaterial(const std::string& Name);
public :
	template<typename T>
	bool CreateMaterial(const std::string& Name)
{
		T* Material = (T*)FindMaterial(Name);

		if (Material)
			return true;

		Material = new T;
		Material->SetName(Name);

		if (!Material->Init())
		{
			SAFE_DELETE(Material);
			return false;
		}

		Material->SetConstantBuffer(m_CBuffer);

		m_mapMaterial.insert(std::make_pair(Name, Material));

		return true;
}

public :
	DECLARE_SINGLE(CMaterialManager);
};



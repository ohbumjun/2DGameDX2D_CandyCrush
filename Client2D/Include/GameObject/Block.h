#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"

class CBlock :  public CGameObject
{
	friend class CScene;
	friend class CBoard;
	friend class CGameObjectPool;
protected :
	CBlock();
	CBlock(const CBlock& Block);
	~CBlock();
private :
	CSharedPtr<CSpriteComponent> m_SpriteComponent;
	CSharedPtr<CStaticMeshComponent> m_StaticComponent;
	class CBoard* m_Board;
private :
	bool m_IsStopBlock;
	int m_Index;
	int m_RowIndex;
	int m_ColIndex;

public:
	static CGameObject* CreateObject() { return new CBlock; }
public :
	bool IsStopBlock() const
	{
		return m_IsStopBlock;
	}
	CSpriteComponent* GetSpriteComponent () const
	{
		return m_SpriteComponent;
	}
public :
	void SetStaticOpacity (float Opacity)
	{
		m_StaticComponent->SetOpacity(Opacity);
	}
	void SetStaticBaseColor(float r, float g, float b, float a)
	{
		m_StaticComponent->SetBaseColor(r, g, b, a);
	}
	void SetStopEnable(bool Enable)
	{
		m_IsStopBlock = Enable;
	}
	void SetIndexInfo(int Index, int RowIndex, int ColIndex);
public :
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
};


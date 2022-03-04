#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"

class CBlock :  public CGameObject
{
	friend class CScene;
	friend class CBoard;
protected :
	CBlock();
	CBlock(const CBlock& Block);
	~CBlock();
private :
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CStaticMeshComponent> m_StaticComponent;
	class CBoard* m_Board;
private :
	bool m_ClickedBlock;
	int m_Index;
	int m_RowIndex;
	int m_ColIndex;
public :
	bool IsClicked() const
{
		return m_ClickedBlock;
}
public :
	void SetBaseColor(float r, float g, float b, float a)
{
		m_StaticComponent->SetBaseColor(r, g, b, a);
}
	void SetClickEnable(bool Enable)
{
		m_ClickedBlock = Enable;
}
	void SetIndexInfo(int Index, int RowIndex, int ColIndex);
public :
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
};


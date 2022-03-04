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
	int m_Index;
	int m_RowIndex;
	int m_ColIndex;
public :
	void SetIndexInfo(int Index, int RowIndex, int ColIndex);
public :
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
};


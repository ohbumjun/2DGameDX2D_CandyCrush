#pragma once
#include "../GameInfo.h"

class CViewPort
{
	friend class CScene;
protected:
	CViewPort();
	~CViewPort();
private:
	class CScene* m_Scene;
public :
	class CScene* GetScene() const
{
		return m_Scene;
}
public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
};


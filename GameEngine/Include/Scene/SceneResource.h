#pragma once
class CSceneResource
{
	friend class CScene;
protected:
	CSceneResource();
	~CSceneResource();
public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
};


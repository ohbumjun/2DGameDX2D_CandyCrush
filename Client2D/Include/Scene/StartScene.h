#pragma once
#include "Scene\SceneMode.h"

class CStartScene :
    public CSceneMode
{
public :
    CStartScene();
    ~CStartScene();
private :
    std::function<void(bool, float)> m_LoadingFunction;
    class CUIWindow* m_StartWidget;
    float m_PlayTimeLeft;
    float m_PlayTimeFull;
public :
    virtual bool Init() override;
    void CreateAnimationSequence();
private :
    void UpdatePlayTime(float DeltaTime);
private :
    virtual void Update(float DeltaTime) override;
public :
    template<typename T>
    void SetLoadingFunction(T* Obj, void (T::*Func)(bool, float))
{
        m_LoadingFunction = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
}
};



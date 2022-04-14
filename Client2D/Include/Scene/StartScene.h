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
public :
    virtual bool Init() override;
    void CreateAnimationSequence();
public :
    template<typename T>
    void SetLoadingFunction(T* Obj, void (T::*Func)(bool, float))
{
        m_LoadingFunction = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
}
};



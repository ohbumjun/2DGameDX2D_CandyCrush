#pragma once
#include "Component\ObjectComponent.h"

class CBoardAILogicComponent :
    public CObjectComponent
{
    friend class CBoard;
    friend class CGameObject;
    friend class CBoardBagLogicComponent;
protected:
    CBoardAILogicComponent();
    virtual ~CBoardAILogicComponent();
private :
    class CBoard* m_Board;
    class CBoardBagLogicComponent* m_BagLogicComponent;
private :
    float m_AICheckDelayTime;
    float m_AICheckDelayTimeMax;
    bool m_IsAIChecked;
public:
    virtual bool Init() override; 
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CObjectComponent* Clone();
public :
    bool IsAIChecked() const
    {
        return m_IsAIChecked;
    }
    float GetAICheckDelayTime() const
    {
        return m_AICheckDelayTime;
    }
    float GetAICheckDelayTimeMax() const
    {
        return m_AICheckDelayTimeMax;
    }
public :
    void SetAIChecked(bool Enable)
    {
        m_IsAIChecked = Enable;
    }
    void ResetAICheckDelayTime()
    {
        m_AICheckDelayTime = m_AICheckDelayTimeMax;
    }
    void DecreaseAICheckTime(float DeltaTime)
    {
        m_AICheckDelayTime -= DeltaTime;
    }
private :
    bool CheckMatchExist();
    bool CheckAIAndPossibleMatch(float DeltaTime);
    int CalculateAICombScore(class CCell* FirstCell, class CCell* SecondCell);
    int CalculateAISpecialCellScore(class CCell* FirstCell);
    // Match 여부와 Score 를 리턴한다.
    std::pair<int, bool>CheckAIRowMatch(int OriginRowIdx, int OriginColIdx,
        int NewRowIdx, int NewColIdx, std::vector<int>& MatchedIdxs);
    std::pair<int, bool> CheckAIColMatch(int OriginRowIdx, int OriginColIdx,
        int NewRowIdx, int NewColIdx, std::vector<int>& MatchedIdxs);
    std::pair<int, bool> CheckAIBagMatch(int OriginRowIdx, int OriginColIdx,
        int NewRowIdx, int NewColIdx, std::vector<int>& MatchedIdxs);
};


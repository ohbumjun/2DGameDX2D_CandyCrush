#pragma once
#include "Component\ObjectComponent.h"

class CBoardBagLogicComponent :
    public CObjectComponent
{
    friend class CBoard;
    friend class CGameObject;
protected:
    CBoardBagLogicComponent();
    virtual ~CBoardBagLogicComponent();
private:
    class CBoard* m_Board;
    class CBoardAILogicComponent* m_AILogicComponent;
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
    bool CheckBagMatch(int RowIndex, int ColIndex, int Index, bool IsClicked);
    std::pair<int, bool> CheckBagRightDownMatch(int OriginRowIndex, int OriginColIndex,
        int NewRowIndex, int NewColIndex, int Index, std::vector<int>& MatchIdxList, bool IsAI);
    std::pair<int, bool> CheckBagRightUpMatch(int OriginRowIndex, int OriginColIndex,
        int NewRowIndex, int NewColIndex, int Index, std::vector<int>& MatchIdxList, bool IsAI);
    std::pair<int, bool> CheckBagLeftDownMatch(int OriginRowIndex, int OriginColIndex,
        int NewRowIndex, int NewColIndex, int Index, std::vector<int>& MatchIdxList, bool IsAI);
    std::pair<int, bool> CheckBagLeftUpMatch(int OriginRowIndex, int OriginColIndex,
        int NewRowIndex, int NewColIndex, int Index, std::vector<int>& MatchIdxList, bool IsAI);
    std::pair<int, bool> CheckBagCenterRightMatch(int OriginRowIndex, int OriginColIndex,
        int NewRowIndex, int NewColIndex, int Index, std::vector<int>& MatchIdxList, bool IsAI);
    std::pair<int, bool> CheckBagCenterLeftMatch(int OriginRowIndex, int OriginColIndex,
        int NewRowIndex, int NewColIndex, int Index, std::vector<int>& MatchIdxList, bool IsAI);
    std::pair<int, bool> CheckBagCenterDownMatch(int OriginRowIndex, int OriginColIndex,
        int NewRowIndex, int NewColIndex, int Index, std::vector<int>& MatchIdxList, bool IsAI);
    std::pair<int, bool> CheckBagCenterUpMatch(int OriginRowIndex, int OriginColIndex,
        int NewRowIndex, int NewColIndex, int Index, std::vector<int>& MatchIdxList, bool IsAI);
};


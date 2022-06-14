#pragma once
#include "Component\ObjectComponent.h"

class CBoardCombLogicComponent :
    public CObjectComponent
{
    friend class CBoard;
    friend class CGameObject;
protected:
    CBoardCombLogicComponent();
    virtual ~CBoardCombLogicComponent();
private:
    class CBoard* m_Board;
    class CBoardDestroyLogicComponent* m_DestroyComponent;
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
    // 봉지 + 봉지 --> 주변 2개 터진다 + 2번 연속으로 
    // 봉지 + 줄무늬 --> 좌우 상하로 3줄씩 없애준다.
    // 봉지 + Mirror Ball --> 해당 봉지 Type 의 Cell 들 모두 제거 + 이후 2번째에는 MirrorBall에 의해 Random Cell 모두 제거
    // 줄무늬 + 줄무늬 --> 가로 + 세로로 1줄씩  제거해준다.
    // 줄무늬 + Mirror Ball --> 해당 색상의 모든 Cell 들을 Horizontal 혹은 Vertical 로 바꾼다음, 그에 맞게 다 터뜨린다.
    // MirrorBall  + MirrorBall --> 화면 상의 모든 Cell 들을 제거 한다.
    bool CheckCombination(class CCell* FirstCell, class CCell* SecondCell);
    bool CheckBagAndBagComb(class CCell* FirstCell, class CCell* SecondCell);
    void DestroyBagAndBagComb(class CCell* FirstCell);
    void DestroyBagLineComb(int RowIndex, int ColIndex);
    bool CheckBagAndRowLineComb(class CCell* FirstCell, class CCell* SecondCell);//
    bool CheckBagAndColLineComb(class CCell* FirstCell, class CCell* SecondCell);
    bool CheckBagAndMirrorBallComb(class CCell* FirstCell, class CCell* SecondCell);
    void ManageBagAndMirrorBallComb(int Index);
    void DestroyMirrorBallOfBagMirrorBallComb(int Index);
    bool CheckLineAndLineComb(class CCell* FirstCell, class CCell* SecondCell);
    bool CheckLineAndMirrorBallComb(class CCell* FirstCell, class CCell* SecondCell);
    bool DestroyLineAndMirrorBallComb(int RowIndex, int ColIndex, int Index);
    bool CheckMirrorBallAndMirrorBallComb(class CCell* FirstCell, class CCell* SecondCell);
    bool DestroyMirrorBallAndMirrorBallComb(class CCell* FirstCell, class CCell* SecondCell);
    void TriggerLineAndMirrorBallCombEffect(int OriginRowIdx, int OriginColIdx, int OriginIdx);
    void TriggerDoubleMirrorBallCombEffect(int OriginRowIdx, int OriginColIdx, int OriginIdx);
};


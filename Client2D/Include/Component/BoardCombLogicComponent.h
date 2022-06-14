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
    // ���� + ���� --> �ֺ� 2�� ������ + 2�� �������� 
    // ���� + �ٹ��� --> �¿� ���Ϸ� 3�پ� �����ش�.
    // ���� + Mirror Ball --> �ش� ���� Type �� Cell �� ��� ���� + ���� 2��°���� MirrorBall�� ���� Random Cell ��� ����
    // �ٹ��� + �ٹ��� --> ���� + ���η� 1�پ�  �������ش�.
    // �ٹ��� + Mirror Ball --> �ش� ������ ��� Cell ���� Horizontal Ȥ�� Vertical �� �ٲ۴���, �׿� �°� �� �Ͷ߸���.
    // MirrorBall  + MirrorBall --> ȭ�� ���� ��� Cell ���� ���� �Ѵ�.
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


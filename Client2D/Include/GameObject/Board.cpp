#include "Board.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"
#include "../Component/BoardAILogicComponent.h"
#include "../Component/BoardBagLogicComponent.h"
#include "../Component/BoardDestroyLogicComponent.h"
#include "../Component/BoardCombLogicComponent.h"
#include "../Component/BoardMatchLogicComponent.h"
#include "../Client.h"

CBoard::CBoard() :
	m_CellsMoving(false),
	m_MouseClick(Mouse_Click::None),
	m_FirstClickCell(nullptr),
	m_SecClickCell(nullptr),
	m_ClickCellsMoveDone(0),
	m_IsCheckUpdateNeeded(false),
	m_IsBeingSpecialDestroyed(false)
	// m_AICheckDelayTime(3.f),
	// m_AICheckDelayTimeMax(3.f)
{
}

CBoard::~CBoard()
{
}

void CBoard::AddClickCellMoveDone()
{
	m_ClickCellsMoveDone += 1;

	if (m_ClickCellsMoveDone >= 2)
	{
		// First, Second Cell 들 Index 다시 세팅
		SwitchClickCellsInfo();

		// Click Cell 정보들 초기화
		ResetClickCellInfos();

		// Match 함수 실행
		FindMatchCellsAfterTwoClick();

		// 다시 0으로 세팅
		m_ClickCellsMoveDone = 0;
	}
}

void CBoard::SwitchClickCellsInfo()
{
	int FirstRowIndex = m_FirstClickCell->GetRowIndex();
	int FirstColIndex = m_FirstClickCell->GetColIndex();
	int FirstIndex = FirstRowIndex * m_ColCount + FirstColIndex;
	// float FirstNewPosY = m_FirstClickCell->GetNewDownPosY();

	int SecRowIndex = m_SecClickCell->GetRowIndex();
	int SecColIndex = m_SecClickCell->GetColIndex();
	int SecIndex = SecRowIndex * m_ColCount + SecColIndex;
	// float SecNewPosY = m_SecClickCell->GetNewDownPosY();

	m_FirstClickCell->SetIndexInfo(SecIndex, SecRowIndex, SecColIndex);
	// m_FirstClickCell->SetNewDownPosY(SecNewPosY);

	m_SecClickCell->SetIndexInfo(FirstIndex, FirstRowIndex, FirstColIndex);
	// m_SecClickCell->SetNewDownPosY(FirstNewPosY);

	// Swap 알고리즘 적용
	// 반드시, CSharedPtr 형태로 해줘야 한다.
	// 그렇지 않으면 m_vecCells[FirstIndex] = m_SecClickCell; 에서, m_vecCells[FirstIndex] 의
	// RefCount가 0이 되어서, 사라져 버린다.
	CSharedPtr<CCell> Temp = m_vecCells[FirstIndex];
	m_vecCells[FirstIndex] = m_SecClickCell;
	m_vecCells[SecIndex] = Temp;

}

void CBoard::FindMatchCellsAfterTwoClick()
{
	bool Match = true;

	if (!m_FirstClickCell || !m_SecClickCell)
		return;

	// Match 되는 녀석들이 있는지 확인한다.
	Match = CheckMatchAfterTwoClick(m_FirstClickCell, m_SecClickCell);

	// todo : Scene에서 제거하기
	// 해당 위치의 Cell 사라지게 하기
	// 이렇게 하면 Scene 내에서는 지워져서 Render는 안되지만
	// 여전히 m_vecCells 안에는 남아있게 된다.
	DestroyCells();

	// 새로운 Cell 생성
	CreateNewCellsAboveShownArea();

	// todo : Match가 있었다면 Click Cell 정보를 초기화 해준다.
	if (Match)
		SetFindMatchCellsDone();

	// todo : Match가 없다면, 기존에 이동시킨 Cell 들을 다시 원래 대로 세팅한다.
	else
	{
		if (m_FirstClickCell && m_SecClickCell)
		{
			// 2개의 Cell 에 새로운 위치 세팅
			m_FirstClickCell->SetClickDestPos(m_SecClickCell->GetWorldPos());
			m_SecClickCell->SetClickDestPos(m_FirstClickCell->GetWorldPos());

			// Switch 중이라고 표시하기
			m_FirstClickCell->SetIsSwitch(true);
			m_SecClickCell->SetIsSwitch(true);

			// 다시 돌아가는 녀석들이라고 세팅
			m_FirstClickCell->SetIsGoingBack(true);
			m_SecClickCell->SetIsGoingBack(true);

			// Cell 이동중 표시하기
			m_CellsMoving = true;
		}
	}
}

void CBoard::SetFindMatchCellsDone()
{
	// 다시 마우스 클릭 상태를 되돌려서, 클릭이 가능하게 세팅한다.
	m_MouseClick = Mouse_Click::None;

	// 이동 여부 False로 다시 세팅
	m_CellsMoving = false;

	ResetClickCellInfos();

	// 클릭한 Cell 들을 nullptr 처리한다
	m_FirstClickCell = nullptr;
	m_SecClickCell = nullptr;
}

void CBoard::CreateNewCellsAboveShownArea()
{
	// todo : 새로운 Cell 생성하기 --> MatchState 적용하기 : ex) m_vecMatchState

	// 최상단 높이
	Vector3 BoardStartPos = GetWorldPos();
	float TopYPos = BoardStartPos.y + m_CellSize.y * (m_RowCount - 1);

	// 위에서 부터 아래로 세팅할 것이다.
	for (int col = 0; col < m_ColCount; col++)
	{
		if (m_vecColNewCellNums[col] == 0)
			continue;

		for (int offset = 0; offset < m_vecColNewCellNums[col]; offset++)
		{
			int RowIndex = m_RowCount - 1 - offset;

			Vector3 WorldPos = Vector3(BoardStartPos.x + m_CellSize.x * col, TopYPos - m_CellSize.y * offset, 1.f);

			Cell_Type_Binary CellBType = ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)Cell_Type::End));
			// int Type = rand() % (int)Cell_Type::End;

			float NewYPos = TopYPos - m_CellSize.y * offset;

			Cell_State CellState = ChangeMatchStateToCellState(Match_State::NoMatch);

			char Name[MAX_PATH] = {};

			sprintf_s(Name, "Cell%d", RowIndex * m_ColCount + col);

			// CreateSingleNewCell(Name, RowIndex, col, WorldPos,NewYPos, (Cell_Type)Type, 0.1f, false, CellState);
			CreateSingleNewCell(Name, RowIndex, col, WorldPos, NewYPos, CellBType, 0.1f, false, CellState);
		}
	}

	// todo : 새롭게 Cell 생성 이후, Random Shuffle
	// ShuffleRandom();


	// Column 별 새로 생성할 Cell 개수 다시 초기화
	for (int i = 0; i < m_ColCount; i++)
	{
		m_vecColNewCellNums[i] = 0;
	}

	// Match State 초기화 해주기
	ResetMatchStateInfo();
}

CCell* CBoard::CreateSingleNewCell(const std::string& Name, int RowIndex, int ColIndex,
	const Vector3& WorldPos, float NewYPos, Cell_Type_Binary Type,
	float Opacity, bool ShowEnable, Cell_State State)
{
	int Index = RowIndex * m_ColCount + ColIndex;

	// CCell* Cell = m_Scene->CreateGameObject<CCell>(Name);

	CGameObject* ObjectFromPool = m_CellObjectPool->GetFromPool();

	CCell* Cell = dynamic_cast<CCell*>(ObjectFromPool);

	// Cell 의 Ref Cnt 를 1 감소시킨다.

	Cell->SetScene(m_Scene);
	Cell->Init();
	Cell->Start();
	
	// 여기에 추가하면 Ref Cnt 가 1 증가한다.
	m_Scene->AddObjectToList(Cell);

	// 그런데 이제 Scene의 Obj List 에 추가된 순간, Scene 내의 Obj List 에서
	// 제거될 때, 같이 해당 Cell 도 사라져야 한다.
	// 더 이상, Object Pool 에서 해당 Cell 정보를 관리하지 않기 때문에
	// 따라서, Scene 의 Obj List 에서 해당 Cell 이 제거될 때, 알아서 Memory 해제가 되게 하기 위해서
	// 여기서 미리 Ref Cnt 를 1 감소 시켜준다.
	Cell->Release();

	// Active = true 상태로 세팅한다.
	// Cell->Activate();

	Vector3 BoardStartPos = GetWorldPos();

	// Owner 세팅 
	Cell->m_Board = this;

	// Scene 세팅 
	Cell->SetScene(m_Scene);

	// x는 열, y는 행
	Cell->SetWorldPos(WorldPos);

	// 크기 세팅 
	Cell->SetWorldScale(Vector3(m_CellSize.x, m_CellSize.y, 1.f));

	// Index 세팅 --> NewPosY도 세팅
	Cell->SetIndexInfo(Index, RowIndex, ColIndex);

	// Pivot 세팅
	Cell->SetPivot(0.5f, 0.5f, 0.f);

	// Type 세팅
	Cell->SetCellType(Type);

	// 투명도 세팅 ( 안보이게 하기 )
	// Cell->SetOpacity(Opacity);
	Cell->SetOpacity(1.f);

	// 안보인다는 멤버 변수 설정
	Cell->SetShowEnable(ShowEnable);

	if (ShowEnable || Opacity > 0.1f)
	{
		Cell->SetShowEnable(ShowEnable);
	}

	// 보여지는 영역 경계선 설정하기
	Cell->SetShownAreaTopYPos(BoardStartPos.y + m_CellSize.y * m_VisualRowCount);

	// WorldY Pos 세팅하기
	Cell->SetWorldYPos(WorldPos.y);

	// NewY Pos 세팅하기
	Cell->SetNewDownPosY(NewYPos);

	// Cell State 세팅 --> Current Animation 세팅
	// Cell->SetCurrentAnimation("Normal");
	Cell->SetCellState(State);

	// MirrorBall Destroy Type Random 세팅
	Cell->SetMirrorBallDestroyType(ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)(Cell_Type::End))));

	// vector 목록에 추가 
	m_vecCells[Index] = Cell;

	return Cell;
}

void CBoard::ManageDestroyedBagInfo(int Index)
{
	// Bag 는 한번 더 터뜨려야 한다.
	if (m_vecCells[Index]->IsSpecialDestroyedBag())
	{
		// 여기서 한번 더 분기점을 줘야 할 것 같다.
		// BagAndBag Combination 폭발 이후, 같은 Combination 효과 대로 터뜨려야 할 지
		if (m_vecCells[Index]->IsBagAndBagFirstDestroyed())
		{
			m_vecCells[Index]->SetDestroyState(Destroy_State::BagAndBag);
			// m_vecCells[Index]->SetIsBagAndBagDestroyed(false);
		}
		// 아니면 그냥 일반 Bag Destroy 대로 터뜨려야 할지 
		else
		{
			m_vecCells[Index]->SetDestroyState(Destroy_State::BagAfter);
		}

		m_vecCells[Index]->SetSpecialDestroyedBag(false);
	}
}

void CBoard::DestroyCells()
{
	int CheckMaxIndex = m_VisualRowCount * m_ColCount;

	// 새로운 Type 세팅 및 Destroy
	int DestroyTargetEndIdx = (int)(m_TotCount * 0.5f);

	for (int Index = 0; Index < DestroyTargetEndIdx; Index++)
	{
		ManageDestroyedBagInfo(Index);

		m_CombLogicComponent->ManageBagAndMirrorBallComb(Index);
	}

	for (int Index = 0; Index < DestroyTargetEndIdx; Index++)
	{
		// BagAfter 의 경우, 그냥 Match 여부와 관계없이 터뜨린다.
		if (m_vecCells[Index]->GetDestroyState() == Destroy_State::BagAfter)
		{
			int RowIndex = m_vecCells[Index]->GetRowIndex();
			int ColIndex = m_vecCells[Index]->GetColIndex();

			m_DestroyLogicComponent->DestroyBagEffect(RowIndex, ColIndex, true);

			// 이 녀석에 대해서는 더이상 Match 여부를 고려하지 않아도 되지 않을까 ?
			// continue;
		}

		// Mirror Ball + Bag 조합 중에서
		// Mirror Ball 에 해당하는 녀석을 터뜨린다.
		m_CombLogicComponent->DestroyMirrorBallOfBagMirrorBallComb(Index);

		// 만약 Match 된 녀석이라면 
		if (m_vecCellIsMatch[Index])
		{
			int RowIndex = m_vecCells[Index]->GetRowIndex();
			int ColIndex = m_vecCells[Index]->GetColIndex();

			// 만약 Destroy_State 라면 --> 즉, 현재 터뜨려야 할 녀석이 있다면
			// if ((int)m_vecDestroyState[Index] > (int)Destroy_State::None)
			if ((int)m_vecCells[Index]->GetDestroyState() > (int)Destroy_State::None)
			{
				m_DestroyLogicComponent->JudgeCellDestroyType(RowIndex, ColIndex, Index);
			}

			// 해당 위치에서의 MatchState 를 확인해서
			// Normal 보다 큰 녀석이면, 그냥 State 만 바꿔주고
			else if ((int)m_vecMatchState[Index] > (int)Match_State::Normal)
			{
				m_vecCells[Index]->SetCellState(ChangeMatchStateToCellState(m_vecMatchState[Index]));

				// m_vecDestroyMarkState[Index] = ChangeMatchStateToDestroyMarkState(m_vecMatchState[Index]);
				m_vecCells[Index]->SetDestroyMarkState(ChangeMatchStateToDestroyMarkState(m_vecMatchState[Index]));
			}
			// 그게 아니라면, 제거 이후, 해당 Col 에서의 제거개수 + 1 을 해주면 된다.
			else
			{
				// 화면에서 제거해주고
				m_DestroyLogicComponent->DestroySingleCell(RowIndex, ColIndex);
			}
		}
		// Match가 아니라면,  --> 그래도 조합은 구성될 수 있다
		else
		{
			int RowIndex = m_vecCells[Index]->GetRowIndex();
			int ColIndex = m_vecCells[Index]->GetColIndex();

			if ((int)m_vecCells[Index]->GetDestroyState() > (int)Destroy_State::None)
			{
				m_DestroyLogicComponent->JudgeCellDestroyType(RowIndex, ColIndex, Index);
			}
		}
	}

	m_IsBeingSpecialDestroyed = CheckIsCellBeingSpecialDestroyed();

	if (!m_IsBeingSpecialDestroyed)
	{
		ResetCellsPauseGoDownInfo();
	}

	// todo : 이 위에 녀석들 New Pos 세팅하기
	// 어차피 사라진 녀석들은 화면에 보여지는 영역
	// 안에서만 사라진 상태로 놓일 것이다. --> m_VisualRowCount
	SetNewPosOfCells();

	// todo : 움직인 Cell 새로운 Index 세팅
	// 여기서 실제로 새로운 Index를 세팅할 것이다.
	SetNewIndexOfCells();


	// m_vecDestroyState 정보 초기화
	int HalfTotalIndex = (int)(m_TotCount * 0.5f);

	for (int i = 0; i < HalfTotalIndex; i++)
	{
		// m_vecDestroyState[i] = Destroy_State::None;
		if (m_vecCells[i]->GetDestroyState() == Destroy_State::BagAfter)
			continue;
		m_vecCells[i]->SetDestroyState(Destroy_State::None);
	}

	//m_vecCellDownNums 정보 초기화
	ResetVecColNewCellNumsInfo();

	// Match 정보도 초기화 해준다
	ResetBoolIsMatchInfo();


	// 각 열에서 사라진 개수도 0으로 초기화 해주기
	// 이것은 CreateNewCells 함수에서도 실행하는 녀석이므로
	// 거기서 쓰고 나서, 거기서 초기화 를 해줘야 한다.
	/*
	for (int i = 0; i < m_ColCount; i++)
	{
		m_vecColNewCellNums[i] = 0;
	}
	*/
}

void CBoard::SetNewPosOfCells()
{
	int DestroyedIndex = -1;

	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			DestroyedIndex = row * m_ColCount + col;

			// 사라진 녀석이 아니라면 Skip
			if (m_vecCells[DestroyedIndex]->IsActive())
				continue;

			// 제거된 녀석 그 위로 새롭게 NewPosY를 세팅해서 내려오게 해준다.
			int ChangeTargetIndex = -1;

			// 내려갈 개수 
			int DownCellNum = m_vecColNewCellNums[col];

			for (int ChangeRow = row + 1; ChangeRow < m_RowCount; ChangeRow++)
			{
				// 내려올 Cell
				ChangeTargetIndex = ChangeRow * m_ColCount + col;

				// 사라진 Cell 이라면 내려오기 조정 X
				if (!m_vecCells[ChangeTargetIndex]->IsActive())
					continue;

				// 현재 위치
				float CurNewPosY = m_vecCells[ChangeTargetIndex]->GetNewDownPosY();

				// 내려갈 위치 조정하기 
				// m_vecCells[ChangeTargetIndex]->SetNewPosY(CurNewPosY - m_CellSize.y * DownCellNum);
				m_vecCells[ChangeTargetIndex]->SetNewPosY(CurNewPosY - m_CellSize.y);

				// 내려갈 Idx 증가시켜주기
				m_vecCellDownNums[ChangeTargetIndex] += 1;
			}
		}
	}
}

void CBoard::SetNewIndexOfCells()
{
	int TargetIndex = -1;
	int NewChangedIndex = -1;
	for (int row = 0; row < m_RowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			// 내려올 Cell
			TargetIndex = row * m_ColCount + col;

			// 만약 해당 Cell 이 내려갈 개수가 없다면 Skip
			if (m_vecCellDownNums[TargetIndex] == 0)
				continue;

			int NRow = (row - m_vecCellDownNums[TargetIndex]);

			NewChangedIndex = NRow * m_ColCount + col;

			m_vecCells[TargetIndex]->SetIndexInfo(NewChangedIndex, NRow, col);
			m_vecCells[NewChangedIndex] = m_vecCells[TargetIndex];

		}
	}
}


bool CBoard::FindMatchUpdate()
{
	bool Match = true; //

	// Cell 들이 움직이지 않고 있을 때만 검사한다
	if (m_CellsMoving)
		return false;

	// 현재 이동시킨 Cell 을 처리중이라면
	if (m_FirstClickCell || m_SecClickCell)
		return false;

	// Match 되는 녀석들이 있는지 확인한다.
	Match = CheckMatchUpdate();

	m_IsMatchExist = m_AILogicComponent->CheckMatchExist();

	// todo : Scene에서 제거하기
	// 해당 위치의 Cell 사라지게 하기
	// 이렇게 하면 Scene 내에서는 지워져서 Render는 안되지만
	// 여전히 m_vecCells 안에는 남아있게 된다.
	DestroyCells();

	// 새로운 Cell 생성
	CreateNewCellsAboveShownArea();

	return Match;
}

bool CBoard::CheckMatchUpdate()
{
	// Cell 이 이동중이라면 X
	if (m_CellsMoving)
		return false;

	// 현재 클릭 상태라면, X
	if (m_FirstClickCell || m_SecClickCell)
		return false;

	bool IsMatchExist = false;

	// 새로운 위치로 이동 완료한 Cell 들에 대해서만 조사할 것이다
	int RowIndex = -1, ColIndex = -1;

	Match_State CellResult;
	Match_State CellRowResult = Match_State::NoMatch;
	Match_State CellColResult = Match_State::NoMatch;
	Match_State CellBagResult = Match_State::NoMatch;

	// 딱 반만 Update 한다.
	int CheckMaxIndex = m_VisualRowCount * m_ColCount;

	for (int i = 0; i < CheckMaxIndex; i++)
	{
		RowIndex = m_vecCells[i]->GetRowIndex();
		ColIndex = m_vecCells[i]->GetColIndex();

		// 1번째 Click Cell에 대한 검사 먼저 하기 
		CellRowResult = m_MatchLogicComponent->CheckRowMatch(RowIndex, ColIndex, i, false);
		CellColResult = m_MatchLogicComponent->CheckColMatch(RowIndex, ColIndex, i, false);

		// 최대 녀석으로 세팅한다.
		CellResult = (int)CellColResult > (int)CellRowResult ? CellColResult : CellRowResult;

		// Bag 조합 검사하기
		CellBagResult = m_BagLogicComponent->CheckBagMatch(RowIndex, ColIndex, i, false) ? Match_State::Bag : Match_State::NoMatch;

		// 최종 결과
		CellResult = (int)CellResult > (int)CellBagResult ? CellResult : CellBagResult;

		m_vecMatchState[i] = CellResult;

		if (CellResult != Match_State::NoMatch)
		{
			IsMatchExist = true;
		}
	}

	// 모든 Cell 들을 다시 m_IsPlacedNew 를 false 처리한다.
	ResetPlacedNewInfo();

	return IsMatchExist;
}

bool CBoard::CheckCellsMoving()
{
	for (int i = 0; i < m_TotCount; i++)
	{
		if (m_vecCells[i]->IsMoving())
			return true;
	}

	return false;
}

void CBoard::ResetMatchStateInfo()
{
	// Match State 초기화 해주기
	for (int i = 0; i < m_TotCount; i++)
	{
		m_vecMatchState[i] = Match_State::NoMatch;
	}
}

void CBoard::ResetBoolIsMatchInfo()
{
	int HalfTotalIndex = (int)(m_TotCount * 0.5f);

	for (int i = 0; i < HalfTotalIndex; i++)
	{
		m_vecCellIsMatch[i] = false;
	}
}

void CBoard::ResetVecColNewCellNumsInfo()
{
	for (int i = 0; i < m_TotCount; i++)
	{
		m_vecCellDownNums[i] = 0;
	}
}

void CBoard::ResetCellDownNumsInfo()
{}

void CBoard::ResetPlacedNewInfo()
{
	for (int i = 0; i < m_TotCount; i++)
	{
		m_vecCells[i]->SetPlacedNew(false);
	}
}

void CBoard::ResetCellStateInfo()
{
	for (int i = 0; i < m_TotCount; i++)
	{
		m_vecCells[i]->SetCellState(Cell_State::Normal);
	}
}

void CBoard::SetMatchStateTrue(int Index)
{
	m_vecCellIsMatch[Index] = true;
}

Cell_State CBoard::ChangeMatchStateToCellState(Match_State State)
{
	switch (State)
	{
	case Match_State::NoMatch:
		return Cell_State::Normal;

	case Match_State::Normal:
		return Cell_State::Normal;

	case Match_State::RowLine:
		return Cell_State::RowLine;

	case Match_State::ColLine:
		return Cell_State::ColLine;

	case Match_State::Bag:
		return Cell_State::Bag;

	case Match_State::MirrorBall:
		return Cell_State::MirrorBall;
	}

	return Cell_State::Normal;
}

Destroy_State CBoard::ChangeMatchStateToDestroyState(Match_State State)
{
	switch (State)
	{
	case Match_State::NoMatch:
		return Destroy_State::None;

	case Match_State::Normal:
		return Destroy_State::None;

	case Match_State::RowLine: // 가로 줄 
		return Destroy_State::Horizontal;

	case Match_State::ColLine: // 세로 줄
		return Destroy_State::Vertical;

	case Match_State::Bag:
		return Destroy_State::Bag;

	case Match_State::MirrorBall:
		return Destroy_State::MirrorBall;
	}

	return Destroy_State::None;
}

Destroy_State CBoard::ChangeDestroyMarkStateToDestroyState(DestroyMark_State State)
{
	switch (State)
	{
	case DestroyMark_State::Horizontal: // 가로 줄 
		return Destroy_State::Horizontal;

	case DestroyMark_State::Vertical: // 세로 줄
		return Destroy_State::Vertical;

	case DestroyMark_State::Bag:
		return Destroy_State::Bag;

	case DestroyMark_State::MirrorBall:
		return Destroy_State::MirrorBall;
	}

	return Destroy_State::None;
}

DestroyMark_State CBoard::ChangeMatchStateToDestroyMarkState(Match_State State)
{
	switch (State)
	{
	case Match_State::RowLine: // 가로 줄 
		return DestroyMark_State::Horizontal;

	case Match_State::ColLine: // 세로 줄
		return DestroyMark_State::Vertical;

	case Match_State::Bag:
		return DestroyMark_State::Bag;

	case Match_State::MirrorBall:
		return DestroyMark_State::MirrorBall;
	}

	return DestroyMark_State::None;
}

Cell_Type_Binary CBoard::ChangeCellTypeToCellBinaryType(Cell_Type Type)
{
	switch (Type)
	{
	case Cell_Type::Blue: // 가로 줄 
		return Cell_Type_Binary::Blue;

	case Cell_Type::Red: // 세로 줄
		return Cell_Type_Binary::Red;

	case Cell_Type::Orange:
		return Cell_Type_Binary::Orange;

	case Cell_Type::Yellow:
		return Cell_Type_Binary::Yellow;

	case Cell_Type::Green:
		return Cell_Type_Binary::Green;
		/*
		*/
	}

	return Cell_Type_Binary::All;
}

void CBoard::AddClickCellMoveBackDone() // 정말로 클릭한 Cell 들의 이동이 끝날 때 실행하는 함수
{
	m_ClickCellsMoveDone += 1;

	if (m_ClickCellsMoveDone >= 2)
	{
		m_ClickCellsMoveDone = 0;

		// 다시 마우스 클릭 상태를 되돌려서, 클릭이 가능하게 세팅한다.
		m_MouseClick = Mouse_Click::None;

		// 이동 여부 False로 다시 세팅
		m_CellsMoving = false;

		// 다시 First, Second Cell 의 Index를 처리한다.
		SwitchClickCellsInfo();

		ResetClickCellInfos();

		// 클릭한 Cell 들을 nullptr 처리한다
		m_FirstClickCell = nullptr;
		m_SecClickCell = nullptr;
	}

}

void CBoard::ResetClickCellInfos()
{
	// 클릭한 Cell 들의 Switch 상태를 false로 되돌려준다 --> 더이상 이동 X
	m_FirstClickCell->SetIsSwitch(false);
	m_SecClickCell->SetIsSwitch(false);

	// Going Back 상태도 false로 만들어준다.
	m_FirstClickCell->SetIsGoingBack(false);
	m_SecClickCell->SetIsGoingBack(false);
}

bool CBoard::CheckMatchAfterTwoClick(CCell* FirstClickCell, CCell* SecClickCell)
{
	if (!FirstClickCell || !SecClickCell)
		return false;

	Match_State FCellResult;
	Match_State FCellRowResult = Match_State::NoMatch;
	Match_State FCellColResult = Match_State::NoMatch;
	Match_State FCellBagResult = Match_State::NoMatch;

	Match_State SCellResult;
	Match_State SCellRowResult = Match_State::NoMatch;
	Match_State SCellColResult = Match_State::NoMatch;
	Match_State SCellBagResult = Match_State::NoMatch;

	// 1번째 Click Cell에 대한 검사 먼저 하기
	FCellRowResult = m_MatchLogicComponent->CheckRowMatch(FirstClickCell->GetRowIndex(),
		FirstClickCell->GetColIndex(), FirstClickCell->GetIndex(), true);
	FCellColResult = m_MatchLogicComponent->CheckColMatch(FirstClickCell->GetRowIndex(),
		FirstClickCell->GetColIndex(), FirstClickCell->GetIndex(), true);

	// todo :  Row, Column을 다 확인한 이후
	// Match_State에 따라서
	// FirstClickCell, SecondCell 이 이동한 위치에 새로운 Ball Type을 세팅할지 결정한다.
	// 최대 녀석으로 세팅한다.
	FCellResult = (int)FCellColResult > (int)FCellRowResult ? FCellColResult : FCellRowResult;

	// Bag 조합 검사하기
	FCellBagResult = m_BagLogicComponent->CheckBagMatch(FirstClickCell->GetRowIndex(),
		FirstClickCell->GetColIndex(), FirstClickCell->GetIndex(), false) ? Match_State::Bag : Match_State::NoMatch;

	// 최종 결과
	FCellResult = (int)FCellResult > (int)FCellBagResult ? FCellResult : FCellBagResult;

	// State 세팅 
	m_vecMatchState[FirstClickCell->GetIndex()] = FCellResult;

	// todo : 여기세 Ball Type을 세팅해서 새롭게 넣어준다.
	// todo : 그러면, 그 위에 녀석들은, 새롭게 내려갈 위치 및 Index 가 달라져야 한다는 것을 의미한다.

	// 2번째 Cell 기준으로 Match 확인하기
	// FirstCell 과 행 혹은 열 평행인지에 따라 다른 로직을 적용
	// 같은 Row를 또 다시 검사해줄 필요는 없기 때문이다
	// 다른 Row 혹은 다른 Col을 조사할 것이다. --> 아니다. 둘다 조사는 해야 한다.
	// if (FirstClickCell->GetColIndex() != SecClickCell->GetColIndex())
	SCellRowResult = m_MatchLogicComponent->CheckRowMatch(SecClickCell->GetRowIndex(),
		SecClickCell->GetColIndex(), SecClickCell->GetIndex(), true);

	// if (FirstClickCell->GetRowIndex() != SecClickCell->GetRowIndex())
	SCellColResult = m_MatchLogicComponent->CheckColMatch(SecClickCell->GetRowIndex(),
		SecClickCell->GetColIndex(), SecClickCell->GetIndex(), true);

	SCellResult = (int)SCellColResult > (int)SCellRowResult ? SCellColResult : SCellRowResult;

	// Bag 조합 검사하기
	SCellBagResult = m_BagLogicComponent->CheckBagMatch(SecClickCell->GetRowIndex(),
		SecClickCell->GetColIndex(), SecClickCell->GetIndex(), false) ? Match_State::Bag : Match_State::NoMatch;

	// 최종 결과
	SCellResult = (int)SCellResult > (int)SCellBagResult ? SCellResult : SCellBagResult;

	// State 세팅하기
	// 단 조건이 있다.
	bool IsSCellResultValid = true;
	if (SCellResult == FCellResult)
	{
		// 1) FCell, SCell 둘다 RowResult, 둘이 Type 이 동일하다면 --> 인접해서 맞춰진 것
		if (SCellResult == Match_State::RowLine && FCellResult == Match_State::RowLine
			&& SecClickCell->GetCellType() == FirstClickCell->GetCellType())
		{
			IsSCellResultValid = false;
		}
		// 2) 둘다 ColMatch + 둘이 Type이 동일하다면 --> 인접 
		if (SCellResult == Match_State::ColLine && FCellResult == Match_State::ColLine
			&& SecClickCell->GetCellType() == FirstClickCell->GetCellType())
		{
			IsSCellResultValid = false;
		}
	}

	if (IsSCellResultValid)
	{
		m_vecMatchState[SecClickCell->GetIndex()] = SCellResult;
	}

	bool Result = (int)SCellResult > (int)Match_State::NoMatch || (int)FCellResult > (int)Match_State::NoMatch;

	// todo : 조합 여부도 확인한다.
	bool CombinationResult = m_CombLogicComponent->CheckCombination(FirstClickCell, SecClickCell);

	if (CombinationResult)
		return true;

	// 만약 Match 가 존재하지 않는다면 , 조합도 없다면
	// 여기는 조합에 해당하지 않는다. 위에서 조합을 체크했기 때문에
	// 둘중 하나라도 Mirror Ball 이 존재하는지 확인한다.
	if (!Result && !CombinationResult)
	{
		if (m_FirstClickCell->GetCellState() == Cell_State::MirrorBall)
		{
			Cell_Type_Binary NextCellType = m_SecClickCell->GetCellType();

			SetMirrorBallDestroyInfo(m_FirstClickCell->GetIndex(), NextCellType);

			return true;
		}
		else if (m_SecClickCell->GetCellState() == Cell_State::MirrorBall)
		{
			Cell_Type_Binary NextCellType = m_FirstClickCell->GetCellType();

			SetMirrorBallDestroyInfo(m_SecClickCell->GetIndex(), NextCellType);

			return true;
		}
	}

	return Result;
}


void CBoard::ResetCellsPauseGoDownInfo()
{
	for (int row = 0; row < m_RowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			m_vecCells[row * m_ColCount + col]->SetPauseGoDown(false);
		}
	}
}

bool CBoard::CheckIsCellBeingSpecialDestroyed()
{
	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{

			if (m_vecCells[row * m_ColCount + col]->IsBeingSpecialDestroyed())
				return true;
		}
	}

	return false;
}


void CBoard::SetMirrorBallDestroyInfo(int Index, Cell_Type_Binary DestroyType)
{
	m_vecCells[Index]->SetMirrorBallDestroyType(DestroyType);

	m_vecCells[Index]->SetDestroyState(Destroy_State::MirrorBall);

	m_vecCellIsMatch[Index] = true;
}


void CBoard::ResetAINoticeState()
{
	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			if (m_vecCells[row * m_ColCount + col]->IsPossibleMatch())
			{
				m_vecCells[row * m_ColCount + col]->ResetPossibleNoticeMatch();
			}
		}
	}
}

bool CBoard::Init()
{
	if (!CGameObject::Init())
		return false;

	// Component 세팅
	m_Sprite = CreateComponent<CStaticMeshComponent>("BoardComponent");

	SetRootComponent(m_Sprite);

	m_BoardBackGround = CreateComponent<CSpriteComponent>("BoardBackComponent");
	m_BoardBackGround->SetTexture(0, 0, (int)Buffer_Shader_Type::Graphic, "BoardBack", TEXT("CandyCrush/BoardImage.png"));
	m_BoardBackGround->SetWorldScale(100.f, 100.f, 1.f);
	m_BoardBackGround->SetLayerName("Board");
	m_BoardBackGround->GetMaterial()->SetRenderState("AlphaBlend");
	m_BoardBackGround->SetOpacity(0.5f);
	m_BoardBackGround->SetBaseColor(1.f, 1.f, 1.f, 1.f);

	m_Sprite->AddChild(m_BoardBackGround);

	// ObjectPool 세팅
	m_CellObjectPool = m_Scene->FindGameObjectPool("CellObjectPool");
	m_BlockObjectPool = m_Scene->FindGameObjectPool("BlockObjectPool");

	// Input Callback 세팅
	CInput::GetInst()->SetKeyCallback("BoardCellClick", Key_State::Key_Down, this,
		&CBoard::ClickCell);

	CInput::GetInst()->SetKeyCallback("ChangeToMirrorBall", Key_State::Key_Down, this,
		&CBoard::ChangeToMirrorBallCell);

	// Logic Component Setting
	m_AILogicComponent = CreateComponent<CBoardAILogicComponent>("BoardAI");
	m_AILogicComponent->m_Board = this;

	m_BagLogicComponent = CreateComponent<CBoardBagLogicComponent>("BoardBagLogic");
	m_BagLogicComponent->m_Board = this;

	m_DestroyLogicComponent = CreateComponent<CBoardDestroyLogicComponent>("BoardDestroyLogic");//
	m_DestroyLogicComponent->m_Board = this;
	
	m_CombLogicComponent = CreateComponent<CBoardCombLogicComponent>("BoardCombLogic");
	m_CombLogicComponent->m_Board = this;
	
	m_MatchLogicComponent = CreateComponent<CBoardMatchLogicComponent>("BoardMatchLogic");
	m_MatchLogicComponent->m_Board = this;

	m_BagLogicComponent->m_AILogicComponent = m_AILogicComponent;
	m_AILogicComponent->m_BagLogicComponent = m_BagLogicComponent;

	m_CombLogicComponent->m_DestroyComponent = m_DestroyLogicComponent;
	m_DestroyLogicComponent->m_CombLogicComponent = m_CombLogicComponent;

	m_MatchLogicComponent->m_CombLogicComponent = m_CombLogicComponent;
	m_MatchLogicComponent->m_BagLogicComponent = m_BagLogicComponent;


	// DirectionSpace.m_DRow = { -1, 1, 0, 0 };
	// DirectionSpace.m_DCol = { 0, 0, 1, -1 };

	return true;
}

void CBoard::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	bool PrevFrameCellMoving = m_CellsMoving;

	m_CellsMoving = CheckCellsMoving();

	// 이전 Frame 까지는 움직이고 있다가, 이번에 멈춘 것 --> 새로 AI Check 해야할 상황
	if (PrevFrameCellMoving == true && m_CellsMoving == false)
	{
		// m_IsAIChecked = false;
		m_AILogicComponent->SetAIChecked(false);
		
		// m_AICheckDelayTime = m_AICheckDelayTimeMax;
		m_AILogicComponent->ResetAICheckDelayTime();
	}

	// CreateNewCells();

	bool IsMatchExist = FindMatchUpdate();

	bool IsPossibleMatchExst = m_AILogicComponent->CheckAIAndPossibleMatch(DeltaTime);

	// Match도 없고, 가능한 경우의 수도 없다면.
	if (!IsMatchExist && !IsPossibleMatchExst)
	{
		// 다시 Shuffle 시킨다.
		ShuffleRandom(m_vecCells);
	}
}

void CBoard::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

bool CBoard::CreateBoard(int CountRow, int CountCol, float WidthRatio, float HeightRatio, const Vector3& LB)
{
	m_RowCount = CountRow * 2; // 보여지지 않는 영역까지 포함
	m_ColCount = CountCol;
	m_TotCount = m_RowCount * m_ColCount;
	m_VisualRowCount = CountRow;
	m_IndexOffset = m_ColCount * m_VisualRowCount;

	// m_vecColNewCellNums : 각 열마다 몇개가 새로 생성되어야 하는가
	m_vecColNewCellNums.resize(CountCol);

	// 각 위치의 Cell 마다, 몇개행이 내려가야 하는가
	m_vecCellDownNums.resize(m_TotCount);

	// 각 Cell 의 Match Type
	m_vecMatchState.resize(m_TotCount);

	// 각 Cell의 Match 여부
	m_vecCellIsMatch.resize((int)(m_TotCount * 0.5f));

	// 각 Cell 의 Destroy State
	// m_vecDestroyState.resize(m_TotCount / 2);

	// 각 Cell의 MarkState
	// m_vecDestroyMarkState.resize(m_TotCount / 2);

	int MatchCellEndIdx = (int)(m_TotCount * 0.5f);

	for (int i = 0; i < MatchCellEndIdx; i++)
	{
		m_vecCellIsMatch[i] = false;
	}

	// 시작점 세팅
	m_Sprite->SetWorldPos(LB);

	// 너비 , 높이 세팅
	Resolution RS = CEngine::GetInst()->GetResolution();

	// 높이의 경우 2배로 세팅한다. --> 실제 화면에 보여지는 영역 + 위로 숨은 영역
	// x,y : 열, 행
	m_Sprite->SetWorldScale((float)(RS.Width * (WidthRatio / 100.f)), (float)(RS.Height * (HeightRatio / 100.f)) * 2.f, 1.f);

	// Block, Cell 세팅
	m_vecCells.resize(m_TotCount);

	Vector3 BoardStartPos = LB;
	Vector3 CellSize = GetWorldScale() / Vector3((float)m_ColCount, (float)m_RowCount, 1.f);

	m_CellSize.x = CellSize.x;
	m_CellSize.y = CellSize.y;

	// BackGround Pos, Scale 조정하기
	Vector3 BoardWorldScale = m_Sprite->GetWorldScale();
	Vector3 BoardImaeScale = Vector3(BoardWorldScale.x + CellSize.x * 2.f, BoardWorldScale.y * 0.5f + CellSize.y * 2.f, BoardWorldScale.z);

	m_BoardBackGround->SetWorldScale(BoardImaeScale);
	m_BoardBackGround->SetRelativePos(Vector3(CellSize.x * -1.f, CellSize.y * -1.f, 1.f));
	m_BoardBackGround->AddWorldPos(Vector3(CellSize.x * -0.5f, CellSize.y * -0.5f, 0.f));

	// Mouse Offset 세팅하기
	m_MousePosOffSet = Vector2(0.5f, 0.5f) * m_CellSize;

	// 실제 Cell 생성하기
	//m_BlockObjectPool = new CGameObjectPool<CBlock>(BlockRegisterNum, m_TotCount);
	// m_CellObjectPool = new CGameObjectPool<CCell>((int)FactoryRegisterNum::CellRegisterNum, m_TotCount);

	for (int row = 0; row < m_RowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			float CellWorldYPos = BoardStartPos.y + m_CellSize.y * row;
			Vector3 WorldPos = Vector3(BoardStartPos.x + m_CellSize.x * col, CellWorldYPos, 1.f);

			float NewYPos = BoardStartPos.y + m_CellSize.y * row;

			Cell_Type_Binary CellBType = ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)Cell_Type::End));
			// int Type = (int)(rand() % (int)Cell_Type::End);

			Cell_State CellState = ChangeMatchStateToCellState(Match_State::NoMatch);

			char Name[MAX_PATH] = {};

			sprintf_s(Name, "Cell%d", row * m_ColCount + col);

			// CCell* NewCell =  CreateSingleNewCell(Name, row, col, WorldPos, NewYPos, (Cell_Type)Type, 1.f,true, CellState);
			CCell* NewCell = CreateSingleNewCell(Name, row, col, WorldPos, NewYPos, CellBType, 1.f, true, CellState);

			if (CellWorldYPos >= BoardStartPos.y + m_CellSize.y * m_VisualRowCount)
			{
				// Opacity 설정
				// Cell->SetOpacity(0.0f);
				// NewCell->SetOpacity(0.1f);

				// 안보인다는 멤버 변수 설정
				// NewCell->SetShowEnable(false);
			}
		}
	}

	// 한번 랜덤하게 섞기
	ShuffleRandom(m_vecCells);

	// Block 만들기 -------------------------------------------------------------------
	m_vecBlocks.resize((int)(m_TotCount * 0.5f));

	// m_BlockObjectPool = new CGameObjectPool<CBlock>((int)FactoryRegisterNum::BlockRegisterNum, m_TotCount);

	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			float CellWorldYPos = BoardStartPos.y + m_CellSize.y * row;

			Vector3 WorldPos = Vector3(BoardStartPos.x + m_CellSize.x * col, CellWorldYPos, 1.f);

			int Index = row * m_ColCount + col;

			// CBlock* Block = CSceneManager::GetInst()->GetScene()->CreateGameObject<CBlock>("Block" + std::to_string(Index));
			CBlock* Block = m_Scene->CreateGameObject<CBlock>("Block" + std::to_string(Index));

			Vector3 BoardStartPos = GetWorldPos();

			// Owner 세팅 
			Block->m_Board = this;

			// Scene 세팅 
			Block->SetScene(m_Scene);

			// x는 열, y는 행
			Vector3 BlockWorldPos = Vector3(
				WorldPos.x + m_CellSize.x * 0.1f,
				WorldPos.y + m_CellSize.y * 0.1f,
				0.f);

			Block->SetWorldPos(WorldPos);
			// Block->SetWorldPos(BlockWorldPos);

			// 크기 세팅 
			// Block->SetWorldScale(Vector3(m_CellSize.x, m_CellSize.y, 1.f));
			Block->SetWorldScale(Vector3(m_CellSize.x * 0.9f, m_CellSize.y * 0.9f, 1.f));

			// Index 세팅 --> NewPosY도 세팅
			Block->SetIndexInfo(Index, row, col);

			// Pivot 세팅
			Block->SetPivot(0.5f, 0.5f, 0.f);

			// Block Sprite 크기 , 위치 세팅
			Block->GetSpriteComponent()->SetWorldPos(WorldPos);

			Block->GetSpriteComponent()->SetWorldScale(Vector3(m_CellSize.x * 0.9f, m_CellSize.y * 0.9f, 1.f));

			// vector 목록에 추가 
			m_vecBlocks[Index] = Block;

		}
	}

	return true;
}

void CBoard::SetClickBlockInfo(int Index)
{
	m_ClickedBlock = m_vecBlocks[Index];
	m_ClickedBlock->SetStaticOpacity(8.f);
	m_ClickedBlock->SetStaticBaseColor(0.f, 0.7f, 1.f, 1.f);
}

void CBoard::ResetClickBlockInfo()
{
	if (!m_ClickedBlock)
		return;

	// 그냥 모든 Cell 들을 그렇게 세팅한다.
	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			m_vecBlocks[row * m_ColCount + col]->SetStaticOpacity(0.f);
			m_vecBlocks[row * m_ColCount + col]->SetStaticBaseColor(1.f, 1.f, 1.f, 1.f);
		}
	}

	// m_ClickedBlock->SetStaticOpacity(0.f);
	// m_ClickedBlock->SetStaticBaseColor(1.f, 1.f, 1.f, 1.f);

	m_ClickedBlock = nullptr;
}

void CBoard::ClickCell(float DeltaTime)
{
	// Cell이 이동 중이라면 Click 안먹게 하기 
	if (m_CellsMoving)
		return;

	Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

	Vector2 ConvertPos = MouseWorldPos - Vector2(GetWorldPos().x, GetWorldPos().y) + m_MousePosOffSet;

	int IndexX = (int)(ConvertPos.x / m_CellSize.x);
	int IndexY = (int)(ConvertPos.y / m_CellSize.y);

	// 범위 밖은 조사하지 않는다. + 클릭 정보 초기화
	if (IndexX < 0 || IndexX >= m_ColCount)
	{
		m_FirstClickCell = nullptr;
		m_SecClickCell = nullptr;
		m_MouseClick = Mouse_Click::None;
		return;
	}

	if (IndexY < 0 || IndexY >= m_VisualRowCount)
	{
		m_FirstClickCell = nullptr;
		m_SecClickCell = nullptr;
		m_MouseClick = Mouse_Click::None;
		return;
	}

	// 첫번째 Cell 선택
	if (m_MouseClick == Mouse_Click::None)
	{
		m_FirstClickCell = m_vecCells[IndexY * m_ColCount + IndexX];
		m_MouseClick = Mouse_Click::First;

		// 클릭한 Board 의 Static Mesh Component 색상을 바꿔준다.
		SetClickBlockInfo(IndexY * m_ColCount + IndexX);
	}
	// 두번째 Cell 선택
	else if (m_MouseClick == Mouse_Click::First)
	{
		// 만약 첫번째 선택된 녀석과 같은 녀석이면 skip
		int FirstCellIndex = m_FirstClickCell->GetIndex();
		int SecCellIndex = (IndexY * m_ColCount + IndexX);

		if ((IndexY * m_ColCount + IndexX) == m_FirstClickCell->GetIndex())
		{
			m_MouseClick = Mouse_Click::None;

			ResetClickBlockInfo();

			return;
		}

		// 인접하지 않는다면 Skip
		int FirstCellColIndex = m_FirstClickCell->GetColIndex();
		int FirstCellRowIndex = m_FirstClickCell->GetRowIndex();

		if (std::abs(m_FirstClickCell->GetColIndex() - IndexX) + std::abs(m_FirstClickCell->GetRowIndex() - IndexY) > 1)
		{
			m_MouseClick = Mouse_Click::None;

			ResetClickBlockInfo(); //

			return;
		}

		// Block 은 원 상태로
		ResetClickBlockInfo();

		// AI 로 체크된 Notice 도 원상태로
		ResetAINoticeState();

		m_SecClickCell = m_vecCells[IndexY * m_ColCount + IndexX];

		Vector3 SecondPos = m_SecClickCell->GetWorldPos();
		Vector3 FirstPos = m_FirstClickCell->GetWorldPos();

		// 2개의 Cell 에 새로운 위치 세팅
		m_FirstClickCell->SetClickDestPos(m_SecClickCell->GetWorldPos());
		m_SecClickCell->SetClickDestPos(m_FirstClickCell->GetWorldPos());

		// Switch 중이라고 표시하기
		m_FirstClickCell->SetIsSwitch(true);
		m_SecClickCell->SetIsSwitch(true);

		// Cell 이동중 표시하기
		m_CellsMoving = true;

		// 실시간 CheckUpdate 표시하기 //
		m_IsCheckUpdateNeeded = true;
	}
}

void CBoard::ChangeToMirrorBallCell(float DeltaTime)
{
	if (m_CellsMoving)
		return;

	Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

	Vector2 ConvertPos = MouseWorldPos - Vector2(GetWorldPos().x, GetWorldPos().y);

	int IndexX = (int)(ConvertPos.x / m_CellSize.x);
	int IndexY = (int)(ConvertPos.y / m_CellSize.y);

	if (IndexX < 0 || IndexX >= m_ColCount)
		return;

	if (IndexY < 0 || IndexY >= m_VisualRowCount)
		return;

	int Index = IndexY * m_ColCount + IndexX;

	// m_vecCells[Index]->SetMirrorBallDestroyType(DestroyType);

	// m_vecCells[Index]->SetDestroyState(Destroy_State::MirrorBall);
	// m_vecCells[Index]->SetDestroyMarkState(DestroyMark_State::MirrorBall);

	m_vecCells[Index]->SetCellState(Cell_State::MirrorBall);
}


void CBoard::ShuffleRandom(std::vector<CCell*>& VecCells)
{
	m_CellsMoving = true;

	// 1) Match도 없고 2) Possible Match는 있을 때까지 기다려야 한다.
	// 2) 몇개 초기화 할 변수들이 존재하는가 ?

	ResetCellStateInfo();

	for (int i = 0; i < m_TotCount; i++)
	{
		Cell_Type_Binary CellBType = ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)Cell_Type::End));

		m_vecCells[i]->SetCellType(CellBType);
	}

	while (true)
	{
		if (m_MatchLogicComponent->IsMatchExistForCells(m_vecCells) == false && m_MatchLogicComponent->IsPossibleMatchExistForCells())
			break;

		ResetCellStateInfo();

		for (int i = 0; i < m_TotCount; i++)
		{
			Cell_Type_Binary CellBType = ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)Cell_Type::End));

			m_vecCells[i]->SetCellType(CellBType);
		}
	}

	// 관련 변수 초기화
	// m_IsAIChecked = false;
	m_AILogicComponent->SetAIChecked(false);

	// m_AICheckDelayTime = m_AICheckDelayTimeMax;
	m_AILogicComponent->ResetAICheckDelayTime();

	m_CellsMoving = false;

}

#pragma once

#include "RenderState.h"

class CDepthStencilState : public CRenderState
{
	friend class CRenderStateManager;
protected:
	CDepthStencilState();
	virtual ~CDepthStencilState() override;
protected :
	UINT m_StencilRef;
	UINT m_PrevStencilRef;
public :
	bool CreateDepthStencilState(
		// 깊이 판정 활성화 여부 
		bool DepthEnable = true,
		// 깊이판정에 실패하면, 단편은 폐기되고, 성공하면,단편은, 혼합연산으로 넘어간다.
		// 단편이 깊이 판정과 스텐실 버퍼를 모두 통과한 경우, 깊이 값의 운명은 깊이 쓰기의 활성화
		// 여부에 달려있다. 필드값이 D3D11_DEPTH_WRITE_MASK_ALL 이면, 단편의 깊이값이
		// 깊이 버퍼에 기록되고, 그렇지 않으면 그냥 폐기된다.
		// 다중 패스 렌더링 기법들에서는, 첫 렌더링 패스에서는 앞에서 설명한 표준적인 Z 버퍼 기법을
		// 이용해서, 깊이 버퍼를 채우고, 그 이후의 패스들에서는, 깊이 버퍼를 깊이 판정에만 사용하는 경우가
		// 흔하다. 어차피 가시성은 첫 패스에서 이미 결정이 났으므로, 그 이후엔느
		// 깊이쓰기를 활성화할 필요가 없는 것이다. 깊이 쓰기를 비활성화 하면 깊이 버퍼를
		// 읽기전용으로만 사용할 수 있고, 성능 향상에 도움이 될 것이다.
		D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		// 전달받은 깊이값을, 깊이 버퍼에서 읽은 깊이값과 비교할때 쓰이는 비교함수
		// D3D11_COMPARISON_LESS 가 표준 Z 버퍼 알고리즘
		D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS,
		// 스탠실 판정 활성화 여부 
		bool StencilEnable = false,
		UINT8 StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		// 면이 앞을 보고 있을 경우, 스탠실 테스트 설정
		D3D11_DEPTH_STENCILOP_DESC FrontFace = {
			D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP,
			D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS
		},
		// 면이 뒤를 보고 있을 경우, 스탠실 테스트 설정 
		D3D11_DEPTH_STENCILOP_DESC BackFace = {
			D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP,
			D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS
		});
public:
	virtual void SetState() override;
	virtual void ResetState() override;
};


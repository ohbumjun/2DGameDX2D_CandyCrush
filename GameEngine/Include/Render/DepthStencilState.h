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
		// ���� ���� Ȱ��ȭ ���� 
		bool DepthEnable = true,
		// ���������� �����ϸ�, ������ ���ǰ�, �����ϸ�,������, ȥ�տ������� �Ѿ��.
		// ������ ���� ������ ���ٽ� ���۸� ��� ����� ���, ���� ���� ����� ���� ������ Ȱ��ȭ
		// ���ο� �޷��ִ�. �ʵ尪�� D3D11_DEPTH_WRITE_MASK_ALL �̸�, ������ ���̰���
		// ���� ���ۿ� ��ϵǰ�, �׷��� ������ �׳� ���ȴ�.
		// ���� �н� ������ ����鿡����, ù ������ �н������� �տ��� ������ ǥ������ Z ���� �����
		// �̿��ؼ�, ���� ���۸� ä���, �� ������ �н��鿡����, ���� ���۸� ���� �������� ����ϴ� ��찡
		// ���ϴ�. ������ ���ü��� ù �н����� �̹� ������ �����Ƿ�, �� ���Ŀ���
		// ���̾��⸦ Ȱ��ȭ�� �ʿ䰡 ���� ���̴�. ���� ���⸦ ��Ȱ��ȭ �ϸ� ���� ���۸�
		// �б��������θ� ����� �� �ְ�, ���� ��� ������ �� ���̴�.
		D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		// ���޹��� ���̰���, ���� ���ۿ��� ���� ���̰��� ���Ҷ� ���̴� ���Լ�
		// D3D11_COMPARISON_LESS �� ǥ�� Z ���� �˰���
		D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS,
		// ���Ľ� ���� Ȱ��ȭ ���� 
		bool StencilEnable = false,
		UINT8 StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		// ���� ���� ���� ���� ���, ���Ľ� �׽�Ʈ ����
		D3D11_DEPTH_STENCILOP_DESC FrontFace = {
			D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP,
			D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS
		},
		// ���� �ڸ� ���� ���� ���, ���Ľ� �׽�Ʈ ���� 
		D3D11_DEPTH_STENCILOP_DESC BackFace = {
			D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP,
			D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS
		});
public:
	virtual void SetState() override;
	virtual void ResetState() override;
};


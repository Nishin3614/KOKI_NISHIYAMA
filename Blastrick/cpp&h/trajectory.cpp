// ------------------------------------------
//
// �O�Ղ̏���[trajectory.h]
// Author : Nishiyama koki
//
// ------------------------------------------
// ------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------
#include "trajectory.h"
#include "meshobit.h"

// ------------------------------------------
// �ÓI�����o�ϐ��̏�����
// ------------------------------------------

// ------------------------------------------
// �R���X�g���N�^
// ------------------------------------------
CTrajectory::CTrajectory()
{
}

// ------------------------------------------
// �f�X�g���N�^
// ------------------------------------------
CTrajectory::~CTrajectory()
{
}

// ------------------------------------------
// ������
// ------------------------------------------
void CTrajectory::Init(void)
{
}

// ------------------------------------------
// �I��
// ------------------------------------------
void CTrajectory::Uninit(void)
{

}

// ------------------------------------------
// �X�V
// ------------------------------------------
void CTrajectory::Update(void)
{
}

// ------------------------------------------
// �`��
// ------------------------------------------
void CTrajectory::Draw(void)
{
}

// ------------------------------------------
// �ݒ�
// 1�s��A2�^�C�v
// ------------------------------------------
void CTrajectory::Set(
	D3DXMATRIX *pmtx,	// �s��
	TYPE type,			// �^�C�v
	int &nFrame		// �t���[����
)
{
	switch (type)
	{
	case TYPE_NORMAL:
		// ��
		CMeshobit::Set(
			pmtx, 
			D3DXVECTOR3(0.0f, 0.0f,0.0f),
			D3DXVECTOR3(0.0f, 80.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_0,
			100.0f,
			40,
			nFrame
		);
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 85.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			CMeshobit::TYPE_0, 
			CMeshobit::TEX_0,
			100.0f,
			40,
			nFrame
		);
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(5.0f, 80.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_1,
			100.0f,
			40,
			nFrame
		);
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 80.0f, 5.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_1,
			100.0f,
			40,
			nFrame
		);
		break;
	case TYPE_TRICK1:
		// ��
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 150.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_0,
			100.0f,
			80,
			nFrame
		);
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 155.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_0,
			100.0f,
			80,
			nFrame
		);
			break;
	case TYPE_TRICK2:
		// ��
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 200.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_0,
			100.0f,
			80,
			nFrame
		);
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 205.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_0,
			100.0f,
			80,
			nFrame
		);
		break;
	default:
		break;
	}
}

// ------------------------------------------
// ����
// ------------------------------------------
CTrajectory * CTrajectory::Create(void)
{
	CTrajectory *pTrajectory = new CTrajectory;
	return pTrajectory;
}

// ------------------------------------------
// �ǂݍ���
// ------------------------------------------
HRESULT CTrajectory::Load(void)
{
	CMeshobit::Load();
	return S_OK;
}

// ------------------------------------------
// ���炩���ߋO�Ղ̐���������
// ------------------------------------------
HRESULT CTrajectory::LoadCreate(void)
{
	return S_OK;
}

// ------------------------------------------
// �J��
// ------------------------------------------
void CTrajectory::Unload(void)
{
	CMeshobit::UnLoad();
}

#ifdef _DEBUG
// ------------------------------------------
// �f�o�b�O�\��
// ------------------------------------------
void CTrajectory::Debug(void)
{
}
#endif // _DEBUG

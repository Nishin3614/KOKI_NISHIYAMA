// ------------------------------------------
//
// ���o�̏���[perfomance.h]
// Author : Nishiyama koki
//
// ------------------------------------------
// ------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------
#include "perfomance.h"
#include "trajectory.h"
#include "3Dparticle.h"
#include "meshsphere.h"
#include "bgdome.h"

// ------------------------------------------
// �ÓI�����o�ϐ��̏�����
// ------------------------------------------

// ------------------------------------------
// �R���X�g���N�^
// ------------------------------------------
CPerfomance::CPerfomance()
{
}

// ------------------------------------------
// �f�X�g���N�^
// ------------------------------------------
CPerfomance::~CPerfomance()
{
}

// ------------------------------------------
// �ݒ�
// 1�s��A2�^�C�v
// ------------------------------------------
void CPerfomance::Set(
	D3DXMATRIX *pmtx,	// �s��
	TYPE type,			// �^�C�v
	D3DXVECTOR3 &pos,	// �ʒu
	D3DXVECTOR3 &rot,	// ��]
	int nFrame			// �t���[����
	)
{
	/*
	switch (type)
	{
	case TYPE_0:
		// �O�Ղ̐ݒ�
		CTrajectory::Set(
			pmtx,					// �ʒu
			CTrajectory::TYPE_0,	// �^�C�v
			nFrame					// �t���[����
		);
		// �p�[�e�B�N������
		C3DParticle::Create(
			C3DParticle::OFFSET_ID_STAR,
			pos
		);
		/*
		// �p�[�e�B�N������
		C3DParticle::Create(
			C3DParticle::OFFSET_ID_CROSSLINE,
			D3DXVECTOR3(
				pmtx->_41,
				pmtx->_42,
				pmtx->_43
			)
		);
		// �ʒu
		D3DXVec3TransformCoord(
			&m_pos,
			&m_offset,
			m_mtx
		);

		// �p�[�e�B�N������
		C3DParticle::Create(
			C3DParticle::OFFSET_ID_RANDAMLINE,
			D3DXVECTOR3(
				pmtx->_41,
				pmtx->_42,
				pmtx->_43
			)
		);
		// ���b�V���X�t�B�A
		CMeshsphere::Set(
			pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f),
			500.0f,
			nFrame
		);
		/*
		// �w�i�̃h�[��
		CBgdome::Set(
			pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			nFrame
		);
		break;
	case TYPE_1:
		// �O�Ղ̐ݒ�
		CTrajectory::Set(
			pmtx,					// �ʒu
			CTrajectory::TYPE_0,	// �^�C�v
			nFrame					// �t���[����
		);
		// �p�[�e�B�N������
		C3DParticle::Create(
			C3DParticle::OFFSET_ID_STAR,
			pos
		);

		// ���b�V���X�t�B�A
		CMeshsphere::Set(
			pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f),
			500.0f,
			nFrame
		);
		/*
		// �w�i�̃h�[��
		CBgdome::Set(
			pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			nFrame
		);
		break;

	default:
		break;
	}
*/
}

#ifdef _DEBUG
// ------------------------------------------
// �f�o�b�O�\��
// ------------------------------------------
void CPerfomance::Debug(void)
{
}
#endif // _DEBUG

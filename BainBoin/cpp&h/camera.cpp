//=============================================================================
//
// �J�������� [camera.cpp]
// Author : KOKI_NISHIYAMA
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "joypad.h"
#include "manager.h"
#include "debugproc.h"
#include "scene_X.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CCamera::CCamera()
{
	m_fx = 1.0f;
	m_fz = 1.0f;
	m_fY = 500.0f;

	// �J�����̏�����
	m_posV = D3DXVECTOR3(m_fx, m_fY, m_fz);
	m_posVDest = m_posV;
	m_posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_move = D3DXVECTOR3(1.0f, 3.0f, 3.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0;
	m_nCntTime = 0;
	m_bfront = false;

	// ���_�ƒ����_�Ƃ̋����v�Z
	D3DXVECTOR3 diffPos = m_posV - m_posR;
	// �����v�Z
	m_fLength = sqrtf(diffPos.x*diffPos.x + diffPos.z * diffPos.z);
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CCamera::~CCamera()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CCamera::Init(void)
{
	m_fx = 1.0f;
	m_fz = -700.0f;
	m_fY = 400.0f;

	// �J�����̏�����
	m_posV = D3DXVECTOR3(m_fx, m_fY, m_fz);
	m_posVDest = m_posV;
	m_posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = m_posR;
	m_posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_move = D3DXVECTOR3(1.0f, 3.0f, 3.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0;
	m_nCntTime = 0;
	m_bfront = false;
	// ���_�ƒ����_�Ƃ̋����v�Z
	D3DXVECTOR3 diffPos = m_posV - m_posR;

	// �����v�Z
	m_fLength = sqrtf(diffPos.x*diffPos.x + diffPos.z * diffPos.z);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CCamera::Uninit(void)
{
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CCamera::Update(void)
{
	// �v���C���[�Ǐ]
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		PlayerFoll();
	}
	// ��]
	Rot();
	// �����_���ɉ�] //
	//-------------------- ���_�ړ� --------------------//
	// �ړI�̎��_
	m_posVDest.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posVDest.y =
		m_posR.y + m_fY;
	m_posVDest.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;

	// �ړI�n���猻�ݒn�̋���(�����_)
	m_posVDiff =
		m_posVDest - m_posV;

	// �����_�Ɋ���
	m_posV.x += m_posVDiff.x * 0.1f;
	m_posV.y += m_posVDiff.y * 0.1f;
	m_posV.z += m_posVDiff.z * 0.1f;
	/*
	CDebugproc::Print("�J�����̎��_x:%f,y:%f,z:%f\n", m_posVDest.x, m_posVDest.y, m_posVDest.z);
	CDebugproc::Print("�J�����̒����_x:%f,y:%f,z:%f\n", m_posR.x, m_posR.y, m_posR.z);
	CDebugproc::Print("�J�����̊p�xx:%f,y:%f,z:%f\n", m_rot.x, m_rot.y, m_rot.z);
	*/
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CCamera::Draw(void)
{
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CCamera * CCamera::Create(void)
{
	// �ϐ��錾
	CCamera * pCamera;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pCamera = new CCamera();
	// ����������
	pCamera->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pCamera;
}

// ----------------------------------------
// �擾
// ----------------------------------------
CCamera * CCamera::Get(void)
{
	return nullptr;
}

// ----------------------------------------
// �ݒ�
// ----------------------------------------
void CCamera::Set(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X //
	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(
		&m_mtxProjection,					// (�e���r���_)
		D3DXToRadian(45.0f),						// ����p(��p)
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// �A�X�y�N�g��
		10.0f,										// MearZ�l(�߂��r���[���ʂ�Z�l)
		20000.0f);									// FarZ�l(�����r���[���ʂ�Z�l)�����l��1000

													// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X //
	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(
		&m_mtxView,	// (�J�����}�����_)
		&m_posV,		// �J�����̎��_
		&m_posR,		// �J�����̒��ړ_
		&m_posU);	// �J�����̏�����x�N�g��

							// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

// ----------------------------------------
// �J�����̕���
// ----------------------------------------
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}

// ----------------------------------------
// �v���C���[�Ǐ]
// ----------------------------------------
void CCamera::PlayerFoll(void)
{
	/* �ϐ��錾 */
	CPlayer * pPlayer = NULL;	// �v���C���[
	// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntLayer++)
	{
		if (pPlayer == NULL) pPlayer = (CPlayer*)CScene::GetActor(CScene::ACTOR_PLAYER, CScene::LAYER_3DOBJECT, nCntLayer);	// �v���C���[
	}

	m_nCntTime++;				// �J�E���g�^�C��
	//-------------------- ���_�ړ� --------------------//
	// �Q�[���̃J�����̉�]�̖ړI�n
	m_posR = pPlayer->GetPos();
}

// ----------------------------------------
// ��]
// ----------------------------------------
void CCamera::Rot(void)
{
	// �����_���ɉ�] //
	// �����v���
	//if (CManager::GetKeyboard()->GetKeyboardPress(DIK_RIGHT))
	//{
	//	m_rot.y += 0.1f;

	//	// -3.14�`3.14�ɉ�]�������Œ肳����
	//	m_rot.y = CCalculation::Rot_One_Limit(m_rot.y);

	//	// ���_�X�V
	//	m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * m_fLength;
	//	m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * m_fLength;

	//	// �J�E���g�^�C��������
	//	m_nCntTime = 0;

	//	// ���ʂ�������off��
	//	m_bfront = false;
	//}

	//// ���v���
	//else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_LEFT))
	//{
	//	m_rot.y -= 0.1f;

	//	// -3.14�`3.14�ɉ�]�������Œ肳����
	//	m_rot.y = CCalculation::Rot_One_Limit(m_rot.y);

	//	// ���_�X�V
	//	m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * m_fLength;
	//	m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * m_fLength;

	//	// �J�E���g�^�C��������
	//	m_nCntTime = 0;

	//	// ���ʂ�������off��
	//	m_bfront = false;
	//}
	// ��
	//if (CManager::GetKeyboard()->GetKeyboardPress(DIK_UP))
	//{
	//	m_fY += 10.0f;
	//}
	//// ��
	//if (CManager::GetKeyboard()->GetKeyboardPress(DIK_DOWN))
	//{
	//	m_fY -= 10.0f;
	//}
}

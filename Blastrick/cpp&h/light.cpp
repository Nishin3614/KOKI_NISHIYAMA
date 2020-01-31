//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : KOKI_NISHIYAMA
//
//=============================================================================
#include "light.h"
#include "manager.h"

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CLight::CLight()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	// �J�E���g���C�g
	for (int nCntLight = 0; nCntLight < TYPE_MAX; nCntLight++)
	{
		// ���C�g���N���A����
		ZeroMemory(&m_light[nCntLight], sizeof(D3DLIGHT9));

		// ���C�g�̎�ނ�ݒ�
		m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ���C�g�̊g�U����ݒ�
		m_light[nCntLight].Diffuse = m_col[nCntLight];

		// ���C�g�̕����̐ݒ�
		D3DXVec3Normalize(&m_VecDir[nCntLight], &m_VecDir[nCntLight]);		// ���K��
		m_light[nCntLight].Direction = m_VecDir[nCntLight];					// ���C�g�ɑ��

		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CLight::~CLight()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CLight::Init(void)
{
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CLight::Uninit(void)
{
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CLight::Update(void)
{
	/*
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	// �鉜�`1
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_U))
	{
		m_VecDir[0].x += 0.01f;
	}
	// �鉜�`1
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_I))
	{
		m_VecDir[0].y += 0.01f;
	}
	// �鉜�`1
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_O))
	{
		m_VecDir[0].z += 0.01f;
	}


	// ���C�g�̕����̐ݒ�
	D3DXVec3Normalize(&m_VecDir[0], &m_VecDir[0]);		// ���K��
	m_light[0].Direction = m_VecDir[0];					// ���C�g�ɑ��

	// ���C�g��ݒ肷��
	pDevice->SetLight(0, &m_light[0]);

	CDebugproc::Print("���C�g�̕���(%.3f,%.3f,%.3f)\n", m_VecDir[0].x, m_VecDir[0].y, m_VecDir[0].z);
	*/
}

// ----------------------------------------
// ��������
// ----------------------------------------
CLight * CLight::Create(void)
{
	// �ϐ��錾
	CLight * pLight;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pLight = new CLight();
	// ����������
	pLight->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pLight;
}

// ----------------------------------------
// ���C�g���擾����
// ----------------------------------------
D3DLIGHT9 & CLight::GetLight(TYPE type)
{
	return m_light[type];
}

// ----------------------------------------
// �F���擾����
// ----------------------------------------
D3DXCOLOR & CLight::GetCol(TYPE type)
{
	return m_col[type];
}

// ----------------------------------------
// �������擾����
// ----------------------------------------
D3DVECTOR & CLight::GetVec(TYPE type)
{
	return 	m_light[type].Direction;
}

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
	for (int nCntLight = 0; nCntLight < LIGHT_MAX; nCntLight++)
	{
		// ���C�g���N���A����
		ZeroMemory(&m_light[nCntLight], sizeof(D3DLIGHT9));

		// ���C�g�̎�ނ�ݒ�
		m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ���C�g�̊g�U����ݒ�
		m_light[nCntLight].Diffuse = m_col[nCntLight];

		// ���C�g�̕����̐ݒ�
		D3DXVec3Normalize(&m_VecDir[nCntLight], &m_VecDir[nCntLight]);			// ���K��
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

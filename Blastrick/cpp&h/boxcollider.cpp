// ----------------------------------------
//
// ���̓����蔻�菈���̐���[boxcollider.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "boxcollider.h"

// ----------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CBoxcollider::CBoxcollider()
{
	// �ϐ��̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_rot = 0;
}

// ----------------------------------------
// �C�j�V�����C�U�R���X�g���N�^
// ----------------------------------------
CBoxcollider::CBoxcollider(ACTOR actor, LAYER layer) : CScene::CScene(actor,layer)
{
	// �ϐ��̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = 0;
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CBoxcollider::~CBoxcollider()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CBoxcollider::Init(void)
{
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CBoxcollider::Uninit(void)
{
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CBoxcollider::Update(void)
{
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CBoxcollider::Draw(void)
{
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
//-------------------------------------------------------------------------------------------------------------
void CBoxcollider::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------
// �쐬����
// ----------------------------------------
CBoxcollider * CBoxcollider::Create()
{
	// �ϐ��錾
	CBoxcollider * pBoxcollider;		// ���̓����蔻��N���X

	// �������̐���(����->��{�N���X,��->�h���N���X)
	pBoxcollider = new CBoxcollider(ACTOR_BOXCOLLIDER,LAYER_UI);

	// ����������
	pBoxcollider->Init();
	
	// ���������I�u�W�F�N�g��Ԃ�
	return pBoxcollider;
}

// ----------------------------------------
// �T�C�Y�ݒ菈��
// ----------------------------------------
void CBoxcollider::SetSize(D3DXVECTOR2 size)
{
	m_size = size;
}

// ----------------------------------------
// �ʒu���ݒ菈��
// ----------------------------------------
void CBoxcollider::SetPosition(D3DXVECTOR3 pos)
{
}

// ----------------------------------------
// ��]���擾����
// ----------------------------------------
void CBoxcollider::SetRot(float frot)
{
}

// ----------------------------------------
// �T�C�Y���擾����
// ----------------------------------------
D3DXVECTOR2 CBoxcollider::GetSize(void)
{
	return m_size;
}

// ----------------------------------------
// �ʒu���擾����
// ----------------------------------------
D3DXVECTOR3 CBoxcollider::GetPosition(void)
{
	return m_pos;
}
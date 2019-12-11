// ----------------------------------------
//
// �u���b�N�����̐���[block.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "block.h"
#include "model_info.h"

// ----------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------
LPDIRECT3DTEXTURE9 CBlock::m_pTexture[TYPE_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
CLoad	*CBlock::m_pload = NULL;							// ���[�h
CModel_info	*CBlock::m_pmodel_info = NULL;					// ���f�����
int		CBlock::m_nAll = 0;									// �ԍ�

// ----------------------------------------
// �C�j�V�����C�U�R���X�g���N�^
// ----------------------------------------
CBlock::CBlock() : CScene_X::CScene_X(ACTOR_BLOCK,LAYER_3DOBJECT)
{
	m_Type = TYPE_1;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CBlock::~CBlock()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CBlock::Init(void)
{
	// X�I�u�W�F�N�g����
	CScene_X::Init();
	// ���f�����ݒ�
	CScene_X::BindModel(m_pmodel_info->GetModel_info());
	// �e�N�X�`���[�ݒ�
	CScene_X::BindTexture(m_pTexture[m_Type]);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CBlock::Uninit(void)
{
	CScene_X::Uninit();
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CBlock::Update(void)
{
	CScene_X::Update();
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CBlock::Draw(void)
{
	CScene_X::Draw();
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CBlock * CBlock::Create(void)
{
	// �ϐ��錾
	CBlock * pBlock;		// �V�[��3D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pBlock = new CBlock();
	// ����������
	pBlock->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pBlock;
}

// ----------------------------------------
// �ǂݍ��ݍ쐬����
// ----------------------------------------
void CBlock::LoadCreate(void)
{
	// �ϐ��錾
	CBlock * pBlock;	// �u���b�N
	// �u���b�N
	for (int nCntBlock = 0; nCntBlock < m_nAll; nCntBlock++)
	{
		pBlock = CBlock::Create();							// ����
		pBlock->SetPos(m_pload->GetInfo(nCntBlock).pos);	// �ʒu
		pBlock->SetRot(m_pload->GetInfo(nCntBlock).rot);	// ��]
	}
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CBlock::Load(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =					// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	char cTex[TYPE_MAX][128] =					// �e�N�X�`���[��
	{
		"data/TEXTURE/block.png",
	};
	char cModelfile[TYPE_MAX][128] =			// ���f���t�@�C����
	{
		"data/MODEL/object/block.x",
	};
	// �e�N�X�`���[�ݒ�
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice, cTex[nCnt], &m_pTexture[nCnt]);
	}
	// ���[�h����
	m_pload = new CLoad;
	// ���[�h�ǂݍ���
	m_nAll = m_pload->Load("data/LOAD/OBJECT/materials.txt");
	// ���f����񐶐�
	m_pmodel_info = new CModel_info;
	// ���f���ǂݍ���
	m_pmodel_info->Load(pDevice, cModelfile[0]);
	return S_OK;
}

// ----------------------------------------
// unload����
// ----------------------------------------
void CBlock::UnLoad(void)
{
	// �e�N�X�`���[�ݒ�
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
	// ���[�h�̔j��
	delete m_pload;
	m_pload = NULL;
	// ���f���̔j��
	m_pmodel_info->Unload();
	delete m_pmodel_info;
	m_pmodel_info = NULL;
}

// ----------------------------------------
// �ʒu�̐ݒ菈��
// ----------------------------------------
void CBlock::SetPos(D3DXVECTOR3 pos)
{
	CScene_X::SetPos(pos);
}

// ----------------------------------------
// ��]�̐ݒ菈��
// ----------------------------------------
void CBlock::SetRot(D3DXVECTOR3 rot)
{
	CScene_X::SetRot(rot);
}

// ----------------------------------------
// �ʒu�̎擾����
// ----------------------------------------
D3DXVECTOR3 CBlock::GetPos(void)
{
	return CScene_X::GetPos();
}
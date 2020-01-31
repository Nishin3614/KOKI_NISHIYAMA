//=============================================================================
//
// ���f������ [player.cpp]
// Author : KOKI NISHIYAMA
//
//=============================================================================
#include "model.h"
#include "light.h"
#include "shadow_mapping.h"

//=============================================================================
// �}�N����`
//=============================================================================

// ----------------------------------------------------------------------------------------------------
// �R���X�g���N�^����
// ----------------------------------------------------------------------------------------------------
CModel::CModel()
{
	m_Xmodel.pMesh = NULL;
	m_Xmodel.pBuffMat = NULL;
	m_Xmodel.nNumMat = 0;
	m_Xmodel.vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.nParent = 0;
	m_Parent = NULL;
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nFrame = 0;
}

// ----------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ----------------------------------------------------------------------------------------------------
CModel::~CModel()
{
}

// ----------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------
void CModel::Init(void)
{
	m_Xmodel.pMesh = NULL;
	m_Xmodel.pBuffMat = NULL;
	m_Xmodel.nNumMat = 0;
	m_Xmodel.vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.nParent = 0;
	m_Parent = NULL;
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nFrame = 0;
}

// ----------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------
void CModel::Uninit(void)
{
}

// ----------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------
void CModel::Update(void)
{
	// ���f�����̃��[�V��������
	m_Xmodel.pos += m_Xmodel.posDiff / (float)m_nFrame;
	m_Xmodel.rot += m_Xmodel.rotDiff / (float)m_nFrame;
}

// ----------------------------------------------------------------------------------------------------
// �`�揈��
// ----------------------------------------------------------------------------------------------------
void CModel::Draw(
	D3DXMATRIX & mtx,
	float & fAlpha
)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX			mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL		*pMat;					// ���݂̃}�e���A���ۑ�
	D3DMATERIAL9		matDef;					// �}�e���A���f�[�^�̃|�C���^
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_Xmodel.rot.y,
		m_Xmodel.rot.x,
		m_Xmodel.rot.z);

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld, &mtxRot);


	// �ʒu�𔽉f //
	// ���s�ړ��s��쐬(�I�t�Z�b�g)
	D3DXMatrixTranslation(&mtxTrans,	// �����̓��ꕨ
		m_Xmodel.pos.x,
		m_Xmodel.pos.y,
		m_Xmodel.pos.z);

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// �e���������Ă���Ƃ�
	if (m_Parent != NULL)
	{
		// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			&m_Parent->m_mtxWorld);
	}
	// ���ׂĂ̐e�̏ꍇ
	else
	{
		// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			&mtx);
	}

	// �V���h�E�}�b�s���O
	CShadowmapping::Draw(
	pDevice,	// �f�o�C�X���
	m_Xmodel,	// ���f�����
	m_mtxWorld	// �}�g���b�N�X���
	);
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);
	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_Xmodel.pBuffMat->GetBufferPointer();

	// �J�E���g�}�e���A��
	for (int nCntMat = 0; nCntMat < (int)m_Xmodel.nNumMat; nCntMat++, pMat++)
	{
		pMat->MatD3D.Diffuse.a = fAlpha;
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat->MatD3D);
		// �e�N�X�`���[�ݒ�
		pDevice->SetTexture(0, m_Xmodel.ppTexture[nCntMat]);
		// �`��
		m_Xmodel.pMesh->DrawSubset(nCntMat);
	}
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

// ----------------------------------------------------------------------------------------------------
// ��������
// ----------------------------------------------------------------------------------------------------
CModel * CModel::Create()
{
	// �ϐ��錾
	CModel * pModel;	// ���f��
	// �������m��
	pModel = new CModel();
	// ������
	pModel->Init();
	// ����Ԃ�
	return pModel;
}

// ----------------------------------------------------------------------------------------------------
// �e�p�[�c�̐ݒ菈��
// ----------------------------------------------------------------------------------------------------
void CModel::SetParent(CModel * pModel)
{
	m_Parent = pModel;
}

// ----------------------------------------------------------------------------------------------------
// ���f���̏��n������
// ----------------------------------------------------------------------------------------------------
void CModel::BindModel(
	XMODEL const &xModel,		// ���f���̏��
	MODEL_OFFSET const &offset	// ���f���̃I�t�Z�b�g
)
{
	// ���f�����̎擾
	m_Xmodel = xModel;
	// ���f���̃I�t�Z�b�g
	m_Xmodel.pos = offset.pos;
	m_Xmodel.rot = offset.rot;
	m_Xmodel.nParent = offset.nParent;
}

// ----------------------------------------------------------------------------------------------------
// ���f���̈ʒu�ݒ菈��
// ----------------------------------------------------------------------------------------------------
void CModel::SetPos(D3DXVECTOR3 &pos)
{
	m_Xmodel.pos = pos;
}

// ----------------------------------------------------------------------------------------------------
// ���[�V�����̐ݒ菈��
// ----------------------------------------------------------------------------------------------------
void CModel::SetMotion(
	KEY &key, 
	int nMaxFrame
)
{
	m_Xmodel.posDest = key.pos;
	m_Xmodel.rotDest = key.rot;
	m_Xmodel.posDiff = (m_Xmodel.posDest - m_Xmodel.pos);
	m_Xmodel.rotDiff = (m_Xmodel.rotDest - m_Xmodel.rot);
	m_nFrame = nMaxFrame;
}

// ----------------------------------------------------------------------------------------------------
// ���f�����擾����
// ----------------------------------------------------------------------------------------------------
XMODEL & CModel::GetXModel(void)
{
	return m_Xmodel;
}

// ----------------------------------------------------------------------------------------------------
// �s����擾����
// ----------------------------------------------------------------------------------------------------
D3DXMATRIX & CModel::GetMtx(void)
{
	return m_mtxWorld;
}

// ----------------------------------------------------------------------------------------------------
// �ʒu���擾����
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 & CModel::GetPos(void)
{
	return m_Xmodel.pos;
}

// ----------------------------------------------------------------------------------------------------
// ��]���擾����
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 & CModel::GetRot(void)
{
	return m_Xmodel.rot;
}

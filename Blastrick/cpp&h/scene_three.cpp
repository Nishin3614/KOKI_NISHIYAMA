// ----------------------------------------
//
// 3D�V�[�������̐���[scene_three.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "scene_three.h"
#include "manager.h"

// ----------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CScene_THREE::CScene_THREE()
{
	// �ϐ��̏�����
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_fLengh = 0;
	m_fAngle = 0;
	m_bBillboard = false;
}

// ----------------------------------------
// �C�j�V�����C�U�R���X�g���N�^
// ----------------------------------------
CScene_THREE::CScene_THREE(ACTOR actor, LAYER layer) : CScene::CScene(actor,layer)
{
	// �ϐ��̏�����
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(100, 0.0f, 100);
	m_fLengh = 0;
	m_fAngle = 0;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CScene_THREE::~CScene_THREE()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CScene_THREE::Init(void)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	D3DXVECTOR3 VecA, VecB;					// �x�N�g��
	D3DXVECTOR3 Cross0, Cross1_2, Cross3;	// �O��
	int nCntBlock = 0;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	pVtx[0].pos = D3DXVECTOR3(-100, 0, 100);
	pVtx[1].pos = D3DXVECTOR3(100, -50, 100);
	pVtx[2].pos = D3DXVECTOR3(-100, -50,-100);
	pVtx[3].pos = D3DXVECTOR3(0, 0, -0);

	// �x�N�g��
	VecA = pVtx[1].pos - pVtx[0].pos;
	VecB = pVtx[2].pos - pVtx[0].pos;
	// �O�όv�Z
	Cross0 = CCalculation::Cross_product(VecA, VecB);
	// �x�N�g��
	VecA = pVtx[2].pos - pVtx[3].pos;
	VecB = pVtx[1].pos - pVtx[3].pos;
	// �O�όv�Z
	Cross3 = CCalculation::Cross_product(VecA, VecB);
	// ���K��
	D3DXVec3Normalize(&Cross0, &Cross0);
	D3DXVec3Normalize(&Cross3, &Cross3);
	// �O�όv�Z
	Cross1_2 = (Cross0 + Cross3) / 2;

	pVtx[0].nor = Cross0;
	pVtx[1].nor = Cross1_2;
	pVtx[2].nor = Cross1_2;
	pVtx[3].nor = Cross3;

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CScene_THREE::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
 		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CScene_THREE::Update(void)
{
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CScene_THREE::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f //
	// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
		m_rot.y, m_rot.x, m_rot.z);	// ���ꂼ��̉�]��

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,					// 2
		&mtxRot);							// 3


	// �ʒu�𔽉f //
	// ���s�ړ��s��쐬(�I�t�Z�b�g)
	D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
		m_pos.x, m_pos.y, m_pos.z);	// ���ꂼ��̈ړ���

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,					// 2
		&mtxTrans);							// 3

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(
		0,
		m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2
		);
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
//-------------------------------------------------------------------------------------------------------------
void CScene_THREE::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------
// �쐬����
// ----------------------------------------
CScene_THREE * CScene_THREE::Create()
{
	// �ϐ��錾
	CScene_THREE * pScene_Two;		// �V�[��3D�N���X

	// �������̐���(����->��{�N���X,��->�h���N���X)
	pScene_Two = new CScene_THREE(ACTOR_SCENE_THREE,LAYER_3DOBJECT);

	// ����������
	pScene_Two->Init();
	
	// ���������I�u�W�F�N�g��Ԃ�
	return pScene_Two;
}

// ----------------------------------------
// �ڂ��Ă���ʂ̍������擾
// ----------------------------------------
float CScene_THREE::GetHeight(D3DXVECTOR3 pos)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;						// ���_���ւ̃|�C���g
	D3DXVECTOR3 VecA, VecB;					// �x�N�g��
	D3DXVECTOR3 Cross0, Cross1, Cross2;	// �O��
	D3DXVECTOR3 CrossPolygon;			// �|���S���̖@��
	D3DXVECTOR3 CrossAns;				// �ʒu�̊O��

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// �x�N�g��
	VecA = pVtx[1].pos - pVtx[0].pos;
	VecB = pos - pVtx[0].pos;
	// �O�όv�Z
	Cross0 = CCalculation::Cross_product(VecA, VecB);
	// �x�N�g��
	VecA = pVtx[2].pos - pVtx[1].pos;
	VecB = pos - pVtx[1].pos;
	// �O�όv�Z
	Cross1 = CCalculation::Cross_product(VecA, VecB);
	// �x�N�g��
	VecA = pVtx[0].pos - pVtx[2].pos;

	VecB = pos - pVtx[2].pos;
	// �O�όv�Z
	Cross2 = CCalculation::Cross_product(VecA, VecB);
	// �|���S���̖@��
	CrossPolygon = pVtx[0].nor;
	VecA = pos - pVtx[0].pos;

	CrossAns = CCalculation::Cross_product(VecA, CrossPolygon);
	// ���K��
	D3DXVec3Normalize(&Cross0, &Cross0);
	D3DXVec3Normalize(&Cross1, &Cross1);
	D3DXVec3Normalize(&Cross2, &Cross2);
	D3DXVec3Normalize(&CrossAns, &CrossAns);
	if (Cross0.y > 0 && Cross1.y > 0 && Cross2.y > 0)
	{
		// �|���S���̖@��
		D3DXVECTOR3 VecAB, VecBC;
		VecA = pos - (m_pos + pVtx[0].pos);
		//D3DXVec3Normalize(&VecA, &VecA);

		float Dot = 0;				// ����
		float fY = 0;				// ����
		// ����
		Dot = CCalculation::Dot_product(VecA, CrossPolygon);
		// �|���S���̎w��ʒu�̍����v�Z
		CrossPolygon = pVtx[0].nor;
		fY = pVtx[0].pos.y + ((-CrossPolygon.x * VecA.x) + (-CrossPolygon.z * VecA.z)) / CrossPolygon.y;
		//fY = pVtx[0].pos.y + ((-CrossPolygon.x * pVtx[0].nor.x) + (-CrossPolygon.z * pVtx[0].nor.z)) / CrossPolygon.y;
		//CDebugproc::Print("[�����v�Z]:���̍��� = ���̃|���S��y - ((�@��x * ����x) - (�@��z * ����z)) / �@��y\n");
		//CDebugproc::Print("[�����v�Z]:%.3f = %.3f - ((%.3f * %.3f) + (%.3f * %.3f)) / %.3f\n", fY, pVtx[0].pos.y, -CrossPolygon.x, VecA.x, -CrossPolygon.z, VecA.z, CrossPolygon.y);
		//CDebugproc::Print("[�v���C���[�̈ʒu]:(%.3f,%.3f,%.3f)\n", pos.x, pos.y, pos.z);
		//CDebugproc::Print("[0]:%.3f,[1]:%.3f,[2]:%.3f\n", m_pos.y + pVtx[0].pos.y, m_pos.y + pVtx[1].pos.y, m_pos.y + pVtx[2].pos.y);
		//CDebugproc::Print("[����]:%.3f\n", Dot);
		//CDebugproc::Print("[����]:%.3f\n", fY);
		//CDebugproc::Print("[�ʂ̖@��]:(%.3f,%.3f,%.3f)\n", CrossPolygon.x, CrossPolygon.y, CrossPolygon.z);
		//CDebugproc::Print("[�ʒu�̊O��]:(%.3f,%.3f,%.3f)\n", CrossAns.x, CrossAns.y, CrossAns.z);
		// ���_�f�[�^�͈̔͂��A�����b�N
		m_pVtxBuff->Unlock();
		return fY;
	}
	// ���_�f�[�^�͈̔͂��A�����b�N
	m_pVtxBuff->Unlock();
	return 0.0f;
}

// ----------------------------------------
// �ʒu�̐ݒ菈��
// ----------------------------------------
void CScene_THREE::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

// ----------------------------------------
// �T�C�Y�ݒ菈��
// ----------------------------------------
void CScene_THREE::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

// ----------------------------------------
// ��]�̐ݒ菈��
// ----------------------------------------
void CScene_THREE::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}
// ----------------------------------------
// �F�̐ݒ菈��
// ----------------------------------------
void CScene_THREE::SetCol(D3DXCOLOR col)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;	// ���_���

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// ���_�J���[
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	// �A�����b�N
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// �\���e�N�X�`���[�ݒ菈��
// ----------------------------------------
void CScene_THREE::SetTex(D3DXVECTOR2 first, D3DXVECTOR2 last)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;	// ���_���

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// ���_�e�N�X�`���[
	pVtx[0].tex = D3DXVECTOR2(first.x, first.y);
	pVtx[1].tex = D3DXVECTOR2(last.x, first.y);
	pVtx[2].tex = D3DXVECTOR2(first.x, last.y);
	pVtx[3].tex = D3DXVECTOR2(last.x, last.y);
	// �A�����b�N
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// �r���{�[�h��ԏ���
// ----------------------------------------
void CScene_THREE::SetBillboard(bool bBillboard)
{
	m_bBillboard = bBillboard;
}

// ----------------------------------------
// �T�C�Y�̎擾����
// ----------------------------------------
D3DXVECTOR3 CScene_THREE::GetSize(void)
{
	return m_size;
}

// ----------------------------------------
// �ʒu�̎擾����
// ----------------------------------------
D3DXVECTOR3 CScene_THREE::GetPos(void)
{
	return m_pos;
}

// ----------------------------------------
// ��]�擾
// ----------------------------------------
D3DXVECTOR3 CScene_THREE::GetRot(void)
{
	return m_rot;
}


// ----------------------------------------
// �e�N�X�`���[�ݒ菈��
// ----------------------------------------
void CScene_THREE::BindTexture(LPDIRECT3DTEXTURE9 p_Tex)
{
	m_pTexture = p_Tex;
}

// ----------------------------------------
//
// �O�Տ����̐���[meshobit.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "meshobit.h"
#include "game.h"
#include "debugproc.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define MESHOBIT_ROTMOVE (0.01f)		// �p�x�̈ړ���
#define MESHOBIT_SIZE (100.0f)			// �T�C�Y
#define MESHOBIT_ANGLE (D3DX_PI / 10)	// �p�Ȃ̊p�x

// ----------------------------------------
//
// �O���[�o���ϐ�
//
// ----------------------------------------

// ----------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------
LPDIRECT3DTEXTURE9 CMeshobit::m_pTex[TEX_MAX] = {};
CMeshobit::MESHOBIT CMeshobit::m_meshobit[MESHOBIT_MAX] = {};

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CMeshobit::CMeshobit() : CScene(ACTOR_MESHOBIT, LAYER_3DPARTICLE)
{
	m_tex = TEX_0;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CMeshobit::~CMeshobit()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CMeshobit::Init(void)
{
	// �O��1�Ԗڂ���2�Ԗڂ̕`��
	for (int nCntMesh = 0; nCntMesh < MESHOBIT_MAX; nCntMesh++)
	{
		// �ݒ�
		m_meshobit[nCntMesh].othermtx = NULL;					// �s��
		m_meshobit[nCntMesh].m_aOffset[0] = D3DVECTOR3_ZERO;	// �I�t�Z�b�g1
		m_meshobit[nCntMesh].m_aOffset[1] = D3DVECTOR3_ZERO;	// �I�t�Z�b�g2
		m_meshobit[nCntMesh].col = D3DXCOLOR_INI;				// �F
		m_meshobit[nCntMesh].type = CMeshobit::TYPE_0;			// �^�C�v
		m_meshobit[nCntMesh].fSize = 0.0f;						// �T�C�Y
		m_meshobit[nCntMesh].nMaxFrame = 0;						// �ő�t���[����
		m_meshobit[nCntMesh].bUse = false;						// �g�p���
	}
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CMeshobit::Uninit(void)
{
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CMeshobit::Update(void)
{
	for (int nCntMesh = 0; nCntMesh < MESHOBIT_MAX; nCntMesh++)
	{
		// �g�p���
		// �t���[�����J�E���g���ő�t���[��������
		if (m_meshobit[nCntMesh].bUse == true &&
			m_meshobit[nCntMesh].nCntFrame < m_meshobit[nCntMesh].nMaxFrame)
		{
			// �ϐ��錾
			VERTEX_3D * pVtx;
			D3DXVec3TransformCoord(
				&m_meshobit[nCntMesh].m_aPosVerTex[0],
				&m_meshobit[nCntMesh].m_aOffset[0],
				m_meshobit[nCntMesh].othermtx
			);
			// 1�Ԗڂ̒��_
			m_meshobit[nCntMesh].obit[0].pos[0] = m_meshobit[nCntMesh].m_aPosVerTex[0];
			D3DXVec3TransformCoord(
				&m_meshobit[nCntMesh].m_aPosVerTex[1],
				&m_meshobit[nCntMesh].m_aOffset[1],
				m_meshobit[nCntMesh].othermtx
			);
			// 2�Ԗڂ̒��_
			m_meshobit[nCntMesh].obit[0].pos[1] =				// �A
				m_meshobit[nCntMesh].m_aPosVerTex[1];
			for (int nCntObit = m_meshobit[nCntMesh].nLine - 1; nCntObit > 0; nCntObit--)
			{
				m_meshobit[nCntMesh].obit[nCntObit].pos[0] = m_meshobit[nCntMesh].obit[nCntObit - 1].pos[0];
				m_meshobit[nCntMesh].obit[nCntObit].pos[1] = m_meshobit[nCntMesh].obit[nCntObit - 1].pos[1];
			}
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			m_meshobit[nCntMesh].pVtxBuff->Lock(
				0,
				0,
				(void **)&pVtx,
				0);
			//���_�ݒ� //
			//�s���[�v
			for (int nCntMeshObit = 0; nCntMeshObit < m_meshobit[nCntMesh].nLine; nCntMeshObit++)
			{
				// ���_���W�̐ݒ�
				pVtx[0].pos = m_meshobit[nCntMesh].obit[nCntMeshObit].pos[0];
				// ���_���W�̐ݒ�
				pVtx[1].pos = m_meshobit[nCntMesh].obit[nCntMeshObit].pos[1];
				// �|�C���g���킹
				pVtx += 2;
			}
			// �A�����b�N
			m_meshobit[nCntMesh].pVtxBuff->Unlock();
			// �t���[�����J�E���g�A�b�v
			m_meshobit[nCntMesh].nCntFrame++;
		}
		// ����ȊO
		else
		{
			// �t���[�����J�E���g������
			m_meshobit[nCntMesh].nCntFrame = 0;
			m_meshobit[nCntMesh].othermtx = NULL;					// �s��
			m_meshobit[nCntMesh].m_aOffset[0] = D3DVECTOR3_ZERO;	// �I�t�Z�b�g1
			m_meshobit[nCntMesh].m_aOffset[1] = D3DVECTOR3_ZERO;	// �I�t�Z�b�g2
			m_meshobit[nCntMesh].col = D3DXCOLOR_INI;				// �F
			m_meshobit[nCntMesh].type = CMeshobit::TYPE_0;			// �^�C�v
			m_meshobit[nCntMesh].fSize = 0.0f;						// �T�C�Y
			m_meshobit[nCntMesh].bUse = false;
		}
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CMeshobit::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;
	// ���ʂ�`��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ���C�e�B���O���[�h����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Z�o�b�t�@�@����
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);	
	// Z�����@����
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	// �O�Օ`��
	for (int nCntMeshObit = 0; nCntMeshObit < MESHOBIT_MAX; nCntMeshObit++)
	{
		if (m_meshobit[nCntMeshObit].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&m_meshobit[nCntMeshObit].matrix);
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &m_meshobit[nCntMeshObit].matrix);
			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, m_meshobit[nCntMeshObit].pVtxBuff, 0, sizeof(VERTEX_3D));
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(
				0,
				m_pTex[m_tex]);
			// �O�Ղ̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,										// �`����n�߂钸�_
				m_meshobit[nCntMeshObit].nNumPolygon	// �|���S����
			);
		}
	}
	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// ���C�e�B���O���[�h�L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// ����(�����)�ŃJ�����O����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// Z�o�b�t�@�@�L��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	// Z�����@�L��
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}

//-------------------------------------------------------------------------------------------------------------
// �ݒ�
// 1�s��A2�I�t�Z�b�g�n�_�A3�I�t�Z�b�g�I�_�A4�F�A5�^�C�v�A6�T�C�Y�A7���̌��A8�t���[����
//-------------------------------------------------------------------------------------------------------------
void CMeshobit::Set(
	D3DXMATRIX	*pMtx,		// �}�g���b�N�X���
	D3DXVECTOR3	&offset1,	// �I�t�Z�b�g1
	D3DXVECTOR3 &offset2,	// �I�t�Z�b�g2
	D3DXCOLOR	&col,		// �F
	TYPE		type,		// �^�C�v
	TEX			tex,		// �e�N�X�`���[
	float		fSize,		// �T�C�Y���
	int			nLine,		// ��
	int			&nFrame		// �t���[����
)
{
	// �O��1�Ԗڂ���2�Ԗڂ̕`��
	for (int nCntMesh = 0; nCntMesh < MESHOBIT_MAX; nCntMesh++)
	{
		if (m_meshobit[nCntMesh].bUse == false)
		{
			// �ϐ��錾
			VERTEX_3D * pVtx;
			// �ݒ�
			m_meshobit[nCntMesh].othermtx = pMtx;			// �s��
			m_meshobit[nCntMesh].m_aOffset[0] = offset1;	// �I�t�Z�b�g1
			m_meshobit[nCntMesh].m_aOffset[1] = offset2;	// �I�t�Z�b�g2
			m_meshobit[nCntMesh].col = col;					// �F
			m_meshobit[nCntMesh].type = type;				// �^�C�v
			m_meshobit[nCntMesh].fSize = fSize;				// �T�C�Y
			m_meshobit[nCntMesh].nMaxFrame = nFrame;		// �ő�t���[����
			// ����𒴂��Ă�����
			if (nLine > OBITLINE_MAX)
			{
#ifdef _DEBUG
				CCalculation::Messanger("�O�Ղ̐��̏���������Ă��܂��B\n");
#endif // _DEBUG
				// ����ɋO�Ղ̐��̍ő吔����
				m_meshobit[nCntMesh].nLine = OBITLINE_MAX;
			}
			// ����ȊO
			else
			{
				m_meshobit[nCntMesh].nLine = nLine;	// ��
			}
			m_meshobit[nCntMesh].nNumPolygon =		// �|���S����
				m_meshobit[nCntMesh].nLine * 2 - 2;

			// �s�񂩂�̈ʒu�̒��_�Z�o
			D3DXVec3TransformCoord(
				&m_meshobit[nCntMesh].m_aPosVerTex[0],
				&m_meshobit[nCntMesh].m_aOffset[0],
				m_meshobit[nCntMesh].othermtx
			);
			// �s�񂩂�̈ʒu�̒��_�Z�o
			D3DXVec3TransformCoord(
				&m_meshobit[nCntMesh].m_aPosVerTex[1],
				&m_meshobit[nCntMesh].m_aOffset[1],
				m_meshobit[nCntMesh].othermtx
			);
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			m_meshobit[nCntMesh].pVtxBuff->Lock(
				0,
				0,
				(void **)&pVtx,
				0);
			//���_�ݒ� //
			//�s���[�v
			for (int nCntMeshObit = 0; nCntMeshObit < m_meshobit[nCntMesh].nLine; nCntMeshObit++)
			{
				// ���_����
				m_meshobit[nCntMesh].obit[nCntMeshObit].pos[0] = 
					m_meshobit[nCntMesh].m_aPosVerTex[0];
				// ���_����
				m_meshobit[nCntMesh].obit[nCntMeshObit].pos[1] =				// �A
					m_meshobit[nCntMesh].m_aPosVerTex[1];
				// ���_���W�̐ݒ�
				pVtx[0].pos = m_meshobit[nCntMesh].obit[0].pos[0];
				float fCol;
				fCol = (float)nCntMeshObit / m_meshobit[nCntMesh].nLine;
				// �J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, 0.0f - (float)nCntMeshObit / m_meshobit[nCntMesh].nLine);
				// �e�N�X�`���[�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntMeshObit / m_meshobit[nCntMesh].nLine, 0.0f);
				// �@���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
				// ���_���W�̐ݒ�
				pVtx[1].pos = m_meshobit[nCntMesh].obit[0].pos[1];
				// �J���[�̐ݒ�
				pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, 1.0f - (float)nCntMeshObit / m_meshobit[nCntMesh].nLine);
				// �e�N�X�`���[�̐ݒ�
				pVtx[1].tex = D3DXVECTOR2(1.0f * nCntMeshObit / m_meshobit[nCntMesh].nLine, 1.0f);
				// �@���̐ݒ�
				pVtx[1].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
				// �|�C���g���킹
				pVtx += 2;
			}

			// �A�����b�N
			m_meshobit[nCntMesh].pVtxBuff->Unlock();
			// �g�p���
			m_meshobit[nCntMesh].bUse = true;
			return;
		}
	}
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
//-------------------------------------------------------------------------------------------------------------
void CMeshobit::Debug(void)
{
	if (m_meshobit[1].othermtx != NULL)
	{
		CDebugproc::Print("�s��ʒu:(%.3f,%.3f,%.3f)\n",
			m_meshobit[1].othermtx->_41,
			m_meshobit[1].othermtx->_42,
			m_meshobit[1].othermtx->_43);
	}

	CDebugproc::Print("���_���Z�ʒu:(%.3f,%.3f,%.3f)\n",
		m_meshobit[1].m_aPosVerTex[0].x,
		m_meshobit[1].m_aPosVerTex[0].y,
		m_meshobit[1].m_aPosVerTex[0].z);


}
#endif // _DEBUG

// ----------------------------------------
// ���b�V������
// ----------------------------------------
HRESULT CMeshobit::LoadMesh(void)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	for (int nCntMesh = 0; nCntMesh < MESHOBIT_MAX; nCntMesh++)
	{
		// �ݒ�
		m_meshobit[nCntMesh].nNumberVertex =			// ���_
			OBITLINE_MAX * 2;
		m_meshobit[nCntMesh].nNumPolygon =				// �|���S��
			MESHOBIT_MAX * 2 - 2;
		// �ݒ�
		m_meshobit[nCntMesh].bUse = false;
		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(
			sizeof(VERTEX_3D) * m_meshobit[nCntMesh].nNumberVertex,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_meshobit[nCntMesh].pVtxBuff,
			NULL);
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
		m_meshobit[nCntMesh].pVtxBuff->Lock(
			0,
			0,
			(void **)&pVtx,
			0);
		//���_�ݒ� //
		//�s���[�v
		for (int nCntMeshObit = 0; nCntMeshObit < OBITLINE_MAX; nCntMeshObit++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// �J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			// �e�N�X�`���[�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCntMeshObit / OBITLINE_MAX, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// ���_���W�̐ݒ�
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// �J���[�̐ݒ�
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
			// �e�N�X�`���[�̐ݒ�
			pVtx[1].tex = D3DXVECTOR2(1.0f * nCntMeshObit / OBITLINE_MAX, 1.0f);
			// �@���̐ݒ�
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// �|�C���g���킹
			pVtx += 2;
		}
		// �A�����b�N
		m_meshobit[nCntMesh].pVtxBuff->Unlock();
	}
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CMeshobit::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice =
		CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���[�����
	char TexData[TEX_MAX][72] =
	{
		//{ "data/TEXTURE/trajectory.png" },
		//{ "data/TEXTURE/kiseki2.jpg" }
		{ "data/TEXTURE/meshobit_10.png" },
		{ "data/TEXTURE/meshobit_10.png" }
	};
	// ���炩���ߋO�Ղ̐���������
	LoadMesh();

	// �e�N�X�`���[�ݒ�
	for (int nCntTex = 0; nCntTex < TEX_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, TexData[nCntTex], &m_pTex[nCntTex]);
	}
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CMeshobit::UnLoad(void)
{
	// �e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < TEX_MAX; nCntTex++)
	{
		if (m_pTex[nCntTex] != NULL)
		{
			m_pTex[nCntTex]->Release();
			m_pTex[nCntTex] = NULL;
		}
	}
	// ���_�o�b�t�@�̊J��
	for (int nCntMesh = 0; nCntMesh < MESHOBIT_MAX; nCntMesh++)
	{
		if (m_meshobit[nCntMesh].pVtxBuff != NULL)
		{
			m_meshobit[nCntMesh].pVtxBuff->Release();
			m_meshobit[nCntMesh].pVtxBuff = NULL;
		}
	}
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CMeshobit * CMeshobit::Create(void)
{
	// �ϐ��錾
	CMeshobit * pMeshobit;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pMeshobit = new CMeshobit();
	// ����������
	pMeshobit->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pMeshobit;
}
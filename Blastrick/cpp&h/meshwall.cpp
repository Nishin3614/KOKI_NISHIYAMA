// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���b�V���E�H�[�������̐���[meshwall.cpp]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/* �`�� */
#include "meshwall.h"
#include "game.h"
#include "scene.h"
#include "debugproc.h"
#include "shadow_mapping.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MESHWALL_ROTMOVE (0.01f)		// �p�x�̈ړ���
#define MESHWALL_SIZE (100.0f)			// �T�C�Y
#define MESHWALL_PHONT "data/TEXTURE/woods.png"
#define MESHWALL_ANGLE (D3DX_PI / 10)	// �p�Ȃ̊p�x

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CMeshwall::m_pTex = NULL;			// �e�N�X�`���[
vector<ARRANGEMENTMESH>	CMeshwall::m_pArrangemesh;		// �z�u�����

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CMeshwall::CMeshwall() : CScene(ACTOR_MESHWALL, LAYER_3DOBJECT)
{
	/* �ϐ��̏����� */
	// ��]��
	m_pVtxBuff = NULL;
	m_pIndex = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNumberVertex = 4;
	m_nNumIndex = 4;
	m_nNumPolygon = 2;
	m_nBlock_Width = 1;
	m_nBlock_Depth = 1;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CMeshwall::~CMeshwall()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMeshwall::Init(void)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;						// ���_���ւ̃|�C���g
	WORD * pIdx;							// �C���f�b�N�X�f�[�^�ւ̃|�C���^
	D3DXVECTOR3 VecA, VecB;					// �x�N�g��
	int nCountDirect;						// �c�̃J�E���g
	int nCountWidth;						// ���̃J�E���g
	int nCntBlock = 0;						// �u���b�N�J�E���g
	float fYAngle;							// y�̊p�x
	D3DXVECTOR3 *pCross;					// �|���S���̊O��
	// ���擾
	LPDIRECT3DDEVICE9 pDevice =				// �f�o�C�X
		CManager::GetRenderer()->GetDevice();
	pCross =								// �������m��
		new D3DXVECTOR3[m_nBlock_Width * m_nBlock_Depth * 2];

	// �u���b�N�`��̌��_�̏����ݒ�
	m_OriginBlock = D3DXVECTOR3(
		m_size.x * -0.5f * m_nBlock_Width,
		m_size.y * m_nBlock_Depth,
		0.0f);
	// �����_���E�C���f�b�N�X�E�|���S���̏����ݒ�(�v�Z)
	m_nNumberVertex =
		(m_nBlock_Depth + 1) * (m_nBlock_Width + 1);
	m_nNumIndex =
		(m_nBlock_Width + 1) * 2 * m_nBlock_Depth +
		2 * (m_nBlock_Depth - 1);
	m_nNumPolygon =
		m_nBlock_Depth * m_nBlock_Width * 2 +
		4 * (m_nBlock_Depth - 1);

	// �p�x�̌v�Z
	fYAngle = MESHWALL_ANGLE;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumberVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) *
		m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIndex,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	//���_�ݒ� //
	//�s���[�v
	for (nCountDirect = 0; nCountDirect < m_nBlock_Depth + 1; nCountDirect++)
	{
		// �񃋁[�v
		for (nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{

			// ���_���W�̐ݒ�
			pVtx[0].pos =
				D3DXVECTOR3(
					m_OriginBlock.x +
					(m_size.x * nCountWidth),
					m_OriginBlock.y - 
					(m_size.y * nCountDirect),
					m_OriginBlock.z
				);

			// �J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`���[�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

			// �|�C���g���킹
			pVtx++;
		}
	}
	// ���_���̃A�h���X�������ɖ߂�
	pVtx -= m_nNumberVertex;
	// �|���S�����Ƃ̖@���̐ݒ�
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth; nCntDepth++, nCntBlock++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width; nCntWidth++, nCntBlock++)
		{
			// �x�N�g��
			VecA = pVtx[nCntBlock + m_nBlock_Width + 2].pos - pVtx[nCntBlock].pos;
			VecB = pVtx[nCntBlock + m_nBlock_Width + 1].pos - pVtx[nCntBlock].pos;
			// �O�όv�Z
			pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] = CCalculation::Cross_product(VecA, VecB);
			// ���K��
			D3DXVec3Normalize(&pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2], &pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2]);
			// �x�N�g��
			VecA = pVtx[nCntBlock + 1].pos - pVtx[nCntBlock].pos;
			VecB = pVtx[nCntBlock + m_nBlock_Width + 2].pos - pVtx[nCntBlock].pos;
			// �O�όv�Z
			pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1] = CCalculation::Cross_product(VecA, VecB);
			// ���K��
			D3DXVec3Normalize(&pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1], &pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1]);
			// ����
			pVtx[nCntBlock].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] + pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
			// �E��
			pVtx[nCntBlock + 1].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
			// ����
			pVtx[nCntBlock + m_nBlock_Width + 1].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2];
			// �E��
			pVtx[nCntBlock + m_nBlock_Width + 2].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] + pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
		}
	}
	nCntBlock = 0;
	// ���_�@���̐ݒ�
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth + 1; nCntDepth++, nCntBlock++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width + 1; nCntWidth++, nCntBlock++)
		{
			// �ŏ�
			if (nCntDepth == 0 && nCntWidth == 0)
			{
				pVtx[0].nor /= 2;
			}
			// �Ō�
			else if (nCntDepth == m_nBlock_Depth && nCntWidth == m_nBlock_Width)
			{
				pVtx[0].nor /= 2;
			}
			// 1�s�̗�̍Ō�
			else if (nCntDepth == 0 && nCntWidth == m_nBlock_Width)
			{
			}
			// �Ō�s�̗�̍ŏ�
			else if (nCntDepth == m_nBlock_Depth && nCntWidth == 0)
			{
			}
			// �ŏ��̍s�܂��͍Ō�̍s
			else if (nCntDepth == 0 || nCntDepth == m_nBlock_Depth)
			{
				pVtx[0].nor /= 3;
			}
			// �ŏ��̗�܂��͍Ō�̗�
			else if (nCntWidth == 0 || nCntWidth == m_nBlock_Width)
			{
				pVtx[0].nor /= 3;
			}
			// ����ȊO
			else
			{
				pVtx[0].nor /= 6;
			}
			pVtx++;
		}
	}

	// �A�����b�N
	m_pVtxBuff->Unlock();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pIndex->Lock(0, 0, (void **)&pIdx, 0);


	// �c�u���b�N��
	for (nCountDirect = 0; nCountDirect < m_nBlock_Depth; nCountDirect++)
	{
		// �Q��ڂ̃��[�v�ȍ~
		if (nCountDirect >= 1)
		{
			// �k�ރ|���S�����̒��_�ǉ�
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + m_nBlock_Width + 1;

			// �C���f�b�N�X�̃|�C���g���킹
			pIdx++;
		}

		// ���u���b�N�̒��_��
		for (nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// �`�揇�Ԃ̃C���f�b�N�X
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + nCountWidth + m_nBlock_Width + 1;
			pIdx[1] = nCountDirect * (m_nBlock_Width + 1) + nCountWidth;

			// �C���f�b�N�X�̃|�C���g���킹
			pIdx += 2;
		}

		// �k�ރ|���S�������K�v������ꍇ
		if (nCountDirect < m_nBlock_Depth - 1)
		{
			// �k�ރ|���S�����̒��_�ǉ�
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + m_nBlock_Width;
			// �C���f�b�N�X�̃|�C���g���킹
			pIdx++;
		}
	}

	// �A�����b�N
	m_pIndex->Unlock();
	delete[] pCross;
	pCross = NULL;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMeshwall::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMeshwall::Update(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMeshwall::Draw(void)
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
	D3DXMatrixRotationYawPitchRoll(&mtxRot,	// �����̓��ꕨ
		m_rot.y, m_rot.x, m_rot.z);			// ���ꂼ��̉�]��
	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,					// 2
		&mtxRot);							// 3
	// ���s�ړ��s��쐬(�I�t�Z�b�g)
	D3DXMatrixTranslation(&mtxTrans,// �����̓��ꕨ
		m_pos.x, m_pos.y, m_pos.z);	// ���ꂼ��̈ړ���
	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));


	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[����ݒ�
	pDevice->SetIndices(
		m_pIndex);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(
		0,
		m_pTex);
	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nNumberVertex,
		0,
		m_nNumPolygon);
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMeshwall::Debug(void)
{
}
#endif // _DEBUG

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��ݏ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CMeshwall::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice =
		CManager::GetRenderer()->GetDevice();
	// �e�L�X�g�f�[�^�̓ǂݍ���
	LoadText();
	// �e�N�X�`���[�ݒ�
	D3DXCreateTextureFromFile(pDevice, MESHWALL_PHONT, &m_pTex);
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾏���j��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMeshwall::UnLoad(void)
{
	// �e�N�X�`���̊J��
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
	vector<ARRANGEMENTMESH>().swap(m_pArrangemesh);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CMeshwall * CMeshwall::Create(
	D3DXVECTOR3 const &pos,		// �ʒu
	D3DXVECTOR3 const &size,	// �T�C�Y
	D3DXVECTOR3 const &rot,		// ��]
	int const &nWidth,			// ���u���b�N
	int const &nDepth,			// �c�u���b�N
	int const &nType			// �^�C�v
)
{
	// �ϐ��錾
	CMeshwall * pMeshwall;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pMeshwall = new CMeshwall();
	// �ʒu���
	pMeshwall->m_pos = pos;
	// �T�C�Y���
	pMeshwall->m_size = size;
	// ��]���
	pMeshwall->m_rot = rot;
	// ���u���b�N
	pMeshwall->m_nBlock_Width = nWidth;
	// �c�u���b�N
	pMeshwall->m_nBlock_Depth = nDepth;
	// ����������
	pMeshwall->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pMeshwall;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂ݂��񂾃e�L�X�g���𐶐�
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CMeshwall::LoadCreate(void)
{
	for (int nCntStaticobj = 0; nCntStaticobj < (signed)m_pArrangemesh.size(); nCntStaticobj++)
	{
		CMeshwall::Create(
			m_pArrangemesh.at(nCntStaticobj).pos,
			D3DXVECTOR3(m_pArrangemesh.at(nCntStaticobj).fSizeA, m_pArrangemesh.at(nCntStaticobj).fSizeB,0.0f),
			m_pArrangemesh.at(nCntStaticobj).rot,
			m_pArrangemesh.at(nCntStaticobj).nBlockWidth,
			m_pArrangemesh.at(nCntStaticobj).nBlockDepth,
			m_pArrangemesh.at(nCntStaticobj).nType
		);
	}
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ڂ��Ă���ʂ̍������擾
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CMeshwall::GetHeight(D3DXVECTOR3 pos)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;					// ���_���ւ̃|�C���g
	D3DXVECTOR3 VecA, VecB,VecObject;	// �x�N�g��
	D3DXVECTOR3 Normal;					// �@��
	float fLength = 0;					// ����
	int nCntBlock = 0;					// �u���b�N��
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// �u���b�N���Ƃ�
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width; nCntWidth++, nCntBlock++)
		{
			// �E��̃|���S��
			if (CCalculation::CrossCollision(&pos, &pVtx[0].pos, &pVtx[1].pos, &pVtx[m_nBlock_Width + 2].pos))
			{
				// �x�N�g��
				VecA = pVtx[m_nBlock_Width + 2].pos - pVtx[1].pos;	// �x�N�g��B
				VecB = pVtx[0].pos - pVtx[1].pos;					// �x�N�g��A
				VecObject = pos - (m_pos + pVtx[1].pos);			// �I�u�W�F�N�g�܂ł̃x�N�g��
				// �����̑��
				fLength = pVtx[1].pos.y;
				// �O��
				D3DXVec3Cross(&Normal, &VecA, &VecB);
				// ���K��
				D3DXVec3Normalize(&Normal, &Normal);
#ifdef _DEBUG
				//// �u���b�N�ԍ�
				//CDebugproc::Print("�u���b�N�ԍ�[%d]\n", nCntBlock);
				//// �|���S���ԍ�
				//CDebugproc::Print("�|���S���ԍ�[%d]\n", nCntBlock * 2 + 1);
				//// �@��
				//CDebugproc::Print("�@��[%.3f,%.3f,%.3f]\n", Normal.x, Normal.y, Normal.z);
#endif // _DEBUG
				// ���_�f�[�^�͈̔͂��A�����b�N
				m_pVtxBuff->Unlock();
				// ������Ԃ�
				return fLength + ((-Normal.x * VecObject.x) + (-Normal.z * VecObject.z)) / Normal.y;
			}
			// �����̃|���S��
			else if (CCalculation::CrossCollision(&pos, &pVtx[m_nBlock_Width + 2].pos, &pVtx[m_nBlock_Width + 1].pos, &pVtx[0].pos))
			{
				// �x�N�g��
				VecA = pVtx[0].pos - pVtx[m_nBlock_Width + 1].pos;						// �x�N�g��A
				VecB = pVtx[m_nBlock_Width + 2].pos - pVtx[m_nBlock_Width + 1].pos;		// �x�N�g��B
				VecObject = pos - (m_pos + pVtx[m_nBlock_Width + 1].pos);				// �I�u�W�F�N�g�܂ł̃x�N�g��
				// �����̑��
				fLength = pVtx[m_nBlock_Width + 1].pos.y;
				// �O��
				D3DXVec3Cross(&Normal, &VecA, &VecB);
				// ���K��
				D3DXVec3Normalize(&Normal, &Normal);
#ifdef _DEBUG
				//// �u���b�N�ԍ�
				//CDebugproc::Print("�u���b�N�ԍ�[%d]\n", nCntBlock);
				//// �|���S���ԍ�
				//CDebugproc::Print("�|���S���ԍ�[%d]\n", nCntBlock * 2);
				//// �@��
				//CDebugproc::Print("�@��[%.3f,%.3f,%.3f]\n", Normal.x,Normal.y,Normal.z);
#endif // _DEBUG
				// ���_�f�[�^�͈̔͂��A�����b�N
				m_pVtxBuff->Unlock();
				// ������Ԃ�
				return fLength + ((-Normal.x * VecObject.x) + (-Normal.z * VecObject.z)) / Normal.y;
			}
			// ���_���̍X�V
			pVtx++;
		}
		// ���_���̍X�V
		pVtx++;

	}

	// ���_�f�[�^�͈̔͂��A�����b�N
	m_pVtxBuff->Unlock();
	// ������Ԃ�
	return 0.0f;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �e�L�X�g�f�[�^�̓ǂݍ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CMeshwall::LoadText(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntError = 0;				// �J�E���g���[�h
	char cRaedText[128];			// �����Ƃ��ēǂݎ��p
	char cHeadText[128];			// ��r����悤
	char cDie[128];					// �s�K�v�ȕ���
	ARRANGEMENTMESH arrangemesh;	// �z�u�����

	// �t�@�C�����J����Ă��Ȃ�������
	if ((pFile = fopen("data/LOAD/MAPPING/wall.txt", "r")) == NULL)
	{// ���b�Z�[�W�̕\��
#ifdef _DEBUG
		CCalculation::Messanger("�e�L�X�g�t�@�C����������܂���ł���");
#endif // _DEBUG
		return E_FAIL;
	}
	// �X�N���v�g�����݂��Ă��邩�`�F�b�N
	if (!CCalculation::Existenceofscript(pFile))
	{
#ifdef _DEBUG
		CCalculation::Messanger("[SCRIP]��������܂���\n�I�����Ă�������");
#endif // _DEBUG
		fclose(pFile);
		return E_FAIL;
	}
	// �G���h�X�N���v�g������܂Ń��[�v
	while (strcmp(cHeadText, "END_SCRIPT") != 0)
	{
		fgets(cRaedText, sizeof(cRaedText), pFile);
		sscanf(cRaedText, "%s", &cHeadText);

		// ���s��������
		if (strcmp(cHeadText, "\n") == 0)
		{
		}

		// ���f���Z�b�g��������
		else if (strcmp(cHeadText, "WALLSET") == 0)
		{

			// �G���h���f���Z�b�g������܂Ń��[�v
			while (strcmp(cHeadText, "END_WALLSET") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);

				// ��ޏ��ǂݍ���
				if (strcmp(cHeadText, "TYPE") == 0)
				{
					sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &arrangemesh.nType);
				}

				// �ʒu���ǂݍ���
				else if (strcmp(cHeadText, "POS") == 0)
				{
					sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &arrangemesh.pos.x, &arrangemesh.pos.y, &arrangemesh.pos.z);
				}

				// ��]���ǂݍ���
				else if (strcmp(cHeadText, "ROT") == 0)
				{
					sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &arrangemesh.rot.x, &arrangemesh.rot.y, &arrangemesh.rot.z);
				}

				// �c�u���b�N���ǂݍ���
				else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
				{
					sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &arrangemesh.nBlockDepth);
				}

				// ���u���b�N���ǂݍ���
				else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
				{
					sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &arrangemesh.nBlockWidth);
				}

				// x�T�C�Y���ǂݍ���
				else if (strcmp(cHeadText, "XSIZE") == 0)
				{
					sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &arrangemesh.fSizeA);
				}

				// y�T�C�Y���ǂݍ���
				else if (strcmp(cHeadText, "YSIZE") == 0)
				{
					sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &arrangemesh.fSizeB);
				}
				// �G���[�J�E���g���C���N�������g
				nCntError++;
				if (nCntError > FILELINE_ERROW)
				{// �G���[
					nCntError = 0;
					fclose(pFile);
					CCalculation::Messanger("�G���h�E�H�[���Z�b�g������܂���");
					return E_FAIL;
				}
			}
			// �ǂ݂��񂾏��ǉ�
			m_pArrangemesh.push_back(arrangemesh);
		}
		// �G���[�J�E���g���C���N�������g
		nCntError++;
		if (nCntError > FILELINE_ERROW)
		{// �G���[
			nCntError = 0;
			fclose(pFile);
			CCalculation::Messanger("�G���h�X�N���v�g������܂���");
			return E_FAIL;
		}
	}
	fclose(pFile);
	return S_OK;
}

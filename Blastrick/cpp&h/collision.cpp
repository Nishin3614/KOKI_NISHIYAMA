// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Collision�����̐���[calculation.cpp]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Collision.h"
#include "3Dline.h"

#ifdef _DEBUG
#include "debugproc.h"
#include "debugcollision.h"
#endif // _DEBUG

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define COLLISION_FILE "data/LOAD/collision.txt"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ�
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CCollision::m_nAll = 0;						// ����
int CCollision::m_nNumLoad = 0;					// ���[�h��
CCollision::INFO CCollision::m_Info[128] = {};	// ���

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCollision::CCollision() : CScene::CScene(ACTOR_COLLISION,LAYER_COLLISION)
{
	// ������
	m_ID = m_nAll;								// �ԍ�
	m_objtype = OBJTYPE_ACTOR;						// �^�C�v
	m_ppos = NULL;								// �ʒu���
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��̈ʒu���
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y���
	m_pmove = NULL;								// �ړ����
	m_nAll++;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCollision::~CCollision()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Init(void)
{
	m_bUse = true;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Uninit(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Update(void)
{
	if (m_objtype == OBJTYPE_ACTOR)	Collision();
	m_posOld = *m_ppos;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Draw(void)
{
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Debug(void)
{
}
#endif // _DEBUG

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��������
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCollision * CCollision::Create(void)
{
	// �ϐ��錾
	CCollision * pCollision;	// �����蔻��
	// �������m��
	pCollision = new CCollision;
	// ������
	pCollision->Init();
	// �Ԃ�
	return pCollision;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾂��̂𐶐�
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::LoadCreate(void)
{
	// �ϐ��錾
	CCollision * pCollision;	// �����蔻��
	for (int nCntLoad = 0; nCntLoad < m_nNumLoad; nCntLoad++)
	{
		pCollision = CCollision::Create();
		pCollision->SetPos(&m_Info[nCntLoad].pos);
		pCollision->SetSize(m_Info[nCntLoad].size);
		pCollision->SetMove(NULL);
		pCollision->SetType(CCollision::OBJTYPE_WAIT);
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ʒu�ݒ菈��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::SetPos(D3DXVECTOR3 * pos)
{
	m_ppos = pos;
	m_posOld = *pos;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �T�C�Y�ݒ菈��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::SetSize(D3DXVECTOR3 const size)
{

	m_size = size;
#ifdef _DEBUG
	CDebugcollision::Create(m_ppos, &m_size, CDebugcollision::COLLISIONTYPE_BOX);
#endif // _DEBUG

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ړ��ݒ菈��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::SetMove(D3DXVECTOR3 * move)
{
	m_pmove = move;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �^�C�v�ݒ菈��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::SetType(OBJTYPE const type)
{
	m_objtype = type;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �g�p��ԏ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::SetUse(bool const bUse)
{
	m_bUse = bUse;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���^�̓����蔻�菈��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::Collision_Come(void)
{
	// �g�p��Ԃł͂Ȃ���ԂȂ�֐��𔲂���
	if (!m_bUse)
	{
		return false;
	}
	// �ϐ��錾
	CCollision * pCollision = NULL;
	bool bOn = false;			// ��ɂ��邩�ǂ���
	// �����蔻�蓯�m�̔�r
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_COLLISION); nCntLayer++)
	{
		// ���擾
		pCollision = (CCollision*)CScene::GetActor(CScene::ACTOR_COLLISION, CScene::LAYER_COLLISION, nCntLayer);	// �����蔻��
		// �����𖞂����Ă��邩
		if (pCollision == NULL ||	// �k���`�F�b�N
			pCollision == this)		// �������
		{							 
			continue;
		}

		// �f�ނ�Y�͈�
		if (this->m_ppos->y + this->m_size.y * 0.5f > pCollision->m_ppos->y - pCollision->m_size.y * 0.5f&&
			this->m_ppos->y - this->m_size.y * 0.5f < pCollision->m_ppos->y + pCollision->m_size.y * 0.5f)
		{
			// �f�ނ�Z�͈�
			if (this->m_ppos->z + this->m_size.z * 0.5f > pCollision->m_ppos->z - pCollision->m_size.z * 0.5f&&
				this->m_ppos->z - this->m_size.z * 0.5f < pCollision->m_ppos->z + pCollision->m_size.z * 0.5f)
			{
				// �����蔻��(��)
				if (this->m_ppos->x + this->m_size.x * 0.5f > pCollision->m_ppos->x - pCollision->m_size.x * 0.5f&&
					this->m_posOld.x + this->m_size.x * 0.5f <= pCollision->m_ppos->x - pCollision->m_size.x * 0.5f)
				{
					// �f�ޏ�̍���
					this->m_ppos->x = pCollision->m_ppos->x - pCollision->m_size.x * 0.5f - this->m_size.x * 0.5f;
				}

				// �����蔻��(�E)
				else if (this->m_ppos->x - this->m_size.x * 0.5f < pCollision->m_ppos->x + pCollision->m_size.x * 0.5f&&
					m_posOld.x - this->m_size.x * 0.5f >= pCollision->m_ppos->x + pCollision->m_size.x * 0.5f)
				{
					// �f�ޏ�̍���
					this->m_ppos->x = pCollision->m_ppos->x + pCollision->m_size.x * 0.5f + this->m_size.x * 0.5f;
				}
			}

			// �f�ނ�X�͈�
			if (this->m_ppos->x + this->m_size.x * 0.5f > pCollision->m_ppos->x - pCollision->m_size.x * 0.5f&&
				this->m_ppos->x - this->m_size.x * 0.5f < pCollision->m_ppos->x + pCollision->m_size.x * 0.5f)
			{
				// �����蔻��(��O)
				if (this->m_ppos->z + this->m_size.z * 0.5f > pCollision->m_ppos->z - pCollision->m_size.z * 0.5f&&
					m_posOld.z + this->m_size.z * 0.5f <= pCollision->m_ppos->z - pCollision->m_size.z * 0.5f)
				{
					// �f�ޏ�̍���
					this->m_ppos->z = pCollision->m_ppos->z - pCollision->m_size.z * 0.5f - this->m_size.z * 0.5f;
				}

				// �����蔻��(��)
				else if (this->m_ppos->z - this->m_size.z * 0.5f < pCollision->m_ppos->z + pCollision->m_size.z * 0.5f&&
					m_posOld.z - this->m_size.z * 0.5f >= pCollision->m_ppos->z + pCollision->m_size.z * 0.5f)
				{
					// �f�ޏ�̍���
					this->m_ppos->z = pCollision->m_ppos->z +
						pCollision->m_size.z * 0.5f +
						this->m_size.z * 0.5f + 0.1f;
				}
			}
		}

		// �f�ނ�Z�͈�
		if (this->m_ppos->z + this->m_size.z * 0.5f > pCollision->m_ppos->z - pCollision->m_size.z * 0.5f&&
			this->m_ppos->z - this->m_size.z * 0.5f < pCollision->m_ppos->z + pCollision->m_size.z * 0.5f)
		{
			// �f�ނ�X�͈�
			if (this->m_ppos->x + this->m_size.x * 0.5f > pCollision->m_ppos->x - pCollision->m_size.x * 0.5f&&
				this->m_ppos->x - this->m_size.x * 0.5f < pCollision->m_ppos->x + pCollision->m_size.x * 0.5f)
			{
				// �����蔻��(��)
				if (this->m_ppos->y + this->m_size.y * 0.5f > pCollision->m_ppos->y - pCollision->m_size.y * 0.5f&&
					m_posOld.y + this->m_size.y * 0.5f <= pCollision->m_ppos->y - pCollision->m_size.y * 0.5f)
				{
					// �f�ޏ�̍���
					this->m_ppos->y = this->m_posOld.y;
					// �ړ��ʂ̏�����
					this->m_pmove->y = 0.0f;
				}

				// �����蔻��(��)
				else if (this->m_ppos->y - this->m_size.y * 0.5f < pCollision->m_ppos->y + pCollision->m_size.y * 0.5f&&
					m_posOld.y - this->m_size.y * 0.5f >= pCollision->m_ppos->y + pCollision->m_size.y * 0.5f)
				{
					// �f�ޏ�̍���
					this->m_ppos->y = pCollision->m_ppos->y + pCollision->m_size.y * 0.5f + this->m_size.y * 0.5f;
					// �ړ��ʂ̏�����
					this->m_pmove->y = 0.0f;
					bOn = true;
				}
			}
		}
	}
	return bOn;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���^�̓����蔻�菈��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Collision(void)
{
	// �g�p��Ԃł͂Ȃ���ԂȂ�֐��𔲂���
	if (!m_bUse)
	{
		return;
	}
	// �ϐ��錾
	CCollision * pCollision = NULL;
	// �����蔻�蓯�m�̔�r
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_COLLISION); nCntLayer++)
	{
		// ��ڂ̓����蔻��
		pCollision = (CCollision*)CScene::GetActor(CScene::ACTOR_COLLISION, CScene::LAYER_COLLISION, nCntLayer);	// �����蔻��
																													// �����𖞂����Ă��邩
		if (pCollision == NULL ||	// �k���`�F�b�N
			pCollision == this)		// �������
		{
			continue;
		}

		// �f�ނ�Y�͈�
		if (this->m_ppos->y + this->m_size.y * 0.5f > pCollision->m_ppos->y - pCollision->m_size.y * 0.5f&&
			this->m_ppos->y - this->m_size.y * 0.5f < pCollision->m_ppos->y + pCollision->m_size.y * 0.5f)
		{
			// �f�ނ�Z�͈�
			if (this->m_ppos->z + this->m_size.z * 0.5f > pCollision->m_ppos->z - pCollision->m_size.z * 0.5f&&
				this->m_ppos->z - this->m_size.z * 0.5f < pCollision->m_ppos->z + pCollision->m_size.z * 0.5f)
			{
				// �����蔻��(��)
				if (this->m_ppos->x + this->m_size.x * 0.5f > pCollision->m_ppos->x - pCollision->m_size.x * 0.5f&&
					this->m_posOld.x + this->m_size.x * 0.5f <= pCollision->m_ppos->x - pCollision->m_size.x * 0.5f)
				{
					// �f�ޏ�̍���
					this->m_ppos->x = pCollision->m_ppos->x - pCollision->m_size.x * 0.5f - this->m_size.x * 0.5f;
				}

				// �����蔻��(�E)
				else if (this->m_ppos->x - this->m_size.x * 0.5f < pCollision->m_ppos->x + pCollision->m_size.x * 0.5f&&
					m_posOld.x - this->m_size.x * 0.5f >= pCollision->m_ppos->x + pCollision->m_size.x * 0.5f)
				{
					// �f�ޏ�̍���
					this->m_ppos->x = pCollision->m_ppos->x + pCollision->m_size.x * 0.5f + this->m_size.x * 0.5f;
				}
			}

			// �f�ނ�X�͈�
			if (this->m_ppos->x + this->m_size.x * 0.5f > pCollision->m_ppos->x - pCollision->m_size.x * 0.5f&&
				this->m_ppos->x - this->m_size.x * 0.5f < pCollision->m_ppos->x + pCollision->m_size.x * 0.5f)
			{
				// �����蔻��(��O)
				if (this->m_ppos->z + this->m_size.z * 0.5f > pCollision->m_ppos->z - pCollision->m_size.z * 0.5f&&
					m_posOld.z + this->m_size.z * 0.5f <= pCollision->m_ppos->z - pCollision->m_size.z * 0.5f)
				{
					// �f�ޏ�̍���
					this->m_ppos->z = pCollision->m_ppos->z - pCollision->m_size.z * 0.5f - this->m_size.z * 0.5f;
				}

				// �����蔻��(��)
				else if (this->m_ppos->z - this->m_size.z * 0.5f < pCollision->m_ppos->z + pCollision->m_size.z * 0.5f&&
					m_posOld.z - this->m_size.z * 0.5f >= pCollision->m_ppos->z + pCollision->m_size.z * 0.5f)
				{
					// �f�ޏ�̍���
					this->m_ppos->z = pCollision->m_ppos->z +
						pCollision->m_size.z * 0.5f +
						this->m_size.z * 0.5f + 0.1f;
				}
			}
		}

		// �f�ނ�Z�͈�
		if (this->m_ppos->z + this->m_size.z * 0.5f > pCollision->m_ppos->z - pCollision->m_size.z * 0.5f&&
			this->m_ppos->z - this->m_size.z * 0.5f < pCollision->m_ppos->z + pCollision->m_size.z * 0.5f)
		{
			// �f�ނ�X�͈�
			if (this->m_ppos->x + this->m_size.x * 0.5f > pCollision->m_ppos->x - pCollision->m_size.x * 0.5f&&
				this->m_ppos->x - this->m_size.x * 0.5f < pCollision->m_ppos->x + pCollision->m_size.x * 0.5f)
			{
				// �����蔻��(��)
				if (this->m_ppos->y + this->m_size.y * 0.5f > pCollision->m_ppos->y - pCollision->m_size.y * 0.5f&&
					m_posOld.y + this->m_size.y * 0.5f <= pCollision->m_ppos->y - pCollision->m_size.y * 0.5f)
				{
					// �f�ޏ�̍���
					this->m_ppos->y = this->m_posOld.y;
					// �ړ��ʂ̏�����
					this->m_pmove->y = 0.0f;
				}

				// �����蔻��(��)
				else if (this->m_ppos->y - this->m_size.y * 0.5f < pCollision->m_ppos->y + pCollision->m_size.y * 0.5f&&
					m_posOld.y - this->m_size.y * 0.5f >= pCollision->m_ppos->y + pCollision->m_size.y * 0.5f)
				{
					// �f�ޏ�̍���
					this->m_ppos->y = pCollision->m_ppos->y + pCollision->m_size.y * 0.5f + this->m_size.y * 0.5f;
					// �ړ��ʂ̏�����
					this->m_pmove->y = 0.0f;
				}
			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��ݏ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CCollision::Load(void)
{
	// �e�L�X�g�f�[�^�̓ǂݍ���
	m_nNumLoad = TextLoad();
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CCollision::TextLoad(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntColli = 0;		// ���f���J�E���g
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

							// �t�@�C���J
	pFile = fopen(COLLISION_FILE, "r");

	// �J����
	if (pFile != NULL)
	{
		// �X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// �ꕶ��ǂݍ���
			sscanf(cRaedText, "%s", &cHeadText);		// ��r�p�e�N�X�g�ɕ�������
		}

		// �X�N���v�g��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// �G���h�X�N���v�g������܂Ń��[�v
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				// ������
				cHeadText[0] = '\0';
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);
				// �R���W�����Z�b�g������
				if (strcmp(cHeadText, "COLLISION") == 0)
				{
					// �G���h�R���W�����Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_COLLISION") != 0)
					{
						// ������
						cHeadText[0] = '\0';
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);
						// �e���ǂݍ���
						if (strcmp(cHeadText, "OBJTYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&m_Info[nCntColli].type);
						}
						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&m_Info[nCntColli].pos.x,
								&m_Info[nCntColli].pos.y,
								&m_Info[nCntColli].pos.z);
						}
						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&m_Info[nCntColli].size.x,
								&m_Info[nCntColli].size.y,
								&m_Info[nCntColli].size.z);
						}
					}
					// ���f���̍X�V
					nCntColli++;
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "�����蔻��ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}

	return nCntColli;
}

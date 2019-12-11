#//*************************************************************************************************************
//
// �Q�[��UI����[GameUI.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "GameUI.h"
#include "scene_two.h"

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
CGameUI::TEXTURE_INFO	*CGameUI::m_pTextureinfo = NULL;	// �e�N�X�`���̏��
int						CGameUI::m_nNumTextureMax = 0;		// �e�N�X�`���ő吔

//-------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------------------------------------------------
CGameUI::CGameUI():CScene(ACTOR_TITLE_UI, LAYER_UI)
{
}

//-------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------------------------------------------------
CGameUI::~CGameUI()
{
}

//-------------------------------------------------------------------------------------------------------------
// �ǂݍ���
//-------------------------------------------------------------------------------------------------------------
HRESULT CGameUI::Load(void)
{

	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice;			// �f�o�C�X�̃|�C���^
	const char			*sFileName;			// �G�t�F�N�g���̃e�L�X�g�t�@�C����
	PFILETAG			pTextFileName;		// �e�N�X�`���t�@�C����
	// �ϐ��̏�����
	pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�̎擾
	sFileName = "data/TEXT/TextureInfo.txt";

	// �e�N�X�`�������擾
	m_nNumTextureMax = CGameUI::LoadNumber(sFileName, "NUMBER");

	// �e�N�X�`���t�@�C�����̃������m��
	pTextFileName = new FILETAG[m_nNumTextureMax];
	// �e�N�X�`�����̃������m��
	m_pTextureinfo = new CGameUI::TEXTURE_INFO[m_nNumTextureMax];

	// �e�N�X�`���t�@�C�����̎擾
	CGameUI::LoadTextureFile(sFileName, pTextFileName, m_nNumTextureMax);

	for (int nCntTexture = 0; nCntTexture < m_nNumTextureMax; nCntTexture++)
	{
		// �e�N�X�`���̓ǂݍ���
		if (D3DXCreateTextureFromFile(pDevice, pTextFileName[nCntTexture].string, &m_pTextureinfo[nCntTexture].pTexture) != D3D_OK)
		{
			MessageBox(NULL, "���ڂ�������܂���\n�e�N�X�`���̓ǂݍ��ݎ��s", "�x���I", MB_ICONWARNING);
			m_pTextureinfo[nCntTexture].pTexture = NULL;
		}
	}

	return E_FAIL;


	delete[m_nNumTextureMax] pTextFileName;
	return S_OK;

}

//-------------------------------------------------------------------------------------------------------------
// �J��
//-------------------------------------------------------------------------------------------------------------
void CGameUI::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < m_nNumTextureMax; nCntTexture++)
	{// �e�N�X�`�����̊J��
		if (m_pTextureinfo != NULL)
		{
			if (m_pTextureinfo[nCntTexture].pTexture != NULL)
			{
				if (m_pTextureinfo[nCntTexture].pTexture != nullptr)
				{
					m_pTextureinfo[nCntTexture].pTexture->Release();
					m_pTextureinfo[nCntTexture].pTexture = NULL;
				}
			}
		}
	}
	// �e�N�X�`�����̃|�C���^�̊J��
	delete[m_nNumTextureMax]m_pTextureinfo;
	m_pTextureinfo = NULL;
}

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CGameUI * CGameUI::Create(void)
{
	CGameUI *pGameUI = new CGameUI;
	pGameUI->Init();
	return pGameUI;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CGameUI::Init(void)
{
	// UI���̓ǂݍ���
	CGameUI::LoadUI();
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CGameUI::Uninit(void)
{
	delete[m_nNumUI]m_pCScene2D;
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CGameUI::Update(void)
{
	for (int nCntGameUI = 0; nCntGameUI < m_nNumUI; nCntGameUI++)
	{

		m_pCScene2D[nCntGameUI].Update();
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CGameUI::Draw(void)
{
	for (int nCntGameUI = 0; nCntGameUI < m_nNumUI; nCntGameUI++)
	{
		m_pCScene2D[nCntGameUI].Draw();
	}
}

//-------------------------------------------------------------------------------------------------------------
// ���̓ǂݍ���
//-------------------------------------------------------------------------------------------------------------
int CGameUI::LoadNumber(const char * TextFile, const char * SetItem)
{
	// �ϐ��錾
	FILE *pFile = NULL;				// �t�@�C���̃|�C���^
	char cRead[128];				// �ǂݍ��ݗp
	char cComp[128];				// ��r�p
	char cEmpty[128];				// �v��Ȃ����̗p
	int nNumber = -1;				// ��
	int nCntError = 0;				// �G���[�p

									// �t�@�C�����J����Ă��Ȃ�������
	if ((pFile = fopen(TextFile, "r")) == NULL)
	{// ���b�Z�[�W�̕\��
#ifdef _DEBUG
		MessageBox(NULL, "�e�L�X�g�t�@�C����������܂���ł���", "�x���I", MB_ICONWARNING);
		return -1;
#endif // _DEBUG
	}
	// �X�N���v�g�����݂��Ă��邩�`�F�b�N
	if (!CGameUI::Existenceofscript(pFile))
	{
#ifdef _DEBUG
		MessageBox(NULL, "[SCRIP]��������܂���\n�I�����Ă�������", "�x���I", MB_ICONWARNING);
		fclose(pFile);
		return -1;
#endif // _DEBUG
	}
	// END_SCRIPT������܂�
	while (strcmp(cComp, "END_SCRIPT") != 0)
	{// END_SCRIPT�܂Ń��[�v
	 // 1�s�ǂݍ���
		fgets(cRead, sizeof(cRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(cRead, "%s", &cComp);

		// �G���[�J�E���g���C���N�������g
		nCntError++;
		if (nCntError > 1048576)
		{// �G���[
			nCntError = 0;
#ifdef _DEBUG
			MessageBox(NULL, "[END_SCRIPT]��������܂���\n�I�����Ă�������", "�x���I", MB_ICONWARNING);
			fclose(pFile);
			return -1;
#endif // _DEBUG

		}
		if (strcmp(cComp, SetItem) == 0)
		{
			sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &nNumber);
			cComp[0] = '\0';
		}
	}
	if (nNumber == -1)
	{
#ifdef _DEBUG
		MessageBox(NULL, "���ڂ�������܂���", "�x���I", MB_ICONWARNING);
		fclose(pFile);
		return -1;
#endif // _DEBUG
	}
	// �t�@�C�������
	fclose(pFile);

	return nNumber;
}

//-------------------------------------------------------------------------------------------------------------
// �e�N�X�`���̓ǂݍ���
//-------------------------------------------------------------------------------------------------------------
void CGameUI::LoadTextureFile(const char * sFileName, PFILETAG TexName, const int & nNumMax)
{
	// �ϐ��錾
	FILE *pFile = NULL;				// �t�@�C���̃|�C���^
	char cRead[128];				// �ǂݍ��ݗp
	char cComp[128];				// ��r�p
	char cEmpty[128];				// �v��Ȃ����̗p
	int nNumCount = 0;				// ���̃J�E���g
	int nCntError = 0;				// �G���[�p

									// �t�@�C�����J����Ă��Ȃ�������
	if ((pFile = fopen(sFileName, "r")) == NULL)
	{// ���b�Z�[�W�̕\��
#ifdef _DEBUG
		MessageBox(NULL, "�e�L�X�g�t�@�C����������܂���ł���", "�x���I", MB_ICONWARNING);
		return;
#endif // _DEBUG
	}
	// �X�N���v�g�����݂��Ă��邩�`�F�b�N
	if (!CGameUI::Existenceofscript(pFile))
	{
#ifdef _DEBUG
		MessageBox(NULL, "[SCRIP]��������܂���\n�I�����Ă�������", "�x���I", MB_ICONWARNING);
		fclose(pFile);
		return;
#endif // _DEBUG
	}

	// END_SCRIPT������܂�
	while (strcmp(cComp, "END_SCRIPT") != 0)
	{// END_SCRIPT�܂Ń��[�v
	 // 1�s�ǂݍ���
		fgets(cRead, sizeof(cRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(cRead, "%s", &cComp);

		// �G���[�J�E���g���C���N�������g
		nCntError++;
		if (nCntError > 1048576)
		{// �G���[
			nCntError = 0;
#ifdef _DEBUG
			MessageBox(NULL, "[END_SCRIPT]��������܂���\n�I�����Ă�������", "�x���I", MB_ICONWARNING);
			fclose(pFile);
			return;
#endif // _DEBUG

		}
		if (strcmp(cComp, "FILENAME") == 0)
		{
			if (nNumCount >= nNumMax)
			{
				MessageBox(NULL, "���f�����������Ă��܂���", "�x���I", MB_ICONWARNING);
				fclose(pFile);
				return;
			}
			sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &TexName[nNumCount].string);
			nNumCount++;
			cComp[0] = '\0';
		}
	}
	if (nNumCount < nNumMax)
	{
#ifdef _DEBUG
		MessageBox(NULL, "���ڂ�������܂���", "�x���I", MB_ICONWARNING);
		fclose(pFile);
		return;
#endif // _DEBUG
	}
	// �t�@�C�������
	fclose(pFile);

}

//-------------------------------------------------------------------------------------------------------------
// SCRIPT�����݂��邩
//-------------------------------------------------------------------------------------------------------------
bool CGameUI::Existenceofscript(FILE * pFile)
{
	// �ϐ��錾
	char cRead[128];	// �ǂݍ��ݗp
	char cComp[128];	// ��r�p
	int nCntError = 0;	// �G���[�p
	while (strcmp(cComp, "SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(cRead, sizeof(cRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(cRead, "%s", &cComp);

		// �G���[�J�E���g���C���N�������g
		nCntError++;
		if (nCntError > 1048576)
		{// �G���[
			nCntError = 0;
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------------------------
// UI���̓ǂݍ���
//-------------------------------------------------------------------------------------------------------------
void CGameUI::LoadUI(void)
{
	// �ϐ��錾
	FILE	*pFile = NULL;				// �t�@�C���̃|�C���^
	char	cRead[128];					// �ǂݍ��ݗp
	char	cComp[128];					// ��r�p
	char	cEmpty[128];				// �v��Ȃ����̗p
	int		nCntError = 0;				// �G���[���b�Z�[�W�p


	// UI�����擾
	m_nNumUI = CGameUI::LoadNumber("data/TEXT/UI_Info.txt", "NUMBER");

	if (m_nNumUI == -1)
	{
		return;
	}

	m_pCScene2D = new CScene_TWO[m_nNumUI];

	// �t�@�C�����J��
	if ((pFile = fopen("data/TEXT/UI_Info.txt", "r")) == NULL)
	{
		return;
	}
	// �X�N���v�g�̗L���̊m�F
	if (!CGameUI::Existenceofscript(pFile))
	{
		return;
	}

	// �ϐ��錾
	int nCntScene2D = 0;		// �V�[��2D�̃J�E���g

	// END_SCRIPT������܂�
	while (strcmp(cComp, "END_SCRIPT") != 0)
	{// END_SCRIPT�܂Ń��[�v
	 // 1�s�ǂݍ���
		fgets(cRead, sizeof(cRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(cRead, "%s", &cComp);

		// �G���[�J�E���g���C���N�������g
		nCntError++;
		if (nCntError > 1048576)
		{// �G���[
			nCntError = 0;
#ifdef _DEBUG
			MessageBox(NULL, "[END_SCRIPT]��������܂���\n�I�����Ă�������", "�x���I", MB_ICONWARNING);
			fclose(pFile);
			return;
#endif // _DEBUG

		}
		if (strcmp(cComp, "SET_UI") == 0)
		{
			// ����
			m_pCScene2D[nCntScene2D].Init();

			while (strcmp(cComp, "}") != 0)
			{// END_SCRIPT�܂Ń��[�v
			 // 1�s�ǂݍ���
				fgets(cRead, sizeof(cRead), pFile);
				// �ǂݍ���Ǖ�������
				sscanf(cRead, "%s", &cComp);

				if (strcmp(cComp, "POS") == 0)
				{
					D3DXVECTOR3 pos;
					sscanf(cRead, "%s %s %f %f %f",
						&cEmpty,
						&cEmpty,
						&pos.x,
						&pos.y,
						&pos.z);
					cComp[0] = '\0';
					m_pCScene2D[nCntScene2D].SetPosition(pos);

				}
				else if (strcmp(cComp, "SIZE") == 0)
				{
					D3DXVECTOR2 size;
					sscanf(cRead, "%s %s %f %f",
						&cEmpty,
						&cEmpty,
						&size.x,
						&size.y);
					cComp[0] = '\0';
					m_pCScene2D[nCntScene2D].SetSize(size*2.0f);
				}

				else if (strcmp(cComp, "ROTATION") == 0)
				{
					float fRot;
					sscanf(cRead, "%s %s %f",
						&cEmpty,
						&cEmpty,
						&fRot);
					cComp[0] = '\0';
					m_pCScene2D[nCntScene2D].SetRot(fRot);

				}
				else if (strcmp(cComp, "COL") == 0)
				{
					D3DXCOLOR col;
					sscanf(cRead, "%s %s %f %f %f %f",
						&cEmpty,
						&cEmpty,
						&col.r,
						&col.g,
						&col.b,
						&col.a);
					cComp[0] = '\0';
					m_pCScene2D[nCntScene2D].SetCol(col);

				}
				else if (strcmp(cComp, "TEXTURETYPE") == 0)
				{
					int nType;
					sscanf(cRead, "%s %s %d",
						&cEmpty,
						&cEmpty,
						&nType);
					cComp[0] = '\0';
					m_pCScene2D[nCntScene2D].BindTexture(m_pTextureinfo[nType].pTexture);

				}
			}
			// �V�[��2D�̃J�E���g��i�߂�
			nCntScene2D++;
		}
	}

}

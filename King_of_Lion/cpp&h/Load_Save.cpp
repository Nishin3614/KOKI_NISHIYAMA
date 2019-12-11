// ------------------------------------------------------------------------------------------
// �ǂݍ��݁E�����o������ [Load_Save.cpp]
// Author : Koki Nishiyama
// ------------------------------------------------------------------------------------------
#include "Load_Save.h"			// �ǂݍ��݁E�����o���w�b�_�[

#include "input.h"

#include "joypad.h"

#include "fade.h"

#include "sound.h"

#include "score.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define	TEXTURE_LOAD_SAVE	"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define LOAD_SAVE_SIZE (30)									// �ǂݍ��݁E�����o���{�[�h�̑傫��
#define LOAD_SAVE_BETWEEN (30)								// �Ԋu
#define POINT_MAX (3)										// ���l
#define LOAD_SAVE_DATE (5)									// �ǂݍ��݁E�����o����
#define SAVE_FILE_NAME ("text.txt")							// �Z�[�u�t�@�C��

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ��錾
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureLoad_Save = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLoad_Save = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3				g_Load_SavePos;					// �ǂݍ��݁E�����o���{�[�h�̃|�W�V����
LOAD_SAVE				g_aLoad_Save[LOAD_SAVE_DATE];	// �ǂݍ��݁E�����o���ϐ�
int g_nLoadScore = 0;									// ���[�h�X�R�A

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitLoad_Save(void)
{
	// �ϐ��錾
	int nCntRank;										// �J�E���g�ǂݍ��݁E�����o��

	// ������
	g_nLoadScore = 0;
	for (nCntRank = 0; nCntRank < LOAD_SAVE_DATE; nCntRank++)
	{
		g_aLoad_Save[nCntRank].nPoint = 0;
	}
	
	// ���[�h�ǂݍ��݁E�����o��
	LoadDate();
}

void SaveData(int nPoint)
{
	// �t�@�C���|�C���g
	FILE *pFile;
	int aDate[6] = {};
	int nDate;
	int nCntLoad_Save;
	int nCntNum;
	int  nCntLoad = 0;
	char cWriteText[128];			// �����Ƃ��ď������ݗp
	char cHeadText[128];			// ����p
	char cEqual[8] = { "=" };		// �C�R�[���p
	char cEnter[8] = { "\n" };	// ���s

								// ����ւ���O�̓ǂݍ��݁E�����o���|�C���g����
	for (nCntLoad_Save = 0; nCntLoad_Save < LOAD_SAVE_DATE; nCntLoad_Save++)
	{
		aDate[nCntLoad_Save] = g_aLoad_Save[nCntLoad_Save].nPoint;
	}

	// ����Ƃ����|�C���g��
	aDate[5] = nPoint;

	// ���ӂ̓ǂݍ��݁E�����o�����[�v
	for (nCntLoad_Save = 0; nCntLoad_Save < LOAD_SAVE_DATE + 1; nCntLoad_Save++)
	{
		// �E�ӂ̓ǂݍ��݁E�����o�����[�v
		for (nCntNum = nCntLoad_Save + 1; nCntNum < LOAD_SAVE_DATE + 1; nCntNum++)
		{
			// �ǂݍ��݁E�����o���|�C���g���r
			if (aDate[nCntLoad_Save] < aDate[nCntNum])
			{
				nDate = aDate[nCntNum];
				aDate[nCntNum] = aDate[nCntLoad_Save];
				aDate[nCntLoad_Save] = nDate;
			}
		}
	}

	// ����ւ�����̓ǂݍ��݁E�����o���|�C���g����
	for (nCntLoad_Save = 0; nCntLoad_Save < LOAD_SAVE_DATE; nCntLoad_Save++)
	{
		g_aLoad_Save[nCntLoad_Save].nPoint = aDate[nCntLoad_Save];
	}

	// �J�E���g�Z�[�u�̏�����
	nCntLoad_Save = 0;

	// �t�@�C���J
	pFile = fopen("data/SAVE/ranking.txt", "w");

	// �J����
	if (pFile != NULL)
	{
		strcpy(cWriteText, "SCRIPT");
		fputs(cWriteText, pFile);

		// �X�N���v�g���ʂ���
		if (strcmp(cWriteText, "SCRIPT") == 0)
		{
			fputs(cEnter, pFile);

			// �G���h�X�N���v�g������܂Ń��[�v
			while (strcmp(cWriteText, "END_SCRIPT") != 0)
			{

				strcpy(cHeadText, "rank");
				if (strcmp(cHeadText, "rank") == 0)
				{
					sprintf(cWriteText, "	%s %s %d", &cHeadText, &cEqual, g_aLoad_Save[nCntLoad_Save].nPoint);
					fputs(cWriteText, pFile);

					fputs(cEnter, pFile);

					nCntLoad_Save++;
				}

				if (nCntLoad_Save >= 5)
				{
					strcpy(cHeadText, "now");
					if (strcmp(cHeadText, "now") == 0)
					{

						sprintf(cWriteText, "	%s %s %d", &cHeadText, &cEqual, aDate[5]);
						fputs(cWriteText, pFile);

						fputs(cEnter, pFile);

					}

					strcpy(cWriteText, "END_SCRIPT");
					fputs(cWriteText, pFile);
				}

			}
		}
		// �t�@�C����
		fclose(pFile);
	}
	// �J���Ȃ�
	else
	{
		printf("�J����܂���ł���\n");
	}
}

void LoadDate(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntLoad_Save = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r�p
	char cDie[128];			// �S�~�p

							// �t�@�C���J
	pFile = fopen("data/SAVE/ranking.txt", "r");

	// �J����
	if (pFile != NULL)
	{
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);
			sscanf(cRaedText, "%s", &cHeadText);
		}

		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);
				if (strcmp(cHeadText, "rank") == 0)
				{
					sscanf(cRaedText, "%s %s %d",
						&cDie, &cDie,
						&g_aLoad_Save[nCntLoad_Save].nPoint);
					nCntLoad_Save++;
				}

				else if (strcmp(cHeadText, "now") == 0)
				{
					sscanf(cRaedText, "%s %s %d",
						&cDie, &cDie,
						&g_nLoadScore);
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		printf("�J����܂���ł���\n");
	}
}

LOAD_SAVE * GetLoad_Save()
{
	return &g_aLoad_Save[0];
}
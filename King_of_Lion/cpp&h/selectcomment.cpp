// ------------------------------------------------------------------------------------------
//
// �I���R�����g�����̐���[selectcoment.cpp]
// Author : Koki Nishiyama 
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "title.h"

#include "input.h"

#include "joypad.h"

#include "fade.h"

#include "sound.h"

#include "selectcomment.h"

#include "arrow.h"

#include "game.h"

#include "tutorial.h"

#include "range.h"

#include "gamefade.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define SELECTCOMMENT_TEX (SELECTCOMMENTTYPE_MAX + SELECTCOMMENTSCENE_AREA)	// �e�N�X�`���[�̐�

#define SELECTUI_SIZE_X (300)												// �I���R�����g���T�C�Y�i���j
#define SELECTUI_SIZE_Y (300)												// �I���R�����g���T�C�Y�i���j

#define SELECTCOMMENTUI_SIZE_X (1200)										// �I���R�����g���T�C�Y�i���j
#define SELECTCOMMENTUI_SIZE_Y (300)										// �I���R�����g���T�C�Y�i���j

#define UI_SIZE_X (200)														// �G���^�[���T�C�Y�i���j
#define UI_SIZE_Y (200)														// �G���^�[���T�C�Y�i���j

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void Update_SelectSelectComment(void);
void Cnt_SelectComment(int nCnt);
void Update_ColChangeSelectComment(void);
void Update_VertexSelectComment(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureSelectComment[SELECTCOMMENT_TEX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelectComment = NULL;												// �e�N�X�`���ւ̃|�C���^
SELECTCOMENT g_aSelectComment[SELECTCOMMENTTYPE_MAX];												// �I���R�����g
int g_nCntScoment = 0;
int g_SelectCommentScene = 0;																		// �V�[��

// ------------------------------------------------------------------------------------------
// �I���R�����g����������
// ------------------------------------------------------------------------------------------
void InitSelectComment(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	VERTEX_2D *pVtx;							// ���_���̔Ԓn

	int nCntSelectComment;

	// �e�N�X�`���[�t�@�C������`
	char *paTextureFile[SELECTCOMMENT_TEX] =
	{
		{ "data/TEXTURE/00_Yes.png" },					// �͂�
		{ "data/TEXTURE/01_No.png" },					// ������
		{ "data/TEXTURE/TUTORIALS/02_Tutorial.png" },	// �`���[�g���A��
		{ "data/TEXTURE/CONTINUE/03_continue.png" },	// �R���e�B�j���[
		{ "data/TEXTURE/AREA/04_area.png" },			// �G���A�ړ�
	};

	// ������
	for (nCntSelectComment = 0; nCntSelectComment < SELECTCOMMENTTYPE_MAX; nCntSelectComment++)
	{
		// �I���R�����g�̈ʒu
		if (nCntSelectComment == SELECTCOMMENTTYPE_YES)
		{
			g_aSelectComment[nCntSelectComment].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.25f, 500.0f, 0.0f);
		}

		// �Q�[���R�����g�̈ʒu
		else if(nCntSelectComment == SELECTCOMMENTTYPE_NO)
		{
			g_aSelectComment[nCntSelectComment].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, 500.0f, 0.0f);
		}
		// �^�C�g���R�����g�̈ʒu
		else if (nCntSelectComment == SELECTCOMMENTTYPE_COME)
		{
			g_aSelectComment[nCntSelectComment].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 200.0f, 0.0f);
		}

		g_aSelectComment[nCntSelectComment].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSelectComment[nCntSelectComment].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aSelectComment[nCntSelectComment].nType = SELECTCOMMENTTYPE_COME;
		g_aSelectComment[nCntSelectComment].nCntCol = 0;
		g_aSelectComment[nCntSelectComment].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCntSelectTex = 0;
		nCntSelectTex < SELECTCOMMENT_TEX;
		nCntSelectTex++)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			paTextureFile[nCntSelectTex],
			&g_pTextureSelectComment[nCntSelectTex]);
	}

	// �I���R�����g�J�E���g�̏�����
	g_nCntScoment = SELECTCOMMENTTYPE_NO;
	g_SelectCommentScene = 0;				// �V�[��

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * SELECTCOMMENTTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffSelectComment,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffSelectComment->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntSelectComment = 0; nCntSelectComment < SELECTCOMMENTTYPE_MAX; nCntSelectComment++)
	{
		if (nCntSelectComment == 2)
		{
			// ���_�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x - SELECTCOMMENTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y - SELECTCOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// ��ڂ̒��_

			pVtx[1].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x + SELECTCOMMENTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y - SELECTCOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// ��ڂ̒��_

			pVtx[2].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x - SELECTCOMMENTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y + SELECTCOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// �O�ڂ̒��_

			pVtx[3].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x + SELECTCOMMENTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y + SELECTCOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// �l�ڂ̒��_
		}

		else
		{
			// ���_�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x - SELECTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y - SELECTUI_SIZE_Y * 0.5f,
				0.0f);	// ��ڂ̒��_

			pVtx[1].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x + SELECTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y - SELECTUI_SIZE_Y * 0.5f,
				0.0f);	// ��ڂ̒��_

			pVtx[2].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x - SELECTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y + SELECTUI_SIZE_Y * 0.5f,
				0.0f);	// �O�ڂ̒��_

			pVtx[3].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x + SELECTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y + SELECTUI_SIZE_Y * 0.5f,
				0.0f);	// �l�ڂ̒��_
		}
		
		// ���_�쐬
		pVtx[0].rhw = 1.0f;									// ��ڂ̒��_�쐬
		pVtx[1].rhw = 1.0f;									// ��ڂ̒��_�쐬
		pVtx[2].rhw = 1.0f;									// �O�ڂ̒��_�쐬
		pVtx[3].rhw = 1.0f;									// �l�ڂ̒��_�쐬

		// �J���[�ݒ�
		pVtx[0].col = g_aSelectComment[nCntSelectComment].col;	// ��ڂ̐F�ݒ�
		pVtx[1].col = g_aSelectComment[nCntSelectComment].col;	// ��ڂ̐F�ݒ�
		pVtx[2].col = g_aSelectComment[nCntSelectComment].col;	// �O�ڂ̐F�ݒ�
		pVtx[3].col = g_aSelectComment[nCntSelectComment].col;	// �l�ڂ̐F�ݒ�

		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// �l�ڂ̃e�N�X�`��

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffSelectComment->Unlock();
}

// ------------------------------------------------------------------------------------------
// �I���R�����g�I������
// ------------------------------------------------------------------------------------------
void UninitSelectComment(void)
{
	int nCntSelectComment;

	// �e�N�X�`���̊J��
	for (nCntSelectComment = 0; nCntSelectComment < SELECTCOMMENTTYPE_MAX; nCntSelectComment++)
	{
		if (g_pTextureSelectComment[nCntSelectComment] != NULL)
		{
			g_pTextureSelectComment[nCntSelectComment]->Release();
			g_pTextureSelectComment[nCntSelectComment] = NULL;
		}
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffSelectComment != NULL)
	{
		g_pVtxBuffSelectComment->Release();
		g_pVtxBuffSelectComment = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �I���R�����g�X�V����
// ------------------------------------------------------------------------------------------

void UpdateSelectComment(void)
{
	// �I������
	Update_SelectSelectComment();

	// �J���[�̕ω�����
	Update_ColChangeSelectComment();

	// ���_��񏈗�
	Update_VertexSelectComment();
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void Update_SelectSelectComment(void)
{
	// �X�e�B�b�N�ł̑���
	static int nCntStickLeft = 0;
	static int nCntStickRight = 0;

	// �W���C�p�b�h�p //
	int nValueH, nValueV;	// �Q�[���p�b�h�̃X�e�B�b�N���̎擾�p

	// �Q�[���p�b�h�̃X�e�B�b�N�����擾
	GetGamepadStickLeft(0, &nValueH, &nValueV);

	// ���x�̌v�Z
	if (nValueH < 0)
	{
		nCntStickLeft++;
	}
	// ���x�̌v�Z
	else if (nValueH > 0)
	{
		nCntStickRight++;
	}
	// ������Ă��Ȃ��Ƃ��͏�����
	else
	{
		nCntStickLeft = 0;
		nCntStickRight = 0;
	}
	// �K��l���������珉����
	if (nCntStickLeft == 20 || nCntStickRight == 20)
	{
		nCntStickLeft = 0;
		nCntStickRight = 0;
	}

	// �J�E���g����
	if (GetKeyboardTrigger(DIK_D) ||
		GetJoyboardTrigger(0, JOYPADKEY_RIGHT) ||
		nCntStickRight == 1)
	{
		// �I��
		Cnt_SelectComment(1);
	}
	else if (GetKeyboardTrigger(DIK_A) ||
		GetJoyboardTrigger(0, JOYPADKEY_LEFT) ||
		nCntStickLeft == 1)
	{
		// �I��
		Cnt_SelectComment(-1);
	}

	if (GetKeyboardTrigger(DIK_RETURN) ||
		GetKeyboardTrigger(DIK_J) ||
		GetJoyboardTrigger(0, JOYPADKEY_B))
	{
		// �͂��I����
		if (g_nCntScoment == SELECTCOMMENTTYPE_YES)
		{
			// ���艹
			PlaySound(SOUND_LABEL_SE_ENTER);

			// �`���[�g���A���̏ꍇ
			if (g_SelectCommentScene == SELECTCOMMENTSCENE_TUTORIALS)
			{
				SetFade(MODE_GAME);
			}

			// �R���e�B�j���[�̏ꍇ
			else if (g_SelectCommentScene == SELECTCOMMENTSCENE_CONTINUE)
			{
				SetFade(MODE_GAME);
			}

			// �G���A�ړ��̏ꍇ
			else if (g_SelectCommentScene == SELECTCOMMENTSCENE_AREA)
			{
				SetGameFade(GAMESTATE_BOSS);
			}
		}

		// �������I����
		else if (g_nCntScoment == SELECTCOMMENTTYPE_NO ||
			GetJoyboardTrigger(0, JOYPADKEY_A))
		{
			// ���艹
			PlaySound(SOUND_LABEL_SE_ENTER);

			// �`���[�g���A���̏ꍇ
			if (g_SelectCommentScene == SELECTCOMMENTSCENE_TUTORIALS)
			{
				SetTutorialState(TUTORIALSSTATE_NORMAL);
				PlayerPos_Range();
			}

			// �R���e�B�j���[�̏ꍇ
			else if (g_SelectCommentScene == SELECTCOMMENTSCENE_CONTINUE)
			{
				SetFade(MODE_TITLE);
			}

			// �G���A�ړ��̏ꍇ
			else if (g_SelectCommentScene == SELECTCOMMENTSCENE_AREA)
			{
				SetGameState(GAMESTATE_NORMAL);
				PlayerPos_Range();
			}
		}
	}

	// ���̈ʒu���X�V
	ArrowPosition(g_aSelectComment[g_nCntScoment].pos);
}

// ------------------------------------------------------------------------------------------
// �J�E���g�ω�����
// ------------------------------------------------------------------------------------------
void Cnt_SelectComment(int nCnt)
{
	// ���艹
	PlaySound(SOUND_LABEL_SE_ENTER);

	// �O��̃�����ʏ��
	g_aSelectComment[g_nCntScoment].col.a = 1.0f;

	// �O��̃J���[�J�E���g��ʏ��
	g_aSelectComment[g_nCntScoment].nCntCol = 0;

	// ���_��񏈗�
	Update_VertexSelectComment();

	// �J�E���g�_�E��
	g_nCntScoment += nCnt;

	// �J�E���g�����E�_�����������
	if (g_nCntScoment < SELECTCOMMENTTYPE_YES)
	{
		g_nCntScoment = SELECTCOMMENTTYPE_COME - 1;
	}

	// �J�E���g�����E�_�𒴂�����
	else if (g_nCntScoment >= SELECTCOMMENTTYPE_COME)
	{
		g_nCntScoment = SELECTCOMMENTTYPE_YES;
	}
}

// ------------------------------------------------------------------------------------------
// �J���[�ω�����
// ------------------------------------------------------------------------------------------
void Update_ColChangeSelectComment(void)
{
	// �J�E���g�A�b�v
	g_aSelectComment[g_nCntScoment].nCntCol++;

	if (g_aSelectComment[g_nCntScoment].nCntCol % 20 == 0)
	{
		// �J���[�ݒ�
		g_aSelectComment[g_nCntScoment].col.a = 1.0f;	// ��ڂ̐F�ݒ�
	}

	else if (g_aSelectComment[g_nCntScoment].nCntCol % 10 == 0)
	{
		// �J���[�ݒ�
		g_aSelectComment[g_nCntScoment].col.a = 0.0f;	// ��ڂ̐F�ݒ�
	}
}

// ------------------------------------------------------------------------------------------
// ���_��񏈗�
// ------------------------------------------------------------------------------------------
void Update_VertexSelectComment(void)
{
	VERTEX_2D *pVtx;			// ���_���

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffSelectComment->Lock(0, 0, (void **)&pVtx, 0);

	// �|�C���g���킹
	pVtx += 4 * g_nCntScoment;

	// �J���[�ݒ�
	pVtx[0].col = g_aSelectComment[g_nCntScoment].col;	// ��ڂ̐F�ݒ�
	pVtx[1].col = g_aSelectComment[g_nCntScoment].col;	// ��ڂ̐F�ݒ�
	pVtx[2].col = g_aSelectComment[g_nCntScoment].col;	// �O�ڂ̐F�ݒ�
	pVtx[3].col = g_aSelectComment[g_nCntScoment].col;	// �l�ڂ̐F�ݒ�

	// �A�����b�N
	g_pVtxBuffSelectComment->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �I���R�����g�`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawSelectComment(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	int nCntSelectComment;

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffSelectComment, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	for (nCntSelectComment = 0; nCntSelectComment < SELECTCOMMENTTYPE_MAX; nCntSelectComment++)
	{
		// �R�����g
		if (nCntSelectComment == SELECTCOMMENTTYPE_COME)
		{
			pDevice->SetTexture(0, g_pTextureSelectComment[g_aSelectComment[nCntSelectComment].nType]);
		}

		// �͂��E������
		else
		{
			pDevice->SetTexture(0, g_pTextureSelectComment[nCntSelectComment]);
		}

		// �I���R�����g�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSelectComment * 4, 2);
	}
}

// ------------------------------------------------------------------------------------------
// �I���R�����g�̃V�[������
// ------------------------------------------------------------------------------------------
void SceneSelectComment(int nScene)
{
	g_SelectCommentScene = nScene;
	g_aSelectComment[SELECTCOMMENTTYPE_COME].nType = SELECTCOMMENTTYPE_COME;
	g_aSelectComment[SELECTCOMMENTTYPE_COME].nType += nScene;
}

// ------------------------------------------------------------------------------------------
// �I���R�����g�̔ԍ��擾����
// ------------------------------------------------------------------------------------------
int GetSelectCommentSelect(void)
{
	return g_nCntScoment;
}

// ------------------------------------------------------------------------------------------
//
// �I���R�����g�擾����
//
// ------------------------------------------------------------------------------------------
SELECTCOMENT *GetSelectComment(void)
{
	return &g_aSelectComment[0];
}
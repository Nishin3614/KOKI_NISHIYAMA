// ----------------------------------------
//
// �f�o�b�O�\�������̐���[debugproc.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "debugproc.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------

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
LPD3DXFONT g_pDebugFont;
char g_aDebugStr[2048];

// ----------------------------------------
// ����������
// ----------------------------------------
void InitDebug(LPDIRECT3DDEVICE9 m_pDevice)
{
	// �f�o�b�O�\���p�t�H���g��ݒ�
	D3DXCreateFont(m_pDevice, 24, 10, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "�l�r �S�V�b�N", &g_pDebugFont);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void UninitDebug(void)
{
	g_pDebugFont->Release();
}

// ----------------------------------------
// �f�o�b�O�\������
// ----------------------------------------
void PrintDebug(char * fmt, ...)
{
	// �ϐ��錾
	va_list va;	// �ϕϐ�
	int nCntString;

	nCntString = strlen(g_aDebugStr);
	// �ψ�����1�ɂ܂Ƃ߂�
	va_start(va, fmt);
	// �܂Ƃ߂ĕ\��
	vsprintf(&g_aDebugStr[nCntString], fmt, va);
	va_end(va);
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void DrawDebug(void)
{
	// �ϐ��錾
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// �\���ꏊ�E�͈�
	// �e�L�X�g�`��
	g_pDebugFont->DrawText(NULL, g_aDebugStr, -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	// �e�L�X�g�̏�����
	g_aDebugStr[0] = '\0';
}


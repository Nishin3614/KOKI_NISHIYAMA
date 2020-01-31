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
LPD3DXFONT CDebugproc::m_pFont = NULL;
char CDebugproc::m_aStr[STRING_MAX] = {};

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CDebugproc::CDebugproc()
{
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CDebugproc::~CDebugproc()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CDebugproc::Init(LPDIRECT3DDEVICE9 m_pDevice)
{
#ifdef _DEBUG


	// �f�o�b�O�\���p�t�H���g��ݒ�
	D3DXCreateFont(m_pDevice, 24, 10, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "�l�r �S�V�b�N", &m_pFont);
#endif // _DEBUG
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CDebugproc::Uninit(void)
{
}

// ----------------------------------------
// �f�o�b�O�\������
// ----------------------------------------
void CDebugproc::Print(char * fmt, ...)
{
#ifdef _DEBUG
	// �ϐ��錾
	va_list va;	// �ϕϐ�
	int nCntString;

	nCntString = strlen(m_aStr);
	// �ψ�����1�ɂ܂Ƃ߂�
	va_start(va, fmt);
	// �܂Ƃ߂ĕ\��
	vsprintf(&m_aStr[nCntString], fmt, va);
	va_end(va);
#endif // _DEBUG
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CDebugproc::Draw(void)
{
#ifdef _DEBUG
	int nCntString;

	nCntString = strlen(m_aStr);
	// �ϐ��錾
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// �\���ꏊ�E�͈�
	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	// �e�L�X�g�`����
	m_aStr[0] = '\0';
	if (nCntString >= STRING_MAX)
	{
		CCalculation::Messanger("CDebugproc::�f�o�b�O�\�����������܂�!!\n");
	}
#endif // _DEBUG
}


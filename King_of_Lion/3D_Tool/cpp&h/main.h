// ------------------------------------------------------------------------------------------
//
// ���C������ [main.h]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include <windows.h>
#include "d3dx9.h"

#include <stdlib.h>						// �V�X�e���w�b�_�[
#include <stdio.h>						// ��ʓI�ȃw�b�_�[
#include <time.h>						// �^�C���w�b�_�[
#include <math.h>						// ���w�֐��w�b�_�[

#define DIRECTINPUT_VERSION (0x0800)	// �x���Ώ��p
#include "dinput.h"						
#include "xaudio2.h"					// BGM
#include "Calculation.h"				// �v�Z�p

// ------------------------------------------------------------------------------------------
// ���C�u�����̃����N
// ------------------------------------------------------------------------------------------
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define SCREEN_WIDTH	(1280)	// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)	// �E�C���h�E�̍���

// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���_�J���[ / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 3�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@���x�N�g�� /���_�J���[ / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// �t�@�C���Q��
#define FILENAME_DATE ("data/SAVE/")
#define FILENAME_FORMAT (".txt")

// ���ׂẴA�C�e���̍ő吔�Œ�ɂ���
#define ALL_MAX (256)

// ------------------------------------------------------------------------------------------
//
// ��ʃ��[�h�̎��
//
// ------------------------------------------------------------------------------------------
typedef enum
{
	MODE_NONE = 0,
	MODE_GAME,		// �Q�[��
	MODE_POUSE,		// �|�[�Y
	MODE_MAX
} MODE;

// ------------------------------------------------------------------------------------------
// �\���̒�`
// ------------------------------------------------------------------------------------------
// �Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float rhw;			// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
} VERTEX_2D;

// 3�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;		// �e�N�X�`���[
} VERTEX_3D;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void SetMode(MODE mode);
MODE GetMode(void);					// ���[�h�̎擾
LPDIRECT3DDEVICE9 GetDevice(void);

#endif
// ------------------------------------------------------------------------------------------
//
// �O�Տ��� [trajectory.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _TRAJECTORY_H_
#define _TRAJECTORY_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define MAX_MESHOBIT (100)							// �O�Ղ̃|���S����

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// �O�Ղ̃^�C�v //
enum
{
	TRAJECTORYTYPE_PLAYER = 0,
	TRAJECTORYTYPE_GIRAFFE,
	TRAJECTORYTYPE_BUFFALO,
	TRAJECTORYTYPE_KING_RION,
	TRAJECTORYTYPE_MAX
};

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
// ---------- �O�Ղ̐� ---------- //
typedef struct
{
	D3DXVECTOR3 pos[2];									// �ʒu
} MESH_OBIT;

// ---------- �O�� ---------- //
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffTrajectory = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 pos;									// �ʒu
	D3DXVECTOR3 ofset;									// �I�t�Z�b�g
	D3DXVECTOR3 rot;									// ��]��
	D3DXMATRIX  mtxWorld;								// ���[���h�}�g���b�N�X
	MESH_OBIT	MeshObit[MAX_MESHOBIT];					// �O�Ղ̃|�C���g
	int	nNumberVertexMeshObit;							// �����_��
	int nNumPolygon;									// ���O��
	int nNumber;										// ���_
	int	nType;											// �^�C�v
	int nParts;											// ���f���̃p�[�c�ԍ�
	int	nMaxMeshObit;									// �ő�O�Ր�
	int	nCntFram;										// �t���[����
	int	nStartFram;										// �J�n�t���[����
	int nEndFram;										// �I���t���[����
	int	nCntObject;										// �ǂ̃I�u�W�F�N�g��
	bool bUse;											// �g�p���
	bool bDisp;											// �\�����
} TRAJECTORY;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitTrajectory(void);
void UninitTrajectory(void);
void UpdateTrajectory(void);
void DrawTrajectory(void);

// �O�Ղ̐ݒ�
int SetTrajectory(
	D3DXVECTOR3 ofset,
	int nParts,
	int MaxMeshObit,
	int StartFram,
	int EndFram,
	int Type);

// �O�Ղ̔���
void OccurrenceTrajectory(
	int IdTrajectory,
	int	nCntObject);

// �O�Ղ̏���
void DeleteTrajectory(int nIdTrajectory);

TRAJECTORY *GetTrajectory(void);
#endif

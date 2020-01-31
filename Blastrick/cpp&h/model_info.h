//=============================================================================
//
// ���f����񏈗� [model_info.h]
// Author : koki_nishiyama
//
//=============================================================================
#ifndef _MODEL_INFO_H_
#define _MODEL_INFO_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "Calculation.h"

//=============================================================================
// �O���錾
//=============================================================================

//=============================================================================
// �\����
//=============================================================================
// �L�[
typedef struct KEY
{
	KEY()
	{
		pos = D3DVECTOR3_ZERO;
		rot = D3DVECTOR3_ZERO;
	}
	D3DXVECTOR3 pos;						// �ʒu
	D3DXVECTOR3 rot;						// ��]
} KEY;

// �L�[���
typedef struct KEY_INFO
{
	KEY_INFO()
	{
		nFrame = 0;
		nNumCollision = 0;
		nMaxCollisiontime = 0;
		nBlust = 0;
		nCameraType = 0;
		Key = NULL;
	}
	int	nFrame;								// �t���[��
	int nNumCollision;						// �����蔻��̌�
	int nMaxCollisiontime;					// �����蔻��̕p�x
	int nBlust;								// ������ѕ�
	int nCameraType;						// �J�����^�C�v
	KEY *Key;								// �L�[
} KEY_INFO;

// ���[�W�������
typedef struct MOTION
{
	MOTION() 
	{
		nLoop = 0;						// ���[�v
		nNumKey = 0;					// �L�[���̐�
		nAllFrame = 0;					// ���[�V�����S�̂̃t���[����
		Collision_nParts = 0;			// �p�[�c�ԍ�
		Collision_Ofset = D3DVECTOR3_ZERO;			// �I�t�Z�b�g
		Collision_Radius = 0;			// �~�̔��a
		Collision_StartFram = 0;		// �J�n�t���[����
		Collision_EndFram = 0;			// �I���t���[����
		Collision_Damage = 0;			// �_���[�W��
		KeyInfo = NULL;					// �L�[���(���[�V������)
	}
	int			nLoop;						// ���[�v
	int			nNumKey;					// �L�[���̐�
	int			nAllFrame = 0;					// ���[�V�����S�̂̃t���[����
	// �����蔻��p //
	int			Collision_nParts;			// �p�[�c�ԍ�
	D3DXVECTOR3	Collision_Ofset;			// �I�t�Z�b�g
	float		Collision_Radius;			// �~�̔��a
	int			Collision_StartFram;		// �J�n�t���[����
	int			Collision_EndFram;			// �I���t���[����
	int			Collision_Damage;			// �_���[�W��

	KEY_INFO	*KeyInfo;					// �L�[���(���[�V������)
} MOTION;

// �I�t�Z�b�g�̏��
typedef struct MODEL_OFFSET
{
	MODEL_OFFSET()
	{
		pos = D3DVECTOR3_ZERO;
		rot = D3DVECTOR3_ZERO;
		nParent = 0;
	}
	char		cXfile[64];					// ���f���̃t�@�C����
	D3DXVECTOR3	pos;						// �ʒu
	D3DXVECTOR3	rot;						// ��]
	int			nParent;					// �e�ԍ�
} MODEL_OFFSET;

// ���f���E���[�V�����̂̏��
typedef struct
{
	MODEL_OFFSET	*pModel_offset;			// ���f���̃I�t�Z�b�g
	MOTION			*pMotion;				// ���[�V����
} MODEL_ALL;

// ���f�����
typedef struct
{
	LPD3DXMESH			pMesh;				// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		pBuffMat;			// �}�e���A�����ւ̃|�C���^
	LPDIRECT3DTEXTURE9	*ppTexture;			// �e�N�X�`���ւ̃_�u���|�C���^�[
	DWORD				nNumMat;			// �}�e���A���̐�
	D3DXVECTOR3			vtxMinMaterials;	// ���f�����̈ʒu�̍ŏ��l
	D3DXVECTOR3			vtxMaxMaterials;	// ���f�����̈ʒu�̍ő�l
	D3DXVECTOR3			pos;				// �ʒu���
	D3DXVECTOR3			posDest;			// �ړI�ʒu
	D3DXVECTOR3			posDiff;			// ���ʒu
	D3DXVECTOR3			rot;				// ��]���
	D3DXVECTOR3			rotDest;			// �ړI��]
	D3DXVECTOR3			rotDiff;			// ����]
	D3DXVECTOR3			size;				// �T�C�Y
	int					nParent;			// �e�ԍ�
} XMODEL;


//=============================================================================
// �N���X
//=============================================================================
class CModel_info
{
public:
	/* �֐� */
	CModel_info();
	~CModel_info();
	// �J��
	void Unload(void);
	// ���擾
	// �ǂݍ���
	HRESULT Load(	// ���f�����
		LPDIRECT3DDEVICE9 pDevice,				// �f�o�C�X
		const char* charModel);					// �t�@�C����
	// ���f���̃e�L�X�g�f�[�^�Ǎ�
	static void TextLoad(
		MODEL_ALL	*pmodelAll,					// ���f���ƃ��[�V������
		int const &nMaxMotion,					// ���[�V������
		int &nMaxkey,							// �L�[��
		const char * file_name					// �t�@�C����
	);
	// �ǂݍ��񂾃��f���̃e�L�X�g�f�[�^�̔j��
	static void TextUnload(
		MODEL_ALL	*pmodelAll,					// ���f���ƃ��[�V������
		int const &nMaxMotion					// ���[�V������
	);
	// 
	// x���f���̏��擾
	XMODEL	&GetXModel(void);					// X���f�����擾
protected:
private:
	XMODEL				m_Xmodel;				// X���f�����
};

#endif

// ----------------------------------------------------------------------------------------------------
//
// Calculation�w�b�_�[�����̐���[calculation.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

#ifndef _CALCULATION_H_
#define _CALCULATION_H_	 // �t�@�C������������߂�

// ----------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

// ----------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------
// D3DVECTOR2�^�̏�����(1.0f)
#define D3DVECTOR2_INI			D3DXVECTOR2(1.0f,1.0f)
// D3DVECTOR2�^�̏�����(0.0f)
#define D3DVECTOR2_ZERO			D3DXVECTOR2(0.0f,0.0f)
// D3DVECTOR3�^�̏�����(0.0f)
#define D3DVECTOR3_ZERO			D3DXVECTOR3(0.0f,0.0f,0.0f)
// D3DVECTOR3�^�̏�����(1.0f)
#define D3DVECTOR3_ONE			D3DXVECTOR3(1.0f,1.0f,1.0f)
// D3DVECTOR4�^�̏�����(0.0f)
#define D3DVECTOR4_ZERO			D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f)
// D3DXCOLOR�^�̏�����
#define D3DXCOLOR_INI			D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)
// D3DXCOLOR�^�̏�����(��{�F)
#define D3DXCOLOR_C(col)		D3DXCOLOR(col,col,col,1.0f)
// D3DXCOLOR�^�̏�����(��{�F,�����x)
#define D3DXCOLOR_CA(col,alpha)	D3DXCOLOR(col,col,col,alpha)
// �t�@�C���ǂݍ��ݍő�s��
#define FILELINE_ERROW (1048576)

// ----------------------------------------------------------------------------------------------------
//
// Calculation���
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// �񋓌^��`
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// �\���̒�`
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// ���O��`
//
// ----------------------------------------------------------------------------------------------------
// ���������s�̕����|�C���^�[
typedef const char * P_CST_CHAR;

// �����^2��
typedef struct INTEGER2
{
	INTEGER2() {}
	INTEGER2(int X, int Y)
	{
		nMax = X;
		nMin = Y;
	}
	int nMax;
	int	nMin;
}INTEGER2, *PINTEGER2;

// �����^3��
typedef struct INTEGER3
{
public:
	INTEGER3() {}
	INTEGER3(int X, int Y, int Z)
	{
		nX = X;
		nY = Y;
		nZ = Z;
	}
	// �L���X�g
	inline operator int* ()
	{
		(int *)nX;
		(int *)nY;
		(int *)nX;
		return *this;
	}
	inline operator CONST int* () const
	{
		(CONST int *)nX;
		(CONST int *)nY;
		(CONST int *)nZ;
		return *this;
	}
	// ������Z�q
	INTEGER3& operator += (CONST INTEGER3& Source)
	{
		nX += Source.nX;
		nY += Source.nY;
		nZ += Source.nZ;
		return *this;
	}
	INTEGER3& operator -= (CONST INTEGER3& Source)
	{
		nX -= nX;
		nY -= nY;
		nZ -= nZ;
		return *this;
	}
	INTEGER3& operator *= (int Source)
	{
		nX *= Source;
		nY *= Source;
		nZ *= Source;
		return *this;
	}
	INTEGER3& operator /= (int Source)
	{
		nX /= Source;
		nY /= Source;
		nZ /= Source;
		return *this;
	}
	// 2�����Z�q
	INTEGER3 operator + (CONST INTEGER3& Source) const
	{
		// �l��n���ϐ�
		INTEGER3 variable;
		variable.nX = nX + Source.nX;
		variable.nY = nY + Source.nY;
		variable.nZ = nZ + Source.nZ;
		return variable;
	}
	INTEGER3 operator - (CONST INTEGER3& Source) const
	{
		// �l��n���ϐ�
		INTEGER3 variable;
		variable.nX = nX - Source.nX;
		variable.nY = nY - Source.nY;
		variable.nZ = nZ - Source.nZ;
		return variable;
	}
	bool operator == (CONST INTEGER3& Source) const
	{
		return Source == INTEGER3(nX, nY, nZ);
	}
	bool operator != (CONST INTEGER3& Source) const
	{
		return Source != INTEGER3(nX, nY, nZ);
	}

	int nX,		// X������
		nY,		// Y������
		nZ;		// Z������
} INTEGER3, *PINTEGER3;

// �����^4��
typedef struct INTEGER4
{
	INTEGER4() {}
	INTEGER4(
		int d_X, 
		int d_Y,
		int d_Z,
		int d_W
	)
	{
		X = d_X;
		Y = d_Y;
		Z = d_Z;
		W = d_W;
	}
	int X;
	int	Y;
	int Z;
	int	W;
}INTEGER4, *PINTEGER4;

// �z�u�����(1:�^�C�v,2:�ʒu,3:��])
typedef struct ARRANGEMENTOBJ
{
	ARRANGEMENTOBJ() {}
	ARRANGEMENTOBJ(
		int d_nType,
		D3DXVECTOR3 d_pos,
		D3DXVECTOR3 d_rot
	)
	{
		nType = d_nType;
		pos = d_pos;
		rot = d_rot;
	}
	int			nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
} ARRANGEMENTOBJ,*pARRANGEMENTOBJ;

// �z�u�����(1:�^�C�v,2:�ʒu,3:��]4:�c�u���b�N,5:���u���b�N,6:�T�C�YA,7:�T�C�YB)
typedef struct ARRANGEMENTMESH
{
	ARRANGEMENTMESH() {}
	ARRANGEMENTMESH(
		int d_nType,
		D3DXVECTOR3 d_pos,
		D3DXVECTOR3 d_rot,
		int			d_nBlockDepth,
		int			d_nBlockWidth,
		float		d_fSizeA,
		float		d_fSizeB
	)
	{
		nType = d_nType;
		pos = d_pos;
		rot = d_rot;
		nBlockDepth = d_nBlockDepth;
		nBlockWidth = d_nBlockWidth;
		fSizeA = d_fSizeA;
		fSizeB = d_fSizeB;
	}
	int			nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int			nBlockDepth;
	int			nBlockWidth;
	float		fSizeA;
	float		fSizeB;
} ARRANGEMENTMESH, *pARRANGEMENTMESH;

// ----------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------
#define INTEGER4_INI			INTEGER4(1,1,1,1)				

// ----------------------------------------------------------------------------------------------------
//
// �N���X
//
// ----------------------------------------------------------------------------------------------------

class CCalculation
{
public:
	/* �֐� */
	// �����̌v�Z
	static D3DXVECTOR3 Difference_Between(
		D3DXVECTOR3 &posA,
		D3DXVECTOR3 &posB
	);
	// �Ԃ̍��̌v�Z(3D�p:�ʒu�A�ʒu)
	static float DiffPoint(
		D3DXVECTOR3 const & posA,
		D3DXVECTOR3 const & posB
	);
	// �Ԃ̍��̌v�Z(2D�p:�ʒu�A�ʒu)
	static float DiffPoint(
		D3DXVECTOR2 const & posA,
		D3DXVECTOR2 const & posB
	);

	// �O�ς̌v�Z
	static D3DXVECTOR3 Cross_product(
		D3DXVECTOR3 &VecA,
		D3DXVECTOR3 &VecB
	);
	// ���ς̌v�Z
	static float Dot_product(
		D3DXVECTOR3 &VecA,
		D3DXVECTOR3 &VecB
	);
	// �l�p�`�̒��ɓ����Ă��邩���Ȃ���
	static bool SquareBottom_Judg(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 size,
		D3DXVECTOR3 rot,
		int			nBlock_W,
		int			nBlock_H,
		D3DXVECTOR3 worldpos
	);

	// �l�p�`�̒��ɓ����Ă��邩���Ȃ���
	static bool SquareSide_Judg(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 size,
		D3DXVECTOR3 rot,
		int			nBlock_W,
		int			nBlock_H,
		D3DXVECTOR3 worldpos
	);

	// �~�̑����Z
	static float Addition_Circle(
		float Length_x,
		float Length_y,
		float Length_z
	);
	// �~�̓����蔻��
	static bool Collision_Circle(
		D3DXVECTOR3 &Me,
		float		fMeSize,
		D3DXVECTOR3 &Opponent,
		float		fOppSize
	);
	// ��]�ʂ̏�� //
	static float Rot_One_Limit(
		float &fRot
	);
	// �X�N���[�����W�����[���h���W�ɕϊ�
	static D3DXVECTOR3* CalcScreenToWorld(
		D3DXVECTOR3* pout,		// �}�E�X�|�C���g
		float Sx,				// �X�N���[��X���W
		float Sy,				// �X�N���[��Y���W
		float fZ,				// �ˉe��Ԃł�Z�l�i0�`1�j
		float Screen_w,			// �X�N���[���̉���
		float Screen_h,			// �X�N���[���̍���
		D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
		D3DXMATRIX* mtxPrj	// �v���W�F�N�V�����}�g���b�N�X
	);
	// �X�N���[�����W��XZ���ʂ̃��[���h���W��_�Z�o
	static D3DXVECTOR3* CalcScreenToXZ(
		D3DXVECTOR3* pout,		// �}�E�X�|�C���g
		float Sx,				// �X�N���[��X���W
		float Sy,				// �X�N���[��Y���W
		float Screen_w,			// �X�N���[���̉���
		float Screen_h,			// �X�N���[���̍���
		D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
		D3DXMATRIX* mtxPrj,		// �v���W�F�N�V�����}�g���b�N�X
		D3DXVECTOR3 obj			// �I�u�W�F�N�g�̈ʒu
	);	
	// ���[���h���W���X�N���[�����W�ɕϊ�(����)
	static D3DXVECTOR3* CalcWorldToScreen(
		D3DXVECTOR3* Obj,		// �I�u�W�F�N�g�̈ʒu
		float Sx,					// �X�N���[��X���W
		float Sy,					// �X�N���[��Y���W
		float fZ,				// �ˉe��Ԃł�Z�l�i0�`1�j
		float Screen_w,			// �X�N���[���̉���
		float Screen_h,			// �X�N���[���̍���
		D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
		D3DXMATRIX* mtxPrj	// �v���W�F�N�V�����}�g���b�N�X
	);
	// �ꎟ�֐�
	static D3DXVECTOR2 Linear_function(
		D3DXVECTOR3 Apos,			// �n�_
		D3DXVECTOR3 Bpos			// �I�_
	);
	// 2�����̌�_(�A��������)
	static D3DXVECTOR3 TwoLine_Inse(
		D3DXVECTOR2 ALinear,	// ���@
		D3DXVECTOR2 BLinear	// ���A
	);
	// �r���̌v�Z
	static bool TlyCollision(
		D3DXVECTOR3 ObjectPos,
		D3DXVECTOR3,
		...
	);
	// �O�ς̓����蔻��
	static bool CrossCollision(
		D3DXVECTOR3 *ObjectPos,
		D3DXVECTOR3 *PosA,
		D3DXVECTOR3 *PosB,
		D3DXVECTOR3 *PosC
	);
	// ���b�Z�[�W����
	static void Messanger(
		const char * cMessa	// ���b�Z�[�W���e
	);
	// �L�[�����̔��f
	static void KeyString(
		int nKeyID,				// �L�[ID
		char *KeyName = NULL	// ����
	);
	// �t�@�C�������݂��Ă��邩
	static bool Existenceofscript(FILE * pFile);

	// ���f���̃e�L�X�g�f�[�^�Ǎ�
	static void MappingText(
		int const *pmodelAll,					// ���f���ƃ��[�V������
		int const &nMaxMotion,					// ���[�V������
		int &nMaxkey,							// �L�[��
		const char * file_name					// �t�@�C����
	);

protected:

private:
	/* �ϐ� */
};

// ----------------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ----------------------------------------------------------------------------------------------------

#endif
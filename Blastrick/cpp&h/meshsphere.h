//-----------------------------------------------------------------------------
//
// ���b�V���X�t�B�A���� [meshsphere.h]
// Author : Nishiyama Koki
//
//-----------------------------------------------------------------------------
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_

#define _CRT_SECURE_NO_WARNINGS

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "scene.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define RADIUS_SIZE		(45.0f)
#define SEPARATE		(15)

//-----------------------------------------------------------------------------
// �N���X��`
//-----------------------------------------------------------------------------
class CMeshsphere :public CScene
{
public:
	/* �񋓌^ */
	typedef enum
	{
		TEXTYPE_WORLD = 0,
		TEXTYPE_STAR,
		TEXTYPE_MAX
	} TEXTYPE;
	/* �\���� */
	typedef struct
	{
		D3DXVECTOR3		pos;								// ���W
		D3DXVECTOR3		rot;								// ��]
		D3DXCOLOR		col;								// �F
		D3DXMATRIX		mtxWorldMeshSphere;					// ���[���h�}�g���b�N�X
		float			fRadius;							// ���a
		float			fRot;								// �e���_�̊p�x
		float			fRot2;								// �e���_�̊p�x
		int				nHeightBlock;						// �����̕�����
		int				nSeparate;							// �~�̒��_�̌�
		int				nMaxVtx;							// ���_�̌�
		int				nMaxIndex;							// �C���f�b�N�X�̌�
		int				nMaxPolygon;						// �|���S���̖���
		int				nIdxCollisionSphere;				// �~�����ID
		int				nMaxFrame;							// �ő�t���[����
		int				nCntFrame;							// �t���[����
		bool			bUse;								// ���_�ƒ����_�̋���
		bool			bDisp;								// �ʂ����ǂ���
		TEXTYPE			Textype;							//  �e�N�X�`���[�^�C�v
		LPDIRECT3DVERTEXBUFFER9	pVtxBuffMeshSphere = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
		LPDIRECT3DINDEXBUFFER9	pIdxBuffMeshSphere = NULL;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	}	MESHSPHERE;
	/* �֐� */
	CMeshsphere();
	~CMeshsphere();

	static CMeshsphere *Create(void);
	void Init(void);	// ����������
	void Uninit(void);	// �I������
	void Update(void);	// �X�V����
	void Draw(void);	// �`�揈��
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

	static HRESULT Load(void);	// �ǂݍ���
	static void Unload(void);	// �j��
	static int Set(
		const D3DXVECTOR3 pos,
		const float fRadius,
		const int nMaxFrame = 0
	);

	static void SetPosition(int nIdxCollisionSphere, D3DXVECTOR3 pos);
	static void DeleteCollision(int nIdxCollisionSphere);
	static void SetRadius(int nIdxCollisionSphere, float fRadius);
private:
	/* �֐� */
	void Change(void);	// �ω�����
	void World(MESHSPHERE &meshsphere);	// ���E
	bool Star(MESHSPHERE &meshsphere);	// ��
	/* �ϐ� */
	static MESHSPHERE	m_aMeshSphere[TEXTYPE_MAX];
	static LPDIRECT3DTEXTURE9 m_pTex[TEXTYPE_MAX];
};

#endif

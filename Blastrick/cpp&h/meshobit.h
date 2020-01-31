// ----------------------------------------
//
// �O�Տ����̐���[meshobit.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _MESHOBIT_H_
#define _MESHOBIT_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "scene.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define MESHOBIT_MAX (32)
#define OBITLINE_MAX (128)

// ----------------------------------------
//
// �O���錾
//
// ----------------------------------------

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CMeshobit : public CScene
{
public:
	/* �񋓌^ */
	// �e�N�X�`���[�^�C�v
	typedef enum
	{
		TEX_0 = 0,
		TEX_1,
		TEX_MAX
	} TEX;
	// �O�Ղ̃^�C�v
	typedef enum
	{
		TYPE_0 = 0,
		TYPE_1,
		TYPE_MAX
	} TYPE;
	/* �\���� */
	typedef struct
	{
		D3DXVECTOR3 pos[2] = {};	// �ʒu
	} OBIT;

	typedef struct
	{
		D3DXVECTOR3				m_aPosVerTex[2] = {};		// �Z�o�������_���W
		D3DXVECTOR3				m_aOffset[2] = {};			// �I�t�Z�b�g���W
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
		OBIT					obit[OBITLINE_MAX] = {};	// �O�Ղ̐���
		D3DXMATRIX				*othermtx;					// ���̍s��
		D3DXMATRIX				matrix;						// �s��
		D3DXVECTOR3				pos;						// �ʒu
		D3DXVECTOR3				*rot;						// ��]��
		D3DXVECTOR3				move;						// �ړ���
		D3DXCOLOR				col;						// �F
		TEX						tex;						// �e�N�X�`���[
		TYPE					type;						// �^�C�v
		float					fSize;						// �T�C�Y
		int						nLine;						// ���̐�
		int						nCntFrame;					// �t���[����
		int						nMaxFrame;					// �ő�t���[����
		int						nMaxMeshObit;				// �ő�̋O�Ր�
		int						nNumberVertex;				// �����_��
		int						nNumPolygon;				// ���|���S��
		bool					bUse;						// �g�p���
	} MESHOBIT;

	/* �֐� */
	CMeshobit();
	~CMeshobit();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//-------------------------------------------------------------------------------------------------------------
	// �ݒ�
	// 1�s��A2�I�t�Z�b�g�n�_�A3�I�t�Z�b�g�I�_�A4�F�A5�^�C�v�A6�T�C�Y�A7���̌��A8�t���[����
	//-------------------------------------------------------------------------------------------------------------
	static void Set(
		D3DXMATRIX  *pMtx,		// �}�g���b�N�X���
		D3DXVECTOR3	&offset1,	// �I�t�Z�b�g1
		D3DXVECTOR3 &offset2,	// �I�t�Z�b�g2
		D3DXCOLOR	&col,		// �F
		TYPE		type,		// �^�C�v
		TEX			tex,		// �e�N�X�`���[
		float		fSize,		// �T�C�Y���
		int			nLine,		// ��
		int			&nFrame		// �t���[����
	);

	// �ǂݍ���
	static HRESULT LoadMesh(void);
	static HRESULT Load(void);
	// �ǂݍ��񂾂��̂�j��
	static void UnLoad(void);
	// ����
	static CMeshobit * Create();
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
protected:

private:
	/* �֐� */
	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex[TEX_MAX];	// �e�N�X�`���[
	static MESHOBIT m_meshobit[MESHOBIT_MAX];	// �O��
	TEX	m_tex;									// �e�N�X�`���[�^�C�v
};

#endif
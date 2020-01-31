// ----------------------------------------
//
// ���b�V���h�[�������̐���[meshdome.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _MESHDOME_H_
#define _MESHDOME_H_	 // �t�@�C������������߂�

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
class CMeshdome : public CScene
{
public:
	/* �񋓌^ */
	typedef enum
	{
		TYPE_NORMAL,
		TYPE_WAVE,
		TYPE_MAX
	} TYPE;
	/* �֐� */
	CMeshdome();
	CMeshdome(ACTOR actor);
	~CMeshdome();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static HRESULT Load(void);
	static void UnLoad(void);
	static CMeshdome * Create(		// �쐬
		D3DXVECTOR3 pos,			// �ʒu
		D3DXVECTOR3 size,			// �T�C�Y
		D3DXVECTOR3 rot,			// �p�x
		D3DXCOLOR	col,			// �F
		int nWidth,					// ���u���b�N
		int nDepth,					// �c�u���b�N
		TYPE type					// �^�C�v
	);
	static CMeshdome * Create(		// �쐬
		D3DXVECTOR3 size,			// �T�C�Y
		int nWidth,					// ���u���b�N
		int nDepth,					// �c�u���b�N
		TYPE type					// �^�C�v
	);
	// �擾
	bool GetUse(void);						// �g�p���
	// �ݒ�
	void SetPos(D3DXVECTOR3 const &pos);	// �ʒu
	void SetRot(D3DXVECTOR3 const &rot);	// ��]
	void SetCol(D3DXCOLOR const &col);		// �F
	void SetUse(bool const bUse);					// �g�p���
protected:

private:
	/* �֐� */
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);	// ���_���W�̐���
	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex[TYPE_MAX];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIndex;	// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 m_OriginBlock;			// �����z�u
	D3DXMATRIX  m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;					// �ʒu
	D3DXVECTOR3 m_size;					// �T�C�Y
	D3DXVECTOR3 m_rot;					// �p�x
	D3DXCOLOR	m_col;					// �F
	int		m_nNumberVertex;			// �����_��
	int		m_nNumIndex;				// ���C���f�b�N�X
	int		m_nNumPolygon;				// ���|���S��
	int		m_nBlock_Depth;				// �c�u���b�N��
	int		m_nBlock_Width;				// ���u���b�N��
	bool	m_bUse;						// �g�p
	TYPE	m_type;						// �^�C�v
};

#endif
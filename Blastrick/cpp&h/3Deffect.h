// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3D�G�t�F�N�g�̏���[effect.h]
// Author : Nishiyama koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _3DEFFECT_H_
#define _3DEFFECT_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�N����`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define EFFECT_MAX			(10000)						// �ő吔
#define EFFECT_NUM_VERTEX	(4)							// ���_��

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X��`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CRenderer;
class C3DEffect :public CScene
{
public:
	/* �񋓌^ */
	typedef enum
	{
		EFFECT_TYPE_NONE = -1,	// �Ȃ�
		EFFECT_TYPE_EXPLOSION,	// ����
		EFFECT_TYPE_SPARK,		// �Ή�
		EFFECT_TYPE_SMOKE,		// ��
		EFFECT_TYPE_LINE,		// ��
		EFFECT_TYPE_MAX			// �ő吔
	} EFFECT_TYPE;

	/* �\���� */
	// �G�t�F�N�g�v���~�e�B�u
	typedef struct
	{
		D3DXVECTOR3		pos;			// �ʒu
		D3DXVECTOR3		rot;			// ��]��
		D3DXVECTOR3		move;			// �ړ���
		D3DXCOLOR		col;			// �F
		D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
		D3DXMATRIX		mtxView;		// �}�g���b�N�X�r���[
		//float			fRadius;		// ���a
		D3DXVECTOR2		size;			// �T�C�Y
		D3DXVECTOR2		sizeValue;		// �T�C�Y�̕ω��l
		float			fAngle;			// �p�x
		float			fAlphaValue;	// �A���t�@�l�̕ω��l
		int				nLife;			// ��������
		int				nTexType;		// �e�N�X�`���̎��
		bool			bUse;			// �g�p���Ă��邩�ǂ���
		EFFECT_TYPE		EffectType;		// �G�t�F�N�g�̎��
	} EFFECT;
	// �t�@�C���^�O
	typedef struct
	{
		char string[64];
	} FILETAG, *PFILETAG;
	// �e�N�X�`���[���
	typedef struct
	{
		LPDIRECT3DTEXTURE9 pTexture;	// �e�N�X�`�����
	} TEXTURE_INFO;

	/* �֐� */
	C3DEffect();
	~C3DEffect();
	static HRESULT Load(void);
	static void Unload(void);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static C3DEffect *Create(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	// ������
	void InitValues(void);
	// ���_�쐬
	HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice);
	// ���_�T�C�Y�ݒ�
	static void SetVartexSize(
		VERTEX_3D *pVtx,
		EFFECT *pEffect = NULL,
		const int *nIndex = NULL
	);
	// ���_�J���[�ݒ�
	static void SetVetexColor(
		VERTEX_3D *pVtx,
		EFFECT *pEffect = NULL, 
		const int *nIndex = NULL
	);
	// ����
	void Delete(int nCntIndex);
	// �S�̂̐ݒ�
	static void Set3DEffect(
		EFFECT_TYPE EffectType,
		int nTexType,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 rot,
		D3DXVECTOR3 move,
		D3DXCOLOR col,
		D3DXVECTOR2 size,
		int nLife,
		D3DXVECTOR2 sizeValue = D3DVECTOR2_ZERO
	);
private:
	/* �֐� */
	// �e�N�X�`���[�̓ǂݍ���
	static HRESULT LoadTextureFile(void);
	bool UpdateLife(VERTEX_3D *pVtx, EFFECT *pEffect);
	bool UpdateVetex(VERTEX_3D *pVtx, EFFECT *pEffect);
	void UpdateMove(VERTEX_3D *pVtx, EFFECT *pEffect);

	/* �����o�ϐ� */
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;					// �o�b�t�@���
	static TEXTURE_INFO				*m_pTexInfo;				// �e�N�X�`�����
	static EFFECT					m_aEffect[EFFECT_MAX];		// �G�t�F�N�g���
	static const float				m_cfBaseAngle;				// ��{�p�x
	static int						m_nNumTextureMax;			// �e�N�X�`���̍ő吔
};

#endif
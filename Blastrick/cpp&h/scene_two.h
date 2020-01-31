// ----------------------------------------
//
// 2D�V�[�������̐���[scene_two.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _SCENE_TWO_H_
#define _SCENE_TWO_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "main.h"
#include "scene.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_TWO : public CScene
{
public:
	/* �񋓌^ */
	typedef enum
	{
		OFFSET_TYPE_CENTER = 0,
		OFFSET_TYPE_LEFT,
		OFFSET_TYPE_MAX
	} OFFSET_TYPE;
	/* �֐� */
	CScene_TWO();
	CScene_TWO(ACTOR actor, LAYER layer);
	~CScene_TWO();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

	// �ݒ� //
	// ���_���W
	void Set_Vtx_Pos(
		OFFSET_TYPE const &offsettype = OFFSET_TYPE_CENTER,			// �I�t�Z�b�g�^�C�v
		VERTEX_2D * pVtx = NULL										// ���_���
	);
	// ���_�J���[
	void Set_Vtx_Col(VERTEX_2D * pVtx = NULL);
	// �T�C�Y
	void SetSize(
		D3DXVECTOR2 const &size										// �T�C�Y
	);		
	// �ʒu
	void SetPosition(
		D3DXVECTOR3 const &pos										// �ʒu
	);
	// �p�x
	void SetRot(
		float const &frot											// �p�x
	);
	// �F
	void SetCol(
		D3DXCOLOR const &col										// �F
	);	
	// �e�N�X�`���[
	void SetTex(
		D3DXVECTOR2 const &first = D3DXVECTOR2(0.0f,0.0f),			// �����̔z�u
		D3DXVECTOR2 const &last = D3DXVECTOR2(1.0f, 1.0f)			// �Ō�̔z�u
	);
	// �擾 //
	D3DXVECTOR2 GetSize(void) const;								// �T�C�Y���
	D3DXVECTOR3 GetPosition(void) const;							// �ʒu���
	float GetLengh(void) const;										// �������

	void BindTexture(LPDIRECT3DTEXTURE9 p_Tex);	// �e�N�X�`���[
	// �쐬
	static CScene_TWO * Create(
		OFFSET_TYPE	const & type,									// �^�C�v
		D3DXVECTOR3 const & pos,									// �ʒu
		D3DXVECTOR2 const & size,									// �T�C�Y
		ACTOR		const & actor = ACTOR_SCENE_TWO,				// �A�N�^�[���
		float		const & frot = 0.0f,							// �p�x
		D3DXCOLOR	const & col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)	// �F
	);						

protected:
private:
	/* �֐� */
	void Offset_Center(VERTEX_2D *pVtx);	// �I�t�Z�b�g���Z���^�[
	void Offset_Left(VERTEX_2D *pVtx);		// �I�t�Z�b�g����
	
	/* �ϐ� */
	LPDIRECT3DTEXTURE9 m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;						// ���S���W
	D3DXVECTOR2 m_size;						// �T�C�Y
	D3DXCOLOR	m_col;						// �J���[
	OFFSET_TYPE	m_offsetType;				// �I�t�Z�b�g�^�C�v
	float m_rot;							// �p�x
	float m_fLengh;							// ����
	float m_fAngle;							// �p�x
};

#endif
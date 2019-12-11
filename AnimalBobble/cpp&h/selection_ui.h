// ----------------------------------------
//
// �I��UI�����̐���[selection_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _SELECTION_UI_H_
#define _SELECTION_UI_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "scene_two.h"

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
class CMark;

// ------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------
class CSelection_ui : public CScene
{
public:
	/* �֐� */
	CSelection_ui();
	~CSelection_ui();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);		// �ǂݍ���
	static void UnLoad(void);		// UnLoad����
	static CSelection_ui * Create(void);	// �쐬
protected:

private:
	/* �񋓌^ */
	typedef enum
	{
		/* �I���v���C���ɂ͏o�����Ȃ�UI */
		SELECTION_UI_BG = 0,
		SELECTION_UI_SINGLE,
		SELECTION_UI_DISCRIPTION,
		SELECTION_UI_MAX
	} SELECTION_UI;
	/* �֐� */

	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex[SELECTION_UI_MAX];	// �e�N�X�`���[�ݒ�
	static D3DXVECTOR3 m_pos[SELECTION_UI_MAX];			// �ʒu���
	static D3DXVECTOR2 m_size[SELECTION_UI_MAX];			// �T�C�Y���
	float	m_fMult;												// �{��
	float	m_fAdd;													// ���Z
	int		m_nCntCol;												// �J���[�J�E���g
	CScene_TWO *m_aScene_Two[SELECTION_UI_MAX];			// �|�[�Y�pui2D
	CMark *m_mark;											// �ڈ�
	int m_nSelect;									// �I��
};

#endif
// ----------------------------------------
//
// �|�[�Y�pPAUSE_UI�����̐���[pause_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _PAUSE_UI_H_
#define _PAUSE_UI_H_	 // �t�@�C������������߂�

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
class CPause_ui : public CScene
{
public:
	/* �֐� */
	CPause_ui();
	~CPause_ui();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);		// �ǂݍ���
	static void UnLoad(void);		// UnLoad����
	static CPause_ui * Create(void);	// �쐬
protected:

private:
	/* �񋓌^ */
	typedef enum
	{
		PAUSE_UI_BG = 0,
		PAUSE_UI_PAUSE,
		PAUSE_UI_GAME,
		PAUSE_UI_TITLE,
		PAUSE_UI_RESTART,
		PAUSE_UI_MAX
	} PAUSE_UI;
	/* �֐� */

	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex[PAUSE_UI_MAX];	// �e�N�X�`���[�ݒ�
	static D3DXVECTOR3 m_pos[PAUSE_UI_MAX];			// �ʒu���
	static D3DXVECTOR2 m_size[PAUSE_UI_MAX];		// �T�C�Y���
	static int m_nSelect;							// �I��
	float	m_fMult;								// �{��
	float	m_fAdd;									// ���Z
	int		m_nCntCol;								// �J���[�J�E���g
	CMark * m_mark;									// �}�[�N
	CScene_TWO *m_aScene_Two[PAUSE_UI_MAX];			// �|�[�Y�pui2D
};

#endif
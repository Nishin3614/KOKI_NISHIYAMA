// ----------------------------------------
//
// �^�C�g��UI�����̐���[title_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _TITLE_UI_H_
#define _TITLE_UI_H_	 // �t�@�C������������߂�

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

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CTitle_ui : public CScene
{
public:
	/* �֐� */
	CTitle_ui();
	~CTitle_ui();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static HRESULT Load(void);		// �ǂݍ���
	static void UnLoad(void);		// UnLoad����
	static CTitle_ui * Create(void);	// �쐬
protected:

private:
	/* �񋓌^ */
	typedef enum
	{
		/* �^�C�g���v���C���ɂ͏o�����Ȃ�UI */
		TITLE_UI_NAME = 0,
		TITLE_UI_CLICK,
		TITLE_UI_MAX
	} TITLE_UI;
	/* �֐� */
	void Various(int nCnt);											// ��ޕʏ���
																	
	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex[TITLE_UI_MAX];	// �e�N�X�`���[�ݒ�
	static D3DXVECTOR3 m_pos[TITLE_UI_MAX];			// �ʒu���
	static D3DXVECTOR2 m_size[TITLE_UI_MAX];			// �T�C�Y���
	float	m_fMult;												// �{��
	float	m_fAdd;													// ���Z
	int		m_nCntCol;												// �J���[�J�E���g
	CScene_TWO *m_aScene_Two[TITLE_UI_MAX];			// �|�[�Y�pui2D
};

#endif
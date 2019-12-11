// ----------------------------------------
//
// �`���[�g���A��UI�����̐���[tutorial_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _TUTORIAL_UI_H_
#define _TUTORIAL_UI_H_	 // �t�@�C������������߂�

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

// ------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------
class CTutorial_ui : public CScene
{
public:
	/* �֐� */
	CTutorial_ui();
	~CTutorial_ui();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);		// �ǂݍ���
	static void UnLoad(void);		// UnLoad����
	static CTutorial_ui * Create(void);	// �쐬
protected:

private:
	/* �֐� */
																	
	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex;	// �e�N�X�`���[�ݒ�
	CScene_TWO *m_aScene_Two;			// �`���[�g���A���pui2D
	int m_nSelect;						// �I��ԍ�
	float m_fAnim;						// �A�j���[�V����
};

#endif
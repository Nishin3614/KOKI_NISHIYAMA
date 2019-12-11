// ----------------------------------------
//
// �v���C�pUI�����̐���[play_element_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _PLAY_ELEMENT_UI_H_
#define _PLAY_ELEMENT_UI_H_	 // �t�@�C������������߂�

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
class CNumber;

// ------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------
class CPlay_element_ui : public CScene
{
public:
	/* �֐� */
	CPlay_element_ui();
	~CPlay_element_ui();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CNumber * GetNumber(void);		// �ԍ��擾
	static HRESULT Load(void);		// �ǂݍ���
	static void UnLoad(void);		// UnLoad����
	static CPlay_element_ui * Create(CManager::PLAYER playerId);	// �쐬
protected:

private:
	/* �񋓌^ */
	typedef enum
	{
		/* �Q�[������UI */
		PLAY_ELEMENT_UI_SCORE = 0,
		PLAY_ELEMENT_UI_TIME,
		PLAY_ELEMENT_UI_MAX
	} PLAY_ELEMENT_UI;
	/* �֐� */

	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex[PLAY_ELEMENT_UI_MAX];	// �e�N�X�`���[�ݒ�
	static D3DXVECTOR3 m_pos[PLAY_ELEMENT_UI_MAX];			// �ʒu���
	static D3DXVECTOR2 m_size[PLAY_ELEMENT_UI_MAX];			// �T�C�Y���
	CScene_TWO *m_aScene_Two[PLAY_ELEMENT_UI_MAX];			// �v���C�pUI2D
	CManager::PLAYER playerID;								// �v���C���[�ԍ�
	CNumber * m_pNumber;									// �ԍ�
};

#endif
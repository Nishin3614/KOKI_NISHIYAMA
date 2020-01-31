// ----------------------------------------
//
// �Q�[��UI�����̐���[game_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _GAME_UI_H_
#define _GAME_UI_H_	 // �t�@�C������������߂�

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

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CGame_ui : public CScene
{
public:
	/* �֐� */
	CGame_ui();
	~CGame_ui();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	CNumber * GetNumber(void);		// �ԍ��擾
	static HRESULT Load(void);		// �ǂݍ���
	static void UnLoad(void);		// UnLoad����
	static CGame_ui * Create(void);	// �쐬
protected:

private:
	/* �񋓌^ */
	typedef enum
	{
		/* �Q�[���v���C���ɂ͏o�����Ȃ�UI */
		GAME_UI_START = 0,
		GAME_UI_STAGECLEAR,
		GAME_UI_GAMEOVER,
		GAME_UI_MAX
	} GAME_UI;
	/* �֐� */

	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex[GAME_UI_MAX];	// �e�N�X�`���[�ݒ�
	static D3DXVECTOR3 m_pos[GAME_UI_MAX];			// �ʒu���
	static D3DXVECTOR2 m_size[GAME_UI_MAX];			// �T�C�Y���
	CScene_TWO *m_aScene_Two[GAME_UI_MAX];			// �|�[�Y�pui2D
	CNumber * m_pNumber;							// �ԍ�
};

#endif
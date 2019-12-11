// ----------------------------------------
//
// �V�[�������̐���[scene.h]
// Author : Koki Nishiyama
//
// ----------------------------------------

#ifndef _SCENE_H_
#define _SCENE_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "manager.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------

// ------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------
class CScene
{
public:
	/* �񋓌^ */
	// ���̔z�u����
	typedef enum
	{
		LAYER_OBJ = 0,
		LAYER_BULLET,
		LAYER_PARTICLE,
		LAYER_UI,
		LAYER_MAX
	} LAYER;
	// ���̔z�u����(��)
	typedef enum
	{
		ACTOR_ARRAY = 0,
		ACTOR_BOX,
		ACTOR_BG,
		ACTOR_BULLET,
		ACTOR_CEILING,
		ACTOR_EFFECT,
		ACTOR_EXPLOSION,
		ACTOR_FRAME,
		ACTOR_GAME_UI,
		ACTOR_LINE,
		ACTOR_MARK,
		ACTOR_MODEL,
		ACTOR_NUMBER,
		ACTOR_PAUSE_UI,
		ACTOR_PLAYER_ELEMENT_UI,
		ACTOR_PLAYER,
		ACTOR_PREDICTION,
		ACTOR_RANK_UI,
		ACTOR_SCENE_TWO,
		ACTOR_SCORE,
		ACTOR_SELECTION_UI,
		ACTOR_TITLE_UI,
		ACTOR_TOPSCORE,
		ACTOR_TUTORIAL_UI,
		ACTOR_MAX
	} ACTOR;

	/* �֐� */
	CScene();
	CScene(ACTOR actor, LAYER layer = LAYER_OBJ);
	virtual ~CScene();
	virtual void Init(void) = 0;		// ������
	virtual void Uninit(void) = 0;		// �I��
	virtual void Update(void) = 0;		// �X�V
	virtual void Draw(void) = 0;		// �`��
	static void ReleaseAll(void);		// �V�[���e�q�����[�X
	static void UpdateAll(void);		// �V�[���e�q�X�V
	static void DrawAll(void);			// �V�[���e�q�`��
	static CScene * GetActor(ACTOR actor, LAYER layer, int nCntScene);	// �V�[�����擾
	static int GetMaxLayer(LAYER layer);								// ���C���[�̍ő吔
	void Release(void);	// �I�u�W�F�N�g�̔j��
protected:

private:
	/* �֐� */
	static void DeadFragAll(void);		// ���S�t���O�����������̂������[�X
										/* �ϐ� */
	static CScene * m_pTop[LAYER_MAX];	// �擪�I�u�W�F�N�g�ւ̃|�C���^
	static CScene * m_pCur[LAYER_MAX];	// ���݃I�u�W�F�N�g�ւ̃|�C���^
	static int m_nMaxLayer[LAYER_MAX];	// ���C���[�ő吔
	CScene * m_pNext;					// ���̃I�u�W�F�N�g�ւ̃|�C���^
	CScene * m_pPrey;					// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	LAYER m_layer;						// ���C���[�ԍ�
	ACTOR m_nActor;						// �I�u�W�F�N�g�ԍ�
	int m_nID;							// �����̔ԍ�
	bool m_bDeadFrag;					// �t���O
};

#endif
// ----------------------------------------
//
// �v���C���[�����̐���[player.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "main.h"
#include "scene_two.h"
#include "prediction.h"

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
class CPlayer : public CScene_TWO
{
public:
	/* �֐� */
	CPlayer();
	~CPlayer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ShotState(bool bShot);
	CManager::PLAYER GetPlayerId(void);
	D3DXVECTOR3 GetPos(void);
	int GetShotCnt(void);				// �e�̔��˃J�E���g�擾
	static HRESULT Load(void);
	static void UnLoad(void);
	static CPlayer * Create(CManager::PLAYER playerId,D3DXVECTOR3 pos);		// �쐬
protected:

private:
	/* �֐� */
	void Animation(void);				// �A�j���[�V�����Ǘ�
	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex;
	D3DXVECTOR3 m_pos;					// �ʒu
	float m_rot;						// ��]��
	int m_nHorizonAnim;					// �����̃A�j���[�V�����p�^�[��
	int m_nVirticalAnim;				// �����̃A�j���[�V�����p�^�[��
	int m_nCntAnim;						// �A�j���[�V�����J�E���g
	int m_nShotCnt;						// �������˃J�E���g
	int m_nShotTime;					// �������˃^�C��
	bool m_bShot;						// ���ˏ��
	CManager::PLAYER m_PlayerId;		// �v���C���[�ԍ�
	CPrediction * m_pprediction;		// �\����
};

#endif
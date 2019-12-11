// ----------------------------------------
//
// �������̐���[box.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _BOX_H_
#define _BOX_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "main.h"
#include "scene_two.h"

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
class CBox : public CScene_TWO
{
public:
	/* �֐� */
	CBox();
	~CBox();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVirticalAnim(int nVirtical);	// �����̃A�j���[�V�����p�^�[���̐ݒ�
	void SetHorizonAnim(int nHorizon);		// �����̃A�j���[�V�����p�^�[���̐ݒ�
	CManager::PLAYER GetPlayerId(void);
	D3DXVECTOR3 GetPos(void);				// �ʒu���擾
	static HRESULT Load(void);
	static void UnLoad(void);
	static CBox * Create(CManager::PLAYER playerId,D3DXVECTOR3 pos);		// �쐬
protected:

private:
	/* �֐� */
	void Animation(void);				// �A�j���[�V�����Ǘ�
	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex;
	int m_nHorizonAnim;					// �����̃A�j���[�V�����p�^�[��
	int m_nVirticalAnim;				// �����̃A�j���[�V�����p�^�[��
	int m_nCntAnim;						// �A�j���[�V�����J�E���g
	CManager::PLAYER m_PlayerId;		// �v���C���[�ԍ�
};

#endif
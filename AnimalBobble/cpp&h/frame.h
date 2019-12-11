// ----------------------------------------
//
// �g�����̐���[frame.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _FRAME_H_
#define _FRAME_H_	 // �t�@�C������������߂�

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
class CFrame : public CScene_TWO
{
public:
	/* �֐� */
	CFrame();
	~CFrame();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CManager::PLAYER GetPlayerId(void);
	D3DXVECTOR3 GetPos(void);	// �ʒu���擾
	D3DXVECTOR2 GetSize(void);	// �T�C�Y���擾
	static HRESULT Load(void);	// �ǂݍ���
	static void UnLoad(void);	// �ǂݍ��񂾏���j��
	static CFrame * Create(CManager::PLAYER playerId,D3DXVECTOR3 pos);	// �쐬
protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	CManager::PLAYER m_PlayerId;	// �v���C���[�ԍ�
};

#endif
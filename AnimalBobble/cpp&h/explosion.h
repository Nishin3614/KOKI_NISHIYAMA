// ----------------------------------------
//
// ���������̐���[explosion.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "main.h"
#include "scene_two.h"
#include "bullet.h"

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
class CExplosion : public CScene_TWO
{
public:
	/* �֐� */
	CExplosion();
	~CExplosion();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CExplosion * Create(D3DXVECTOR3 pos, CBullet::TYPE nType);	// �쐬
	static HRESULT Load(void);									// �ǂݍ���
	static void UnLoad(void);									// �ǂݍ��񂾏���j��
protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex[CBullet::TYPE_MAX];	// �e�N�X�`���[���
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXCOLOR m_col;							// �F
	int	m_nLife;								// �̗�
	CBullet::TYPE m_type;						// �^�C�v
};
#endif
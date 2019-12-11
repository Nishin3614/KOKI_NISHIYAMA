// ------------------------------------------------------------------------------------------
//
// �V�䏈���̐���[ceiling.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _CEILING_H_
#define _CEILING_H_	 // �t�@�C������������߂�

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#include "main.h"
#include "scene_two.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------
class CCeiling : public CScene_TWO
{
public:
	/* �֐� */
	CCeiling();
	~CCeiling();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CManager::PLAYER GetPlayerId(void);
	D3DXVECTOR3 GetPos(void);		// �ʒu���擾
	D3DXVECTOR2 GetSize(void);		// �T�C�Y���擾
	// �ʒu�ƃT�C�Y�̐ݒ�
	void SetPosSize(
		D3DXVECTOR3 pos,	// �ʒu
		D3DXVECTOR2 size	// �T�C�Y
	);
	static HRESULT Load(void);		// �ǂݍ���
	static void UnLoad(void);		// �ǂݍ��񂾏���j��
	static CCeiling * Create(		// �쐬
		CManager::PLAYER playerId,	// �v���C���[�ԍ�
		D3DXVECTOR3 pos,			// �ʒu	
		D3DXVECTOR2 size			// �T�C�Y
	);
protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	CManager::PLAYER m_PlayerId;	// �v���C���[�ԍ�
};

#endif
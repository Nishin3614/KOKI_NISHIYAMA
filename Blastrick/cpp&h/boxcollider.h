// ----------------------------------------
//
// ���̓����蔻�菈���̐���[boxcollider.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _BOXCOLLIDER_H_
#define _BOXCOLLIDER_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "scene.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CBoxcollider : public CScene
{
public:
	/* �֐� */
	CBoxcollider();
	CBoxcollider(ACTOR actor, LAYER layer);
	~CBoxcollider();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

	void SetSize(D3DXVECTOR2 size);						// �T�C�Y�ݒ�
	void SetPosition(D3DXVECTOR3 pos);					// �ʒu�ݒ�
	void SetRot(float frot);							// ��]�ݒ�
	D3DXVECTOR2 GetSize(void);							// �T�C�Y���擾
	D3DXVECTOR3 GetPosition(void);						// �ʒu���擾

	static CBoxcollider * Create();						// �쐬

protected:
private:
	D3DXVECTOR3 m_pos;					// ���S���W
	float m_rot;						// �p�x
	D3DXVECTOR2 m_size;					// �T�C�Y
};

#endif
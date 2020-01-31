//=============================================================================
//
// �_�l�ԏ��� [rothuman.h]
// Author : koki_nishiyama
//
//=============================================================================
#ifndef _ROTHUMAN_H_
#define _ROTHUMAN_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "enemy.h"

//=============================================================================
// �O���錾
//=============================================================================

//=============================================================================
// �N���X
//=============================================================================
class CRothuman : public CEnemy
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_DAMAGE,
		STATE_DIE
	} STATE;

	// ---------���[�V�����^�C�v---------- //
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_MOVE,
		MOTIONTYPE_ATTACK,
		MOTIONTYPE_JUMP,
		MOTIONTYPE_LANDING,
		MOTIONTYPE_MAX
	} MOTIONTYPE;

	/* �֐� */
	CRothuman();
	~CRothuman();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	// �쐬
	static CRothuman * Create(		
		D3DXVECTOR3 pos			// �ʒu
	);
	static HRESULT LoadCreate(void);
	static HRESULT Load(void);		// �ǂݍ���
	static void UnLoad(void);		// UnLoad����
protected:
private:
	/* �֐� */
	void move(void);	// �ړ�
	void rot(void);		// ��]

	// AI�p
	void AiAction(void);
	void AttackAction(void);
	void NormalAction(void);
	/* �ϐ� */
};
#endif

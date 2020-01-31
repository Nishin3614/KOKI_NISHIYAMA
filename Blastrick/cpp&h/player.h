// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���f������ [player.h]
// Author : koki_nishiyama
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "character.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CStateGauge;
class C2DGauge;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CPlayer : public CCharacter
{
public:
	// ---------���[�V�����^�C�v---------- //
	typedef enum
	{
		/* �ʏ탂�[�V���� */
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_MOVE,
		/* �R���{1 */
		MOTIONTYPE_DIAGONALCUT,
		MOTIONTYPE_THRUST,
		/* �R���{2 */
		MOTIONTYPE_REVERSESIDE,
		/* �R���{3 */
		MOTIONTYPE_SLASHER,
		MOTIONTYPE_WHIRLWIND,
		/* �K���` */
		MOTIONTYPE_TRICK1_FIREUPSIDE,
		MOTIONTYPE_TRICK2_FLASHSTRIKE,
		MOTIONTYPE_TRICK3_PAINLIGHTNING,
		MOTIONTYPE_MAX
	} MOTIONTYPE;
	/* �֐� */
	CPlayer();
	~CPlayer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	D3DXVECTOR3 GetPos(void);		// �ʒu�擾
	D3DXVECTOR3 GetRot(void);		// ��]�擾
	static CPlayer * Create();		// �쐬
	static HRESULT Load(void);		// �ǂݍ���
	static void UnLoad(void);		// UnLoad����
protected:
private:
	/* �\���� */
	/* �֐� */
	void Move(void);				// �ړ�
	void Stop(void);				// ��~
	void State(void);				// ���
	void MotionState(void);			// ���[�V�������̏��
	void Motion_Slasher(void);		// �X���b�V���[
	void Motion_Attack(void);		// �U��

	void Attack(void);				// �U��
	void At_Normal(void);			// �m�[�}���U��
	void At_DiagonalCut(void);		// �R���{�P_�ǉ����͂Ȃ�(�΂ߐ؂�)
	void At_Thrust(void);			// �R���{�P_�ǉ����́�(�˂�)
	void At_ReverseSide(void);		// �R���{2_�ǉ�����(�t�؂�)
	void ConboAttack(void);			// �R���{�U��
	void Trick(void);				// �鉜�`
	void Trick1_FireUpside(void);	// �鉜�`1
	void Trick2_FlashStrike(void);	// �鉜�`2
	void Trick3_PainLightning(void);// �鉜�`3
	/* �ϐ� */
	D3DXVECTOR3	m_posold;			// �O�̈ʒu
	int			m_nConbo;			// �R���{
	int			m_nInvalConbo;		// �R���{�C���^�[�o��
	bool		m_bTrick;			// �鉜�`�����ǂ���

	C2DGauge	*p2DHPGauge;		// HP�Q�[�W
	C2DGauge	*p2DTrickGauge;		// �鉜�`�Q�[�W	
};
#endif

// ----------------------------------------
//
// �����蔻�菈���̐���[collider.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _COLLIDER_H_
#define _COLLIDER_H_	 // �t�@�C������������߂�

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
class CCollider : public CScene
{
public:
	/* �񋓌^ */
	// �Ǘ���
	typedef enum
	{
		OWNER_MYSELF = 0,
		OWNER_YOU,
		OWNER_MAX
	} OWNER;
	/* �֐� */
	CCollider();
	CCollider(ACTOR actor, LAYER layer);
	~CCollider();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	// �ݒ�
	void Set(	
		D3DXMATRIX	*mtx,				// �s��
		D3DXVECTOR3 const &offset,		// �I�t�Z�b�g�ʒu
		float		const &fRadius,		// ���a
		int			const &nStartFrame,	// �X�^�[�g
		int			const &nEndFrame,	// �G���h
		int			const &nAttack		// �U����
	);
	// �g�p���
	void SetUse(bool const bUse);
	// �ʒu�ݒ�
	void SetPos(D3DXVECTOR3 const &pos);	
	// �ʒu�擾
	D3DXVECTOR3 GetPos(void);					// �ʒu
	// �L�����N�^�[�擾
	int GetCharacter(void);						// �L�����N�^�[�^�C�v
	// �����̈ʒu����̐���
	static CCollider * Create(
		D3DXVECTOR3	const &pos,					// �ʒu
		float		const &fRadius,				// ���a
		int			const &nAttack,				// �U����
		int			const &nCharacter,			// �L�����N�^�[
		OWNER		const &owner = OWNER_YOU	// �Ǘ���
	);
	// �s�񂩂�̐���
	static CCollider * Create(
		D3DXMATRIX	*mtx,						// �s��
		D3DXVECTOR3 const &offset,				// �I�t�Z�b�g�ʒu
		float		const &fRadius,				// ���a
		int			const &nStartFrame,			// �X�^�[�g
		int			const &nEndFrame,			// �G���h
		int			const &nAttack,				// �U����
		int			const &nCharacter,			// �L�����N�^�[
		OWNER		const &owner = OWNER_MYSELF	// �Ǘ���
	);

protected:
private:
	/* �֐� */
	void OwnerMyself(void);		// �Ǘ��Ҏ���
	void OwnerYou(void);		// �Ǘ��Ҏg�p���Ă������
	/* �ϐ� */
	D3DXMATRIX	*m_mtx;			// �s��
	D3DXVECTOR3 m_offset;		// �I�t�Z�b�g�ʒu
	D3DXVECTOR3 m_pos;			// ���S���W
	OWNER		m_owner;		// �Ǘ���
	float		m_fRadius;		// ���a
	int			m_nStartFrame;	// �X�^�[�g
	int			m_nEndFrame;	// �G���h
	int			m_nCntFrame;	// �t���[���J�E���g
	int			m_nAttack;		// �U����
	int			m_nCharacter;	// �L�����N�^�[�^�C�v
	bool		m_bUse;			// �L�����ǂ���
};

#endif
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �J�������� [camera.h]
// Author : KOKI_NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCamera
{
public:
	/* �񋓌^ */
	typedef enum
	{
		TYPE_FOLLOW = 0,
		TYPE_FRONT,
		TYPE_BACK,
		TYPE_RIGHT,
		TYPE_LEFT,
		TYPE_SLASH,
		TYPE_SLASH_NEAR,
		TYPE_SLASH_FAR,
		TYPE_MAX,
	} TYPE;
	/* �\���� */
	// �ǂݍ��ݗp
	typedef struct
	{
		D3DXVECTOR3 rot;	// ��]��
		D3DXVECTOR2 offset;	// �����_�̃I�t�Z�b�g
		float fLengh;		// �����_�Ǝ��_�̒���
		float fHeight;		// �����_�Ǝ��_�̍���
		int nType;			// �^�C�v
	} LOAD;
	/* �֐� */
	CCamera();
	~CCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCamera * Create(void);	// �쐬
	// �ǂݍ���
	static HRESULT Load(void);
	// �j��
	static void Unload(void);
	// �J�����̏�񏉊���
	void InfoInit(
		D3DXVECTOR3 const &pos,	// �ʒu���
		D3DXVECTOR3 const &rot	// ��]���
		);
	// �J�������擾
	CCamera *Get(void);
	// �J�����ݒ�
	void Set(void);
	void SetPosR(
		D3DXVECTOR3 const & pos,	// �ʒu��� 
		D3DXVECTOR3 const & rot		// ��]��� 
	);
	void SetType(TYPE type);
	D3DXVECTOR3 GetRot(void);
protected:

private:
	/* �֐� */
	void Rot(void);						// ��]����
	void Type(void);					// �^�C�v����
	/* �ϐ� */
	D3DXVECTOR3		m_posV;				// ���_
	D3DXVECTOR3		m_posVDest;			// �ړI���_
	D3DXVECTOR3		m_posVDiff;			// �ړI���_���王�_
	D3DXVECTOR3		m_posR;				// �����_
	D3DXVECTOR3		m_posRDest;			// �ڕW�����_
	D3DXVECTOR3		m_posRDiff;			// �ړI�����_���璍���_
	D3DXVECTOR3		m_posU;				// ������x�N�g��
	D3DXVECTOR3		m_between;			// �v���C���[�Ƃ̋��� 
	D3DXVECTOR3		m_last;				// 
	D3DXVECTOR3		m_move;				// �ړ���
	D3DXVECTOR3		m_rot;				// ���_�̉�]��
	D3DXVECTOR3		m_rotDest;			// ���_�̖ړI��]�n�_
	D3DXVECTOR3		m_rotDiff;			// �ړI��]�n�_���猻�݉�]�n�_
	D3DXVECTOR3		m_rotBetween;		// �ړI��]�n�_�ƌ��݉�]�n�_�̍���
	D3DXVECTOR3		m_rotOrigin;		// ���̉�]��
	D3DXMATRIX		m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X(�e���r���_)
	D3DXMATRIX		m_mtxView;			// �r���[�}�g���b�N�X(�J�����}�����_)
	TYPE			m_type;				// �^�C�v[
	float			m_fHeight;			// �����_����̎��_�̍���
	float			m_fLength;			// ���_�ƒ����_�̋���
	static	LOAD	m_load[TYPE_MAX];	// ���ۑ�
};

#endif

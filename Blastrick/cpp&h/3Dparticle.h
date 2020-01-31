// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3D�p�[�e�B�N���̏���[particle.h]
// Author : Nishiyama koki
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _3DPARTICLE_H_
#define _3DPARTICLE_H_

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "Scene.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�N����`
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X��`
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class C3DParticle :public CScene
{
public:
	/* �񋓌^ */
	// �G�t�F�N�g�^�C�v
	typedef enum
	{
		TYPE_NORMAL = 0,
		TYPE_CHARGE,
		TYPE_MAX,
	} TYPE;
	// �����ݒ�
	typedef enum
	{
		OFFSET_ID_NONE = -1,
		OFFSET_ID_DIRT,				// �y��
		OFFSET_ID_AWAYSMOKE,		// ������т��ނ�
		OFFSET_ID_EXPLOSIONSMOKE,	// �������ނ�
		OFFSET_ID_SHOCKWAVE,		// �Ռ��g
		OFFSET_ID_GET,				// 
		OFFSET_ID_SPARKS,			// �Ή�
		OFFSET_ID_STAR,				// ����
		OFFSET_ID_CROSSLINE,		// �N���X���C��
		OFFSET_ID_WEAVELINE,		// �҂ݕ����C��
		OFFSET_ID_RANDAMLINE,		// �����_�����C��
		OFFSET_ID_CENTERRANDAMLINE,	// ���S�����_�����C��
		OFFSET_ID_ONELINE,			// ����C��
		OFFSET_ID_CONCENTLINE,		// �R�l�N�g���C��
		OFFSET_ID_MAX,
	} PARTICLE_OFFSET_ID;
	/* �\���� */
	typedef struct
	{
		int					nFrame;			// �t���[����
		int					nNumber;		// ��
		int					nEffeTexType;	// �G�t�F�N�g�̃e�N�X�`���̎��
		int					nEffeType;		// �G�t�F�N�g�̎��
		C3DParticle::TYPE	type;			// ���(��Ɉړ���)

		D3DXVECTOR3			*pSpeedRate;	// ���x���[�g

		D3DXVECTOR3			*pPos;			// �ʒu�̃|�C���^
		PINTEGER2			pPosXRand;		// �ʒuX���W�����_���p�̃|�C���^
		PINTEGER2			pPosYRand;		// �ʒuY���W�����_���p�̃|�C���^
		PINTEGER2			pPosZRand;		// �ʒuY���W�����_���p�̃|�C���^


		float				*pfLength;		// ���_����̋����̃|�C���^
		PINTEGER2			pnLengthRand;	// ���_����̋��������_���p�̃|�C���^

		D3DXCOLOR			*pCol;			// �F�̃|�C���^
		bool				bRedRand;		// �ԃ����_��
		bool				bGreenRand;		// �΃����_��
		bool				bBlueRand;		// �����_��
		bool				bAlphaRand;		// �����x�����_��

		D3DXVECTOR2			*pSize;			// �T�C�Y
		PINTEGER2			pSizeXRand;		// �T�C�Yx�����_���p�̃|�C���^�[
		PINTEGER2			pSizeYRand;		// �T�C�Yy�����_���p�̃|�C���^�[

		D3DXVECTOR3			*pRot;			// ��]�̃|�C���^�[
		PINTEGER2			pnAngleRand;	// �p�x�̃����_���p�̃|�C���^

		float				*pfSpeed;		// ���x�̃|�C���^
		PINTEGER2			pnSpeedRand;	// ���x�̃����_���p�̃|�C���^

		int					*pnLife;		// ���C�t�̃|�C���^
		PINTEGER2			pnLifeRand;		// ���C�t�̃����_���p�̃|�C���^
	} PARTICLE_OFFSET;

	/* �֐� */
	C3DParticle();
	~C3DParticle();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static C3DParticle * Create(PARTICLE_OFFSET_ID OffsetID, CONST D3DXVECTOR3 origin);
	static HRESULT Load(void);
	static void Unload(void);
	void SetParticle(PARTICLE_OFFSET_ID OffsetID);
	void SetOrigin(CONST D3DXVECTOR3 &Origin);

private:
	/* �֐� */
	void Update_Col(void);		// �J���[
	void Update_Size(void);		// �T�C�Y
	void Update_Life(void);		// ���C�t
	void Update_Speed(void);	// �X�s�[�h
	void Update_Angle(void);	// �p�x
	void Update_Length(void);	// ����
	void Update_Pos(void);		// �ʒu
	void Update_Move(void);		// �ړ���

	/* �ϐ� */
	static PARTICLE_OFFSET	m_ParticleOffset[OFFSET_ID_MAX];	// �p�[�e�B�N���̃I�t�Z�b�g
	PARTICLE_OFFSET_ID		m_offsetID;							// �I�t�Z�b�gID
	int						m_nFlameCount;						// �t���[���̃J�E���g
	D3DXVECTOR3				m_Origin;							// ���_
};

#endif

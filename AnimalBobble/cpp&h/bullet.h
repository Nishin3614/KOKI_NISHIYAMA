// ------------------------------------------------------------------------------------------
//
// �e�����̐���[bullet.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_	 // �t�@�C������������߂�

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#include "scene_two.h"
#include "array.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------
#define BULLETSIZE (50.0f)	// �e�̃T�C�Y

// ------------------------------------------------------------------------------------------
//
// �O�u�N���X
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------
class CBullet : public CScene_TWO
{
public:
	/* �񋓌^ */
	// �^�C�v
	typedef enum
	{
		TYPE_BEE = 0,	// �n�`
		TYPE_CAT,		// �L
		TYPE_FISH,		// ��
		TYPE_HAWK,		// ��
		TYPE_SNAKE,		// ��
		TYPE_MAX
	} TYPE;
	// ����
	typedef enum
	{
		DIRECTION_NORTHWEST = 0,// �k��
		DIRECTION_NORTHEAST,	// �k��
		DIRECTION_EAST,			// ��
		DIRECTION_SOUTHEAST,	// �쓌
		DIRECTION_SOUTHWEST,	// �쐼
		DIRECTION_WEST,			// ��
		DIRECTION_MAX
	} DIRECTION;
	// ���
	typedef enum
	{
		STATE_BOX = 0,	// ��
		STATE_PLAYER,	// �v���C���[
		STATE_OUT,		// ���o
		STATE_END,		// �����ꂽ
		STATE_MAX
	} STATE;
	
	/* �֐� */
	CBullet();
	~CBullet();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos);			// �ʒu
	void SetPosGoal(D3DXVECTOR3 posgoal);	// �ړI�̈ʒu�܂�
	void SetRot(float frot);				// ��]���
	void SetState(STATE state);				// ���
	CManager::PLAYER GetPlayerId(void);		// �v���C���[�ԍ�
	STATE GetState(void);					// ���
	D3DXVECTOR3 GetPos(void);				// �ʒu
	static CBullet * Create(				// �쐬
		CManager::PLAYER PlayerId,
		D3DXVECTOR3 pos,
		TYPE nType);	
	static CBullet * LoadCreate(			// �ǂݍ��񂾂��̂��쐬
		CManager::PLAYER PlayerId,
		int nMode, 
		int nStage);	
	static HRESULT Load(void);				// �ǂݍ���
	static void LoadStage(void);			// �X�e�[�W�ǂݍ���
	static void UnLoad(void);				// �ǂݍ��񂾏���j��
	static int GetAllBullet(void);			// �e�̑���
protected:

private:
	/* �֐� */
	void Box(void);								// �{�b�N�X�ҋ@��
	void Player(void);							// �v���C���[�ҋ@��
	void Goout(void);							// ���łɑł������̊֐�
	void DownAnim(void);						// ������A�j���[�V����
	void Collision(void);						// �����蔻��
	void Collision_After(void);					// ����������̏���
	void DownOne(void);							// �����鏈��
	void ConectDelete(CBullet * bulletOri);		// �A���폜
	void ConectDown(CBullet * bulletCom);		// �A������
	void Delete(void);							// �폜����
	void Dead(void);							// �Q�[���I�[�o�[
	void BoxToPlayer(void);						// �v���C���[�̈ʒu��
	bool Existence(								// ���݂��Ă��邩�ǂ���
		CBullet * bulletOri,		// �e�̔�r��
		CBullet*bulletCom,			// �e�̔�r��
		DIRECTION direction);		// ����
	bool BulletJudg(							// ��������
		CBullet * pBulletOrigin,	// �e�̔�r��
		CBullet * pBulletComp,		// �e�̔�r��
		CManager::PLAYER plyerid,	// �v���C���[�ԍ�
		CBullet::STATE state,		// �e�̏��
		bool bJubg					// ����
	);
	
	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex[TYPE_MAX];	// �e�N�X�`���[���
	static int m_nCntJudg;						// �Ȃ����Ă��鐔
	static int m_nCntIdMax;						// �i���o�[����
	/* ��l�p */
	static char m_cFileName[ONEPLAYER_STAGE][128];	// �t�@�C����
	static int m_nLineLoad[ONEPLAYER_STAGE][MAX_ARRAY_LINE * MAX_ARRAY_COLUMN];		// �s�ԍ��ۑ�
	static int m_nColumnLoad[ONEPLAYER_STAGE][MAX_ARRAY_LINE * MAX_ARRAY_COLUMN];	// ��ԍ��ۑ�
	static int m_nType[ONEPLAYER_STAGE][MAX_ARRAY_LINE * MAX_ARRAY_COLUMN];			// ��ޔԍ��ۑ�
	static int m_nAllLoad[ONEPLAYER_STAGE];	// �ǂݍ��ތ�
	static int m_nDownCntLoad[ONEPLAYER_STAGE];						// ������J�E���g
	static int m_nDownCnt[CManager::PLAYER_MAX];					// ������J�E���g
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_posgoal;						// �ڕW�n�_
	D3DXVECTOR3 m_posdif;						// �ڕW�n�_�ƌ��ݒn�̋���
	D3DXVECTOR3 m_move;							// �ړ���
	TYPE	m_type;								// �^�C�v
	STATE	m_state;							// ���
	CManager::PLAYER m_PlayerId;				// �v���C���[�ԍ�
	float	m_fRot;								// ��]��
	int		m_nLife;							// �̗�
	int		m_nLine;							// �s�ԍ�				
	int		m_nColumn;							// ��ԍ�
	int		m_nCntMove;							// �ړ��J�E���g
	bool	m_bJudg;							// ���肳�ꂽ���ǂ���
	bool	m_bDown;							// ��������̂��ǂ���
	bool	m_bMove;							// �e�������Ă��邩�ǂ���
	bool m_bShake;											// �h���J�E���g
};
#endif


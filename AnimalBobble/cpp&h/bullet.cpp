// ------------------------------------------------------------------------------------------
//
// �e�����̐���[bullet.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#include "bullet.h"
#include "effect.h"
#include "explosion.h"
#include "score.h"
#include "player.h"
#include "frame.h"
#include "array.h"
#include "box.h"
#include "fade.h"
#include "game.h"
#include "ceiling.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------
#define BULLETSIZE (50.0f)
#define BULLETSIX (D3DX_PI * 2 / CBullet::DIRECTION_MAX)
#define BULLETSIX_ONE (BULLETSIX / 2)
#define BULLET_CONNECT (3)
#define BULLET_MOVE (15)
#define BULLET_CNTMOVE (50)

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CBullet::m_pTex[CBullet::TYPE_MAX] = {};
int CBullet::m_nCntJudg = 0;
int CBullet::m_nCntIdMax = 0;
//----- ��l�p -----//
char CBullet::m_cFileName[ONEPLAYER_STAGE][128] =		// �t�@�C����
{
	"data/LOAD/Single/stage1.txt",
	"data/LOAD/Single/stage2.txt",
	"data/LOAD/Single/stage3.txt",
	"data/LOAD/Single/stage4.txt",
	"data/LOAD/Single/stage5.txt"
};
int CBullet::m_nLineLoad[ONEPLAYER_STAGE][MAX_ARRAY_LINE * MAX_ARRAY_COLUMN] = {};
int CBullet::m_nColumnLoad[ONEPLAYER_STAGE][MAX_ARRAY_LINE * MAX_ARRAY_COLUMN] = {};
int CBullet::m_nType[ONEPLAYER_STAGE][MAX_ARRAY_LINE * MAX_ARRAY_COLUMN] = {};
int CBullet::m_nAllLoad[ONEPLAYER_STAGE] = {};
int CBullet::m_nDownCntLoad[ONEPLAYER_STAGE] = { 10,10,10,10,10 };
int CBullet::m_nDownCnt[CManager::PLAYER_MAX] = {};

// ------------------------------------------------------------------------------------------
// �R���X�g���N�^����
// ------------------------------------------------------------------------------------------
CBullet::CBullet() : CScene_TWO::CScene_TWO(ACTOR_BULLET, LAYER_BULLET)
{
	//----- �ϐ��̏����� */
	// �ʒu
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// �ړI�ʒu
	m_posgoal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// �ړ���
	m_move = D3DXVECTOR3(BULLET_MOVE, BULLET_MOVE, 0.0f);
	// �^�C�v
	m_type = TYPE_BEE;
	// ���
	m_state = STATE_BOX;
	// ��]��
	m_fRot = D3DX_PI;
	// �̗�
	m_nLife = 50;
	// �s�ԍ�
	m_nLine = -1;
	// ��ԍ�
	m_nColumn = 9;
	// �ړ��J�E���g
	m_nCntJudg = 0;
	// �Ȃ����Ă��锻��
	m_bJudg = false;
	// �����Ă锻��
	m_bDown = false;
	// �ړ����Ă��邩�ǂ���
	m_bMove = false;
	// �v���C���[�ԍ�
	m_PlayerId = CManager::PLAYER_1;
}

// ------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ------------------------------------------------------------------------------------------
CBullet::~CBullet()
{
}

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void CBullet::Init(void)
{
	// �T�C�Y�ݒ�
	CScene_TWO::SetSize(D3DXVECTOR2(BULLETSIZE, BULLETSIZE));
	// ������
	CScene_TWO::Init();
	// ���݂̈ʒu�ݒ�
	CScene_TWO::SetPosition(m_pos);
	// �e�N�X�`���[�ݒ�
	CScene_TWO::BindTexture(m_pTex[m_type]);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void CBullet::Uninit(void)
{
	if (m_state == STATE_OUT || m_state == STATE_END)
	{
		m_nCntIdMax--;
	}
	CScene_TWO::Uninit();
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void CBullet::Update(void)
{
	// ��Ԃɂ����
	if (m_state == STATE_OUT)			// ���o��
	{
		Goout();
	}
	else if (m_state == STATE_PLAYER)	// �v���C���[�ҋ@��
	{
		Player();
	}
	else if (m_state == STATE_BOX)		// �{�b�N�X�ҋ@��
	{
		Box();
	}
	else if (m_state == STATE_END)		// �������A�j���[�V����
	{
		DownAnim();
	}
	// ���݂̈ʒu�ݒ�
	CScene_TWO::SetPosition(m_pos);
	// �X�V
	CScene_TWO::Update();
}

// ------------------------------------------------------------------------------------------
// �{�b�N�X�ҋ@��
// ------------------------------------------------------------------------------------------
void CBullet::Box(void)
{
}

// ------------------------------------------------------------------------------------------
// �v���C���[�ҋ@��
// ------------------------------------------------------------------------------------------
void CBullet::Player(void)
{
	// �ړ��J�E���g��20�����̏ꍇ
	if (m_nCntMove < BULLET_CNTMOVE)
	{
		m_pos.x += (m_posgoal.x - m_pos.x) / BULLET_CNTMOVE;	// �ڕW�n�_�ɋ߂Â�
		m_nCntMove++;								// �ړ��J�E���g�A�b�v
	}
	// �ړ��J�E���g��20�̏ꍇ
	else if (m_nCntMove == BULLET_CNTMOVE)
	{
		// �ϐ��錾
		CPlayer * pPlayer = NULL;	// �v���C���[
		// ���擾
		for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
		{
			if (pPlayer == NULL)	pPlayer = (CPlayer*)CScene::GetActor(ACTOR_PLAYER, LAYER_OBJ, nCntLayer);	// �v���C���[
		}
		pPlayer->ShotState(false);																				// �e�ł��Ă��邩�̏��
		m_pos = m_posgoal;																						// �ڕW�n�_�ɑ��
	}
}

// ------------------------------------------------------------------------------------------
// ���łɑł������̏ꍇ
// ------------------------------------------------------------------------------------------
void CBullet::Goout(void)
{
	// �ړ�
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	//----- �����蔻�� -----//
	Collision();
	// �폜����
	Delete();
	// �Q�[���I�[�o�[
	Dead();
	// ��i�����鏈��
	DownOne();
}

// ------------------------------------------------------------------------------------------
// ������A�j���[�V����
// ------------------------------------------------------------------------------------------
void CBullet::DownAnim(void)
{
	// �ϐ��錾
	CArray * pArray = NULL;	// �z��
	// �ړ���
	m_move.y += float(rand() % 10) / 10;
	// �ʒu�X�V
	m_pos.y += m_move.y;
	// ���擾
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_UI); nCntLayer++)
	{
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);	// �z��
	}

	if (m_pos.y > 700.0f)
	{
		pArray->DeleteState(m_nLine, m_nColumn);
		// �����[�X
		Release();
		return;
	}
}

// ------------------------------------------------------------------------------------------
// �����蔻��
// ------------------------------------------------------------------------------------------
void CBullet::Collision(void)
{
	// �ϐ��錾
	CBullet * pBullet = NULL;	// �e
	CFrame * pFrame = NULL;		// �t���[��
	CArray * pArray = NULL;		// �z��
	CPlayer * pPlayer = NULL;	// �v���C���[
	CCeiling * pCeiling = NULL;	// �V��

	// ���擾
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
	{
		if(pFrame == NULL) pFrame = (CFrame*)CScene::GetActor(ACTOR_FRAME, LAYER_OBJ, nCntLayer);			// �t���[��
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);			// �z��
		if (pPlayer == NULL)	pPlayer = (CPlayer*)CScene::GetActor(ACTOR_PLAYER, LAYER_OBJ, nCntLayer);	// �v���C���[
		if (pCeiling == NULL) pCeiling = (CCeiling*)CScene::GetActor(ACTOR_CEILING, LAYER_OBJ, nCntLayer);	// �V��
	}
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_UI); nCntLayer++)
	{
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);			// �z��
	}

	// �ړ���Ԃł����
	if (m_bMove)
	{
		//----- �A���m�̓����蔻��̏��� -----//
		for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
		{
			// ���擾
			pBullet = (CBullet*)CScene::GetActor(ACTOR_BULLET, LAYER_BULLET, nCnt);	// �e
																					// ��������
			if (BulletJudg(this, pBullet, m_PlayerId, STATE_OUT, false) == true)
			{
				// �~�̓����蔻��
				if (CCalculation::Collision_Circle(
					m_pos + D3DXVECTOR3(sinf(m_fRot) * GetLengh(), cosf(m_fRot) * GetLengh(), 0.0f),	// ���݂̃I�u�W�F�N�g�̈ʒu
					0,																					// ���݂̃I�u�W�F�N�g�̔��a
					pBullet->m_pos,																		// �ړI�̃I�u�W�F�N�g�̈ʒu
					GetLengh()))																		// �ړI�̃I�u�W�F�N�g�̔��a
				{
					// �k���`�F�b�N
					if (pArray != NULL)
					{
						// �߂��ʒu����Ԃ�
						m_pos = pArray->AroundPosCorrection(
							m_pos,					// �ʒu���
							pBullet->m_nLine,		// ���������e�̍s���
							pBullet->m_nColumn,		// ���������e�̗���
							&m_nLine,				// ������ɍs�����e�̍s���
							&m_nColumn);			// ������ɍs�����e�̗���
					}
					// ����������̏���
					Collision_After();
					// �֐��𔲂���
					return;
				}

			}
		}
		//----- �V��ƖA�̓����蔻�� -----//
		// �k���`�F�b�N
		if (pCeiling != NULL &&
			pCeiling->GetPlayerId() == m_PlayerId &&
			pArray != NULL)
		{
			// y�̌��E�l�𒴂����Ƃ�
			if (m_pos.y - CScene_TWO::GetSize().y / 2 <
				pCeiling->GetPos().y + pCeiling->GetSize().y)
			{
				// �߂��z��̈ʒu����Ԃ�
				m_pos = pArray->TopPosCorrection(m_pos, 0, &m_nColumn);
				// �s�͏����̂܂�
				m_nLine = 0;
				// ����������̏���
				Collision_After();
			}
		}
		// �k���`�F�b�N
		if (pFrame != NULL &&
			pFrame->GetPlayerId() == m_PlayerId)
		{
			// x�̌��E�l�𒴂����Ƃ�
			if (m_pos.x - CScene_TWO::GetSize().x / 2 <=
				pFrame->GetPos().x - pFrame->GetSize().x / 2 ||
				m_pos.x + CScene_TWO::GetSize().x / 2 >=
				pFrame->GetPos().x + pFrame->GetSize().x / 2)
			{
				m_move.x *= -1;
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// ����������̏���
// ------------------------------------------------------------------------------------------
void CBullet::Collision_After(void)
{
	// �ړ��ʂ�����������
	m_move.x = 0;
	m_move.y = 0;
	// ������I���ɂ���
	m_bJudg = true;
	// �ړ���Ԃ��I�t�ɂ���
	m_bMove = false;
	// �v���C���[�Ɉʒu���ړ�
	BoxToPlayer();
	// �J�E���g�A�b�v
	m_nCntJudg++;
	//----- �����鏈�� -----//
	ConectDelete(this);
	// ������ݒ菈��
	m_nDownCnt[m_PlayerId]++;
}

// ------------------------------------------------------------------------------------------
// �A���폜����
// ------------------------------------------------------------------------------------------
void CBullet::ConectDelete(CBullet * bulletCom)	// ��r�������
{
	// �ϐ��錾
	int Direction;		// ���݂̕���
	CBullet * pBullet;	// �e

	// �����w��
	Direction = DIRECTION_NORTHWEST;	// �k�̕���

	while (1)
	{
		//----- �ׂ荇�����I�u�W�F�N�g�����邩�Ȃ��� -----//
		for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
		{
			// ���擾
			pBullet = (CBullet*)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// �A
			// ��������
			if (BulletJudg(bulletCom, pBullet, bulletCom->m_PlayerId, STATE_OUT, false) &&					// �A�̏���
				pBullet->m_type == bulletCom->m_type &&												// �^�C�v
				Existence(pBullet, bulletCom, (DIRECTION)Direction))								// ���݂��Ă��邩�ǂ���
			{
				// �����锻��
				pBullet->m_bJudg = true;
				// �J�E���g�A�b�v
				m_nCntJudg++;
				//----- �A���폜���� -----//
				ConectDelete(pBullet);
			}
		}
		// �����C���N�������g
		Direction++;
		// �����S���m�F�����烋�[�v�𔲂���
		if (Direction == DIRECTION_MAX)
		{
			return;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �A����������
// ------------------------------------------------------------------------------------------
void CBullet::ConectDown(CBullet * bulletCom)
{
	// �ϐ��錾
	int Direction;		// ���݂̕���
	CBullet * pBullet;

	// �����w��
	Direction = DIRECTION_NORTHWEST;	// �k�̕���

	while (1)
	{
		// �ׂ荇�����I�u�W�F�N�g�����邩�Ȃ���
		for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
		{
			pBullet = (CBullet*)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// ���擾
																									// ��������
			if (BulletJudg(bulletCom, pBullet, bulletCom->m_PlayerId, STATE_OUT, false) &&					// �A�̏���
				Existence(pBullet, bulletCom, (DIRECTION)Direction))								// ���݂��Ă��邩�ǂ���
			{
				// �����锻��
				pBullet->m_bJudg = true;
				//----- �A���폜���� -----//
				ConectDown(pBullet);
			}
		}
		// �����C���N�������g
		Direction++;

		// �����S���m�F�����烋�[�v�𔲂���
		if (Direction == DIRECTION_MAX)
		{
			return;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �폜����
// ------------------------------------------------------------------------------------------
void CBullet::Delete(void)
{
	// �ϐ��錾
	CBullet * pBullet = NULL;	// ���
	CScore * pScore = NULL;	// �X�R�A
	CArray * pArray = NULL;	// �z��
	CManager::PLAYER plyerid = m_PlayerId;	// �v���C���[��ID
	bool bTopDelete = false;				// ��ɔz�u���Ă���A�������Ă��邩�ǂ���

	// ���擾
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
	{
		if (pScore == NULL)	pScore = (CScore*)CScene::GetActor(ACTOR_SCORE, LAYER_OBJ, nCntLayer);	// �X�R�A
	}
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_UI); nCntLayer++)
	{
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);	// �z��
	}
	//----- �A���폜 -----//
	for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
	{
		// ���擾
		pBullet = (CBullet*)GetActor(ACTOR_BULLET, LAYER_BULLET, nCnt);	// �A
		if (BulletJudg(nullptr, pBullet, plyerid, STATE_OUT, true))	// �A�̏���
		{
			if (m_nCntJudg >= BULLET_CONNECT)
			{
				// �G�t�F�N�g
				for (int nCntEffect = 0; nCntEffect < 10; nCntEffect++)
				{
					// �ϐ��錾
					CEffect *pEffect;	// effect
					D3DXCOLOR Nowcol;	// �o�����̐F
					D3DXCOLOR Goalcol;	// �����鎞�̐F
					float frot =		// �p�x
						rand() % 314 * 0.01f - rand() % 314 * 0.01f;	

					switch (m_type)
					{
						// �n�`
					case CBullet::TYPE_BEE:
						Nowcol = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
						Goalcol = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
						break;
						// �L
					case CBullet::TYPE_CAT:
						Nowcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						Goalcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
						break;
						// ��
					case CBullet::TYPE_FISH:
						Nowcol = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
						Goalcol = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f);
						break;
						// ��
					case CBullet::TYPE_HAWK:
						Nowcol = D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f);
						Goalcol = D3DXCOLOR(1.0f, 0.5f, 0.2f, 0.0f);
						break;
						// ��
					case CBullet::TYPE_SNAKE:
						Nowcol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						Goalcol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f);
						break;
					default:
						break;
					}
					pEffect = CEffect::Create(
						pBullet->GetPos(),	// �ʒu
						Nowcol,				// �J���[
						Goalcol,			// �ڕW�̃J���[
						frot,				// �����p�x
						float(rand() % 5),	// ����
						30.0f,				// �T�C�Y
						1.0f,				// �ڕW�T�C�Y
						40					// ���C�t(������)
					);
					pEffect->Fountain(frot, 50);
				}
				
				// ��ɔz�u���Ă���
				bTopDelete = true;
				pBullet->m_state = STATE_END;
				pArray->DeleteState(pBullet->m_nLine, pBullet->m_nColumn);
				// �����[�X
				pBullet->Release();
				// �X�R�A�ݒ�
				pScore->SetScore(100);
			}
			else
			{
				pBullet->m_bJudg = false;
			}
		}
	}
	//----- �A������ -----//
	if (bTopDelete)
	{
		//----- �A�������I�� -----//
		for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
		{
			// ���擾
			pBullet = (CBullet*)GetActor(ACTOR_BULLET, LAYER_BULLET, nCnt);	// �A
			// ��������
			if (BulletJudg(this, pBullet, plyerid, STATE_OUT, false) &&	// ��������
				pBullet->m_nLine == 0)	// �s0�̎�
			{
				// �Ȃ����Ă�����
				pBullet->m_bJudg = true;
				// �A����������
				ConectDown(pBullet);
			}
		}
		//----- �A������ -----//
		for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
		{
			// ���擾
			pBullet = (CBullet*)GetActor(ACTOR_BULLET, LAYER_BULLET, nCnt);	// �A
																			// ��������
			if (BulletJudg(nullptr, pBullet, plyerid, STATE_OUT, false))	// �A�̏���
			{
				// ���
				pBullet->m_state = STATE_END;
				// �X�R�A�ݒ�
				pScore->SetScore(100);
				// �J�E���g�A�b�v
				m_nCntJudg++;
			}
		}
	}
	//----- ���v�|�C���g -----//
	if (m_nCntJudg >= BULLET_CONNECT)
	{
		// �����鎞�̉�
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_DELETE);
		pScore->SetScore(500 * m_nCntJudg);
	}
	// �Ȃ���������������
	m_nCntJudg = 0;
}

// ------------------------------------------------------------------------------------------
// �����鏈��
// ------------------------------------------------------------------------------------------
void CBullet::DownOne(void)
{
	// �ϐ��錾
	CBullet * pBullet = NULL;	// �A
	CArray * pArray = NULL;		// �z��
	CCeiling * pCeiling = NULL;	// �v���C���[
	// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_OBJ); nCntLayer++)
	{
		if (pCeiling == NULL) pCeiling = (CCeiling*)CScene::GetActor(ACTOR_CEILING, LAYER_OBJ, nCntLayer);	// �V��
	}
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_UI); nCntLayer++)
	{
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);	// �z��
	}

	// ���o�����A�Ƃ�
	if (m_state == STATE_OUT)
	{
		// �h�炷���o
		if (m_nDownCnt[m_PlayerId] == m_nDownCntLoad[CManager::GetGame()->GetStage()] - 1 &&	// ������O
			!m_bMove)																			// �ړ����Ă��Ȃ��A
		{
			float frot = rand() % 314 * 0.01f - rand() % 314 * 0.01f;
			m_pos.x = pArray->GetPos(m_nLine, m_nColumn).x + sinf(frot) * 2.0f;
			m_pos.y = pArray->GetPos(m_nLine, m_nColumn).y + cosf(frot) * 2.0f;
		}
		// ������
		if (m_nDownCnt[m_PlayerId] == m_nDownCntLoad[CManager::GetGame()->GetStage()])			// �����鎞
		{
			// �����鎞�̉�
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_DOWN);
			// �����ʒu������ɉ�����
			pArray->OriginPosChange();
			// �V���������
			pCeiling->SetPosSize(
				pCeiling->GetPos() +
				D3DXVECTOR3(0.0f, BULLETSIZE / 2, 0.0f),
				pCeiling->GetSize() + D3DXVECTOR2(0.0, BULLETSIZE / 2));

			//----- �Ĕz�u -----//
			for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
			{
				// ���擾
				pBullet = (CBullet*)GetActor(ACTOR_BULLET, LAYER_BULLET, nCnt);	// �A
																				// ��������
				if (BulletJudg(nullptr, pBullet, m_PlayerId, STATE_OUT, false))
				{
					// �Ĕz�u
					pBullet->m_pos = pArray->TargetPosCorrection(pBullet->m_nLine, pBullet->m_nColumn);
				}
			}
			// �J�E���g��������
			m_nDownCnt[m_PlayerId] = 0;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �Q�[���I�[�o�[����
// ------------------------------------------------------------------------------------------
void CBullet::Dead(void)
{
	// �ϐ��錾
	CArray * pArray = NULL;	// �z��

	// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_OBJ); nCntLayer++)
	{
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);	// �z��
	}

	// ����ɒB������
	if (m_nLine == MAX_ARRAY_LINE - 1 - pArray->GetDownCnt())
	{
		if (m_state != STATE_END)
		{
			// �Q�[�����[�h
			if (CManager::GetMode() == CManager::MODE_GAME)
			{
				// �Q�[���I�[�o�[�W���Ȃ��ꍇ
				if (CManager::GetGame()->GetState() != CGame::STATE_GAMEOVER)
				{
					CManager::GetGame()->SetState(CGame::STATE_GAMEOVER);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// ������v���C���[�֏���
// ------------------------------------------------------------------------------------------
void CBullet::BoxToPlayer(void)
{
	// �ϐ��錾
	CBox *pBox = NULL;			// ��
	CPlayer * pPlayer = NULL;	// �v���C���[
	// ���擾
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
	{
		if (pBox == NULL) pBox = (CBox*)CScene::GetActor(ACTOR_BOX, LAYER_OBJ, nCntLayer);				// ��
		if (pPlayer == NULL) pPlayer = (CPlayer*)CScene::GetActor(ACTOR_PLAYER, LAYER_OBJ, nCntLayer);	// �v���C���[
	}

	//----- ������v���C���[�ւ̈ړ� -----//
	for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
	{
		// ���擾
		CBullet *pBullet = (CBullet *)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// �e
		// �k���`�F�b�N
		if (pBullet != NULL)
		{
			// ������v���C���[��
			if (pBullet->GetPlayerId() == m_PlayerId &&
				pBullet->GetState() == CBullet::STATE_BOX)
			{
				pBullet->SetPosGoal(pPlayer->GetPos());				// �ʒu�ݒ�
				pBullet->SetState(CBullet::STATE_PLAYER);	// ��Ԑݒ�
			}
		}
	}
	// �e����
	CBullet::Create(m_PlayerId, pBox->GetPos(), (CBullet::TYPE)(rand() % (int)CBullet::TYPE_MAX));

	//----- ���̃A�j���[�V�����p�^�[�� -----//
	// �����̃A�j���[�V�����p�^�[���ݒ�
	pBox->SetVirticalAnim(1);
	// �����̃A�j���[�V�����p�^�[���ݒ�
	pBox->SetHorizonAnim(1);
}

// ------------------------------------------------------------------------------------------
// ���݂��邩���Ȃ�������
// ------------------------------------------------------------------------------------------
bool CBullet::Existence(CBullet * bulletOri, CBullet * bulletCom, DIRECTION direction)
{
	// �ϐ��錾
	bool bExist = false;	// ���݂��Ă��邩���Ȃ���

	//----- �����ɉ����� -----//
	switch (direction)
	{
		// �k��
	case DIRECTION_NORTHWEST:
		// �񂪋�����
		if (bulletCom->m_nLine % 2 == 0)
		{
			// �z��ԍ����ꏏ��������
			if (bulletOri->m_nLine == bulletCom->m_nLine - 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn - 1)
			{
				// ���݂��Ă���
				bExist = true;
			}
		}
		// �񂪊��
		else
		{
			// �z��ԍ����ꏏ��������
			if (bulletOri->m_nLine == bulletCom->m_nLine - 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn)
			{
				// ���݂��Ă���
				bExist = true;
			}
		}
		break;
		// �k��
	case DIRECTION_NORTHEAST:
		// �񂪋�����
		if (bulletCom->m_nLine % 2 == 0)
		{
			// �z��ԍ����ꏏ��������
			if (bulletOri->m_nLine == bulletCom->m_nLine - 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn)
			{
				// ���݂��Ă���
				bExist = true;
			}
		}
		// �񂪊��
		else
		{
			// �z��ԍ����ꏏ��������
			if (bulletOri->m_nLine == bulletCom->m_nLine - 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn + 1)
			{
				// ���݂��Ă���
				bExist = true;
			}
		}
		break;
		// ��
	case DIRECTION_EAST:
		// �z��ԍ����ꏏ��������
		if (bulletOri->m_nLine == bulletCom->m_nLine &&
			bulletOri->m_nColumn == bulletCom->m_nColumn + 1)
		{
			// ���݂��Ă���
			bExist = true;
		}

		break;
		// �쓌
	case DIRECTION_SOUTHEAST:
		// �񂪋�����
		if (bulletCom->m_nLine % 2 == 0)
		{
			// �z��ԍ����ꏏ��������
			if (bulletOri->m_nLine == bulletCom->m_nLine + 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn)
			{
				// ���݂��Ă���
				bExist = true;
			}
		}
		// �񂪊��
		else
		{
			// �z��ԍ����ꏏ��������
			if (bulletOri->m_nLine == bulletCom->m_nLine + 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn + 1)
			{
				// ���݂��Ă���
				bExist = true;
			}
		}
		break;
		// �쐼
	case DIRECTION_SOUTHWEST:
		// �񂪋�����
		if (bulletCom->m_nLine % 2 == 0)
		{
			// �z��ԍ����ꏏ��������
			if (bulletOri->m_nLine == bulletCom->m_nLine + 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn - 1)
			{
				// ���݂��Ă���
				bExist = true;
			}
		}
		// �񂪊��
		else
		{
			// �z��ԍ����ꏏ��������
			if (bulletOri->m_nLine == bulletCom->m_nLine + 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn)
			{
				// ���݂��Ă���
				bExist = true;
			}
		}
		break;
		// ��
	case DIRECTION_WEST:
		// �z��ԍ����ꏏ��������
		if (bulletOri->m_nLine == bulletCom->m_nLine &&
			bulletOri->m_nColumn == bulletCom->m_nColumn - 1)
		{
			// ���݂��Ă���
			bExist = true;
		}
		break;

	default:
		break;
	}
	return bExist;
}

// ------------------------------------------------------------------------------------------
// �������菈��
// ------------------------------------------------------------------------------------------
bool CBullet::BulletJudg(
	CBullet * pBulletOrigin,	// �e�̔�r��
	CBullet * pBulletComp,		// �e�̔�r��
	CManager::PLAYER plyerid,	// �v���C���[�ԍ�
	CBullet::STATE state,		// �e�̏��
	bool bJubg					// ����
)
{
	// ����
	if (pBulletComp != NULL &&					// �k���`�F�b�N
		pBulletComp != pBulletOrigin &&			// �v���C���[�ԍ�
		pBulletComp->m_PlayerId == plyerid &&	// �v���C���[�ԍ�
		pBulletComp->m_state == state &&		// ���
		pBulletComp->m_bJudg == bJubg)			// �Ȃ����Ă��Ȃ���Ԃ̎�
	{
		return true;
	}
	return false;
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void CBullet::Draw(void)
{
	CScene_TWO::Draw();
}

// ------------------------------------------------------------------------------------------
// �ʒu���ݒ菈��
// ------------------------------------------------------------------------------------------
void CBullet::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
	m_posgoal = pos;
}

// ------------------------------------------------------------------------------------------
// �ڕW�n�_�̐ݒ菈��
// ------------------------------------------------------------------------------------------
void CBullet::SetPosGoal(D3DXVECTOR3 posgoal)
{
	m_posgoal = posgoal;
	m_posdif = (m_posgoal - m_pos) / 2 + m_pos;
}

// ------------------------------------------------------------------------------------------
// ��Ԑݒ菈��
// ------------------------------------------------------------------------------------------
void CBullet::SetRot(float frot)
{
	// �ړ��ʂ̐ݒ�
	m_move.x *= sinf(frot);
	m_move.y *= cosf(frot);
}

// ------------------------------------------------------------------------------------------
// ��Ԑݒ菈��
// ------------------------------------------------------------------------------------------
void CBullet::SetState(STATE state)
{
	// ��Ԑݒ�
	m_state = state;
	// �ړ���Ԃ�
	m_bMove = true;
	// �e�̃X�e�[�^�X���o�͂�����
	if (m_state == STATE_OUT)
	{
		// �������Z
		m_nCntIdMax++;
	}
}

// ------------------------------------------------------------------------------------------
// �v���C���[�ԍ��擾����
// ------------------------------------------------------------------------------------------
CManager::PLAYER CBullet::GetPlayerId(void)
{
	return m_PlayerId;
}

// ------------------------------------------------------------------------------------------
// ��Ԏ擾����
// ------------------------------------------------------------------------------------------
CBullet::STATE CBullet::GetState(void)
{
	return m_state;
}

// ------------------------------------------------------------------------------------------
// �ʒu���擾����
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 CBullet::GetPos(void)
{
	return m_pos;
}

// ------------------------------------------------------------------------------------------
// �ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
HRESULT CBullet::Load(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =					// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	char cTex[CBullet::TYPE_MAX][128] =			// �e�N�X�`���[��
	{
		"data/TEXTURE/Bullet/Bee.png",
		"data/TEXTURE/Bullet/Cat.png",
		"data/TEXTURE/Bullet/Fish.png",
		"data/TEXTURE/Bullet/Hawk.png",
		"data/TEXTURE/Bullet/Snake.png",
	};

	// �e�N�X�`���[�ݒ�
	for (int nCnt = 0; nCnt < CBullet::TYPE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice, cTex[nCnt], &m_pTex[nCnt]);
	}

	// �X�e�[�W�ǂݍ���
	LoadStage();

	return S_OK;
}

// ------------------------------------------------------------------------------------------
// �ǂݍ��񂾏���j��
// ------------------------------------------------------------------------------------------
void CBullet::UnLoad(void)
{
	for (int nCnt = 0; nCnt < CBullet::TYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̊J��
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �e�̑�������
// ------------------------------------------------------------------------------------------
int CBullet::GetAllBullet(void)
{
	return m_nCntIdMax;
}

// ------------------------------------------------------------------------------------------
// �쐬����
// ------------------------------------------------------------------------------------------
CBullet * CBullet::Create(CManager::PLAYER PlayerId, D3DXVECTOR3 pos, TYPE type)
{
	// �ϐ��錾
	CBullet * pBullet;		// �e2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pBullet = new CBullet();
	// ���݂̈ʒu
	pBullet->m_pos = pos;
	pBullet->m_posgoal = pos;
	// �^�C�v�̐ݒ�
	pBullet->m_type = type;
	// �v���C���[�ԍ�
	pBullet->m_PlayerId = PlayerId;
	// ����������
	pBullet->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pBullet;
}

// ------------------------------------------------------------------------------------------
// �ǂݍ��ݍ쐬����
// ------------------------------------------------------------------------------------------
CBullet * CBullet::LoadCreate(CManager::PLAYER PlayerId, int nMode, int nStage)
{
	// �ϐ��錾
	CBullet * pBullet = NULL;	// �e2D�N���X
	CArray * pArray = NULL;		// ��N���X
	CFrame * pFrame = NULL;		// �t���[���N���X
	CCeiling * pCeiling = NULL;	// �V��N���X

	// ���擾
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
	{
		if (pFrame == NULL) pFrame = (CFrame*)CScene::GetActor(ACTOR_FRAME, LAYER_OBJ, nCntLayer);			// �t���[��
		if (pCeiling == NULL) pCeiling = (CCeiling*)CScene::GetActor(ACTOR_CEILING, LAYER_OBJ, nCntLayer);	// �V��
	}
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_UI); nCntLayer++)
	{
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);	// �z��
	}

	// �������ꂽ�Ƃ��̏����l�ɕύX
	pArray->BiginPos();
	// �k���`�F�b�N
	if (pCeiling != NULL)
	{
		pCeiling->SetPosSize(D3DXVECTOR3(pFrame->GetPos().x, pFrame->GetPos().y - pFrame->GetSize().y / 2, 0.0f), D3DXVECTOR2(pFrame->GetSize().x / 2, 1.0f));
	}
	// 
	m_nDownCnt[PlayerId] = 0;
	m_nCntIdMax = 0;

	for (int nCntBullet = 0; nCntBullet < m_nAllLoad[nStage]; nCntBullet++)
	{
		// �������̐���(����->��{�N���X,��->�h���N���X)
		pBullet = new CBullet();
		// �^�C�v�̐ݒ�
		pBullet->m_type = (CBullet::TYPE)m_nType[nStage][nCntBullet];
		// �v���C���[�ԍ�
		pBullet->m_PlayerId = PlayerId;
		// �s�ݒ�
		pBullet->m_nLine = m_nLineLoad[nStage][nCntBullet];
		// ��ݒ�
		pBullet->m_nColumn = m_nColumnLoad[nStage][nCntBullet];
		// �ʒu�ݒ�
		pBullet->m_pos = pArray->GetPos(pBullet->m_nLine, pBullet->m_nColumn);
		// �ʒu�ݒ�
		pBullet->m_posgoal = pBullet->m_pos;
		// �ړ��ʐݒ�
		pBullet->m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// ��Ԑݒ�
		pBullet->m_state = STATE_OUT;
		// ����������
		pBullet->Init();
		// �߂��ʒu����Ԃ�
		pArray->SetState(pBullet->m_nLine, pBullet->m_nColumn, true);
		// �������Z
		m_nCntIdMax++;
	}
	// ���������I�u�W�F�N�g��Ԃ�
	return NULL;
}

// ------------------------------------------------------------------------------------------
// �X�e�[�W�ǂݍ���
// ------------------------------------------------------------------------------------------
void CBullet::LoadStage(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntLoad = 0;		// �J�E���g���[�h
	int nCntBullet = 0;		// �J�E���g�e
	char cReadText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

							// �X�e�[�W�J�E���g
	for (int nCntStage = 0; nCntStage < ONEPLAYER_STAGE; nCntStage++)
	{
		m_nAllLoad[nCntStage] = 0;

		// �t�@�C���J
		pFile = fopen(m_cFileName[nCntStage], "r");

		// �J����
		if (pFile != NULL)
		{
			// �X�N���v�g������܂Ń��[�v
			while (strcmp(cHeadText, "SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);	// �ꕶ��ǂݍ���
				sscanf(cReadText, "%s", &cHeadText);		// ��r�p�e�N�X�g�ɕ�������
			}

			// �X�N���v�g��������
			if (strcmp(cHeadText, "SCRIPT") == 0)
			{
				// �G���h�X�N���v�g������܂Ń��[�v
				while (strcmp(cHeadText, "END_SCRIPT") != 0)
				{
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);

					// �Z�b�g��������
					if (strcmp(cHeadText, "BULLETSET") == 0)
					{
						// �G���h�Z�b�g������܂Ń��[�v
						while (strcmp(cHeadText, "END_BULLETSET") != 0)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);

							// ��ޏ��ǂݍ���
							if (strcmp(cHeadText, "TYPE") == 0)
							{
								sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_nType[nCntStage][m_nAllLoad[nCntStage]]);
							}

							// �s���ǂݍ���
							else if (strcmp(cHeadText, "LINE") == 0)
							{
								sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_nLineLoad[nCntStage][m_nAllLoad[nCntStage]]);
							}

							// ����ǂݍ���
							else if (strcmp(cHeadText, "COLUMN") == 0)
							{
								sscanf(cReadText, "%s %s %d",
									&cDie, &cDie,
									&m_nColumnLoad[nCntStage][m_nAllLoad[nCntStage]]);
							}

						}

						// �������J�E���g�A�b�v
						m_nAllLoad[nCntStage]++;
					}
				}
			}
			// �t�@�C����
			fclose(pFile);
		}

		// �J���Ȃ�
		else
		{
		}
	}
}

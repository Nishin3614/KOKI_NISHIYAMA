// ----------------------------------------
//
// �V�[�������̐���[player.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "player.h"
#include "bullet.h"
#include "box.h"
#include "effect.h"
#include "game.h"
#include "debugproc.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define PLAYER_ROTMOVE (0.01f)
#define PLAYER_SIZE (100.0f)
#define PLAYER_TEX "data/TEXTURE/Player/BobChanAnimation.png"
#define PLAYER_ANIMCOUNT (4)
#define PLAYER_HORIZONANIM (1.0f/PLAYER_ANIMCOUNT)
#define PLAYER_VERTICALANIM (1.0f/PLAYER_ANIMCOUNT)
#define PLAYER_SHOTCOUNT (10)									// �v���C���[�̔��˃J�E���g

// ----------------------------------------
//
// �O���[�o���ϐ�
//
// ----------------------------------------

// ----------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------
LPDIRECT3DTEXTURE9 CPlayer::m_pTex = NULL;

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CPlayer::CPlayer() : CScene_TWO::CScene_TWO(ACTOR_PLAYER,LAYER_OBJ)
{
	/* �ϐ��̏����� */
	// ��]��
	m_rot = 0.0f;
	// �e���
	m_bShot = false;
	// �����̃A�j���[�V�����p�^�[��
	m_nHorizonAnim = 0;
	// �����̃A�j���[�V�����p�^�[��
	m_nVirticalAnim = 0;
	// �A�j���[�V�����J�E���g
	m_nCntAnim = 0;
	// ���˃J�E���g
	m_nShotCnt = PLAYER_SHOTCOUNT;
	// ���˃^�C��
	m_nShotTime = 0;
	// �v���C���[�ԍ�
	m_PlayerId = CManager::PLAYER_1;		
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CPlayer::~CPlayer()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CPlayer::Init(void)
{
	// �ϐ��錾
	CBullet *pBullet = NULL;	// �e
	CBox *pBox = NULL;	// ��
	// �T�C�Y�ݒ�
	CScene_TWO::SetSize(D3DXVECTOR2(PLAYER_SIZE, PLAYER_SIZE));
	// ������
	CScene_TWO::Init();
	// �ʒu�ݒ�
	CScene_TWO::SetPosition(m_pos);
	// �e�N�X�`���[�ݒ�
	CScene_TWO::BindTexture(m_pTex);
	// ���_�J���[�ݒ�
	CScene_TWO::SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	// �\���e�N�X�`���[�ݒ�
	CScene_TWO::SetTex(
		D3DXVECTOR2(0.0f, 0.0f), 
		D3DXVECTOR2(0.25f, 0.25f));
	// �e�̐���
	pBullet = CBullet::Create(m_PlayerId,m_pos, (CBullet::TYPE)(rand() % (int)CBullet::TYPE_MAX));
	// �v���C���[�̈ʒu��
	pBullet->SetPos(m_pos);																// �ʒu�ݒ�
	pBullet->SetState(CBullet::STATE_PLAYER);											// ��Ԑݒ�
	// ���擾
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
	{
		if (pBox == NULL)	pBox = (CBox*)CScene::GetActor(ACTOR_BOX, LAYER_OBJ, nCntLayer);	// ��
	}

	// �e�̐���
	pBullet = CBullet::Create(m_PlayerId,pBox->GetPos(), (CBullet::TYPE)(rand() % (int)CBullet::TYPE_MAX));
	// �\�����̐���
	m_pprediction = CPrediction::Create(m_PlayerId, m_pos, CPrediction::TYPE_BEE);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CPlayer::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CPlayer::Update(void)
{
	// �ϐ��錾
	D3DXVECTOR3 pos;
	/* �R���g���[���[�p�ϐ� */
	int nValueH = 0;		// ��������
	int nValueV = 0;		// ��������
	CKeyboard * pKeyboard;	// �L�[�{�[�h���
	CJoypad * pJoypad;		// �W���C�p�b�h���
							
	// ���擾
	pKeyboard = CManager::GetKeyboard();	// �L�[�{�[�h
	pJoypad = CManager::GetJoy();		   	// �W���C�p�b�h
	// ���݂̈ʒu���擾
	pos = CScene_TWO::GetPosition();

	/*
	pos = D3DXVECTOR3(
		(float)CManager::GetMouse()->GetX(),
		(float)CManager::GetMouse()->GetY(),
		0.0f);
	*/

	// �R���g���[���[�̍��X�e�B�b�N�̏��擾
	pJoypad->GetStickLeft(0, &nValueH, &nValueV);
	/*
	// ���X�e�B�b�N�𓮂����Ă��邩�ǂ���
	if (nValueH != 0 || nValueV != 0)
	{
		// �p�x
		m_rot = -atan2f((float)nValueH, (float)nValueV);
	}
	*/
	if (CManager::GetGame()->GetState() == CGame::STATE_NORMAL)
	{
		// �v���C���[�ԍ��P
		if (m_PlayerId == CManager::PLAYER_1)
		{
			/* ��] */
			// ��
			if (pKeyboard->GetKeyboardPress(DIK_A))
			{
				m_rot += PLAYER_ROTMOVE;
			}
			// �E
			else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_D))
			{
				m_rot -= PLAYER_ROTMOVE;
			}

			/* ��]�ʐ��� */
			if (m_rot >= D3DX_PI / 2 - 0.1f)
			{
				m_rot = -D3DX_PI / 2 + 0.2f;
			}
			else if (m_rot <= -D3DX_PI / 2 + 0.1f)
			{
				m_rot = D3DX_PI / 2 - 0.2f;
			}


			/* �e���� */
			// �e��Ԃ�OFF�̏ꍇ
			if (!m_bShot)
			{
				if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) ||
					pJoypad->GetTrigger(0, pJoypad->KEY_B))
				{
					// �����鎞�̉�
					CManager::GetSound()->PlaySound(CSound::LABEL_SE_THROW);

					for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
					{
						// ���擾
						CBullet *pBullet = (CBullet *)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// �e
						// �k���`�F�b�N
						if (pBullet != NULL)
						{
							if (pBullet->GetPlayerId() == m_PlayerId)
							{
								// �v���C���[����̕��o
								if (pBullet->GetState() == CBullet::STATE_PLAYER)
								{
									pBullet->SetState(CBullet::STATE_OUT);		// ��Ԑݒ�
									pBullet->SetRot(D3DX_PI + m_rot);			// ��]���ݒ�
								}
							}
						}
					}
					// �e��Ԃ�ON�ɂ���
					m_bShot = true;
					// �����̃A�j���[�V�����p�^�[��
					m_nVirticalAnim = 2;
					// ���˃J�E���g������
					m_nShotCnt = PLAYER_SHOTCOUNT;

				}
				else if (m_nShotCnt <= 0)
				{
					for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
					{
						// ���擾
						CBullet *pBullet = (CBullet *)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// �e
																															// �k���`�F�b�N
						if (pBullet != NULL)
						{
							if (pBullet->GetPlayerId() == m_PlayerId)
							{
								// �v���C���[����̕��o
								if (pBullet->GetState() == CBullet::STATE_PLAYER)
								{
									pBullet->SetState(CBullet::STATE_OUT);		// ��Ԑݒ�
									pBullet->SetRot(D3DX_PI + m_rot);			// ��]���ݒ�
								}
							}
						}
					}
					// �e��Ԃ�ON�ɂ���
					m_bShot = true;
					// �����̃A�j���[�V�����p�^�[��
					m_nVirticalAnim = 2;
					// ���˃J�E���g������
					m_nShotCnt = PLAYER_SHOTCOUNT;
				}
			}
		}
		// �v���C���[�ԍ��P
		if (m_PlayerId == CManager::PLAYER_2)
		{
			/* ��] */
			// ��
			if (pKeyboard->GetKeyboardPress(DIK_LEFT))
			{
				m_rot += PLAYER_ROTMOVE;
			}
			// �E
			else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_RIGHT))
			{
				m_rot -= PLAYER_ROTMOVE;
			}

			/* ��]�ʐ��� */
			if (m_rot >= D3DX_PI / 2 - 0.1f)
			{
				m_rot = -D3DX_PI / 2 + 0.2f;
			}
			else if (m_rot <= -D3DX_PI / 2 + 0.1f)
			{
				m_rot = D3DX_PI / 2 - 0.2f;
			}

			/* �e���� */
			// �e��Ԃ�OFF�̏ꍇ
			if (!m_bShot)
			{
				if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) ||
					pJoypad->GetTrigger(0, pJoypad->KEY_B))
				{
					// �����鎞�̉�
					CManager::GetSound()->PlaySound(CSound::LABEL_SE_THROW);
					for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
					{
						// ���擾
						CBullet *pBullet = (CBullet *)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// �e
																															// �k���`�F�b�N
						if (pBullet != NULL)
						{
							if (pBullet->GetPlayerId() == m_PlayerId)
							{
								// �v���C���[����̕��o
								if (pBullet->GetState() == CBullet::STATE_PLAYER)
								{
									pBullet->SetState(CBullet::STATE_OUT);		// ��Ԑݒ�
									pBullet->SetRot(D3DX_PI + m_rot);			// ��]���ݒ�
								}
							}
						}
					}
					// �e��Ԃ�ON�ɂ���
					m_bShot = true;
					// �����̃A�j���[�V�����p�^�[��
					m_nVirticalAnim = 2;
					// ���˃J�E���g������
					m_nShotCnt = PLAYER_SHOTCOUNT;
				}
				else if (m_nShotCnt <= 0)
				{
					for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
					{
						// ���擾
						CBullet *pBullet = (CBullet *)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// �e
																															// �k���`�F�b�N
						if (pBullet != NULL)
						{
							if (pBullet->GetPlayerId() == m_PlayerId)
							{
								// �v���C���[����̕��o
								if (pBullet->GetState() == CBullet::STATE_PLAYER)
								{
									pBullet->SetState(CBullet::STATE_OUT);		// ��Ԑݒ�
									pBullet->SetRot(D3DX_PI + m_rot);			// ��]���ݒ�
								}
							}
						}
					}
					// �e��Ԃ�ON�ɂ���
					m_bShot = true;
					// �����̃A�j���[�V�����p�^�[��
					m_nVirticalAnim = 2;
					// ���˃J�E���g������
					m_nShotCnt = PLAYER_SHOTCOUNT;
				}
			}
		}

		// ���˃^�C���A�b�v
		m_nShotTime++;
		// ���˃J�E���g�_�E��
		if (m_nShotTime % 60 == 0)
		{
			m_nShotCnt--;
		}
	}
	// �\�����̉�]�ݒ�
	m_pprediction->SetRot(m_rot);
	// �ʒu�ݒ�
	CScene_TWO::SetPosition(pos);
	// ��]�ݒ�
	CScene_TWO::SetRot(m_rot);
	// �A�j���[�V�����ω�
	Animation();
	// �X�V
	CScene_TWO::Update();
}

// ----------------------------------------
// �A�j���[�V��������
// ----------------------------------------
void CPlayer::Animation(void)
{
	// �A�j���[�V�����J�E���g�A�b�v
	m_nCntAnim++;
	// 20�t���[�����A�j���[�V��������
	if (m_nCntAnim % 20 == 0)
	{
		// �A�j���[�V�����ω�
		m_nHorizonAnim = (m_nHorizonAnim + 1) % PLAYER_ANIMCOUNT;
	}
	// ���_�e�N�X�`���[�ݒ�
	CScene_TWO::SetTex(
		D3DXVECTOR2(m_nHorizonAnim * PLAYER_HORIZONANIM, m_nVirticalAnim * PLAYER_VERTICALANIM),
		D3DXVECTOR2(m_nHorizonAnim * PLAYER_HORIZONANIM + PLAYER_HORIZONANIM, m_nVirticalAnim * PLAYER_VERTICALANIM + PLAYER_VERTICALANIM));
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CPlayer::Draw(void)
{
	CScene_TWO::Draw();
}

// ----------------------------------------
// �e�̏�ԏ���
// ----------------------------------------
void CPlayer::ShotState(bool bShot)
{
	// ���
	m_bShot = bShot;
	// �����̃A�j���[�V�����p�^�[��
	m_nVirticalAnim = 1;
}

// ----------------------------------------
// �v���C���[�ԍ��擾����
// ----------------------------------------
CManager::PLAYER CPlayer::GetPlayerId(void)
{
	return m_PlayerId;
}

// ----------------------------------------
// �ʒu���擾����
// ----------------------------------------
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return GetPosition();
}

// ----------------------------------------
// �e�̔��˃J�E���g����
// ----------------------------------------
int CPlayer::GetShotCnt(void)
{
	return m_nShotCnt;
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CPlayer::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice =		
		CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���[�ݒ�
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEX, &m_pTex);
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CPlayer::UnLoad(void)
{
	// �e�N�X�`���̊J��
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CPlayer * CPlayer::Create(CManager::PLAYER playerId, D3DXVECTOR3 pos)
{
	// �ϐ��錾
	CPlayer * pPlayer;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pPlayer = new CPlayer();
	// �ʒu���
	pPlayer->m_pos = pos;
	// �v���C���[�ԍ�
	pPlayer->m_PlayerId = playerId;
	// ����������
	pPlayer->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pPlayer;
}
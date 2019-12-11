// ----------------------------------------
//
// �v���C�pUI�����̐���[play_element_ui.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "play_element_ui.h"
#include "fade.h"
#include "mark.h"
#include "game.h"
#include "number.h"
#include "score.h"
#include "box.h"
#include "player.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------

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
LPDIRECT3DTEXTURE9 CPlay_element_ui::m_pTex[PLAY_ELEMENT_UI_MAX] = {};
D3DXVECTOR3 CPlay_element_ui::m_pos[PLAY_ELEMENT_UI_MAX] = {};
D3DXVECTOR2 CPlay_element_ui::m_size[PLAY_ELEMENT_UI_MAX] = {};

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CPlay_element_ui::CPlay_element_ui() : CScene::CScene(ACTOR_PLAYER_ELEMENT_UI, LAYER_UI)
{
	// ������
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}
	playerID = CManager::PLAYER_1;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CPlay_element_ui::~CPlay_element_ui()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CPlay_element_ui::Init(void)
{
	// �ϐ��錾
	CScore * pScore = NULL;
	CBox * pBox = NULL;			// ��
	CPlayer * pPlayer = NULL;	// �v���C���[
	// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_OBJ); nCntLayer++)
	{
		if (pScore == NULL) pScore = (CScore*)CScene::GetActor(CScene::ACTOR_SCORE, CScene::LAYER_OBJ, nCntLayer);	// �X�R�A
		if (pBox == NULL) pBox = (CBox*)CScene::GetActor(ACTOR_BOX, LAYER_OBJ, nCntLayer);							// ��
		if (pPlayer == NULL) pPlayer = (CPlayer*)CScene::GetActor(ACTOR_PLAYER, LAYER_OBJ, nCntLayer);				// �v���C���[
	}

	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = new CScene_TWO;
		m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
		m_aScene_Two[nCnt]->Init();
		m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);

		// �X�R�A�̈ʒu
		if (nCnt == PLAY_ELEMENT_UI_SCORE)
		{
			// �I�����[�h
			if (CManager::GetMode() == CManager::MODE_GAME)
			{
				m_aScene_Two[nCnt]->SetPosition(
					pScore->GetPos() + D3DXVECTOR3(0.0f,-10.0,0.0f));
			}
		}
		// �^�C���̈ʒu
		else if (nCnt == PLAY_ELEMENT_UI_TIME)
		{
			// �I�����[�h
			if (CManager::GetMode() == CManager::MODE_GAME)
			{
				// �ʒu���ݒ�
				m_aScene_Two[nCnt]->SetPosition(pBox->GetPos() + D3DXVECTOR3(0.0f, -100.0f, 0.0f));
				// �^�C���@
				m_pNumber = CNumber::Create();
				m_pNumber->SetNum(pPlayer->GetShotCnt());
				m_pNumber->SetPos(m_aScene_Two[nCnt]->GetPosition(),30.0f);
				m_pNumber->SetTex(CNumber::TEX_SCORE);
				m_pNumber->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}
	}
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CPlay_element_ui::Uninit(void)
{
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt]->Uninit();
		delete m_aScene_Two[nCnt];
		m_aScene_Two[nCnt] = NULL;
	}
	// �ԍ��I������
	if (m_pNumber != NULL)
	{
		m_pNumber->Uninit();
		delete m_pNumber;
		m_pNumber = NULL;
	}
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CPlay_element_ui::Update(void)
{
	// �ϐ��錾
	CPlayer * pPlayer = NULL;	// �v���C���[
	// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_OBJ); nCntLayer++)
	{
		if (pPlayer == NULL) pPlayer = (CPlayer*)CScene::GetActor(ACTOR_PLAYER, LAYER_OBJ, nCntLayer);				// �v���C���[
	}
	// �X�V
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		// ��ނɂ���ď�����ς���
		m_aScene_Two[nCnt]->Update();
	}
	//�ԍ��ݒ�
	if (pPlayer->GetShotCnt() <= 5)
	{
		m_pNumber->SetNum(pPlayer->GetShotCnt());
		m_pNumber->Update();
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CPlay_element_ui::Draw(void)
{
	// �ϐ��錾
	CPlayer * pPlayer = NULL;	// �v���C���[
	// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_OBJ); nCntLayer++)
	{
		if (pPlayer == NULL) pPlayer = (CPlayer*)CScene::GetActor(ACTOR_PLAYER, LAYER_OBJ, nCntLayer);	// �v���C���[
	}

	// �X�R�A��UI
	m_aScene_Two[CPlay_element_ui::PLAY_ELEMENT_UI_SCORE]->Draw();
	if (pPlayer->GetShotCnt() <= 5)
	{
		// �^�C����UI
		m_aScene_Two[CPlay_element_ui::PLAY_ELEMENT_UI_TIME]->Draw();
		//�ԍ�
		m_pNumber->Draw();
	}

}

// ----------------------------------------
// �ԍ��擾����
// ----------------------------------------
CNumber * CPlay_element_ui::GetNumber(void)
{
	return m_pNumber;
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CPlay_element_ui::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���[�����
	char TexData[PLAY_ELEMENT_UI_MAX][72] =
	{
		{"data/TEXTURE/UI/Frame_Score2.png"},	// �X�R�A
		{"data/TEXTURE/UI/Time_Comment.png"},	// �^�C��
	};
	// �ʒu�����
	D3DXVECTOR3 pos[PLAY_ELEMENT_UI_MAX] =
	{
		{D3DXVECTOR3(100.0f,100.0f,0.0f)},
		{D3DXVECTOR3(20.0f,20.0f,0.0f)},
	};
	// �T�C�Y�����
	D3DXVECTOR2 size[PLAY_ELEMENT_UI_MAX] =
	{
		{D3DXVECTOR2(500.0f,70.0f)},
		{D3DXVECTOR2(50.0f,50.0f) },
	};

	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		// �e�N�X�`���[����������
		if (strcmp(TexData[nCnt], "") != 0)
		{
			/* �e�N�X�`���[�̃��[�h */
			D3DXCreateTextureFromFile(pDevice, TexData[nCnt], &m_pTex[nCnt]);
			/* �ʒu�E�T�C�Y�̃��[�h */
			m_pos[nCnt] = pos[nCnt];
			m_size[nCnt] = size[nCnt];
		}
	}
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CPlay_element_ui::UnLoad(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CPlay_element_ui * CPlay_element_ui::Create(CManager::PLAYER playerId)
{
	// �ϐ��錾
	CPlay_element_ui * pPlay_element_ui;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pPlay_element_ui = new CPlay_element_ui();
	// 
	pPlay_element_ui->playerID = playerId;
	// ����������
	pPlay_element_ui->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pPlay_element_ui;
}

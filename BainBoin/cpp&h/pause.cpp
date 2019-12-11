//------------------------------------------------------
//�N���X�^����[polygo n.cpp]
//Author:FUJIWARA MASATO
//
//--------------------------------------------------------
#include "main.h"
#include "pause.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "scene_two.h"
#include "fade.h"

//--------------------------------------------------------
//
// �}�N����`
//
//--------------------------------------------------------
//�摜�̑��΃p�X�𕡐��Ǘ�����ϐ�
char g_aFileName[PAUSE_TEXTURE][256] = {"data/TEXTURE/PAUSE/00_sky.jpg",
										"data/TEXTURE/PAUSE/00_pousecomment.png",
										"data/TEXTURE/PAUSE/01_GoGame.png",
										"data/TEXTURE/PAUSE/02_GoTitle.png",
																	};
//--------------------------------------------------------
//
// static�̏�����
//
//--------------------------------------------------------
//�z����̏������ׂď�����
LPDIRECT3DTEXTURE9 CPause::m_pTexture[PAUSE_TEXTURE] = {};
bool CPause::m_bPause = false;

//------------------------------------------------------
//
//	�R���X�g���N�^
//
//--------------------------------------------------------
CPause::CPause() : CScene_TWO(ACTOR_PAUSE_UI,LAYER_UI)
{
	m_type = PAUSETYPE_UI;//��Ԃ�������
	m_state = PAUSESTATE_1;//��Ԃ�������
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);//�F�̏�����
}
//------------------------------------------------------
//
//	�f�X�g���N�^
//
//--------------------------------------------------------
CPause::~CPause()
{
}
//------------------------------------------------------
//
//	�e�N�X�`���ǂݍ���
//
//--------------------------------------------------------
HRESULT CPause::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevive = pRenderer->GetDevice();

	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
	{
		D3DXCreateTextureFromFile(pDevive,
			&g_aFileName[nCntPause][0],
			&m_pTexture[nCntPause]);
	}

	return S_OK;
}
//------------------------------------------------------
//
//	�e�N�X�`���j��
//
//--------------------------------------------------------
void CPause::Unload(void)
{
	/*
	//�e�N�X�`���̉��
	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
	{
		if (m_pTexture != NULL)
		{
			m_pTexture[nCntPause]->Release();
			m_pTexture[nCntPause] = NULL;
		}
	}
	*/
}
//------------------------------------------------------
//
//	����������
//
//------------------------------------------------------
void CPause::Init(void)
{
	if (m_type == PAUSETYPE_NORMAL)
	{//�w�i
		CScene_TWO::Init();
		// �ʒu�ƃT�C�Y
		SetSize(D3DXVECTOR2(PAU_SCR_SIZE_X, PAU_SCR_SIZE_Y));
		SetPosition(D3DXVECTOR3(640, 360, 0.0f));

	}

	if (m_type == PAUSETYPE_UI)
	{//[PAUSE]�̕���
		CScene_TWO::Init();
		// �ʒu�ƃT�C�Y
		SetSize(D3DXVECTOR2(PAU_UI_SIZE_X, PAU_UI_SIZE_Y));
		SetPosition(D3DXVECTOR3(640, 200, 0.0f));
	}

	if (m_type == PAUSETYPE_BUTTAN1)
	{//�I������{�^��1
		CScene_TWO::Init();
		// �ʒu�ƃT�C�Y
		SetSize(D3DXVECTOR2(PAU_BUT_SIZE_X, PAU_BUT_SIZE_Y));
		SetPosition(D3DXVECTOR3(640, 250, 0.0f));
	}

	if (m_type == PAUSETYPE_BUTTAN2)
	{//�I������{�^��2
		CScene_TWO::Init();
		// �ʒu�ƃT�C�Y
		SetSize(D3DXVECTOR2(PAU_BUT_SIZE_X, PAU_BUT_SIZE_Y));
		SetPosition(D3DXVECTOR3(640, 270, 0.0f));
	}
}
//------------------------------------------------------
//
//	�I������
//
//------------------------------------------------------
void CPause::Uninit(void)
{
	CScene_TWO::Uninit();
}
//------------------------------------------------------
//
//	�X�V����
//
//------------------------------------------------------
void CPause::Update(void)
{
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	//�L�[�{�[�h
	if (pKeyboard->GetKeyboardTrigger(DIK_UP))
	{//�L�[�̏����������
		if (m_state == PAUSESTATE_2)
		{//�I������Ă���{�^����2�Ԗڂ�������1�ԖڂɈڂ�
			m_state = PAUSESTATE_1;
		}
	}

	else if (pKeyboard->GetKeyboardTrigger(DIK_DOWN))
	{//�L�[�̉�����������
		if (m_state == PAUSESTATE_1)
		{//�I������Ă���{�^����1�Ԗڂ�������2�ԖڂɈڂ�
			m_state = PAUSESTATE_2;
		}
	}

	//�W���C�X�e�B�b�N �\���L�[
	if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_UP))
	{
		// �G���^�[��
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);
		if (m_state == PAUSESTATE_2)
		{//�I������Ă���{�^����2�Ԗڂ�������1�ԖڂɈڂ�
			m_state = PAUSESTATE_1;
		}
	}
	else if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_DOWN))
	{
		// �G���^�[��
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);
		if (m_state == PAUSESTATE_1)
		{//�I������Ă���{�^����1�Ԗڂ�������2�ԖڂɈڂ�
			m_state = PAUSESTATE_2;
		}
	}


	if (m_state == PAUSESTATE_1)
	{//1���I������Ă��鎞
		if (m_type == PAUSETYPE_BUTTAN1)
		{//�^�C�v���{�^��1��������T�C�Y���g�傷��
			// �ʒu�ƃT�C�Y
			CScene_TWO::SetPosition(D3DXVECTOR3(640, 380, 0.0f));
			CScene_TWO::SetSize(D3DXVECTOR2(PAU_BUT_SIZE_X + 70, PAU_BUT_SIZE_Y + 70));

		}

		//2�̃T�C�Y�����ɖ߂�
		if (m_type == PAUSETYPE_BUTTAN2)
		{//�T�C�Y���k������
		 // �ʒu�ƃT�C�Y
			CScene_TWO::SetPosition(D3DXVECTOR3(640, 550, 0.0f));
			CScene_TWO::SetSize(D3DXVECTOR2(PAU_BUT_SIZE_X, PAU_BUT_SIZE_Y));
		}
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_A) || CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_B))
		{//�^�C�g����
			//CManager::GetFade()->SetFade(CManager::MODE_GAME);
			SetPause(false);
		}
	}

	if (m_state == PAUSESTATE_2)
	{//2���I������Ă��鎞
		if (m_type == PAUSETYPE_BUTTAN2)
		{//�^�C�v���{�^��2��������T�C�Y���g�傷��
		 // �ʒu�ƃT�C�Y
			CScene_TWO::SetPosition(D3DXVECTOR3(640, 550, 0.0f));
			CScene_TWO::SetSize(D3DXVECTOR2(PAU_BUT_SIZE_X + 70, PAU_BUT_SIZE_Y + 70));
		}

		//1�̃T�C�Y�����ɖ߂�
		if (m_type == PAUSETYPE_BUTTAN1)
		{//�T�C�Y���k������
			CScene_TWO::SetPosition(D3DXVECTOR3(640, 380, 0.0f));
			CScene_TWO::SetSize(D3DXVECTOR2(PAU_BUT_SIZE_X, PAU_BUT_SIZE_Y));
		}
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_A) || CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_B))
		{//���߂���
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			SetPause(false);
		}
	}
}
//------------------------------------------------------
//
//	�`�揈��
//
//------------------------------------------------------
void CPause::Draw(void)
{
	CScene_TWO::Draw();
}
//------------------------------------------------------
//
//	��������
//
//------------------------------------------------------
CPause *CPause::Create(PAUSETYPE type)
{
	CPause *pPause;

	pPause = new CPause;

	pPause->m_type = type;

	pPause->Init();

	int nCount = 0;
	for (int nCntPause = 0; nCntPause < type;nCntPause++)
	{
		nCount++;
	}
	pPause->BindTexture(m_pTexture[nCount]);

	return pPause;
}

// ------------------------------------------
// ��Ԑݒ�
// ------------------------------------------
void CPause::SetPause(bool bPause)
{
	m_bPause = bPause;
}

// ------------------------------------------
// ��Ԏ擾
// ------------------------------------------
bool CPause::GetPause(void)
{
	return m_bPause;
}

//------------------------------------------------------
//クラス型処理[polygo n.cpp]
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
// マクロ定義
//
//--------------------------------------------------------
//画像の相対パスを複数管理する変数
char g_aFileName[PAUSE_TEXTURE][256] = {"data/TEXTURE/PAUSE/00_sky.jpg",
										"data/TEXTURE/PAUSE/00_pousecomment.png",
										"data/TEXTURE/PAUSE/01_GoGame.png",
										"data/TEXTURE/PAUSE/02_GoTitle.png",
																	};
//--------------------------------------------------------
//
// staticの初期化
//
//--------------------------------------------------------
//配列内の情報をすべて初期化
LPDIRECT3DTEXTURE9 CPause::m_pTexture[PAUSE_TEXTURE] = {};
bool CPause::m_bPause = false;

//------------------------------------------------------
//
//	コンストラクタ
//
//--------------------------------------------------------
CPause::CPause() : CScene_TWO(ACTOR_PAUSE_UI,LAYER_UI)
{
	m_type = PAUSETYPE_UI;//状態を初期化
	m_state = PAUSESTATE_1;//状態を初期化
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);//色の初期化
}
//------------------------------------------------------
//
//	デストラクタ
//
//--------------------------------------------------------
CPause::~CPause()
{
}
//------------------------------------------------------
//
//	テクスチャ読み込み
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
//	テクスチャ破棄
//
//--------------------------------------------------------
void CPause::Unload(void)
{
	/*
	//テクスチャの解放
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
//	初期化処理
//
//------------------------------------------------------
void CPause::Init(void)
{
	if (m_type == PAUSETYPE_NORMAL)
	{//背景
		CScene_TWO::Init();
		// 位置とサイズ
		SetSize(D3DXVECTOR2(PAU_SCR_SIZE_X, PAU_SCR_SIZE_Y));
		SetPosition(D3DXVECTOR3(640, 360, 0.0f));

	}

	if (m_type == PAUSETYPE_UI)
	{//[PAUSE]の文字
		CScene_TWO::Init();
		// 位置とサイズ
		SetSize(D3DXVECTOR2(PAU_UI_SIZE_X, PAU_UI_SIZE_Y));
		SetPosition(D3DXVECTOR3(640, 200, 0.0f));
	}

	if (m_type == PAUSETYPE_BUTTAN1)
	{//選択するボタン1
		CScene_TWO::Init();
		// 位置とサイズ
		SetSize(D3DXVECTOR2(PAU_BUT_SIZE_X, PAU_BUT_SIZE_Y));
		SetPosition(D3DXVECTOR3(640, 250, 0.0f));
	}

	if (m_type == PAUSETYPE_BUTTAN2)
	{//選択するボタン2
		CScene_TWO::Init();
		// 位置とサイズ
		SetSize(D3DXVECTOR2(PAU_BUT_SIZE_X, PAU_BUT_SIZE_Y));
		SetPosition(D3DXVECTOR3(640, 270, 0.0f));
	}
}
//------------------------------------------------------
//
//	終了処理
//
//------------------------------------------------------
void CPause::Uninit(void)
{
	CScene_TWO::Uninit();
}
//------------------------------------------------------
//
//	更新処理
//
//------------------------------------------------------
void CPause::Update(void)
{
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	//キーボード
	if (pKeyboard->GetKeyboardTrigger(DIK_UP))
	{//キーの上を押したら
		if (m_state == PAUSESTATE_2)
		{//選択されているボタンが2番目だったら1番目に移す
			m_state = PAUSESTATE_1;
		}
	}

	else if (pKeyboard->GetKeyboardTrigger(DIK_DOWN))
	{//キーの下を押したら
		if (m_state == PAUSESTATE_1)
		{//選択されているボタンが1番目だったら2番目に移す
			m_state = PAUSESTATE_2;
		}
	}

	//ジョイスティック 十字キー
	if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_UP))
	{
		// エンター音
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);
		if (m_state == PAUSESTATE_2)
		{//選択されているボタンが2番目だったら1番目に移す
			m_state = PAUSESTATE_1;
		}
	}
	else if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_DOWN))
	{
		// エンター音
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);
		if (m_state == PAUSESTATE_1)
		{//選択されているボタンが1番目だったら2番目に移す
			m_state = PAUSESTATE_2;
		}
	}


	if (m_state == PAUSESTATE_1)
	{//1が選択されている時
		if (m_type == PAUSETYPE_BUTTAN1)
		{//タイプがボタン1だったらサイズを拡大する
			// 位置とサイズ
			CScene_TWO::SetPosition(D3DXVECTOR3(640, 380, 0.0f));
			CScene_TWO::SetSize(D3DXVECTOR2(PAU_BUT_SIZE_X + 70, PAU_BUT_SIZE_Y + 70));

		}

		//2のサイズを元に戻す
		if (m_type == PAUSETYPE_BUTTAN2)
		{//サイズを縮小する
		 // 位置とサイズ
			CScene_TWO::SetPosition(D3DXVECTOR3(640, 550, 0.0f));
			CScene_TWO::SetSize(D3DXVECTOR2(PAU_BUT_SIZE_X, PAU_BUT_SIZE_Y));
		}
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_A) || CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_B))
		{//タイトルへ
			//CManager::GetFade()->SetFade(CManager::MODE_GAME);
			SetPause(false);
		}
	}

	if (m_state == PAUSESTATE_2)
	{//2が選択されている時
		if (m_type == PAUSETYPE_BUTTAN2)
		{//タイプがボタン2だったらサイズを拡大する
		 // 位置とサイズ
			CScene_TWO::SetPosition(D3DXVECTOR3(640, 550, 0.0f));
			CScene_TWO::SetSize(D3DXVECTOR2(PAU_BUT_SIZE_X + 70, PAU_BUT_SIZE_Y + 70));
		}

		//1のサイズを元に戻す
		if (m_type == PAUSETYPE_BUTTAN1)
		{//サイズを縮小する
			CScene_TWO::SetPosition(D3DXVECTOR3(640, 380, 0.0f));
			CScene_TWO::SetSize(D3DXVECTOR2(PAU_BUT_SIZE_X, PAU_BUT_SIZE_Y));
		}
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_A) || CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_B))
		{//初めから
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			SetPause(false);
		}
	}
}
//------------------------------------------------------
//
//	描画処理
//
//------------------------------------------------------
void CPause::Draw(void)
{
	CScene_TWO::Draw();
}
//------------------------------------------------------
//
//	生成処理
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
// 状態設定
// ------------------------------------------
void CPause::SetPause(bool bPause)
{
	m_bPause = bPause;
}

// ------------------------------------------
// 状態取得
// ------------------------------------------
bool CPause::GetPause(void)
{
	return m_bPause;
}

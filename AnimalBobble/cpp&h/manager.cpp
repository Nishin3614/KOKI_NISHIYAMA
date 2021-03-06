// ------------------------------------------------------------------------------------------
//
// マネージャー処理の説明[manager.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
/* My */
#include "manager.h"

/* 描画 */
#include "scene.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "ranking.h"
#include "selection.h"
#include "fade.h"

// ------------------------------------------------------------------------------------------
//
// マクロ関数
//
// ------------------------------------------------------------------------------------------
#define STARTMODE (MODE_TITLE)

// ------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------------------------------------------------------
CRenderer *CManager::m_renderer = NULL;
CKeyboard * CManager::m_keyboard = NULL;
CJoypad * CManager::m_joypad = NULL;
CMouse * CManager::m_mouse = NULL;
CSound * CManager::m_sound = NULL;
CFade * CManager::m_fade = NULL;
CTitle * CManager::m_title = NULL;
CTutorial * CManager::m_tutorial = NULL;
CGame * CManager::m_game = NULL;
CRanking * CManager::m_ranking = NULL;
CSelection * CManager::m_selection = NULL;
CManager::MODE CManager::m_mode = CManager::MODE_TITLE;

// ------------------------------------------------------------------------------------------
// コンストラクタ
// ------------------------------------------------------------------------------------------
CManager::CManager()
{
}

// ------------------------------------------------------------------------------------------
// デストラクタ
// ------------------------------------------------------------------------------------------
CManager::~CManager()
{
}

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
HRESULT CManager::Init(HWND hWnd, BOOL bWindow, HINSTANCE hInstance)
{
	/* メモリの生成 */
	/* 入力デバイス */
	// キーボードの生成
	m_keyboard = new CKeyboard;
	// ジョイパッドの生成
	m_joypad = new CJoypad;
	// マウスの生成
	m_mouse = new CMouse;
	/* ゲームに欠かせないもの */
	// サウンドの生成
	m_sound = new CSound;
	// レンダリングの生成
	m_renderer = new CRenderer;
	/* 画面 */
	// 画面遷移の生成
	m_fade = new CFade;
	// タイトルの生成
	m_title = new CTitle;
	// チュートリアルの生成
	m_tutorial = new CTutorial;
	// ゲームの生成
	m_game = new CGame;
	// ランキングの生成
	m_ranking = new CRanking;
	// 選択画面の生成
	m_selection = new CSelection;

	// モードの設定
	m_mode = STARTMODE;

	/* 初期化 */
	// キーボード
	m_keyboard->Init(hInstance, hWnd);
	// ジョイパッド
	m_joypad->Init(hInstance, hWnd);
	// マウス
	m_mouse->Init(hInstance, hWnd);
	// サウンド
	m_sound->Init(hWnd);
	// レンダリング
	m_renderer->Init(hWnd,bWindow);
	// 画面遷移
	m_fade->Init(STARTMODE);
	return S_OK;
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void CManager::Uninit(void)
{
	/* 終了 */
	// キーボード
	m_keyboard->Uninit();
	// ジョイパッド
	m_joypad->Uninit();
	// マウス
	m_mouse->Uninit();
	// 入力デバイス
	CInput::ReleaseAll();
	// サウンド
	m_sound->Uninit();
	// レンダリング
	m_renderer->Uninit();
	// 画面遷移
	m_fade->Uninit();

	/* メモリの破棄 */
	// キーボード
	if (m_keyboard != NULL)
	{
		delete m_keyboard;
		m_keyboard = NULL;
	}
	// ジョイパッド
	if (m_joypad != NULL)
	{
		delete m_joypad;
		m_joypad = NULL;
	}
	// マウス
	if (m_mouse != NULL)
	{
		delete m_mouse;
		m_mouse = NULL;
	}
	// サウンド
	if (m_sound != NULL)
	{
		delete m_sound;
		m_sound = NULL;
	}
	// レンダリング
	if (m_renderer != NULL)
	{
		delete m_renderer;
		m_renderer = NULL;
	}
	// 画面遷移
	if (m_fade != NULL)
	{
		delete m_fade;
		m_fade = NULL;
	}
	// タイトル
	if (m_title != NULL)
	{
		m_title->Uninit();
		delete m_title;
		m_title = NULL;
	}
	// チュートリアル
	if (m_tutorial != NULL)
	{
		m_tutorial->Uninit();
		delete m_tutorial;
		m_tutorial = NULL;
	}
	// ゲーム
	if (m_game != NULL)
	{
		m_game->Uninit();
		delete m_game;
		m_game = NULL;
	}
	// ランキング
	if (m_ranking != NULL)
	{
		m_ranking->Uninit();
		delete m_ranking;
		m_ranking = NULL;
	}
	// 選択画面
	if (m_selection != NULL)
	{
		m_selection->Uninit();
		delete m_selection;
		m_selection = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void CManager::Update(void)
{
	// キーボード
	m_keyboard->Update();
	// ジョイパッド
	m_joypad->Update();
	// マウス
	m_mouse->Update();
	// レンダリング
	m_renderer->Update();

	// 前のモード終了
	switch (m_mode)
	{
		// タイトル
	case MODE_TITLE:
		m_title->Update();
		break;
		// コース選択
	case MODE_SELECTION:
		m_selection->Update();
		break;

		// チュートリアル
	case MODE_TUTORIAL:
		m_tutorial->Update();
		break;
		// ゲーム
	case MODE_GAME:
		m_game->Update();
		break;
		// ランキング
	case MODE_RANKING:
		m_ranking->Update();
		break;
	default:
		break;
	}

	// 画面遷移
	m_fade->Update();
}

// ------------------------------------------------------------------------------------------
//
// 描画関数
//
// ------------------------------------------------------------------------------------------
void CManager::Draw(void)
{
	// レンダリング
	m_renderer->Draw();
}

// ------------------------------------------------------------------------------------------
// モード設定関数
// ------------------------------------------------------------------------------------------
void CManager::SetMode(MODE mode)
{
	// 前のモード終了
	switch (m_mode)
	{
		// タイトル
	case MODE_TITLE:
		m_title->Uninit();
		// タイトルBGM
		if (mode != MODE_SELECTION)
		{
			m_sound->StopSound();
			m_sound->StopSound(CSound::LABEL_BGM_TITLE);
		}
		break;
		// コース選択
	case MODE_SELECTION:
		m_selection->Uninit();
		// タイトルBGM
		m_sound->StopSound();
		m_sound->StopSound(CSound::LABEL_BGM_TITLE);
		break;
		// チュートリアル
	case MODE_TUTORIAL:
		m_tutorial->Uninit();
		// ゲームBGM
		m_sound->StopSound();
		m_sound->StopSound(CSound::LABEL_BGM_GAME);

		break;
		// ゲーム
	case MODE_GAME:
		m_game->Uninit();
		// ゲームBGM
		m_sound->StopSound();
		m_sound->StopSound(CSound::LABEL_BGM_GAME);
		break;
		// ランキング
	case MODE_RANKING:
		m_ranking->Uninit();
		// ランキングBGM
		m_sound->StopSound();
		m_sound->StopSound(CSound::LABEL_BGM_RANKINGMOVE);
		break;
	default:
		break;
	}

	m_mode = mode;	// 現在のモードを代入

	// 現在のモード初期化
	switch (mode)
	{
		// タイトル
	case MODE_TITLE:
		m_title->Init();
		// タイトルBGM
		m_sound->PlaySound(CSound::LABEL_BGM_TITLE);
		break;
		// コース選択
	case MODE_SELECTION:
		m_selection->Init();
		if (mode != MODE_TITLE)
		{
			// タイトルBGM
			m_sound->PlaySound(CSound::LABEL_BGM_TITLE);
		}
		break;
		// チュートリアル
	case MODE_TUTORIAL:
		m_tutorial->Init();
		// ゲームBGM
		m_sound->PlaySound(CSound::LABEL_BGM_GAME);
		break;
		// ゲーム
	case MODE_GAME:
		m_game->Init();
		// ゲームBGM
		m_sound->PlaySound(CSound::LABEL_BGM_GAME);
		break;
		// ランキング
	case MODE_RANKING:
		m_ranking->Init();
		// ランキングBGM
		m_sound->PlaySound(CSound::LABEL_BGM_RANKINGMOVE);
		break;
	default:
		break;
	}
}

// ------------------------------------------------------------------------------------------
// レンダリング取得関数
// ------------------------------------------------------------------------------------------
CRenderer * CManager::GetRenderer(void)
{
	return m_renderer;
}

// ------------------------------------------------------------------------------------------
// キーボード取得関数
// ------------------------------------------------------------------------------------------
CKeyboard * CManager::GetKeyboard(void)
{
	return m_keyboard;
}

// ------------------------------------------------------------------------------------------
// ジョイパッド取得関数
// ------------------------------------------------------------------------------------------
CJoypad * CManager::GetJoy(void)
{
	return m_joypad;
}

// ------------------------------------------------------------------------------------------
// マウス取得関数
// ------------------------------------------------------------------------------------------
CMouse * CManager::GetMouse(void)
{
	return m_mouse;
}

// ------------------------------------------------------------------------------------------
// サウンド取得関数
// ------------------------------------------------------------------------------------------
CSound * CManager::GetSound(void)
{
	return m_sound;
}

// ------------------------------------------------------------------------------------------
// 画面遷移取得関数
// ------------------------------------------------------------------------------------------
CFade * CManager::GetFade(void)
{
	return m_fade;
}

// ------------------------------------------------------------------------------------------
// ゲーム取得関数
// ------------------------------------------------------------------------------------------
CGame * CManager::GetGame(void)
{
	return m_game;
}

// ------------------------------------------------------------------------------------------
// タイトル取得関数
// ------------------------------------------------------------------------------------------
CTitle * CManager::GetTitle(void)
{
	return m_title;
}

// ------------------------------------------------------------------------------------------
// 選択取得関数
// ------------------------------------------------------------------------------------------
CSelection * CManager::GetSelection(void)
{
	return m_selection;
}

// ------------------------------------------------------------------------------------------
// チュートリアル取得関数
// ------------------------------------------------------------------------------------------
CTutorial * CManager::GetTutorial(void)
{
	return m_tutorial;
}

// ------------------------------------------------------------------------------------------
// ランキング取得関数
// ------------------------------------------------------------------------------------------
CRanking * CManager::GetRanking(void)
{
	return m_ranking;
}

// ------------------------------------------------------------------------------------------
// モード取得関数
// ------------------------------------------------------------------------------------------
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

// ------------------------------------------
//
// タイトル処理 [title.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "title.h"
/* 描画 */
#include "tlybg.h"
#include "title_ui.h"
#include "fade.h"

// ------------------------------------------
//
// マクロ定義
//
// ------------------------------------------

// ------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------

// ------------------------------------------
// コンストラクタ
// ------------------------------------------
CTitle::CTitle()
{
	m_nCntChange = 0;
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CTitle::~CTitle()
{
}

// ------------------------------------------
// 初期化
// ------------------------------------------
void CTitle::Init(void)
{
	/* 作成 */
	// 背景
	CTlyBg::Create(CManager::MODE_TITLE);
	// UI
	CTitle_ui::Create();
	// カウント遷移
	m_nCntChange = 0;
}

// ------------------------------------------
// 終了
// ------------------------------------------
void CTitle::Uninit(void)
{
	CScene::ReleaseAll();
}

// ------------------------------------------
// 更新
// ------------------------------------------
void CTitle::Update(void)
{
	// カウント遷移
	m_nCntChange++;
	// ランキング遷移
	if (m_nCntChange >= INTERVAL_TIME)
	{
		// 画面遷移の状態が遷移していない状態だったら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{		
			CManager::GetFade()->SetFade(CManager::MODE_RANKING);
		}
	}
	// 選択画面遷移
	//キーボード
	else if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		// 画面遷移の状態が遷移していない状態だったら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
			// エンター音
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);
		}
	}
	//ジョイスティック
	else if (CManager::GetJoy()->GetPress(0, CJoypad::KEY_A)||
		CManager::GetJoy()->GetPress(0, CJoypad::KEY_B)||
		CManager::GetJoy()->GetPress(0, CJoypad::KEY_START))
	{
		// 画面遷移の状態が遷移していない状態だったら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
			// エンター音
			//CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);
		}
	}
}

// ------------------------------------------
// 描画
// ------------------------------------------
void CTitle::Draw(void)
{
}

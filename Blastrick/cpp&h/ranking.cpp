// ------------------------------------------
//
// ランキング処理 [ranking.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "ranking.h"
/* 描画 */
#include "rank_ui.h"
#include "topscore.h"
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
CRanking::CRanking()
{
	m_CntChange = 0;
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CRanking::~CRanking()
{
}

// ------------------------------------------
// 初期化
// ------------------------------------------
void CRanking::Init(void)
{
	/* 作成 */
	// ランク用ui
	CRank_ui::Create();
	// カウント遷移
	m_CntChange = 0;
}

// ------------------------------------------
// 終了
// ------------------------------------------
void CRanking::Uninit(void)
{
	CScene::ReleaseAll();
}

// ------------------------------------------
// 更新
// ------------------------------------------
void CRanking::Update(void)
{
	// カウント遷移アップ
	m_CntChange++;
	if (m_CntChange >= INTERVAL_TIME)
	{
		// 画面遷移の状態が遷移していない状態だったら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CTopscore::SetScore(0);
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}
	// モードの設定
	else if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		// エンター音
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_ENTER);
		// 画面遷移の状態が遷移していない状態だったら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CTopscore::SetScore(0);
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}
	// 選択画面遷移
	else if (CManager::GetJoy() != NULL)
	{
		// モードの設定
		if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_START) ||
			CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_B) ||
			CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_A))
		{
			// エンター音
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_ENTER);
			// 画面遷移の状態が遷移していない状態だったら
			if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				CTopscore::SetScore(0);
				CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			}
		}
	}
}

// ------------------------------------------
// 描画
// ------------------------------------------
void CRanking::Draw(void)
{
}

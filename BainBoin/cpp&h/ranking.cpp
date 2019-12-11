// ------------------------------------------
//
// ランキング処理 [ranking.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "ranking.h"
/* 描画 */
#include "tlybg.h"
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
	// 背景
	CTlyBg::Create(CManager::MODE_RANKING);
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
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);
		// 画面遷移の状態が遷移していない状態だったら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CTopscore::SetScore(0);
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}

	//ジョイスティック
	// モードの設定
	else if (CManager::GetJoy()->GetPress(0, CJoypad::KEY_A)||
		CManager::GetJoy()->GetPress(0, CJoypad::KEY_B))
	{
		// エンター音
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);
		// 画面遷移の状態が遷移していない状態だったら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CTopscore::SetScore(0);
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}
}

// ------------------------------------------
// 描画
// ------------------------------------------
void CRanking::Draw(void)
{
}

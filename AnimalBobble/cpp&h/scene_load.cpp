// ----------------------------------------
//
// シーン処理の説明[scene_load.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene_load.h"

/* 描画 */
#include "effect.h"
#include "tlybg.h"
#include "frame.h"
#include "ceiling.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "number.h"
#include "box.h"
#include "mark.h"
#include "prediction.h"
#include "rank_ui.h"
#include "topscore.h"
#include "title_ui.h"
#include "selection_ui.h"
#include "pause_ui.h"
#include "game_ui.h"
#include "play_element_ui.h"
#include "tutorial_ui.h"
#include "line.h"

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CScene_load::CScene_load()
{
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CScene_load::~CScene_load()
{
}

// ----------------------------------------
// シーン親子作成処理
// ----------------------------------------
void CScene_load::LoadAll(void)
{
	/* テクスチャー読み込み */
	// 背景
	CTlyBg::Load();
	// 枠
	CFrame::Load();
	// 天井
	CCeiling::Load();
	// エフェクト
	CEffect::Load();
	// 爆発
	CExplosion::Load();
	// プレイヤー
	CPlayer::Load();
	// 予測線
	CPrediction::Load();
	// 弾
	CBullet::Load();
	// 番号
	CNumber::Load();
	// 箱
	CBox::Load();
	// 目印
	CMark::Load();
	// タイトルUI
	CTitle_ui::Load();
	// チュートリアルUI
	CTutorial_ui::Load();
	// 選択UI
	CSelection_ui::Load();
	// ランク用UI
	CRank_ui::Load();
	// 上位スコア
	CTopscore::Load();
	// プレイヤー用UI
	CPlay_element_ui::Load();
	// ゲーム用UI
	CGame_ui::Load();
	// ポーズUI
	CPause_ui::Load();
	// 線
	CLine::Load();
}

// ----------------------------------------
// 読み込んだものを破棄する処理
// ----------------------------------------
void CScene_load::UnLoadAll(void)
{
	/* 読み込んだものの破棄*/
	// 背景
	CTlyBg::UnLoad();
	// 枠
	CFrame::UnLoad();
	// 天井
	CCeiling::UnLoad();
	// エフェクト
	CEffect::UnLoad();
	// 爆発
	CExplosion::UnLoad();
	// プレイヤー
	CPlayer::UnLoad();
	// 予測線
	CPrediction::UnLoad();
	// 弾
	CBullet::UnLoad();
	// 番号
	CNumber::UnLoad();
	// 箱
	CBox::UnLoad();
	// 目印
	CMark::UnLoad();
	// タイトルUI
	CTitle_ui::UnLoad();
	// チュートリアルUI
	CTutorial_ui::UnLoad();
	// 選択UI
	CSelection_ui::UnLoad();
	// ランク用UI
	CRank_ui::UnLoad();
	// 上位スコア
	CTopscore::Save();
	// プレイヤー用UI
	CPlay_element_ui::UnLoad();
	// ゲーム用UI
	CGame_ui::UnLoad();
	// ポーズUI
	CPause_ui::UnLoad();
	// 線
	CLine::UnLoad();
}
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
#include "tlybg.h"
#include "score.h"
#include "number.h"
#include "mark.h"
#include "rank_ui.h"
#include "title_ui.h"
#include "play_element_ui.h"
#include "tutorial_ui.h"
#include "floor.h"
#include "player.h"
#include "item.h"
#include "pause.h"
#include "3Deffect.h"
#include "3Dparticle.h"
#include "block.h"
#include "meshdome.h"
#include "collision.h"
#include "gauge.h"
#include "meshsphere.h"
#include "topscore.h"

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
	// 番号
	CNumber::Load();
	// 目印
	CMark::Load();
	// タイトルUI
	CTitle_ui::Load();
	// チュートリアルUI
	CTutorial_ui::Load();
	// ランク用UI
	CRank_ui::Load();
	// プレイヤー用UI
	CPlay_element_ui::Load();
	// 床
	CFloor::Load();
	// メッシュドーム
	CMeshdome::Load();
	// メッシュスフィア
	CMeshsphere::Load();
	// プレイヤー
	CPlayer::Load();
	// ポーズ
	CPause::Load();
	// アイテム
	CItem::Load();
	// ブロック
	CBlock::Load();
	// 3Deffect
	C3DEffect::Load();
	// 3Dparticle
	C3DParticle::Load();
	// 当たり判定
	CCollision::Load();
	// ゲージ
	CGauge::Load();
	// 上位スコア
	CTopscore::Load();
}

// ----------------------------------------
// 読み込んだものを破棄する処理
// ----------------------------------------
void CScene_load::UnLoadAll(void)
{
	/* 読み込んだものの破棄*/
	// 背景
	CTlyBg::UnLoad();
	// 目印
	CMark::UnLoad();
	// タイトルUI
	CTitle_ui::UnLoad();
	// チュートリアルUI
	CTutorial_ui::UnLoad();
	// ランク用UI
	CRank_ui::UnLoad();
	// プレイヤー用UI
	CPlay_element_ui::UnLoad();
	// 床
	CFloor::UnLoad();
	// メッシュドーム
	CMeshdome::UnLoad();
	// メッシュスフィア
	CMeshsphere::Unload();
	// プレイヤー
	CPlayer::UnLoad();
	// ポーズ
	CPause::Unload();
	// アイテム
	CItem::UnLoad();
	// ブロック
	CBlock::UnLoad();
	// 3Deffect
	C3DEffect::Unload();
	// 3Dparticle
	C3DParticle::Unload();
	// ゲージ
	CGauge::UnLoad();
}
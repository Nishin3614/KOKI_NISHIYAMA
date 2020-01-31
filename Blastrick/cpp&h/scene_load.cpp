// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// シーン処理の説明[scene_load.cpp]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_load.h"
/* 描画 */
#include "score.h"
#include "number.h"
#include "mark.h"
#include "rank_ui.h"
#include "topscore.h"
#include "title_ui.h"
#include "pause_ui.h"
#include "game_ui.h"
#include "play_element_ui.h"
#include "tutorial_ui.h"
#include "2Dgauge.h"
#include "3Deffect.h"
#include "3Dparticle.h"
#include "item.h"
#include "floor.h"
#include "meshwall.h"
#include "meshdome.h"
#include "meshsphere.h"
#include "collision.h"
#include "trajectory.h"
#include "camera.h"
#include "forceline.h"
#include "player.h"
#include "rothuman.h"
#include "staticobj.h"
#include "stategauge.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_load::CScene_load()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_load::~CScene_load()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// シーン親子作成処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_load::LoadAll(void)
{
	/* ----------テクスチャー読み込み---------- */
	/* UI */
	// タイトルUI
	if(!CTitle_ui::Load()== S_OK)
	{
		CCalculation::Messanger("タイトルUI読み取り失敗");
	}
	// チュートリアルUI
	if (!CTutorial_ui::Load()== S_OK)
	{
		CCalculation::Messanger("チュートリアルUI読み取り失敗");
	}
	// ランク用UI
	if (!CRank_ui::Load()== S_OK)
	{
		CCalculation::Messanger("ランクUI読み取り失敗");
	}
	// 上位スコア
	if (!CTopscore::Load()== S_OK)
	{
		CCalculation::Messanger("上位スコア読み取り失敗");
	}
	// プレイヤー用UI
	if(!CPlay_element_ui::Load()== S_OK)
	{
		CCalculation::Messanger("プレイヤーUI読み取り失敗");
	}
	// ゲーム用UI
	if(!CGame_ui::Load()== S_OK)
	{
		CCalculation::Messanger("ゲームUI読み取り失敗");
	}
	// ポーズUI
	if(!CPause_ui::Load()== S_OK)
	{
		CCalculation::Messanger("ポーズUI読み取り失敗");
	}
	/* 効果 */
	// 3Deffect
	if(!C3DEffect::Load()== S_OK)
	{
		CCalculation::Messanger("3Dエフェクト読み取り失敗");
	}
	// 3Dparticle
	if(!C3DParticle::Load()== S_OK)
	{
		CCalculation::Messanger("3Dパーティクル読み取り失敗");
	}
	// 軌跡
	if (!CTrajectory::Load() == S_OK)
	{
		CCalculation::Messanger("軌跡読み取り失敗");
	}

	/* ゲームに必要なもの */
	// 当たり判定
	if(!CCollision::Load()== S_OK)
	{
		CCalculation::Messanger("当たり判定読み取り失敗");
	}
	// カメラ
	if (!CCamera::Load() == S_OK)
	{
		CCalculation::Messanger("カメラ読み取り失敗");
	}
	// ステージゲージ
	if (!CStateGauge::Load() == S_OK)
	{
		CCalculation::Messanger("ステートゲージ読み取り失敗");
	}
	// 2Dゲージ
	if (!C2DGauge::Load() == S_OK)
	{
		CCalculation::Messanger("2Dゲージ読み取り失敗");
	}

	/* 2Dオブジェクト */
	// 集中線
	if (!CForceline::Load() == S_OK)
	{
		CCalculation::Messanger("集中線読み取り失敗");
	}
	// 番号
	if (!CNumber::Load() == S_OK)
	{
		CCalculation::Messanger("番号読み取り失敗");
	}
	// 目印
	if (!CMark::Load() == S_OK)
	{
		CCalculation::Messanger("目印読み取り失敗");
	}

	/* 3Dオブジェクト*/

	/* メッシュ */
	// 床
	if (!CFloor::Load() == S_OK)
	{
		CCalculation::Messanger("床読み取り失敗");
	}
	// メッシュドーム
	if (!CMeshdome::Load() == S_OK)
	{
		CCalculation::Messanger("メッシュドーム読み取り失敗");
	}
	// メッシュスフィア
	if (!CMeshsphere::Load() == S_OK)
	{
		CCalculation::Messanger("メッシュスフィア読み取り失敗");
	}
	// メッシュウォール
	if (!CMeshwall::Load() == S_OK)
	{
		CCalculation::Messanger("メッシュウォール読み取り失敗");
	}

	/* 3Dモデル*/
	// プレイヤー
	if(!CPlayer::Load()== S_OK)
	{
		CCalculation::Messanger("プレイヤー読み取り失敗");
	}
	// 棒人間
	if (!CRothuman::Load() == S_OK)
	{
		CCalculation::Messanger("棒人間読み取り失敗");
	}
	// 静的オブジェクト
	if (!CStaticobj::Load() == S_OK)
	{
		CCalculation::Messanger("静的オブジェクト読み取り失敗");
	}
	// アイテム
	if (!CItem::Load() == S_OK)
	{
		CCalculation::Messanger("アイテム読み取り失敗");
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだものを破棄する処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_load::UnLoadAll(void)
{
	/* 読み込んだものの破棄*/
	// 番号
	CNumber::UnLoad();
	// 目印
	CMark::UnLoad();
	// タイトルUI
	CTitle_ui::UnLoad();
	// チュートリアルUI
	CTutorial_ui::UnLoad();
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
	// 床
	CFloor::UnLoad();
	// プレイヤー
	CPlayer::UnLoad();
	// 棒人間
	CRothuman::UnLoad();
	// 3Deffect
	C3DEffect::Unload();
	// 3Dparticle
	C3DParticle::Unload();
	// 軌跡
	CTrajectory::Unload();
	// メッシュドーム
	CMeshdome::UnLoad();
	// メッシュスフィア
	CMeshsphere::Unload();
	// アイテム
	CItem::UnLoad();
	// 2Dゲージ
	C2DGauge::UnLoad();
	// 集中線
	CForceline::UnLoad();
	// 静的オブジェクト
	CStaticobj::UnLoad();
	// メッシュウォール
	CMeshwall::UnLoad();
}
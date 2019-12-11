// ------------------------------------------------------------------------------------------
//
// チュートリアル処理の説明[tutorial.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------
#include "tutorial.h"
#include "tutorialcomment.h"
#include "input.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "titlecomment.h"
#include "camera.h"
#include "floor.h"
#include "wall.h"
#include "materials.h"
#include "giraffe.h"
#include "light.h"
#include "shadow.h"
#include "trajectory.h"
#include "hemisphere.h"
#include "dome.h"
#include "player.h"
#include "Billboard.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "item.h"
#include "hpgauge.h"
#include "range.h"
#include "select.h"
#include "selectcomment.h"
#include "arrow.h"
#include "buffalo.h"
#include "water.h"
#include "shogira.h"
#include "prediction.h"

// ------------------------------------------------------------------------------------------
//
// チュートリアル初期化処理
//
// ------------------------------------------------------------------------------------------
int g_TutorialState = TUTORIALSSTATE_NORMAL;	// 通常状態

// ------------------------------------------------------------------------------------------
//
// チュートリアル初期化処理
//
// ------------------------------------------------------------------------------------------

void InitTutorial(void)
{
	// 状態の初期化
	g_TutorialState = TUTORIALSSTATE_NORMAL;	// 通常状態

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// 爆発の初期化
	InitExplosion();

	// エフェクト初期化
	InitEffect();

	// 軌跡の初期化処理
	InitTrajectory();

	// 弾の初期化処理
	InitBullet();

	// ビルボードの初期化処理
	InitBillboard();

	// 範囲の初期化処理
	InitRange();

	// ポリゴンの初期化処理
	InitFLOOR();

	// ドームの初期化
	InitDome();

	// 半球の初期化
	InitHemisphere();

	// 壁の初期化処理
	InitWall();

	// 影の初期化処理
	InitShadow();

	// 予測線の初期化処理
	InitPrediction();

	// マテリアルの初期化処理
	InitMaterials();

	// プレイヤーの初期化処理
	InitPlayer();

	// アイテムの初期化処理
	InitItem();

	// キリンの初期化処理
	InitGiraffe();

	// キリンの初期化処理
	InitShogira();

	// バッファローの初期化処理
	InitBuffalo();

	// 水の初期化処理
	InitWATER();

	// HPゲージの初期化処理
	InitHpGauge();

	// チュートリアルコメント初期化
	InitTutorialComment();

	// 選択の初期化
	InitSelect();

	// 選択コメントの初期化
	InitSelectComment();

	// 選択コメントのタイプ
	SceneSelectComment(SELECTCOMMENTSCENE_TUTORIALS);

	// 矢印の初期化
	InitArrow();
}

// ------------------------------------------------------------------------------------------
//
// チュートリアル終了処理
//
// ------------------------------------------------------------------------------------------

void UninitTutorial(void)
{
	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// 爆発の終了処理
	UninitExplosion();

	// エフェクトの終了処理
	UninitEffect();

	// 弾の終了処理
	UninitBullet();

	// ビルボードの終了処理
	UninitBillboard();

	// 範囲の終了処理
	UninitRange();

	// 3Dポリゴンの終了処理
	UninitFLOOR();

	// ドームの終了処理
	UninitDome();

	// 半球の終了処理
	UninitHemisphere();

	// 壁の終了処理
	UninitWall();

	// 影の終了処理
	UninitShadow();

	// 予測線の終了処理
	UninitPrediction();

	// マテリアルの終了処理
	UninitMaterials();

	// プレイヤーの終了処理
	UninitPlayer();

	// アイテムの終了処理
	UninitItem();

	// キリンの終了処理
	UninitGiraffe();

	// キリンの終了処理
	UninitShogira();

	// バッファローの終了処理
	UninitBuffalo();

	// 水の終了処理
	UninitWATER();

	// 軌跡の終了処理
	UninitTrajectory();

	// HPゲージの終了処理
	UninitHpGauge();

	// チュートリアルコメント終了処理
	UninitTutorialComment();

	// 選択の終了処理
	UninitSelect();

	// 選択コメントの終了処理
	UninitSelectComment();

	// 矢印の終了処理
	UninitArrow();
}

// ------------------------------------------------------------------------------------------
//
// チュートリアル更新処理
//
// ------------------------------------------------------------------------------------------

void UpdateTutorial(void)
{
	switch (g_TutorialState)
	{
	case TUTORIALSSTATE_NORMAL:
		// カメラの更新処理
		UpdateCamera();

		// ライトの更新処理
		UpdateLight();

		// 爆発の更新処理
		UpdateExplosion();

		// エフェクトの更新処理
		UpdateEffect();

		// 弾の更新処理
		UpdateBullet();

		// ビルボードの更新処理
		UpdateBillboard();

		// 範囲の更新処理
		UpdateRange();

		// ポリゴンの更新処理
		UpdateFLOOR();

		// ドームの更新処理
		UpdateDome();

		// 半球の更新処理
		UpdateHemisphere();

		// 壁の更新処理
		UpdateWall();

		// 影の更新処理
		UpdateShadow();

		// 予測線の更新処理
		UpdatePrediction();

		// マテリアルの更新処理
		UpdateMaterials();

		// モデルの更新処理
		UpdatePlayer();

		// アイテムの更新処理
		UpdateItem();

		// キリンの更新処理
		UpdateGiraffe();

		// キリンの更新処理
		UpdateShogira();

		// バッファローの更新処理
		//UpdateBuffalo();

		// 水の更新処理
		UpdateWATER();

		// 軌跡の更新処理
		UpdateTrajectory();

		// HPゲージの更新処理
		UpdateHpGauge();

		// チュートリアルコメント更新処理
		UpdateTutorialComment();

		break;

	case TUTORIALSSTATE_SELECT:
		// 選択の更新処理
		UpdateSelect();

		// 選択コメントの更新処理
		UpdateSelectComment();

		// 矢印の更新処理
		UpdateArrow();

		break;

	default:
		break;
	}
}

// ------------------------------------------------------------------------------------------
//
// チュートリアル描画処理
//
// ------------------------------------------------------------------------------------------
void DrawTutorial(void)
{
	// カメラの設定
	SetCamera();

	// ポリゴンの描画処理
	DrawFLOOR();

	// 壁の描画処理
	DrawWall();

	// ドームの描画処理
	DrawDome();

	// 半球の描画処理
	DrawHemisphere();

	// 弾の描画処理
	DrawBullet();

	// 範囲の終了処理
	DrawRange();

	// 爆発の描画処理
	DrawExplosion();

	// エフェクトの描画処理
	DrawEffect();

	// 影の描画処理
	DrawShadow();

	// 予測線の描画処理
	DrawPrediction();

	// マテリアルの描画処理
	DrawMaterials();

	// モデルの描画処理
	DrawPlayer();

	// アイテムの描画処理
	DrawItem();

	// キリンの描画処理
	DrawGiraffe();

	// キリンの描画処理
	DrawShogira();

	// バッファローの描画処理
	DrawBuffalo();

	// ビルボードの描画処理
	DrawBillboard();

	// 軌跡の描画処理
	DrawTrajectory();

	// 水の描画処理
	DrawWATER();

	// HPゲージの描画処理
	DrawHpGauge();

	// チュートリアルコメント描画処理
	DrawTutorialComment();

	// 選択時
	if (g_TutorialState == TUTORIALSSTATE_SELECT)
	{
		// 選択の描画処理
		DrawSelect();

		// 選択コメントの描画処理
		DrawSelectComment();

		// 矢印の描画処理
		DrawArrow();
	}
}

// ------------------------------------------------------------------------------------------
//
// チュートリアル設定処理
//
// ------------------------------------------------------------------------------------------
void SetTutorialState(TUTORIALSSTATE state)
{
	g_TutorialState = state;
}

// ------------------------------------------------------------------------------------------
//
// 要素背景処理の説明[element.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------
#include "element.h"
#include "input.h"
#include "joypad.h"
#include "mouse.h"
#include "camera.h"
#include "light.h"
#include "materials.h"
#include "shadow.h"
#include "wall.h"
#include "bullet.h"
#include "floor.h"
#include "dome.h"
#include "explosion.h"
#include "effect.h"
#include "giraffe.h"
#include "hemisphere.h"
#include "trajectory.h"
#include "buffalo.h"
#include "item.h"
#include "king_rion.h"
#include "Billboard.h"
#include "range.h"
#include "water.h"
#include "shogira.h"
#include "prediction.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// 要素背景初期化処理
//
// ------------------------------------------------------------------------------------------
void InitElement(void)
{
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

	// アイテムの初期化処理
	InitItem();

	// キリンの初期化処理
	InitGiraffe();

	// キリンの初期化処理
	InitShogira();

	// バッファローの初期化処理
	InitBuffalo();

	// 百獣の王の初期化処理
	InitKing_Rion();

	// 水の初期化処理
	InitWATER();
}

// ------------------------------------------------------------------------------------------
//
// 要素背景終了処理
//
// ------------------------------------------------------------------------------------------
void UninitElement(void)
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

	// ビルボードの初期化処理
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

	// アイテムの終了処理
	UninitItem();

	// キリンの終了処理
	UninitGiraffe();

	// キリンの終了処理
	UninitShogira();

	// バッファローの終了処理
	UninitBuffalo();

	// 百獣の王の終了処理
	UninitKing_Rion();

	// 軌跡の終了処理
	UninitTrajectory();

	// 水の終了処理
	UninitWATER();
}

// ------------------------------------------------------------------------------------------
//
// 要素背景更新処理
//
// ------------------------------------------------------------------------------------------
void UpdateElement(void)
{
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

	// アイテムの更新処理
	UpdateItem();

	// キリンの更新処理
	UpdateGiraffe();

	// キリンの更新処理
	UpdateShogira();

	// バッファローの更新処理
	UpdateBuffalo();

	// 百獣の王の更新処理
	UpdateKing_Rion();

	// 軌跡の更新処理
	UpdateTrajectory();

	// 水の更新処理
	UpdateWATER();
}

// ------------------------------------------------------------------------------------------
//
// 要素背景描画処理
//
// ------------------------------------------------------------------------------------------
void DrawElement(void)
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

	// ビルボードの描画処理
	DrawBillboard();

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

	// アイテムの描画処理
	DrawItem();

	// キリンの描画処理
	DrawGiraffe();

	// キリンの描画処理
	DrawShogira();

	// バッファローのの描画処理
	DrawBuffalo();

	// 百獣の王の描画処理
	DrawKing_Rion();

	// 軌跡の描画処理
	DrawTrajectory();

	// 水の描画処理
	DrawWATER();
}
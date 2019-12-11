// ------------------------------------------------------------------------------------------
//
// Calculationヘッダー処理の説明[calculation.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

#ifndef _CALCULATION_H_
#define _CALCULATION_H_	 // ファイル名を基準を決める

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include "main.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// Calculation種類
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// 構造体定義
//
// ------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------------------------------------------------------
// 外積の計算 //
float Cross_product(
	D3DXVECTOR3 VecA,
	D3DXVECTOR3 VecB);

// 距離の計算 //
D3DXVECTOR3 Difference_Between(
	D3DXVECTOR3 posA,
	D3DXVECTOR3 posB);

// 四角形の中に入っているかいないか
bool SquareBottom_Judg(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 size,
	D3DXVECTOR3 rot,
	int			nBlock_W,
	int			nBlock_H,
	D3DXVECTOR3 worldpos
);

// 四角形の中に入っているかいないか
bool SquareSide_Judg(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 size,
	D3DXVECTOR3 rot,
	int			nBlock_W,
	int			nBlock_H,
	D3DXVECTOR3 worldpos
);

// 円の足し算 //
float Addition_Circle(
	float Length_x,
	float Length_y,
	float Length_z);

// 円の当たり判定 //
bool Collision_Circle(
	D3DXVECTOR3 Me,
	float		fMeSize,
	D3DXVECTOR3 Opponent,
	float		fOppSize);

// 回転量の上限 //
float Rot_One_Limit(
	float fRot);

// スクリーン座標をワールド座標に変換
D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* pout,		// マウスポイント
	float Sx,					// スクリーンX座標
	float Sy,					// スクリーンY座標
	float fZ,				// 射影空間でのZ値（0～1）
	float Screen_w,			// スクリーンの横幅
	float Screen_h,			// スクリーンの高さ
	D3DXMATRIX* mtxView,	// ビューマトリックス
	D3DXMATRIX* mtxPrj);	// プロジェクションマトリックス

							// スクリーン座標をXZ平面のワールド座標交点算出
D3DXVECTOR3* CalcScreenToXZ(
	D3DXVECTOR3* pout,		// マウスポイント
	float Sx,				// スクリーンX座標
	float Sy,				// スクリーンY座標
	float Screen_w,			// スクリーンの横幅
	float Screen_h,			// スクリーンの高さ
	D3DXMATRIX* mtxView,	// ビューマトリックス
	D3DXMATRIX* mtxPrj,		// プロジェクションマトリックス
	D3DXVECTOR3 obj);		// オブジェクトの位置

							// ワールド座標をスクリーン座標に変換(多分)
D3DXVECTOR3* CalcWorldToScreen(
	D3DXVECTOR3* Obj,		// オブジェクトの位置
	D3DXMATRIX Wobj,		// ワールド座標上でのオブジェクトのマトリックス
	float Screen_w,			// スクリーンの横幅
	float Screen_h,			// スクリーンの高さ
	D3DXMATRIX* mtxView,	// ビューマトリックス
	D3DXMATRIX* mtxPrj);	// プロジェクションマトリックス

#endif
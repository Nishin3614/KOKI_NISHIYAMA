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
// クラス
//
// ------------------------------------------------------------------------------------------
class CCalculation
{
public:
	/* 関数 */
	// 距離の計算 //
	static D3DXVECTOR3 Difference_Between(
		D3DXVECTOR3 &posA,	// 位置A
		D3DXVECTOR3 &posB	// 位置B
	);

	// 外積の計算 //
	static float Cross_product(
		D3DXVECTOR3 &VecA,	// 位置A
		D3DXVECTOR3 &VecB	// 位置B
	);

	// 四角形の中に入っているかいないか
	static bool SquareBottom_Judg(
		D3DXVECTOR3 &pos,		// 位置
		D3DXVECTOR3 &size,		// サイズ
		D3DXVECTOR3 &rot,		// 回転量
		int			nBlock_W,	// 横ブロック
		int			nBlock_H,	// 縦ブロック
		D3DXVECTOR3 &worldpos	// ワールド座標
	);

	// 四角形の中に入っているかいないか
	static bool SquareSide_Judg(
		D3DXVECTOR3 &pos,		// 位置
		D3DXVECTOR3 &size,		// サイズ
		D3DXVECTOR3 &rot,		// 回転量
		int			nBlock_W,	// 横ブロック
		int			nBlock_H,	// 縦ブロック
		D3DXVECTOR3 &worldpos	// ワールド座標
	);

	// 円の足し算 //
	static float Addition_Circle(
		float Length_x,	// x
		float Length_y,	// y
		float Length_z	// z
	);

	// 円の当たり判定 //
	static bool Collision_Circle(
		D3DXVECTOR3 &Me,		// 自分の位置
		float		fMeSize,	// 半径
		D3DXVECTOR3 &Opponent,	// 相手の位置
		float		fOppSize	// 半径
	);

	// 回転量の上限 //
	static float Rot_One_Limit(
		float &fRot	// 回転量
	);

	// スクリーン座標をワールド座標に変換
	static D3DXVECTOR3* CalcScreenToWorld(
		D3DXVECTOR3* pout,		// マウスポイント
		float Sx,				// スクリーンX座標
		float Sy,				// スクリーンY座標
		float fZ,				// 射影空間でのZ値（0～1）
		float Screen_w,			// スクリーンの横幅
		float Screen_h,			// スクリーンの高さ
		D3DXMATRIX* mtxView,	// ビューマトリックス
		D3DXMATRIX* mtxPrj		// プロジェクションマトリックス
	);
	// スクリーン座標をXZ平面のワールド座標交点算出
	static D3DXVECTOR3* CalcScreenToXZ(
		D3DXVECTOR3* pout,		// マウスポイント
		float Sx,				// スクリーンX座標
		float Sy,				// スクリーンY座標
		float Screen_w,			// スクリーンの横幅
		float Screen_h,			// スクリーンの高さ
		D3DXMATRIX* mtxView,	// ビューマトリックス
		D3DXMATRIX* mtxPrj,		// プロジェクションマトリックス
		D3DXVECTOR3 &obj		// オブジェクトの位置
	);
	// ワールド座標をスクリーン座標に変換(多分)
	static D3DXVECTOR3* CalcWorldToScreen(
		D3DXVECTOR3* Obj,		// オブジェクトの位置
		float Sx,				// スクリーンX座標
		float Sy,				// スクリーンY座標
		float fZ,				// 射影空間でのZ値（0～1）
		float Screen_w,			// スクリーンの横幅
		float Screen_h,			// スクリーンの高さ
		D3DXMATRIX* mtxView,	// ビューマトリックス
		D3DXMATRIX* mtxPrj		// プロジェクションマトリックス
	);
	// 一次関数
	static D3DXVECTOR2 Linear_function(
		D3DXVECTOR3 &Apos,	// 始点
		D3DXVECTOR3 &Bpos	// 終点
	);
	// 2線分の交点(連立方程式)
	static D3DXVECTOR3 TwoLine_Inse(
		D3DXVECTOR2 &ALinear,	// 線①
		D3DXVECTOR2 &BLinear	// 線②
	);
protected:

private:
	/* 変数 */
};

// ------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------------------------------------------------------

#endif
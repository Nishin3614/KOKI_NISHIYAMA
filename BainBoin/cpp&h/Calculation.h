// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
//
// Calculationヘッダー処理の説明[calculation.h]
// Author : Koki Nishiyama
//
// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

#ifndef _CALCULATION_H_
#define _CALCULATION_H_	 // ファイル名を基準を決める

// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
//
// インクルードファイル
//
// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
//
// マクロ定義
//
// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
//
// Calculation種類
//
// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
//
// 構造体定義
//
// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

// ------------------------------------------
//
// 名前定義
//
// ------------------------------------------
// 書き換え不可の文字ポインター
typedef const char * P_CST_CHAR;

typedef struct INTEGER2
{
	INTEGER2() {}
	INTEGER2(int X, int Y)
	{
		nMax = X;
		nMin = Y;
	}
	int nMax;
	int	nMin;
}INTEGER2, *PINTEGER2;

typedef struct STATUS_INFO
{
	STATUS_INFO() {}
	STATUS_INFO(float fsJumpforce, float	fsRateJumpforce, float fsRotforce, float fsRateRotforce)
	{
		fJumpforce		= fsJumpforce;		// ジャンプ力
		fRateJumpforce	= fsRateJumpforce;	// ジャンプ力の上昇率
		fRotforce		= fsRotforce;		// 回転力
		fRateRotforce	= fsRateRotforce;	// 回転力の上昇率
	}
	float	fJumpforce;				// ジャンプ力
	float	fRateJumpforce;			// ジャンプ力の上昇率
	float	fRotforce;				// 回転力
	float	fRateRotforce;			// 回転力の上昇率
} STATUS_INFO;

// ------------------------------------------
//
// クラス
//
// ------------------------------------------

class CCalculation
{
public:
	/* 関数 */
	// 距離の計算 //

	static D3DXVECTOR3 Difference_Between(
		D3DXVECTOR3 posA,
		D3DXVECTOR3 posB);

	// 外積の計算 //
	static D3DXVECTOR3 Cross_product(
		D3DXVECTOR3 VecA,
		D3DXVECTOR3 VecB);

	// 内積の計算 //
	static float Dot_product(
		D3DXVECTOR3 VecA,
		D3DXVECTOR3 VecB);

	// 四角形の中に入っているかいないか
	static bool SquareBottom_Judg(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 size,
		D3DXVECTOR3 rot,
		int			nBlock_W,
		int			nBlock_H,
		D3DXVECTOR3 worldpos
	);

	// 四角形の中に入っているかいないか
	static bool SquareSide_Judg(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 size,
		D3DXVECTOR3 rot,
		int			nBlock_W,
		int			nBlock_H,
		D3DXVECTOR3 worldpos
	);

	// 円の足し算 //
	static float Addition_Circle(
		float Length_x,
		float Length_y,
		float Length_z
	);

	// 円の当たり判定 //
	static bool Collision_Circle(
		D3DXVECTOR3 Me,
		float		fMeSize,
		D3DXVECTOR3 Opponent,
		float		fOppSize
	);

	// 回転量の上限 //
	static float Rot_One_Limit(
		float fRot);

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
		D3DXVECTOR3 obj);		// オブジェクトの位置

	// ワールド座標をスクリーン座標に変換(多分)
	static D3DXVECTOR3* CalcWorldToScreen(
		D3DXVECTOR3* Obj,		// オブジェクトの位置
		float Sx,					// スクリーンX座標
		float Sy,					// スクリーンY座標
		float fZ,				// 射影空間でのZ値（0～1）
		float Screen_w,			// スクリーンの横幅
		float Screen_h,			// スクリーンの高さ
		D3DXMATRIX* mtxView,	// ビューマトリックス
		D3DXMATRIX* mtxPrj);	// プロジェクションマトリックス

	// 一次関数
	static D3DXVECTOR2 Linear_function(
		D3DXVECTOR3 Apos,			// 始点
		D3DXVECTOR3 Bpos);			// 終点

	// 2線分の交点(連立方程式)
	static D3DXVECTOR3 TwoLine_Inse(
		D3DXVECTOR2 ALinear,	// 線①
		D3DXVECTOR2 BLinear);	// 線②

	// 途中の計算
	static bool TlyCollision(
		D3DXVECTOR3 ObjectPos,
		D3DXVECTOR3,
		...
		);
	// 外積の当たり判定
	static bool CrossCollision(
		D3DXVECTOR3 *ObjectPos,
		D3DXVECTOR3 *PosA,
		D3DXVECTOR3 *PosB,
		D3DXVECTOR3 *PosC
	);
protected:

private:
	/* 変数 */
};

// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
//
// プロトタイプ宣言
//
// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

#endif
// ------------------------------------------------------------------------------------------
//
// Calculation処理の説明[calculation.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------
#include "Calculation.h"

// ------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// 外積の計算
// ------------------------------------------------------------------------------------------
float Cross_product(
	D3DXVECTOR3 VecA,
	D3DXVECTOR3 VecB)
{
	return VecA.z * VecB.x - VecA.x * VecB.z;
}

// ------------------------------------------------------------------------------------------
// 底面の四角形の中に入っているかいないか
// ------------------------------------------------------------------------------------------
bool SquareBottom_Judg(
	D3DXVECTOR3 pos,		
	D3DXVECTOR3 size, 
	D3DXVECTOR3 rot, 
	int			nBlock_W,
	int			nBlock_H,
	D3DXVECTOR3 worldpos
)
{
	// 変数宣言
	D3DXVECTOR3 SquareDiff[4];
	D3DXVECTOR3 WorldDiff[4];
	float		fCross_p[4];
	bool		bCross = false;

	// 左の距離
	SquareDiff[0] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H));

	// 上の距離
	SquareDiff[1] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H));

	// 右の距離
	SquareDiff[2] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H));

	// 下の距離
	SquareDiff[3] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H));

	// 左の距離
	WorldDiff[0] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		worldpos);

	// 上の距離
	WorldDiff[1] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		worldpos);

	// 右の距離
	WorldDiff[2] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		worldpos);

	// 下の距離
	WorldDiff[3] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		worldpos);

	// 外積の中か外か
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		// 外積
		fCross_p[nCnt] = Cross_product(SquareDiff[nCnt], WorldDiff[nCnt]);

		// 外積の判定 //
		// 中
		if (fCross_p[nCnt] > 0)
		{
			bCross = true;
		}

		// 外
		else
		{
			return false;
		}
	}

	// 中か外かを返す
	return bCross;
}

// ------------------------------------------------------------------------------------------
// 側面の四角形の中に入っているかいないか
// ------------------------------------------------------------------------------------------
bool SquareSide_Judg(
	D3DXVECTOR3 pos, 
	D3DXVECTOR3 size,
	D3DXVECTOR3 rot, 
	int nBlock_W, 
	int nBlock_H, 
	D3DXVECTOR3 worldpos)
{
	// 高さの範囲
	if (worldpos.y < pos.y + size.y * nBlock_H &&
		worldpos.y >= pos.y)
	{
		// 横の範囲
		if (worldpos.x > pos.x - size.x * 0.5f * nBlock_W * sinf(rot.y) &&
			worldpos.x < pos.x + size.x * 0.5f * nBlock_W * sinf(rot.y))
		{
			return true;
		}
		// 奥の範囲
		if (worldpos.z > pos.z - size.x * 0.5f * nBlock_W * cosf(rot.y) &&
			worldpos.z < pos.z + size.x * 0.5f * nBlock_W * cosf(rot.y))
		{
			return true;
		}
	}


	// 中か外かを返す
	return false;
}

// ------------------------------------------------------------------------------------------
// 座標同士の差計算
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 Difference_Between(
	D3DXVECTOR3 posA,
	D3DXVECTOR3 posB)
{
	// 差
	D3DXVECTOR3 Diff;

	//座標同士の差計算
	Diff = posA - posB;

	return Diff;
}

// ------------------------------------------------------------------------------------------
// 円の足し算計算
// ------------------------------------------------------------------------------------------
float Addition_Circle(
	float Length_x, 
	float Length_y, 
	float Length_z)
{
	// 変数宣言
	float fLengthX = 0;
	float fLengthY = 0;
	float fLengthZ = 0;
	float Lenght = 0;

	// 円の計算
	fLengthX = Length_x * Length_x;
	fLengthY = Length_y * Length_y;
	fLengthZ = Length_z * Length_z;

	// 円同士の足し算
	Lenght = fLengthX + fLengthY + fLengthZ;

	return Lenght;
}

// ------------------------------------------------------------------------------------------
// 円の当たり判定
// ------------------------------------------------------------------------------------------
bool Collision_Circle(D3DXVECTOR3 Me, float fMeSize, D3DXVECTOR3 Opponent, float fOppSize)
{
	// 変数宣言
	float fRange;		// 範囲
	float fDistance;	// 距離

						// 範囲の計算
	fRange = fMeSize * fMeSize + fOppSize * fOppSize;

	// 実際の距離の計算
	fDistance =
		(Me.x - Opponent.x) * (Me.x - Opponent.x) +
		(Me.y - Opponent.y) * (Me.y - Opponent.y) +
		(Me.z - Opponent.z) * (Me.z - Opponent.z);

	// 当たっているかいないかを返す
	return (fRange >= fDistance) ? true : false;
}

// ------------------------------------------------------------------------------------------
// 回転の限界計算
// ------------------------------------------------------------------------------------------
float Rot_One_Limit(float fRot)
{
	// -3.14〜3.14に回転差分を固定させる
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2;		// -3.14未満
	}

	else if (fRot > D3DX_PI)
	{
		fRot += -D3DX_PI * 2;	// 3.14超過
	}

	return fRot;
}

// ------------------------------------------------------------------------------------------
// スクリーン座標をワールド座標に変換
// ------------------------------------------------------------------------------------------
D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* pout,	// マウスポイント
	float Sx,				// スクリーンX座標
	float Sy,				// スクリーンY座標
	float fZ,			// 射影空間でのZ値（0〜1）
	float Screen_w,		// スクリーンの横幅
	float Screen_h,		// スクリーンの高さ
	D3DXMATRIX* mtxView,// ビューマトリックス
	D3DXMATRIX* mtxPrj)	// プロジェクションマトリックス
{
	// 各行列の逆行列を算出 //
	// 変数宣言
	D3DXMATRIX mtxInvViex;		// 計算用逆行列ビューマトリックス
	D3DXMATRIX mtxInvPrj;		// 計算用逆行列プロジェクションマトリックス
	D3DXMATRIX mtxVP;			// 計算用ビューポット
	D3DXMATRIX mtxInvViexport;	// 計算用逆行列ビューポット
	D3DXMATRIX mtxChange;		// 座標変換用マトリックス

	// ビューポットの行列作成
	D3DXMatrixIdentity(&mtxVP);	// 初期化
	mtxVP._11 = Screen_w / 2.0f;
	mtxVP._22 = -Screen_h / 2.0f;
	mtxVP._41 = Screen_w / 2.0f;
	mtxVP._42 = Screen_h / 2.0f;

	// 逆行列の計算
	D3DXMatrixInverse(&mtxInvViex, NULL, mtxView);		// ビューマトリックス
	D3DXMatrixInverse(&mtxInvPrj, NULL, mtxPrj);		// プロジェクションマトリックス
	D3DXMatrixInverse(&mtxInvViexport, NULL, &mtxVP);	// ビューポットマトリックス

	// スクリーンからワールドへ
	mtxChange = mtxInvViexport * mtxInvPrj * mtxInvViex;		// 座標変換用
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3(Sx, Sy, fZ), &mtxChange);	// スクリーンからワールドへ
	
	// ワールドのポイントを返す
	return pout;
}

// ------------------------------------------------------------------------------------------
// スクリーン座標をXZ平面のワールド座標交点算出
// ------------------------------------------------------------------------------------------
D3DXVECTOR3* CalcScreenToXZ(
	D3DXVECTOR3* pout,		// ワールド座標の入れ物
	float Sx,				// スクリーンX座標
	float Sy,				// スクリーンY座標
	float Screen_w,			// スクリーンの横幅
	float Screen_h,			// スクリーンの高さ
	D3DXMATRIX* mtxView,	// ビューマトリックス
	D3DXMATRIX* mtxPrj,		// プロジェクションマトリックス
	D3DXVECTOR3 obj			// オブジェクトの位置
)
{				
	// 変数宣言
	D3DXVECTOR3 Mearpos;	// 最近値
	D3DXVECTOR3 Farpos;		// 最遠値
	D3DXVECTOR3 Ray;		// マウスポイント方向

	// スクリーン座標をワールド座標に変換
	CalcScreenToWorld(		// 最近値
		&Mearpos, 
		Sx, Sy,
		0.0f,
		Screen_w, Screen_h,
		mtxView,
		mtxPrj);	

	CalcScreenToWorld(		// 最遠値
		&Farpos, 
		Sx, Sy,
		1.0f,
		Screen_w, Screen_h,
		mtxView,
		mtxPrj);	
	
	// マウスポイント方向
	Ray = Farpos - Mearpos;
	D3DXVec3Normalize(&Ray, &Ray);	// 正規化

	D3DXVECTOR3 diff;

	// 改善する必要がある
	diff = Mearpos - obj;
	diff = -(*D3DXVec3Normalize(&diff, &diff));

	// 下床との交点 //
	if (Ray.y <= 0)
	{
		// 床交点
		float LRay = D3DXVec3Dot(&Ray, &D3DXVECTOR3(0, 1, 0));		// マウスの方向の内積
		float LP0 = D3DXVec3Dot(&(obj - Mearpos), &D3DXVECTOR3(0, 1, 0));	// 原点の内積
		*pout = Mearpos + (LP0 / LRay)*Ray;
	}

	/* 起きていない場合は遠くの壁との交点を出力	*/
	// 上床との交点 //
	else
	{
		// 床交点
		float LRay = D3DXVec3Dot(&Ray, &D3DXVECTOR3(0, -1, 0));		// マウスの方向の内積
		float LP0 = D3DXVec3Dot(&(obj - Mearpos), &D3DXVECTOR3(0, -1, 0));	// 原点の内積
		*pout = Mearpos + (LP0 / LRay)*Ray;

		//*pout = Farpos;
	}

	return pout;
}

// ------------------------------------------------------------------------------------------
// ワールド座標をスクリーン座標に変換(多分)
// ------------------------------------------------------------------------------------------
D3DXVECTOR3* CalcWorldToScreen(
	D3DXVECTOR3* Obj,	// オブジェクトの位置
	D3DXMATRIX Wobj,	// ワールド座標上のオブジェクトの位置
	float Screen_w,		// スクリーンの横幅
	float Screen_h,		// スクリーンの高さ
	D3DXMATRIX* mtxView,// ビューマトリックス
	D3DXMATRIX* mtxPrj)	// プロジェクションマトリックス
{
	// 各行列の逆行列を算出 //
	// 変数宣言
	D3DXMATRIX mtxVP;			// 計算用ビューポット
	D3DXMATRIX mtxChange;				// 

	// ビューポットの行列作成
	D3DXMatrixIdentity(&mtxVP);			// 初期化
	mtxVP._11 = SCREEN_WIDTH / 2.0f;
	mtxVP._22 = -SCREEN_HEIGHT / 2.0f;
	mtxVP._41 = SCREEN_WIDTH / 2.0f;
	mtxVP._42 = SCREEN_HEIGHT / 2.0f;

	mtxChange = Wobj * mtxVP * *mtxPrj * *mtxView;

	return Obj;
}
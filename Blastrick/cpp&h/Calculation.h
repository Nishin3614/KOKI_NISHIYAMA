// ----------------------------------------------------------------------------------------------------
//
// Calculationヘッダー処理の説明[calculation.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

#ifndef _CALCULATION_H_
#define _CALCULATION_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

// ----------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------
// D3DVECTOR2型の初期化(1.0f)
#define D3DVECTOR2_INI			D3DXVECTOR2(1.0f,1.0f)
// D3DVECTOR2型の初期化(0.0f)
#define D3DVECTOR2_ZERO			D3DXVECTOR2(0.0f,0.0f)
// D3DVECTOR3型の初期化(0.0f)
#define D3DVECTOR3_ZERO			D3DXVECTOR3(0.0f,0.0f,0.0f)
// D3DVECTOR3型の初期化(1.0f)
#define D3DVECTOR3_ONE			D3DXVECTOR3(1.0f,1.0f,1.0f)
// D3DVECTOR4型の初期化(0.0f)
#define D3DVECTOR4_ZERO			D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f)
// D3DXCOLOR型の初期化
#define D3DXCOLOR_INI			D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)
// D3DXCOLOR型の初期化(基本色)
#define D3DXCOLOR_C(col)		D3DXCOLOR(col,col,col,1.0f)
// D3DXCOLOR型の初期化(基本色,透明度)
#define D3DXCOLOR_CA(col,alpha)	D3DXCOLOR(col,col,col,alpha)
// ファイル読み込み最大行数
#define FILELINE_ERROW (1048576)

// ----------------------------------------------------------------------------------------------------
//
// Calculation種類
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// 列挙型定義
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// 構造体定義
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// 名前定義
//
// ----------------------------------------------------------------------------------------------------
// 書き換え不可の文字ポインター
typedef const char * P_CST_CHAR;

// 整数型2個
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

// 整数型3個
typedef struct INTEGER3
{
public:
	INTEGER3() {}
	INTEGER3(int X, int Y, int Z)
	{
		nX = X;
		nY = Y;
		nZ = Z;
	}
	// キャスト
	inline operator int* ()
	{
		(int *)nX;
		(int *)nY;
		(int *)nX;
		return *this;
	}
	inline operator CONST int* () const
	{
		(CONST int *)nX;
		(CONST int *)nY;
		(CONST int *)nZ;
		return *this;
	}
	// 代入演算子
	INTEGER3& operator += (CONST INTEGER3& Source)
	{
		nX += Source.nX;
		nY += Source.nY;
		nZ += Source.nZ;
		return *this;
	}
	INTEGER3& operator -= (CONST INTEGER3& Source)
	{
		nX -= nX;
		nY -= nY;
		nZ -= nZ;
		return *this;
	}
	INTEGER3& operator *= (int Source)
	{
		nX *= Source;
		nY *= Source;
		nZ *= Source;
		return *this;
	}
	INTEGER3& operator /= (int Source)
	{
		nX /= Source;
		nY /= Source;
		nZ /= Source;
		return *this;
	}
	// 2項演算子
	INTEGER3 operator + (CONST INTEGER3& Source) const
	{
		// 値を渡す変数
		INTEGER3 variable;
		variable.nX = nX + Source.nX;
		variable.nY = nY + Source.nY;
		variable.nZ = nZ + Source.nZ;
		return variable;
	}
	INTEGER3 operator - (CONST INTEGER3& Source) const
	{
		// 値を渡す変数
		INTEGER3 variable;
		variable.nX = nX - Source.nX;
		variable.nY = nY - Source.nY;
		variable.nZ = nZ - Source.nZ;
		return variable;
	}
	bool operator == (CONST INTEGER3& Source) const
	{
		return Source == INTEGER3(nX, nY, nZ);
	}
	bool operator != (CONST INTEGER3& Source) const
	{
		return Source != INTEGER3(nX, nY, nZ);
	}

	int nX,		// X軸成分
		nY,		// Y軸成分
		nZ;		// Z軸成分
} INTEGER3, *PINTEGER3;

// 整数型4個
typedef struct INTEGER4
{
	INTEGER4() {}
	INTEGER4(
		int d_X, 
		int d_Y,
		int d_Z,
		int d_W
	)
	{
		X = d_X;
		Y = d_Y;
		Z = d_Z;
		W = d_W;
	}
	int X;
	int	Y;
	int Z;
	int	W;
}INTEGER4, *PINTEGER4;

// 配置物情報(1:タイプ,2:位置,3:回転)
typedef struct ARRANGEMENTOBJ
{
	ARRANGEMENTOBJ() {}
	ARRANGEMENTOBJ(
		int d_nType,
		D3DXVECTOR3 d_pos,
		D3DXVECTOR3 d_rot
	)
	{
		nType = d_nType;
		pos = d_pos;
		rot = d_rot;
	}
	int			nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
} ARRANGEMENTOBJ,*pARRANGEMENTOBJ;

// 配置物情報(1:タイプ,2:位置,3:回転4:縦ブロック,5:横ブロック,6:サイズA,7:サイズB)
typedef struct ARRANGEMENTMESH
{
	ARRANGEMENTMESH() {}
	ARRANGEMENTMESH(
		int d_nType,
		D3DXVECTOR3 d_pos,
		D3DXVECTOR3 d_rot,
		int			d_nBlockDepth,
		int			d_nBlockWidth,
		float		d_fSizeA,
		float		d_fSizeB
	)
	{
		nType = d_nType;
		pos = d_pos;
		rot = d_rot;
		nBlockDepth = d_nBlockDepth;
		nBlockWidth = d_nBlockWidth;
		fSizeA = d_fSizeA;
		fSizeB = d_fSizeB;
	}
	int			nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int			nBlockDepth;
	int			nBlockWidth;
	float		fSizeA;
	float		fSizeB;
} ARRANGEMENTMESH, *pARRANGEMENTMESH;

// ----------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------
#define INTEGER4_INI			INTEGER4(1,1,1,1)				

// ----------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------

class CCalculation
{
public:
	/* 関数 */
	// 距離の計算
	static D3DXVECTOR3 Difference_Between(
		D3DXVECTOR3 &posA,
		D3DXVECTOR3 &posB
	);
	// 間の差の計算(3D用:位置、位置)
	static float DiffPoint(
		D3DXVECTOR3 const & posA,
		D3DXVECTOR3 const & posB
	);
	// 間の差の計算(2D用:位置、位置)
	static float DiffPoint(
		D3DXVECTOR2 const & posA,
		D3DXVECTOR2 const & posB
	);

	// 外積の計算
	static D3DXVECTOR3 Cross_product(
		D3DXVECTOR3 &VecA,
		D3DXVECTOR3 &VecB
	);
	// 内積の計算
	static float Dot_product(
		D3DXVECTOR3 &VecA,
		D3DXVECTOR3 &VecB
	);
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

	// 円の足し算
	static float Addition_Circle(
		float Length_x,
		float Length_y,
		float Length_z
	);
	// 円の当たり判定
	static bool Collision_Circle(
		D3DXVECTOR3 &Me,
		float		fMeSize,
		D3DXVECTOR3 &Opponent,
		float		fOppSize
	);
	// 回転量の上限 //
	static float Rot_One_Limit(
		float &fRot
	);
	// スクリーン座標をワールド座標に変換
	static D3DXVECTOR3* CalcScreenToWorld(
		D3DXVECTOR3* pout,		// マウスポイント
		float Sx,				// スクリーンX座標
		float Sy,				// スクリーンY座標
		float fZ,				// 射影空間でのZ値（0〜1）
		float Screen_w,			// スクリーンの横幅
		float Screen_h,			// スクリーンの高さ
		D3DXMATRIX* mtxView,	// ビューマトリックス
		D3DXMATRIX* mtxPrj	// プロジェクションマトリックス
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
		D3DXVECTOR3 obj			// オブジェクトの位置
	);	
	// ワールド座標をスクリーン座標に変換(多分)
	static D3DXVECTOR3* CalcWorldToScreen(
		D3DXVECTOR3* Obj,		// オブジェクトの位置
		float Sx,					// スクリーンX座標
		float Sy,					// スクリーンY座標
		float fZ,				// 射影空間でのZ値（0〜1）
		float Screen_w,			// スクリーンの横幅
		float Screen_h,			// スクリーンの高さ
		D3DXMATRIX* mtxView,	// ビューマトリックス
		D3DXMATRIX* mtxPrj	// プロジェクションマトリックス
	);
	// 一次関数
	static D3DXVECTOR2 Linear_function(
		D3DXVECTOR3 Apos,			// 始点
		D3DXVECTOR3 Bpos			// 終点
	);
	// 2線分の交点(連立方程式)
	static D3DXVECTOR3 TwoLine_Inse(
		D3DXVECTOR2 ALinear,	// 線�@
		D3DXVECTOR2 BLinear	// 線�A
	);
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
	// メッセージ発生
	static void Messanger(
		const char * cMessa	// メッセージ内容
	);
	// キー文字の反映
	static void KeyString(
		int nKeyID,				// キーID
		char *KeyName = NULL	// 文字
	);
	// ファイルが存在しているか
	static bool Existenceofscript(FILE * pFile);

	// モデルのテキストデータ読込
	static void MappingText(
		int const *pmodelAll,					// モデルとモーションの
		int const &nMaxMotion,					// モーション数
		int &nMaxkey,							// キー数
		const char * file_name					// ファイル名
	);

protected:

private:
	/* 変数 */
};

// ----------------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ----------------------------------------------------------------------------------------------------

#endif
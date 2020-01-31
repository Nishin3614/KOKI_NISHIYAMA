//=============================================================================
//
// モデル情報処理 [model_info.h]
// Author : koki_nishiyama
//
//=============================================================================
#ifndef _MODEL_INFO_H_
#define _MODEL_INFO_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "Calculation.h"

//=============================================================================
// 前方宣言
//=============================================================================

//=============================================================================
// 構造体
//=============================================================================
// キー
typedef struct KEY
{
	KEY()
	{
		pos = D3DVECTOR3_ZERO;
		rot = D3DVECTOR3_ZERO;
	}
	D3DXVECTOR3 pos;						// 位置
	D3DXVECTOR3 rot;						// 回転
} KEY;

// キー情報
typedef struct KEY_INFO
{
	KEY_INFO()
	{
		nFrame = 0;
		nNumCollision = 0;
		nMaxCollisiontime = 0;
		nBlust = 0;
		nCameraType = 0;
		Key = NULL;
	}
	int	nFrame;								// フレーム
	int nNumCollision;						// 当たり判定の個数
	int nMaxCollisiontime;					// 当たり判定の頻度
	int nBlust;								// 吹っ飛び方
	int nCameraType;						// カメラタイプ
	KEY *Key;								// キー
} KEY_INFO;

// モージョン情報
typedef struct MOTION
{
	MOTION() 
	{
		nLoop = 0;						// ループ
		nNumKey = 0;					// キー情報の数
		nAllFrame = 0;					// モーション全体のフレーム数
		Collision_nParts = 0;			// パーツ番号
		Collision_Ofset = D3DVECTOR3_ZERO;			// オフセット
		Collision_Radius = 0;			// 円の半径
		Collision_StartFram = 0;		// 開始フレーム数
		Collision_EndFram = 0;			// 終了フレーム数
		Collision_Damage = 0;			// ダメージ数
		KeyInfo = NULL;					// キー情報(モーション数)
	}
	int			nLoop;						// ループ
	int			nNumKey;					// キー情報の数
	int			nAllFrame = 0;					// モーション全体のフレーム数
	// 当たり判定用 //
	int			Collision_nParts;			// パーツ番号
	D3DXVECTOR3	Collision_Ofset;			// オフセット
	float		Collision_Radius;			// 円の半径
	int			Collision_StartFram;		// 開始フレーム数
	int			Collision_EndFram;			// 終了フレーム数
	int			Collision_Damage;			// ダメージ数

	KEY_INFO	*KeyInfo;					// キー情報(モーション数)
} MOTION;

// オフセットの情報
typedef struct MODEL_OFFSET
{
	MODEL_OFFSET()
	{
		pos = D3DVECTOR3_ZERO;
		rot = D3DVECTOR3_ZERO;
		nParent = 0;
	}
	char		cXfile[64];					// モデルのファイル名
	D3DXVECTOR3	pos;						// 位置
	D3DXVECTOR3	rot;						// 回転
	int			nParent;					// 親番号
} MODEL_OFFSET;

// モデル・モーションのの情報
typedef struct
{
	MODEL_OFFSET	*pModel_offset;			// モデルのオフセット
	MOTION			*pMotion;				// モーション
} MODEL_ALL;

// モデル情報
typedef struct
{
	LPD3DXMESH			pMesh;				// メッシュ情報へのポインタ
	LPD3DXBUFFER		pBuffMat;			// マテリアル情報へのポインタ
	LPDIRECT3DTEXTURE9	*ppTexture;			// テクスチャへのダブルポインター
	DWORD				nNumMat;			// マテリアルの数
	D3DXVECTOR3			vtxMinMaterials;	// モデル情報の位置の最小値
	D3DXVECTOR3			vtxMaxMaterials;	// モデル情報の位置の最大値
	D3DXVECTOR3			pos;				// 位置情報
	D3DXVECTOR3			posDest;			// 目的位置
	D3DXVECTOR3			posDiff;			// 差位置
	D3DXVECTOR3			rot;				// 回転情報
	D3DXVECTOR3			rotDest;			// 目的回転
	D3DXVECTOR3			rotDiff;			// 差回転
	D3DXVECTOR3			size;				// サイズ
	int					nParent;			// 親番号
} XMODEL;


//=============================================================================
// クラス
//=============================================================================
class CModel_info
{
public:
	/* 関数 */
	CModel_info();
	~CModel_info();
	// 開放
	void Unload(void);
	// 情報取得
	// 読み込み
	HRESULT Load(	// モデル情報
		LPDIRECT3DDEVICE9 pDevice,				// デバイス
		const char* charModel);					// ファイル名
	// モデルのテキストデータ読込
	static void TextLoad(
		MODEL_ALL	*pmodelAll,					// モデルとモーションの
		int const &nMaxMotion,					// モーション数
		int &nMaxkey,							// キー数
		const char * file_name					// ファイル名
	);
	// 読み込んだモデルのテキストデータの破棄
	static void TextUnload(
		MODEL_ALL	*pmodelAll,					// モデルとモーションの
		int const &nMaxMotion					// モーション数
	);
	// 
	// xモデルの情報取得
	XMODEL	&GetXModel(void);					// Xモデル情報取得
protected:
private:
	XMODEL				m_Xmodel;				// Xモデル情報
};

#endif

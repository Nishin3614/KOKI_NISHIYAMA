// ------------------------------------------
//
// モーション処理 [motion.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#ifndef _MOTION_H_
#define _MOTION_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------
// インクルードファイル
// ------------------------------------------
#include "manager.h"
#include "model_info.h"

//=============================================================================
// 構造体
//=============================================================================
// キー
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 回転
} KEY;

// キー情報
typedef struct
{
	int	nFrame;	// フレーム
	KEY *Key;	// キー
} KEY_INFO;

// モージョン情報
typedef struct
{
	int			nLoop;					// ループ
	int			nNumKey;				// キー情報の数

	// 当たり判定用 //
	int			Collision_nParts;		// パーツ番号
	D3DXVECTOR3	Collision_Ofset;		// オフセット
	float		Collision_Radius;		// 円の半径
	int			Collision_StartFram;	// 開始フレーム数
	int			Collision_EndFram;		// 終了フレーム数
	int			Collision_Damage;		// ダメージ数

	// 軌跡用 //
	int			Orbit_nParts;			// パーツ番号
	D3DXVECTOR3	Orbit_Ofset;			// オフセット
	int			Orbit_Mesh;				// 軌跡の線
	int			Orbit_StartFram;		// 開始フレーム数
	int			Orbit_EndFram;			// 終了フレーム数
	int			Orbit_nType;			// タイプ
	int			Orbit_nIdTrajectory;	// 軌跡の番号
	bool		Orbit_bUse;				// 軌跡の使用状態
	KEY_INFO	*KeyInfo;				// キー情報(モーション数)
} MOTION;

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CMotion
{
public:
	/* 関数 */
	CMotion();
	~CMotion();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// 作成
	static CMotion * Create();		
	static void Load(
		MOTION * pMotion,
		MODEL_OFFSET * pOffset,
		int nMaxkey,
		const char * file_name
	);
	static void UnLoad(
		MOTION * pMotion,
		int nMaxMotion
	);
protected:
private:
};
#endif
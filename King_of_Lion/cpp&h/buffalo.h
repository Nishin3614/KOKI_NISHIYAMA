// ------------------------------------------------------------------------------------------
//
// バッファロー処理 [buffalo.h]
// Author : 
//
// ------------------------------------------------------------------------------------------
#ifndef _BUFFALO_H_
#define _BUFFALO_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"
#include "wall.h"
#include "floor.h"
#include "materials.h"
#include "model.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define BUFFALOTYPE_MAX (13)	// モデルタイプ
#define	MAX_BUFFALO (128)		// バッファローの最大個数

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------モーションセット---------- //
typedef enum
{
	BUFFALOMOSIONSET_NEUTRAL = 0,
	BUFFALOMOSIONSET_MOVE,
	BUFFALOMOSIONSET_ASSAULT_WAIT,
	BUFFALOMOSIONSET_ASSAULT,
	BUFFALOMOSIONSET_MAX
} BUFFALOMOSIONSET;

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------状態---------- //
typedef enum
{
	BUFFALOSTATUS_NEUTRAL = 0,
	BUFFALOSTATUS_DAMAGE,
	BUFFALOSTATUS_DIE,
	BUFFALOSTATUS_MAX
} BUFFALOSTATUS;

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
// ---------- キリンのステータス情報 ---------- //
typedef struct
{
	int		nStatus;							// ステータス状態
	int		nCntStatus;							// カウントステート
	int		nLife;								// HP
	int		nAttack;							// 攻撃力
	bool	bColition;							// 当たった判定
	bool	bAttack;							// 攻撃状態
	bool	bMove;								// 移動状態
	bool	bRot;								// 回転状態
} BUFFALO_STATUS;

// ---------- バッファロー ---------- //
typedef struct
{
	MODEL			model[BUFFALOTYPE_MAX];		// バッファロー情報
	D3DXVECTOR3		pos;						// 位置
	D3DXVECTOR3		posold;						// 前の位置
	D3DXVECTOR3		posLast;					// 行きたい位置
	D3DXVECTOR3		posBetween;					// 位置の差分
	D3DXVECTOR3		posKeyBetween;				// キー間の距離
	D3DXVECTOR3		posOrigin;					// 初期位置
	D3DXVECTOR3		move;						// 移動量
	D3DXVECTOR3		rot;						// 現在回転量
	D3DXVECTOR3		rotLast;					// 向きたい方向
	D3DXVECTOR3		rotbetween;					// 回転の差分
	D3DXVECTOR3		rotKeyBetween;				// キー間の回転の差分
	D3DXVECTOR3		vtxMinMaterials;			// バッファローの位置の最小値
	D3DXVECTOR3		vtxMaxMaterials;			// バッファローの位置の最大値
	D3DXVECTOR3		size;						// バッファローのサイズ
	D3DXMATRIX		mtxWorldBuffalo;			// ワールドマトリックス
	WALL			*pWall;						// 対象の壁のポイント
	FLOOR			*pMeshField;				// 対象の床のポイント
	MATERIALS		*pMaterials;				// 対象の素材のポイント
	int				nMotionType;				// モーションタイプ
	int				nMotionTypeOld;				// 前回のモーションタイプ
	int				nCntMotion;					// モーションの切り替えカウント
	int				nParent;					// 親
	int				nIndex;						// パーツ
	int				nFram;						// フレーム
	int				nCntKeySet;					// キーセットカウント
	int				nIdxShadow;					// 影の番号
	int				nAttack;					// アタックカウント
	float			fLength;					// 長さ
	bool			bUse;						// 使用状況
	bool			bDisp;						// 表示状態

	// カラー情報
	int			nCntCor;						// カラーの状態カウント
	float		diffuse;						// α情報

	// ステータス //
	BUFFALO_STATUS	status;						// ステータス

	// β盤用
	int nCntTotal;								// 合計
} BUFFALO;

// ---------- バッファローキー情報 ---------- //
typedef struct
{
	int nLoop;									// ループ
	int nNumKey;								// キー情報の数

	// 当たり判定用 //
	int			Collision_nParts;				// パーツ番号
	D3DXVECTOR3	Collision_Ofset;				// オフセット
	float		Collision_Radius;				// 円の半径
	int			Collision_StartFram;			// 開始フレーム数
	int			Collision_EndFram;				// 終了フレーム数
	int			Collision_Damage;				// ダメージ数

	// 軌跡用 //
	int			Orbit_nParts;					// パーツ番号
	D3DXVECTOR3	Orbit_Ofset;					// オフセット
	int			Orbit_Mesh;						// 軌跡の線
	int			Orbit_StartFram;				// 開始フレーム数
	int			Orbit_EndFram;					// 終了フレーム数
	int			Orbit_nType;					// タイプ
	int			Orbit_nIdTrajectory;			// 軌跡の番号
	bool		Orbit_bUse;						// 軌跡の使用状態

	KEY_INFO KeyInfo[10];						// キー情報(モーション数)
} MOSION_BUFFALO;


// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitBuffalo(void);
void UninitBuffalo(void);
void UpdateBuffalo(void);
void DrawBuffalo(void);

void HitBuffalo(int nCntBuffalo, int nDamage);		// 攻撃が当たった処理

void ColisionBuffalo(
	D3DXVECTOR3 * pos,
	D3DXVECTOR3 * posOld,
	D3DXVECTOR3 * move,
	D3DXVECTOR3 * size);

void SetBuffalo(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot);		// 攻撃が当たった処理

BUFFALO *GetBuffalo(void);

// バッファローの撃破数
int GetBuffaloTotal(void);
#endif

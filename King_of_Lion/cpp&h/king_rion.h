// ------------------------------------------------------------------------------------------
//
// キリン処理 [king_rion.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _KING_RION_H_
#define _KING_RION_H_

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
#define KING_RIONTYPE_MAX (13)
#define	MAX_KING_RION (10)
#define KING_RION_KEYSET_MAX (10)

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------

// ---------モーションセット---------- //
typedef enum
{
	KING_RIONMOSIONSET_NEUTRAL = 0,
	KING_RIONMOSIONSET_MOVE,
	KING_RIONMOSIONSET_RUNMOVE,
	KING_RIONMOSIONSET_NORMALATACK,
	KING_RIONMOSIONSET_SHOT,
	KING_RIONMOSIONSET_MAX
} KING_RIONMOSIONSET;

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------状態---------- //
typedef enum
{
	KING_RIONSTATUS_NEUTRAL = 0,
	KING_RIONSTATUS_DAMAGE,
	KING_RIONSTATUS_DIE,
	KING_RIONSTATUS_MAX
} KING_RIONSTATUS;

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
// ---------- 百獣の王のステータス情報 ---------- //
typedef struct
{
	int		nStatus;							// ステータス状態
	int		nCntStatus;							// カウントステート
	int		nCntAttack;							// 攻撃するカウント
	int		nLife;								// HP
	int		nAttack;							// 攻撃力
	bool	bColition;							// 当たった判定
	bool	bAttack;							// 攻撃状態
	bool	bShot;								// 発射状態
	bool	bMove;								// 移動状態
	bool	bRot;								// 回転状態
} KING_RION_STATUS;

// ---------- キリン ---------- //
typedef struct
{
	MODEL			model[KING_RIONTYPE_MAX];	// キリン情報
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
	D3DXVECTOR3		vtxMinMaterials;			// キリンの位置の最小値
	D3DXVECTOR3		vtxMaxMaterials;			// キリンの位置の最大値
	D3DXVECTOR3		size;						// キリンのサイズ
	D3DXMATRIX		mtxWorldKing_Rion;			// ワールドマトリックス
	WALL			*pWall;						// 対象の壁のポイント
	FLOOR			*pMeshField;				// 対象の床のポイント
	MATERIALS		*pMaterials;				// 対象の素材のポイント
	int				nMotionType;				// モーションタイプ
	int				nMotionTypeOld;				// 前回のモーションタイプ
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
	KING_RION_STATUS	status;					// ステータス

	// β盤用
	int nCntTotal;								// 合計
} KING_RION;

// ---------- キリンキー情報 ---------- //
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

	KEY_INFO KeyInfo[KING_RION_KEYSET_MAX];		// キーセット情報
} MOSION_KING_RION;


// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitKing_Rion(void);
void UninitKing_Rion(void);
void UpdateKing_Rion(void);
void DrawKing_Rion(void);

void HitKing_Rion(int nCntKing_Rion,int nDamage);		// 攻撃が当たった処理

void ColisionKing_Rion(
	D3DXVECTOR3 * pos,		// 位置
	D3DXVECTOR3 * posOld, 	// 前回の位置
	D3DXVECTOR3 * move,		// 移動量
	D3DXVECTOR3 * size		// サイズ
);

// 百獣の王の撃破数
int GetKing_RionTotal(void);

KING_RION *GetKing_Rion(void);

MODEL *GetKing_Rion_Model(int nCntKing_Rion);
#endif

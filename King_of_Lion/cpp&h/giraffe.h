// ------------------------------------------------------------------------------------------
//
// キリン処理 [giraffe.h]
// Author : 
//
// ------------------------------------------------------------------------------------------
#ifndef _GIRAFFE_H_
#define _GIRAFFE_H_

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
#define GIRAFFETYPE_MAX (11)
#define	MAX_GIRAFFE (128)

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------

// ---------モーションセット---------- //
typedef enum
{
	GIRAFFEMOSIONSET_NEUTRAL = 0,
	GIRAFFEMOSIONSET_MOVE,
	GIRAFFEMOSIONSET_NORMALATACK,
	GIRAFFEMOSIONSET_SHOT,
	GIRAFFEMOSIONSET_MAX
} GIRAFFEMOSIONSET;

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------状態---------- //
typedef enum
{
	GIRAFFESTATUS_NEUTRAL = 0,
	GIRAFFESTATUS_DAMAGE,
	GIRAFFESTATUS_DIE,
	GIRAFFESTATUS_MAX
} GIRAFFESTATUS;

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
	bool	bShot;								// 発射状態
	bool	bMove;								// 移動状態
	bool	bRot;								// 回転状態
} GIRAFFE_STATUS;

// ---------- キリン ---------- //
typedef struct
{
	MODEL			model[GIRAFFETYPE_MAX];		// キリン情報
	D3DXVECTOR3		pos;						// 位置
	D3DXVECTOR3		posold;						// 前の位置
	D3DXVECTOR3		posLast;					// 行きたい位置
	D3DXVECTOR3		posBetween;					// 位置の差分
	D3DXVECTOR3		posKeyBetween;				// キー間の距離
	D3DXVECTOR3		posOrigin;					// 初期位置
	D3DXVECTOR3		posAttack;					// 標的
	D3DXVECTOR3		move;						// 移動量
	D3DXVECTOR3		rot;						// 現在回転量
	D3DXVECTOR3		rotLast;					// 向きたい方向
	D3DXVECTOR3		rotbetween;					// 回転の差分
	D3DXVECTOR3		rotKeyBetween;				// キー間の回転の差分
	D3DXVECTOR3		vtxMinMaterials;			// キリンの位置の最小値
	D3DXVECTOR3		vtxMaxMaterials;			// キリンの位置の最大値
	D3DXVECTOR3		size;						// キリンのサイズ
	D3DXMATRIX		mtxWorldGiraffe;			// ワールドマトリックス
	D3DXCOLOR		EffectCol;					// エフェクト用の色
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
	int				nIdxPrediction;				// 予測線の番号
	int				nAttack;					// アタックカウント
	int				nCntDamage;					// ダメージ
	int				nCntAnim;					// アニメ
	float			fLength;					// 長さ
	bool			bUse;						// 使用状況
	bool			bDisp;						// 表示状態

	// カラー情報
	int			nCntCor;						// カラーの状態カウント
	float		diffuse;						// α情報

	// ステータス //
	GIRAFFE_STATUS	status;						// ステータス

	// β盤用
	int nCntTotal;								// 合計
} GIRAFFE;

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

	KEY_INFO KeyInfo[5];						// キー情報(モーション数)
} MOSION_GIRAFFE;


// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitGiraffe(void);
void UninitGiraffe(void);
void UpdateGiraffe(void);
void DrawGiraffe(void);

void HitGiraffe(int nCntGiraffe,int nDamage);		// 攻撃が当たった処理

void ColisionGiraffe(
	D3DXVECTOR3 * pos, 
	D3DXVECTOR3 * posOld, 
	D3DXVECTOR3 * move,
	D3DXVECTOR3 * size);

// キリンの撃破数
int GetGiraffeTotal(void);

void SetGiraffe(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot);		// 攻撃が当たった処理

GIRAFFE *GetGiraffe(void);
#endif

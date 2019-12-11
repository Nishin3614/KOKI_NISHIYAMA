// ------------------------------------------------------------------------------------------
//
// モデル処理 [player.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"
#include "wall.h"
#include "floor.h"
#include "materials.h"
#include "model.h"
#include "water.h"

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
typedef enum
{
	PLAYER_STATE_NORMAL = 0,
	PLAYER_STATE_DAMAGE,
	PLAYER_STATE_DIE
} PLAYER_STATE;

// ---------モデル情報---------- //
typedef enum
{
	PLAYERTYPE_00_RION_BODY = 0,
	PLAYERTYPE_01_RION_HEAD,
	PLAYERTYPE_02_RION_RING,
	PLAYERTYPE_03_RION_RIGHT,
	PLAYERTYPE_04_RION_LEFT,
	PLAYERTYPE_05_RION_BACK_RIGHT2,
	PLAYERTYPE_06_RION_BACK_LEFT2,
	PLAYERTYPE_07_RION_TAIL_TAIL_JOINT,
	PLAYERTYPE_08_RION_TAIL_TAIL,
	PLAYERTYPE_09_RION_TAIL_TAIL_EDGE,
	PLAYERTYPE_MAX
} PLAYERTYPE;

// ---------モーションセット---------- //
typedef enum
{
	PLAYERMOSIONSET_NEUTRAL = 0,
	PLAYERMOSIONSET_MOVE,
	PLAYERMOSIONSET_JUMP,
	PLAYERMOSIONSET_LANDING,
	PLAYERMOSIONSET_NORMALATACK,
	PLAYERMOSIONSET_SHOT,
	PLAYERMOSIONSET_MAX
} PLAYERMOSIONSET;

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
// ---------- 主人公のステータス情報 ---------- //
typedef struct
{
	int		nState;			// 現状のステータス
	int		nCntState;		// カウントステータス
	int		nMaxLife;		// 最大HP
	int		nLife;			// HP
	int		nLastLife;		// 目的のHP
	int		nMaxMP;			// 最大MP
	int		nMP;			// MP
	int		nLastMP;		// 目的のMP
	int		nAttack;		// 攻撃力
	float	fLength;	// 攻撃の当たり範囲
	bool	bColition;	// 攻撃があたっているかどうか
} PLAYER_STATUS;

// ---------- モデル ---------- //
typedef struct
{
	// プレイヤーの基本情報 //
	MODEL		model[PLAYERTYPE_MAX];	// モデル情報
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 posold;					// 前の位置
	D3DXVECTOR3 posLast;				// 行きたい位置
	D3DXVECTOR3 posKeyBetween;			// キー間の距離
	D3DXVECTOR3	posOrigin;				// 初期位置
	D3DXVECTOR3 move;					// 移動量
	D3DXVECTOR3 rot;					// 現在回転量
	D3DXVECTOR3 rotLast;				// 向きたい方向
	D3DXVECTOR3 rotbetween;				// 回転の差分
	D3DXVECTOR3 rotKeyBetween;			// キー間の回転の差分
	D3DXVECTOR3 vtxMinMaterials;		// モデルの位置の最小値
	D3DXVECTOR3 vtxMaxMaterials;		// モデルの位置の最大値
	D3DXVECTOR3 size;					// モデルのサイズ
	D3DXMATRIX  mtxWorldPlayer;			// ワールドマトリックス
	WALL		*pWall;					// 対象の壁のポイント
	FLOOR		*pMeshField;			// 対象の床のポイント
	MATERIALS	*pMaterials;			// 対象の素材のポイント
	WATER		*pWater;				// 対象の水のポイント
	int			nMotionType;			// モーションタイプ
	int			nMotionTypeOld;			// 前回のモーションタイプ
	int			nParent;				// 親
	int			nIndex;					// パーツ
	int			nFram;					// フレーム
	int			nCntKeySet;				// キーセットカウント
	int			nIdxShadow;				// 影の番号
	
	// カラー情報
	int			nCntCor;				// カラーの状態カウント
	float		diffuse;			// α情報

	// 主人公のステータス情報 //
	PLAYER_STATUS status;

	float		fLength;				// 長さ
	bool		bUse;					// 使用状況
	bool		bjump;					// ジャンプ状態
} PLAYER;

// ---------- モージョン情報 ---------- //
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
	KEY_INFO	KeyInfo[5];				// キー情報(モーション数)
} MOSION_PLAYER;


// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

// ダメージを受けたとき
void HitPlayer(int fDamage);

// プレイヤーステータス更新
void StatusUpPlayer(
	int nUpLife,
	int nUpMp,
	int nUpAttack);

// プレイヤー回復
void RecoveryPlayer(
	int nRecoveryLife,
	int nRecoveryMP);

void PlayerPosition(void);

// プレイヤー情報
PLAYER *GetPlayer(void);

// プレイヤーのモデル情報
MODEL *GetPlayerModel(void);

// プレイヤーの位置情報
D3DXVECTOR3 GetPlayer_Pos(void);

// プレイヤーの前回の位置情報
D3DXVECTOR3 GetPlayer_PosOld(void);

// プレイヤーの回転情報
D3DXVECTOR3 GetPlayer_Rot(void);

// プレイヤーの位置情報
float GetPlayer_Radius(void);

// プレイヤーの状態情報
PLAYER_STATUS * GetPlayerStatus(void);
#endif

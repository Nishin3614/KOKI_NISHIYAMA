// ------------------------------------------------------------------------------------------
//
// 敵処理 [enemy.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

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
#define ENEMY_TYPE (10)		// タイプ数
#define ENEMY_PARTS (20)	// パーツ数
#define ENEMY_KEY_SET (10)	// キーセット数
#define ENEMY_MOTION (10)	// モーション数
#define ENEMY_MAT (10)		// マテリアル数

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------

// ---------モーションセット---------- //
typedef enum
{
	ENEMYMOSIONSET_NEUTRAL = 0,
	ENEMYMOSIONSET_MOVE,
	ENEMYMOSIONSET_NORMALATACK,
	ENEMYMOSIONSET_SHOT,
	ENEMYMOSIONSET_MAX
} ENEMYMOSIONSET;

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------状態---------- //
typedef enum
{
	ENEMYSTATUS_NEUTRAL = 0,
	ENEMYSTATUS_DAMAGE,
	ENEMYSTATUS_DIE,
	ENEMYSTATUS_MAX
} ENEMYSTATUS;

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
// ---------- トライ敵 ---------- //
typedef struct
{
	// 敵専用変数
	int				Num_Model;								// 敵数
	char			aFileModel[ENEMY_PARTS][128];

	// キャラクターの個人ステータス
	float			move;				// 移動量
	float			fJump;				// ジャンプ量
	float			fRadius;			// 半径

} ENEMYMODELINFO;

// ---------- トライ敵 ---------- //
typedef struct
{
	LPD3DXMESH			paMesh;							// メッシュ情報へのポインタ
	LPD3DXBUFFER		paBuffMat;						// マテリアル情報へのポインタ
	DWORD				nNumMat;						// マテリアルの数
	LPDIRECT3DTEXTURE9	paTextureEnemy[ENEMY_MAT];		// テクスチャへのポインタ
	D3DXMATRIX			mtxWorldModel;					// ワールドマトリックス
	D3DXVECTOR3			pos;							// 位置
	D3DXVECTOR3			posLast;						// 行きたい位置
	D3DXVECTOR3			posKeyBetween;					// キー間の距離
	D3DXVECTOR3			posOrigin;						// 初期位置
	D3DXVECTOR3			rot;							// 現在回転量
	D3DXVECTOR3			rotLast;						// 行きたい方向
	D3DXVECTOR3			rotKeyBetween;					// キー間の回転量
	D3DXVECTOR3			size;							// サイズ
	D3DXVECTOR3			vtxMinMaterials;				// 敵の位置の最小値
	D3DXVECTOR3			vtxMaxMaterials;				// 敵の位置の最大値
	int					nIdxModelParent;				// 親敵のインデックス
	int					nFram;							// フレーム
	int					nTexType;						// テクスチャータイプ
	int					nCntKeySet;						// キーセットカウント

	char				aFileTex[ENEMY_MAT][128];		// テクスチャーデータ
} ENEMYMODEL;

// ---------- キー要素 ---------- //
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
} ENEMYKEY;

// ---------- キー情報 ---------- //
typedef struct
{
	int nFram;
	ENEMYKEY key[ENEMY_PARTS];	// パーツ数
} ENEMYKEY_INFO;

// ---------- 敵キー情報 ---------- //
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

	ENEMYKEY_INFO KeyInfo[ENEMY_KEY_SET];						// キー情報(モーション数)
} MOSION_ENEMY;

// ---------- 敵のステータス情報 ---------- //
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
} ENEMY_STATUS;

// ---------- 敵 ---------- //
typedef struct
{
	ENEMYMODEL		model[ENEMY_PARTS];		// 敵情報
	D3DXVECTOR3		pos;					// 位置
	D3DXVECTOR3		posold;					// 前の位置
	D3DXVECTOR3		posLast;				// 行きたい位置
	D3DXVECTOR3		posBetween;				// 位置の差分
	D3DXVECTOR3		move;					// 移動量
	D3DXVECTOR3		rot;					// 現在回転量
	D3DXVECTOR3		rotLast;				// 向きたい方向
	D3DXVECTOR3		rotbetween;				// 回転の差分
	D3DXVECTOR3		vtxMinMaterials;		// 敵の位置の最小値
	D3DXVECTOR3		vtxMaxMaterials;		// 敵の位置の最大値
	D3DXVECTOR3		size;					// 敵のサイズ
	D3DXMATRIX		mtxWorldEnemy;			// ワールドマトリックス
	WALL			*pWall;					// 対象の壁のポイント
	FLOOR			*pMeshField;			// 対象の床のポイント
	MATERIALS		*pMaterials;			// 対象の素材のポイント
	int				nMotionType;			// モーションタイプ
	int				nMotionTypeOld;			// 前回のモーションタイプ
	int				nParent;				// 親
	int				nIndex;					// パーツ
	int				nFram;					// フレーム
	int				nCntKeySet;				// キーセットカウント
	int				nIdxShadow;				// 影の番号
	int				nType;					// キャラクターの種類
	int				nTexType;				// テクスチャー情報
	float			fLength;				// 長さ
	bool			bUse;					// 使用状況
	bool			bDisp;					// 表示状態

	// カラー情報
	int			nCntCor;					// カラーの状態カウント
	float		diffuse;					// α情報

	// ステータス //
	ENEMY_STATUS	status;					// ステータス

	// β盤用
	int nCntTotal;							// 合計
} ENEMY;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void HitEnemy(int nCntEnemy,int nDamage);		// 攻撃が当たった処理

void ColisionEnemy(
	D3DXVECTOR3 * pos, 
	D3DXVECTOR3 * posOld, 
	D3DXVECTOR3 * move,
	D3DXVECTOR3 * size);

// 敵の撃破数
int GetEnemyTotal(void);

ENEMY *GetEnemy(void);
#endif

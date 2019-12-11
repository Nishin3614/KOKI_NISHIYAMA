//=============================================================================
//
// モデル処理 [player.h]
// Author : koki_nishiyama
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "scene.h"
#include "model.h"
#include "motion.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYERCOLLISION_SIZE (30.0f)

//=============================================================================
// 前方宣言
//=============================================================================

//=============================================================================
// クラス
//=============================================================================
class CPlayer : public CScene
{
public:
	// ---------モーションタイプ---------- //
	typedef enum
	{
		MOTIONTYPE_WAIT = 0,
		MOTIONTYPE_JUMP,
		MOTIONTYPE_LANDING,
		MOTIONTYPE_LANDING_FAIL1,
		MOTIONTYPE_LANDING_FAIL2,
		MOTIONTYPE_MAX
	} MOTIONTYPE;

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
		PLAYERTYPE_MAX
	} PLAYERTYPE;

	/* 関数 */
	CPlayer();
	~CPlayer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPos(void);		// 位置取得
	D3DXVECTOR3 GetRot(void);		// 回転取得
	static CPlayer * Create();		// 作成
	static void Load(void);			// 読み込み
	static void UnLoad(void);		// UnLoadする
	// ステータス読み込み
	void LoadStatus(void);
	// ステータス保存
	void SaveStatus(void);
	// ステータスの設定
	void SetStatus(STATUS_INFO Status);
	// ステータス上昇
	void AddJump(void);
	void AddRot(void);
	// 宣言区域
	void Restriction(void);
	bool GetbJump(void);

protected:
private:
	/* 関数 */
	void Move(void);				// 移動
	float GetResultJumpforce(void);
	float GetResultRotforce(void);

	// 伸縮処理
	void ExpanAndContraProc(void);
	// 伸縮倍率の取得
	float GetExpanAndContraMagni(void);
	// 伸る時間の取得
	int GetExtendTime(void);
	// 縮む時間の取得
	int GetShrinkTime(void);
	// ゲージ量の倍率の取得
	float GetJumpMagni(void);
	// ジャンプ力の倍率
	float GetJumpforce(void);
	/* 変数 */
	static CModel_info			*m_pModel_info;						// モデル情報の保存
	static MODEL_OFFSET			*m_pModel_offset;					// モデルの初期配置
	static MOTION				*m_pMotion;							// モーション情報の保存
	CModel 						*m_pModel[PLAYERTYPE_MAX] = {};		// モデル
	D3DXMATRIX					m_mtxWorld;							// 行列
	D3DXVECTOR3					m_pos;								// 位置
	D3DXVECTOR3					m_posold;							// 前の位置
	D3DXVECTOR3					m_move;								// 移動量
	D3DXVECTOR3					m_rot;								// 現在回転量
	D3DXVECTOR3					m_rotLast;							// 向きたい方向
	D3DXVECTOR3					m_rotbetween;						// 回転の差分
	D3DXVECTOR3					m_size;								// モデルのサイズ
	int							m_nMaxModel;						// 最大モデル数
	int							m_nMotiontype;						// モーションタイプ
	int							m_nMotiontypeOld;					// 前回のモーションタイプ
	int							m_nMaxMotion;						// 最大モーション数
	int							m_keyinfoCnt;						// キー情報のカウント
	int							m_nFrame;							// フレームカウント
	bool						m_bJump;							// ジャンプ
	float						m_fCntRot;							// 回転カウント
	// 当たり判定
	CCollision					*m_pCollision;						// 当たり判定情報

	STATUS_INFO					m_Status;							// ステータス情報
	int							m_nNumJumpItem;						// 獲得したジャンプアイテム数
	int							m_nNumRotItem;						// 獲得した回転アイテム数
	D3DXVECTOR3					m_scal;								// スケール

	int							m_JumpTime;							// ジャンプしている時の時間
	int							m_nJumpCharge;						// ジャンプチャージ
	D3DXVECTOR3					m_JumpRot;
};

//=============================================================================
// 構造体
//=============================================================================
// ---------- 主人公のステータス情報 ---------- //
typedef struct
{
	int		nState;		// 現状のステータス
	int		nCntState;	// カウントステータス
	int		nMaxLife;	// 最大HP
	int		nLife;		// HP
	int		nLastLife;	// 目的のHP
	int		nMaxMP;		// 最大MP
	int		nMP;		// MP
	int		nLastMP;	// 目的のMP
	int		nAttack;	// 攻撃力
	float	fLength;	// 攻撃の当たり範囲
	bool	bColition;	// 攻撃があたっているかどうか
} PLAYER_STATUS;
#endif

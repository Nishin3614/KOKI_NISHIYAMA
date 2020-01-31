// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// モデル処理 [player.h]
// Author : koki_nishiyama
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "character.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 前方宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CStateGauge;
class C2DGauge;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// クラス
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CPlayer : public CCharacter
{
public:
	// ---------モーションタイプ---------- //
	typedef enum
	{
		/* 通常モーション */
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_MOVE,
		/* コンボ1 */
		MOTIONTYPE_DIAGONALCUT,
		MOTIONTYPE_THRUST,
		/* コンボ2 */
		MOTIONTYPE_REVERSESIDE,
		/* コンボ3 */
		MOTIONTYPE_SLASHER,
		MOTIONTYPE_WHIRLWIND,
		/* 必奥義 */
		MOTIONTYPE_TRICK1_FIREUPSIDE,
		MOTIONTYPE_TRICK2_FLASHSTRIKE,
		MOTIONTYPE_TRICK3_PAINLIGHTNING,
		MOTIONTYPE_MAX
	} MOTIONTYPE;
	/* 関数 */
	CPlayer();
	~CPlayer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	D3DXVECTOR3 GetPos(void);		// 位置取得
	D3DXVECTOR3 GetRot(void);		// 回転取得
	static CPlayer * Create();		// 作成
	static HRESULT Load(void);		// 読み込み
	static void UnLoad(void);		// UnLoadする
protected:
private:
	/* 構造体 */
	/* 関数 */
	void Move(void);				// 移動
	void Stop(void);				// 停止
	void State(void);				// 状態
	void MotionState(void);			// モーション時の状態
	void Motion_Slasher(void);		// スラッシャー
	void Motion_Attack(void);		// 攻撃

	void Attack(void);				// 攻撃
	void At_Normal(void);			// ノーマル攻撃
	void At_DiagonalCut(void);		// コンボ１_追加入力なし(斜め切り)
	void At_Thrust(void);			// コンボ１_追加入力↑(突き)
	void At_ReverseSide(void);		// コンボ2_追加入力(逆切り)
	void ConboAttack(void);			// コンボ攻撃
	void Trick(void);				// 秘奥義
	void Trick1_FireUpside(void);	// 秘奥義1
	void Trick2_FlashStrike(void);	// 秘奥義2
	void Trick3_PainLightning(void);// 秘奥義3
	/* 変数 */
	D3DXVECTOR3	m_posold;			// 前の位置
	int			m_nConbo;			// コンボ
	int			m_nInvalConbo;		// コンボインターバル
	bool		m_bTrick;			// 秘奥義中かどうか

	C2DGauge	*p2DHPGauge;		// HPゲージ
	C2DGauge	*p2DTrickGauge;		// 秘奥義ゲージ	
};
#endif

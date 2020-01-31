//=============================================================================
//
// 棒人間処理 [rothuman.h]
// Author : koki_nishiyama
//
//=============================================================================
#ifndef _ROTHUMAN_H_
#define _ROTHUMAN_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "enemy.h"

//=============================================================================
// 前方宣言
//=============================================================================

//=============================================================================
// クラス
//=============================================================================
class CRothuman : public CEnemy
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_DAMAGE,
		STATE_DIE
	} STATE;

	// ---------モーションタイプ---------- //
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_MOVE,
		MOTIONTYPE_ATTACK,
		MOTIONTYPE_JUMP,
		MOTIONTYPE_LANDING,
		MOTIONTYPE_MAX
	} MOTIONTYPE;

	/* 関数 */
	CRothuman();
	~CRothuman();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	// 作成
	static CRothuman * Create(		
		D3DXVECTOR3 pos			// 位置
	);
	static HRESULT LoadCreate(void);
	static HRESULT Load(void);		// 読み込み
	static void UnLoad(void);		// UnLoadする
protected:
private:
	/* 関数 */
	void move(void);	// 移動
	void rot(void);		// 回転

	// AI用
	void AiAction(void);
	void AttackAction(void);
	void NormalAction(void);
	/* 変数 */
};
#endif

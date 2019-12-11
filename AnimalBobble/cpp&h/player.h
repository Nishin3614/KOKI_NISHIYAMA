// ----------------------------------------
//
// プレイヤー処理の説明[player.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "main.h"
#include "scene_two.h"
#include "prediction.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

// ------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------
class CPlayer : public CScene_TWO
{
public:
	/* 関数 */
	CPlayer();
	~CPlayer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ShotState(bool bShot);
	CManager::PLAYER GetPlayerId(void);
	D3DXVECTOR3 GetPos(void);
	int GetShotCnt(void);				// 弾の発射カウント取得
	static HRESULT Load(void);
	static void UnLoad(void);
	static CPlayer * Create(CManager::PLAYER playerId,D3DXVECTOR3 pos);		// 作成
protected:

private:
	/* 関数 */
	void Animation(void);				// アニメーション管理
	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex;
	D3DXVECTOR3 m_pos;					// 位置
	float m_rot;						// 回転量
	int m_nHorizonAnim;					// 水平のアニメーションパターン
	int m_nVirticalAnim;				// 垂直のアニメーションパターン
	int m_nCntAnim;						// アニメーションカウント
	int m_nShotCnt;						// 強制発射カウント
	int m_nShotTime;					// 強制発射タイム
	bool m_bShot;						// 発射状態
	CManager::PLAYER m_PlayerId;		// プレイヤー番号
	CPrediction * m_pprediction;		// 予測線
};

#endif
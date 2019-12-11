// ----------------------------------------
//
// 箱処理の説明[box.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _BOX_H_
#define _BOX_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "main.h"
#include "scene_two.h"

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
class CBox : public CScene_TWO
{
public:
	/* 関数 */
	CBox();
	~CBox();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVirticalAnim(int nVirtical);	// 垂直のアニメーションパターンの設定
	void SetHorizonAnim(int nHorizon);		// 水平のアニメーションパターンの設定
	CManager::PLAYER GetPlayerId(void);
	D3DXVECTOR3 GetPos(void);				// 位置情報取得
	static HRESULT Load(void);
	static void UnLoad(void);
	static CBox * Create(CManager::PLAYER playerId,D3DXVECTOR3 pos);		// 作成
protected:

private:
	/* 関数 */
	void Animation(void);				// アニメーション管理
	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex;
	int m_nHorizonAnim;					// 水平のアニメーションパターン
	int m_nVirticalAnim;				// 垂直のアニメーションパターン
	int m_nCntAnim;						// アニメーションカウント
	CManager::PLAYER m_PlayerId;		// プレイヤー番号
};

#endif
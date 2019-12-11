// ----------------------------------------
//
// 予測線処理の説明[prediction.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _PREDICTION_H_
#define _PREDICTION_H_	 // ファイル名を基準を決める

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
class CPrediction : public CScene_TWO
{
public:
	/* 列挙型 */
	// タイプ
	typedef enum
	{
		TYPE_BEE = 0,	// ハチ
		TYPE_CAT,		// 猫
		TYPE_FISH,		// 魚
		TYPE_HAWK,		// 鷹
		TYPE_SNAKE,		// 蛇
		TYPE_MAX
	} TYPE;

	/* 関数 */
	CPrediction();
	~CPrediction();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos);
	void SetRot(float fRot);
	static CPrediction * Create(CManager::PLAYER playerId,D3DXVECTOR3 pos, TYPE nType);	// 作成
	static HRESULT Load(void);									// 読み込み
	static void UnLoad(void);									// 読み込んだ情報を破棄
protected:

private:
	/* 関数 */
	void Limit(float fRot);				// 限界点
	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex;	// テクスチャー情報
	D3DXVECTOR3 m_pos;					// 位置
	D3DXCOLOR m_col;					// カラー
	TYPE m_type;						// タイプ
	CManager::PLAYER m_PlayerId;		// プレイヤー番号
};
#endif
// ------------------------------------------------------------------------------------------
//
// 天井処理の説明[ceiling.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _CEILING_H_
#define _CEILING_H_	 // ファイル名を基準を決める

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------
#include "main.h"
#include "scene_two.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------
class CCeiling : public CScene_TWO
{
public:
	/* 関数 */
	CCeiling();
	~CCeiling();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CManager::PLAYER GetPlayerId(void);
	D3DXVECTOR3 GetPos(void);		// 位置情報取得
	D3DXVECTOR2 GetSize(void);		// サイズ情報取得
	// 位置とサイズの設定
	void SetPosSize(
		D3DXVECTOR3 pos,	// 位置
		D3DXVECTOR2 size	// サイズ
	);
	static HRESULT Load(void);		// 読み込み
	static void UnLoad(void);		// 読み込んだ情報を破棄
	static CCeiling * Create(		// 作成
		CManager::PLAYER playerId,	// プレイヤー番号
		D3DXVECTOR3 pos,			// 位置	
		D3DXVECTOR2 size			// サイズ
	);
protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	CManager::PLAYER m_PlayerId;	// プレイヤー番号
};

#endif
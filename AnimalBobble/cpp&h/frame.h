// ----------------------------------------
//
// 枠処理の説明[frame.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _FRAME_H_
#define _FRAME_H_	 // ファイル名を基準を決める

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
class CFrame : public CScene_TWO
{
public:
	/* 関数 */
	CFrame();
	~CFrame();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CManager::PLAYER GetPlayerId(void);
	D3DXVECTOR3 GetPos(void);	// 位置情報取得
	D3DXVECTOR2 GetSize(void);	// サイズ情報取得
	static HRESULT Load(void);	// 読み込み
	static void UnLoad(void);	// 読み込んだ情報を破棄
	static CFrame * Create(CManager::PLAYER playerId,D3DXVECTOR3 pos);	// 作成
protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	CManager::PLAYER m_PlayerId;	// プレイヤー番号
};

#endif
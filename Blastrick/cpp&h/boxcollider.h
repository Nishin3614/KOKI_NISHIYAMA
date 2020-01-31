// ----------------------------------------
//
// 箱の当たり判定処理の説明[boxcollider.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _BOXCOLLIDER_H_
#define _BOXCOLLIDER_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CBoxcollider : public CScene
{
public:
	/* 関数 */
	CBoxcollider();
	CBoxcollider(ACTOR actor, LAYER layer);
	~CBoxcollider();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

	void SetSize(D3DXVECTOR2 size);						// サイズ設定
	void SetPosition(D3DXVECTOR3 pos);					// 位置設定
	void SetRot(float frot);							// 回転設定
	D3DXVECTOR2 GetSize(void);							// サイズ情報取得
	D3DXVECTOR3 GetPosition(void);						// 位置情報取得

	static CBoxcollider * Create();						// 作成

protected:
private:
	D3DXVECTOR3 m_pos;					// 中心座標
	float m_rot;						// 角度
	D3DXVECTOR2 m_size;					// サイズ
};

#endif
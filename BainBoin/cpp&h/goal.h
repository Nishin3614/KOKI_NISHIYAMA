// ----------------------------------------
//
// ゴール処理の説明[goal.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _GOAL_H_
#define _GOAL_H_	 // ファイル名を基準を決める

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


// ----------------------------------------
//
// 前方宣言
//
// ----------------------------------------

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CGoal : public CScene
{
public:
	/* 関数 */
	CGoal();
	~CGoal();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void UnLoad(void);
	static CGoal * Create(		// 作成
		D3DXVECTOR3 pos
	);
protected:

private:
	/* 関数 */
	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex;
	D3DXVECTOR3 m_pos;					// 位置
};

#endif
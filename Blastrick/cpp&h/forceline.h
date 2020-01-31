// ----------------------------------------
//
// 集中線処理の説明[forceline.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _FORCELINE_H_
#define _FORCELINE_H_	 // ファイル名を基準を決める

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
class CForceline : public CScene_TWO
{
public:
	/* 関数 */
	CForceline();
	~CForceline();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// 使用状態
	static void Use(bool bUse);
	static HRESULT Load(void);
	static void UnLoad(void);
	static CForceline * Create(void);		// 作成
protected:

private:
	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex;
	static bool	m_bUse;			
};

#endif
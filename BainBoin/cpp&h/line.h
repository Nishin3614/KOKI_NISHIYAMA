// ----------------------------------------
//
// 線処理の説明[line.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _LINE_H_
#define _LINE_H_	 // ファイル名を基準を決める

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
#define MAX_LINE (10)

// ------------------------------------------
//
// 前方宣言
//
// ------------------------------------------

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CLine : public CScene
{
public:
	/* 関数 */
	CLine();
	~CLine();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);			// 位置設定
	static CLine * Create(D3DXVECTOR3 pos1,D3DXVECTOR3 pos2);	// 作成
	static HRESULT Load(void);									// 読み込み
	static void UnLoad(void);									// 読み込んだ情報を破棄
protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
};

#endif
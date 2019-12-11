// ----------------------------------------
//
// 3D線処理の説明[3Dline.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _3DLINE_H_
#define _3DLINE_H_	 // ファイル名を基準を決める

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
#define MAX_3DLINE (10)

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
class C3DLine : public CScene
{
public:
	/* 関数 */
	C3DLine();
	~C3DLine();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(			// 位置設定
		D3DXVECTOR3 &pos1,
		D3DXVECTOR3 &pos2
	);			
	static C3DLine * Create(	// 作成
		D3DXVECTOR3 &pos,
		D3DXVECTOR3 &rot, 
		D3DXVECTOR3 &pos1,
		D3DXVECTOR3 &pos2
	);
	static HRESULT Load(void);									// 読み込み
	static void UnLoad(void);									// 読み込んだ情報を破棄
protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX  m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 回転
};

#endif
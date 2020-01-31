// ----------------------------------------
//
// メッシュマネージャー処理の説明[meshmanager.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _MESHMANAGER_H_
#define _MESHMANAGER_H_	 // ファイル名を基準を決める

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
class CMeshmanager : public CScene
{
public:
	/* 関数 */
	CMeshmanager();
	CMeshmanager(ACTOR actor);
	~CMeshmanager();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	void MakeVtx(void);
	static HRESULT Load(void);
	static void UnLoad(void);
	static CMeshmanager * Create(		// 作成
		D3DXVECTOR3 pos,
		D3DXVECTOR3 size,
		D3DXVECTOR3 rot,
		int nWidth,
		int nDepth
	);
	float GetHeight(D3DXVECTOR3 pos);								// 高さ取得

protected:

private:
	/* 関数 */
	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIndex;	// インデックスバッファのポインタ
	D3DXVECTOR3 m_OriginBlock;			// 初期配置
	D3DXMATRIX  m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_size;					// サイズ
	D3DXVECTOR3 m_rot;					// 角度
	int	m_nNumberVertex;				// 総頂点数
	int m_nNumIndex;					// 総インデックス
	int m_nNumPolygon;					// 総ポリゴン
	int m_nBlock_Depth;					// 縦ブロック数
	int m_nBlock_Width;					// 横ブロック数
};

#endif
// ----------------------------------------
//
// メッシュドーム処理の説明[meshdome.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _MESHDOME_H_
#define _MESHDOME_H_	 // ファイル名を基準を決める

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
class CMeshdome : public CScene
{
public:
	/* 列挙型 */
	typedef enum
	{
		TYPE_NORMAL,
		TYPE_WAVE,
		TYPE_MAX
	} TYPE;
	/* 関数 */
	CMeshdome();
	CMeshdome(ACTOR actor);
	~CMeshdome();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static HRESULT Load(void);
	static void UnLoad(void);
	static CMeshdome * Create(		// 作成
		D3DXVECTOR3 pos,			// 位置
		D3DXVECTOR3 size,			// サイズ
		D3DXVECTOR3 rot,			// 角度
		D3DXCOLOR	col,			// 色
		int nWidth,					// 横ブロック
		int nDepth,					// 縦ブロック
		TYPE type					// タイプ
	);
	static CMeshdome * Create(		// 作成
		D3DXVECTOR3 size,			// サイズ
		int nWidth,					// 横ブロック
		int nDepth,					// 縦ブロック
		TYPE type					// タイプ
	);
	// 取得
	bool GetUse(void);						// 使用状態
	// 設定
	void SetPos(D3DXVECTOR3 const &pos);	// 位置
	void SetRot(D3DXVECTOR3 const &rot);	// 回転
	void SetCol(D3DXCOLOR const &col);		// 色
	void SetUse(bool const bUse);					// 使用状態
protected:

private:
	/* 関数 */
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);	// 頂点座標の生成
	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex[TYPE_MAX];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIndex;	// インデックスバッファのポインタ
	D3DXVECTOR3 m_OriginBlock;			// 初期配置
	D3DXMATRIX  m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_size;					// サイズ
	D3DXVECTOR3 m_rot;					// 角度
	D3DXCOLOR	m_col;					// 色
	int		m_nNumberVertex;			// 総頂点数
	int		m_nNumIndex;				// 総インデックス
	int		m_nNumPolygon;				// 総ポリゴン
	int		m_nBlock_Depth;				// 縦ブロック数
	int		m_nBlock_Width;				// 横ブロック数
	bool	m_bUse;						// 使用
	TYPE	m_type;						// タイプ
};

#endif
// ----------------------------------------
//
// 軌跡処理の説明[meshobit.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _MESHOBIT_H_
#define _MESHOBIT_H_	 // ファイル名を基準を決める

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
#define MESHOBIT_MAX (32)
#define OBITLINE_MAX (128)

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
class CMeshobit : public CScene
{
public:
	/* 列挙型 */
	// テクスチャータイプ
	typedef enum
	{
		TEX_0 = 0,
		TEX_1,
		TEX_MAX
	} TEX;
	// 軌跡のタイプ
	typedef enum
	{
		TYPE_0 = 0,
		TYPE_1,
		TYPE_MAX
	} TYPE;
	/* 構造体 */
	typedef struct
	{
		D3DXVECTOR3 pos[2] = {};	// 位置
	} OBIT;

	typedef struct
	{
		D3DXVECTOR3				m_aPosVerTex[2] = {};		// 算出した頂点座標
		D3DXVECTOR3				m_aOffset[2] = {};			// オフセット座標
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;			// 頂点バッファへのポインタ
		OBIT					obit[OBITLINE_MAX] = {};	// 軌跡の線個数
		D3DXMATRIX				*othermtx;					// 他の行列
		D3DXMATRIX				matrix;						// 行列
		D3DXVECTOR3				pos;						// 位置
		D3DXVECTOR3				*rot;						// 回転量
		D3DXVECTOR3				move;						// 移動量
		D3DXCOLOR				col;						// 色
		TEX						tex;						// テクスチャー
		TYPE					type;						// タイプ
		float					fSize;						// サイズ
		int						nLine;						// 線の数
		int						nCntFrame;					// フレーム数
		int						nMaxFrame;					// 最大フレーム数
		int						nMaxMeshObit;				// 最大の軌跡数
		int						nNumberVertex;				// 総頂点数
		int						nNumPolygon;				// 総ポリゴン
		bool					bUse;						// 使用状態
	} MESHOBIT;

	/* 関数 */
	CMeshobit();
	~CMeshobit();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//-------------------------------------------------------------------------------------------------------------
	// 設定
	// 1行列、2オフセット始点、3オフセット終点、4色、5タイプ、6サイズ、7線の個数、8フレーム数
	//-------------------------------------------------------------------------------------------------------------
	static void Set(
		D3DXMATRIX  *pMtx,		// マトリックス情報
		D3DXVECTOR3	&offset1,	// オフセット1
		D3DXVECTOR3 &offset2,	// オフセット2
		D3DXCOLOR	&col,		// 色
		TYPE		type,		// タイプ
		TEX			tex,		// テクスチャー
		float		fSize,		// サイズ情報
		int			nLine,		// 線
		int			&nFrame		// フレーム数
	);

	// 読み込み
	static HRESULT LoadMesh(void);
	static HRESULT Load(void);
	// 読み込んだものを破棄
	static void UnLoad(void);
	// 生成
	static CMeshobit * Create();
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
protected:

private:
	/* 関数 */
	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex[TEX_MAX];	// テクスチャー
	static MESHOBIT m_meshobit[MESHOBIT_MAX];	// 軌跡
	TEX	m_tex;									// テクスチャータイプ
};

#endif
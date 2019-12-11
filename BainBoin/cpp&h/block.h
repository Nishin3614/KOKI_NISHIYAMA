// ----------------------------------------
//
// ブロック処理の説明[block.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _BLOCK_H_
#define _BLOCK_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene_X.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

//=============================================================================
// 前方宣言
//=============================================================================

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CBlock : public CScene_X
{
public:
	/* 列挙型 */
	// タイプ
	typedef enum
	{
		TYPE_1 = 0,	// 回転率アップ
		TYPE_MAX
	} TYPE;

	/* 関数 */
	CBlock();
	~CBlock();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos);			// 位置設定
	void SetRot(D3DXVECTOR3 rot);			// 回転設定
	D3DXVECTOR3 GetPos(void);				// 位置取得
	static CBlock * Create(void);			// 作成
	static void CBlock::LoadCreate(void);	// 読み込んだものを生成
	static HRESULT Load(void);				// 読み込み
	static void UnLoad(void);				// 破棄
protected:
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];		// テクスチャへのポインタ
	static CLoad	*m_pload;							// ロード
	static CModel_info	*m_pmodel_info;					// モデル情報
	static int		m_nAll;								// 総数
	TYPE			m_Type;								// タイプ
};

#endif
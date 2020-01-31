// ----------------------------------------
//
// アイテム処理の説明[item.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _ITEM_H_
#define _ITEM_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene_three.h"

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
class CItem : public CScene_THREE
{
public:
	/* 列挙型 */
	// タイプ
	typedef enum
	{
		TYPE_ROT = 0,	// 回転率アップ
		TYPE_JUMP,		// ジャンプ力アップ
		TYPE_COIN,		// コイン
		TYPE_MAX
	} TYPE;

	/* 関数 */
	CItem();
	CItem(ACTOR actor, LAYER layer);
	~CItem();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTexture(TYPE type);							// テクスチャー設定
	void SetSize(D3DXVECTOR3 size);						// サイズ設定
	void SetPos(D3DXVECTOR3 pos);						// 位置設定
	void SetRot(D3DXVECTOR3 rot);						// 回転設定
	void SetCol(D3DXCOLOR col);							// 色設定
	void SetTex(D3DXVECTOR2 first,D3DXVECTOR2 last);	// 表示テクスチャー設定
	D3DXVECTOR3 GetSize(void);							// サイズ取得
	D3DXVECTOR3 GetRot(void);							// 回転取得
	D3DXVECTOR3 GetPos(void);							// 位置取得
	static CItem * Create(bool bBillboard);				// 作成
	static void LoadCreate(void);						// アイテム生成
	static HRESULT Load(void);							// 読み込み
	static void UnLoad(void);							// 破棄
	static CLoad *GetLoad(void);						// ロード取得
protected:
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];		// テクスチャへのポインタ
	static CLoad	*m_pload;		// ロード
	static int		m_nAll;			// 番号
	static int		m_nRotCntAll;		// 回転個数
	static int		m_nJumpCntAll;		// ジャンプ個数
	D3DXVECTOR3		m_OriginBlock;	// 初期配置
	D3DXMATRIX		m_mtxWorld;		// ワールドマトリックス
	TYPE			m_Type;		// タイプ
	float			m_fLengh;		// 長さ
	float			m_fAngle;		// 角度
	bool			m_bBillboard;	// ビルボード状態
};
#endif
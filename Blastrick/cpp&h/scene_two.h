// ----------------------------------------
//
// 2Dシーン処理の説明[scene_two.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _SCENE_TWO_H_
#define _SCENE_TWO_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "main.h"
#include "scene.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_TWO : public CScene
{
public:
	/* 列挙型 */
	typedef enum
	{
		OFFSET_TYPE_CENTER = 0,
		OFFSET_TYPE_LEFT,
		OFFSET_TYPE_MAX
	} OFFSET_TYPE;
	/* 関数 */
	CScene_TWO();
	CScene_TWO(ACTOR actor, LAYER layer);
	~CScene_TWO();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

	// 設定 //
	// 頂点座標
	void Set_Vtx_Pos(
		OFFSET_TYPE const &offsettype = OFFSET_TYPE_CENTER,			// オフセットタイプ
		VERTEX_2D * pVtx = NULL										// 頂点情報
	);
	// 頂点カラー
	void Set_Vtx_Col(VERTEX_2D * pVtx = NULL);
	// サイズ
	void SetSize(
		D3DXVECTOR2 const &size										// サイズ
	);		
	// 位置
	void SetPosition(
		D3DXVECTOR3 const &pos										// 位置
	);
	// 角度
	void SetRot(
		float const &frot											// 角度
	);
	// 色
	void SetCol(
		D3DXCOLOR const &col										// 色
	);	
	// テクスチャー
	void SetTex(
		D3DXVECTOR2 const &first = D3DXVECTOR2(0.0f,0.0f),			// 初期の配置
		D3DXVECTOR2 const &last = D3DXVECTOR2(1.0f, 1.0f)			// 最後の配置
	);
	// 取得 //
	D3DXVECTOR2 GetSize(void) const;								// サイズ情報
	D3DXVECTOR3 GetPosition(void) const;							// 位置情報
	float GetLengh(void) const;										// 長さ情報

	void BindTexture(LPDIRECT3DTEXTURE9 p_Tex);	// テクスチャー
	// 作成
	static CScene_TWO * Create(
		OFFSET_TYPE	const & type,									// タイプ
		D3DXVECTOR3 const & pos,									// 位置
		D3DXVECTOR2 const & size,									// サイズ
		ACTOR		const & actor = ACTOR_SCENE_TWO,				// アクター情報
		float		const & frot = 0.0f,							// 角度
		D3DXCOLOR	const & col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)	// 色
	);						

protected:
private:
	/* 関数 */
	void Offset_Center(VERTEX_2D *pVtx);	// オフセットがセンター
	void Offset_Left(VERTEX_2D *pVtx);		// オフセットが左
	
	/* 変数 */
	LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;						// 中心座標
	D3DXVECTOR2 m_size;						// サイズ
	D3DXCOLOR	m_col;						// カラー
	OFFSET_TYPE	m_offsetType;				// オフセットタイプ
	float m_rot;							// 角度
	float m_fLengh;							// 長さ
	float m_fAngle;							// 角度
};

#endif
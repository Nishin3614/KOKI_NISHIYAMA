// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 2Dゲージ処理の説明[2Dgauge.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _2DGAUGE_H_
#define _2DGAUGE_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 前方宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_TWO;

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class C2DGauge : public CScene
{
public:
	/* 関数 */
	C2DGauge();
	~C2DGauge();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

	static HRESULT Load(void);		// 読み込み
	static void UnLoad(void);		// UnLoadする
	// 作成
	static C2DGauge * Create(
		D3DXVECTOR3 const &pos,											// 位置
		D3DXVECTOR2 const &size,										// サイズ
		D3DXCOLOR	const &Maincol,										// メインの色
		D3DXCOLOR	const &Undercol = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),	// 下の色
		D3DXCOLOR	const &Blockcol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)	// ブロックの色
		);
	// 設定 //
	// サイズが変わる定数
	void SetConstance(
		float const &fMaxValue	// 最大値
	);
	// 色
	void SetCol(
		D3DXCOLOR	const &Maincol,											// メインの色
		D3DXCOLOR	const &Undercol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),	// 下の色
		D3DXCOLOR	const &Blockcol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)		// ブロックの色
	);
	// 位置
	void SetPos(
		D3DXVECTOR3 const &pos												// 位置
	);
	// 取得 //
	// 位置
	D3DXVECTOR3 GetPos(void) const;

	// ゲージ変化
	void ChangeGauge(
		float const &fValue		// 現在の値
	);

protected:

private:
	/* 列挙型 */
	typedef enum
	{
		/* ゲーム内のUI */
		GAUGE_BLOCK = 0,
		GAUGE_UNDERLINE,
		GAUGE_MAINLINE,
		GAUGE_MAX
	} GAUGE;
	/* 関数 */

	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex;	// テクスチャー設定
	D3DXVECTOR2	m_size[GAUGE_MAX];					// サイズ情報
	float		m_fSizeLast;						// サイズの目標値
	float		m_fSizeDiff;						// サイズの差分
	float		m_fConstance;						// サイズが変わる定数
	D3DXCOLOR	m_col[GAUGE_MAX];					// カラー情報
	D3DXVECTOR3	m_pos;								// 位置情報
	CScene_TWO *m_aScene_Two[GAUGE_MAX];			// 2Dゲージ2D
};

#endif
// ----------------------------------------
//
// チュートリアルUI処理の説明[tutorial_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _GAUGE_UI_H_
#define _GAUGE_UI_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene_two.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define GAGE_VALUE_MAX		(120)			// ゲージの値の最大

#define GAGE_VALUE_FAST		(GAGE_VALUE_MAX * 0.3333f)	// ゲージの値1
#define GAGE_VALUE_SECOND	(GAGE_VALUE_MAX * 0.6666f)	// ゲージの値2
#define GAGE_VALUE_THIRD	(GAGE_VALUE_MAX * 1.0f)		// ゲージの値3

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
class CGauge : public CScene
{
public:
	/* 列挙型 */
	typedef enum
	{
		GAUGE_GAUGE = 0,//ゲージの外枠
		GAUGE_FRAME,//ゲージ本体
		GAUGE_MAX
	} GAUGE;

	/* 関数 */
	CGauge();
	~CGauge();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);		// 読み込み
	static void UnLoad(void);		// UnLoadする
	static CGauge * Create(void);	// 作成
	int GetGaugeValue(void);		//
	void ResetJump(void);
protected:

private:
	/* 関数 */
	void Various(int nCnt);											// 種類別処理

	/* 変数 */
	static LPDIRECT3DTEXTURE9	m_pTex[GAUGE_MAX];			// テクスチャー設定
	static D3DXVECTOR3			m_pos[GAUGE_MAX];			// 位置情報
	static D3DXVECTOR2			m_size[GAUGE_MAX];			// サイズ情報
	CScene_TWO					*m_aScene_Two[GAUGE_MAX];	// チュートリアル用ui2D
	int							m_nCharge;					// ジャンプゲージの量
	int							m_nMaxGauge;				// ゲージの最大値
	float						m_fTex;						//テクスチャの座標
};

#endif
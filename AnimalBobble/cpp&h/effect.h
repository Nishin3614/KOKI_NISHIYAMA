// ----------------------------------------
//
// エフェクト処理の説明[effect.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_	 // ファイル名を基準を決める

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
#define MAX_EFFECT (1028)

// ------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------
class CEffect : public CScene_TWO
{
public:
	/* 列挙型 */
	// タイプ
	typedef enum
	{
		TYPE_CERCLE,
		TYPE_FOUNTAIN,
	} TYPE;
	/* 関数 */
	CEffect();
	~CEffect();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Fountain(					/* 噴水移動 */
		float fGoalrot,				// 目的角度
		int nSplit					// 分割数
		);		
	void Circle(					/* 円移動 */
		int nSplit					// 分割数
		);
	static HRESULT Load(void);
	static void UnLoad(void);
	static CEffect * Create(		/* 作成 */
		D3DXVECTOR3 pos,			// 位置
		D3DXCOLOR col,				// カラー
		D3DXCOLOR goalcol,			// 目標のカラー
		float fRot,					// 初期角度
		float fSpeed,				// 速さ
		float fSize,				// サイズ
		float fGoalsize,			// 目標サイズ
		int nLife					// ライフ(分割数)
		);		
protected:

private:
	/* 変数宣言 */
	static LPDIRECT3DTEXTURE9 m_pTex;					// テクスチャー
	D3DXVECTOR3 m_pos = {};								// 中心座標
	D3DXCOLOR m_col = {};								// カラー
	D3DXCOLOR m_Goalcol = {};							// 目標カラー
	D3DXCOLOR m_Diffcol = {};							// カラー差分
	float m_fRot;										// 角度
	float m_fGoalrot;									// 目標角度
	float m_fDiffrot;									// 角度差分
	float m_Speed;										// 速さ
	float m_fSize;										// サイズ
	float m_fGoalsize;									// 目標サイズ
	float m_fDiffsize;									// サイズ差分
	int m_nLife;										// ライフ
	int m_nSplit;										// 分割数
	int m_nCntSplit;									// 分割カウント
};

#endif
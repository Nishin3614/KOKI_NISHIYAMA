// ----------------------------------------
//
// 番号処理の説明[number.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _NUMBER_H_
#define _NUMBER_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "manager.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define MAX_NUMBER (10)

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CNumber
{
public:
	/* 列挙型 */
	// テクスチャータイプ
	typedef enum
	{
		TEX_SCORE,
		TEX_STAGE,
		TEX_MAX
	} TEX;

	/* 関数 */
	CNumber();
	~CNumber();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNum(int nNum);		// 番号設定
	void SetPos(D3DXVECTOR3 pos, float fsize);
	void SetTex(TEX tex);		// テクスチャー
	void SetCol(D3DXCOLOR col);	// カラー
	static HRESULT Load(void);
	static void UnLoad(void);
	static CNumber * Create();	// 作成
protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex[TEX_MAX];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	TEX m_texID;						// テクスチャー番号
	float m_rot;						// 角度
	float m_fLengh;						// 長さ
	float m_fAngle;						// 角度
};

#endif
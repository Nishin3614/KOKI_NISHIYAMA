// ----------------------------------------
//
// バッググラウンドドーム処理の説明[bgdome.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _BGDOME_H_
#define _BGDOME_H_	 // ファイル名を基準を決める

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
class CMeshdome;

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CBgdome : public CScene
{
public:
	/* 列挙型 */
	typedef enum
	{
		TYPE_BG = 0,
		TYPE_STAR,
		TYPE_MAX
	} TYPE;
	/* 関数 */
	CBgdome();
	~CBgdome();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static HRESULT Load(void);
	static void UnLoad(void);
	static CBgdome * Create(void);		// 作成
	static void SetUse(bool bUse);			// 使用状態
	static void SetPos(D3DXVECTOR3 const &pos);	// 位置
	static void SetRot(D3DXVECTOR3 const &rot);	// 回転
	static void Set(
		D3DXVECTOR3 const &pos,
		D3DXVECTOR3 const &rot,
		int const &nFrame
	);
protected:

private:
	/* 変数 */
	static CMeshdome	*m_aMeshdome[TYPE_MAX];	// メッシュドーム
	static bool			m_bUse;					// 使用状態
	static int			m_CntFrame;				// カウントフレーム
	static int			m_nMaxFrame;			// 最大フレーム
};

#endif
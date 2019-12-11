// ----------------------------------------
//
// 中間地点処理の説明[waypoint.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _WAYPOINT_H_
#define _WAYPOINT_H_	 // ファイル名を基準を決める

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
class CWaypoint : public CScene
{
public:
	/* 関数 */
	CWaypoint();
	~CWaypoint();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void UnLoad(void);
	static CWaypoint * Create(		// 作成
		D3DXVECTOR3 pos
	);
	bool GetWaypoint(void);	// 中間地点
	D3DXVECTOR3 GetPos(void);	// 中間地点の位置
protected:

private:
	/* 関数 */
	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex;
	CMeshdome * m_dome;					// ドーム情報
	D3DXVECTOR3 m_pos;					// 位置
	bool		m_bWaypoint;			// 中間地点
};

#endif
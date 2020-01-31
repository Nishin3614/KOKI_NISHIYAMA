// ------------------------------------------
//
// 軌跡の処理[trajectory.h]
// Author : Nishiyama koki
//
// ------------------------------------------
#ifndef _TRAJECTORY_H_
#define _TRAJECTORY_H_

// ------------------------------------------
// インクルードファイル
// ------------------------------------------
#include "manager.h"

// ------------------------------------------
// マクロ定義
// ------------------------------------------

// ------------------------------------------
// クラス定義
// ------------------------------------------
class CTrajectory
{
public:
	/* 列挙型 */
	typedef enum
	{
		TYPE_NORMAL = 0,	// 通常軌跡
		TYPE_TRICK1,		// 秘奥義1の軌跡
		TYPE_TRICK2,		// 秘奥義2の軌跡
		TYPE_MAX
	} TYPE;
	/* 関数 */
	CTrajectory();
	~CTrajectory();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// ------------------------------------------
	// 設定
	// 1行列、2タイプ
	// ------------------------------------------
	static void Set(
		D3DXMATRIX *pmtx,	// 行列
		TYPE type,			// タイプ
		int &nFrame			// フレーム数
	);

#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static CTrajectory * Create(void);
	static HRESULT LoadCreate(void);	// あらかじめ軌跡の生成をする
	static HRESULT Load(void);
	static void Unload(void);

private:
};

#endif

// ------------------------------------------
//
// 演出の処理[perfomance.h]
// Author : Nishiyama koki
//
// ------------------------------------------
#ifndef _PERFOMANCE_H_
#define _PERFOMANCE_H_

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
class CPerfomance
{
public:
	/* 列挙型 */
	typedef enum
	{
		TYPE_MESHOBIT1 = 0,
		TYPE_MESHOBIT2,
		TYPE_MESHOBIT3,
		TYPE_,
		TYPE_1,
		TYPE_MAX
	} TYPE;
	/* 関数 */
	CPerfomance();
	~CPerfomance();
	// ------------------------------------------
	// 設定
	// 1行列、2タイプ
	// ------------------------------------------
	static void Set(
		D3DXMATRIX *pmtx,	// 行列
		TYPE type,			// タイプ
		D3DXVECTOR3 &pos,	// 位置
		D3DXVECTOR3 &rot,	// 回転
		int Frame			// フレーム数
		);

#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

private:
};

#endif

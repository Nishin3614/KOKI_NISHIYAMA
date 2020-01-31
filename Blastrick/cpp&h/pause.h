// ------------------------------------------
//
// ポーズヘッダー処理 [pause.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#ifndef _PAUSE_H_
#define _PAUSE_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------
#include "main.h"

// ------------------------------------------
//
// マクロ定義
//
// ------------------------------------------

// ------------------------------------------
//
// 前方宣言
//
// ------------------------------------------

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CPause
{
public:
	/* 関数 */
	CPause();
	~CPause();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static void SetPause(bool bPause);	// ポーズの状態設定
	static bool GetPause(void);			// ポーズの状態取得
protected:
private:
	static bool m_bPause;
};
#endif

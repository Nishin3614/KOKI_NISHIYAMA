// ------------------------------------------
//
// レンダリング処理の説明[renderer.h]
// Author : Koki Nishiyama
//
// ------------------------------------------

#ifndef _RENDERER_H_
#define _RENDERER_H_	 // ファイル名を基準を決める

// ------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------
#include "main.h"

// ------------------------------------------
//
// マクロ関数
//
// ------------------------------------------

// ------------------------------------------
//
// 前方宣言
//
// ------------------------------------------
class CDebugproc;
class CCamera;
class CLight;	

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CRenderer
{
public:
	/* 関数 */
	CRenderer();
	~CRenderer();
	HRESULT  Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// 情報取得
	LPDIRECT3DDEVICE9 GetDevice(void);	// デバイス
	CCamera * GetCamera(void);			// カメラ
	CLight * GetLight(void);			// ライト
protected:

private:
	/* 変数 */
	LPDIRECT3D9 m_pD3D;				// Direct3Dオブジェクトのポインタ
	LPDIRECT3DDEVICE9 m_pDevice;	// Direct3Dデバイスのポインタ
	CCamera * m_pCamera;				// カメラ情報
	CLight * m_pLight;				// ライト情報
#ifdef _DEBUG
	static CDebugproc * m_debugproc;		// デバッグ表示
	void DrawFPS(void);				// FPS関数
#endif
};

#endif // !_RENDERER_H_
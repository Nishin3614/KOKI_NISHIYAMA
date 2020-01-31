//*************************************************************************************************************
//
// エディットウィンドウ処理[Editwindow.h]
// Author : Nishiyama koki
//
//*************************************************************************************************************
#ifndef _STATUS_H_
#define _STATUS_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "Calculation.h"

//-------------------------------------------------------------------------------------------------------------
// 構造体
//-------------------------------------------------------------------------------------------------------------
typedef struct
{
	char cName;
	int nID;
	int nType;
	float fHp;
	float fSpeed;
	float fAttack;
} STATUS_INFO;

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CStatus
{
public:
	CStatus();
	~CStatus();
	// 生成
	static CStatus *Create(void);								// ステータス
	void CreateEditWindow(HINSTANCE hInstance, int CmdShow);	// ウィンドウの生成
	void EditWindowRegister(HINSTANCE hInstance);				// ウィンドウクラスの登録
	void EditWindowUnregister(void);							// ウィンドウクラスの破棄
	// ロード・セーブ
	static void StatusLoad(STATUS_INFO *Status);				// ステータスの読み込み
	static void StatusSave(void);								// ステータスセーブ
	// 設定
	static HWND SetWindowEditDisabled(							// 名前の枠
		HWND *hWndthis,											// 特定の枠
		HWND hWnd, 												// すべての枠
		int nID, 												// 番号
		INTEGER2 pos, 											// 位置
		char* name);											// 名前
	static HWND SetWindowEdit(									// 数値入力用の枠
		HWND *hWndthis,											// 特定の枠
		HWND hWnd,												// すべての枠
		int nID,												// 番号
		INTEGER2 pos,											// 位置
		int *nValue = NULL,										// 整数型
		float *fValue = NULL,									// 浮動小数点型
		char *cValue = NULL										// 文字列型
	);

private:
	/* 変数 */
	static WNDCLASSEX	m_wcex;	// ウィンドウクラス
	static HWND			m_hWnd;	// ハンドルウィンドウ
	static RECT			m_rect;	// 結果

	STATUS_INFO			m_Status;	// ステータス情報
};















#endif
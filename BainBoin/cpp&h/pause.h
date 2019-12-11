//------------------------------------------------------
//
//	記号定数と疑似命令
//
//--------------------------------------------------------
#ifndef _PAUSE_H_
#define _PAUSE_H_
//------------------------------------------------------
//
//	インクルードファイル
//
//--------------------------------------------------------
#include "scene_two.h"

//------------------------------------------------------
//
//	マクロ定義
//
//--------------------------------------------------------
#define PAU_SCR_SIZE_X (500.0f)//ポーズの背景の横のサイズ
#define PAU_SCR_SIZE_Y (250.0f)//ポーズの背景の縦のサイズ

#define PAU_UI_SIZE_X (250.0f)//文字[PAUAE]の横のサイズ
#define PAU_UI_SIZE_Y (90.0f)//文字[PAUAE]の縦のサイズ

#define PAU_BUT_SIZE_X (120.0f)//一つ目のボタンの横のサイズ
#define PAU_BUT_SIZE_Y (50.0f)//一つ目のボタンの横のサイズ

#define PAUSE_TEXTURE (4)//使う画像の数
//------------------------------------------------------
//
//	前方宣言
//
//--------------------------------------------------------
class CScene_TWO;

//===========================================================
//
//	クラス型の定義
//
//===========================================================
class CPause :public CScene_TWO
{
public:
	enum PAUSETYPE
	{
		PAUSETYPE_NORMAL,//背景画像
		PAUSETYPE_UI,//PAUSE文字
		PAUSETYPE_BUTTAN1,//ポーズボタン1
		PAUSETYPE_BUTTAN2,//ポーズボタン2
		PAUSETYPE_MAX
	};

public:
	enum PAUSESTATE
	{
		PAUSESTATE_NORMAL,//未選択
		PAUSESTATE_1,//1つめの場所を選択中
		PAUSESTATE_2,//2つめの場所を選択中
	};

public:
	//関数宣言
	CPause();//コンストラクタ (インスタンスが生成されると自動的に呼び出される)
	~CPause();//デストラクタ (インスタンスが破棄されると自動的に呼び出される)
	static HRESULT Load(void);//画像の読み込み
	static void Unload(void);//画像の破棄
	void Init(void);//初期化
	void Uninit(void);//終了
	void Update(void);//更新
	void Draw(void);//描画

	static CPause * Create(PAUSETYPE type);//生成
	static void SetPause(bool bPause);	// ポーズの状態設定
	static bool GetPause(void);			// ポーズの状態取得
private:
	//静的変数宣言
	static LPDIRECT3DTEXTURE9 m_pTexture[PAUSE_TEXTURE];//画像を複数管理する変数
	static bool m_bPause;
	//変数宣言
	PAUSETYPE m_type;//UIの種類
	PAUSESTATE m_state;//選択できるボタンの状態
	D3DXCOLOR m_col;//色の変数
};
#endif


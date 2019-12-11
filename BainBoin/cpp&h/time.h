//------------------------------------------------------
//
//	記号定数と疑似命令
//
//--------------------------------------------------------
#ifndef _TIME_H_
#define _TIME_H_
//------------------------------------------------------
//
//	インクルードファイル
//
//--------------------------------------------------------
#include "scene.h"
#include "renderer.h"
#include "number.h"
//------------------------------------------------------
//
//	マクロ定義
//
//--------------------------------------------------------
#define TIME_DIGIT (3)//桁数
#define TIMEX	(20)//幅
#define TIMEY	(40)//高さ
#define TIME_POSX	(1025.0f)//X座標
#define TIME_POSY	(150.0f)//Y座標
#define TIME_SPACE	(40.0f)//間隔
//------------------------------------------------------
//
//	前方宣言
//
//--------------------------------------------------------
class CScene;
//===========================================================
//
//	クラス型の定義
//
//===========================================================
class CTime :public CScene
{
public:

	//関数宣言
	CTime();//コンストラクタ (インスタンスが生成されると自動的に呼び出される)
	~CTime();//デストラクタ (インスタンスが破棄されると自動的に呼び出される)

	void Init(void);//初期化
	void Uninit(void);//終了
	void Update(void);//更新
	void Draw(void);//描画

	static CTime * Create(void);//生成
	void SetPosition(D3DXVECTOR3 pos);//座標の設定
	void DecreasesTime(void);//時間の減少

	bool GetTimeOverFlag(void);

private:

	//静的変数宣言

	//変数宣言
	CNumber *m_apNumber[TIME_DIGIT];//桁数分の変数
	D3DXVECTOR3 m_pos;//座標
	int m_nTime;//時間
	bool m_bTimeOver;
};
#endif


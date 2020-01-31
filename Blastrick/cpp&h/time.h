// ------------------------------------------------------------------------------------------
//
// タイム処理 [time.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
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
#define TIME_DIGIT (2)//桁数

//------------------------------------------------------
//
//	前方宣言
//
//--------------------------------------------------------
class CScene;

//--------------------------------------------------------
//
//	クラス型の定義
//
//--------------------------------------------------------
class CTime :public CScene
{
public:
	/* 関数 */
	CTime();	//コンストラクタ (インスタンスが生成されると自動的に呼び出される)
	~CTime();	//デストラクタ (インスタンスが破棄されると自動的に呼び出される)

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

	static CTime * Create(D3DXVECTOR3 const &pos);	//生成
	void SetPos(D3DXVECTOR3 pos);					//座標の設定
	void DecreasesTime(void);						//時間の減少
	bool GetTimeOverFlag(void);						// タイムオーバーフラグ

private:

	//静的変数宣言

	//変数宣言
	CNumber *m_apNumber[TIME_DIGIT];				//桁数分の変数
	D3DXVECTOR3 m_pos;								//座標
	int m_nTime;									//時間
	bool m_bTimeOver;								
};
#endif
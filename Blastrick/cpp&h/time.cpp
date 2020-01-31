// ------------------------------------------------------------------------------------------
//
// タイム処理 [time.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

//--------------------------------------------------------
//
// インクルードファイル
//
//--------------------------------------------------------
#include "main.h"
#include "time.h"
#include "manager.h"
#include "scene.h"
#include "number.h"

//--------------------------------------------------------
//
// マクロ定義
//
//--------------------------------------------------------
#define TIME_SPACE	(40.0f)//間隔

//--------------------------------------------------------
//
// 静的変数初期化
//
//--------------------------------------------------------

//------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------
CTime::CTime(void) :CScene(ACTOR_TIME,LAYER_UI)
{
	m_nTime = DERAY_TIME(99);
	m_bTimeOver = false;
}
//------------------------------------------------------
// デストラクタ
//--------------------------------------------------------
CTime::~CTime()
{
}
//------------------------------------------------------
// 初期化処理
//------------------------------------------------------
void CTime::Init(void)
{
	// 変数宣言
	int nNumber;

	for (int nCount = 0; nCount < TIME_DIGIT; nCount++)
	{
		// 生成
		m_apNumber[nCount] = CNumber::Create();
		// 位置の設定
		m_apNumber[nCount]->SetPos(D3DXVECTOR3(m_pos.x + nCount * TIME_SPACE, m_pos.y, 0.0f), TIME_SPACE);
		// タイムの1桁を取得
		nNumber = (int)(m_nTime / 60) % (int)powf(10.0f, (float)(TIME_DIGIT - nCount)) / (int)powf(10.0f, TIME_DIGIT - 1.0f - nCount);
		// ナンバーの設定
		m_apNumber[nCount]->SetNum(nNumber);
	}
}
//------------------------------------------------------
// 終了処理
//------------------------------------------------------
void CTime::Uninit(void)
{
	for (int nCount = 0; nCount < TIME_DIGIT; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Uninit();
			delete m_apNumber[nCount];
			m_apNumber[nCount] = NULL;
		}
	}
}
//------------------------------------------------------
// 更新処理
//------------------------------------------------------
void CTime::Update(void)
{
	// 変数宣言
	int nNumber;
	// タイムダウン
	DecreasesTime();
	for (int nCount = 0; nCount < TIME_DIGIT; nCount++)
	{
		// ヌルチェック
		if (m_apNumber[nCount] != NULL)
		{
			// ナンバーの更新
			m_apNumber[nCount]->Update();
			// タイムの1桁を取得
			nNumber = int((m_nTime / 60) % (int)powf(10.0f, (float)(TIME_DIGIT - nCount)) / (int)powf(10.0f, TIME_DIGIT - 1.0f - nCount));
			// ナンバーの設定
			m_apNumber[nCount]->SetNum(nNumber);
		}
	}
}
//------------------------------------------------------
// 描画処理
//------------------------------------------------------
void CTime::Draw(void)
{
	for (int nCount = 0; nCount < TIME_DIGIT; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Draw();
		}
	}
}

#ifdef _DEBUG
// ------------------------------------------
// デバッグ表示
// ------------------------------------------
void CTime::Debug(void)
{
}
#endif // _DEBUG

//------------------------------------------------------
// ポジションの設定
//------------------------------------------------------
void CTime::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//------------------------------------------------------
// 時間減少処理
//------------------------------------------------------
void CTime::DecreasesTime()
{
	m_nTime--;

	if (m_nTime <= 0)
	{
		m_nTime = 0;
		m_bTimeOver = true;
	}
}
//------------------------------------------------------
// タイムオーバーフラグの取得
//------------------------------------------------------
bool CTime::GetTimeOverFlag(void)
{
	return m_bTimeOver;
}
//------------------------------------------------------
// 生成処理
//------------------------------------------------------
CTime *CTime::Create(D3DXVECTOR3 const &pos)
{
	// 変数宣言
	CTime *pTime;
	pTime = new CTime;
	pTime->m_pos = pos;
	pTime->Init();
	return pTime;
}

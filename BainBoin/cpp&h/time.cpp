//------------------------------------------------------
//クラス型処理[main.cpp]
//Author:Masato Fujiwara
//
//--------------------------------------------------------

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
#define TIME_UP_MAX (60 * 60 * 2)

//--------------------------------------------------------
//
// staticの初期化
//
//--------------------------------------------------------
//------------------------------------------------------
//
//	コンストラクタ
//
//--------------------------------------------------------
CTime::CTime(void) :CScene(ACTOR_TIME,LAYER_UI)
{
	m_nTime = TIME_UP_MAX;
	m_bTimeOver = false;
	m_nTime = TIME_UP_MAX;
}
//------------------------------------------------------
//
//	デストラクタ
//
//--------------------------------------------------------
CTime::~CTime()
{
}
//------------------------------------------------------
//
//	初期化処理
//
//------------------------------------------------------
void CTime::Init(void)
{
	int nNumber;

	for (int nCount = 0; nCount < TIME_DIGIT; nCount++)
	{
		m_apNumber[nCount] = CNumber::Create();
		m_apNumber[nCount]->SetPos(D3DXVECTOR3(TIME_POSX + nCount * TIME_SPACE, TIME_POSY, 0.0f), TIME_SPACE);
		nNumber = (int)(m_nTime / 60) % (int)powf(10.0f, (float)(TIME_DIGIT - nCount)) / (int)powf(10.0f, TIME_DIGIT - 1.0f - nCount);

		m_apNumber[nCount]->SetNum(nNumber);
	}
}
//------------------------------------------------------
//
//	終了処理
//
//------------------------------------------------------
void CTime::Uninit(void)
{
	for (int nCount = 0; nCount < TIME_DIGIT; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Uninit();
		}
	}
}
//------------------------------------------------------
//
//	更新処理
//
//------------------------------------------------------
void CTime::Update(void)
{
	DecreasesTime();

	int nNumber;

	for (int nCount = 0; nCount < TIME_DIGIT; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Update();

			nNumber = int((m_nTime / 60) % (int)powf(10.0f, (float)(TIME_DIGIT - nCount)) / (int)powf(10.0f, TIME_DIGIT - 1.0f - nCount));

			m_apNumber[nCount]->SetNum(nNumber);
		}
	}
}
//------------------------------------------------------
//
//	描画処理
//
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
//------------------------------------------------------
//
//	ポジションの設定
//
//------------------------------------------------------
void CTime::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//------------------------------------------------------
//
//	時間減少処理
//
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
//
//	タイムオーバーフラグの取得
//
//------------------------------------------------------
bool CTime::GetTimeOverFlag(void)
{
	return m_bTimeOver;
}
//------------------------------------------------------
//
//	生成処理
//
//------------------------------------------------------
CTime *CTime::Create(void)
{
	CTime *pTime;

	pTime = new CTime;

	pTime->Init();

	return pTime;
}

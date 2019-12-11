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
#include "score.h"
#include "manager.h"
#include "scene.h"
#include "number.h"
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
CScore::CScore(void) :CScene(ACTOR_SCORE,LAYER_UI)
{
}
//------------------------------------------------------
//
//	デストラクタ
//
//--------------------------------------------------------
CScore::~CScore()
{
}
//------------------------------------------------------
//
//	初期化処理
//
//------------------------------------------------------
void CScore::Init(void)
{
	for (int nCount = 0; nCount < SCORE_DIGIT; nCount++)
	{
		m_apNumber[nCount] = CNumber::Create();
		m_apNumber[nCount]->SetNum(0);
		m_apNumber[nCount]->SetPos(D3DXVECTOR3(SCORE_POSX + nCount * SCORE_SPACE, SCORE_POSY, 0.0f), SCORE_SPACE);
	}
}
//------------------------------------------------------
//
//	終了処理
//
//------------------------------------------------------
void CScore::Uninit(void)
{
	for (int nCount = 0; nCount < SCORE_DIGIT; nCount++)
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
void CScore::Update(void)
{
	for (int nCount = 0; nCount < SCORE_DIGIT; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Update();
		}
	}
}
//------------------------------------------------------
//
//	描画処理
//
//------------------------------------------------------
void CScore::Draw(void)
{
	for (int nCount = 0; nCount < SCORE_DIGIT; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Draw();
		}
	}
}
//------------------------------------------------------
//
//	スコアの計算処理
//
//------------------------------------------------------
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;

	for (int nCount = 0; nCount < SCORE_DIGIT; nCount++)
	{
		int nNumber = m_nScore % (int)powf(10.0f, 8.0f - nCount) / (int)powf(10.0f, 8.0f - 1.0f - nCount);

		m_apNumber[nCount]->SetNum(nNumber);
	}
}
//------------------------------------------------------
//
//	ポジションの設定
//
//------------------------------------------------------
void CScore::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//------------------------------------------------------
//
//	スコア取得の設定
//
//------------------------------------------------------
int CScore::GetScore(void)
{
	return m_nScore;
}
//------------------------------------------------------
//
//	生成処理
//
//------------------------------------------------------
CScore *CScore::Create(void)
{
	CScore *pScore;

	pScore = new CScore;

	pScore->Init();

	return pScore;
}

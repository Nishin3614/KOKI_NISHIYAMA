// ----------------------------------------
//
// スコア処理の説明[score.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "score.h"
#include "number.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define SCORE_SIZE (50.0f)

// ----------------------------------------
//
// グローバル変数
//
// ----------------------------------------

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CScore::CScore() : CScene(ACTOR_SCORE, LAYER_UI)
{
	// スコア
	m_nScore = 0;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CScore::~CScore()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CScore::Init(void)
{	
	// 変数宣言
	int nScore;

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		// 桁の数字
		nScore = m_nScore % (int)powf(10.0f, (float)nCnt + 1.0f) / (int)powf(10.0f, (float)nCnt);
		m_pNumber[nCnt] = CNumber::Create();
		m_pNumber[nCnt]->SetNum(nScore);
		m_pNumber[nCnt]->SetPos(
			D3DXVECTOR3(m_pos.x + (MAX_SCORE * SCORE_SIZE) / 2 - nCnt * SCORE_SIZE, m_pos.y, 0.0f),
			SCORE_SIZE);
		m_pNumber[nCnt]->SetTex(CNumber::TEX_SCORE);
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CScore::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		if (m_pNumber[nCnt] != NULL)
		{
			m_pNumber[nCnt]->Uninit();
			delete m_pNumber[nCnt];
			m_pNumber[nCnt] = NULL;
		}
	}
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CScore::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_pNumber[nCnt]->Update();
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CScore::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_pNumber[nCnt]->Draw();
	}
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// デバッグ表示
//-------------------------------------------------------------------------------------------------------------
void CScore::Debug(void)
{
}
#endif // _DEBUG


// ----------------------------------------
// スコア設定処理
// ----------------------------------------
void CScore::SetScore(int nPoint)
{
	// 変数宣言
	int nScore;

	// 現在のポイントを加算
	m_nScore += nPoint;
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		// 桁の数字
		nScore = m_nScore % (int)powf(10.0f, (float)nCnt + 1.0f) / (int)powf(10.0f, (float)nCnt);
		m_pNumber[nCnt]->SetNum(nScore);
		m_pNumber[nCnt]->SetPos(
			D3DXVECTOR3(m_pos.x + (MAX_SCORE * SCORE_SIZE) / 2 - nCnt * SCORE_SIZE, m_pos.y, 0.0f),
			SCORE_SIZE);
	}
}

// ----------------------------------------
// スコア取得処理
// ----------------------------------------
int CScore::GetScore(void)
{
	return m_nScore;
}

// ----------------------------------------
// 位置情報取得処理
// ----------------------------------------
D3DXVECTOR3 CScore::GetPos(void)
{
	return m_pos;
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CScore * CScore::Create(D3DXVECTOR3 pos)
{
	// 変数宣言
	CScore * pScore;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pScore = new CScore();
	// 位置情報代入
	pScore->m_pos = pos;
	pScore->m_pos.y -= SCORE_SIZE / 2;
	// 初期化処理
	pScore->Init();
	// 生成したオブジェクトを返す
	return pScore;
}
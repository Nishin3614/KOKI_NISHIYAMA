// ----------------------------------------
//
// 上位スコア処理の説明[topscore.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "topscore.h"
#include "number.h"
#include "mark.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define TOPSCORE_SIZE (50.0f)

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
//CNumber * CTopscore::m_pNumber[MAX_NUMBERSCORE] = {};	// 上位スコア表示
int CTopscore::m_nTopScore[MAX_TOPSCORE] = {};			// 上位スコア
int CTopscore::m_nMyScore = 0;							// マイスコア
int CTopscore::m_nInRank = -1;							// インランク

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CTopscore::CTopscore() : CScene(ACTOR_TOPSCORE,LAYER_UI)
{
	// 上位スコア
	m_nScore = 0;
	m_pNumber[MAX_NUMBERSCORE] = {};
	m_pos = {};
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CTopscore::~CTopscore()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CTopscore::Init(void)
{	
	int nTopscore = 0;
	for (int nCnt = 0; nCnt < MAX_NUMBERSCORE; nCnt++)
	{
		// 桁の数字
		m_pNumber[nCnt] = CNumber::Create();
		m_pNumber[nCnt]->SetNum(nTopscore);
		m_pNumber[nCnt]->SetPos(
			m_pos + D3DXVECTOR3(100.0f + (MAX_NUMBERSCORE * TOPSCORE_SIZE) / 2 - nCnt * TOPSCORE_SIZE * 2, 0.0f, 0.0f),
			TOPSCORE_SIZE);
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CTopscore::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_NUMBERSCORE; nCnt++)
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
void CTopscore::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_NUMBERSCORE; nCnt++)
	{
		m_pNumber[nCnt]->Update();
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CTopscore::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_NUMBERSCORE; nCnt++)
	{
		m_pNumber[nCnt]->Draw();
	}
}

// ----------------------------------------
// スコアの読み込み処理
// ----------------------------------------
void CTopscore::Load(void)
{
	// 変数宣言
	FILE * pFile;

	pFile = fopen("data/TEXT/rank.txt", "r");

	// ファイルが開かれているとき
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_TOPSCORE; nCnt++)
		{
			fscanf(pFile, "%d", &m_nTopScore[nCnt]);
		}
		fclose(pFile);
	}
	else
	{
		fclose(pFile);
	}
}

// ----------------------------------------
// スコアのセーブ処理
// ----------------------------------------
void CTopscore::Save(void)
{
	// 変数宣言
	FILE * pFile;

	pFile = fopen("data/TEXT/rank.txt", "w");

	// ファイルが開かれているとき
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_TOPSCORE; nCnt++)
		{
			fprintf(pFile, "%d\n", m_nTopScore[nCnt]);
		}
		fclose(pFile);
	}
	else
	{
	}
}

// ----------------------------------------
// スコア設定処理
// ----------------------------------------
void CTopscore::SetScore(int nScore)
{
	// 変数宣言
	int nAllScore[6];
	int nComp = nScore;
	int nCont;
	// 仮の変数に代入
	for (int nCnt = 0; nCnt < MAX_TOPSCORE; nCnt++)
	{
		nAllScore[nCnt] = m_nTopScore[nCnt];
	}
	// 最後の配列に今回のスコアを代入
	nAllScore[5] = nScore;
	// 現在のスコアを保存
	m_nMyScore = nScore;
	// インランク初期化
	m_nInRank = -1;

	// ランク入れ替え
	for (int nCnt = 0; nCnt < MAX_TOPSCORE; nCnt++)
	{
		for (int nCnt2 = nCnt + 1; nCnt2 < MAX_TOPSCORE + 1; nCnt2++)
		{
			// 比較
			if (nAllScore[nCnt] < nAllScore[nCnt2])
			{
				nCont = nAllScore[nCnt];
				nAllScore[nCnt] = nAllScore[nCnt2];
				nAllScore[nCnt2] = nCont;
			}
		}
	}
	// 保存用に代入
	for (int nCnt = 0; nCnt < MAX_TOPSCORE; nCnt++)
	{
		m_nTopScore[nCnt] = nAllScore[nCnt];
		// 初期値だったら
		if (m_nInRank == -1)
		{
			// myスコアランクイン
			if (m_nMyScore == m_nTopScore[nCnt])
			{
				m_nInRank = nCnt;
			}
		}
	}
}

// ----------------------------------------
// 上位スコア設定処理
// ----------------------------------------
void CTopscore::SetTopscore(int nNumber)
{
	// 変数宣言
	int nTopscore;
	for (int nCnt = 0; nCnt < MAX_NUMBERSCORE; nCnt++)
	{
		// 桁の数字
		nTopscore = m_nTopScore[nNumber] % (int)powf(10.0f, (float)nCnt + 1.0f) / (int)powf(10.0f, (float)nCnt);
		m_pNumber[nCnt]->SetNum(nTopscore);
		m_pNumber[nCnt]->SetPos(
			m_pos + D3DXVECTOR3(0.0f +(MAX_NUMBERSCORE * TOPSCORE_SIZE) / 2 - nCnt * TOPSCORE_SIZE, 0.0f, 0.0f), 
			TOPSCORE_SIZE);
	}
	// myスコアが上がった時
	if (m_nInRank == nNumber)
	{
		CMark::Create(
			m_pos + D3DXVECTOR3(30.0f,0.0f,0.0f),
			D3DXVECTOR2(MAX_NUMBERSCORE * TOPSCORE_SIZE + TOPSCORE_SIZE + 20, TOPSCORE_SIZE),
			CScene::ACTOR_MARK);
	}

}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CTopscore * CTopscore::Create(D3DXVECTOR3 pos)
{
	// 変数宣言
	CTopscore * pTopscore;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pTopscore = new CTopscore();
	// 位置情報代入
	pTopscore->m_pos = pos;
	// 初期化処理
	pTopscore->Init();
	// 生成したオブジェクトを返す
	return pTopscore;
}
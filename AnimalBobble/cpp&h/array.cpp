// ----------------------------------------
//
// 配列処理の説明[array.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "array.h"
#include "line.h"
#include "bullet.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

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
int CArray::m_nDownCnt = 0;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CArray::CArray() : CScene(ACTOR_ARRAY, LAYER_UI)
{
	m_BiginPos;							// 生成されたときの初期位置
	m_OriginPos;						// 初期位置
	m_PlayerId = CManager::PLAYER_1;	// プレイヤー番号
	m_pline = NULL;						// 線
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CArray::~CArray()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CArray::Init(void)
{

	//----- 配列の設定 -----//
	// 行
	for (int nCntLine = 0; nCntLine < MAX_ARRAY_LINE; nCntLine++)
	{
		// 偶数の場合
		if (nCntLine % 2 == 0)
		{
			// 列
			for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
			{
				// 位置設定
				m_VariousPos[nCntLine][nCntColumn] = 
					m_OriginPos + 
					D3DXVECTOR3(BULLETSIZE * nCntColumn, BULLETSIZE * nCntLine, 0.0f);
				// 配置状態
				m_bState[nCntLine][nCntColumn] = false;
			}
		}
		// 奇数の場合
		else
		{
			// 列
			for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
			{
				// 位置設定
				m_VariousPos[nCntLine][nCntColumn] =
					m_OriginPos +
					D3DXVECTOR3(BULLETSIZE / 2 + BULLETSIZE * nCntColumn, BULLETSIZE * nCntLine,0.0f);
				// 配置状態
				m_bState[nCntLine][nCntColumn] = false;
			}
		}
		// 上限線
		if (nCntLine == MAX_ARRAY_LINE - 1)
		{
			// 上限の線生成
			m_pline	= CLine::Create(
				m_VariousPos[MAX_ARRAY_LINE - 1][0] + D3DXVECTOR3(-BULLETSIZE, -BULLETSIZE / 2, 0.0f),
				m_VariousPos[MAX_ARRAY_LINE - 1][MAX_ARRAY_COLUMN - 1] + D3DXVECTOR3(0.0f, -BULLETSIZE / 2, 0.0f));
		}
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CArray::Uninit(void)
{
	// 線の破棄
	if (m_pline != NULL)
	{
		m_pline->Uninit();
		delete m_pline;
		m_pline = NULL;
	}
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CArray::Update(void)
{
	// ヌルチェック
	if (m_pline != NULL)
	{
		m_pline->Update();
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CArray::Draw(void)
{
	// ヌルチェック
	if (m_pline != NULL)
	{
		m_pline->Draw();
	}
}

// ----------------------------------------
// 位置情報取得処理
// ----------------------------------------
D3DXVECTOR3 CArray::GetPos(int nCntLine, int nCntColumn)
{
	return m_VariousPos[nCntLine][nCntColumn];
}

// ----------------------------------------
// 上の位置情報補正処理
// ----------------------------------------
D3DXVECTOR3 CArray::TopPosCorrection(
	D3DXVECTOR3 &pos,	// 位置アドレス
	int nLine, 			// 行
	int *nIdColumn		// 列ポインタ
)
{
	// 変数宣言
	D3DXVECTOR3 posDiffe = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Lengh[MAX_ARRAY_COLUMN];

	// 位置情報の差分
	posDiffe = CCalculation::Difference_Between(pos, m_VariousPos[nLine][*nIdColumn]);	// 弾-配列
	// 距離の計算
	Lengh[*nIdColumn] = sqrtf(
		posDiffe.x * posDiffe.x +
		posDiffe.y * posDiffe.y);

	/* 行列の設定 */
	// 列
	for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
	{
		// 配置状態がOFFの場合
		if (m_bState[nLine][nCntColumn] == false)
		{
			// 位置情報の差分
			posDiffe = CCalculation::Difference_Between(pos, m_VariousPos[nLine][nCntColumn]);	// 弾-配列
			// 距離の計算
			Lengh[nCntColumn] = sqrtf(
				posDiffe.x * posDiffe.x +
				posDiffe.y * posDiffe.y);
			// 距離の比較
			if (Lengh[*nIdColumn] > Lengh[nCntColumn])
			{
				// 行列設定
				*nIdColumn = nCntColumn;
			}
		}
	}
	// 配置状態ON
	m_bState[nLine][*nIdColumn] = true;
	return m_VariousPos[nLine][*nIdColumn];
}

// ----------------------------------------
// 周囲の位置情報補正処理
// ----------------------------------------
D3DXVECTOR3 CArray::AroundPosCorrection(
	D3DXVECTOR3 &pos,	// 位置アドレス
	int nLine,			// 行
	int nColumn,		// 列
	int *nIdLine,		// 行情報ポインタ
	int *nIdColumn		// 列情報ポインタ
)
{
	// 変数宣言
	D3DXVECTOR3 posDiffe = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Lengh[MAX_ARRAY_LINE][MAX_ARRAY_COLUMN] = {};

	// 初めの行番号記入
	for (int nCntLine = nLine + 1; nCntLine >= nLine - 1; nCntLine--)
	{
		// 不明な行へのアクセス拒否
		if (nCntLine >= 0)
		{
			*nIdLine = nCntLine;
		}
	}
	// 初めの列番号記入
	for (int nCntColumn = nColumn + 1; nCntColumn >= nColumn - 1; nCntColumn--)
	{
		// 不明な行へのアクセス拒否
		if (nCntColumn >= 0)
		{
			*nIdColumn = nCntColumn;
		}
	}

	// 位置情報の差分
	posDiffe = CCalculation::Difference_Between(pos, m_VariousPos[*nIdLine][*nIdColumn]);	// 弾-配列
	// 距離の計算
	Lengh[*nIdLine][*nIdColumn] = sqrtf(
		posDiffe.x * posDiffe.x +
		posDiffe.y * posDiffe.y);

	/* 行列の設定 */
	// 行
	for (int nCntLine = nLine - 1; nCntLine <= nLine + 1; nCntLine++)
	{
		// 列
		for (int nCntColumn = nColumn - 1; nCntColumn <= nColumn + 1; nCntColumn++)
		{
			// 配列内の場合
			if (nCntLine >= 0 && nCntLine <= MAX_ARRAY_LINE - 1 &&		// 行の範囲
				nCntColumn >= 0 && nCntColumn <= MAX_ARRAY_COLUMN - 1)	// 列の範囲
			{
				// 配置状態
				if (m_bState[nCntLine][nCntColumn] == false &&		// 配列になにも配置されていない状態
					!(nCntLine == nLine && nCntColumn == nColumn))	// 同じ行かつ同じ列ではない場合
				{
					// 位置情報の差分
					posDiffe = CCalculation::Difference_Between(pos, m_VariousPos[nCntLine][nCntColumn]);	// 弾-配列
																														// 距離の計算
					Lengh[nCntLine][nCntColumn] = sqrtf(
						posDiffe.x * posDiffe.x +
						posDiffe.y * posDiffe.y);
					// 距離の比較
					if (fabsf(Lengh[*nIdLine][*nIdColumn]) > fabsf(Lengh[nCntLine][nCntColumn]))
					{
						// 行列設定
						*nIdLine = nCntLine;
						*nIdColumn = nCntColumn;
					}
				}
			}
		}
	}
	// 配置状態ON
	m_bState[*nIdLine][*nIdColumn] = true;
 	return m_VariousPos[*nIdLine][*nIdColumn];
}

// ----------------------------------------
// 配置状態を一つ下げる処理
// ----------------------------------------
D3DXVECTOR3 CArray::TargetPosCorrection(
	int nLine,	// 行
	int nColumn	// 列
)
{
	return m_VariousPos[nLine][nColumn];
}

// ----------------------------------------
// 初期値を変化処理
// ----------------------------------------
void CArray::OriginPosChange(void)
{
	// 初期位置yを一つ下に下げる
	m_OriginPos.y += BULLETSIZE;
	// カウントアップ
	m_nDownCnt++;
	// 配列位置を再設定\\
	// 行
	for (int nCntLine = 0; nCntLine < MAX_ARRAY_LINE; nCntLine++)
	{
		// 偶数の場合
		if (nCntLine % 2 == 0)
		{
			// 列
			for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
			{
				m_VariousPos[nCntLine][nCntColumn] = m_OriginPos + D3DXVECTOR3(BULLETSIZE * nCntColumn, BULLETSIZE * nCntLine, 0.0f);
			}
		}
		// 奇数の場合
		else
		{
			// 列
			for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
			{
				m_VariousPos[nCntLine][nCntColumn] = m_OriginPos + D3DXVECTOR3(BULLETSIZE / 2 + BULLETSIZE * nCntColumn, BULLETSIZE * nCntLine, 0.0f);
			}
		}
	}
}

// ----------------------------------------
// 生成されたときの初期値処理
// ----------------------------------------
void CArray::BiginPos(void)
{
	m_OriginPos = m_BiginPos;
	m_nDownCnt = 0;				// カウントダウンを初期化
	// 配列位置を再設定\\
		// 行
	for (int nCntLine = 0; nCntLine < MAX_ARRAY_LINE; nCntLine++)
	{
		// 偶数の場合
		if (nCntLine % 2 == 0)
		{
			// 列
			for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
			{
				m_VariousPos[nCntLine][nCntColumn] = m_OriginPos + D3DXVECTOR3(BULLETSIZE * nCntColumn, BULLETSIZE * nCntLine, 0.0f);
			}
		}
		// 奇数の場合
		else
		{
			// 列
			for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
			{
				m_VariousPos[nCntLine][nCntColumn] = m_OriginPos + D3DXVECTOR3(BULLETSIZE / 2 + BULLETSIZE * nCntColumn, BULLETSIZE * nCntLine, 0.0f);
			}
		}
	}

}

// ----------------------------------------
// 配置状態処理
// ----------------------------------------
void CArray::DeleteState(int nCntLine, int nCntColumn)
{
	m_bState[nCntLine][nCntColumn] = false;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
void CArray::SetState(int nCntline, int nCntColumn,bool bState)
{
	m_bState[nCntline][nCntColumn] = bState;
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CArray * CArray::Create(CManager::PLAYER playerId, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 変数宣言
	CArray * pArray;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pArray = new CArray();
	// 初期位置の設定
	pArray->m_OriginPos = D3DXVECTOR3(pos.x - size.x / 2 + BULLETSIZE / 2, pos.y - size.y / 2 + BULLETSIZE / 2, 0.0f);
	// 生成されたときの初期配置
	pArray->m_BiginPos = pArray->m_OriginPos;
	// 初期化処理
	pArray->Init();
	// プレイヤー番号
	pArray->m_PlayerId = playerId;
	// 生成したオブジェクトを返す
	return pArray;
}

// ----------------------------------------
// ダウンカウントを取得処理
// ----------------------------------------
int CArray::GetDownCnt(void)
{
	return m_nDownCnt;
}
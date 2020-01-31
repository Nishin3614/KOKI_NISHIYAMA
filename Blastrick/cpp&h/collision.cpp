// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Collision処理の説明[calculation.cpp]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Collision.h"
#include "3Dline.h"

#ifdef _DEBUG
#include "debugproc.h"
#include "debugcollision.h"
#endif // _DEBUG

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define COLLISION_FILE "data/LOAD/collision.txt"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// グローバル変数
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CCollision::m_nAll = 0;						// 総数
int CCollision::m_nNumLoad = 0;					// ロード数
CCollision::INFO CCollision::m_Info[128] = {};	// 情報

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCollision::CCollision() : CScene::CScene(ACTOR_COLLISION,LAYER_COLLISION)
{
	// 初期化
	m_ID = m_nAll;								// 番号
	m_objtype = OBJTYPE_ACTOR;						// タイプ
	m_ppos = NULL;								// 位置情報
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回の位置情報
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ情報
	m_pmove = NULL;								// 移動情報
	m_nAll++;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCollision::~CCollision()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Init(void)
{
	m_bUse = true;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Uninit(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Update(void)
{
	if (m_objtype == OBJTYPE_ACTOR)	Collision();
	m_posOld = *m_ppos;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Draw(void)
{
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Debug(void)
{
}
#endif // _DEBUG

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 生成処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCollision * CCollision::Create(void)
{
	// 変数宣言
	CCollision * pCollision;	// 当たり判定
	// メモリ確保
	pCollision = new CCollision;
	// 初期化
	pCollision->Init();
	// 返す
	return pCollision;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだものを生成
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::LoadCreate(void)
{
	// 変数宣言
	CCollision * pCollision;	// 当たり判定
	for (int nCntLoad = 0; nCntLoad < m_nNumLoad; nCntLoad++)
	{
		pCollision = CCollision::Create();
		pCollision->SetPos(&m_Info[nCntLoad].pos);
		pCollision->SetSize(m_Info[nCntLoad].size);
		pCollision->SetMove(NULL);
		pCollision->SetType(CCollision::OBJTYPE_WAIT);
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 位置設定処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::SetPos(D3DXVECTOR3 * pos)
{
	m_ppos = pos;
	m_posOld = *pos;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// サイズ設定処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::SetSize(D3DXVECTOR3 const size)
{

	m_size = size;
#ifdef _DEBUG
	CDebugcollision::Create(m_ppos, &m_size, CDebugcollision::COLLISIONTYPE_BOX);
#endif // _DEBUG

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 移動設定処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::SetMove(D3DXVECTOR3 * move)
{
	m_pmove = move;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// タイプ設定処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::SetType(OBJTYPE const type)
{
	m_objtype = type;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 使用状態処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::SetUse(bool const bUse)
{
	m_bUse = bUse;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 箱型の当たり判定処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::Collision_Come(void)
{
	// 使用状態ではない状態なら関数を抜ける
	if (!m_bUse)
	{
		return false;
	}
	// 変数宣言
	CCollision * pCollision = NULL;
	bool bOn = false;			// 上にいるかどうか
	// 当たり判定同士の比較
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_COLLISION); nCntLayer++)
	{
		// 情報取得
		pCollision = (CCollision*)CScene::GetActor(CScene::ACTOR_COLLISION, CScene::LAYER_COLLISION, nCntLayer);	// 当たり判定
		// 条件を満たしているか
		if (pCollision == NULL ||	// ヌルチェック
			pCollision == this)		// 同じ情報
		{							 
			continue;
		}

		// 素材のY範囲
		if (this->m_ppos->y + this->m_size.y * 0.5f > pCollision->m_ppos->y - pCollision->m_size.y * 0.5f&&
			this->m_ppos->y - this->m_size.y * 0.5f < pCollision->m_ppos->y + pCollision->m_size.y * 0.5f)
		{
			// 素材のZ範囲
			if (this->m_ppos->z + this->m_size.z * 0.5f > pCollision->m_ppos->z - pCollision->m_size.z * 0.5f&&
				this->m_ppos->z - this->m_size.z * 0.5f < pCollision->m_ppos->z + pCollision->m_size.z * 0.5f)
			{
				// 当たり判定(左)
				if (this->m_ppos->x + this->m_size.x * 0.5f > pCollision->m_ppos->x - pCollision->m_size.x * 0.5f&&
					this->m_posOld.x + this->m_size.x * 0.5f <= pCollision->m_ppos->x - pCollision->m_size.x * 0.5f)
				{
					// 素材状の左に
					this->m_ppos->x = pCollision->m_ppos->x - pCollision->m_size.x * 0.5f - this->m_size.x * 0.5f;
				}

				// 当たり判定(右)
				else if (this->m_ppos->x - this->m_size.x * 0.5f < pCollision->m_ppos->x + pCollision->m_size.x * 0.5f&&
					m_posOld.x - this->m_size.x * 0.5f >= pCollision->m_ppos->x + pCollision->m_size.x * 0.5f)
				{
					// 素材状の左に
					this->m_ppos->x = pCollision->m_ppos->x + pCollision->m_size.x * 0.5f + this->m_size.x * 0.5f;
				}
			}

			// 素材のX範囲
			if (this->m_ppos->x + this->m_size.x * 0.5f > pCollision->m_ppos->x - pCollision->m_size.x * 0.5f&&
				this->m_ppos->x - this->m_size.x * 0.5f < pCollision->m_ppos->x + pCollision->m_size.x * 0.5f)
			{
				// 当たり判定(手前)
				if (this->m_ppos->z + this->m_size.z * 0.5f > pCollision->m_ppos->z - pCollision->m_size.z * 0.5f&&
					m_posOld.z + this->m_size.z * 0.5f <= pCollision->m_ppos->z - pCollision->m_size.z * 0.5f)
				{
					// 素材状の左に
					this->m_ppos->z = pCollision->m_ppos->z - pCollision->m_size.z * 0.5f - this->m_size.z * 0.5f;
				}

				// 当たり判定(奥)
				else if (this->m_ppos->z - this->m_size.z * 0.5f < pCollision->m_ppos->z + pCollision->m_size.z * 0.5f&&
					m_posOld.z - this->m_size.z * 0.5f >= pCollision->m_ppos->z + pCollision->m_size.z * 0.5f)
				{
					// 素材状の左に
					this->m_ppos->z = pCollision->m_ppos->z +
						pCollision->m_size.z * 0.5f +
						this->m_size.z * 0.5f + 0.1f;
				}
			}
		}

		// 素材のZ範囲
		if (this->m_ppos->z + this->m_size.z * 0.5f > pCollision->m_ppos->z - pCollision->m_size.z * 0.5f&&
			this->m_ppos->z - this->m_size.z * 0.5f < pCollision->m_ppos->z + pCollision->m_size.z * 0.5f)
		{
			// 素材のX範囲
			if (this->m_ppos->x + this->m_size.x * 0.5f > pCollision->m_ppos->x - pCollision->m_size.x * 0.5f&&
				this->m_ppos->x - this->m_size.x * 0.5f < pCollision->m_ppos->x + pCollision->m_size.x * 0.5f)
			{
				// 当たり判定(下)
				if (this->m_ppos->y + this->m_size.y * 0.5f > pCollision->m_ppos->y - pCollision->m_size.y * 0.5f&&
					m_posOld.y + this->m_size.y * 0.5f <= pCollision->m_ppos->y - pCollision->m_size.y * 0.5f)
				{
					// 素材状の左に
					this->m_ppos->y = this->m_posOld.y;
					// 移動量の初期化
					this->m_pmove->y = 0.0f;
				}

				// 当たり判定(上)
				else if (this->m_ppos->y - this->m_size.y * 0.5f < pCollision->m_ppos->y + pCollision->m_size.y * 0.5f&&
					m_posOld.y - this->m_size.y * 0.5f >= pCollision->m_ppos->y + pCollision->m_size.y * 0.5f)
				{
					// 素材状の左に
					this->m_ppos->y = pCollision->m_ppos->y + pCollision->m_size.y * 0.5f + this->m_size.y * 0.5f;
					// 移動量の初期化
					this->m_pmove->y = 0.0f;
					bOn = true;
				}
			}
		}
	}
	return bOn;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 箱型の当たり判定処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Collision(void)
{
	// 使用状態ではない状態なら関数を抜ける
	if (!m_bUse)
	{
		return;
	}
	// 変数宣言
	CCollision * pCollision = NULL;
	// 当たり判定同士の比較
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_COLLISION); nCntLayer++)
	{
		// 一つ目の当たり判定
		pCollision = (CCollision*)CScene::GetActor(CScene::ACTOR_COLLISION, CScene::LAYER_COLLISION, nCntLayer);	// 当たり判定
																													// 条件を満たしているか
		if (pCollision == NULL ||	// ヌルチェック
			pCollision == this)		// 同じ情報
		{
			continue;
		}

		// 素材のY範囲
		if (this->m_ppos->y + this->m_size.y * 0.5f > pCollision->m_ppos->y - pCollision->m_size.y * 0.5f&&
			this->m_ppos->y - this->m_size.y * 0.5f < pCollision->m_ppos->y + pCollision->m_size.y * 0.5f)
		{
			// 素材のZ範囲
			if (this->m_ppos->z + this->m_size.z * 0.5f > pCollision->m_ppos->z - pCollision->m_size.z * 0.5f&&
				this->m_ppos->z - this->m_size.z * 0.5f < pCollision->m_ppos->z + pCollision->m_size.z * 0.5f)
			{
				// 当たり判定(左)
				if (this->m_ppos->x + this->m_size.x * 0.5f > pCollision->m_ppos->x - pCollision->m_size.x * 0.5f&&
					this->m_posOld.x + this->m_size.x * 0.5f <= pCollision->m_ppos->x - pCollision->m_size.x * 0.5f)
				{
					// 素材状の左に
					this->m_ppos->x = pCollision->m_ppos->x - pCollision->m_size.x * 0.5f - this->m_size.x * 0.5f;
				}

				// 当たり判定(右)
				else if (this->m_ppos->x - this->m_size.x * 0.5f < pCollision->m_ppos->x + pCollision->m_size.x * 0.5f&&
					m_posOld.x - this->m_size.x * 0.5f >= pCollision->m_ppos->x + pCollision->m_size.x * 0.5f)
				{
					// 素材状の左に
					this->m_ppos->x = pCollision->m_ppos->x + pCollision->m_size.x * 0.5f + this->m_size.x * 0.5f;
				}
			}

			// 素材のX範囲
			if (this->m_ppos->x + this->m_size.x * 0.5f > pCollision->m_ppos->x - pCollision->m_size.x * 0.5f&&
				this->m_ppos->x - this->m_size.x * 0.5f < pCollision->m_ppos->x + pCollision->m_size.x * 0.5f)
			{
				// 当たり判定(手前)
				if (this->m_ppos->z + this->m_size.z * 0.5f > pCollision->m_ppos->z - pCollision->m_size.z * 0.5f&&
					m_posOld.z + this->m_size.z * 0.5f <= pCollision->m_ppos->z - pCollision->m_size.z * 0.5f)
				{
					// 素材状の左に
					this->m_ppos->z = pCollision->m_ppos->z - pCollision->m_size.z * 0.5f - this->m_size.z * 0.5f;
				}

				// 当たり判定(奥)
				else if (this->m_ppos->z - this->m_size.z * 0.5f < pCollision->m_ppos->z + pCollision->m_size.z * 0.5f&&
					m_posOld.z - this->m_size.z * 0.5f >= pCollision->m_ppos->z + pCollision->m_size.z * 0.5f)
				{
					// 素材状の左に
					this->m_ppos->z = pCollision->m_ppos->z +
						pCollision->m_size.z * 0.5f +
						this->m_size.z * 0.5f + 0.1f;
				}
			}
		}

		// 素材のZ範囲
		if (this->m_ppos->z + this->m_size.z * 0.5f > pCollision->m_ppos->z - pCollision->m_size.z * 0.5f&&
			this->m_ppos->z - this->m_size.z * 0.5f < pCollision->m_ppos->z + pCollision->m_size.z * 0.5f)
		{
			// 素材のX範囲
			if (this->m_ppos->x + this->m_size.x * 0.5f > pCollision->m_ppos->x - pCollision->m_size.x * 0.5f&&
				this->m_ppos->x - this->m_size.x * 0.5f < pCollision->m_ppos->x + pCollision->m_size.x * 0.5f)
			{
				// 当たり判定(下)
				if (this->m_ppos->y + this->m_size.y * 0.5f > pCollision->m_ppos->y - pCollision->m_size.y * 0.5f&&
					m_posOld.y + this->m_size.y * 0.5f <= pCollision->m_ppos->y - pCollision->m_size.y * 0.5f)
				{
					// 素材状の左に
					this->m_ppos->y = this->m_posOld.y;
					// 移動量の初期化
					this->m_pmove->y = 0.0f;
				}

				// 当たり判定(上)
				else if (this->m_ppos->y - this->m_size.y * 0.5f < pCollision->m_ppos->y + pCollision->m_size.y * 0.5f&&
					m_posOld.y - this->m_size.y * 0.5f >= pCollision->m_ppos->y + pCollision->m_size.y * 0.5f)
				{
					// 素材状の左に
					this->m_ppos->y = pCollision->m_ppos->y + pCollision->m_size.y * 0.5f + this->m_size.y * 0.5f;
					// 移動量の初期化
					this->m_pmove->y = 0.0f;
				}
			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込み処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CCollision::Load(void)
{
	// テキストデータの読み込み
	m_nNumLoad = TextLoad();
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込み
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CCollision::TextLoad(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntColli = 0;		// モデルカウント
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

							// ファイル開
	pFile = fopen(COLLISION_FILE, "r");

	// 開けた
	if (pFile != NULL)
	{
		// スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// 一文を読み込む
			sscanf(cRaedText, "%s", &cHeadText);		// 比較用テクストに文字を代入
		}

		// スクリプトだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// エンドスクリプトが来るまでループ
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				// 初期化
				cHeadText[0] = '\0';
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);
				// コリジョンセット来たら
				if (strcmp(cHeadText, "COLLISION") == 0)
				{
					// エンドコリジョンセットが来るまでループ
					while (strcmp(cHeadText, "END_COLLISION") != 0)
					{
						// 初期化
						cHeadText[0] = '\0';
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);
						// 親情報読み込み
						if (strcmp(cHeadText, "OBJTYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&m_Info[nCntColli].type);
						}
						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&m_Info[nCntColli].pos.x,
								&m_Info[nCntColli].pos.y,
								&m_Info[nCntColli].pos.z);
						}
						// 回転情報読み込み
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&m_Info[nCntColli].size.x,
								&m_Info[nCntColli].size.y,
								&m_Info[nCntColli].size.z);
						}
					}
					// モデルの更新
					nCntColli++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "当たり判定読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}

	return nCntColli;
}

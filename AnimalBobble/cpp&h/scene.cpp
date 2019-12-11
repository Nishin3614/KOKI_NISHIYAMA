// ----------------------------------------
//
// シーン処理の説明[scene.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene.h"
#include "pause.h"

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
CScene * CScene::m_pTop[LAYER_MAX] = {};					// 先頭オブジェクトへのポインタ
CScene * CScene::m_pCur[LAYER_MAX] = {};					// 現在オブジェクトへのポインタ
int CScene::m_nMaxLayer[LAYER_MAX] = {};					// レイヤーの最大数

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CScene::CScene()
{
}

// ----------------------------------------
// オーバーロードコンストラクタ
// ----------------------------------------
CScene::CScene(ACTOR actor, LAYER layer)
{
	/* 現在の奴 */
	if (m_pCur[layer] != NULL)
	{
		// 前のオブジェクトのポインタ
		this->m_pPrey = m_pCur[layer];
		// 次のオブジェクトのポインタ
		m_pCur[layer]->m_pNext = this;
	}

	// 現在のポインタ
	m_pCur[layer] = this;

	// 先頭オブジェクトのポインタ
	if (m_pTop[layer] == NULL)
	{
		m_pTop[layer] = this;
	}
	/* 過去の奴 */
	// 描画順設定
	m_layer = layer;
	// オブジェクト情報設定
	m_nActor = actor;
	// フラグ
	m_bDeadFrag = false;
	// レイヤーアップ
	m_nMaxLayer[layer]++;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CScene::~CScene()
{
}

// ----------------------------------------
// シーン親子リリース処理
// ----------------------------------------
void CScene::ReleaseAll(void)
{
	/* 現在の奴 */
	// 変数宣言
	CScene * pScene = NULL;
	CScene *pSceneNext;	// 次のシーン
	for (int nCnt = 0; nCnt < LAYER_MAX; nCnt++)
	{
		// 先頭ポイントを格納
		pScene = m_pTop[nCnt];
		while (pScene != NULL)
		{
			pSceneNext = pScene->m_pNext;
			pScene->Release();
			delete pScene;
			pScene = NULL;
			pScene = pSceneNext;
		}
		// 先頭ポイントを格納
		m_pTop[nCnt] = NULL;
		// 現在のオブジェクトにNULL
		m_pCur[nCnt] = NULL;
	}
}

// ----------------------------------------
// シーン親子更新処理
// ----------------------------------------
void CScene::UpdateAll(void)
{
	// 変数宣言
	CScene * pScene = NULL;
	CScene *pSceneNext;	// 次のシーン
	for (int nCnt = 0; nCnt < LAYER_MAX; nCnt++)
	{
		// 先頭ポイントを格納
		pScene = m_pTop[nCnt];
		while (pScene != NULL)
		{
			// 次のポインタを代入
			pSceneNext = pScene->m_pNext;
			// ポーズUI以外
			if (pScene->m_nActor != ACTOR_PAUSE_UI)
			{
				pScene->Update();
			}
			// ポーズUI　
			else
			{
				// ポーズ状態の時
				if (CPause::GetPause() == true)
				{
					pScene->Update();
				}
			}
			pScene = pSceneNext;
		}
	}
	// フラグが立ったものをすべてリリース
	DeadFragAll();
}

// ----------------------------------------
// シーン親子描画処理
// ----------------------------------------
void CScene::DrawAll(void)
{
	/* 現在の奴 */
	// 変数宣言
	CScene * pScene = NULL;
	CScene *pSceneNext;	// 次のシーン
	for (int nCnt = 0; nCnt < LAYER_MAX; nCnt++)
	{
		// 先頭ポイントを格納
		pScene = m_pTop[nCnt];
		while (pScene != NULL)
		{
			// 次のポインタ情報格納
			pSceneNext = pScene->m_pNext;
			// ポーズUI以外
			if (pScene->m_nActor != ACTOR_PAUSE_UI)
			{
				pScene->Draw();
			}
			// ポーズUI　
			else
			{
				// ポーズ状態の時
				if (CPause::GetPause() == true)
				{
					pScene->Draw();
				}
			}
			pScene = pSceneNext;
		}
	}
}

// ----------------------------------------
// シーンの取得
// ----------------------------------------
CScene *CScene::GetActor(ACTOR actor, LAYER layer, int nCntScene)
{
	// 変数宣言
	CScene * pScene = NULL;
	CScene *pSceneNext;	// 次のシーン
	int nCnt = 0;
	// 先頭ポイントを格納
	pScene = m_pTop[layer];
	// カウント
	for (int nCnt = 0; nCnt < nCntScene; nCnt++)
	{
		// 次のポインタ情報格納
		if (pScene != NULL)
		{
			pSceneNext = pScene->m_pNext;
			pScene = pSceneNext;
		}
	}
	// シーン情報を返す
	if (pScene != NULL)
	{
		if (pScene->m_nActor == actor) return pScene;
		else return NULL;
	}
	else return NULL;
}

// ----------------------------------------
// レイヤーの最大数の取得
// ----------------------------------------
int CScene::GetMaxLayer(LAYER layer)
{
	return m_nMaxLayer[layer];
}

// ----------------------------------------
// オブジェクトの破棄
// ----------------------------------------
void CScene::Release(void)
{
	// 死亡フラグが立った
	m_bDeadFrag = true;
}

// ----------------------------------------
// 死亡フラグが立ったものをリリース処理
// ----------------------------------------
void CScene::DeadFragAll(void)
{
	// 変数宣言
	CScene * pScene;	// シーン
	CScene *pSceneNext;	// 次のシーン

	for (int nCnt = 0; nCnt < LAYER_MAX; nCnt++)
	{
		// 先頭ポイントを格納
		pScene = m_pTop[nCnt];
		while (pScene != NULL)
		{
			// 次のポインタを代入
			pSceneNext = pScene->m_pNext;
			// 死亡フラグが立っているとき
			if (pScene->m_bDeadFrag == true)
			{
				// 先頭と現在のオブジェクトのポインタが同じ場合
				if (pScene == m_pTop[nCnt] && pScene == m_pCur[nCnt])
				{
					m_pTop[nCnt] = NULL;
					m_pCur[nCnt] = NULL;
				}
				// 先頭オブジェクトのポインタの変更
				else if (pScene == m_pTop[nCnt])
				{
					m_pTop[nCnt] = pSceneNext;
					m_pTop[nCnt]->m_pPrey = NULL;
				}
				// 現在のオブジェクトのポインタの変更
				else if (pScene == m_pCur[nCnt])
				{
					m_pCur[nCnt] = pScene->m_pPrey;
					m_pCur[nCnt]->m_pNext = NULL;
				}
				// 間のコネクト
				else
				{
					// 前回の次の情報を代入
					pScene->m_pPrey->m_pNext = pSceneNext;
					// 次の前回の情報を代入
					pSceneNext->m_pPrey = pScene->m_pPrey;
				}
				// レイヤーダウン
				m_nMaxLayer[nCnt]--;

				pScene->Uninit();
				delete pScene;
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}
}

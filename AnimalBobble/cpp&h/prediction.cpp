// ----------------------------------------
//
// 予測線処理の説明[prediction.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "prediction.h"
#include "frame.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define PREDICTION_TEXTURE ("data/TEXTURE/Player/Line.png")

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
LPDIRECT3DTEXTURE9 CPrediction::m_pTex = NULL;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CPrediction::CPrediction() : CScene_TWO::CScene_TWO(ACTOR_PREDICTION, LAYER_OBJ)
{
	/* 変数の初期化 */
	// 位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// タイプ
	m_type = CPrediction::TYPE_BEE;
	// プレイヤー番号
	m_PlayerId = CManager::PLAYER_1;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CPrediction::~CPrediction()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CPrediction::Init(void)
{
	// サイズ設定
	CScene_TWO::SetSize(D3DXVECTOR2(50.0f, 300.0f));
	// 初期化
	CScene_TWO::Init();
	// 長さ
	CScene_TWO::SetLengh(D3DXVECTOR2(CScene_TWO::GetSize().x / 2, CScene_TWO::GetSize().y));
	// 現在の位置設定
	CScene_TWO::SetPosition(m_pos);
	// 色設定
	CScene_TWO::SetCol(m_col);
	// テクスチャー設定
	CScene_TWO::BindTexture(m_pTex);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CPrediction::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CPrediction::Update(void)
{
	// 現在の位置設定
	CScene_TWO::SetPosition(m_pos);
	// 更新
	CScene_TWO::Update();
	// 長さ
	CScene_TWO::SetLengh(D3DXVECTOR2(CScene_TWO::GetSize().x / 2, CScene_TWO::GetSize().y));
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CPrediction::Draw(void)
{
	// 描画処理
	CScene_TWO::Draw();
}

// ----------------------------------------
// 位置設定処理
// ----------------------------------------
void CPrediction::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
	CScene_TWO::SetPosition(pos);
}

// ----------------------------------------
// 回転設定処理
// ----------------------------------------
void CPrediction::SetRot(float fRot)
{
	Limit(fRot);					// 限界点
	CScene_TWO::SetRot(fRot);	// 回転設定
}

// ----------------------------------------
// 限界点処理
// ----------------------------------------
void CPrediction::Limit(float fRot)
{
	// 変数宣言
	CFrame *pFrame = NULL;
	float fCons = 0;
	float fSection = 0;
	float fLengh;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posdiff;
	D3DXVECTOR2 Linear;

	// 情報取得
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
	{
		if (pFrame == NULL)	pFrame = (CFrame*)CScene::GetActor(ACTOR_FRAME, LAYER_OBJ, nCntLayer);	// フレーム
	}

	// xの位置が同じじゃない場合
	if (m_pos.x != m_pos.x + sinf(fRot))
	{
		// 一次関数
		Linear = CCalculation::Linear_function(m_pos, D3DXVECTOR3(m_pos.x + sinf(fRot), m_pos.y + cosf(fRot), 0.0f));

		/* 左上限 */
		if (Linear.x * (pFrame->GetPos().x - pFrame->GetSize().x / 2) + Linear.y < (pFrame->GetPos().y + pFrame->GetSize().y / 2) &&
			Linear.x * (pFrame->GetPos().x - pFrame->GetSize().x / 2) + Linear.y >(pFrame->GetPos().y - pFrame->GetSize().y / 2))
		{
			pos = D3DXVECTOR3(
				pFrame->GetPos().x - pFrame->GetSize().x / 2,
				Linear.x * (pFrame->GetPos().x - pFrame->GetSize().x / 2) + Linear.y,
				0.0f);
		}
		/* 右上限*/
		else if (Linear.x * (pFrame->GetPos().x + pFrame->GetSize().x / 2) + Linear.y < (pFrame->GetPos().y + pFrame->GetSize().y / 2) &&
			Linear.x * (pFrame->GetPos().x + pFrame->GetSize().x / 2) + Linear.y >(pFrame->GetPos().y - pFrame->GetSize().y / 2))
		{
			pos = D3DXVECTOR3(
				pFrame->GetPos().x + pFrame->GetSize().x / 2,
				Linear.x * (pFrame->GetPos().x + pFrame->GetSize().x / 2) + Linear.y,
				0.0f);
		}
		/* 上上限 */
		else if ((Linear.y - pFrame->GetPos().y - pFrame->GetSize().y / 2) / -Linear.x > (pFrame->GetPos().x - pFrame->GetSize().x / 2) &&
			(Linear.y - pFrame->GetPos().y - pFrame->GetSize().y / 2) / -Linear.x < (pFrame->GetPos().x + pFrame->GetSize().x / 2))
		{
			pos = D3DXVECTOR3(
				(Linear.y - (pFrame->GetPos().y - pFrame->GetSize().y / 2)) / -Linear.x,
				pFrame->GetPos().y - pFrame->GetSize().y / 2,
				0.0f);
		}
	}
	// それ以外
	else
	{
		pos.x = m_pos.x;
		pos.y = pFrame->GetPos().y - pFrame->GetSize().y / 2;
	}
	posdiff = pos - m_pos;

	fLengh = sqrtf(posdiff.x * posdiff.x + posdiff.y * posdiff.y);
	CScene_TWO::SetSize(D3DXVECTOR2(50.0f, fLengh));
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CPrediction::Load(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	// テクスチャー設定
	D3DXCreateTextureFromFile(pDevice, PREDICTION_TEXTURE, &m_pTex);

	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CPrediction::UnLoad(void)
{
	// テクスチャの開放
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CPrediction * CPrediction::Create(CManager::PLAYER playerId,D3DXVECTOR3 pos, TYPE type)
{
	// 変数宣言
	CPrediction * pPrediction;		// 予測線2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pPrediction = new CPrediction();
	// 現在の位置
	pPrediction->m_pos = pos;
	// タイプ
	pPrediction->m_type = type;
	// 色
	switch (pPrediction->m_type)
	{
		// ハチ
	case TYPE_BEE:
		pPrediction->m_col = D3DXCOLOR(1.0f, 0.7f, 0.2f, 1.0f);
		break;
		// 猫
	case TYPE_CAT:
		pPrediction->m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
		// 魚
	case TYPE_FISH:
		pPrediction->m_col = D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
		break;
		// 鷹
	case TYPE_HAWK:
		pPrediction->m_col = D3DXCOLOR(0.4f, 0.2f, 0.2f, 1.0f);
		break;
		// 鷹
	case TYPE_SNAKE:
		pPrediction->m_col = D3DXCOLOR(0.1f, 1.0f, 0.1f, 1.0f);
		break;
	default:
		break;
	}
	// 初期化処理
	pPrediction->Init();
	// プレイヤー番号
	pPrediction->m_PlayerId = playerId;
	// 生成したオブジェクトを返す
	return pPrediction;
}
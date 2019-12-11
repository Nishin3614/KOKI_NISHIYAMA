// ----------------------------------------
//
// アイテム処理の説明[item.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "item.h"
#include "player.h"
#include "score.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define ITEMCOLLISION_SIZE (30.0f)

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------
LPDIRECT3DTEXTURE9 CItem::m_pTexture[TYPE_MAX] = {};		// テクスチャへのポインタ
CLoad	*CItem::m_pload = NULL;								// ロード
int		CItem::m_nAll = 0;									// 番号
int		CItem::m_nRotCntAll = 0;							// 回転個数
int		CItem::m_nJumpCntAll = 0;							// ジャンプ個数

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CItem::CItem()
{
	// 変数の初期化
	m_Type = TYPE_ROT;
	m_fLengh = 0;
	m_fAngle = 0;
	m_bBillboard = false;
}

// ----------------------------------------
// イニシャライザコンストラクタ
// ----------------------------------------
CItem::CItem(ACTOR actor, LAYER layer) : CScene_THREE::CScene_THREE(actor,layer)
{
	// 変数の初期化
	m_fLengh = 0;
	m_fAngle = 0;
	m_bBillboard = false;
	m_Type = TYPE_ROT;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CItem::~CItem()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CItem::Init(void)
{
	// シーン3Dの初期化
	CScene_THREE::Init();
	CScene_THREE::SetBillboard(true);				// ビルボード
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CItem::Uninit(void)
{
	CScene_THREE::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CItem::Update(void)
{
	// 変数宣言
	CPlayer *pPlayer = NULL;	// プレイヤー
	CScore *pScore = NULL;	// スコア
	// 情報取得
	for (int nCntlayer = 0; nCntlayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntlayer++)
	{
		if (pPlayer = (CPlayer *)CScene::GetActor(CScene::ACTOR_PLAYER, CScene::LAYER_3DOBJECT, nCntlayer)) break;
	}
	// 情報取得
	for (int nCntlayer = 0; nCntlayer < CScene::GetMaxLayer(CScene::LAYER_UI); nCntlayer++)
	{
		if (pScore = (CScore *)CScene::GetActor(CScene::ACTOR_SCORE, CScene::LAYER_UI, nCntlayer)) break;
	}

	// アイテムの取得
	if (CCalculation::Collision_Circle(
		GetPos(),					// 自分の位置
		ITEMCOLLISION_SIZE, 		// 自分の半径
		pPlayer->GetPos(), 			// 相手の位置
		PLAYERCOLLISION_SIZE))		// 相手の半径
	{
		if (m_Type == TYPE_ROT)
		{
			// SEを鳴らす
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_8);
			pPlayer->AddRot();					// 回転率アップ
		}
		else if (m_Type == TYPE_JUMP)
		{
			// SEを鳴らす
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_8);
			pPlayer->AddJump();					// ジャンプ率アップ
		}
		else if (m_Type == TYPE_COIN)
		{
			// SEを鳴らす
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_7);
			pScore->AddScore(SCORE_UP);			// スコアアップ
		}
		Release();
	}
	CScene_THREE::SetRot(CScene_THREE::GetRot() + D3DXVECTOR3(0.0f,0.1f,0.0f));
	CScene_THREE::Update();
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CItem::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	// アルファテストの設定に変更
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテスト有効
	pDevice->SetRenderState(D3DRS_ALPHAREF, 200);				// アルファ値の基準設定(基準より大きいと描画する)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 状況に応じてアルファテストを実行する(新ピクセル > 現在のピクセル)
	pDevice->SetRenderState(D3DRS_LIGHTING, false);				// ライティングoff
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 描画
	CScene_THREE::Draw();
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// アルファテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// アルファテスト無効
	pDevice->SetRenderState(D3DRS_LIGHTING, true);				// ライティングon
}

// ----------------------------------------
// テクスチャー設定処理
// ----------------------------------------
void CItem::SetTexture(TYPE type)
{
	// 設定
	CScene_THREE::BindTexture(m_pTexture[type]);	// テクスチャー
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CItem * CItem::Create(bool bBillboard)
{
	// 変数宣言
	CItem * pItem;		// シーン3Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pItem = new CItem(ACTOR_ITEM,LAYER_3DOBJECT);
	// ビルボードのかけるかどうか
	pItem->m_bBillboard = bBillboard;
	// 初期化処理
	pItem->Init();
	// 生成したオブジェクトを返す
	return pItem;
}

// ----------------------------------------
// 読み込み作成処理
// ----------------------------------------
void CItem::LoadCreate(void)
{
	// 変数宣言
	CItem * pItem;	// ブロック
						// ブロック
	for (int nCntItem = 0; nCntItem < m_nAll; nCntItem++)
	{
		pItem = CItem::Create(true);						// 生成
		pItem->SetTexture((TYPE)m_pload->GetInfo(nCntItem).nType);
		pItem->m_Type = (TYPE)m_pload->GetInfo(nCntItem).nType;
		pItem->SetPos(m_pload->GetInfo(nCntItem).pos);		// 位置
		pItem->SetRot(m_pload->GetInfo(nCntItem).rot);		// 回転
		pItem->SetSize(m_pload->GetInfo(nCntItem).size);	// サイズ
	}
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CItem::Load(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	char cTex[TYPE_MAX][128] =					// テクスチャー名
	{
		"data/TEXTURE/UI/RotationUP000.png",
		"data/TEXTURE/UI/JumpUP.png",
		"data/TEXTURE/UI/COIN.png"
	};

	// テクスチャー設定
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice, cTex[nCnt], &m_pTexture[nCnt]);
	}
	// ロード生成
	m_pload = new CLoad;
	// ロード読み込み
	m_nAll = m_pload->Load("data/LOAD/OBJECT/billboard.txt");

	return S_OK;
}

// ----------------------------------------
// unload処理
// ----------------------------------------
void CItem::UnLoad(void)
{
	// テクスチャー設定
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
	// ロードの破棄
	delete m_pload;
	m_pload = NULL;
}

// ----------------------------------------
// ロード取得
// ----------------------------------------
CLoad * CItem::GetLoad(void)
{
	return m_pload;
}

// ----------------------------------------
// 位置の設定処理
// ----------------------------------------
void CItem::SetPos(D3DXVECTOR3 pos)
{
	CScene_THREE::SetPos(pos);
}

// ----------------------------------------
// サイズ設定処理
// ----------------------------------------
void CItem::SetSize(D3DXVECTOR3 size)
{
	CScene_THREE::SetSize(size);
}

// ----------------------------------------
// 回転の設定処理
// ----------------------------------------
void CItem::SetRot(D3DXVECTOR3 rot)
{
	CScene_THREE::SetRot(rot);
}
// ----------------------------------------
// 色の設定処理
// ----------------------------------------
void CItem::SetCol(D3DXCOLOR col)
{
}

// ----------------------------------------
// 表示テクスチャー設定処理
// ----------------------------------------
void CItem::SetTex(D3DXVECTOR2 first, D3DXVECTOR2 last)
{
}

// ----------------------------------------
// サイズの取得処理
// ----------------------------------------
D3DXVECTOR3 CItem::GetSize(void)
{
	return CScene_THREE::GetSize();
}

// ----------------------------------------
// 回転の取得処理
// ----------------------------------------
D3DXVECTOR3 CItem::GetRot(void)
{
	return CScene_THREE::GetRot();
}

// ----------------------------------------
// 位置の取得処理
// ----------------------------------------
D3DXVECTOR3 CItem::GetPos(void)
{
	return CScene_THREE::GetPos();
}

// ----------------------------------------
//
// 目印処理の説明[mark.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "mark.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define MARK_SIZE (50.0f)
#define MARKBOMCHAN_ANIMCOUNT (4)
#define MARKBOMCHAN_HORIZONANIM (1.0f/MARKBOMCHAN_ANIMCOUNT)
#define MARKHOSSAN_ANIMCOUNT (2)
#define MARKHOSSAN_HORIZONANIM (1.0f/MARKHOSSAN_ANIMCOUNT)

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
LPDIRECT3DTEXTURE9 CMark::m_pTex[MAX_MARK] = {};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CMark::CMark()
{
}

// ----------------------------------------
// イニシャライザコンストラクタ処理
// ----------------------------------------
CMark::CMark(CScene::ACTOR actor) : CScene::CScene(actor,LAYER_UI)
{
	// 初期化
	for (int nCnt = 0; nCnt < MAX_MARK; nCnt++)
	{
		m_nHorizonAnim[nCnt] = 0;	// 水平のアニメーションパターン
		m_nVirticalAnim[nCnt] = 0;	// 垂直のアニメーションパターン
		m_nCntAnim[nCnt] = 0;		// アニメーションカウント
		m_aScene_Two[nCnt] = NULL;
	}
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CMark::~CMark()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CMark::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_MARK; nCnt++)
	{
		m_aScene_Two[nCnt] = new CScene_TWO;
		m_aScene_Two[nCnt]->SetSize({ MARK_SIZE,MARK_SIZE });
		m_aScene_Two[nCnt]->Init();
		/* 表示テクスチャー設定 */
		// ボムちゃん
		if (nCnt == 0)
		{
			// 表示テクスチャー設定
			m_aScene_Two[nCnt]->SetTex(
				D3DXVECTOR2(0.0f, 0.0f),
				D3DXVECTOR2(0.25f, 1.0f));
			// 色設定
			m_aScene_Two[nCnt]->SetCol(D3DXCOLOR(rand() % 10 / 10.0f, rand() % 10 / 10.0f, rand() % 10 / 10.0f, 1.0f));
		}
		// ほッさん
		else
		{
			// 表示テクスチャー設定
			m_aScene_Two[nCnt]->SetTex(
				D3DXVECTOR2(0.0f, 0.0f),
				D3DXVECTOR2(0.5f, 1.0f));
			// 色設定
			m_aScene_Two[nCnt]->SetCol(D3DXCOLOR(rand() % 10 / 10.0f, rand() % 10 / 10.0f, rand() % 10 / 10.0f, 1.0f));
		}
		m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);

	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CMark::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_MARK; nCnt++)
	{
		if (m_aScene_Two[nCnt] != NULL)
		{
			m_aScene_Two[nCnt]->Uninit();
			delete m_aScene_Two[nCnt];
			m_aScene_Two[nCnt] = NULL;
		}
	}
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CMark::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_MARK; nCnt++)
	{
		if (m_aScene_Two[nCnt] != NULL)
		{
			// 位置情報の更新
			m_pos += (m_posGoal - m_pos) / 20;
			// サイズ情報の更新
			m_size += (m_sizeGoal - m_size) / 20;
			// 位置・サイズの設定
			SetPos(m_pos, m_size);
			// 表示テクスチャー
			Animation(nCnt);
			m_aScene_Two[nCnt]->Update();
		}
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CMark::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	for (int nCnt = 0; nCnt < MAX_MARK; nCnt++)
	{
		if (m_aScene_Two[nCnt] != NULL)
		{
			m_aScene_Two[nCnt]->Draw();
		}
	}
	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CMark::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// テクスチャー情報代入
	char TexData[MAX_MARK][36] =
	{
		{"data/TEXTURE/Effect/effect000.png"},	// タイトル用の背景
		{"data/TEXTURE/Effect/effect000.png"},		// 選択画面用の背景
	};
	// 読み込み
	for (int nCnt = 0; nCnt < MAX_MARK; nCnt++)
	{
		// テクスチャーがあったら
		if (strcmp(TexData[nCnt], "") != 0)
		{
			D3DXCreateTextureFromFile(pDevice, TexData[nCnt], &m_pTex[nCnt]);
		}
	}

	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CMark::UnLoad(void)
{
	// テクスチャの開放
		for (int nCnt = 0; nCnt < MAX_MARK; nCnt++)
		{
			if (m_pTex[nCnt] != NULL)
			{
				m_pTex[nCnt]->Release();
				m_pTex[nCnt] = NULL;
			}
		}
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CMark * CMark::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, CScene::ACTOR actor)
{
	// 変数宣言
	CMark * pMark;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pMark = new CMark(actor);
	// 初期化処理
	pMark->Init();
	// 位置情報保存
	pMark->m_pos = pos;
	pMark->m_posGoal = pos;
	// サイズ情報保存
	pMark->m_size = size;
	pMark->m_sizeGoal = size;
	// 位置・サイズの設定
	pMark->SetPos(pos,size);
	// 生成したオブジェクトを返す
	return pMark;
}

// ----------------------------------------
// アニメーション処理
// ----------------------------------------
void CMark::Animation(int nCnt)
{
	// アニメーションカウントアップ
	m_nCntAnim[nCnt]++;
	// ボムちゃんアニメ
	if (nCnt == 0)
	{
		// 20フレームずつアニメーションする
		if (m_nCntAnim[nCnt] % 20 == 0)
		{
			// アニメーション変化
			m_nHorizonAnim[nCnt] = (m_nHorizonAnim[nCnt] + 1) % MARKBOMCHAN_ANIMCOUNT;
		}
		// 頂点テクスチャー設定
		m_aScene_Two[nCnt]->SetTex(
			D3DXVECTOR2(m_nHorizonAnim[nCnt] * MARKBOMCHAN_HORIZONANIM, 0.0f),
			D3DXVECTOR2(m_nHorizonAnim[nCnt] * MARKBOMCHAN_HORIZONANIM + MARKBOMCHAN_HORIZONANIM, 1.0f));
	}
	// ボムちゃんアニメ
	if (nCnt == 1)
	{
		// 20フレームずつアニメーションする
		if (m_nCntAnim[nCnt] % 40 == 0)
		{
			// アニメーション変化
			m_nHorizonAnim[nCnt] = (m_nHorizonAnim[nCnt] + 1) % MARKHOSSAN_ANIMCOUNT;
		}
		// 頂点テクスチャー設定
		m_aScene_Two[nCnt]->SetTex(
			D3DXVECTOR2(m_nHorizonAnim[nCnt] * MARKHOSSAN_HORIZONANIM, 0.0f),
			D3DXVECTOR2(m_nHorizonAnim[nCnt] * MARKHOSSAN_HORIZONANIM + MARKHOSSAN_HORIZONANIM, 1.0f));
	}
}

// ----------------------------------------
// 位置・サイズの設定処理
// ----------------------------------------
void CMark::SetPos(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	m_pos = pos;
	m_size = size;
	// 位置の設定
	m_aScene_Two[0]->SetPosition({ pos.x - size.x / 2,pos.y,0.0f });
	m_aScene_Two[1]->SetPosition({ pos.x + size.x / 2,pos.y,0.0f });
}

// ----------------------------------------
// 目標の位置・サイズの設定処理
// ----------------------------------------
void CMark::SetPosGoal(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	m_posGoal = pos;
	m_sizeGoal = size;
}

// ----------------------------------------
// 位置情報取得処理
// ----------------------------------------
D3DXVECTOR3 CMark::GetPos(void)
{
	return m_pos;
}

// ----------------------------------------
// サイズ情報取得処理
// ----------------------------------------
D3DXVECTOR2 CMark::GetSize(void)
{
	return m_size;
}
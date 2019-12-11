#//*************************************************************************************************************
//
// ゲームUI処理[GameUI.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "GameUI.h"
#include "scene_two.h"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CGameUI::TEXTURE_INFO	*CGameUI::m_pTextureinfo = NULL;	// テクスチャの情報
int						CGameUI::m_nNumTextureMax = 0;		// テクスチャ最大数

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CGameUI::CGameUI():CScene(ACTOR_TITLE_UI, LAYER_UI)
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CGameUI::~CGameUI()
{
}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
HRESULT CGameUI::Load(void)
{

	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice;			// デバイスのポインタ
	const char			*sFileName;			// エフェクト情報のテキストファイル名
	PFILETAG			pTextFileName;		// テクスチャファイル名
	// 変数の初期化
	pDevice = CManager::GetRenderer()->GetDevice();		// デバイスの取得
	sFileName = "data/TEXT/TextureInfo.txt";

	// テクスチャ数を取得
	m_nNumTextureMax = CGameUI::LoadNumber(sFileName, "NUMBER");

	// テクスチャファイル名のメモリ確保
	pTextFileName = new FILETAG[m_nNumTextureMax];
	// テクスチャ情報のメモリ確保
	m_pTextureinfo = new CGameUI::TEXTURE_INFO[m_nNumTextureMax];

	// テクスチャファイル名の取得
	CGameUI::LoadTextureFile(sFileName, pTextFileName, m_nNumTextureMax);

	for (int nCntTexture = 0; nCntTexture < m_nNumTextureMax; nCntTexture++)
	{
		// テクスチャの読み込み
		if (D3DXCreateTextureFromFile(pDevice, pTextFileName[nCntTexture].string, &m_pTextureinfo[nCntTexture].pTexture) != D3D_OK)
		{
			MessageBox(NULL, "項目が見つかりません\nテクスチャの読み込み失敗", "警告！", MB_ICONWARNING);
			m_pTextureinfo[nCntTexture].pTexture = NULL;
		}
	}

	return E_FAIL;


	delete[m_nNumTextureMax] pTextFileName;
	return S_OK;

}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void CGameUI::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < m_nNumTextureMax; nCntTexture++)
	{// テクスチャ情報の開放
		if (m_pTextureinfo != NULL)
		{
			if (m_pTextureinfo[nCntTexture].pTexture != NULL)
			{
				if (m_pTextureinfo[nCntTexture].pTexture != nullptr)
				{
					m_pTextureinfo[nCntTexture].pTexture->Release();
					m_pTextureinfo[nCntTexture].pTexture = NULL;
				}
			}
		}
	}
	// テクスチャ情報のポインタの開放
	delete[m_nNumTextureMax]m_pTextureinfo;
	m_pTextureinfo = NULL;
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CGameUI * CGameUI::Create(void)
{
	CGameUI *pGameUI = new CGameUI;
	pGameUI->Init();
	return pGameUI;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CGameUI::Init(void)
{
	// UI情報の読み込み
	CGameUI::LoadUI();
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CGameUI::Uninit(void)
{
	delete[m_nNumUI]m_pCScene2D;
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CGameUI::Update(void)
{
	for (int nCntGameUI = 0; nCntGameUI < m_nNumUI; nCntGameUI++)
	{

		m_pCScene2D[nCntGameUI].Update();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CGameUI::Draw(void)
{
	for (int nCntGameUI = 0; nCntGameUI < m_nNumUI; nCntGameUI++)
	{
		m_pCScene2D[nCntGameUI].Draw();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 個数の読み込み
//-------------------------------------------------------------------------------------------------------------
int CGameUI::LoadNumber(const char * TextFile, const char * SetItem)
{
	// 変数宣言
	FILE *pFile = NULL;				// ファイルのポインタ
	char cRead[128];				// 読み込み用
	char cComp[128];				// 比較用
	char cEmpty[128];				// 要らないもの用
	int nNumber = -1;				// 個数
	int nCntError = 0;				// エラー用

									// ファイルが開かれていなかったら
	if ((pFile = fopen(TextFile, "r")) == NULL)
	{// メッセージの表示
#ifdef _DEBUG
		MessageBox(NULL, "テキストファイルが見つかりませんでした", "警告！", MB_ICONWARNING);
		return -1;
#endif // _DEBUG
	}
	// スクリプトが存在しているかチェック
	if (!CGameUI::Existenceofscript(pFile))
	{
#ifdef _DEBUG
		MessageBox(NULL, "[SCRIP]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
		fclose(pFile);
		return -1;
#endif // _DEBUG
	}
	// END_SCRIPTが来るまで
	while (strcmp(cComp, "END_SCRIPT") != 0)
	{// END_SCRIPTまでループ
	 // 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > 1048576)
		{// エラー
			nCntError = 0;
#ifdef _DEBUG
			MessageBox(NULL, "[END_SCRIPT]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
			fclose(pFile);
			return -1;
#endif // _DEBUG

		}
		if (strcmp(cComp, SetItem) == 0)
		{
			sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &nNumber);
			cComp[0] = '\0';
		}
	}
	if (nNumber == -1)
	{
#ifdef _DEBUG
		MessageBox(NULL, "項目が見つかりません", "警告！", MB_ICONWARNING);
		fclose(pFile);
		return -1;
#endif // _DEBUG
	}
	// ファイルを閉じる
	fclose(pFile);

	return nNumber;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャの読み込み
//-------------------------------------------------------------------------------------------------------------
void CGameUI::LoadTextureFile(const char * sFileName, PFILETAG TexName, const int & nNumMax)
{
	// 変数宣言
	FILE *pFile = NULL;				// ファイルのポインタ
	char cRead[128];				// 読み込み用
	char cComp[128];				// 比較用
	char cEmpty[128];				// 要らないもの用
	int nNumCount = 0;				// 個数のカウント
	int nCntError = 0;				// エラー用

									// ファイルが開かれていなかったら
	if ((pFile = fopen(sFileName, "r")) == NULL)
	{// メッセージの表示
#ifdef _DEBUG
		MessageBox(NULL, "テキストファイルが見つかりませんでした", "警告！", MB_ICONWARNING);
		return;
#endif // _DEBUG
	}
	// スクリプトが存在しているかチェック
	if (!CGameUI::Existenceofscript(pFile))
	{
#ifdef _DEBUG
		MessageBox(NULL, "[SCRIP]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
		fclose(pFile);
		return;
#endif // _DEBUG
	}

	// END_SCRIPTが来るまで
	while (strcmp(cComp, "END_SCRIPT") != 0)
	{// END_SCRIPTまでループ
	 // 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > 1048576)
		{// エラー
			nCntError = 0;
#ifdef _DEBUG
			MessageBox(NULL, "[END_SCRIPT]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
			fclose(pFile);
			return;
#endif // _DEBUG

		}
		if (strcmp(cComp, "FILENAME") == 0)
		{
			if (nNumCount >= nNumMax)
			{
				MessageBox(NULL, "モデル数が合っていません", "警告！", MB_ICONWARNING);
				fclose(pFile);
				return;
			}
			sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &TexName[nNumCount].string);
			nNumCount++;
			cComp[0] = '\0';
		}
	}
	if (nNumCount < nNumMax)
	{
#ifdef _DEBUG
		MessageBox(NULL, "項目が見つかりません", "警告！", MB_ICONWARNING);
		fclose(pFile);
		return;
#endif // _DEBUG
	}
	// ファイルを閉じる
	fclose(pFile);

}

//-------------------------------------------------------------------------------------------------------------
// SCRIPTが存在するか
//-------------------------------------------------------------------------------------------------------------
bool CGameUI::Existenceofscript(FILE * pFile)
{
	// 変数宣言
	char cRead[128];	// 読み込み用
	char cComp[128];	// 比較用
	int nCntError = 0;	// エラー用
	while (strcmp(cComp, "SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > 1048576)
		{// エラー
			nCntError = 0;
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------------------------
// UI情報の読み込み
//-------------------------------------------------------------------------------------------------------------
void CGameUI::LoadUI(void)
{
	// 変数宣言
	FILE	*pFile = NULL;				// ファイルのポインタ
	char	cRead[128];					// 読み込み用
	char	cComp[128];					// 比較用
	char	cEmpty[128];				// 要らないもの用
	int		nCntError = 0;				// エラーメッセージ用


	// UI数を取得
	m_nNumUI = CGameUI::LoadNumber("data/TEXT/UI_Info.txt", "NUMBER");

	if (m_nNumUI == -1)
	{
		return;
	}

	m_pCScene2D = new CScene_TWO[m_nNumUI];

	// ファイルを開く
	if ((pFile = fopen("data/TEXT/UI_Info.txt", "r")) == NULL)
	{
		return;
	}
	// スクリプトの有無の確認
	if (!CGameUI::Existenceofscript(pFile))
	{
		return;
	}

	// 変数宣言
	int nCntScene2D = 0;		// シーン2Dのカウント

	// END_SCRIPTが来るまで
	while (strcmp(cComp, "END_SCRIPT") != 0)
	{// END_SCRIPTまでループ
	 // 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > 1048576)
		{// エラー
			nCntError = 0;
#ifdef _DEBUG
			MessageBox(NULL, "[END_SCRIPT]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
			fclose(pFile);
			return;
#endif // _DEBUG

		}
		if (strcmp(cComp, "SET_UI") == 0)
		{
			// 生成
			m_pCScene2D[nCntScene2D].Init();

			while (strcmp(cComp, "}") != 0)
			{// END_SCRIPTまでループ
			 // 1行読み込む
				fgets(cRead, sizeof(cRead), pFile);
				// 読み込んど文字列代入
				sscanf(cRead, "%s", &cComp);

				if (strcmp(cComp, "POS") == 0)
				{
					D3DXVECTOR3 pos;
					sscanf(cRead, "%s %s %f %f %f",
						&cEmpty,
						&cEmpty,
						&pos.x,
						&pos.y,
						&pos.z);
					cComp[0] = '\0';
					m_pCScene2D[nCntScene2D].SetPosition(pos);

				}
				else if (strcmp(cComp, "SIZE") == 0)
				{
					D3DXVECTOR2 size;
					sscanf(cRead, "%s %s %f %f",
						&cEmpty,
						&cEmpty,
						&size.x,
						&size.y);
					cComp[0] = '\0';
					m_pCScene2D[nCntScene2D].SetSize(size*2.0f);
				}

				else if (strcmp(cComp, "ROTATION") == 0)
				{
					float fRot;
					sscanf(cRead, "%s %s %f",
						&cEmpty,
						&cEmpty,
						&fRot);
					cComp[0] = '\0';
					m_pCScene2D[nCntScene2D].SetRot(fRot);

				}
				else if (strcmp(cComp, "COL") == 0)
				{
					D3DXCOLOR col;
					sscanf(cRead, "%s %s %f %f %f %f",
						&cEmpty,
						&cEmpty,
						&col.r,
						&col.g,
						&col.b,
						&col.a);
					cComp[0] = '\0';
					m_pCScene2D[nCntScene2D].SetCol(col);

				}
				else if (strcmp(cComp, "TEXTURETYPE") == 0)
				{
					int nType;
					sscanf(cRead, "%s %s %d",
						&cEmpty,
						&cEmpty,
						&nType);
					cComp[0] = '\0';
					m_pCScene2D[nCntScene2D].BindTexture(m_pTextureinfo[nType].pTexture);

				}
			}
			// シーン2Dのカウントを進める
			nCntScene2D++;
		}
	}

}

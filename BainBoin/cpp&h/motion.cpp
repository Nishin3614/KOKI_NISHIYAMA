//=============================================================================
//
// モーション処理 [player.cpp]
// Author : KOKI NISHIYAMA
//
//=============================================================================
#include "motion.h"

//=============================================================================
// マクロ定義
//=============================================================================

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CMotion::CMotion()
{
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CMotion::~CMotion()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CMotion::Init(void)
{
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CMotion::Uninit(void)
{
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CMotion::Update(void)
{
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CMotion::Draw(void)
{
}

// ----------------------------------------
// 生成処理
// ----------------------------------------
CMotion * CMotion::Create()
{
	return NULL;
}

// ----------------------------------------
// 読み込み設定
// ----------------------------------------
void CMotion::Load(
	MOTION * pMotion,
	MODEL_OFFSET * pOffset,
	int nMaxkey,
	const char * file_name
)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntMotion = 0;		// モーションカウント
	int	nCntKeySet = 0;		// フレーム数
	int	nCntKey = 0;		// モデル数
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	pFile = fopen(file_name, "r");

	// 開けた
	if (pFile != NULL)
	{
		// スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			// 初期化
			cHeadText[0] = '\0';
			fgets(cRaedText, sizeof(cRaedText), pFile);	// 一文を読み込む
			sscanf(cRaedText, "%s", &cHeadText);		// 比較用テキストに文字を代入
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

				// モーションセットが来たら
				if (strcmp(cHeadText, "MOTIONSET") == 0)
				{
					// モデルの初期化
					nCntKeySet = 0;
					// エンドモーションセットが来るまでループ
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						// 初期化
						cHeadText[0] = '\0';
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ループするかどうか情報読み込み
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&pMotion[nCntMotion].nLoop);
						}
						// キー数情報読み込み
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &pMotion[nCntMotion].nNumKey);
							// モーション数生成
							pMotion[nCntMotion].KeyInfo = new KEY_INFO[pMotion[nCntMotion].nNumKey];
						}

						// 当たり判定情報読み込み
						else if (strcmp(cHeadText, "COLLISION") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %f %d %d",
								&cDie, &cDie,
								&pMotion[nCntMotion].Collision_nParts,
								&pMotion[nCntMotion].Collision_Ofset.x,
								&pMotion[nCntMotion].Collision_Ofset.y,
								&pMotion[nCntMotion].Collision_Ofset.z,
								&pMotion[nCntMotion].Collision_Radius,
								&pMotion[nCntMotion].Collision_StartFram,
								&pMotion[nCntMotion].Collision_EndFram);
						}

						// ダメージ情報読み込み
						else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&pMotion[nCntMotion].Collision_Damage);
						}

						// 軌跡情報読み込み
						else if (strcmp(cHeadText, "ORBIT") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %d %d %d",
								&cDie, &cDie,
								&pMotion[nCntMotion].Orbit_nParts,
								&pMotion[nCntMotion].Orbit_Ofset.x,
								&pMotion[nCntMotion].Orbit_Ofset.y,
								&pMotion[nCntMotion].Orbit_Ofset.z,
								&pMotion[nCntMotion].Orbit_Mesh,
								&pMotion[nCntMotion].Orbit_StartFram,
								&pMotion[nCntMotion].Orbit_EndFram);

							// キャラクタータイプ
							//pMotion[nCntMotion].Orbit_nType = TRAJECTORYTYPE_PLAYER;

							// 使用状態
							pMotion[nCntMotion].Orbit_bUse = true;
						}

						// キー情報読み込み
						else if (strcmp(cHeadText, "KEYSET") == 0)
						{
							// 初期化
							cHeadText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// フレーム情報読み込み
							if (strcmp(cHeadText, "FRAME") == 0)
							{
								// フレーム数
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pMotion[nCntMotion].KeyInfo[nCntKeySet].nFrame);
							}

							// キーの初期化
							nCntKey = 0;
							// モデル数生成
							pMotion[nCntMotion].KeyInfo[nCntKeySet].Key = new KEY[nMaxkey];
							// エンドキーセットが来るまでループ
							while (strcmp(cHeadText, "END_KEYSET") != 0)
							{
								// 初期化
								cHeadText[0] = '\0';
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);

								// キー情報読み込み
								if (strcmp(cHeadText, "KEY") == 0)
								{
									// エンドキーが来るまでループ
									while (strcmp(cHeadText, "END_KEY") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);

										// 位置情報読み込み
										if (strcmp(cHeadText, "POS") == 0)
										{
											// フレーム数
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].pos.x,
												&pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].pos.y,
												&pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].pos.z);

											// パーツの位置情報を加える
											pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].pos += pOffset[nCntKey].pos;
										}

										// 回転情報読み込み
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// フレーム数
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].rot.x,
												&pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].rot.y,
												&pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].rot.z);
											// パーツの位置情報を加える
											pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].rot += pOffset[nCntKey].rot;
										}
									}
									// カウントキー
									nCntKey++;
								}
							}
							// カウントモデル
							nCntKeySet++;
						}
					}
					// カウントプレイヤー
					nCntMotion++;
				}
			}
		}

		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "プレイヤーデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ----------------------------------------
// 情報の破棄設定
// ----------------------------------------
void CMotion::UnLoad(
	MOTION * pMotion,
	int nMaxMotion
)
{
	// 変数宣言
	int	nCntMotion = 0;		// モーションカウント
	int	nCntKeySet = 0;		// フレーム数6

	for (nCntMotion = 0; nCntMotion < nMaxMotion; nCntMotion++)
	{
		// キーの破棄
		for (nCntKeySet = 0; nCntKeySet < pMotion[nCntMotion].nNumKey; nCntKeySet++)
		{
			delete[] pMotion[nCntMotion].KeyInfo[nCntKeySet].Key;
			pMotion[nCntMotion].KeyInfo[nCntKeySet].Key = NULL;
		}
		// キー情報の破棄
		delete[] pMotion[nCntMotion].KeyInfo;
		pMotion[nCntMotion].KeyInfo = NULL;
	}
	delete[] pMotion;
	pMotion = NULL;
}
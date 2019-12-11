// ------------------------------------------------------------------------------------------
// 読み込み・書き出し処理 [Load_Save.cpp]
// Author : Koki Nishiyama
// ------------------------------------------------------------------------------------------
#include "Load_Save.h"			// 読み込み・書き出しヘッダー

#include "input.h"

#include "joypad.h"

#include "fade.h"

#include "sound.h"

#include "score.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define	TEXTURE_LOAD_SAVE	"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define LOAD_SAVE_SIZE (30)									// 読み込み・書き出しボードの大きさ
#define LOAD_SAVE_BETWEEN (30)								// 間隔
#define POINT_MAX (3)										// 数値
#define LOAD_SAVE_DATE (5)									// 読み込み・書き出し数
#define SAVE_FILE_NAME ("text.txt")							// セーブファイル

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// グローバル変数宣言
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureLoad_Save = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLoad_Save = NULL;		// 頂点バッファへのポインタ
D3DXVECTOR3				g_Load_SavePos;					// 読み込み・書き出しボードのポジション
LOAD_SAVE				g_aLoad_Save[LOAD_SAVE_DATE];	// 読み込み・書き出し変数
int g_nLoadScore = 0;									// ロードスコア

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitLoad_Save(void)
{
	// 変数宣言
	int nCntRank;										// カウント読み込み・書き出し

	// 初期化
	g_nLoadScore = 0;
	for (nCntRank = 0; nCntRank < LOAD_SAVE_DATE; nCntRank++)
	{
		g_aLoad_Save[nCntRank].nPoint = 0;
	}
	
	// ロード読み込み・書き出し
	LoadDate();
}

void SaveData(int nPoint)
{
	// ファイルポイント
	FILE *pFile;
	int aDate[6] = {};
	int nDate;
	int nCntLoad_Save;
	int nCntNum;
	int  nCntLoad = 0;
	char cWriteText[128];			// 文字として書き込み用
	char cHeadText[128];			// 代入用
	char cEqual[8] = { "=" };		// イコール用
	char cEnter[8] = { "\n" };	// 改行

								// 入れ替える前の読み込み・書き出しポイントを代入
	for (nCntLoad_Save = 0; nCntLoad_Save < LOAD_SAVE_DATE; nCntLoad_Save++)
	{
		aDate[nCntLoad_Save] = g_aLoad_Save[nCntLoad_Save].nPoint;
	}

	// 今回とったポイント数
	aDate[5] = nPoint;

	// 左辺の読み込み・書き出しループ
	for (nCntLoad_Save = 0; nCntLoad_Save < LOAD_SAVE_DATE + 1; nCntLoad_Save++)
	{
		// 右辺の読み込み・書き出しループ
		for (nCntNum = nCntLoad_Save + 1; nCntNum < LOAD_SAVE_DATE + 1; nCntNum++)
		{
			// 読み込み・書き出しポイントを比較
			if (aDate[nCntLoad_Save] < aDate[nCntNum])
			{
				nDate = aDate[nCntNum];
				aDate[nCntNum] = aDate[nCntLoad_Save];
				aDate[nCntLoad_Save] = nDate;
			}
		}
	}

	// 入れ替えた後の読み込み・書き出しポイントを代入
	for (nCntLoad_Save = 0; nCntLoad_Save < LOAD_SAVE_DATE; nCntLoad_Save++)
	{
		g_aLoad_Save[nCntLoad_Save].nPoint = aDate[nCntLoad_Save];
	}

	// カウントセーブの初期化
	nCntLoad_Save = 0;

	// ファイル開
	pFile = fopen("data/SAVE/ranking.txt", "w");

	// 開けた
	if (pFile != NULL)
	{
		strcpy(cWriteText, "SCRIPT");
		fputs(cWriteText, pFile);

		// スクリプトが通った
		if (strcmp(cWriteText, "SCRIPT") == 0)
		{
			fputs(cEnter, pFile);

			// エンドスクリプトが来るまでループ
			while (strcmp(cWriteText, "END_SCRIPT") != 0)
			{

				strcpy(cHeadText, "rank");
				if (strcmp(cHeadText, "rank") == 0)
				{
					sprintf(cWriteText, "	%s %s %d", &cHeadText, &cEqual, g_aLoad_Save[nCntLoad_Save].nPoint);
					fputs(cWriteText, pFile);

					fputs(cEnter, pFile);

					nCntLoad_Save++;
				}

				if (nCntLoad_Save >= 5)
				{
					strcpy(cHeadText, "now");
					if (strcmp(cHeadText, "now") == 0)
					{

						sprintf(cWriteText, "	%s %s %d", &cHeadText, &cEqual, aDate[5]);
						fputs(cWriteText, pFile);

						fputs(cEnter, pFile);

					}

					strcpy(cWriteText, "END_SCRIPT");
					fputs(cWriteText, pFile);
				}

			}
		}
		// ファイル閉
		fclose(pFile);
	}
	// 開けない
	else
	{
		printf("開けれませんでした\n");
	}
}

void LoadDate(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad_Save = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較用
	char cDie[128];			// ゴミ用

							// ファイル開
	pFile = fopen("data/SAVE/ranking.txt", "r");

	// 開けた
	if (pFile != NULL)
	{
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);
			sscanf(cRaedText, "%s", &cHeadText);
		}

		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);
				if (strcmp(cHeadText, "rank") == 0)
				{
					sscanf(cRaedText, "%s %s %d",
						&cDie, &cDie,
						&g_aLoad_Save[nCntLoad_Save].nPoint);
					nCntLoad_Save++;
				}

				else if (strcmp(cHeadText, "now") == 0)
				{
					sscanf(cRaedText, "%s %s %d",
						&cDie, &cDie,
						&g_nLoadScore);
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		printf("開けれませんでした\n");
	}
}

LOAD_SAVE * GetLoad_Save()
{
	return &g_aLoad_Save[0];
}
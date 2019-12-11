// ----------------------------------------
//
// サウンド処理の説明[sound.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "main.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define MAX_SOUND (10)

// ------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------
class CSound
{
public:
	/* 列挙型 */
	// ラベル
	typedef enum
	{
		LABEL_BGM_TITLE = 0,	// タイトル
		LABEL_BGM_GAME,			// ゲーム
		LABEL_BGM_RANKINGMOVE,	// ランキング
		LABEL_SE_ENTER,			// 決定音
		LABEL_SE_FAIL,			// 失敗
		LABEL_SE_DOWN,			// 天井が下に下がるとき
		LABEL_SE_THROW,			// 投げる時
		LABEL_SE_DELETE,		// 消える時
		LABEL_SE_CLEAR,			// クリア時
		LABEL_MAX,
	} LABEL;
	/* 関数 */
	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(LABEL label);
	void StopSound(LABEL label);
	void StopSound(void);
protected:

private:
	/* 関数 */
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	/* 変数 */
	static char m_pFilename[LABEL_MAX][36];					// ファイル名
	static int m_nCntLoop[LABEL_MAX];						// ループカウント
	static IXAudio2 *m_pXAudio2;							// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice *m_pMasteringVoice;		// マスターボイス
	static IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
	static BYTE *m_apDataAudio[LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[LABEL_MAX];					// オーディオデータサイズ
};

#endif
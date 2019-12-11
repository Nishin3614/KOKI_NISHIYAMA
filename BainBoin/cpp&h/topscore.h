// ----------------------------------------
//
// 上位上位スコア処理の説明[topscore.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _TOPSCORE_H_
#define _TOPSCORE_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define MAX_NUMBERSCORE (7)
#define MAX_TOPSCORE (5)

class CNumber;
class CMark;
// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CTopscore : public CScene
{
public:
	/* 関数 */
	CTopscore();
	~CTopscore();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTopscore(int nNumber);
	static void Load(void);
	static void Save(void);
	static void SetScore(const int nScore);			// スコア設定
	static CTopscore * Create(D3DXVECTOR3 pos);	// 作成

protected:

private:
	/* 関数 */
	/* 変数 */
	static int m_nTopScore[MAX_TOPSCORE];	// 上位スコア
	static int m_nMyScore;					// マイスコア
	static int m_nInRank;					// ランクイン
	CNumber *m_pNumber[MAX_NUMBERSCORE];	// 上位スコア表示
	D3DXVECTOR3 m_pos;						// 位置情報
	int m_nScore;						// 得点
};

#endif
// ----------------------------------------
//
// 配列処理の説明[array.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _ARRAY_H_
#define _ARRAY_H_	 // ファイル名を基準を決める

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
#define MAX_ARRAY_LINE (10)		// 行の最大数
#define MAX_ARRAY_COLUMN (10)	// 列の最大数

// ------------------------------------------------------------------------------------------
//
// 前方宣言
//
// ------------------------------------------------------------------------------------------
class CLine;	// 線

// ------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------
class CArray : public CScene
{
public:
	/* 関数 */
	CArray();
	~CArray();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPos(int nCntLine, int nCntColumn);
	// 周りの行列情報を渡すかも
	D3DXVECTOR3 TopPosCorrection(
		D3DXVECTOR3 &pos,	// 位置アドレス
		int nLine, 			// 行
		int *nIdColumn		// 列ポインタ
	);						
	// 周りの行列情報を渡すかも
	D3DXVECTOR3 AroundPosCorrection(
		D3DXVECTOR3 &pos,	// 位置アドレス
		int nLine,			// 行
		int nColumn,		// 列
		int *nIdLine,		// 行情報ポインタ
		int *nIdColumn		// 列情報ポインタ
	);	
	// 選択した行列情報を一つ下に下げる
	D3DXVECTOR3 TargetPosCorrection(
		int nLine,	// 行
		int nColumn	// 列
	);												
	void OriginPosChange(void);																				// 初期値を変化
	void BiginPos(void);																					// 生成されたとの初期値
	void DeleteState(int nCntLine, int nCntColumn);															// 配置状態をfalseへ
	void SetState(int nCntline, int nCntColumn,bool bState);												// 配列状態設定
	static CArray * Create(CManager::PLAYER playerId, D3DXVECTOR3 pos, D3DXVECTOR2 size);					// 作成
	static int GetDownCnt(void);																			// カウントダウンを取得
protected:

private:
	/* 変数 */
	D3DXVECTOR3 m_BiginPos = {};										// 生成されたときの初期位置
	D3DXVECTOR3 m_OriginPos = {};										// 初期位置
	D3DXVECTOR3 m_VariousPos[MAX_ARRAY_LINE][MAX_ARRAY_COLUMN] = {};	// 様々な位置
	CManager::PLAYER m_PlayerId;										// プレイヤー番号
	bool m_bState[MAX_ARRAY_LINE][MAX_ARRAY_COLUMN] = {};				// 配置されているかされていないか
	CLine * m_pline;													// 線
	static int m_nDownCnt;												// 落ちるカウント
};

#endif
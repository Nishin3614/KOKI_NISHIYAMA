//------------------------------------------------------
//
//	記号定数と疑似命令
//
//--------------------------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_
//------------------------------------------------------
//
//	インクルードファイル
//
//--------------------------------------------------------
#include "scene.h"
#include "renderer.h"
#include "number.h"
//------------------------------------------------------
//
//	マクロ定義
//
//--------------------------------------------------------
#define SCORE_DIGIT (8)			//桁数
#define SCOREX	(20)			//幅
#define SCOREY	(40)			//高さ
#define SCORE_POSX	(900.0f)	//X座標
#define SCORE_POSY	(50.0f)		//Y座標
#define SCORE_SPACE	(40.0f)		//スコアの間隔
#define SCORE_UP	(500)		// スコアの上昇

//------------------------------------------------------
//
//	前方宣言
//
//--------------------------------------------------------
class CScene;
//===========================================================
//
//	クラス型の定義
//
//===========================================================
class CScore :public CScene
{
public:
	//関数宣言
	CScore();//コンストラクタ (インスタンスが生成されると自動的に呼び出される)
	~CScore();//デストラクタ (インスタンスが破棄されると自動的に呼び出される)

	void Init(void);//初期化
	void Uninit(void);//終了
	void Update(void);//更新
	void Draw(void);//描画

	static CScore * Create(void);//生成
	void AddScore(int nValue);//スコアの追加
	void SetPosition(D3DXVECTOR3 pos);//座標の設定
	int GetScore(void);					// スコアの取得
private:

	//静的変数宣言

	//変数宣言
	CNumber *m_apNumber[SCORE_DIGIT];//桁数分の配列
	D3DXVECTOR3 m_pos;//座標
	int m_nScore;
};
#endif


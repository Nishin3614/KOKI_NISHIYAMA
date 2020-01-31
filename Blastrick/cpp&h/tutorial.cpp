// ------------------------------------------
//
// チュートリアル処理 [tutorial.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "tutorial.h"
#include "tutorial_ui.h"

/* 描画 */
#include "fade.h"

// ------------------------------------------
//
// マクロ定義
//
// ------------------------------------------

// ------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------

// ------------------------------------------
// コンストラクタ
// ------------------------------------------
CTutorial::CTutorial()
{
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CTutorial::~CTutorial()
{
}

// ------------------------------------------
// 初期化
// ------------------------------------------
void CTutorial::Init(void)
{
	CTutorial_ui::Create();
}

// ------------------------------------------
// 終了
// ------------------------------------------
void CTutorial::Uninit(void)
{
	CScene::ReleaseAll();
}

// ------------------------------------------
// 更新
// ------------------------------------------
void CTutorial::Update(void)
{
}

// ------------------------------------------
// 描画
// ------------------------------------------
void CTutorial::Draw(void)
{
}

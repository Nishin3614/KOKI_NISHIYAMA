// ------------------------------------------------------------------------------------------
//
// 選択画面処理 [selection.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "selection.h"
/* 描画 */
#include "tlybg.h"
#include "selection_ui.h"
#include "fade.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// コンストラクタ
// ------------------------------------------------------------------------------------------
CSelection::CSelection()
{
}

// ------------------------------------------------------------------------------------------
// デストラクタ
// ------------------------------------------------------------------------------------------
CSelection::~CSelection()
{
}

// ------------------------------------------------------------------------------------------
// 初期化
// ------------------------------------------------------------------------------------------
void CSelection::Init(void)
{
	/* 作成 */
	// 背景
	CTlyBg::Create(CManager::MODE_SELECTION);
	// 選択UI
	CSelection_ui::Create();
}

// ------------------------------------------------------------------------------------------
// 終了
// ------------------------------------------------------------------------------------------
void CSelection::Uninit(void)
{
	CScene::ReleaseAll();
}

// ------------------------------------------------------------------------------------------
// 更新
// ------------------------------------------------------------------------------------------
void CSelection::Update(void)
{
}

// ------------------------------------------------------------------------------------------
// 描画
// ------------------------------------------------------------------------------------------
void CSelection::Draw(void)
{
}

#include "main.h"
#include "map_temp.h"

map_temp::map_temp() {
	// マップデータ初期化処理
	for (int i = 0; i <= DATA_MAX; i++)
	{
		MapDeta[i] = 0;
	}


	// マップ生成(0:戦闘、1:ランダムイベント、2:休憩、3:鍛冶屋、4:ボス)

	// ランダムイベント(st7固定)
	MapDeta[7] = 1;

	// 休憩1(st3-6)(1-2)
	RandNum[0] = GetRand(1) + 1;
	for (int i = 0; i < RandNum[0];) {
		int r = GetRand(3) + 3;
		if (MapDeta[r] == 0) {
			MapDeta[r] = 2;
			i++;
		}
		else continue;
	}
	// 休憩2(st8-18)(2-3)
	RandNum[1] = GetRand(1) + 2;
	for (int i = 0; i < RandNum[1];) {
		int r = GetRand(11) + 8;
		// 未変更(0なら)変更
		if (MapDeta[r] == 0) {
			MapDeta[r] = 2;
			i++;
		}
		else continue;
	}
	// 休憩3(st19固定)
	MapDeta[19] = 2;

	// 鍛冶屋(st14-18)(1)
	RandNum[2] = 1;
	for (int i = 0; i < RandNum[2];) {
		int r = GetRand(5) + 14;
		if (MapDeta[r] == 0) {
			MapDeta[r] = 3;
			i++;
		}
		else continue;
	}

	// ボス(st20固定)
	MapDeta[20] = 4;

	// アイコン位置をデフォルトにセット
	for (int i = 0; i < DATA_MAX; i++)
	{
		icon_loc[i][0] = icon_loc_def[i][0];
		icon_loc[i][1] = icon_loc_def[i][1];
	}

	// アイコン移動初期化処理
	icon_vec = 0;

	// 画像読込
	if (battle_img == 0) battle_img = (LoadGraph("resources/images/skeleton.png"));
	if (event_img == 0) event_img = (LoadGraph("resources/images/event.png"));
	if (rest_img == 0) rest_img = (LoadGraph("resources/images/rest.png"));
	if (anvil_img == 0) anvil_img = (LoadGraph("resources/images/anvil.png"));
	if (boss_img == 0) boss_img = (LoadGraph("resources/images/boss.png"));
	if (map_cursor == 0) map_cursor = (LoadGraph("resources/images/map_cursor.png"));
}
map_temp::~map_temp() {
	DeleteGraph(battle_img);
	DeleteGraph(event_img);
	DeleteGraph(rest_img);
	DeleteGraph(anvil_img);
	DeleteGraph(boss_img);
	DeleteGraph(map_cursor);
}

Scene* map_temp::update() {

	// アイコン移動距離リセット
	icon_vec = 0;

	// スクロール
	if (InputCtrl::GetStickRatio(L).y >= 0.2 || InputCtrl::GetStickRatio(L).y <= -0.2)
	{
		// 上スクロール
		if (icon_loc[20][1] < 50) {
			if (InputCtrl::GetStickRatio(L).y >= 0.2 && InputCtrl::GetStickRatio(L).y < 0.5) {
				icon_vec = 1;
			}
			else if (InputCtrl::GetStickRatio(L).y >= 0.5 && InputCtrl::GetStickRatio(L).y < 0.8) {
				icon_vec = 3;
			}
			else if (InputCtrl::GetStickRatio(L).y >= 0.8) {
				icon_vec = 5;
			}
		}
		// 下スクロール
		if (icon_loc[0][1] > SCREEN_HEIGHT - 100) {
			if (InputCtrl::GetStickRatio(L).y <= -0.2 && InputCtrl::GetStickRatio(L).y > -0.5) {
				icon_vec = -1;
			}
			else if (InputCtrl::GetStickRatio(L).y <= -0.5 && InputCtrl::GetStickRatio(L).y > -0.8) {
				icon_vec = -3;
			}
			else if (InputCtrl::GetStickRatio(L).y <= -0.8) {
				icon_vec = -5;
			}
		}
	}

	// アイコン移動処理
	for (int i = 0; i < DATA_MAX; i++)
	{
		icon_loc[i][1] = icon_loc[i][1] + icon_vec;
	}

	return this;
};

void map_temp::draw() const {

	for (int i = 0; i < DATA_MAX; i++)
	{
		// デバック表示
		DrawFormatString(10, 30, 0xffff00, "内部データ");
		DrawFormatString(10 * i + 10, 50, 0xffffff, "%d", MapDeta[i]);

		// ステージ間のライン
		for (int j = 0; next_stage[i][j] > 0 && j <= 2; j++)
		{
			int next_loc = next_stage[i][j];

			DrawLine(icon_loc[i][0] + 25, icon_loc[i][1] + 25,
				icon_loc[next_loc][0] + 25, icon_loc[next_loc][1] + 25, 0xffffff);
		}

		// アイコン表示
		switch (MapDeta[i]) {
		case 0:
			DrawGraph(icon_loc[i][0], icon_loc[i][1], battle_img, TRUE);
			break;
		case 1:
			DrawGraph(icon_loc[i][0], icon_loc[i][1], event_img, TRUE);
			break;
		case 2:
			DrawGraph(icon_loc[i][0], icon_loc[i][1], rest_img, TRUE);
			break;
		case 3:
			DrawGraph(icon_loc[i][0], icon_loc[i][1], anvil_img, TRUE);
			break;
		case 4:
			DrawGraph(icon_loc[i][0], icon_loc[i][1], boss_img, TRUE);
			break;
		default:
			break;
		}
		DrawFormatString(icon_loc[i][0], icon_loc[i][1], 0x00ff00, "%d", i);
	}

}

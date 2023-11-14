#pragma once

class Player;
class GameUI;

class Rest
{
private:
	GameUI* ui;

	int interval;
	int cursor_num;


	int cursor_image;
	int bonfire_image;
	int button_image;

	bool is_select;

public:
	//コンストラクタ
	Rest(GameUI* ui);
	//デストラクタ
	~Rest();

	//描画に関すること以外の更新を実装する
	void update(Player* player, bool& flg);

	//描画に関することの更新を実装する
	void draw()const;

};
//#include "rest.h"
#include "main.h"

//#include"Player.h"
//#include"game_ui.h"

Rest::Rest(GameUI* ui)
{
	this->ui = ui;

	cursor_interval = 0;
	interval = 0;
	cursor_num = 0;


	cursor_image = LoadGraph("resources/images/武器/片手剣.png");
	bonfire_image = LoadGraph("resources/images/bonfirelit.png");
	button_image = LoadGraph("resources/images/button_a.png");

	logo_dicision_image = LoadGraph("resources/images/Logo/UI/logo_dicision.png");
	logo_rest_image = LoadGraph("resources/images/Logo/UI/logo_rest.png");
	logo_pray_image = LoadGraph("resources/images/Logo/UI/logo_pray.png");

	SoundManager::SetBGM("bgm_breakstage");
	SoundManager::SetBGM("bgm_breaktime");
	SoundManager::SetVolumeBGM("bgm_breaktime", 50);

	SoundManager::SetSE("se_system_normal_decision");		//タイトル以外のカーソル決定音
	SoundManager::SetSE("se_system_select_syu");			//カーソル移動音
	SoundManager::SetSE("se_system_healing");				//回復の音
	SoundManager::SetSE("se_system_blessing");				//祝福の音

	is_select = false;
	is_ok = false;
	rest_buf_flg = false;
}

Rest::~Rest()
{
	DeleteGraph(cursor_image);
	DeleteGraph(bonfire_image);
	DeleteGraph(button_image);
	DeleteGraph(logo_dicision_image);
	DeleteGraph(logo_rest_image);
	DeleteGraph(logo_pray_image);
}

void Rest::update(Player* player, int& mode, int& stage)
{
	SoundManager::PlaySoundBGM("bgm_breakstage");
	SoundManager::PlaySoundBGM("bgm_breaktime");

	ui->setHP(player->GetPlayer_HP(), 100, (int)(player->GetPlayer_HP()));
	ui->update();

	cursor_x = cursor_num * 445;

	if (cursor_interval < 15)
	{
		cursor_interval++;
	}

	if (is_select)
	{
		if (interval < 120)
		{
			interval++;
		}
		else
		{
			is_ok = true;
			stage++;
			mode = GameSceneMode::map;
			Init();
			SoundManager::StopSoundBGMs();
			SoundManager::SetSoundBGMsPosition(0);
		}
	}
	else
	{
		//左スティックを右に
		if ((InputCtrl::GetStickRatio(L).x > 0.8 || InputCtrl::GetButtonState(XINPUT_BUTTON_DPAD_RIGHT)) && cursor_interval >= 15)
		{
			//カーソルの移動音
			SoundManager::PlaySoundSE("se_system_select_syu", false);
			cursor_num++;
			cursor_interval = 0;
			if (cursor_num > 1)
			{
				cursor_num = 0;
			}
		}

		//左スティックを左に
		if ((InputCtrl::GetStickRatio(L).x < -0.8 || InputCtrl::GetButtonState(XINPUT_BUTTON_DPAD_LEFT)) && cursor_interval >= 15)
		{
			//カーソルの移動音
			SoundManager::PlaySoundSE("se_system_select_syu", false);
			cursor_num--;
			cursor_interval = 0;
			if (cursor_num < 0)
			{
				cursor_num = 1;
			}
		}

		if (InputCtrl::GetButtonState(XINPUT_BUTTON_A) == PRESS || InputCtrl::GetKeyState(KEY_INPUT_SPACE) == PRESS)
		{
			if (cursor_num == 0)
			{
				//回復の音
				SoundManager::PlaySoundSE("se_system_healing");
				player->SetPlayer_HP(-100);
				is_select = true;
			}
			else if (cursor_num == 1)
			{
				//祝福の音
				SoundManager::PlaySoundSE("se_system_blessing");
				rest_buf_flg = true;
				is_select = true;
			}
		}

	}
}

void Rest::draw() const
{
	ui->drawHP();

	DrawRotaGraph(640, 300, .7f, 0, bonfire_image, TRUE);
	DrawRotaGraph(280 + cursor_x, 610, .15, 1, cursor_image, TRUE);
	DrawGraph(1150, 650, button_image, TRUE);
	DrawRotaGraph(1210, 667, 0.1, 0, logo_dicision_image, TRUE);

	if (is_select != true)
	{
		SetFontSize(48);
		//DrawString(370, 600, "休憩する", 0xffffff);
		DrawRotaGraph(470, 600, 0.2, 0, logo_rest_image, TRUE);
		DrawRotaGraph(870, 600, 0.2, 0, logo_pray_image, TRUE);
		//DrawString(770, 600, "祈る", 0xffffff);
		SetFontSize(16);
	}
	else
	{
		if (cursor_num == 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 230);
			DrawBox(0, (SCREEN_HEIGHT / 3), SCREEN_WIDTH, (SCREEN_HEIGHT / 3) * 2, 0x000000, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			SetFontSize(48);
			DrawString((SCREEN_WIDTH / 2) - 48 * 4.5, (SCREEN_HEIGHT / 2) - 24, "HPが全回復しました", 0xffffff);
			SetFontSize(16);
		}
		else if (cursor_num == 1)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 230);
			DrawBox(0, (SCREEN_HEIGHT / 3), SCREEN_WIDTH, (SCREEN_HEIGHT / 3) * 2, 0x000000, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			SetFontSize(48);
			DrawString((SCREEN_WIDTH / 2) - 48 * 4.5, (SCREEN_HEIGHT / 2) - 24, "バフを獲得しました", 0xffffff);
			SetFontSize(16);
		}
	}
}

void Rest::Init()
{
	interval = 0;

	cursor_num = 0;

	is_select = false;
	is_ok = false;

}

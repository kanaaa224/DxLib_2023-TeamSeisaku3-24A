﻿#include "Devil_king.h"
#include "Common.h"
#include <math.h>
#include "inputCtrl.h"
#define BTN_DEBUG
#define DEBUG

Devil_king::Devil_king()
{
	img = LoadGraph("resources/images/enemy_tmp_images/ma.png");
	hp = DEVILKING_MAX_HP;
	damage = DEVILKING_ATTAK_DAMAGE;
	location.x = _SCREEN_WIDHT_ / 2;
	location.y = 60;
	//シールド
	shield = MAX_SHIELD;
	shieldFlg = false;

	//-----大きい弾-----//
}

Devil_king::~Devil_king()
{
	DeleteGraph(img);
}

void Devil_king::Update(Player* player)
{
	//プレイヤーの移動量をdiffにセット
	SetPlayerAmountOfTravel_X(player->Player_MoveX());
	SetPlayerAmountOfTravel_Y(player->Player_MoveY());
	//プレイヤーの座標をdiffLocationにセット
	SetPlayer_Location(player->GetLocation());

	//移動処理
	location.x = location.x - diff.x;
	location.y = location.y - diff.y;

	//大きい弾の生成
	if (shieldFlg == false) {
		if (bigBulletCreateFlg == false) {
			bigBulletCreateCounter++;
		}

		if (bigBulletCreateCounter >= BIG_BULLET_CREATE_TIME) {
			bigBulletCreateFlg = true;
			bigBulletCreateCounter = 0;
		}

	}
	
	//シールド
	if (bigBulletHitFlg == true) {
		shield -= 10;
		bigBulletHitFlg = false;
	}

	if (shield <= 0) {
		shieldFlg = true;
	}

	//ダウンタイム
	if (shieldFlg == true) {
		//downTimeCounterが設定した値になったらシールドを復活させる
		if (downTimeCounter >= DOWN_TIME) {
			shield = MAX_SHIELD;
			shieldFlg = false;
			downTimeCounter = 0;
		}

		downTimeCounter++;
	}

	//武器からの攻撃とHPが０以上なら赤く表示する
	if (hitWeaponFlg == true && hp > 0) {
		redDrawFlg = true;
	}
	hitWeaponFlg = false;

	//リスポーンFlg
	if (hp <= 0) {
		respawnFlg = false;
	}

	//赤く表示
	if (redFrameCounter == RED_FRAME) {
		redDrawFlg = false;
		redFrameCounter = 0;
	}
	if (redDrawFlg == true) {
		redFrameCounter++;
	}

#ifdef BTN_DEBUG
	if (InputCtrl::GetKeyState(KEY_INPUT_D) == PRESS && hp >= 0) {
		hitWeaponFlg = true;
		hp -= 100;
	}
	else {
		hitWeaponFlg = false;
	}
#endif // BTN_DEBUG
}

void Devil_king::Draw() const
{
	DrawRotaGraph((int)location.x, (int)location.y, 1, 0, img, TRUE);//通常時
	if (redDrawFlg == true) {
		SetDrawBright(255, 0, 0);
		DrawRotaGraph((int)location.x, (int)location.y, 1, 0, img, TRUE);//赤色表示
		SetDrawBright(255, 255, 255);
	}

#ifdef DEBUG
	DrawFormatString(location.x, location.y, C_BLUE,      "シールド:%0.2f", shield);
	DrawFormatString(location.x, location.y + 10, C_BLUE, "   Flg  :%d", shieldFlg);
#endif // DEBUG

}

void Devil_king::BeamUpdate()
{

}

void Devil_king::BeamDraw() const
{

}

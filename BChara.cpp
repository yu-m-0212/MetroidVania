//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "BChara.h"
#include  "MyPG.h"
#include  "Task_Map2D.h"

//-----------------------------------------------------------------------------
//モーションを更新（変更なしの場合	false)
bool  BChara::UpdateMotion(State  nm_)
{
	if (nm_ == this->state) {
		return  false;
	}
	else {
		this->state = nm_;		//モーション変更
		this->moveCnt = 0;		//行動カウンタクリア
		this->animCnt = 0;		//アニメーションカウンタのクリア
		return  true;
	}
}

//-----------------------------------------------------------------------------
//頭上接触判定
bool  BChara::CheckHead()
{
	//あたり判定を基にして頭上矩形を生成
	ML::Box2D  head(this->hitBase.x,
		this->hitBase.y - 1,
		this->hitBase.w,
		1);
	head.Offset((int)this->pos.x, (int)this->pos.y);


	auto   map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
	if (nullptr == map) { return false; }//マップが無ければ判定しない(出来ない）
	return map->CheckHit(head);
}
//-----------------------------------------------------------------------------
//めり込まない移動処理
void BChara::CheckMove(ML::Vec2&  e_)
{
	//マップが存在するか調べてからアクセス
	auto   map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
	if (nullptr == map) { return; }//マップが無ければ判定しない(出来ない）

										  //横軸に対する移動
	while (e_.x != 0) {
		float  preX = this->pos.x;
		if (e_.x >= 1) { this->pos.x += 1;		e_.x -= 1; }
		else if (e_.x <= -1) { this->pos.x -= 1;		e_.x += 1; }
		else { this->pos.x += e_.x;		e_.x = 0; }
		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
		if (true == map->CheckHit(hit)) {
			this->pos.x = preX;		//移動をキャンセル
			break;
		}
	}
	//縦軸に対する移動
	while (e_.y != 0) {
		float  preY = this->pos.y;
		if (e_.y >= 1) { this->pos.y += 1;		e_.y -= 1; }
		else if (e_.y <= -1) { this->pos.y -= 1;		e_.y += 1; }
		else { this->pos.y += e_.y;		e_.y = 0; }
		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
		if (true == map->CheckHit(hit)) {
			this->pos.y = preY;		//移動をキャンセル
			break;
		}
	}
}
//-----------------------------------------------------------------------------
//足元接触判定
bool  BChara::CheckFoot()
{
	//あたり判定を基にして足元矩形を生成
	ML::Box2D  foot(this->hitBase.x,
		this->hitBase.y + this->hitBase.h,
		this->hitBase.w,
		1);
	foot.Offset(this->pos);

	auto   map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
	if (nullptr == map) { return false; }//マップが無ければ判定しない(出来ない）
	return map->CheckHit(foot);
}
//-----------------------------------------------------------------------------
//正面接触判定（サイドビューゲーム専用）
bool BChara::CheckFront_LR()
{
	//正面の矩形に使用する変数を宣言
	int x = 0;
	if (this->angle_LR == Left) { x = this->hitBase.x - 1; }			  //キャラが左向きの時
	else                        { x = this->hitBase.x + this->hitBase.w; }//キャラが右向きの時
	//あたり判定を基にして正面矩形を生成
	ML::Box2D  front(x,this->hitBase.y,1,this->hitBase.h);
	front.Offset(this->pos);

	auto   map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
	if (nullptr == map) { return false; }//マップが無ければ判定しない(出来ない）
	return map->CheckHit(front);
}
//-----------------------------------------------------------------------------
//正面足元判定（サイドビューゲーム専用）
bool BChara::CheckFrontFoot_LR()
{
	//正面足元の矩形に使用する変数を宣言
	int x = 0;
	if (this->angle_LR == Left) { x = this->hitBase.x - 1; }//キャラが左向きの時
	else { x = this->hitBase.x + this->hitBase.w; }			//キャラが右向きの時
	//あたり判定を基にして正面足元矩形を生成
	ML::Box2D  frontFoot(x, this->hitBase.y+this->hitBase.h, 1, 1);
	frontFoot.Offset(this->pos);

	auto   map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
	if (nullptr == map) { return false; }//マップが無ければ判定しない(出来ない）
	return map->CheckHit(frontFoot);
}
//-----------------------------------------------------------------------------
//接触判定
bool BChara::CheckHit(const ML::Box2D& hit_)
{
	ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
	return me.Hit(hit_);
}
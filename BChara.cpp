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


	auto   map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
	if (nullptr == map) { return false; }//マップが無ければ判定しない(出来ない）
	return map->CheckHit(head);
}
//-----------------------------------------------------------------------------
//めり込まない移動処理
void BChara::CheckMove(ML::Vec2&  e_)
{
	//マップが存在するか調べてからアクセス
	auto   map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
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

	auto   map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
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

	auto   map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
	if (nullptr == map) { return false; }//マップが無ければ判定しない(出来ない）
	return map->CheckHit(front);
}
//背面接触判定（サイドビューゲーム専用）
bool BChara::CheckBack_LR()
{
	//背面の矩形に使用する変数を宣言
	int x = 0;
	if (this->angle_LR == Left) { x = this->hitBase.x + this->hitBase.w; }//キャラが左向きの時
	else						{ x = this->hitBase.x - 1; }			  //キャラが右向きの時
	//あたり判定を基にして背面矩形を生成
	ML::Box2D back(x, this->hitBase.y, 1, this->hitBase.h);
	back.Offset(this->pos);

	auto   map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
	if (nullptr == map) { return false; }//マップが無ければ判定しない(出来ない）
	return map->CheckHit(back);
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

	auto   map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
	if (nullptr == map) { return false; }//マップが無ければ判定しない(出来ない）
	return map->CheckHit(frontFoot);
}
//-----------------------------------------------------------------------------
//接触判定
bool BChara::CheckHit(const ML::Box2D& hit_)
{
	//キャラクタとの判定範囲に変更
	ML::Box2D me = this->recieveBase.OffsetCopy(this->pos);
	return me.Hit(hit_);
}
//自身から見た対象の位置を左右で返す
//引数	：	（現在の向き,比較対象のX座標）
BChara::Angle_LR BChara::Check_LR(const BChara::Angle_LR& now_, const float& x_)
{
	if (x_ < this->pos.x)
	{
		return Left;
	}
	else if (this->pos.x < x_)
	{
		return Right;
	}
	else
	{
		return now_;
	}
}
//-----------------------------------------------------------------------------
//アクセサメソッド

//状態を指定する
//引数	：	（State型）
void BChara::Set_State(const State& state_)
{
	this->state = state_;
}
//範囲攻撃か否かを指定する
//引数	：	（0か1）
void BChara::Set_Range_Wide(const int& wide_)
{
	this->range_Wide = wide_;
}
//ノックバックするか否かを指定する
//引数	：	（0か1）
void BChara::Set_Tip(const int& tip_)
{
	this->tip = tip_;
}
//遺体の消滅フラグを指定する
//引数	：	（0か1）
void BChara::Set_Flag_Erase(const int& erase_)
{
	this->flag_erase = erase_;
}
//ふっとび量を指定する
//引数	：	（ML::Vec2）
void BChara::Set_Move_Back(const ML::Vec2& move_)
{
	this->move_back = move_;
}
//範囲攻撃か否かを取得する
bool BChara::Get_Range_Wide()
{
	return this->range_Wide;
}
//ノックバックするか否かを取得する
bool BChara::Get_Tip()
{
	return this->tip;
}
//遺体の消滅フラグを取得する
bool BChara::Get_Flag_Erase()
{
	return this->flag_erase;
}
//角度を取得する
float BChara::Get_Angle()
{
	return this->angle;
}
//ふっとび量を取得する
ML::Vec2 BChara::Get_Move_Back()
{
	return this->move_back;
}
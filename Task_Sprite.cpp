//-------------------------------------------------------------------
//カメラ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Sprite.h"
#include  "Task_Map2D.h"
#include  "Task_Player.h"

namespace  Sprite
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "SpriteImg";
		DG::Image_Create(this->imageName, "./data/image/妖精.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->render2D_Priority[1] = 0.5f;		//描画順
		this->controllerName = "P1";			//コントローラー名初期化
		this->speed_to_Vec = 0.05f;				//目標への移動量
		this->dist_to_Vec = 200.0f;				//プレイヤからの距離
		this->dist_height = float(SIZE_CHIP*8);	//プレイヤからの上方位置

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//ポーズ
		if (ge->pause) { return; }
		//プレイヤとの相対距離を取得
		//対象が存在するか確認してからアクセス
		if (auto tg = this->target.lock())
		{
			//プレイヤのやや上方を追従する
			ML::Vec2  toVec = ML::Vec2(tg->pos.x,tg->pos.y-this->dist_height) - this->pos;
			//右スティックの向きに合わせて自分の移動先を変更
			auto in = DI::GPad_GetState(this->controllerName);
			float angle = atan2(in.RStick.axis.y, in.RStick.axis.x);
			//スティックが入力されている場合のみ
			if (!angle == 0.0f)
			{
				toVec += ML::Vec2(cos(angle)*this->dist_to_Vec, sin(angle)*this->dist_to_Vec);
			}
			//ターゲットに近づく
			this->pos += toVec * this->speed_to_Vec;
		}
		//カメラの位置を再調整
		{
			//自分を画面の何処に置くか（今回は画面中央）
			int  px = ge->camera2D.w / 2;
			int  py = ge->camera2D.h / 2;
			//自分を画面中央に置いた時のカメラの左上座標を求める
			int  cpx = int(this->pos.x) - px;
			int  cpy = int(this->pos.y) - py;
			//カメラの座標を更新
			ge->camera2D.x = cpx;
			ge->camera2D.y = cpy;
		}
		//マップ外を写さないようにする調整処理
		auto   map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
		if (nullptr != map) {
			map->AjastCameraPos();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//デバッグモードのみ表示
		if (!ge->debugMode) { return; }
		ML::Box2D  draw(-16, -16, 32, 32);
		draw.Offset(this->pos);
		ML::Box2D  src(0, 0, 32, 32);

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.5f, 1, 1, 1));
	}
	//追従対象を指定
	void Object::Set_Target(const weak_ptr<BChara> pl_)
	{
		this->target = pl_;
	}
	//マップ外と上方向のあたり判定
	bool Object::Check_Out_OF_Map_Top()
	{
		RECT  r = { ge->camera2D.w / 2, ge->camera2D.y - 1, ge->camera2D.w / 2 + 1, ge->camera2D.y + 1 };
		//矩形がマップ外に出ていたら丸め込みを行う
		auto map = ge->GetTask_One_G<Map2D::Object>("フィールド");
		RECT  m = {
			map->hitBase.x,
			map->hitBase.y,
			map->hitBase.x + map->hitBase.w,
			map->hitBase.y + map->hitBase.h };
		if (r.left   < m.left) { r.left = m.left; }
		if (r.top    < m.top) { r.top = m.top; }
		if (r.right  > m.right) { r.right = m.right; }
		if (r.bottom > m.bottom) { r.bottom = m.bottom; }

		//ループ範囲調整
		int sx, sy, ex, ey;
		sx = r.left / SIZE_CHIP;
		sy = r.top / SIZE_CHIP;
		ex = (r.right - 1) / SIZE_CHIP;
		ey = (r.bottom - 1) / SIZE_CHIP;

		//範囲内の障害物を探す
		//視界の範囲外は-1
		for (int y = sy; y <= ey; ++y) {
			for (int x = sx; x <= ex; ++x) {
				if (0 > map->arr[y][x]) {
					return true;
				}
			}
		}
		return false;
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}
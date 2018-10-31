//-------------------------------------------------------------------
//２次元配列マップ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Player.h"
#include  "Task_Enemy01.h"
#include  "Task_EnemySearch.h"
#include  "Task_Item00.h"

namespace  Map2D
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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
		this->render2D_Priority[1] = 0.9f;	//描画順
		this->imageName = "MapChipImg";		//イメージ名の初期化
		this->cnt_Quake = 0;				//画面を揺らす周期
		this->dist_Quake = 0;				//画面揺れ幅
		this->limit_Quake = 0;				//画面を揺らす時間
		//マップのゼロクリア
		for (int y = 0; y < SIZE_MAP_H; ++y) {
			for (int x = 0; x < SIZE_MAP_W; ++x) 
			{
				this->arr[y][x] = 0;
			}
		}
		this->sizeX = 0;
		this->sizeY = 0;
		this->hitBase = ML::Box2D(0, 0, 0, 0);

		//マップチップ情報の初期化
		//画像リソースに番号を振る
		for (int c = 0; c < 24; ++c) 
		{
			int  x = (c % 4);
			int  y = (c / 4);
			//src
			this->chip[c] = ML::Box2D(x * SIZE_CHIP, y * SIZE_CHIP, SIZE_CHIP, SIZE_CHIP);
		}

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		DG::Image_Erase(this->imageName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//カメラが完全にマップ外を指しているか調べる
		if (false == this->hitBase.Hit(ge->camera2D)) {
			return; //完全に外に出ていたらその時点で描画処理を取りやめる
		}
		//カメラとマップが重なっている範囲だけの矩形を作る
		RECT  c = {
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h };
		RECT  m = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h };
		//２つの矩形の重なっている範囲だけの矩形を求める
		RECT  isr;
		isr.left = max(c.left, m.left);
		isr.top = max(c.top, m.top);
		isr.right = min(c.right, m.right);
		isr.bottom = min(c.bottom, m.bottom);
		//ループ範囲を決定
		int sx, sy, ex, ey;
		sx = isr.left / SIZE_CHIP;
		sy = isr.top / SIZE_CHIP;
		ex = (isr.right - 1) / SIZE_CHIP;
		ey = (isr.bottom - 1) / SIZE_CHIP;

		//画面内の範囲だけ描画
		for (int y = sy; y <= ey; ++y) {
			for (int x = sx; x <= ex; ++x) {
				ML::Box2D  draw(x * SIZE_CHIP, y * SIZE_CHIP, SIZE_CHIP, SIZE_CHIP);
				//画面揺れ処理（マップチップの縦軸を揺らす）
				if (this->limit_Quake > 0)
				{
					draw.y += int(sin(this->cnt_Quake)*this->dist_Quake);
				}
				//スクロール対応
				draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
				DG::Image_Draw(this->imageName, draw, this->chip[this->arr[y][x]]);
			}
		}
		//各カウンタ増減
		if (this->limit_Quake > 0)
		{
			this->cnt_Quake++;
			this->limit_Quake--;
		}
	}
	//-------------------------------------------------------------------
	//マップファイル読み込み(今は仮処理）
	bool  Object::Load(const  string&  fpath_)
	{
		//ファイルを開く（読み込み）
		ifstream   fin(fpath_);
		if (!fin) { return  false; }//読み込み失敗
		//チップファイル名の読み込みと、画像のロード
		string   chipFileName, chipFilePath;
		fin >> chipFileName;
		chipFilePath = "./data/image/" + chipFileName;
		DG::Image_Create(this->imageName, chipFilePath);


		//マップ配列サイズの読み込み
		fin >> this->sizeX >> this->sizeY;
		this->hitBase = ML::Box2D(0, 0, this->sizeX * SIZE_CHIP, this->sizeY * SIZE_CHIP);

		//マップ配列データの読み込み
		for (int y = 0; y < this->sizeY; ++y) {
			for (int x = 0; x < this->sizeX; ++x) {
				fin >> this->arr[y][x];
			}
		}
		//ファイルを閉じる
		fin.close();
		//チップ番号に対応した処理
		this->Map_Adjust_Chip();
		return  true;
	}
	//-------------------------------------------------------------------
	//あたり判定
	bool Object::CheckHit(const  ML::Box2D&  h_)
	{
		RECT  r = { h_.x, h_.y, h_.x + h_.w, h_.y + h_.h };
		//矩形がマップ外に出ていたら丸め込みを行う
		RECT  m = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h };
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
		for (int y = sy; y <= ey; ++y) {
			for (int x = sx; x <= ex; ++x) {
				if (8 <= this->arr[y][x]) {
					return true;
				}
			}
		}
		return false;
	}
	//-------------------------------------------------------------------
	//マップ外を見せないようにカメラを位置調整する
	void  Object::AjastCameraPos()
	{
		//カメラとマップの範囲を用意
		RECT  c = {
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h };
		RECT  m = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h };

		//カメラの位置を調整
		if (c.right  > m.right) { ge->camera2D.x = m.right - ge->camera2D.w; }
		if (c.bottom > m.bottom) { ge->camera2D.y = m.bottom - ge->camera2D.h; }
		if (c.left < m.left) { ge->camera2D.x = m.left; }
		if (c.top  < m.top) { ge->camera2D.y = m.top; }
		//マップがカメラより小さい場合
		if (this->hitBase.w < ge->camera2D.w) { ge->camera2D.x = m.left; }
		if (this->hitBase.h < ge->camera2D.h) { ge->camera2D.y = m.top; }
	}
	//チップ番号に対応した処理
	//壁の向きに合わせてリソースを変える
	//チップ番号に合わせたキャラクタを生成する
	void Object::Map_Adjust_Chip()
	{
		//縦横サイズの2次元配列を宣言
		int w_map[SIZE_MAP_H][SIZE_MAP_W];
		for (int y = 0; y < this->sizeY; ++y)
		{
			for (int x = 0; x < this->sizeX; ++x)
			{
				int no;
				switch (this->arr[y][x])
				{
				default:
					no = this->arr[y][x];
					break;
				//アイテム00
				case 6:
					this->Create_Item00(ML::Vec2(float(x*SIZE_CHIP), float(y*SIZE_CHIP)));
					//透明マスで上書き
					no = 0;
					break;
				//エネミー01
				case 7:
					this->Create_Enemy01(ML::Vec2(float(x*SIZE_CHIP), float(y*SIZE_CHIP)));
					no = 0;
					break;
				//壁の場合は隣とくっつける
				case 8:
					//壁マスの基準値
					no = 8;
					//上が壁？
					if (y - 1 >= 0 && this->arr[y - 1][x] == 8)
					{
						no += 1;
					}
					//右が壁？
					if (x + 1 <= this->sizeX - 1 && this->arr[y][x + 1] == 8)
					{
						no += 2;
					}
					//下が壁？
					if (y + 1 <= this->sizeY - 1 && this->arr[y + 1][x] == 8)
					{
						no += 4;
					}
					//左が壁？
					if (x - 1 >= 0 && this->arr[y][x - 1] == 8)
					{
						no += 8;
					}
					break;
				}
				w_map[y][x] = no;
			}
		}
		//マップを上書きコピー
		memcpy(this->arr, w_map, sizeof(this->arr));
	}
	//エネミーの生成
	//引数	：	（初期座標,移動速度,HP）
	void Object::Create_Enemy01(const ML::Vec2& pos_)
	{
		auto ene = Enemy01::Object::Create(true);
		ene->pos = pos_;
		auto es = EnemySearch::Object::Create(true);
		es->hitBase = ene->Get_Search();
		es->Set_Target(ene);
	}
	//アイテム00の生成
	//引数	：	（初期座標）
	void Object::Create_Item00(const ML::Vec2& pos_)
	{
		auto item = Item00::Object::Create(true);
		item->pos = pos_;
	}
	//画面の揺れ幅を指定する
	void Object::Set_Dist_Quake(const int& dist_)
	{
		this->dist_Quake = dist_;
	}
	//画面を揺らす時間を指定する
	void Object::Set_Limit_Quake(const int& limit_)
	{
		this->limit_Quake = limit_;
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
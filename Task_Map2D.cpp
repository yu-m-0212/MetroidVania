//-------------------------------------------------------------------
//�Q�����z��}�b�v
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
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[1] = 0.9f;	//�`�揇
		this->imageName = "MapChipImg";		//�C���[�W���̏�����
		this->cnt_Quake = 0;				//��ʂ�h�炷����
		this->dist_Quake = 0;				//��ʗh�ꕝ
		this->limit_Quake = 0;				//��ʂ�h�炷����
		//�}�b�v�̃[���N���A
		for (int y = 0; y < SIZE_MAP_H; ++y) {
			for (int x = 0; x < SIZE_MAP_W; ++x) 
			{
				this->arr[y][x] = 0;
			}
		}
		this->sizeX = 0;
		this->sizeY = 0;
		this->hitBase = ML::Box2D(0, 0, 0, 0);

		//�}�b�v�`�b�v���̏�����
		//�摜���\�[�X�ɔԍ���U��
		for (int c = 0; c < 24; ++c) 
		{
			int  x = (c % 4);
			int  y = (c / 4);
			//src
			this->chip[c] = ML::Box2D(x * SIZE_CHIP, y * SIZE_CHIP, SIZE_CHIP, SIZE_CHIP);
		}

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		DG::Image_Erase(this->imageName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�J���������S�Ƀ}�b�v�O���w���Ă��邩���ׂ�
		if (false == this->hitBase.Hit(ge->camera2D)) {
			return; //���S�ɊO�ɏo�Ă����炻�̎��_�ŕ`�揈��������߂�
		}
		//�J�����ƃ}�b�v���d�Ȃ��Ă���͈͂����̋�`�����
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
		//�Q�̋�`�̏d�Ȃ��Ă���͈͂����̋�`�����߂�
		RECT  isr;
		isr.left = max(c.left, m.left);
		isr.top = max(c.top, m.top);
		isr.right = min(c.right, m.right);
		isr.bottom = min(c.bottom, m.bottom);
		//���[�v�͈͂�����
		int sx, sy, ex, ey;
		sx = isr.left / SIZE_CHIP;
		sy = isr.top / SIZE_CHIP;
		ex = (isr.right - 1) / SIZE_CHIP;
		ey = (isr.bottom - 1) / SIZE_CHIP;

		//��ʓ��͈̔͂����`��
		for (int y = sy; y <= ey; ++y) {
			for (int x = sx; x <= ex; ++x) {
				ML::Box2D  draw(x * SIZE_CHIP, y * SIZE_CHIP, SIZE_CHIP, SIZE_CHIP);
				//��ʗh�ꏈ���i�}�b�v�`�b�v�̏c����h�炷�j
				if (this->limit_Quake > 0)
				{
					draw.y += int(sin(this->cnt_Quake)*this->dist_Quake);
				}
				//�X�N���[���Ή�
				draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
				DG::Image_Draw(this->imageName, draw, this->chip[this->arr[y][x]]);
			}
		}
		//�e�J�E���^����
		if (this->limit_Quake > 0)
		{
			this->cnt_Quake++;
			this->limit_Quake--;
		}
	}
	//-------------------------------------------------------------------
	//�}�b�v�t�@�C���ǂݍ���(���͉������j
	bool  Object::Load(const  string&  fpath_)
	{
		//�t�@�C�����J���i�ǂݍ��݁j
		ifstream   fin(fpath_);
		if (!fin) { return  false; }//�ǂݍ��ݎ��s
		//�`�b�v�t�@�C�����̓ǂݍ��݂ƁA�摜�̃��[�h
		string   chipFileName, chipFilePath;
		fin >> chipFileName;
		chipFilePath = "./data/image/" + chipFileName;
		DG::Image_Create(this->imageName, chipFilePath);


		//�}�b�v�z��T�C�Y�̓ǂݍ���
		fin >> this->sizeX >> this->sizeY;
		this->hitBase = ML::Box2D(0, 0, this->sizeX * SIZE_CHIP, this->sizeY * SIZE_CHIP);

		//�}�b�v�z��f�[�^�̓ǂݍ���
		for (int y = 0; y < this->sizeY; ++y) {
			for (int x = 0; x < this->sizeX; ++x) {
				fin >> this->arr[y][x];
			}
		}
		//�t�@�C�������
		fin.close();
		//�`�b�v�ԍ��ɑΉ���������
		this->Map_Adjust_Chip();
		return  true;
	}
	//-------------------------------------------------------------------
	//�����蔻��
	bool Object::CheckHit(const  ML::Box2D&  h_)
	{
		RECT  r = { h_.x, h_.y, h_.x + h_.w, h_.y + h_.h };
		//��`���}�b�v�O�ɏo�Ă�����ۂߍ��݂��s��
		RECT  m = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h };
		if (r.left   < m.left) { r.left = m.left; }
		if (r.top    < m.top) { r.top = m.top; }
		if (r.right  > m.right) { r.right = m.right; }
		if (r.bottom > m.bottom) { r.bottom = m.bottom; }

		//���[�v�͈͒���
		int sx, sy, ex, ey;
		sx = r.left / SIZE_CHIP;
		sy = r.top / SIZE_CHIP;
		ex = (r.right - 1) / SIZE_CHIP;
		ey = (r.bottom - 1) / SIZE_CHIP;

		//�͈͓��̏�Q����T��
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
	//�}�b�v�O�������Ȃ��悤�ɃJ�������ʒu��������
	void  Object::AjastCameraPos()
	{
		//�J�����ƃ}�b�v�͈̔͂�p��
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

		//�J�����̈ʒu�𒲐�
		if (c.right  > m.right) { ge->camera2D.x = m.right - ge->camera2D.w; }
		if (c.bottom > m.bottom) { ge->camera2D.y = m.bottom - ge->camera2D.h; }
		if (c.left < m.left) { ge->camera2D.x = m.left; }
		if (c.top  < m.top) { ge->camera2D.y = m.top; }
		//�}�b�v���J������菬�����ꍇ
		if (this->hitBase.w < ge->camera2D.w) { ge->camera2D.x = m.left; }
		if (this->hitBase.h < ge->camera2D.h) { ge->camera2D.y = m.top; }
	}
	//�`�b�v�ԍ��ɑΉ���������
	//�ǂ̌����ɍ��킹�ă��\�[�X��ς���
	//�`�b�v�ԍ��ɍ��킹���L�����N�^�𐶐�����
	void Object::Map_Adjust_Chip()
	{
		//�c���T�C�Y��2�����z���錾
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
				//�A�C�e��00
				case 6:
					this->Create_Item00(ML::Vec2(float(x*SIZE_CHIP), float(y*SIZE_CHIP)));
					//�����}�X�ŏ㏑��
					no = 0;
					break;
				//�G�l�~�[01
				case 7:
					this->Create_Enemy01(ML::Vec2(float(x*SIZE_CHIP), float(y*SIZE_CHIP)));
					no = 0;
					break;
				//�ǂ̏ꍇ�ׂ͗Ƃ�������
				case 8:
					//�ǃ}�X�̊�l
					no = 8;
					//�オ�ǁH
					if (y - 1 >= 0 && this->arr[y - 1][x] == 8)
					{
						no += 1;
					}
					//�E���ǁH
					if (x + 1 <= this->sizeX - 1 && this->arr[y][x + 1] == 8)
					{
						no += 2;
					}
					//�����ǁH
					if (y + 1 <= this->sizeY - 1 && this->arr[y + 1][x] == 8)
					{
						no += 4;
					}
					//�����ǁH
					if (x - 1 >= 0 && this->arr[y][x - 1] == 8)
					{
						no += 8;
					}
					break;
				}
				w_map[y][x] = no;
			}
		}
		//�}�b�v���㏑���R�s�[
		memcpy(this->arr, w_map, sizeof(this->arr));
	}
	//�G�l�~�[�̐���
	//����	�F	�i�������W,�ړ����x,HP�j
	void Object::Create_Enemy01(const ML::Vec2& pos_)
	{
		auto ene = Enemy01::Object::Create(true);
		ene->pos = pos_;
		auto es = EnemySearch::Object::Create(true);
		es->hitBase = ene->Get_Search();
		es->Set_Target(ene);
	}
	//�A�C�e��00�̐���
	//����	�F	�i�������W�j
	void Object::Create_Item00(const ML::Vec2& pos_)
	{
		auto item = Item00::Object::Create(true);
		item->pos = pos_;
	}
	//��ʂ̗h�ꕝ���w�肷��
	void Object::Set_Dist_Quake(const int& dist_)
	{
		this->dist_Quake = dist_;
	}
	//��ʂ�h�炷���Ԃ��w�肷��
	void Object::Set_Limit_Quake(const int& limit_)
	{
		this->limit_Quake = limit_;
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
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
	//���\�[�X�N���X�̐���
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
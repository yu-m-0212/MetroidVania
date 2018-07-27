//-------------------------------------------------------------------
//�Q�����z��}�b�v
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"

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
		this->render2D_Priority[1] = 0.9f;
		this->imageName = "MapChipImg";
		this->chipSize = 64;
		//�}�b�v�̃[���N���A
		for (int y = 0; y < 100; ++y) {
			for (int x = 0; x < 100; ++x) {
				this->arr[y][x] = 0;
			}
		}
		this->sizeX = 0;
		this->sizeY = 0;
		this->hitBase = ML::Box2D(0, 0, 0, 0);

		//�}�b�v�`�b�v���̏�����
		//�摜���\�[�X�ɔԍ���U��
		for (int c = 0; c < 16; ++c) {
			int  x = (c % 8);
			int  y = (c / 8);
			//src
			this->chip[c] = ML::Box2D(x * 32, y * 32, 32, 32);
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
		sx = isr.left / this->chipSize;
		sy = isr.top / this->chipSize;
		ex = (isr.right - 1) / this->chipSize;
		ey = (isr.bottom - 1) / this->chipSize;

		//��ʓ��͈̔͂����`��
		for (int y = sy; y <= ey; ++y) {
			for (int x = sx; x <= ex; ++x) {
				ML::Box2D  draw(x * this->chipSize, y * this->chipSize, this->chipSize, this->chipSize);
				//�X�N���[���Ή�
				draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

				DG::Image_Draw(this->imageName, draw, this->chip[this->arr[y][x]]);
			}
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
		this->hitBase = ML::Box2D(0, 0, this->sizeX * this->chipSize, this->sizeY * this->chipSize);

		//�}�b�v�z��f�[�^�̓ǂݍ���
		for (int y = 0; y < this->sizeY; ++y) {
			for (int x = 0; x < this->sizeX; ++x) {
				fin >> this->arr[y][x];
			}
		}
		fin.close();

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
		sx = r.left / this->chipSize;
		sy = r.top / this->chipSize;
		ex = (r.right - 1) / this->chipSize;
		ey = (r.bottom - 1) / this->chipSize;

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
//-------------------------------------------------------------------
//�v���C���̏o���e
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Shot01.h"
#include  "Task_Player.h"
#include  "Task_Shot00.h"
#include  "Task_Barrier.h"
#include  "Task_Effect.h"
#include  "Task_Boss_Head.h"

namespace  Shot01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "Shot01Img";
		DG::Image_Create(this->imageName, "./data/image/Shot00.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
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
		this->render2D_Priority[1] = 0.4f;		//�`�揇
		this->recieveBase = this->hitBase;		//�L�����N�^�Ƃ̐ڐG��`
		this->flag_Erase = true;				//�ڐG�������邩
		this->flag_reflect = false;				//���˂����e���ۂ�
		this->power = 0;						//�U���́i�������Ɏw�肷��j
		this->rate_reflect_power = 3;			//���˂����e�ɗ^����U���͔{��
		this->rate_speed_reflect = 2.0f;		//���˂����Ƃ��ɗ^��������{��
		this->limit_Erase = 0;					//���ł���܂ł̎���
		this->add_un_hit_player = 60;			//�v���C���ɗ^���閳�G����
		this->add_un_hit_boss = 8;				//�{�X�ɗ^���閳�G����
		this->angle = 0.0f;						//�p�x
		this->eff = new Task_Effect::Object();	//���\�b�h�Ăяo��
		this->cnt_anim = 0;						//�A�j���J�E���^
		this->speed_anim = 15;					//�A�j���X�s�[�h
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//�A�j���[�V�����̓|�[�Y�����s��
		this->cnt_anim++;
		//�|�[�Y
		if (ge->pause) { return; }
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		this->moveCnt++;
		//�e��Ԃ��Ƃ̏���
		this->Move();

		//�ړ�
		this->pos += this->moveVec;
		//�v���C���Ƃ̔���
		if(!this->flag_reflect)
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("�v���C��");
			if (nullptr == pl) { return; }
			for (auto it = targets->begin();
				it != targets->end();
				++it) {
				//����ɐڐG�̗L�����m�F������
				if ((*it)->CheckHit(me)) {
					//����Ƀ_���[�W�̏������s�킹��
					BChara::AttackInfo at = { this->power,0,0 };
					(*it)->Recieved(this, at, this->add_un_hit_player);
					//�V���b�g�̂ݏ���
					//�i���͕����̂ɂ�����
					if (this->flag_Erase)
					{
						//�Ή������q�b�g���̃G�t�F�N�g�𐶐�
						//����A�����ɂ͑Ώۂ̓G�̍��W�������
						this->Effect_Hit((*it)->pos);
						this->Kill();
					}
					else
					{
						//�Ή������q�b�g���̃G�t�F�N�g�𐶐�
						//����A�����ɂ͑Ώۂ̓G�̍��W�������
						//�i���͋�`���c��ׁA���������u�Ԃ̂݃G�t�F�N�g�𐶐�����
						if ((*it)->moveCnt == 0)
						{
							this->Effect_Hit((*it)->pos);
						}
					}
					break;
				}
			}
		}
		//���ˏ���
		auto barrier = ge->GetTask_One_GN<Barrier::Object>(Barrier::defGroupName,Barrier::defName);
		if (nullptr != barrier)
		{
			ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
			ML::Box2D you = barrier->recieveBase.OffsetCopy(barrier->pos);
			//���g���܂����˒e�ł͂Ȃ��Ƃ�
			if (!this->flag_reflect)
			{
				if (you.Hit(me))
				{
					if (barrier->state==LandStomp||
						barrier->state==AirStomp||
						barrier->state==StompLanding)
					{
						this->moveVec = ML::Vec2(-this->moveVec.x, -this->moveVec.y);
						//���˃t���O���]
						this->flag_reflect = true;
						//���˂����e�U���͕␳��������
						this->power *= this->rate_reflect_power;
						//���˂����e�̑��x���グ��
						this->moveVec *= this->rate_speed_reflect;
					}
				}
			}
		}
		//�ˌ��͕ǂɓ�����Ə��ł���
		if (this->flag_Erase)
		{
			if (auto map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName))
			{
				ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
				if (true == map->CheckHit(hit))
				{
					//�ǂɓ��������Ƃ��̓G�t�F�N�g�𐶐����Ȃ�
					this->Kill();
				}
			}
		}
		//���E�̎��Ԃ��}���������
		if (this->moveCnt >= this->limit_Erase)
		{
			//���Ő\��
			this->Kill();
			return;
		}
		//���˂����e�̓G�l�~�[�Ƃ�������s��
		if (this->flag_reflect)
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("�G");
			if (nullptr == pl) { return; }
			for (auto it = targets->begin();
				it != targets->end();
				++it) {
				//����ɐڐG�̗L�����m�F������
				if ((*it)->CheckHit(me)) {
					//����Ƀ_���[�W�̏������s�킹��
					BChara::AttackInfo at = { this->power,0,0 };
					(*it)->Recieved(this, at, this->add_un_hit_player);
					//�Ή������q�b�g���̃G�t�F�N�g�𐶐�
					//����A�����ɂ͑Ώۂ̓G�̍��W�������
					this->Effect_Hit((*it)->pos);
					this->Kill();
					break;
				}
			}
		}
		//���˂����e�̓{�X�Ƃ�������s��
		if (this->flag_reflect)
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("�{�X");
			if (nullptr == pl) { return; }
			for (auto it = targets->begin();
				it != targets->end();
				++it) {
				//����ɐڐG�̗L�����m�F������
				if ((*it)->CheckHit(me)) {
					//����Ƀ_���[�W�̏������s�킹��
					BChara::AttackInfo at = { this->power,0,0 };
					(*it)->Recieved(this, at, this->add_un_hit_boss);
					//�h�ꑬ�x����莞�ԏオ��
					auto head =
						ge->GetTask_One_GN<Boss_Head::Object>(Boss_Head::defGroupName, Boss_Head::defName);
					head->limit_hit_reflect = LIMIT_HIT_REFLECT;
					//�Ή������q�b�g���̃G�t�F�N�g�𐶐�
					//����A�����ɂ͑Ώۂ̓G�̍��W�������
					this->Effect_Hit((*it)->pos);
					this->Kill();
					break;
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase;
		draw.Offset(this->pos);
		ML::Box2D src(0, 192, 16, 16);
		//���˂��ꂽ�e�̓v���C���̐F�ɕύX����
		if (this->flag_reflect)
		{
			src.y = 208;
		}
		//�A�j���[�V����
		int anim;
		anim = this->cnt_anim / this->speed_anim;
		anim %= 2;
		src.x = this->hitBase.w*anim;
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}
	//�Ăяo�����^�X�N���������ݒ肷��
	void Object::Set_Limit(const int& cl_)
	{
		this->limit_Erase = cl_;
	}
	//�ǂ�G�ɏՓ˂����Ƃ��A�����邩�ۂ����w�肷��
	void Object::Set_Erase(const int& erase_)
	{
		this->flag_Erase = erase_;
	}
	//�O�����琶������ہA�U���͂��w��
	//����	�F	�i�����l�j
	void Object::Set_Power(const int& pow_)
	{
		this->power = pow_;
	}
	//�p�x���w�肷��
	//����	�F	�i�p�x�j
	void Object::Set_Angle(const float& angle_)
	{
		this->angle = angle_;
	}
	//��Ԃ��Ƃɍs�����w�肷��
	void Object::Move()
	{
		switch (this->state)
		{
		default:
			break;
		case Shoot:
			//�G�ɏՓ˂����Ƃ������邩�ۂ�
			this->flag_Erase = true;
			break;
		}
	}
	//���ł���ہA��Ԃɉ����ăG�t�F�N�g�𐶐�
	//����	�F	�i�G�t�F�N�g�𐶐�������W�j
	//case�̒��Ő錾�������[�J���ϐ��̃X�R�[�v��switch���S�̂ɋy�ԋ��ꂪ�����
	//�Ӑ}�I�Ƀu���b�N{}��݂��邱��
	void Object::Effect_Hit(const ML::Vec2& pos_)
	{
		switch (this->state)
		{
		default:
			break;
		case Shoot:
			this->eff->Create_Effect(1, this->pos, this->angle,this->angle_LR);
			break;
		}
	}
	//�V���b�g�𐶐�����
	//����	�F	�i���W,�ړ���,��`,���Ŏ���,�U����,�ǂɓ��������������邩�ۂ��j
	void Object::Create_Shot(const ML::Vec2& pos_, const ML::Vec2& vec_, const ML::Box2D& hit_, const int& time_, const int& power_, const bool& erase_)
	{
		auto shot = Shot01::Object::Create(true);
		shot->pos = pos_;
		shot->moveVec = vec_;
		shot->hitBase = hit_;
		shot->limit_Erase = time_;
		shot->power = power_;
		shot->flag_Erase = erase_;
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
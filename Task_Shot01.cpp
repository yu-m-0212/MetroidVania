//-------------------------------------------------------------------
//�v���C���̏o���e
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Shot01.h"
#include  "Task_Player.h"
#include  "Task_Shot00.h"
#include  "Task_Effect.h"

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
		this->power = 0;						//�U����
		this->limit_Erase = 0;					//���ł���܂ł̎���
		this->add_un_hit = 60;					//�v���C���ɗ^���閳�G����
		this->angle = 0.0f;						//�p�x
		this->eff = new Task_Effect::Object();	//���\�b�h�Ăяo��
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
		//�|�[�Y
		if (ge->pause) { return; }
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		this->moveCnt++;
		//�e��Ԃ��Ƃ̏���
		this->Move();

		//�ړ�
		this->pos += this->moveVec;
		//�����蔻��
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
					(*it)->Received(this, at,this->add_un_hit);
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
		auto shot_pl = ge->GetTask_One_G<Shot00::Object>(Shot00::defGroupName);
		if (nullptr != shot_pl)
		{
			ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
			ML::Box2D you = shot_pl->hitBase.OffsetCopy(shot_pl->pos);
			if (you.Hit(me) && shot_pl->state == StompLanding)
			{
				this->moveVec = ML::Vec2(-this->moveVec.x, -this->moveVec.y);
			}
		}
		//�ˌ��͕ǂɓ�����Ə��ł���
		if (this->flag_Erase)
		{
			if (auto map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName,Map2D::defName))
			{
				ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
				if (true == map->CheckHit(hit))
				{
					//�Ή������q�b�g���̃G�t�F�N�g�𐶐������ł���
					this->Effect_Hit(this->pos);
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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase;
		draw.Offset(this->pos);
		ML::Box2D src(0, 0, 32, 32);
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.5f, 1.0f, 0.0f, 0.0f));
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
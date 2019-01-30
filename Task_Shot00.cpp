//-------------------------------------------------------------------
//�v���C���̏o���e
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Shot00.h"
#include  "Task_Player.h"
#include  "Task_Effect.h"
#include  "Task_Boss_Head.h"

namespace  Shot00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "Shot00Img";
		DG::Image_Create(this->imageName, "./data/image/Shot00.png");
		this->name_se_reflect = "reflect_se";
		DM::Sound_CreateSE(this->name_se_reflect, "./data/sound/wav/reflect_shot_player.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DM::Sound_Erase(this->name_se_reflect);
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
		this->power = 0;						//�U����
		this->un_hit = 8;						//����ɗ^���閳�G����
		this->limit_Erase = 0;					//���ł���܂ł̎���
		this->eff = new Task_Effect::Object();	//���\�b�h�Ăяo��
		this->flag_reflect = false;				//���˂������ۂ�
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
		//�G���Ƃ̐ڐG����
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("�G");
			if (nullptr != targets)
			{
				for (auto it = targets->begin();
					it != targets->end();
					++it) {
					//����ɐڐG�̗L�����m�F������
					if ((*it)->CheckHit(me))
					{
						//����Ƀ_���[�W�̏������s�킹��
						BChara::AttackInfo at = { this->power,0,0 };
						(*it)->Recieved(this, at, this->un_hit);
						//���������u�Ԃ̂݃G�t�F�N�g�𐶐�����
						this->Effect_Hit((*it)->pos);
						this->Kill();
						break;
					}
				}
			}
		}
		//�{�X�Ƃ̐ڐG����
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("�{�X");
			if (nullptr != targets)
			{
				for (auto it = targets->begin();
					it != targets->end();
					++it) {
					//����ɐڐG�̗L�����m�F������
					if ((*it)->CheckHit(me)) 
					{
						//����Ƀ_���[�W�̏������s�킹��
						BChara::AttackInfo at = { this->power,0,0 };
						(*it)->Recieved(this, at, this->un_hit);
						//�G�t�F�N�g�𐶐�������
						this->Effect_Hit((*it)->pos);
						this->Kill();
						break;
					}
				}
			}
		}
		//�{�X�̓����Ƃ̐ڐG����
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto boss_head = ge->GetTask_One_G<Boss_Head::Object>(Boss_Head::defGroupName);
			if (nullptr != boss_head)
			{
				//���˂���Ă��Ȃ����̂݃{�X�Ƃ̏������s��
				if (!this->flag_reflect)
				{
					//�{�X�w�b�h���C�₵�Ă���΃_���[�W
					if (boss_head->state == Stan)
					{
						//����ɐڐG�̗L�����m�F������
						if (boss_head->CheckHit(me))
						{
							//����Ƀ_���[�W�̏������s�킹��
							BChara::AttackInfo at = { this->power,0,0 };
							boss_head->Recieved(this, at, this->un_hit);
							//�G�t�F�N�g�𐶐�������
							this->Effect_Hit(boss_head->pos);
							this->Kill();
							//�ړ��ʂ̔��]
							this->moveVec = ML::Vec2(-this->moveVec.x, -this->moveVec.y);
						}
					}
					//�{�X�w�b�h���C�₵�Ă��Ȃ���Δ��˂����
					else
					{
						//����ɐڐG�̗L�����m�F������
						if (boss_head->CheckHit(me))
						{
							//SE�̍Đ�
							DM::Sound_Play_Volume(this->res->name_se_reflect, false, VOLUME_SE_REFLECT_SHOT);
							//X�����݂̂̔��]
							this->moveVec.x = -this->moveVec.x;
							this->angle = -this->angle;
							//�t���O���]
							this->flag_reflect = true;
						}
					}
				}
			}
		}
		//�ˌ��͕ǂɓ�����Ə��ł���
		auto map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
			if (nullptr != map)
			{
				ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
				if (true == map->CheckHit(hit))
				{
					//�ǂƂ̐ڐG�ł̓G�t�F�N�g�𐶐����Ȃ�
					this->Kill();
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
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		ML::Box2D src(0, 224, 32, 32);
		if (this->state == Shoot ||
			this->state == Jumpshoot ||
			this->state == Fallshoot)
		{
			DG::Image_Rotation(this->res->imageName, this->angle, ML::Vec2(float(this->hitBase.w / 2), float(this->hitBase.h / 2)));
			DG::Image_Draw(this->res->imageName, draw, src, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
		}
		//�ȉ��U���͈͕\��
		if (ge->debugMode)
		{
			ML::Box2D srcDebug(0, 0, 32, 32);
			DG::Image_Rotation(this->res->imageName, this->angle, ML::Vec2(float(this->hitBase.w / 2), float(this->hitBase.h / 2)));
			DG::Image_Draw(this->res->imageName, draw, srcDebug, ML::Color(0.5f, 1.0f, 0.0f, 0.0f));
		}
	}
	//��Ԃ��Ƃɍs�����w�肷��
	void Object::Move()
	{
		auto pl = ge->GetTask_One_G <Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		switch (this->state)
		{
		default:
			break;
		case StompLanding:
			break;
		case Shoot:
			break;
		case Air:
			//�p���`���̓v���C���̓����ɍ��킹�Ĕ����`���O�i����
			this->moveVec = pl->moveVec;
			//�v���C�����ǂɏՓ˂�����ړ��ʂ�0��
			if (pl->CheckFront_LR() || pl->CheckBack_LR())
			{
				this->moveVec.x = 0.0f;
			}
			//�v���C�����_���[�W���󂯂�(��Ԃ��ς��)�Ə���
			if (pl->state == Damage)
			{
				this->Kill();
				return;
			}
			break;
		case Jumpshoot:
		case Fallshoot:
			break;
		}
	}
	//���ł���ہA��Ԃɉ����ăG�t�F�N�g�𐶐�
	//����	�F	�i�G�t�F�N�g�𐶐�������W�j
	void Object::Effect_Hit(const ML::Vec2& pos_)
	{
		switch (this->state)
		{
		default:
			break;
		case Shoot:
		case Jumpshoot:
		case Fallshoot:
			this->eff->Create_Effect(1, this->pos, this->angle, this->angle_LR);
		}
	}
	//�Ăяo�����^�X�N���������ݒ肷��
	void Object::Set_Limit(const int& cl_)
	{
		this->limit_Erase = cl_;
	}
	//�O�����琶������ہA�U���͂��w��
	//����	�F	�i�����l�j
	void Object::Set_Power(const int& pow_)
	{
		this->power = pow_;
	}
	//�O������`��p�x���w��
	//����	�F	�i�p�x�j
	void Object::Set_Angle(const float& angle_)
	{
		this->angle = angle_;
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
//-------------------------------------------------------------------
//�v���C���̔��������锻���`�i���˃A�r���e�B�j
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Barrier.h"
#include  "Task_Player.h"
#include  "Task_Effect.h"

namespace  Barrier
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "barrier";
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
		this->power = 0;						//�U����
		this->un_hit = 8;						//����ɗ^���閳�G����
		this->limit_Erase = 0;					//���ł���܂ł̎���
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
		//�v���C���̍��W�ɍ��킹��
		this->pos = pl->pos;
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
						if ((*it)->moveCnt == 0)
						{
							this->Effect_Hit((*it)->pos);
						}
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
						//���������u�Ԃ̂݃G�t�F�N�g�𐶐�����
						if ((*it)->moveCnt == 0)
						{
							this->Effect_Hit((*it)->pos);
						}
						break;
					}
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
		ML::Box2D draw = this->recieveBase;
		draw.Offset(this->pos);
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
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
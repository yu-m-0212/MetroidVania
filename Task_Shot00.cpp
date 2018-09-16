//-------------------------------------------------------------------
//�v���C���̏o���e
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Shot00.h"
#include  "Task_Player.h"
#include  "Task_Effect.h"

namespace  Shot00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "Shot00Img";
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
		this->render2D_Priority[1] = 0.4f;
		this->recieveBase = this->hitBase;
		this->eraseFlag = true;
		this->power = 0;			
		this->cntLimit = 0;			//���ł���܂ł̎���
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
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		this->moveCnt++;
		//�e��Ԃ��Ƃ̏���
		this->Move();
		
		//�ړ�
		this->pos += this->moveVec;
		//�����蔻��
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("�G");
			for (auto it = targets->begin();
				it != targets->end();
				++it) {
				//����ɐڐG�̗L�����m�F������
				if ((*it)->CheckHit(me)) {
					//����Ƀ_���[�W�̏������s�킹��
					BChara::AttackInfo at = { this->power,0,0 };
					(*it)->Received(this, at);
					//�V���b�g�̂ݏ���
					//�i���͕����̂ɂ�����
					if (this->eraseFlag)
					{
						//�Ή������q�b�g���̃G�t�F�N�g�𐶐�
						//����A�����ɂ͑Ώۂ̓G�̍��W�������
						this->Effect_Hit((*it)->pos);
						this->Kill();
					}
					else
					{
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
		//�ˌ��͕ǂɓ�����Ə��ł���
		if (this->eraseFlag)
		{
			if (auto map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v")) 
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
		if (this->moveCnt >= this->cntLimit)
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
		//�V���b�g�Ƃ��ČĂ΂ꂽ�Ƃ��A�e�̕`��
		if (this->state == Shoot ||
			this->state == Jumpshoot ||
			this->state == Fallshoot)
		{
			ML::Box2D src(0, 224, 32, 32);
			DG::Image_Draw(this->res->imageName, draw, src, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
		}
		//�ȉ��U���͈͕\��
		if (ge->debugMode)
		{
			ML::Box2D srcDebug(0, 0, 32, 32);
			DG::Image_Draw(this->res->imageName, draw, srcDebug, ML::Color(0.5f, 1.0f, 0.0f, 0.0f));
		}
	}
	//�Ăяo�����^�X�N���������ݒ肷��
	void Object::Set_Limit(const int& cl_)
	{
		this->cntLimit = cl_;
	}
	//�ǂ�G�ɏՓ˂����Ƃ��A�����邩�ۂ����w�肷��
	void Object::Set_Erase(const int& erase_)
	{
		this->eraseFlag = erase_;
	}
	//�O�����琶������ہA�U���͂��w��
	//����	�F	�i�����l�j
	void Object::Set_Power(const int& pow_)
	{
		this->power = pow_;
	}
	//��Ԃ��Ƃɍs�����w�肷��
	void Object::Move()
	{
		auto pl = ge->GetTask_One_G <Player::Object>("�v���C��");
		switch (this->state)
		{
		default:
			break;
		case Punch1:
		case Punch2:
			//�G�ɏՓ˂����Ƃ������邩�ۂ�
			this->eraseFlag = false;
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
		case StompLanding:
			this->eraseFlag = false;
			if (pl->state == Damage)
			{
				this->Kill();
			}
			break;
		case Shoot:
			//�G�ɏՓ˂����Ƃ������邩�ۂ�
			this->eraseFlag = true;
			break;
		case Air:
			//�G�ɏՓ˂����Ƃ������邩�ۂ�
			this->eraseFlag = false;
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
			//�G�ɏՓ˂����Ƃ������邩�ۂ�
			this->eraseFlag = true;
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
		case StompLanding:
		{
			auto StompLandingHitEffect = Effect::Object::Create(true);
			//�͈͍U���͓G�̈ʒu����ɃG�t�F�N�g�𐶐�����
			StompLandingHitEffect->pos = pos_;
			StompLandingHitEffect->Set_Limit(18);
			StompLandingHitEffect->state = ImpactPunch;
			//�͈͍U���͒e�ƓG�Ƃ̈ʒu�֌W�ō��E�����߂�
			if (this->pos.x - pos_.x > 0)
			{
				StompLandingHitEffect->angle_LR = Left;
			}
			else
			{
				StompLandingHitEffect->angle_LR = Right;
			}
			break;
		}
		case Shoot:
		{
			auto ShootHitEffect = Effect::Object::Create(true);
			ShootHitEffect->pos = pos_;
			ShootHitEffect->Set_Limit(18);
			ShootHitEffect->state = ImpactPunch;
			ShootHitEffect->angle_LR = this->angle_LR;
			break;
		}
		case Jumpshoot:
		case Fallshoot:
		{
			auto AirshootHitEffect = Effect::Object::Create(true);
			AirshootHitEffect->pos = pos_;
			AirshootHitEffect->Set_Limit(18);
			AirshootHitEffect->state = ImpactPunch;
			AirshootHitEffect->angle_LR = this->angle_LR;
			break;
		}
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
//-------------------------------------------------------------------
//�v���C���̏o���e
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Shot00.h"
#include  "Task_Player.h"

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
		this->eraseFlag = true;
		this->hp = 0;				//hp�����l
		this->cntLimit = 0;			//���ł���܂ł̎���
		this->hitBase = ML::Box2D(-32, -32, 64, 64);

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
		this->moveCnt++;
		//���f�[�^���^�X�N���
		//���E�̎��Ԃ��}���������
		if (this->moveCnt >= this->cntLimit) {
			//���Ő\��
			this->Kill();
			return;
		}
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
					BChara::AttackInfo at = { this->hp,0,0 };
					(*it)->Received(this, at);
					//�V���b�g�̂ݏ���
					//�i���͕����̂ɂ�����
					if (this->eraseFlag)
					{
						this->Kill();
					}
					break;
				}
			}
		}
		//�ˌ��͕ǂɓ�����Ə��ł���
		if (this->eraseFlag)
		{
			if (auto map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v")) {
				ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
				if (true == map->CheckHit(hit)) {
					//���Ő\��
					this->Kill();
					//���Ŏ��A�G�t�F�N�g�𐶐�����ꍇ�͂����ɋL�q����
					return;
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
		ML::Box2D src(0, 0, 32, 32);
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
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
	//��Ԃ��Ƃɍs�����w�肷��
	void Object::Move()
	{
		auto pl = ge->GetTask_One_G <Player::Object>("�v���C��");
		switch (this->state)
		{
		default:
			break;
		case Punch1:
			//�G�ɏՓ˂����Ƃ������邩�ۂ�
			this->eraseFlag = false;
			//�p���`���̓v���C���̓����ɍ��킹�Ĕ����`���O�i����
			this->moveVec.x = pl->moveVec.x;
			//�v���C�����ǂɏՓ˂�����ړ��ʂ�0��
			if (pl->CheckFront_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case Punch2:
			//�G�ɏՓ˂����Ƃ������邩�ۂ�
			this->eraseFlag = false;
			//�p���`���̓v���C���̓����ɍ��킹�Ĕ����`���O�i����
			this->moveVec.x = pl->moveVec.x;
			//�v���C�����ǂɏՓ˂�����ړ��ʂ�0��
			if (pl->CheckFront_LR())
			{
				this->moveVec.x = 0.0f;
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
			this->moveVec.x = pl->moveVec.x;
			//�v���C�����ǂɏՓ˂�����ړ��ʂ�0��
			if (pl->CheckFront_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case Airshoot:
			//�G�ɏՓ˂����Ƃ������邩�ۂ�
			this->eraseFlag = true;
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
#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�{�X�i�A�b�p�[�j
//-------------------------------------------------------------------
#include "BChara.h"
#include "Boss.h"
#include "Task_Shot01.h"

namespace  Boss_Upper
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�{�X");		//�O���[�v��
	const  string  defName("NoName");		//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	//�ύX������������������������������������������������������
		//���L����ϐ��͂����ɒǉ�����
		string name_image;	//�摜���\�[�X
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
		//���h��
		float speed_shake;		//���x���h��
		float std_pos_x;		//���h���l
		float cnt_shake;		//�J�E���^���h��
		float interval_shake;	//�Ԋu���h��
		//�V���b�g����
		int cnt_move;			//�J�E���^�s��
		int interval_shot;		//�������ԃV���b�g
		//�V���b�g���
		ML::Vec2 vec_shot;		//�ړ��ʃV���b�g
		ML::Box2D hit_shot;		//��`�V���b�g
		//�C���X�^���X
		Boss* boss;				//�{�X�N���X�|�C���^
		Shot01::Object* shot;	//�V���b�g�I�u�W�F�N�g�|�C���^
	public:
	};
}
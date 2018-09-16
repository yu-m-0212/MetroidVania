#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Player
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�v���C��");		//�O���[�v��
	const  string  defName("��");				//�^�X�N��
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
		//���L����ϐ��͂����ɒǉ�����
		string		imageName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	//�ύX�s����������������������������������������������������
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
	//�ǉ��ϐ�
		string controllerName;
		float max_StompFallSpeed;	//�X�g���v���̍ő���ʑ��x
		float shotSpeed;			//�V���b�g���x
		int addUnHitTime;			//��e���ɓ����閳�G����
		int interval_Shot;			//�ˌ��̔��ˊԊu�i�t���[���j
		int limit_JumpAngleChange;	//�W�����v���A�X�e�B�b�N��|�����Ƃ���莞�ԓ��Ȃ������ς�����
		int limit_Stomp;			//�p�����ԃX�g���v
		int limit_StompEffect;		//�p�����ԃX�g���v����
		int limit_StompHoldTime;	//�d�����ԃX�g���v
		int limit_Shot;				//�p�����ԃV���b�g
		int limit_HealEffect;		//�p�����ԉ񕜌���
		int power_Stomp;			//�U���̓X�g���v
		int power_Shot;				//�U���̓V���b�g
		ML::Box2D range_Stomp;		//�͈̓X�g���v
		ML::Box2D range_Shot;		//�͈̓V���b�g
		ML::Vec2 moveBack_Stomp;	//�ӂ��ƂїʃX�g���v
	public:
		//�v�l���󋵔��f(�X�e�[�^�X����j
		void  Think();
		//���[�V�����ɑΉ���������
		void  Move();
		//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
		//����	�F	(�U����,�U�����)
		void Received(BChara* from_, AttackInfo at_);
		//HP�̒l���擾����
		int Get_HP();
		//HP�̍ő�l���擾����
		int Get_Max_HP();
		//�A�j���[�V��������
		BChara::DrawInfo  Anim();
		//�V���b�g�����X�^���_�[�h
		void Shot_Appear();
		//�s���V���b�g��
		void Move_Shot();
	};
}
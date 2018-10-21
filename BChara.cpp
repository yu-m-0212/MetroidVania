//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "BChara.h"
#include  "MyPG.h"
#include  "Task_Map2D.h"

//-----------------------------------------------------------------------------
//���[�V�������X�V�i�ύX�Ȃ��̏ꍇ	false)
bool  BChara::UpdateMotion(State  nm_)
{
	if (nm_ == this->state) {
		return  false;
	}
	else {
		this->state = nm_;		//���[�V�����ύX
		this->moveCnt = 0;		//�s���J�E���^�N���A
		this->animCnt = 0;		//�A�j���[�V�����J�E���^�̃N���A
		return  true;
	}
}
//-----------------------------------------------------------------------------
//����ڐG����
bool  BChara::CheckHead()
{
	//�����蔻�����ɂ��ē����`�𐶐�
	ML::Box2D  head(this->hitBase.x,
		this->hitBase.y - 1,
		this->hitBase.w,
		1);
	head.Offset((int)this->pos.x, (int)this->pos.y);


	auto   map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
	if (nullptr == map) { return false; }//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j
	return map->CheckHit(head);
}
//-----------------------------------------------------------------------------
//�߂荞�܂Ȃ��ړ�����
void BChara::CheckMove(ML::Vec2&  e_)
{
	//�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
	auto   map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
	if (nullptr == map) { return; }//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j

										  //�����ɑ΂���ړ�
	while (e_.x != 0) {
		float  preX = this->pos.x;
		if (e_.x >= 1) { this->pos.x += 1;		e_.x -= 1; }
		else if (e_.x <= -1) { this->pos.x -= 1;		e_.x += 1; }
		else { this->pos.x += e_.x;		e_.x = 0; }
		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
		if (true == map->CheckHit(hit)) {
			this->pos.x = preX;		//�ړ����L�����Z��
			break;
		}
	}
	//�c���ɑ΂���ړ�
	while (e_.y != 0) {
		float  preY = this->pos.y;
		if (e_.y >= 1) { this->pos.y += 1;		e_.y -= 1; }
		else if (e_.y <= -1) { this->pos.y -= 1;		e_.y += 1; }
		else { this->pos.y += e_.y;		e_.y = 0; }
		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
		if (true == map->CheckHit(hit)) {
			this->pos.y = preY;		//�ړ����L�����Z��
			break;
		}
	}
}
//-----------------------------------------------------------------------------
//�����ڐG����
bool  BChara::CheckFoot()
{
	//�����蔻�����ɂ��đ�����`�𐶐�
	ML::Box2D  foot(this->hitBase.x,
		this->hitBase.y + this->hitBase.h,
		this->hitBase.w,
		1);
	foot.Offset(this->pos);

	auto   map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
	if (nullptr == map) { return false; }//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j
	return map->CheckHit(foot);
}
//-----------------------------------------------------------------------------
//���ʐڐG����i�T�C�h�r���[�Q�[����p�j
bool BChara::CheckFront_LR()
{
	//���ʂ̋�`�Ɏg�p����ϐ���錾
	int x = 0;
	if (this->angle_LR == Left) { x = this->hitBase.x - 1; }			  //�L�������������̎�
	else                        { x = this->hitBase.x + this->hitBase.w; }//�L�������E�����̎�
	//�����蔻�����ɂ��Đ��ʋ�`�𐶐�
	ML::Box2D  front(x,this->hitBase.y,1,this->hitBase.h);
	front.Offset(this->pos);

	auto   map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
	if (nullptr == map) { return false; }//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j
	return map->CheckHit(front);
}
//�w�ʐڐG����i�T�C�h�r���[�Q�[����p�j
bool BChara::CheckBack_LR()
{
	//�w�ʂ̋�`�Ɏg�p����ϐ���錾
	int x = 0;
	if (this->angle_LR == Left) { x = this->hitBase.x + this->hitBase.w; }//�L�������������̎�
	else						{ x = this->hitBase.x - 1; }			  //�L�������E�����̎�
	//�����蔻�����ɂ��Ĕw�ʋ�`�𐶐�
	ML::Box2D back(x, this->hitBase.y, 1, this->hitBase.h);
	back.Offset(this->pos);

	auto   map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
	if (nullptr == map) { return false; }//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j
	return map->CheckHit(back);
}
//-----------------------------------------------------------------------------
//���ʑ�������i�T�C�h�r���[�Q�[����p�j
bool BChara::CheckFrontFoot_LR()
{
	//���ʑ����̋�`�Ɏg�p����ϐ���錾
	int x = 0;
	if (this->angle_LR == Left) { x = this->hitBase.x - 1; }//�L�������������̎�
	else { x = this->hitBase.x + this->hitBase.w; }			//�L�������E�����̎�
	//�����蔻�����ɂ��Đ��ʑ�����`�𐶐�
	ML::Box2D  frontFoot(x, this->hitBase.y+this->hitBase.h, 1, 1);
	frontFoot.Offset(this->pos);

	auto   map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
	if (nullptr == map) { return false; }//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j
	return map->CheckHit(frontFoot);
}
//-----------------------------------------------------------------------------
//�ڐG����
bool BChara::CheckHit(const ML::Box2D& hit_)
{
	//�L�����N�^�Ƃ̔���͈͂ɕύX
	ML::Box2D me = this->recieveBase.OffsetCopy(this->pos);
	return me.Hit(hit_);
}
//-----------------------------------------------------------------------------
//�A�N�Z�T���\�b�h

//��Ԃ��w�肷��
//����	�F	�iState�^�j
void BChara::Set_State(const State& state_)
{
	this->state = state_;
}
//�͈͍U�����ۂ����w�肷��
//����	�F	�i0��1�j
void BChara::Set_Range_Wide(const int& wide_)
{
	this->range_Wide = wide_;
}
//�m�b�N�o�b�N���邩�ۂ����w�肷��
//����	�F	�i0��1�j
void BChara::Set_Tip(const int& tip_)
{
	this->tip = tip_;
}
//�͈͍U�����ۂ����擾����
bool BChara::Get_Range_Wide()
{
	return this->range_Wide;
}
//�m�b�N�o�b�N���邩�ۂ����擾����
bool BChara::Get_Tip()
{
	return this->tip;
}
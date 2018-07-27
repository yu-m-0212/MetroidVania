#include "DG2014_Ver3_51.h"

namespace DG_
{
	unordered_map<string, Image::WP>	 Image::rep;
	bool  Image::dt;

	Image::Image( )
	:tex( ), angle(0), center(0,0)
	{
	}
	//-----------------------------------------------------------------------
	bool  Image::Initialize(const  string&  fpath_)
	{
		auto  dgi = DG_::DGObject::GetInst( );	if(dgi == 0){	return false; }
		//
		tex = Texture::Create(fpath_);
		if(!tex){		return false;	}
		sw = 1.0f / tex->Desc( ).Width;
		sh = 1.0f / tex->Desc( ).Height;
		return true;
	}
	//-----------------------------------------------------------------------
	Image::~Image( )
	{
		tex.reset( );
	}
	//-----------------------------------------------------------------------
	//��������
	Image::SP Image::Create(const string&  fpath_)
	{
		auto it = rep.find( fpath_ );
		//���ɓǍ��ς݂̃f�[�^������ꍇ
		if(it != rep.end( )){
			if(Image::SP sp = (*it).second.lock( )){
				return sp;
			}
		}
		//�V�K�Ǎ����K�v�ȏꍇ
		if( Image::SP sp = CreateRep(fpath_) ){
			rep[fpath_] = sp;
			return sp;
		}
		return nullptr;
	}
	//�d������摜��ʓr�ǂݍ���
	Image::SP Image::CreateRep(const string&  fpath_)
	{
		if( Image::SP  ob = Image::SP(new Image( )) ){
			if( ob->Initialize(fpath_) ){	return ob;	}
		}
		return nullptr;
	}
	//-----------------------------------------------------------------------
	////�o�b�t�@�̓��e��`�悷��
	//void  Image::Draw( )
	//{
	//	auto  dgi = DG_::DGObject::GetInst( );	if(dgi == 0){	return; }
	//	dgi->Flush2D( );
	//}
	//�o�b�t�@�ɓo�^����
	void  Image::Draw(const ML::Box2D&  d_, const ML::Box2D&  s_, const ML::Color&  c_)
	{
		dt = true;
		//SRC���e�N�X�`��UV���W�ɕϊ�
		float  u = s_.x * sw,  v = s_.y * sh,  w = s_.w * sw,  h = s_.h * sh;

		float  dx = (float)d_.x, dy = (float)d_.y, dw = (float)d_.w, dh = (float)d_.h;
		float  dwh = dw * 0.5f, dhh = dh * 0.5f;
		ML::Vec2 scale(dw, dh);									//	�X�P�[�����O�W���i�{�����w��j
		ML::Vec2 drawPos(dx + dwh, -(dy + dhh));				//	�`�����W
		ML::Vec2 rotCentr(center.x - dwh, -(center.y - dhh));	//	��]�����W

		D3DX10_SPRITE  dd;
		dd.ColorModulate = c_.D3D( );
		dd.TexCoord = ML::Vec2(u, v);
		dd.TexSize  = ML::Vec2(w, h);
		dd.pTexture = this->tex->tex_srv;
		dd.TextureIndex = 0;
		D3DXMatrixTransformation2D(
				&dd.matWorld, nullptr, 0.0f, &scale,
				&rotCentr, -angle, &drawPos);

		auto  dgi = DG_::DGObject::GetInst( );	if(dgi == 0){	return; }
		dgi->PushBack_D3DX10_SPRITE( dd );
	}
	//��]
	void  Image::Rotation(float  a_, const ML::Vec2&  c_)
	{
		angle = a_;
		center = c_;
	}
	//�T�C�Y�擾
	POINT  Image::Size( )
	{
		POINT  r = {this->tex->Desc( ).Width, this->tex->Desc( ).Height};
		return  r;
	}
	//-----------------------------------------------------------------------
}

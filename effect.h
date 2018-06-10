#pragma once
#include "interface.h"

float randomFire(float r);

struct Ef02 : IEffect {
	// �g�p����ϐ�������
	//Array<Vec2> effectPos; // �z��(���ϓ����邱�Ƃ�z�肵�ĉϒ��z��)
	
	Vec2 effectPos;

	std::unique_ptr<Anime> attackEffect;
							// �R���X�g���N�^�[
	Ef02(const Vec2& pos);

	// �X�V�֐� ����t�͎���(�b��)
	bool update(double t) override;


};

struct Efplayer : IEffect {
	// �g�p����ϐ�������
	Array<Vec2> effectPos; // �z��(���ϓ����邱�Ƃ�z�肵�ĉϒ��z��)

						   // �R���X�g���N�^�[
	Efplayer(const Vec2& pos);

	// �X�V�֐� ����t�͎���(�b��)
	bool update(double t) override;

	// ���W���ړ�������
	void movePos();


};

struct Fire : IEffect {
	
	Fire();

	bool update(double t) override;

};

struct TextEffect : IEffect{
	//�\�����鐔�l����
	int m_value;


	//�\������ʒu����
	const Vec2 m_position;

	TextEffect(int value, const Vec2& position)
		:m_value(value)
		, m_position(position) 
		
	{}

	bool update(double t) override
	{
		// 1.0 �b�ŏ�����
		if (t >= 1.0 )
		{
			return false;
		}

		if (t <= 0.5) {
			FontAsset(L"font").regionCenter(Format(999), m_position + Vec2(0, -30)).draw(Palette::Black);
			FontAsset(L"font").drawCenter(Format(m_value), m_position + Vec2(0, -30), HSV(60 - m_value));
		}
		else {
			//0.5�b�ȏ�o�߂ňÂ��Ȃ�B
			FontAsset(L"font").regionCenter(Format(999), m_position + Vec2(0, -30)).draw(Palette::Black);
			FontAsset(L"font").drawCenter(Format(m_value), m_position + Vec2(0, -30), HSV(60 - m_value).toColorF(1-2*(t-0.5)));
		}
		return true;
	}
	
};
;


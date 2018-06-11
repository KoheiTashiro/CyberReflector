#pragma once
#include "interface.h"

class IPause;

//-------------------------------------------------------
// PauseManager
//-------------------------------------------------------

//�V���O���g���Ŏ���
class PauseManager final : Uncopyable
{

	//�����[���I���Ȃ��͔���J�����o�ɃA�N�Z�X�ł���t�����Y�Ȃ񂾂ˁI
	friend IPause;
private:
	PauseManager() = default;

	//�Ǘ����郊�X�g
	std::list<IPause*> m_pauseList;

	void pauseAll();
	void resumeAll();

	//���X�g�փf�[�^�̒ǉ�
	void add(IPause*const pause);

	//���X�g����f�[�^�̍폜
	void erase(const IPause*const pause);


	static PauseManager& Instance();

	static void Add(IPause*const pause);

	static void Erase(const IPause*const pause);

public:

	//���ׂă|�[�Y
	static void Pause();

	//���ׂă��W���[��
	//static void Resume();

	//���ׂăA�b�v�f�[�g
	static void update();
};

//-------------------------------------------------------
// IPause
//-------------------------------------------------------

//�|�[�Y���������N���X�͂�����p��
class IPause
{
public:
	IPause();

	virtual ~IPause();

	virtual void pause() = 0;
	//virtual void resume() = 0;
	virtual void update() = 0;

};

//-------------------------------------------------------


//�e���v���[�g�͎����ɂ����Ȃ��炵��
//���x����`����ăG���[�͂��Ȃ��񂩂ȁH
template<class Type>
class Pauseable :
	public IPause,
	public Type
{
public:
	//�R���X�g���N�^�̈��p��
	using Type::Type;

	void pause()override
	{
		this->Type::pause();
	}

	/*void resume()override
	{
		this->Type::resume();
	}*/

	void update()override
	{
		this->Type::resume();
	}
};

#pragma once
/*
//#include "interface.h"

void PauseManager::add(IPause*const pause)
{
	m_pauseList.emplace_back(pause);
}


void PauseManager::erase(const IPause*const pause)
{
	s3d::Erase(m_pauseList, pause);
}

PauseManager& PauseManager::Instance()
{
	static PauseManager instance;
	return instance;
}

void PauseManager::Add(IPause*const pause)
{
	Instance().add(pause);
}

void PauseManager::Erase(const IPause*const pause)
{
	Instance().erase(pause);
}

//すべてポーズ
void PauseManager::Pause()
{
	Instance().pauseAll();
}

//すべてリジューム
void PauseManager::update()
{
	Instance().resumeAll();
}

IPause::IPause()
{
	PauseManager::Add(this);
}

IPause::~IPause()
{
	PauseManager::Erase(this);
}

//すべてポーズ
void PauseManager::pauseAll()
{
	for (auto&& elm : m_pauseList)
	{
		elm->pause();
	}
}

//すべてリジューム
void PauseManager::resumeAll()
{
	for (auto&& elm : m_pauseList)
	{
		elm->update();
	}
}
*/
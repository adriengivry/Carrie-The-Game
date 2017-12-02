#include "State_Game.h"
#include "StateManager.h"
#include "Utilities.h"
#include "Player.h"
#include "Enemy.h"

State_Game::State_Game(StateManager* l_stateManager) :
	BaseState(l_stateManager) {}

State_Game::~State_Game() {}

void State_Game::OnCreate()
{
	// Create quick access variable to shared managers
	LOAD_UTILITY()

	// OnCreate core
	++gameInfo->m_gameStarted;
	gameInfo->m_gameOver = false;

	textureManager->RequireResource("Game_Bg");

	m_backgroundSprite.setTexture(*textureManager->GetResource("Game_Bg"));

	actorManager->SetPlayer(new Player(m_stateMgr->GetContext(), 1200, 500));
	actorManager->SetNpc(new Npc(m_stateMgr->GetContext(), 1600, 250));

	// Adding callbacks
	evMgr->AddCallback(StateType::Game, "Key_Escape", &State_Game::MainMenu, this);
	evMgr->AddCallback(StateType::Game, "Key_P", &State_Game::Pause, this);
}

void State_Game::OnDestroy()
{
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Game,"Key_Escape");
	evMgr->RemoveCallback(StateType::Game,"Key_P");

	ActorManager* actorManager = m_stateMgr->GetContext()->m_actorManager;
	actorManager->CleanActors();
}

void State_Game::Update(const sf::Time& l_time)
{
	const sf::Vector2u l_windSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();
	ActorManager* actorManager = m_stateMgr->GetContext()->m_actorManager;

	if (!m_stateMgr->GetContext()->m_eventManager->IsFocused())
		Pause(nullptr);
	
	actorManager->Update(l_time);

	if (m_stateMgr->GetContext()->m_gameInfo->m_gameOver)
		GameOver();
}

void State_Game::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();
	ActorManager* actorManager = m_stateMgr->GetContext()->m_actorManager;

	window->draw(m_backgroundSprite);
	
	actorManager->Draw();
}

void State_Game::MainMenu(EventDetails* l_details) const
{
	ActorManager* actorManager = m_stateMgr->GetContext()->m_actorManager;

	actorManager->GetPlayer()->StopControl();
	m_stateMgr->SwitchTo(StateType::MainMenu); 
}

void State_Game::Pause(EventDetails* l_details) const
{
	ActorManager* actorManager = m_stateMgr->GetContext()->m_actorManager;

	actorManager->GetPlayer()->StopControl();
	m_stateMgr->SwitchTo(StateType::Paused);
}

void State_Game::Activate()
{
	SoundManager* soundManager = m_stateMgr->GetContext()->m_soundManager;
	soundManager->Pause("Menu");
	soundManager->PlayMusic("Game", true, true);
}
void State_Game::Deactivate() {}

void State_Game::GameOver() const
{
	m_stateMgr->SwitchTo(StateType::GameOver);
	m_stateMgr->Remove(StateType::Game);
}

#include "State_Game.h"
#include "StateManager.h"
#include "Utilities.h"

State_Game::State_Game(StateManager* l_stateManager) :
	BaseState(l_stateManager),
	m_actorManager(nullptr) {}

State_Game::~State_Game() {}

void State_Game::OnCreate()
{
	// Create quick access variable to shared managers
	LOAD_UTILITY()

	// OnCreate core
	++gameInfo->m_gameStarted;

	textureManager->RequireResource("Game_Bg");
	textureManager->RequireResource("Player");

	m_backgroundSprite.setTexture(*textureManager->GetResource("Game_Bg"));

	m_actorManager = new ActorManager(m_stateMgr->GetContext());
	m_actorManager->CreateActors();

	// Adding callbacks
	evMgr->AddCallback(StateType::Game, "Key_Escape", &State_Game::MainMenu, this);
	evMgr->AddCallback(StateType::Game, "Key_P", &State_Game::Pause, this);
}

void State_Game::OnDestroy()
{
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Game,"Key_Escape");
	evMgr->RemoveCallback(StateType::Game,"Key_P");

	delete m_actorManager;
}

void State_Game::Update(const sf::Time& l_time)
{
	const sf::Vector2u l_windSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();

	if (!m_stateMgr->GetContext()->m_eventManager->IsFocused())
		Pause(nullptr);
	
	m_actorManager->Update(l_time);

	std::vector<Actor*>& actors = m_actorManager->GetActors();

	for (auto it : actors)
		if (m_actorManager->GetPlayer()->IsIntersecting(it))
			GameOver();
}

void State_Game::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();

	window->draw(m_backgroundSprite);
	
	m_actorManager->Draw();
}

void State_Game::MainMenu(EventDetails* l_details) const
{
	m_actorManager->GetPlayer()->StopControl();
	m_stateMgr->SwitchTo(StateType::MainMenu); 
}

void State_Game::Pause(EventDetails* l_details) const
{
	m_actorManager->GetPlayer()->StopControl();
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

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
	gameInfo->Reset();

	textureManager->RequireResource("Game_Bg");

	m_backgroundSprite.setTexture(*textureManager->GetResource("Game_Bg"));

	actorManager->SetPlayer(new Player(m_stateMgr->GetContext(), windowCenter.x, 920));
	actorManager->SetNpc(new Npc(m_stateMgr->GetContext(), windowCenter.x, 250));
	actorManager->SetDoor(0, new Door(m_stateMgr->GetContext(), 559, 198));
	actorManager->SetDoor(1, new Door(m_stateMgr->GetContext(), 1362, 198));
	
	for (int i = 0; i < 3; ++i)
		actorManager->AddSpawnPoint(new SpawnPoint(m_stateMgr->GetContext()));

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

	if (LevelCompleted())
	{
		m_stateMgr->GetContext()->m_gameInfo->m_levelCompleted = true;
		actorManager->GetNpc()->Activate();
		actorManager->GetDoor(0)->Activate();
		actorManager->GetDoor(1)->Activate();
	}

	if (m_stateMgr->GetContext()->m_gameInfo->m_doorPassed)
	{
		OnDestroy();
		OnCreate();
	}
}

void State_Game::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();
	ActorManager* actorManager = m_stateMgr->GetContext()->m_actorManager;

	window->draw(m_backgroundSprite);
	
	actorManager->Draw();

	sf::Text levelLabel;
	levelLabel.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro"));
	levelLabel.setString("LEVEL " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_currentLevel));
	levelLabel.setPosition(window->getSize().x / 2, 50);
	Utils::centerOrigin(levelLabel);
	m_stateMgr->GetContext()->m_wind->GetRenderWindow()->draw(levelLabel);

	sf::Text spawnedEnemies;
	spawnedEnemies.setString("SPAWNED ENEMIES : " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_spawnedEnemies));
	spawnedEnemies.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro"));
	spawnedEnemies.setPosition(0, 0);

	sf::Text travelledDistance;
	travelledDistance.setString("TRAVELLED DISTANCE : " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_travelledDistance));
	travelledDistance.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro"));
	travelledDistance.setPosition(0, 50);

	sf::Text spawnedProjectiles;
	spawnedProjectiles.setString("SPAWNED PROJECTILES : " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_spawnedProjectiles));
	spawnedProjectiles.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro"));
	spawnedProjectiles.setPosition(0, 100);

	m_stateMgr->GetContext()->m_wind->GetRenderWindow()->draw(spawnedEnemies);
	m_stateMgr->GetContext()->m_wind->GetRenderWindow()->draw(spawnedProjectiles);
	m_stateMgr->GetContext()->m_wind->GetRenderWindow()->draw(travelledDistance);
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

bool State_Game::LevelCompleted() const
{
	std::vector<SpawnPoint*> spawnPoints = m_stateMgr->GetContext()->m_actorManager->GetSpawnPoints();
	std::vector<Enemy*> enemies = m_stateMgr->GetContext()->m_actorManager->GetEnemies();

	bool levelComplete = true;

	for (auto spawnPoint : spawnPoints)
		if (!spawnPoint->IsDone())
			levelComplete = false;

	if (enemies.size() != 0)
		levelComplete = false;

	return levelComplete;
}

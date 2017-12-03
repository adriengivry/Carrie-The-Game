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
	textureManager->RequireResource("Background_Edges");

	m_backgroundSprite.setTexture(*textureManager->GetResource("Game_Bg"));
	m_backgroundEdgesSprites.setTexture(*textureManager->GetResource("Background_Edges"));

	actorManager->SetPlayer(new Player(m_stateMgr->GetContext(), windowCenter.x, 920));
	actorManager->SetNpc(new Npc(m_stateMgr->GetContext(), windowCenter.x, 250));
	actorManager->SetDoor(0, new Door(m_stateMgr->GetContext(), 559, 198, true));
	actorManager->SetDoor(1, new Door(m_stateMgr->GetContext(), 1362, 198, false));
	
	for (int i = 0; i < 5; ++i)
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

	window->draw(m_backgroundEdgesSprites);

	sf::Text levelLabel;
	levelLabel.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro"));
	levelLabel.setString("LEVEL " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_currentLevel));
	levelLabel.setPosition(window->getSize().x / 2, 50);
	Utils::centerOrigin(levelLabel);
	window->draw(levelLabel);

	sf::RectangleShape consoleBackground;
	consoleBackground.setSize(sf::Vector2f(350, 200));
	consoleBackground.setPosition(0, 0);
	consoleBackground.setFillColor(sf::Color(0, 0, 0, 200));

	sf::Text spawnedEnemies;
	spawnedEnemies.setString("SPAWNED ENEMIES : " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_spawnedEnemies));
	spawnedEnemies.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Console"));
	spawnedEnemies.setCharacterSize(18);
	spawnedEnemies.setPosition(0, 0);

	sf::Text travelledDistance;
	travelledDistance.setString("TRAVELLED DISTANCE : " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_travelledDistance));
	travelledDistance.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Console"));
	travelledDistance.setCharacterSize(18);
	travelledDistance.setPosition(0, 20);

	sf::Text spawnedProjectiles;
	spawnedProjectiles.setString("SPAWNED PROJECTILES : " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_spawnedProjectiles));
	spawnedProjectiles.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Console"));
	spawnedProjectiles.setCharacterSize(18);
	spawnedProjectiles.setPosition(0, 40);

	// CURSES
	sf::Text reverseMove;
	reverseMove.setString("REVERSE MOVE : " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_reverseMovement));
	reverseMove.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Console"));
	reverseMove.setCharacterSize(18);
	reverseMove.setPosition(0, 80);

	sf::Text slowerCarrie;
	slowerCarrie.setString("SLOWER CARRIE : " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_slowerCarrie));
	slowerCarrie.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Console"));
	slowerCarrie.setCharacterSize(18);
	slowerCarrie.setPosition(0, 100);

	sf::Text slowerProjectiles;
	slowerProjectiles.setString("SLOWER PROJECTILES : " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_slowerProjectiles));
	slowerProjectiles.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Console"));
	slowerProjectiles.setCharacterSize(18);
	slowerProjectiles.setPosition(0, 120);

	sf::Text weakerProjectiles;
	weakerProjectiles.setString("WEAKER PROJECTILES : " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_weakerProjectiles));
	weakerProjectiles.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Console"));
	weakerProjectiles.setCharacterSize(18);
	weakerProjectiles.setPosition(0, 140);

	// CARRIE LIFE BAR
	sf::RectangleShape rect;
	sf::Vector2f barSize = sf::Vector2f(800, 30);
	rect.setPosition(window->getSize().x / 2, window->getSize().y - 35);
	rect.setFillColor(sf::Color::White);
	rect.setSize(barSize);
	Utils::centerOrigin(rect);
	window->draw(rect);
	rect.setFillColor(sf::Color::Red);
	barSize.x *= m_stateMgr->GetContext()->m_actorManager->GetPlayer()->GetLife() / m_stateMgr->GetContext()->m_actorManager->GetPlayer()->GetMaxLife();
	rect.setSize(barSize);
	window->draw(rect);

	// DRAW THINGS
	window->draw(consoleBackground);
	window->draw(spawnedEnemies);
	window->draw(spawnedProjectiles);
	window->draw(travelledDistance);

	window->draw(reverseMove);
	window->draw(slowerCarrie);
	window->draw(slowerProjectiles);
	window->draw(weakerProjectiles);
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

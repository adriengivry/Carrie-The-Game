#include "State_Game.h"
#include "StateManager.h"
#include "Utilities.h"
#include "Player.h"
#include "Enemy.h"

State_Game::State_Game(StateManager* l_stateManager) :
	BaseState(l_stateManager), m_whiteRectOpacity(255),
	m_sliderPauseTimer(0),
	m_startTransition(false),
	m_transitionEnd(false)
{
}

State_Game::~State_Game() {}

void State_Game::OnCreate()
{
	// Create quick access variable to shared managers
	LOAD_UTILITY()

	// OnCreate core
	++gameInfo->m_gameStarted;
	gameInfo->Reset();

	m_startTransition = false;
	m_transitionEnd = false;

	m_transitionSlider.setPosition(0, -1500);
	m_sliderPauseTimer = 0;

	m_whiteRectOpacity = 255;

	m_whiteRect.setPosition(0, 0);
	m_whiteRect.setSize(sf::Vector2f(windowSize.x, windowSize.y));
	m_whiteRect.setFillColor(sf::Color::White);

	m_curseText.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro"));
	m_curseText.setPosition(windowCenter.x, -650);
	m_curseText.setFillColor(sf::Color::Yellow);
	m_curseText.setCharacterSize(45);
	m_curseText.setString("");

	for (uint8_t i = 0; i < 4; ++i)
	{
		m_curseIcon[i].setPosition(windowCenter.x + 550 + 70 * i, windowSize.y - 40);
	}

	if (textureManager->RequireResource("Curse_Icon_Reverse"))
		m_curseIcon[0].setTexture(*textureManager->GetResource("Curse_Icon_Reverse"));

	if (textureManager->RequireResource("Curse_Icon_Slower_Carrie"))
		m_curseIcon[1].setTexture(*textureManager->GetResource("Curse_Icon_Slower_Carrie"));

	if (textureManager->RequireResource("Curse_Icon_Slower_Projectile"))
		m_curseIcon[2].setTexture(*textureManager->GetResource("Curse_Icon_Slower_Projectile"));

	if (textureManager->RequireResource("Curse_Icon_Weaker_Projectile"))
		m_curseIcon[3].setTexture(*textureManager->GetResource("Curse_Icon_Weaker_Projectile"));

	const MapType randomMap = static_cast<MapType>(Utils::randomgen(0, 1));
	gameInfo->m_mapType = randomMap;
	switch (randomMap)
	{
	default:
		textureManager->RequireResource("Map1_Background");
		textureManager->RequireResource("Map1_Edges");
		m_backgroundSprite.setTexture(*textureManager->GetResource("Map1_Background"));
		m_backgroundEdgesSprites.setTexture(*textureManager->GetResource("Map1_Edges"));
		break;

	case MapType::MAP1:
		textureManager->RequireResource("Map1_Background");
		textureManager->RequireResource("Map1_Edges");
		m_backgroundSprite.setTexture(*textureManager->GetResource("Map1_Background"));
		m_backgroundEdgesSprites.setTexture(*textureManager->GetResource("Map1_Edges"));
		break;

	case MapType::MAP2:
		textureManager->RequireResource("Map2_Background");
		textureManager->RequireResource("Map2_Edges");
		m_backgroundSprite.setTexture(*textureManager->GetResource("Map2_Background"));
		m_backgroundEdgesSprites.setTexture(*textureManager->GetResource("Map2_Edges"));
		break;
	}

	actorManager->SetPlayer(new Player(m_stateMgr->GetContext(), windowCenter.x, 920));
	actorManager->SetNpc(new Npc(m_stateMgr->GetContext(), windowCenter.x, 250));
	actorManager->SetDoor(0, new Door(m_stateMgr->GetContext(), 559, 198, true));
	actorManager->SetDoor(1, new Door(m_stateMgr->GetContext(), 1362, 198, false));

	const uint8_t numberOfSpawners = 3 + gameInfo->m_currentLevel / 4;
	
	for (int i = 0; i < numberOfSpawners; ++i)
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

	if (!m_startTransition)
		if (m_whiteRectOpacity > 0)
		{
			m_whiteRectOpacity -= 255 * l_time.asSeconds();
			if (m_whiteRectOpacity < 0)
				m_whiteRectOpacity = 0;
			sf::Color newColor = m_whiteRect.getFillColor();
			newColor.a = m_whiteRectOpacity;
			m_whiteRect.setFillColor(newColor);
		}

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

	if (m_stateMgr->GetContext()->m_gameInfo->m_doorPassed && !m_startTransition)
	{
		m_startTransition = true;
		if (m_stateMgr->GetContext()->m_gameInfo->m_getCursed)
		{
			switch (m_stateMgr->GetContext()->m_gameInfo->m_curseType)
			{
			default:
			case REVERSE_MOVEMENT:
				m_curseText.setString("YOUR KEYBOARD IS NOW FUCKED UP ;)");
				break;

			case SLOWER_CARRIE:
				m_curseText.setString("DO YOU LIKE GLUE? <3");
				break;

			case SLOWER_PROJECTILES:
				m_curseText.setString("TRY TO TO THROW IT FASTER!");
				break;

			case WEAKER_PROJECTILES:
				m_curseText.setString("OUPS! I BROKE YOUR CHOCOLATE SQUARE :)");
				break;
			}
			Utils::centerOrigin(m_curseText);

			if (m_stateMgr->GetContext()->m_textureManager->RequireResource("Curse_Slider"))
				m_transitionSlider.setTexture(*m_stateMgr->GetContext()->m_textureManager->GetResource("Curse_Slider"));
		}

		if (!m_stateMgr->GetContext()->m_gameInfo->m_getCursed)
			if (m_stateMgr->GetContext()->m_textureManager->RequireResource("Truth_Slider"))
				m_transitionSlider.setTexture(*m_stateMgr->GetContext()->m_textureManager->GetResource("Truth_Slider"));
	}

	if (m_startTransition)
	{
		if (m_transitionSlider.getPosition().y >= 0)
		{
			m_sliderPauseTimer += l_time.asSeconds();
		}
		else
		{
			m_transitionSlider.move(0, 1250 * l_time.asSeconds());
			m_curseText.move(0, 1250 * l_time.asSeconds());
			if (m_transitionSlider.getPosition().y >= 0)
				m_transitionSlider.setPosition(0, 0);
		}

		if (m_sliderPauseTimer >= 1.5f)
		{
			m_transitionSlider.move(0, -5000 * l_time.asSeconds());
			m_curseText.move(0, -5000 * l_time.asSeconds());
			if (m_transitionSlider.getPosition().y <= -1500)
				m_transitionEnd = true;
		}

		if (m_transitionEnd)
		{
			m_stateMgr->GetContext()->m_soundManager->PlayMusic("Game");
			OnDestroy();
			OnCreate();
		}
	}
}

void State_Game::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();
	ActorManager* actorManager = m_stateMgr->GetContext()->m_actorManager;

	window->draw(m_backgroundSprite);
	
	actorManager->Draw();

	DrawUserInterface();

	window->draw(m_whiteRect);

	if (m_startTransition)
	{
		if (m_sliderPauseTimer >= 1.5f)
		{
			m_whiteRect.setFillColor(sf::Color::White);
		}

		window->draw(m_transitionSlider);
		if (m_curseText.getString() != "")
			window->draw(m_curseText);
	}
}

void State_Game::DrawUserInterface()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();

	// CARRIE LIFE BAR
	sf::RectangleShape rect;
	sf::Vector2f barSize = sf::Vector2f(802, 60);
	rect.setPosition(window->getSize().x / 2, window->getSize().y - 45);
	rect.setFillColor(sf::Color::Black);
	rect.setSize(barSize);
	Utils::centerOrigin(rect);
	window->draw(rect);
	rect.setFillColor(sf::Color::Red);
	barSize.x *= m_stateMgr->GetContext()->m_actorManager->GetPlayer()->GetLife() / m_stateMgr->GetContext()->m_actorManager->GetPlayer()->GetMaxLife();
	rect.setSize(barSize);
	window->draw(rect);

	window->draw(m_backgroundEdgesSprites);

	sf::Sprite filter;
	if (m_stateMgr->GetContext()->m_textureManager->RequireResource("Filter"))
	{
		filter.setTexture(*m_stateMgr->GetContext()->m_textureManager->GetResource("Filter"));
		filter.setPosition(0, 0);
		filter.setColor(sf::Color(255, 0, 0, 255 - 200 * (m_stateMgr->GetContext()->m_actorManager->GetPlayer()->GetLife() / m_stateMgr->GetContext()->m_actorManager->GetPlayer()->GetMaxLife())));
		m_stateMgr->GetContext()->m_wind->GetRenderWindow()->draw(filter);
	}

	sf::Text levelLabel;
	levelLabel.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro"));
	levelLabel.setString("LEVEL " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_currentLevel));
	levelLabel.setFillColor(sf::Color::Black);
	levelLabel.setPosition(window->getSize().x / 2 - 600, window->getSize().y - 40);
	Utils::centerOrigin(levelLabel);
	window->draw(levelLabel);

	if (m_stateMgr->GetContext()->m_gameInfo->m_debugMode)
		DrawConsole();

	sf::Text curseStack;
	curseStack.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro"));
	curseStack.setFillColor(sf::Color::Black);
	curseStack.setCharacterSize(15);


	for (uint8_t i = 0; i < 4; ++i)
	{
		Utils::centerOrigin(m_curseIcon[i]);
		window->draw(m_curseIcon[i]);
		switch (i)
		{
		default:
			break;

		case REVERSE_MOVEMENT:
			curseStack.setString(std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_reverseMovement));
			break;

		case SLOWER_CARRIE:
			curseStack.setString(std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_slowerCarrie));
			break;

		case SLOWER_PROJECTILES:
			curseStack.setString(std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_slowerProjectiles));
			break;

		case WEAKER_PROJECTILES:
			curseStack.setString(std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_weakerProjectiles));
			break;
		}
		curseStack.setPosition(m_curseIcon[i].getPosition() + sf::Vector2f(-25, 0));
		curseStack.move(0, 20);
		sf::CircleShape badge;
		badge.setPosition(curseStack.getPosition());
		badge.setRadius(15);
		badge.setFillColor(sf::Color::White);
		badge.setOutlineColor(sf::Color::Black);
		badge.setOutlineThickness(2);
		Utils::centerOrigin(badge);
		window->draw(badge);
		Utils::centerOrigin(curseStack);
		window->draw(curseStack);
	}
}

void State_Game::DrawConsole() const
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();

	sf::RectangleShape consoleBackground;
	consoleBackground.setSize(sf::Vector2f(350, 80));
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

	// DRAW THINGS
	window->draw(consoleBackground);
	window->draw(spawnedEnemies);
	window->draw(spawnedProjectiles);
	window->draw(travelledDistance);
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

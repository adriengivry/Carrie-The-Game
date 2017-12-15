#include "State_Game.h"
#include "StateManager.h"
#include "Utilities.h"
#include "Player.h"
#include "Enemy.h"
#include "CakeKing.h"
#include "JellyQueen.h"
#include "CuringPotion.h"
#include "ExtraLife.h"
#include "SmallCuringPotion.h"
#include "PerforantChocolateUpgrade.h"
#include "TripleChocolateUpgrade.h"

State_Game::State_Game(StateManager* l_stateManager) :
	BaseState(l_stateManager),
	m_isShopingLevel(false), 
	m_currentLevel(0),
	m_whiteRectOpacity(255),
	m_framerate(0),
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

	if (m_stateMgr->GetContext()->m_fontManager->RequireResource("Retro"))
		m_levelEndText.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro"));

	m_levelEndText.setPosition(windowCenter.x, -650);
	m_levelEndText.setFillColor(sf::Color::Yellow);
	m_levelEndText.setCharacterSize(45);
	m_levelEndText.setString("");

	for (uint8_t i = 0; i < 5; ++i)
	{
		m_curseIcon[i].setPosition(windowCenter.x + 500 + 70 * i, windowSize.y - 40);
	}

	if (textureManager->RequireResource("Curse_Icon_Reverse"))
		m_curseIcon[0].setTexture(*textureManager->GetResource("Curse_Icon_Reverse"));

	if (textureManager->RequireResource("Curse_Icon_Slower_Carrie"))
		m_curseIcon[1].setTexture(*textureManager->GetResource("Curse_Icon_Slower_Carrie"));

	if (textureManager->RequireResource("Curse_Icon_Slower_Projectile"))
		m_curseIcon[2].setTexture(*textureManager->GetResource("Curse_Icon_Slower_Projectile"));

	if (textureManager->RequireResource("Curse_Icon_Weaker_Projectile"))
		m_curseIcon[3].setTexture(*textureManager->GetResource("Curse_Icon_Weaker_Projectile"));

	if (textureManager->RequireResource("Curse_Icon_Reduced_Precision"))
		m_curseIcon[4].setTexture(*textureManager->GetResource("Curse_Icon_Reduced_Precision"));

	bool isBossLevel = false;

	if (gameInfo->m_gameMode == GameMode::ARCADE)
		isBossLevel = gameInfo->m_currentLevel % 5 == 0;
	else if (gameInfo->m_gameMode == GameMode::BOSS_RUSH)
		isBossLevel = gameInfo->m_currentLevel % 1 == 0;

	m_isShopingLevel = gameInfo->m_bossBeaten;
	gameInfo->m_bossBeaten = false;

	if (isBossLevel || m_isShopingLevel)
	{
		gameInfo->m_mapType = MapType::MAP_BOSS;
	}
	else
	{
		if (gameInfo->m_currentLevel <= 5)
		{
			gameInfo->m_mapType = MapType::MAP1;
		}
		else if (gameInfo->m_currentLevel > 5 && gameInfo->m_currentLevel <= 10)
		{
			gameInfo->m_mapType = MapType::MAP2;
		}
		else
		{
			gameInfo->m_mapType = MapType::MAP3;
		}
	}

	switch (gameInfo->m_mapType)
	{
	default:
		if (textureManager->RequireResource("Map1_Background"))
			m_backgroundSprite.setTexture(*textureManager->GetResource("Map1_Background"));

		if (textureManager->RequireResource("Map1_Edges"))
			m_backgroundEdgesSprites.setTexture(*textureManager->GetResource("Map1_Edges"));
		break;

	case MapType::MAP1:
		if (textureManager->RequireResource("Map1_Background"))
			m_backgroundSprite.setTexture(*textureManager->GetResource("Map1_Background"));
		
		if (textureManager->RequireResource("Map1_Edges"))
			m_backgroundEdgesSprites.setTexture(*textureManager->GetResource("Map1_Edges"));
		break;

	case MapType::MAP2:
		if (textureManager->RequireResource("Map2_Background"))
			m_backgroundSprite.setTexture(*textureManager->GetResource("Map2_Background"));

		if (textureManager->RequireResource("Map2_Edges"))
			m_backgroundEdgesSprites.setTexture(*textureManager->GetResource("Map2_Edges"));
		break;

	case MapType::MAP3:
		if (textureManager->RequireResource("Map3_Background"))
			m_backgroundSprite.setTexture(*textureManager->GetResource("Map3_Background"));

		if (textureManager->RequireResource("Map3_Edges"))
			m_backgroundEdgesSprites.setTexture(*textureManager->GetResource("Map3_Edges"));
		break;

	case MapType::MAP_BOSS:
		if (textureManager->RequireResource("Map_Boss_Background"))
			m_backgroundSprite.setTexture(*textureManager->GetResource("Map_Boss_Background"));

		if (textureManager->RequireResource("Map_Boss_Edges"))
			m_backgroundEdgesSprites.setTexture(*textureManager->GetResource("Map_Boss_Edges"));
		break;
	}

	actorManager->SetPlayer(new Player(m_stateMgr->GetContext(), windowCenter.x, 920));

	if (m_isShopingLevel)
	{
		actorManager->SetDoor(0, new Door(m_stateMgr->GetContext(), windowCenter.x, 194, true, true));
		actorManager->AddBuyable(new ExtraLife(m_stateMgr->GetContext(), windowCenter.x - 600, windowCenter.y));
		actorManager->AddBuyable(new ExtraLife(m_stateMgr->GetContext(), windowCenter.x - 300, windowCenter.y));
		actorManager->AddBuyable(new SmallCuringPotion(m_stateMgr->GetContext(), windowCenter.x + 300, windowCenter.y));
		actorManager->AddBuyable(new CuringPotion(m_stateMgr->GetContext(), windowCenter.x + 600, windowCenter.y));
		actorManager->AddBuyable(new PerforantChocolateUpgrade(m_stateMgr->GetContext(), windowCenter.x + 600, windowCenter.y + 300));
		actorManager->AddBuyable(new TripleChocolateUpgrade(m_stateMgr->GetContext(), windowCenter.x - 600, windowCenter.y + 300));
		gameInfo->m_levelCompleted = true;
	}
	else
	{
		actorManager->SetNpc(new Npc(m_stateMgr->GetContext(), windowCenter.x, 275));
		actorManager->SetDoor(0, new Door(m_stateMgr->GetContext(), 559, 194, true));
		actorManager->SetDoor(1, new Door(m_stateMgr->GetContext(), 1362, 194, false));
		uint8_t numberOfSpawners = gameInfo->m_currentLevel / 2;
		if (isBossLevel)
		{
			numberOfSpawners += 1;
			switch (Utils::randomgen(0, 1))
			{
			default:
			case 0:
				actorManager->AddEnemy(new CakeKing(m_stateMgr->GetContext(), windowCenter.x, 250));
				for (int i = 0; i < numberOfSpawners; ++i)
					actorManager->AddSpawnPoint(new SpawnPoint(m_stateMgr->GetContext(), SpawnerType::CAKEMONSTER_SPAWNER));
				break;

			case 1:
				actorManager->AddEnemy(new JellyQueen(m_stateMgr->GetContext(), windowCenter.x, 250));
				for (int i = 0; i < numberOfSpawners; ++i)
					actorManager->AddSpawnPoint(new SpawnPoint(m_stateMgr->GetContext(), SpawnerType::JELLY_SPAWNER));
				break;
			}
		}
		else
		{
			numberOfSpawners += 3;
			for (int i = 0; i < numberOfSpawners; ++i)
				actorManager->AddSpawnPoint(new SpawnPoint(m_stateMgr->GetContext()));
		}
	}

	m_currentLevel = gameInfo->m_currentLevel;
	UpdateCurseDetails();

	// Adding callbacks
	evMgr->AddCallback(StateType::Game, "Key_Escape", &State_Game::MainMenu, this);
	evMgr->AddCallback(StateType::Game, "Key_P", &State_Game::Pause, this);
	evMgr->AddCallback(StateType::Game, "Key_K", &State_Game::KillEnemies, this);
	evMgr->AddCallback(StateType::Game, "Key_M", &State_Game::AddMoney, this);
	evMgr->AddCallback(StateType::Game, "Key_L", &State_Game::AddLife, this);
	evMgr->AddCallback(StateType::Game, "Key_C", &State_Game::AddCarrot, this);
	evMgr->AddCallback(StateType::Game, "Key_F1", &State_Game::ToggleDebugMode, this);
}

void State_Game::OnDestroy()
{
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Game, "Key_Escape");
	evMgr->RemoveCallback(StateType::Game, "Key_P");
	evMgr->RemoveCallback(StateType::Game, "Key_K");
	evMgr->RemoveCallback(StateType::Game, "Key_M");
	evMgr->RemoveCallback(StateType::Game, "Key_L");
	evMgr->RemoveCallback(StateType::Game, "Key_C");
	evMgr->RemoveCallback(StateType::Game, "Key_F1");

	ActorManager* actorManager = m_stateMgr->GetContext()->m_actorManager;
	actorManager->CleanActors();
}

void State_Game::Update(const sf::Time& l_time)
{
	const sf::Vector2u l_windSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();
	ActorManager* actorManager = m_stateMgr->GetContext()->m_actorManager;

	m_framerate = 1 / l_time.asSeconds();
	
	m_stateMgr->GetContext()->m_audioManager->SetMusicPitch("Game", 1 + (actorManager->GetPlayer()->GetMaxLife() - actorManager->GetPlayer()->GetLife()) / 500);

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

	if (!m_isShopingLevel)
	{
		if (LevelCompleted())
		{
			m_stateMgr->GetContext()->m_gameInfo->m_levelCompleted = true;
			actorManager->GetNpc()->Activate();
			actorManager->GetDoor(0)->Activate();
			actorManager->GetDoor(1)->Activate();
		}
		else
		{
			m_stateMgr->GetContext()->m_gameInfo->m_levelDuration += l_time.asSeconds();
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
					m_levelEndText.setString("YOUR KEYBOARD IS NOW FUCKED UP!");
					break;

				case SLOWER_CARRIE:
					m_levelEndText.setString("DO YOU LIKE GLUE?");
					break;

				case SLOWER_PROJECTILES:
					m_levelEndText.setString("TRY TO TO THROW IT FASTER!");
					break;

				case WEAKER_PROJECTILES:
					m_levelEndText.setString("OUPS! I BROKE YOUR CHOCOLATE SQUARE");
					break;

				case REDUCED_PRECISION:
					m_levelEndText.setString("ARE YOU DRUNK?");
					break;
				}

				if (m_stateMgr->GetContext()->m_textureManager->RequireResource("Curse_Slider"))
					m_transitionSlider.setTexture(*m_stateMgr->GetContext()->m_textureManager->GetResource("Curse_Slider"));
			}

			if (!m_stateMgr->GetContext()->m_gameInfo->m_getCursed)
			{
				if (m_stateMgr->GetContext()->m_textureManager->RequireResource("Truth_Slider"))
					m_transitionSlider.setTexture(*m_stateMgr->GetContext()->m_textureManager->GetResource("Truth_Slider"));

				if (m_stateMgr->GetContext()->m_gameInfo->m_goodAnswerCombo > 1)
					m_levelEndText.setString("+" + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_goodAnswerCombo) + " CARROTS FOR YOU!");
				else
					m_levelEndText.setString("+" + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_goodAnswerCombo) + " CARROT FOR YOU!");

				m_levelEndText.setFillColor(sf::Color(255, 69, 0));
				m_levelEndText.move(0, 100);
			}

			Utils::centerOrigin(m_levelEndText);
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
				m_levelEndText.move(0, 1250 * l_time.asSeconds());
				if (m_transitionSlider.getPosition().y >= 0)
					m_transitionSlider.setPosition(0, 0);
			}

			if (m_sliderPauseTimer >= 1.5f)
			{
				m_transitionSlider.move(0, -5000 * l_time.asSeconds());
				m_levelEndText.move(0, -5000 * l_time.asSeconds());
				if (m_transitionSlider.getPosition().y <= -1500)
					m_transitionEnd = true;
			}

			if (m_transitionEnd)
			{
				m_stateMgr->GetContext()->m_audioManager->PlayMusic("Game");
				OnDestroy();
				OnCreate();
			}
		}
	}
	else
	{
		if (m_stateMgr->GetContext()->m_gameInfo->m_doorPassed)
		{
			OnDestroy();
			OnCreate();
		}
	}

	if (m_isShopingLevel)
		UpdateCurseDetails();
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
		if (m_levelEndText.getString() != "")
			window->draw(m_levelEndText);
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
	rect.setFillColor(sf::Color(255, 150, 200));
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
	if (m_stateMgr->GetContext()->m_fontManager->RequireResource("Retro"))
		levelLabel.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro"));
	levelLabel.setString("LEVEL " + std::to_string(m_currentLevel));
	levelLabel.setFillColor(sf::Color::White);
	levelLabel.setPosition(window->getSize().x / 2 - 850, window->getSize().y - 40);
	Utils::centerOrigin(levelLabel);
	window->draw(levelLabel);

	// DRAW TOOTHPASTE INFOS
	sf::Sprite toothPasteSprite;
	if (m_stateMgr->GetContext()->m_textureManager->RequireResource("Toothpaste"))
	{
		toothPasteSprite.setTexture(*m_stateMgr->GetContext()->m_textureManager->GetResource("Toothpaste"));
		toothPasteSprite.setPosition(window->getSize().x / 2 - 700, window->getSize().y - 40);
		Utils::centerOrigin(toothPasteSprite);
	}
	window->draw(toothPasteSprite);

	sf::Text toothPasteCounter;
	if (m_stateMgr->GetContext()->m_fontManager->RequireResource("Retro"))
		toothPasteCounter.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro"));
	toothPasteCounter.setString("x" + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_toothPaste));
	toothPasteCounter.setFillColor(sf::Color::White);
	toothPasteCounter.setPosition(toothPasteSprite.getPosition());
	toothPasteCounter.move(toothPasteSprite.getGlobalBounds().width / 2, -toothPasteCounter.getGlobalBounds().height / 2);
	window->draw(toothPasteCounter);

	// DRAW CARROTS INFO
	sf::Sprite carrotSprite;
	if (m_stateMgr->GetContext()->m_textureManager->RequireResource("Carrot"))
	{
		carrotSprite.setTexture(*m_stateMgr->GetContext()->m_textureManager->GetResource("Carrot"));
		carrotSprite.setPosition(toothPasteCounter.getGlobalBounds().left + toothPasteCounter.getGlobalBounds().width + 100, window->getSize().y - 40);
		Utils::centerOrigin(carrotSprite);
	}
	window->draw(carrotSprite);

	sf::Text carrotCounter;
	if (m_stateMgr->GetContext()->m_fontManager->RequireResource("Retro"))
		carrotCounter.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro"));
	carrotCounter.setString("x" + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_carrots));
	carrotCounter.setFillColor(sf::Color::White);
	carrotCounter.setPosition(carrotSprite.getPosition());
	carrotCounter.move(carrotSprite.getGlobalBounds().width / 2, -carrotCounter.getGlobalBounds().height / 2);
	window->draw(carrotCounter);

	if (m_stateMgr->GetContext()->m_gameInfo->m_debugMode)
	{
		DrawConsole();
		DrawFPS();
	}

	sf::Text curseStack;
	if (m_stateMgr->GetContext()->m_fontManager->RequireResource("Retro"))
		curseStack.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro"));
	curseStack.setFillColor(sf::Color::Black);
	curseStack.setCharacterSize(18);


	for (uint8_t i = 0; i < 5; ++i)
	{
		bool isEmpty = false;

		Utils::centerOrigin(m_curseIcon[i]);

		switch (i)
		{
		default:
			break;

		case REVERSE_MOVEMENT:
			curseStack.setString(std::to_string(m_curseDetails.m_reverseMovement));
			isEmpty = m_curseDetails.m_reverseMovement == 0;
			break;

		case SLOWER_CARRIE:
			curseStack.setString(std::to_string(m_curseDetails.m_slowerCarrie));
			isEmpty = m_curseDetails.m_slowerCarrie == 0;
			break;

		case SLOWER_PROJECTILES:
			curseStack.setString(std::to_string(m_curseDetails.m_slowerProjectiles));
			isEmpty = m_curseDetails.m_slowerProjectiles == 0;
			break;

		case WEAKER_PROJECTILES:
			curseStack.setString(std::to_string(m_curseDetails.m_weakerProjectiles));
			isEmpty = m_curseDetails.m_weakerProjectiles == 0;
			break;

		case REDUCED_PRECISION:
			curseStack.setString(std::to_string(m_curseDetails.m_reducedPrecision));
			isEmpty = m_curseDetails.m_reducedPrecision == 0;
			break;
		}

		if (!isEmpty)
		{
			window->draw(m_curseIcon[i]);

			curseStack.setPosition(m_curseIcon[i].getPosition() + sf::Vector2f(-25, 0));
			curseStack.move(0, 20);
			sf::CircleShape badge;
			badge.setFillColor(sf::Color(255, 255, 255, 150));
			badge.setPosition(curseStack.getPosition());
			badge.setRadius(14);

			if (i != 0)
			{
				Utils::centerOrigin(badge);
				window->draw(badge);
				Utils::centerOrigin(curseStack);
				window->draw(curseStack);
			}
		}
	}
}

void State_Game::DrawConsole() const
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();

	sf::RectangleShape consoleBackground;
	consoleBackground.setSize(sf::Vector2f(350, 110));
	consoleBackground.setPosition(0, 0);
	consoleBackground.setFillColor(sf::Color(0, 0, 0, 200));

	sf::Text spawnedEnemies;
	spawnedEnemies.setString("SPAWNED ENEMIES : " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_spawnedEnemies));
	if (m_stateMgr->GetContext()->m_fontManager->RequireResource("Console"))
		spawnedEnemies.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Console"));
	spawnedEnemies.setCharacterSize(18);
	spawnedEnemies.setPosition(0, 0);

	sf::Text travelledDistance;
	travelledDistance.setString("TRAVELLED DISTANCE : " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_travelledDistance));
	if (m_stateMgr->GetContext()->m_fontManager->RequireResource("Console"))
		travelledDistance.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Console"));
	travelledDistance.setCharacterSize(18);
	travelledDistance.setPosition(0, 20);

	sf::Text spawnedProjectiles;
	spawnedProjectiles.setString("SPAWNED PROJECTILES : " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_spawnedProjectiles));
	if (m_stateMgr->GetContext()->m_fontManager->RequireResource("Console"))
		spawnedProjectiles.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Console"));
	spawnedProjectiles.setCharacterSize(18);
	spawnedProjectiles.setPosition(0, 40);

	sf::Text levelDuration;
	levelDuration.setString("LEVEL DURATION: " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_levelDuration));
	if (m_stateMgr->GetContext()->m_fontManager->RequireResource("Console"))
		levelDuration.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Console"));
	levelDuration.setCharacterSize(18);
	levelDuration.setPosition(0, 60);

	sf::Text spawnedSpawners;
	spawnedSpawners.setString("SPAWNED SPAWNERS: " + std::to_string(m_stateMgr->GetContext()->m_gameInfo->m_spawnedSpawnPoints));
	if (m_stateMgr->GetContext()->m_fontManager->RequireResource("Console"))
		spawnedSpawners.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Console"));
	spawnedSpawners.setCharacterSize(18);
	spawnedSpawners.setPosition(0, 80);
	

	// DRAW THINGS
	window->draw(consoleBackground);
	window->draw(spawnedEnemies);
	window->draw(spawnedProjectiles);
	window->draw(travelledDistance);
	window->draw(levelDuration);
	window->draw(spawnedSpawners);
}

void State_Game::DrawFPS() const
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();

	sf::Text fpsCounter;
	if (m_stateMgr->GetContext()->m_fontManager->RequireResource("Console"))
		fpsCounter.setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Console"));
	fpsCounter.setCharacterSize(25);
	fpsCounter.setString(std::to_string(m_framerate) + " FPS");
	fpsCounter.setPosition(window->getSize().x - 120, 20);

	window->draw(fpsCounter);
}

void State_Game::UpdateCurseDetails()
{
	GameInfo* gameInfo = m_stateMgr->GetContext()->m_gameInfo;

	m_curseDetails.m_reverseMovement = gameInfo->m_reverseMovement;
	m_curseDetails.m_slowerCarrie = gameInfo->m_slowerCarrie;
	m_curseDetails.m_slowerProjectiles = gameInfo->m_slowerProjectiles;
	m_curseDetails.m_weakerProjectiles = gameInfo->m_weakerProjectiles;
	m_curseDetails.m_reducedPrecision = gameInfo->m_reducedPrecision;
}

void State_Game::MainMenu(EventDetails* l_details) const
{
	if (CanInteract())
	{
		ActorManager* actorManager = m_stateMgr->GetContext()->m_actorManager;

		actorManager->GetPlayer()->StopControl();
		m_stateMgr->SwitchTo(StateType::MainMenu);
	}
}

void State_Game::Pause(EventDetails* l_details) const
{
	if (CanInteract())
	{
		ActorManager* actorManager = m_stateMgr->GetContext()->m_actorManager;

		actorManager->GetPlayer()->StopControl();
		m_stateMgr->SwitchTo(StateType::Paused);
	}
}

void State_Game::KillEnemies(EventDetails* l_details) const
{
	if (m_stateMgr->GetContext()->m_gameInfo->m_debugMode)
		m_stateMgr->GetContext()->m_actorManager->CleanEnemies();
}

void State_Game::AddMoney(EventDetails* l_details) const
{
	if (m_stateMgr->GetContext()->m_gameInfo->m_debugMode)
		m_stateMgr->GetContext()->m_gameInfo->m_toothPaste += 50;
}

void State_Game::AddLife(EventDetails* l_details) const
{
	if (m_stateMgr->GetContext()->m_gameInfo->m_debugMode)
		m_stateMgr->GetContext()->m_actorManager->GetPlayer()->AddLife(20);
}

void State_Game::AddCarrot(EventDetails* l_details) const
{
	if (m_stateMgr->GetContext()->m_gameInfo->m_debugMode)
		m_stateMgr->GetContext()->m_gameInfo->m_carrots += 1;
}

void State_Game::ToggleDebugMode(EventDetails* l_details) const
{
	#ifdef _DEBUG
		m_stateMgr->GetContext()->m_gameInfo->m_debugMode = !m_stateMgr->GetContext()->m_gameInfo->m_debugMode;
	#endif
}

void State_Game::Activate()
{
	AudioManager* audioManager = m_stateMgr->GetContext()->m_audioManager;
	audioManager->Pause("Menu");
	audioManager->PlayMusic("Game");
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

bool State_Game::CanInteract() const
{
	return !m_startTransition;
}

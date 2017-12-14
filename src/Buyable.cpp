#include "Buyable.h"
#include "StateManager.h"


Buyable::Buyable(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Actor(p_sharedContext, p_x, p_y)
{
	m_alreadyBought = false;
	m_itemInfo.m_name = "NO_NAME";
	m_itemInfo.m_price = 0;

	if (m_sharedContext->m_textureManager->RequireResource("Toothpaste"))
		m_toothPasteSprite.setTexture(*m_sharedContext->m_textureManager->GetResource("Toothpaste"));

	if (m_sharedContext->m_fontManager->RequireResource("Retro"))
	{
		m_itemNameLabel.setFont(*m_sharedContext->m_fontManager->GetResource("Retro"));
		m_itemNameLabel.setCharacterSize(30);
		m_itemNameLabel.setFillColor(sf::Color::Yellow);

		m_itemDescriptionLabel.setFont(*m_sharedContext->m_fontManager->GetResource("Retro"));
		m_itemDescriptionLabel.setCharacterSize(18);
		m_itemDescriptionLabel.setFillColor(sf::Color::White);

		m_itemPriceLabel.setFont(*m_sharedContext->m_fontManager->GetResource("Retro"));
		m_itemPriceLabel.setCharacterSize(22);
	}

	m_labelBackground.setFillColor(sf::Color(0, 0, 0, 150));
}

bool Buyable::CanBuy() const
{
	return m_itemInfo.m_price <= m_sharedContext->m_gameInfo->m_toothPaste;
}

void Buyable::TryToBuy()
{
	if (CanBuy() && !m_alreadyBought)
	{
		m_alreadyBought = true;
		m_sharedContext->m_gameInfo->m_toothPaste -= m_itemInfo.m_price;
		ApplyEffect();
	}
}

void Buyable::Draw() const
{
	if (!m_alreadyBought)
	{
		Actor::Draw();
		m_sharedContext->m_wind->GetRenderWindow()->draw(m_labelBackground);
		m_sharedContext->m_wind->GetRenderWindow()->draw(m_toothPasteSprite);
		m_sharedContext->m_wind->GetRenderWindow()->draw(m_itemNameLabel);
		m_sharedContext->m_wind->GetRenderWindow()->draw(m_itemDescriptionLabel);
		m_sharedContext->m_wind->GetRenderWindow()->draw(m_itemPriceLabel);
	}
}

void Buyable::Update(const sf::Time& l_time)
{
	if (!m_alreadyBought)
	{
		Actor::Update(l_time);

		UpdatePrice();

		if (m_sharedContext->m_actorManager->GetPlayer()->GetPosition().DistanceTo(m_position) <= __BUY_DISTANCE)
			TryToBuy();

		const int16_t yOffset = -180;

		m_itemNameLabel.setPosition(m_position.ToSFVector());
		m_itemNameLabel.move(0, yOffset);
		Utils::centerOrigin(m_itemNameLabel);
		m_itemNameLabel.setString(m_itemInfo.m_name);

		m_itemDescriptionLabel.setPosition(m_itemNameLabel.getPosition());
		m_itemDescriptionLabel.move(0, 40);
		Utils::centerOrigin(m_itemDescriptionLabel);
		m_itemDescriptionLabel.setString(m_itemInfo.m_description);

		m_itemPriceLabel.setPosition(m_itemDescriptionLabel.getPosition());
		m_itemPriceLabel.move(0, 50);
		Utils::centerOrigin(m_itemPriceLabel);
		m_itemPriceLabel.setString("x" + std::to_string(m_itemInfo.m_price));
		if (CanBuy())
			m_itemPriceLabel.setFillColor(sf::Color::Green);
		else
			m_itemPriceLabel.setFillColor(sf::Color::Red);

		m_toothPasteSprite.setPosition(m_itemPriceLabel.getGlobalBounds().left, m_itemPriceLabel.getGlobalBounds().top + m_itemPriceLabel.getLocalBounds().height / 2);
		Utils::centerOrigin(m_toothPasteSprite);
		m_toothPasteSprite.move(-35, 0);

		const float padding = 20;

		float min_x = m_itemNameLabel.getGlobalBounds().left;
		float max_x = m_itemNameLabel.getGlobalBounds().left + m_itemNameLabel.getGlobalBounds().width;

		const float min_y = m_itemNameLabel.getGlobalBounds().top;
		const float max_y = m_toothPasteSprite.getGlobalBounds().top + m_toothPasteSprite.getGlobalBounds().height;

		if (min_x > m_toothPasteSprite.getGlobalBounds().left)
			min_x = m_toothPasteSprite.getGlobalBounds().left;

		if (max_x < m_itemPriceLabel.getGlobalBounds().left + m_itemPriceLabel.getGlobalBounds().width)
			max_x = m_itemPriceLabel.getGlobalBounds().width;

		m_labelBackground.setPosition(min_x - padding, min_y - padding);
		m_labelBackground.setSize(sf::Vector2f(max_x - min_x + padding * 2, max_y - min_y + padding * 2));
	}
}

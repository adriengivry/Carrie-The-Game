#include "Buyable.h"
#include "StateManager.h"


Buyable::Buyable(SharedContext* p_sharedContext, const float p_x, const float p_y, const MoneyType p_moneyType)
	: Actor(p_sharedContext, p_x, p_y)
{
	m_alreadyBought = false;
	m_itemInfo.m_name = "NO_NAME";
	m_owned = false;
	m_itemInfo.m_price = 0;
	m_itemInfo.m_requiredMoney = p_moneyType;

	switch (m_itemInfo.m_requiredMoney)
	{
	case MoneyType::TOOTHPASTE:
		if (m_sharedContext->m_textureManager->RequireResource("Toothpaste"))
			m_moneySprite.setTexture(*m_sharedContext->m_textureManager->GetResource("Toothpaste"));
		break;

	case MoneyType::CARROT:
		if (m_sharedContext->m_textureManager->RequireResource("Carrot"))
			m_moneySprite.setTexture(*m_sharedContext->m_textureManager->GetResource("Carrot"));
		break;
	}
	
	if (m_sharedContext->m_fontManager->RequireResource("Retro"))
	{
		m_itemNameLabel.setFont(*m_sharedContext->m_fontManager->GetResource("Retro"));
		m_itemNameLabel.setCharacterSize(30);
		m_itemNameLabel.setFillColor(sf::Color::Yellow);

		m_itemDescriptionLabel.setFont(*m_sharedContext->m_fontManager->GetResource("Retro"));
		m_itemDescriptionLabel.setCharacterSize(20);

		m_itemPriceLabel.setFont(*m_sharedContext->m_fontManager->GetResource("Retro"));
		m_itemPriceLabel.setCharacterSize(22);
	}

	m_labelBackground.setFillColor(sf::Color(0, 0, 0, 150));
}

bool Buyable::CanBuy() const
{
	switch (m_itemInfo.m_requiredMoney)
	{
		case MoneyType::TOOTHPASTE: return m_itemInfo.m_price <= m_sharedContext->m_gameInfo->m_toothPaste;
		case MoneyType::CARROT:		return m_itemInfo.m_price <= m_sharedContext->m_gameInfo->m_carrots;
	}

	return false;
}

void Buyable::TryToBuy()
{
	if (BuyingCondition() && CanBuy() && !m_alreadyBought)
	{
		m_alreadyBought = true;
		
		switch (m_itemInfo.m_requiredMoney)
		{
		case MoneyType::TOOTHPASTE: m_sharedContext->m_gameInfo->m_toothPaste -= m_itemInfo.m_price;	break;
		case MoneyType::CARROT:		m_sharedContext->m_gameInfo->m_carrots -= m_itemInfo.m_price;		break;
		}
		ApplyEffect();
	}
}

void Buyable::Draw() const
{
	if (!m_alreadyBought && !m_owned)
	{
		Actor::Draw();
		m_sharedContext->m_wind->GetRenderWindow()->draw(m_labelBackground);
		m_sharedContext->m_wind->GetRenderWindow()->draw(m_moneySprite);
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

		if (m_sharedContext->m_actorManager->GetPlayer()->GetPosition().DistanceTo(m_position) <= __BUY_DISTANCE && !m_owned)
			TryToBuy();

		const int16_t yOffset = -180;

		m_itemNameLabel.setPosition(m_position.ToSFVector());
		m_itemNameLabel.move(0, yOffset);
		Utils::centerOrigin(m_itemNameLabel);
		m_itemNameLabel.setString(m_itemInfo.m_name);

		m_itemDescriptionLabel.setPosition(m_itemNameLabel.getPosition());
		m_itemDescriptionLabel.move(0, 40);
		Utils::centerOrigin(m_itemDescriptionLabel);

		if (m_owned)
		{
			m_itemDescriptionLabel.setFillColor(sf::Color::Red);
			m_itemDescriptionLabel.setString("ALREADY OWNED!");
		}
		else
		{
			if (BuyingCondition())
			{
				m_itemDescriptionLabel.setFillColor(sf::Color::White);
				m_itemDescriptionLabel.setString(m_itemInfo.m_description);
			}
			else
			{
				m_itemDescriptionLabel.setFillColor(sf::Color::Red);
				m_itemDescriptionLabel.setString("Useless for now");
			}
		}

		m_itemPriceLabel.setPosition(m_itemDescriptionLabel.getPosition());
		m_itemPriceLabel.move(0, 50);
		Utils::centerOrigin(m_itemPriceLabel);
		m_itemPriceLabel.setString("x" + std::to_string(m_itemInfo.m_price));
		if (CanBuy())
			m_itemPriceLabel.setFillColor(sf::Color::Green);
		else
			m_itemPriceLabel.setFillColor(sf::Color::Red);

		m_moneySprite.setPosition(m_itemPriceLabel.getGlobalBounds().left, m_itemPriceLabel.getGlobalBounds().top + m_itemPriceLabel.getLocalBounds().height / 2);
		Utils::centerOrigin(m_moneySprite);
		m_moneySprite.setScale(0.7f, 0.7f);
		m_moneySprite.move(-35, 0);

		const float padding = 20;

		float min_x = m_itemNameLabel.getGlobalBounds().left;
		float max_x = m_itemNameLabel.getGlobalBounds().left + m_itemNameLabel.getGlobalBounds().width;

		const float min_y = m_itemNameLabel.getGlobalBounds().top;
		const float max_y = m_moneySprite.getGlobalBounds().top + m_moneySprite.getGlobalBounds().height;

		if (min_x > m_moneySprite.getGlobalBounds().left)
			min_x = m_moneySprite.getGlobalBounds().left;

		if (max_x < m_itemPriceLabel.getGlobalBounds().left + m_itemPriceLabel.getGlobalBounds().width)
			max_x = m_itemPriceLabel.getGlobalBounds().width;

		m_labelBackground.setPosition(min_x - padding, min_y - padding);
		m_labelBackground.setSize(sf::Vector2f(max_x - min_x + padding * 2, max_y - min_y + padding * 2));
	}
}

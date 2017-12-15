#pragma once
#include "Actor.h"

enum class MoneyType
{
	TOOTHPASTE,
	CARROT
};

struct ItemInfo
{
	std::string m_name;
	std::string m_description;
	uint16_t m_defaultPrice;
	uint16_t m_price;
	MoneyType m_requiredMoney;
};

class Buyable : public Actor
{
	const float __BUY_DISTANCE = 100;

public:
	explicit Buyable(SharedContext* p_sharedContext, const float p_x, const float p_y, const MoneyType p_moneyType = MoneyType::TOOTHPASTE);
	~Buyable() {}

	uint16_t GetPrice() const { return m_itemInfo.m_price; }
	bool AlreadyBought() const { return m_alreadyBought; }
	bool CanBuy() const;
	virtual bool BuyingCondition() const { return true; }
	void TryToBuy();

	virtual void ApplyEffect() = 0;
	virtual void UpdatePrice() = 0;

	void Draw() const override;
	void Update(const sf::Time& l_time) override;

protected:
	sf::RectangleShape m_labelBackground;
	sf::Text m_itemNameLabel;
	sf::Text m_itemDescriptionLabel;
	sf::Text m_itemPriceLabel;
	sf::Sprite m_moneySprite;

	ItemInfo m_itemInfo;

	bool m_alreadyBought;

	bool m_owned;
};

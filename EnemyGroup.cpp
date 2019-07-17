#include <algorithm>
#include "EnemyGroup.h"
using namespace std;

const int EnemyGroup::kEnemyNumber = 8;
const int EnemyGroup::kEnemiesOnLine = 8;

EnemyGroup::EnemyGroup(const BackBuffer * aBackBuffer)
	:mBackBuffer(aBackBuffer)
{
	GenerateEnemies();
}

void EnemyGroup::GenerateEnemies()
{
	mEnemies.clear();

	auto generator = [this](int aIndex)
	{
		auto pos = Vec2(45 + 100 * (aIndex % kEnemiesOnLine),
			100 + 100 * (aIndex / kEnemiesOnLine));

		return std::make_unique<Enemy>(mBackBuffer, pos);
	};

	for (int i = 0; i < kEnemyNumber; ++i)
	{
		mEnemies.push_back(generator(i));
	}
}

bool EnemyGroup::HandleBullet(Bullet & aBullet)
{
	auto shot = std::find_if(mEnemies.begin(), mEnemies.end(),
		[&](unique_ptr<Enemy> & aEnemy)
	{
		return aEnemy->IsShot(aBullet);
	});

	if (shot != mEnemies.end())
	{
		mEnemies.erase(shot);

		return true;
	}

	return false;
}

bool EnemyGroup::IsEmpty() const
{
	return mEnemies.empty();
}

void EnemyGroup::Draw()
{
	std::for_each(mEnemies.begin(), mEnemies.end(),
		[](auto & aEnemy)
	{
		aEnemy->Draw();
	});

	for (auto & bullet : mBullets)
	{
		bullet->Draw();
	}
}

void EnemyGroup::ShootRandom()
{
	static size_t lastTime = ::GetTickCount();
	size_t currentTime = ::GetTickCount();
	if (currentTime - lastTime < 1000)
	{
		return;
	}
	else
	{
		lastTime = currentTime;
	}

	static bool init = false;
	if (!init)
	{
		srand(time(0));
	}

	auto idx = rand() % mEnemies.size();
	mBullets.push_back(mEnemies[idx]->Shoot());
}

void EnemyGroup::Update(float aTimeElapsed)
{
	for (auto & enemy : mEnemies)
	{
		enemy->Update(aTimeElapsed);
	}

	for (auto & bullet : mBullets)
	{
		bullet->Update(aTimeElapsed);
	}

}

EnemyGroup::Iter EnemyGroup::begin()
{
	return mBullets.begin();
}

EnemyGroup::Iter EnemyGroup::end()
{
	return mBullets.end();
}

EnemyGroup::ConstIter EnemyGroup::cbegin() const
{
	return mBullets.cbegin();
}

EnemyGroup::ConstIter EnemyGroup::cend() const
{
	return mBullets.cend();
}

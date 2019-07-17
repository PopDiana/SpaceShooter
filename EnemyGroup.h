#pragma once

#include <vector>
#include <iterator>
#include <memory>
#include <time.h>
#include <stdlib.h>
#include "Enemy.h"
#include "Bullet.h"
#include "BackBuffer.h"

class EnemyGroup
{
public:
	using Iter = std::vector<std::unique_ptr<EnemyBullet>>::iterator;
	using ConstIter = std::vector<std::unique_ptr<EnemyBullet>>::const_iterator;

	EnemyGroup(const BackBuffer * aBackBuffer);

	void GenerateEnemies();

	bool HandleBullet(Bullet & aBullet);

	bool IsEmpty() const;

	void Draw();

	void ShootRandom();

	void Update(float aTimeElapsed);

	Iter begin();

	Iter end();

	ConstIter cbegin() const;

	ConstIter cend() const;

private:
	static const int kEnemyNumber;
	static const int kEnemiesOnLine;
	std::vector<std::unique_ptr<Enemy>> mEnemies;
	std::vector<std::unique_ptr<EnemyBullet>> mBullets;
	const BackBuffer * mBackBuffer;
};

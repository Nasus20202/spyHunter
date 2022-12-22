#include "game.h"

Game::Game()
{
	cars = NULL;
	player = NULL;
}

Game::~Game()
{
	delete[] cars;
}

void Game::AddCar(Car *car)
{
	Car* *temp = new Car*[carsAmount + 1];
	for (int i = 0; i < carsAmount; i++)
		temp[i] = cars[i];
	temp[carsAmount] = car;
	carsAmount++;
	delete[] cars;
	cars = temp;
}

void Game::RemoveCar(const int index)
{
	Car** temp = new Car * [carsAmount - 1];
	for (int i = 0; i < index; i++)
		temp[i] = cars[i];
	for (int i = index; i < carsAmount - 1; i++)
		temp[i] = cars[i + 1];
	carsAmount--;
	delete[] cars;
	cars = temp;
}

void Game::NewGame(Player* player)
{
	delete[] cars;
	cars = NULL;
	carsAmount = 0;
	delete this->player;
	this->player = player;
}

Player* Game::GetPlayer()
{
	return player;
}

Car* Game::GetCar(const int index)
{
	if (index < 0 || index >= carsAmount)
		return NULL;
	return cars[index];
}

Car** Game::GetCars()
{
	return cars;
}

int Game::GetCarsAmount()
{
	return carsAmount;
}

void Game::Update(const double delta)
{
	Player* player = this->GetPlayer();
	player->Update();
	const double playerSpeed = player->GetSpeed();
	for (int i = 0; i < GetCarsAmount(); i++) {
		GetCar(i)->Update(delta, playerSpeed);
	}
}

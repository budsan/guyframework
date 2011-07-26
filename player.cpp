#include "player.h"
#include "input/input.h"
#include "world.h"

bool Player::firstPlayer = true;
SpriteAnimData Player::data;

Player::Player(Tilemap &parent) : TilemapCharacter(parent)
{
	if (firstPlayer)
	{
		firstPlayer = false;
		data.Load("data/mario.anim");
	}

	setAnimData(&data);
	ensureAnim("Idle");
	jumpTimeLeft = 0.0f;
	crouched = false;
}

void Player::Update(float GameTime)
{
	InputState state = Input::Instance().getInputState();

	//constantes
	const math::vec2f run_acc(1400, 0); //aceleracion corriendo
	const math::vec2f air_acc(400, 0); //aceleracion corriendo
	const math::vec2f fri_acc(1000, 800); //aceleracion de la friccion
	const math::vec2f gra_acc(0,-600); //aceleracion de la gravedad
	const math::vec2f vel_max(225, 300); //velocidad maxima que adquiere un personaje
	const math::vec2f vel_jmp(225, 225); //velocidad que adquiere un personaje al saltar

	if (crouched)
	{
		ensureAnim("Crouch");
	}
	else
	{
		if (grounded)
		{
			if (state.getKeyState(K_LEFT) && !state.getKeyState(K_RIGHT))
			{
				if (vel.x > 0) ensureAnim("Turn");
				else ensureAnim("Walk");
			}
			else if (state.getKeyState(K_RIGHT) && !state.getKeyState(K_LEFT))
			{
				if (vel.x < 0) ensureAnim("Turn");
				else ensureAnim("Walk");
			}
			else
			{
				ensureAnim("Idle");
			}
		}
		else
		{
			if (onWall) ensureAnim("OnWall");
			else ensureAnim("Jump");
		}
	}

	acc = math::vec2f(0,0);
	crouched = ((crouched||grounded) && state.getKeyState(K_DOWN)) || (crouched&&!grounded);
	if ( state.getKeyState(K_LEFT) ) {
		setScaleWidth(-1); //Mirror del sprite
		if (grounded) {
			if (!crouched) acc -= run_acc;
		}
		else acc -= air_acc;
	}
	if ( state.getKeyState(K_RIGHT)) {
		setScaleWidth(+1); //Sprite sin mirror
		if (grounded) {
			if (!crouched) acc += run_acc;
		}
		else acc += air_acc;
	}

	//FRICTION
	fri = math::vec2f(0,0);
	if (grounded)
	{
		if (crouched) fri.x = fri_acc.x/2;
		else          fri.x = fri_acc.x;
	}
	else
	{
		fri.x = fri_acc.x/8;
	}

	if (!crouched && onWall)
	{
		fri.y = fri_acc.y;
	}

	//Si en el frame anterior estaba tocando el suelo, inicializando
	//jumpTimeLeft a mas de 0 permite al jugador saltar durante ese rato
	if ( state.getKeyDown(K_JUMP) && (grounded||(onWall&&!crouched)))
	{
		jumpTimeLeft = 0.35f;
		if (onWall && !grounded && !crouched) {
			vel.x = vel_jmp.x * -1.0f * float(onWall);
		}
	}

	if (state.getKeyState(K_JUMP) && jumpTimeLeft > 0)
		vel.y = vel_jmp.y;
	else //A la que deja de pulsarse el boton de saltar cae de inmediato
		jumpTimeLeft = 0;

	if (jumpTimeLeft <= 0) acc += gra_acc; //La gravedad afecta mientras no salte
	else jumpTimeLeft -= GameTime; //Se le resta el tiempo mientras salta

	vel_lim = vel_max;
	grounded = false;
	onWall = ONWALL_NO;
	TilemapCharacter::Update(GameTime);
}

bool Player::onLeftCollision(int x, int j)
{
	vel.x = 0;
	onWall = ONWALL_LEFT;
	return true;
}

bool Player::onRightCollision(int x, int j)
{
	vel.x = 0;
	onWall = ONWALL_RIGHT;
	return true;
}

bool Player::onUpCollision(int x, int j)
{
	vel.y = 0;
	jumpTimeLeft = 0;
	return true;
}

bool Player::onDownCollision(int x, int j)
{
	vel.y = 0;
	grounded = true;
	return true;
}

void Player::Reset()
{
	vel = math::vec2f(0,0);
}

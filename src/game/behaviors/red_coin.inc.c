/**
 * This file contains the initialization and behavior for red coins.
 * Behavior controls audio and the orange number spawned, as well as interacting with
 * the course's red coin star.
 */

/**
 * Red coin's hitbox details.
 */
static struct ObjectHitbox sRedCoinHitbox = {
    /* interactType:      */ INTERACT_COIN,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 100,
    /* height:            */ 64,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

/**
 * Red coin initialization function. Sets the coin's hitbox and parent object.
 */
void s_coin_appear_init(void)
{
	// This floor and floor height are size. Perhaps for orange number spawns originally?
	struct Surface* dummyFloor;
	UNUSED f32 floorHeight = mcBGGroundCheck(o->oPosX, o->oPosY, o->oPosZ, &dummyFloor);

	struct Object* hiddenRedCoinStar;

	// Set the red coins to have a parent of the closest red coin star.
	hiddenRedCoinStar = s_find_obj(sm64::bhv::bhvHiddenRedCoinStar());
	if(hiddenRedCoinStar != NULL)
		o->parentObj = hiddenRedCoinStar;
	else
	{
		hiddenRedCoinStar = s_find_obj(sm64::bhv::bhvBowserCourseRedCoinStar());
		if(hiddenRedCoinStar != NULL)
		{
			o->parentObj = hiddenRedCoinStar;
		}
		else
		{
			o->parentObj = NULL;
		}
	}

	s_set_hitparam(o, &sRedCoinHitbox);
}

/**
 * Main behavior for red coins. Primarily controls coin collection noise and spawning
 * the orange number counter.
 */
void s_coin_appear_main(void)
{
	// If Mario interacted with the object...
	if(o->oInteractStatus & INT_STATUS_INTERACTED)
	{
		// ...and there is a red coin star in the level...
		if(o->parentObj != NULL)
		{
			// ...increment the star's counter.
			o->parentObj->oHiddenStarTriggerCounter++;

			// For JP version, play an identical sound for all coins.
			// Spawn the orange number counter, as long as it isn't the last coin.
			if(o->parentObj->oHiddenStarTriggerCounter != 8)
			{
				AppNumber(o->parentObj->oHiddenStarTriggerCounter, 0, 0, 0);
			}

			// On all versions but the JP version, each coin collected plays a higher noise.
			AudStartSound(SOUND_MENU_COLLECT_RED_COIN + (((u8)o->parentObj->oHiddenStarTriggerCounter - 1) << 16), gDefaultSoundArgs);
		}

		CoinCollected();
		// Despawn the coin.
		o->oInteractStatus = 0;
	}
}

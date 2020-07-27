
struct ObjectHitbox sRecoveryHeartHitbox = {
    /* interactType:      */ 0,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 50,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 50,
    /* hurtboxHeight:     */ 50,
};

void bhv_recovery_heart_loop(void)
{
	s_set_hitparam(o, &sRecoveryHeartHitbox);
	if(s_hitcheck(o, gMarioObject))
	{
		if(o->oSpinningHeartPlayedSound == 0)
		{
			objsound(SOUND_GENERAL_HEART_SPIN);
			o->oSpinningHeartPlayedSound += 1;
		}

		o->oAngleVelYaw = (s32)(200.0f * playerWorks[0].forwardVel) + 1000;
	}
	else
	{
		o->oSpinningHeartPlayedSound = 0;

		if((o->oAngleVelYaw -= 50 / FRAME_RATE_SCALER_INV) < 400)
		{
			o->oAngleVelYaw		   = 400;
			o->oSpinningHeartTotalSpin = 0;
		}
	}

	if((o->oSpinningHeartTotalSpin += o->oAngleVelYaw) >= 0x10000)
	{
		playerWorks[0].healCounter += 4;
		o->oSpinningHeartTotalSpin -= 0x10000;
	}

	o->oFaceAngleYaw += o->oAngleVelYaw * FRAME_RATE_SCALER;
}

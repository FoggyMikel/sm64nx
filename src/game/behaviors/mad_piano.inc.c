
static struct ObjectHitbox sMadPianoHitbox = {
    /* interactType:      */ INTERACT_MR_BLIZZARD,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 3,
    /* health:            */ 99,
    /* numLootCoins:      */ 0,
    /* radius:            */ 200,
    /* height:            */ 150,
    /* hurtboxRadius:     */ 200,
    /* hurtboxHeight:     */ 150,
};

static void mad_piano_act_wait(void)
{
	s_set_skelanimeNo(0);

	if(o->oDistanceToMario < 500.0f)
	{
		if(o->oTimer > 20 * FRAME_RATE_SCALER_INV)
		{
			if(playerWorks[0].forwardVel > 10.0f)
			{
				o->oAction = MAD_PIANO_ACT_ATTACK;
				s_hitON();
			}
		}
	}
	else
	{
		o->oTimer = 0;
	}

	s_player_slideout_RH(280.0f, 150.0f);
}

static void mad_piano_act_attack(void)
{
	s_enemybgcheck();
	s_set_skelanimeNo(1);
	func_802F9378(0, 0, SOUND_OBJ_MAD_PIANO_CHOMPING);

	if(o->oDistanceToMario < 500.0f)
	{
		o->oTimer = 0;
	}

	if(o->oTimer > 80 * FRAME_RATE_SCALER_INV && s_check_animeend())
	{
		o->oAction     = MAD_PIANO_ACT_WAIT;
		o->oForwardVel = 0.0f;
		s_hitOFF();
	}
	else
	{
		f32 dx	       = o->oPosX - o->oHomeX;
		f32 dz	       = o->oPosZ - o->oHomeZ;
		f32 distToHome = sqrtf(dx * dx + dz * dz);

		if(distToHome > 400.0f)
		{
			distToHome = 400.0f / distToHome;
			o->oPosX   = o->oHomeX + dx * distToHome;
			o->oPosZ   = o->oHomeZ + dz * distToHome;
		}

		s_chase_angleY(o->oAngleToMario, 400 / FRAME_RATE_SCALER_INV);
		o->oForwardVel = 5.0f;
	}

	obj_check_attacks(&sMadPianoHitbox, o->oAction);
	s_enemymove(78);
}

void bhv_mad_piano_update(void)
{
	if(!(o->activeFlags & 0x0008))
	{
		o->oFaceAngleYaw = o->oMoveAngleYaw - 0x4000;

		switch(o->oAction)
		{
			case MAD_PIANO_ACT_WAIT:
				mad_piano_act_wait();
				break;
			case MAD_PIANO_ACT_ATTACK:
				mad_piano_act_attack();
				break;
		}
	}
}

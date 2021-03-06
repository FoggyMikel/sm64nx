struct TripletButterflyActivationData
{
	s32 model;
	const BehaviorScript* behavior;
	f32 scale;
};

static struct ObjectHitbox sTripletButterflyExplodeHitbox = {
    /* interactType:      */ INTERACT_MR_BLIZZARD,
    /* downOffset:        */ 50,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 1,
    /* numLootCoins:      */ 0,
    /* radius:            */ 100,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 100,
    /* hurtboxHeight:     */ 50,
};

static struct TripletButterflyActivationData sTripletButterflyActivationData[] = {
    {MODEL_BOWLING_BALL, NULL, 0.5f},
    {MODEL_1UP, sm64::bhv::bhv1upWalking(), 1.0f},
};

static void triplet_butterfly_act_init(void)
{
	s32 butterflyNum;
	s32 i;

	butterflyNum = o->oBehParams2ndByte & TRIPLET_BUTTERFLY_BP_BUTTERFLY_NUM;
	if(butterflyNum != 0 || o->oDistanceToMario < 200.0f)
	{
		if(butterflyNum == 0)
		{
			for(i = 1; i <= 2; i++)
			{
				s_makeobj_chain(i, 0, 0, 0, o, MODEL_BUTTERFLY, sm64::bhv::bhvTripletButterfly());
			}

			o->oTripletButterflySelectedButterfly = RandomU16() % 3;
		}

		//! TODO: Describe this glitch
		if(o->parentObj->oTripletButterflySelectedButterfly == o->oBehParams2ndByte)
		{
			o->oTripletButterflyType = TRIPLET_BUTTERFLY_TYPE_SPAWN_1UP;
		}
		else if(o->parentObj->oBehParams2ndByte & TRIPLET_BUTTERFLY_BP_NO_BOMBS)
		{
			o->oTripletButterflyType = TRIPLET_BUTTERFLY_TYPE_NORMAL;
		}
		// Default butterfly type is TRIPLET_BUTTERFLY_TYPE_EXPLODES

		o->oAction = TRIPLET_BUTTERFLY_ACT_WANDER;

		o->oTripletButterflyBaseYaw = o->oBehParams2ndByte * (0x10000 / 3);
		o->oMoveAngleYaw	    = (s32)(o->oTripletButterflyBaseYaw + random_linear_offset(0, 0x5555));
		o->oTripletButterflySpeed   = random_linear_offset(15, 15);

		s_shape_disp();
	}
}

static void triplet_butterfly_act_wander(void)
{
	if(o->oDistanceToMario > 1500.0f && !sm64::config().camera().disableDistanceClip())
	{
		s_remove_obj(o);
	}
	else
	{
		approach_f32_ptr(&o->oTripletButterflySpeed, 8.0f, 0.5f * FRAME_RATE_SCALER);
		if(o->oTimer < 60 * FRAME_RATE_SCALER_INV)
		{
			o->oTripletButterflyTargetYaw = s_calc_returnangle();
		}
		else
		{
			o->oTripletButterflyTargetYaw = (s32)o->oTripletButterflyBaseYaw;

			if(o->oTimer > 110 * FRAME_RATE_SCALER_INV && o->oDistanceToMario < 200.0f && o->oTripletButterflyType > TRIPLET_BUTTERFLY_TYPE_NORMAL)
			{
				o->oAction		  = TRIPLET_BUTTERFLY_ACT_ACTIVATE;
				o->oTripletButterflySpeed = 0.0f;
			}
		}

		if(o->oHomeY < o->oFloorHeight)
		{
			o->oHomeY = o->oFloorHeight;
		}

		if(o->oPosY < o->oHomeY + random_linear_offset(50, 50))
		{
			o->oTripletButterflyTargetPitch = -0x2000;
		}
		else
		{
			o->oTripletButterflyTargetPitch = 0x2000;
		}

		obj_move_pitch_approach(o->oTripletButterflyTargetPitch, 400 / FRAME_RATE_SCALER_INV);
		s_chase_angleY(o->oTripletButterflyTargetYaw, random_linear_offset(400 / FRAME_RATE_SCALER_INV, 800 / FRAME_RATE_SCALER_INV));
	}
}

static void triplet_butterfly_act_activate(void)
{
	if(o->oTimer > 20 * FRAME_RATE_SCALER_INV)
	{
		if(o->oTripletButterflyModel == 0)
		{
			s_makeobj_chain_scale(0, 0, -40, 0, 1.5f, o, MODEL_SMOKE, sm64::bhv::bhvWhitePuffSmoke2());
			o->oTripletButterflyModel = sTripletButterflyActivationData[o->oTripletButterflyType].model;
			s_change_shape(o->oTripletButterflyModel);
			s_softspritemodeON(o);
			o->oTripletButterflyScale = 0.0f;
			o->oHomeY		  = o->oPosY;
		}
		else if(o->oTripletButterflyScale >= sTripletButterflyActivationData[o->oTripletButterflyType].scale)
		{
			if(o->oTripletButterflyType != TRIPLET_BUTTERFLY_TYPE_EXPLODES)
			{
				s_makeobj_nowpos(o, o->oTripletButterflyModel, sTripletButterflyActivationData[o->oTripletButterflyType].behavior);
				s_remove_obj(o);
			}
			else
			{
				o->oAction	     = TRIPLET_BUTTERFLY_ACT_EXPLODE;
				o->oWallHitboxRadius = 100.0f;
			}
		}

		o->oTripletButterflyScale += sTripletButterflyActivationData[o->oTripletButterflyType].scale / (30.0f * FRAME_RATE_SCALER);

		if(o->oTripletButterflyType == TRIPLET_BUTTERFLY_TYPE_EXPLODES)
		{
			o->oGraphYOffset = 250.0f * o->oTripletButterflyScale;
			o->oPosY	 = o->oHomeY - o->oGraphYOffset;
		}
	}
}

static void triplet_butterfly_act_explode(void)
{
	f32 scaleIncrease;

	obj_check_attacks(&sTripletButterflyExplodeHitbox, -1);

	if(o->oAction == -1 || (o->oMoveFlags & 0x00000200) || o->oTimer >= 158 * FRAME_RATE_SCALER_INV)
	{
		o->oPosY += o->oGraphYOffset;
		s_makeobj_nowpos(o, MODEL_EXPLOSION, sm64::bhv::bhvExplosion());
		s_remove_obj(o);
	}
	else
	{
		if(o->oTimer > 120 * FRAME_RATE_SCALER_INV)
		{
			scaleIncrease = 0.04f * coss(o->oTripletButterflyScalePhase);
			if(scaleIncrease > 0.0f)
			{
				scaleIncrease *= 4.5f;
				o->oTripletButterflyScalePhase += 10000 / FRAME_RATE_SCALER_INV;
			}
			else
			{
				o->oTripletButterflyScalePhase += 4000 / FRAME_RATE_SCALER_INV;
			}

			o->oTripletButterflyScale += scaleIncrease * FRAME_RATE_SCALER;
		}

		approach_f32_ptr(&o->oTripletButterflySpeed, 20.0f, 1.0f * FRAME_RATE_SCALER);
		s_chase_angleY(o->oAngleToMario, 800 / FRAME_RATE_SCALER_INV);
		obj_turn_pitch_toward_mario(-100.0f, 800 / FRAME_RATE_SCALER_INV);
	}
}

void bhv_triplet_butterfly_update(void)
{
	s_enemybgcheck();

	switch(o->oAction)
	{
		case TRIPLET_BUTTERFLY_ACT_INIT:
			triplet_butterfly_act_init();
			break;
		case TRIPLET_BUTTERFLY_ACT_WANDER:
			triplet_butterfly_act_wander();
			break;
		case TRIPLET_BUTTERFLY_ACT_ACTIVATE:
			triplet_butterfly_act_activate();
			break;
		case TRIPLET_BUTTERFLY_ACT_EXPLODE:
			triplet_butterfly_act_explode();
			break;
	}

	s_set_scale(o->oTripletButterflyScale);
	obj_compute_vel_from_move_pitch(o->oTripletButterflySpeed);
	s_enemymove(78);
}

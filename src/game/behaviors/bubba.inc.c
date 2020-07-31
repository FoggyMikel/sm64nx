// bubba.inc.c

static struct ObjectHitbox sBubbaHitbox = {
    /* interactType:      */ INTERACT_CLAM_OR_BUBBA,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 99,
    /* numLootCoins:      */ 0,
    /* radius:            */ 300,
    /* height:            */ 200,
    /* hurtboxRadius:     */ 300,
    /* hurtboxHeight:     */ 200,
};

void bubba_act_0(void)
{
	f32 sp24;

	sp24 = s_calc_enemyscope();
	treat_far_home_as_mario(2000.0f);
	o->oAnimState = 0;

	o->oBubbaUnk1AC = obj_get_pitch_to_home(sp24);

	approach_f32_ptr(&o->oBubbaUnkF4, 5.0f, 0.5f * FRAME_RATE_SCALER);

	if(o->oBubbaUnkFC != 0)
	{
		if(s_calc_dangle(o->oMoveAngleYaw, o->oBubbaUnk1AE) < 800)
		{
			o->oBubbaUnkFC = 0;
		}
	}
	else
	{
		if(o->oDistanceToMario >= 25000.0f)
		{
			o->oBubbaUnk1AE = o->oAngleToMario;
			o->oBubbaUnkF8	= random_linear_offset(20, 30);
		}

		if((o->oBubbaUnkFC = o->oMoveFlags & 0x00000200) != 0)
		{
			o->oBubbaUnk1AE = s_wall_rebound();
		}
		else if(o->oTimer > 30 * FRAME_RATE_SCALER_INV && o->oDistanceToMario < 2000.0f)
		{
			o->oAction = 1;
		}
		else if(o->oBubbaUnkF8 != 0)
		{
			o->oBubbaUnkF8 -= 1;
		}
		else
		{
			o->oBubbaUnk1AE = obj_random_fixed_turn(0x2000);
			o->oBubbaUnkF8	= random_linear_offset(100, 100);
		}
	}
}

void bubba_act_1(void)
{
	s16 val06;
	s16 val04;

	treat_far_home_as_mario(2500.0f);
	if(o->oDistanceToMario > 2500.0f)
	{
		o->oAction = 0;
	}
	else if(o->oBubbaUnk100 != 0)
	{
		if(--o->oBubbaUnk100 == 0)
		{
			objsound(SOUND_OBJ_BUBBA_CHOMP);
			o->oAction = 0;
		}
		else if(o->oBubbaUnk100 < 15)
		{
			o->oAnimState = 1;
		}
		else if(o->oBubbaUnk100 == 20)
		{
			val06 = 10000 - (s16)(20.0f * (find_water_level(o->oPosX, o->oPosZ) - o->oPosY));
			o->oBubbaUnk1AC -= val06;
			o->oMoveAnglePitch = o->oBubbaUnk1AC;
			o->oBubbaUnkF4	   = 40.0f;
			obj_compute_vel_from_move_pitch(o->oBubbaUnkF4);
			o->oAnimState = 0;
			;
		}
		else
		{
			o->oBubbaUnk1AE = o->oAngleToMario;
			o->oBubbaUnk1AC = o->oBubbaUnk104;

			s_chase_angleY(o->oBubbaUnk1AE, 400 / FRAME_RATE_SCALER_INV);
			obj_move_pitch_approach(o->oBubbaUnk1AC, 400 / FRAME_RATE_SCALER_INV);
		}
	}
	else
	{
		if(s_calc_dangle(gMarioObject->oFaceAngleYaw, o->oAngleToMario) < 0x3000)
		{
			val04 = 0x4000 - atan2s(800.0f, o->oDistanceToMario - 800.0f);
			if((s16)(o->oMoveAngleYaw - o->oAngleToMario) < 0)
			{
				val04 = -val04;
			}

			o->oBubbaUnk1AE = o->oAngleToMario + val04;
			;
		}
		else
		{
			o->oBubbaUnk1AE = o->oAngleToMario;
		}

		o->oBubbaUnk1AC = o->oBubbaUnk104;

		if(obj_is_near_to_and_facing_mario(500.0f, 3000) && s_calc_dangle(o->oBubbaUnk1AC, o->oMoveAnglePitch) < 3000)
		{
			o->oBubbaUnk100 = 30;
			o->oBubbaUnkF4	= 0;
			o->oAnimState	= 1;
		}
		else
		{
			approach_f32_ptr(&o->oBubbaUnkF4, 20.0f, 0.5f * FRAME_RATE_SCALER);
		}
	}
}

void bhv_bubba_loop(void)
{
	UNUSED s32 unused;
	struct Object* sp38;
	s16 sp36;

	o->oInteractionSubtype &= ~INT_SUBTYPE_EATS_MARIO;
	o->oBubbaUnk104 = obj_turn_pitch_toward_mario(120.0f, 0);

	if(s_calc_dangle(o->oAngleToMario, o->oMoveAngleYaw) < 0x1000 && s_calc_dangle(o->oBubbaUnk104 + 0x800, o->oMoveAnglePitch) < 0x2000)
	{
		if(o->oAnimState != 0 && o->oDistanceToMario < 250.0f)
		{
			o->oInteractionSubtype |= INT_SUBTYPE_EATS_MARIO;
		}

		o->hurtboxRadius = 100.0f;
	}
	else
	{
		o->hurtboxRadius = 150.0f;
	}

	s_enemybgcheck();

	switch(o->oAction)
	{
		case 0:
			bubba_act_0();
			break;
		case 1:
			bubba_act_1();
			break;
	}

	if(o->oMoveFlags & 0x00000078)
	{
		if(o->oMoveFlags & 0x00000008)
		{
			sp38 = s_makeobj_nowpos(o, MODEL_WATER_SPLASH, sm64::bhv::bhvWaterSplash());
			if(sp38 != NULL)
			{
				s_scale(sp38, 3.0f);
			}

			o->oBubbaUnk108 = o->oVelY;
			o->oBubbaUnk10C = 0.0f;
			;
		}
		else
		{
			approach_f32_ptr(&o->oBubbaUnk108, 0.0f, 4.0f * FRAME_RATE_SCALER);

			if((o->oBubbaUnk10C -= o->oBubbaUnk108) > 1.0f)
			{
				sp36 = RandomU16();
				o->oBubbaUnk10C -= 1.0f;
				s_makeobj_chain(0, 150.0f * coss(sp36), 0x64, 150.0f * sins(sp36), o, MODEL_WHITE_PARTICLE_SMALL, sm64::bhv::bhvSmallParticleSnow());
			}
		}

		obj_smooth_turn(&o->oBubbaUnk1B0, &o->oMoveAnglePitch, o->oBubbaUnk1AC, 0.05f, 10, 50, 2000);
		obj_smooth_turn(&o->oBubbaUnk1B2, &o->oMoveAngleYaw, o->oBubbaUnk1AE, 0.05f, 10, 50, 2000);
		obj_compute_vel_from_move_pitch(o->oBubbaUnkF4);
	}
	else
	{
		o->oBubbaUnkF4	   = sqrtf(o->oForwardVel * o->oForwardVel + o->oVelY * o->oVelY);
		o->oMoveAnglePitch = obj_get_pitch_from_vel();
		obj_face_pitch_approach(o->oMoveAnglePitch, 400 / FRAME_RATE_SCALER_INV);
		o->oBubbaUnk1B0 = 0;
	}

	obj_face_pitch_approach(o->oMoveAnglePitch, 400 / FRAME_RATE_SCALER_INV);
	obj_check_attacks(&sBubbaHitbox, o->oAction);

	s_enemymove(78);

	o->oFloorHeight += 150.0f * FRAME_RATE_SCALER;

	if(o->oPosY < o->oFloorHeight)
	{
		o->oPosY = o->oFloorHeight;
	}
}

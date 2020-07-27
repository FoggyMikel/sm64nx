// boo.c.inc

static struct ObjectHitbox sBooGivingStarHitbox = {
    /* interactType: */ 0,
    /* downOffset: */ 0,
    /* damageOrCoinValue: */ 3,
    /* health: */ 3,
    /* numLootCoins: */ 0,
    /* radius: */ 140,
    /* height: */ 80,
    /* hurtboxRadius: */ 40,
    /* hurtboxHeight: */ 60,
};

// Relative positions
static s16 sCourtyardBooTripletPositions[][3] = {{0, 50, 0}, {210, 110, 210}, {-210, 70, -210}};

static void boo_stop(void)
{
	o->oForwardVel = 0.0f;
	o->oVelY       = 0.0f;
	o->oGravity    = 0.0f;
}

void bhv_boo_init(void)
{
	o->oBooInitialMoveYaw = o->oMoveAngleYaw;
}

static s32 boo_should_be_stopped(void)
{
	if(obj_has_behavior(sm64::bhv::bhvMerryGoRoundBigBoo()) || obj_has_behavior(sm64::bhv::bhvMerryGoRoundBoo()))
	{
		if(gMarioOnMerryGoRound == FALSE)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if(o->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM)
		{
			return TRUE;
		}

		if(o->oRoom == 10)
		{
			if(gTimeStopState & TIME_STOP_MARIO_OPENED_DOOR)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

static s32 boo_should_be_active(void)
{
	f32 activationRadius;

	if(obj_has_behavior(sm64::bhv::bhvBalconyBigBoo()))
	{
		activationRadius = 5000.0f;
	}
	else
	{
		activationRadius = 1500.0f;
	}

	if(obj_has_behavior(sm64::bhv::bhvMerryGoRoundBigBoo()) || obj_has_behavior(sm64::bhv::bhvMerryGoRoundBoo()))
	{
		if(gMarioOnMerryGoRound == TRUE)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if(o->oRoom == -1)
	{
		if(o->oDistanceToMario < activationRadius)
		{
			return TRUE;
		}
	}
	else if(!boo_should_be_stopped())
	{
		if(o->oDistanceToMario < activationRadius && (o->oRoom == gMarioCurrentRoom || gMarioCurrentRoom == 0))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void bhv_courtyard_boo_triplet_init(void)
{
	s32 i;
	struct Object* boo;

	if(playerMeter.stars < 12)
	{
		s_remove_obj(o);
	}
	else
	{
		for(i = 0; i < 3; i++)
		{
			boo = s_makeobj_chain(0x01, sCourtyardBooTripletPositions[i][0], sCourtyardBooTripletPositions[i][1], sCourtyardBooTripletPositions[i][2], o, MODEL_BOO, sm64::bhv::bhvGhostHuntBoo());

			boo->oMoveAngleYaw = RandomU16();
		}
	}
}

static void boo_approach_target_opacity_and_update_scale(void)
{
	f32 scale;

	if(o->oBooTargetOpacity != o->oOpacity)
	{
		if(o->oBooTargetOpacity > o->oOpacity)
		{
			o->oOpacity += 20 / FRAME_RATE_SCALER_INV;

			if(o->oBooTargetOpacity < o->oOpacity)
			{
				o->oOpacity = o->oBooTargetOpacity;
			}
		}
		else
		{
			o->oOpacity -= 20 / FRAME_RATE_SCALER_INV;

			if(o->oBooTargetOpacity > o->oOpacity)
			{
				o->oOpacity = o->oBooTargetOpacity;
			}
		}
	}

	scale = (o->oOpacity / 255.0f * 0.4 + 0.6) * o->oBooBaseScale;
	scale_object(o, scale); // why no s_set_scale? was s_set_scale written later?
}

static void boo_oscillate(s32 ignoreOpacity)
{
	o->oFaceAnglePitch = sins(o->oBooOscillationTimer / FRAME_RATE_SCALER_INV) * 0x400;

	if(o->oOpacity == 0xFF || ignoreOpacity == TRUE)
	{
		o->header.gfx.scale[0] = sins(o->oBooOscillationTimer / FRAME_RATE_SCALER_INV) * 0.08 + o->oBooBaseScale;
		o->header.gfx.scale[1] = -sins(o->oBooOscillationTimer / FRAME_RATE_SCALER_INV) * 0.08 + o->oBooBaseScale;
		o->header.gfx.scale[2] = o->header.gfx.scale[0];
		o->oGravity	       = sins(o->oBooOscillationTimer / FRAME_RATE_SCALER_INV) * o->oBooBaseScale;
		o->oBooOscillationTimer += 0x400 / FRAME_RATE_SCALER_INV;
	}
}

static s32 boo_vanish_or_appear(void)
{
	s16 relativeAngleToMario   = abs_angle_diff(o->oAngleToMario, o->oMoveAngleYaw);
	s16 relativeMarioFaceAngle = abs_angle_diff(o->oMoveAngleYaw, gMarioObject->oFaceAngleYaw);
	// magic?
	s16 relativeAngleToMarioThreshhold   = 0x1568;
	s16 relativeMarioFaceAngleThreshhold = 0x6b58;
	s32 doneAppearing		     = FALSE;

	o->oVelY = 0.0f;

	if(relativeAngleToMario > relativeAngleToMarioThreshhold || relativeMarioFaceAngle < relativeMarioFaceAngleThreshhold)
	{
		if(o->oOpacity == 40)
		{
			o->oBooTargetOpacity = 255;
			objsound(SOUND_OBJ_BOO_LAUGH_LONG);
		}

		if(o->oOpacity > 180)
		{
			doneAppearing = TRUE;
		}
	}
	else if(o->oOpacity == 255)
	{
		o->oBooTargetOpacity = 40;
	}

	return doneAppearing;
}

static void boo_set_move_yaw_for_during_hit(s32 hurt)
{
	s_hitOFF();

	o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
	o->oBooMoveYawBeforeHit = (f32)o->oMoveAngleYaw;

	if(hurt != FALSE)
	{
		o->oBooMoveYawDuringHit = gMarioObject->oMoveAngleYaw;
	}
	else if(coss((s16)o->oMoveAngleYaw - (s16)o->oAngleToMario) < 0.0f)
	{
		o->oBooMoveYawDuringHit = o->oMoveAngleYaw;
	}
	else
	{
		o->oBooMoveYawDuringHit = (s16)(o->oMoveAngleYaw + 0x8000);
	}
}

static void boo_move_during_hit(s32 roll, f32 fVel)
{
	// Boos seem to have been supposed to oscillate up then down then back again
	// when hit. However it seems the programmers forgot to scale the cosine,
	// so the Y velocity goes from 1 to -1 and back to 1 over 32 frames.
	// This is such a small change that the Y position only changes by 5 units.
	// It's completely unnoticable in-game.
	s32 oscillationVel = o->oTimer * 0x800 / FRAME_RATE_SCALER_INV + 0x800;

	o->oForwardVel	 = fVel;
	o->oVelY	 = coss(oscillationVel);
	o->oMoveAngleYaw = o->oBooMoveYawDuringHit;

	if(roll != FALSE)
	{
		o->oFaceAngleYaw += D_8032F0CC[o->oTimer / FRAME_RATE_SCALER_INV] / FRAME_RATE_SCALER_INV;
		o->oFaceAngleRoll += D_8032F0CC[o->oTimer / FRAME_RATE_SCALER_INV] / FRAME_RATE_SCALER_INV;
	}
}

static void big_boo_shake_after_hit(void)
{
	// Oscillate yaw
	s32 oscillationVel = o->oTimer * 0x2000 / FRAME_RATE_SCALER_INV - 0x3E000;
	o->oFaceAngleYaw += coss(oscillationVel) * 0x400 / FRAME_RATE_SCALER_INV;
}

static void boo_reset_after_hit(void)
{
	o->oMoveAngleYaw = o->oBooMoveYawBeforeHit;
	o->oFlags |= OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
	o->oInteractStatus = 0;
}

// called iff boo/big boo/cage boo is in action 2, which only occurs if it was non-attack-ly interacted with/bounced on?
static s32 boo_update_after_bounced_on(f32 a0)
{
	boo_stop();

	if(o->oTimer == 0)
	{
		boo_set_move_yaw_for_during_hit(FALSE);
	}

	if(o->oTimer < 32 * FRAME_RATE_SCALER_INV)
	{
		boo_move_during_hit(FALSE, D_8032F0CC[o->oTimer / FRAME_RATE_SCALER_INV] / 5000.0f * a0);
	}
	else
	{
		s_hitON();
		boo_reset_after_hit();
		o->oAction = 1;
		return TRUE;
	}

	return FALSE;
}

// called iff big boo nonlethally hit
static s32 big_boo_update_during_nonlethal_hit(f32 a0)
{
	boo_stop();

	if(o->oTimer == 0)
	{
		boo_set_move_yaw_for_during_hit(TRUE);
	}

	if(o->oTimer < 32 * FRAME_RATE_SCALER_INV)
	{
		boo_move_during_hit(TRUE, D_8032F0CC[o->oTimer / FRAME_RATE_SCALER_INV] / 5000.0f * a0);
	}
	else if(o->oTimer < 48 * FRAME_RATE_SCALER_INV)
	{
		big_boo_shake_after_hit();
	}
	else
	{
		s_hitON();
		boo_reset_after_hit();

		o->oAction = 1;

		return TRUE;
	}

	return FALSE;
}

// called every frame once mario lethally hits the boo until the boo is deleted,
// returns whether death is complete
static s32 boo_update_during_death(void)
{
	struct Object* parentBigBoo;

	if(o->oTimer == 0)
	{
		o->oForwardVel	   = 40.0f;
		o->oMoveAngleYaw   = gMarioObject->oMoveAngleYaw;
		o->oBooDeathStatus = BOO_DEATH_STATUS_DYING;
		o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
	}
	else
	{
		if(o->oTimer == 5 * FRAME_RATE_SCALER_INV)
		{
			o->oBooTargetOpacity = 0;
		}

		if(o->oTimer > 30 * FRAME_RATE_SCALER_INV || o->oMoveFlags & 0x200)
		{
			s_kemuri();
			o->oBooDeathStatus = BOO_DEATH_STATUS_DEAD;

			if(o->oBooParentBigBoo != NULL)
			{
				parentBigBoo = o->oBooParentBigBoo;

				if(!obj_has_behavior(sm64::bhv::bhvBoo()))
				{
					parentBigBoo->oBigBooNumMinionBoosKilled++;
				}
			}

			return TRUE;
		}
	}

	o->oVelY = 5.0f;
	o->oFaceAngleRoll += 0x800 / FRAME_RATE_SCALER_INV;
	o->oFaceAngleYaw += 0x800 / FRAME_RATE_SCALER_INV;

	return FALSE;
}

static s32 obj_has_attack_type(u32 attackType)
{
	if((o->oInteractStatus & INT_STATUS_ATTACK_MASK) == attackType)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static s32 boo_get_attack_status(void)
{
	s32 attackStatus = BOO_NOT_ATTACKED;

	if(o->oInteractStatus & INT_STATUS_INTERACTED)
	{
		if((o->oInteractStatus & INT_STATUS_WAS_ATTACKED) && obj_has_attack_type(ATTACK_FROM_ABOVE) == FALSE)
		{
			s_hitOFF();

			o->oInteractStatus = 0;

			objsound(SOUND_OBJ_BOO_LAUGH_SHORT);

			attackStatus = BOO_ATTACKED;
		}
		else
		{
			objsound(SOUND_OBJ_BOO_BOUNCE_TOP);

			o->oInteractStatus = 0;

			attackStatus = BOO_BOUNCED_ON;
		}
	}

	return attackStatus;
}

// boo idle/chasing movement?
static void boo_chase_mario(f32 a0, s16 a1, f32 a2)
{
	f32 sp1C;
	s16 sp1A;

	if(boo_vanish_or_appear())
	{
		o->oInteractType = 0x8000;

		if(obj_lateral_dist_from_mario_to_home() > 1500.0f)
		{
			sp1A = obj_angle_to_home();
		}
		else
		{
			sp1A = o->oAngleToMario;
		}

		obj_rotate_yaw_toward(sp1A, a1 / FRAME_RATE_SCALER_INV);
		o->oVelY = 0.0f;

		if(mario_is_in_air_action() == 0)
		{
			sp1C = o->oPosY - gMarioObject->oPosY;
			if(a0 < sp1C && sp1C < 500.0f)
			{
				o->oVelY = func_802A0BF4(o->oPosY, gMarioObject->oPosY + 50.0f, 10.f, 2.0f);
			}
		}

		func_8029F684(10.0f - o->oBooNegatedAggressiveness, a2);

		if(o->oForwardVel != 0.0f)
		{
			boo_oscillate(FALSE);
		}
	}
	else
	{
		o->oInteractType = 0;
		// why is boo_stop not used here
		o->oForwardVel = 0.0f;
		o->oVelY       = 0.0f;
		o->oGravity    = 0.0f;
	}
}

static void ActionBoo0(void)
{
	o->activeFlags |= ACTIVE_FLAG_MOVE_THROUGH_GRATE;

	if(o->oBehParams2ndByte == 2)
	{
		o->oRoom = 10;
	}

	obj_set_pos_to_home();
	o->oMoveAngleYaw = o->oBooInitialMoveYaw;
	boo_stop();

	o->oBooParentBigBoo  = s_find_obj(sm64::bhv::bhvGhostHuntBigBoo());
	o->oBooBaseScale     = 1.0f;
	o->oBooTargetOpacity = 0xFF;

	if(boo_should_be_active())
	{
		// Condition is met if the object is sm64::bhv::bhvBalconyBigBoo() or sm64::bhv::bhvMerryGoRoundBoo()
		if(o->oBehParams2ndByte == 2)
		{
			o->oBooParentBigBoo = NULL;
			o->oAction	    = 5;
		}
		else
		{
			o->oAction = 1;
		}
	}
}

static void ActionBoo5(void)
{
	if(o->oTimer < 30 * FRAME_RATE_SCALER_INV)
	{
		o->oVelY       = 0.0f;
		o->oForwardVel = 13.0f;
		boo_oscillate(FALSE);
		o->oWallHitboxRadius = 0.0f;
	}
	else
	{
		o->oAction	     = 1;
		o->oWallHitboxRadius = 30.0f;
	}
}

static void ActionBoo1(void)
{
	s32 attackStatus;

	if(o->oTimer == 0)
	{
		o->oBooNegatedAggressiveness = -RandomFloat() * 5.0f;
		o->oBooTurningSpeed	     = (s32)(RandomFloat() * 128.0f);
	}

	boo_chase_mario(-100.0f, o->oBooTurningSpeed + 0x180, 0.5f);
	attackStatus = boo_get_attack_status();

	if(boo_should_be_stopped())
	{
		o->oAction = 0;
	}

	if(attackStatus == BOO_BOUNCED_ON)
	{
		o->oAction = 2;
	}

	if(attackStatus == BOO_ATTACKED)
	{
		o->oAction = 3;
	}

	if(attackStatus == BOO_ATTACKED)
	{
		obj_remove_sound(SOUND_OBJ_DYING_ENEMY1);
	}
}

static void ActionBoo2(void)
{
	if(boo_update_after_bounced_on(20.0f))
	{
		o->oAction = 1;
	}
}

static void ActionBoo3(void)
{
	if(boo_update_during_death())
	{
		if(o->oBehParams2ndByte != 0)
		{
			s_remove_obj(o);
		}
		else
		{
			o->oAction = 4;
			obj_disable();
		}
	}
}

// Called when a Go on a Ghost Hunt boo dies
static void ActionBoo4(void)
{
	s32 dialogID;

	// If there are no remaining "minion" boos, show the dialog of the Big Boo
	if(s_find_obj(sm64::bhv::bhvGhostHuntBoo()) == NULL)
	{
		dialogID = DIALOG_108;
	}
	else
	{
		dialogID = DIALOG_107;
	}

	if(obj_update_dialog(2, 2, dialogID, 0))
	{
		obj_remove_sound(SOUND_OBJ_DYING_ENEMY1);
		s_remove_obj(o);

		if(dialogID == DIALOG_108)
		{ // If the Big Boo should spawn, play the jingle
			Na_NazoClearBgm();
		}
	}
}

static void (*sBooActions[])(void) = {ActionBoo0, ActionBoo1, ActionBoo2, ActionBoo3, ActionBoo4, ActionBoo5};

void bhv_boo_loop(void)
{
	// PARTIAL_UPDATE

	obj_update_floor_and_walls();
	s_modejmp(sBooActions);
	obj_move_standard(78);
	boo_approach_target_opacity_and_update_scale();

	if(object_has_behavior(o->parentObj, sm64::bhv::bhvMerryGoRoundBooManager()))
	{
		if(o->activeFlags == 0)
		{
			o->parentObj->oMerryGoRoundBooManagerNumBoosKilled++;
		}
	}

	o->oInteractStatus = 0;
}

static void ActionBooGivingStar0(void)
{
	if(obj_has_behavior(sm64::bhv::bhvBalconyBigBoo()))
	{
		obj_set_secondary_camera_focus();
		// number of killed boos set > 5 so that boo always loads
		// redundant? this is also done in behavior_data.s
		o->oBigBooNumMinionBoosKilled = 10;
	}

	o->oBooParentBigBoo = NULL;

	if(boo_should_be_active() && gDebugInfo[5][0] + 5 <= o->oBigBooNumMinionBoosKilled)
	{
		o->oAction = 1;

		obj_set_pos_to_home();
		o->oMoveAngleYaw = o->oBooInitialMoveYaw;

		s_shape_disp();

		o->oBooTargetOpacity = 0xFF;
		o->oBooBaseScale     = 3.0f;
		o->oHealth	     = 3;

		s_set_scale(3.0f);
		s_hitON();
	}
	else
	{
		s_shape_hide();
		s_hitOFF();
		boo_stop();
	}
}

static void ActionBooGivingStar1(void)
{
	s32 attackStatus;
	s16 sp22;
	f32 sp1C;

	if(o->oHealth == 3)
	{
		sp22 = 0x180;
		sp1C = 0.5f;
	}
	else if(o->oHealth == 2)
	{
		sp22 = 0x240;
		sp1C = 0.6f;
	}
	else
	{
		sp22 = 0x300;
		sp1C = 0.8f;
	}

	boo_chase_mario(-100.0f, sp22, sp1C);

	attackStatus = boo_get_attack_status();

	// redundant; this check is in boo_should_be_stopped
	if(obj_has_behavior(sm64::bhv::bhvMerryGoRoundBigBoo()))
	{
		if(gMarioOnMerryGoRound == FALSE)
		{
			o->oAction = 0;
		}
	}
	else if(boo_should_be_stopped())
	{
		o->oAction = 0;
	}

	if(attackStatus == BOO_BOUNCED_ON)
	{
		o->oAction = 2;
	}

	if(attackStatus == BOO_ATTACKED)
	{
		o->oAction = 3;
	}

	if(attackStatus == 1)
	{
		obj_remove_sound(SOUND_OBJ_THWOMP);
	}
}

static void ActionBooGivingStar2(void)
{
	if(boo_update_after_bounced_on(20.0f))
	{
		o->oAction = 1;
	}
}

static void big_boo_spawn_ghost_hunt_star(void)
{
	s_enemyset_star(980.0f, 1100.0f, 250.0f);
}

static void big_boo_spawn_balcony_star(void)
{
	s_enemyset_star(700.0f, 3200.0f, 1900.0f);
}

static void big_boo_spawn_merry_go_round_star(void)
{
	struct Object* merryGoRound;

	s_enemyset_star(-1600.0f, -2100.0f, 205.0f);

	merryGoRound = s_find_obj(sm64::bhv::bhvMerryGoRound());

	if(merryGoRound != NULL)
	{
		merryGoRound->oMerryGoRoundStopped = TRUE;
	}
}

static void ActionBooGivingStar3(void)
{
	if(o->oTimer == 0)
	{
		o->oHealth--;
	}

	if(o->oHealth == 0)
	{
		if(boo_update_during_death())
		{
			obj_disable();

			o->oAction = 4;

			set_object_angle(o, 0, 0, 0);

			if(o->oBehParams2ndByte == 0)
			{
				big_boo_spawn_ghost_hunt_star();
			}
			else if(o->oBehParams2ndByte == 1)
			{
				big_boo_spawn_merry_go_round_star();
			}
			else
			{
				big_boo_spawn_balcony_star();
			}
		}
	}
	else
	{
		if(o->oTimer == 0)
		{
			s_kemuri();
			o->oBooBaseScale -= 0.5;
		}

		if(big_boo_update_during_nonlethal_hit(40.0f))
		{
			o->oAction = 1;
		}
	}
}

static void ActionBooGivingStar4(void)
{
	boo_stop();

	if(o->oBehParams2ndByte == 0)
	{
		set_object_pos(o, 973, 0, 626);

		if(o->oTimer > 60 * FRAME_RATE_SCALER_INV && o->oDistanceToMario < 600.0f)
		{
			set_object_pos(o, 973, 0, 717);

			s_makeobj_chain(0, 0, 0, 0, o, MODEL_BBH_STAIRCASE_STEP, sm64::bhv::bhvBooBossSpawnedBridge());
			s_makeobj_chain(1, 0, 0, -200, o, MODEL_BBH_STAIRCASE_STEP, sm64::bhv::bhvBooBossSpawnedBridge());
			s_makeobj_chain(2, 0, 0, 200, o, MODEL_BBH_STAIRCASE_STEP, sm64::bhv::bhvBooBossSpawnedBridge());

			s_remove_obj(o);
		}
	}
	else
	{
		s_remove_obj(o);
	}
}

static void (*sBooGivingStarActions[])(void) = {ActionBooGivingStar0, ActionBooGivingStar1, ActionBooGivingStar2, ActionBooGivingStar3, ActionBooGivingStar4};

void bhv_big_boo_loop(void)
{
	// PARTIAL_UPDATE

	s_set_hitparam(o, &sBooGivingStarHitbox);

	o->oGraphYOffset = o->oBooBaseScale * 60.0f;

	obj_update_floor_and_walls();
	s_modejmp(sBooGivingStarActions);
	obj_move_standard(78);

	boo_approach_target_opacity_and_update_scale();
	o->oInteractStatus = 0;
}

static void ActionBooWithCage0(void)
{
	o->oBooParentBigBoo  = NULL;
	o->oBooTargetOpacity = 0xFF;
	o->oBooBaseScale     = 2.0f;

	s_set_scale(2.0f);
	s_hitON();

	if(boo_should_be_active())
	{
		o->oAction = 1;
	}
}

static void ActionBooWithCage1(void)
{
	s32 attackStatus;

	boo_chase_mario(100.0f, 512, 0.5f);

	attackStatus = boo_get_attack_status();

	if(boo_should_be_stopped())
	{
		o->oAction = 0;
	}

	if(attackStatus == BOO_BOUNCED_ON)
	{
		o->oAction = 2;
	}

	if(attackStatus == BOO_ATTACKED)
	{
		o->oAction = 3;
	}
}

static void ActionBooWithCage2(void)
{
	if(boo_update_after_bounced_on(20.0f))
	{
		o->oAction = 1;
	}
}

static void ActionBooWithCage3(void)
{
	if(boo_update_during_death())
	{
		s_remove_obj(o);
	}
}

void bhv_boo_with_cage_init(void)
{
	struct Object* cage;

	if(playerMeter.stars < 12)
	{
		s_remove_obj(o);
	}
	else
	{
		cage		 = s_makeobj_nowpos(o, MODEL_HAUNTED_CAGE, sm64::bhv::bhvBooCage());
		cage->oBehParams = o->oBehParams;
	}
}

static void (*sBooWithCageActions[])(void) = {ActionBooWithCage0, ActionBooWithCage1, ActionBooWithCage2, ActionBooWithCage3};

void bhv_boo_with_cage_loop(void)
{
	// PARTIAL_UPDATE

	obj_update_floor_and_walls();
	s_modejmp(sBooWithCageActions);
	obj_move_standard(78);

	boo_approach_target_opacity_and_update_scale();
	o->oInteractStatus = 0;
}

void bhv_merry_go_round_boo_manager_loop(void)
{
	switch(o->oAction)
	{
		case 0:
			if(o->oDistanceToMario < 1000.0f)
			{
				if(o->oMerryGoRoundBooManagerNumBoosKilled < 5)
				{
					if(o->oMerryGoRoundBooManagerNumBoosSpawned != 5)
					{
						if(o->oMerryGoRoundBooManagerNumBoosSpawned - o->oMerryGoRoundBooManagerNumBoosKilled < 2)
						{
							s_makeobj_nowpos(o, MODEL_BOO, sm64::bhv::bhvMerryGoRoundBoo());
							o->oMerryGoRoundBooManagerNumBoosSpawned++;
						}
					}

					o->oAction++;
				}

				if(o->oMerryGoRoundBooManagerNumBoosKilled > 4)
				{
					struct Object* boo = s_makeobj_nowpos(o, MODEL_BOO, sm64::bhv::bhvMerryGoRoundBigBoo());
					copy_object_behavior_params(boo, o);

					o->oAction = 2;

					Na_NazoClearBgm();
				}
			}

			break;
		case 1:
			if(o->oTimer > 60 * FRAME_RATE_SCALER_INV)
			{
				o->oAction = 0;
			}

			break;
		case 2:
			break;
	}
}

void obj_set_secondary_camera_focus(void)
{
	gSecondCameraFocus = o;
}

void bhv_animated_texture_loop(void)
{
	func_802A3470();
}

void bhv_boo_in_castle_loop(void)
{
	s16 targetAngle;

	o->oBooBaseScale = 2.0f;

	if(o->oAction == 0)
	{
		s_shape_hide();

		if(playerMeter.stars < 12)
		{
			s_remove_obj(o);
		}

		if(gMarioCurrentRoom == 1)
		{
			o->oAction++;
		}
	}
	else if(o->oAction == 1)
	{
		s_shape_disp();

		o->oOpacity = 180;

		if(o->oTimer == 0)
		{
			s_set_scale(o->oBooBaseScale);
		}

		if(o->oDistanceToMario < 1000.0f)
		{
			o->oAction++;
			objsound(SOUND_OBJ_BOO_LAUGH_LONG);
		}

		o->oForwardVel = 0.0f;
		targetAngle    = o->oAngleToMario;
	}
	else
	{
		obj_forward_vel_approach_upward(32.0f, 1.0f * FRAME_RATE_SCALER);

		o->oHomeX = -1000.0f;
		o->oHomeZ = -9000.0f;

		targetAngle = obj_angle_to_home();

		if(o->oPosZ < -5000.0f)
		{
			if(o->oOpacity > 0)
			{
				o->oOpacity -= 20 / FRAME_RATE_SCALER_INV;
			}
			else
			{
				o->oOpacity = 0;
			}
		}

		if(o->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM)
		{
			o->oAction = 1;
		}
	}

	o->oVelY = 0.0f;

	targetAngle = obj_angle_to_home();

	obj_rotate_yaw_toward(targetAngle, 0x5A8 / FRAME_RATE_SCALER_INV);
	boo_oscillate(TRUE);
	s_optionmove_F();
}

void bhv_boo_boss_spawned_bridge_loop(void)
{
	f32 targetY;

	switch(o->oBehParams2ndByte)
	{
		case 1:
			targetY = 0.0f;
			break;
		case 0:
			targetY = -206.0f;
			break;
		case 2:
			targetY = -413.0f;
			break;
	}

	switch(o->oAction)
	{
		case 0:
			o->oPosY = o->oHomeY - 620.0f;
			o->oAction++;
			// fallthrough
		case 1:
			o->oPosY += 8.0f * FRAME_RATE_SCALER;
			objsound_level(SOUND_ENV_ELEVATOR2);

			if(o->oPosY > targetY)
			{
				o->oPosY = targetY;
				o->oAction++;
			}

			break;
		case 2:
			if(o->oTimer == 0)
			{
				objsound(SOUND_GENERAL_UNKNOWN4_LOWPRIO);
			}

			if(func_802A362C(o->oTimer))
			{
				o->oAction++;
			}

			break;
		case 3:
			if(o->oTimer == 0 && o->oBehParams2ndByte == 1)
			{
				Na_NazoClearBgm();
			}

			break;
	}
}
